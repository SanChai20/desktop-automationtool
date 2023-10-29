#include "gamescriptimageprocessor.h"
#include <QDebug>
#include <map>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
//#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/ocl.hpp>

GameScriptImageProcessor::GameScriptImageProcessor()
{

}

GameScriptImageProcessor::~GameScriptImageProcessor()
{
    m_rKeyPoints1.clear();
    m_rKNNMatches.clear();
    m_rKNNMatchesOptional.clear();
    m_rGoodMatches.clear();
    m_rMatchMask.clear();
    m_rPointsOnScene.clear();
    m_rPointsOnObject.clear();
    m_rCornersOnScene.clear();
    m_rCornersOnObject.clear();
}

void QImage2cvMat(const QImage &image, cv::Mat& output)
{
    QImage::Format format = image.format();
    switch(format)
    {
    case QImage::Format_Grayscale8: // 灰度图，每个像素点1个字节（8位）
        // Mat构造：行数，列数，存储结构，数据，step每行多少字节
        output = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.bits(), image.bytesPerLine()).clone();
        break;
    case QImage::Format_ARGB32: // uint32存储0xAARRGGBB，pc一般小端存储低位在前，所以字节顺序就成了BGRA
    case QImage::Format_RGB32: // Alpha为FF
    case QImage::Format_ARGB32_Premultiplied:
        output = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.bits(), image.bytesPerLine()).clone();
        break;
    case QImage::Format_RGB888: // RR,GG,BB字节顺序存储
        output = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine()).clone();
        // opencv需要转为BGR的字节顺序
        cv::cvtColor(output, output, cv::COLOR_RGB2BGR);
        break;
    case QImage::Format_RGBA64: // uint64存储，顺序和Format_ARGB32相反，RGBA
        output = cv::Mat(image.height(), image.width(), CV_16UC4, (void*)image.bits(), image.bytesPerLine()).clone();
        // opencv需要转为BGRA的字节顺序
        cv::cvtColor(output, output, cv::COLOR_RGBA2BGRA);
        break;
    default:
        break;
    }
}

void cvMat2QImage(const cv::Mat &mat, QImage& output)
{
    switch(mat.type())
    {
    case CV_8UC1:
        // QImage构造：数据，宽度，高度，每行多少字节，存储结构
        output = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, (int)mat.step, QImage::Format_Grayscale8);
        break;
    case CV_8UC3:
        output = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, (int)mat.step, QImage::Format_RGB888);
        output = output.rgbSwapped(); // BRG转为RGB
        // Qt5.14增加了Format_BGR888
        // image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.cols * 3, QImage::Format_BGR888);
        break;
    case CV_8UC4:
        output = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, (int)mat.step, QImage::Format_ARGB32);
        break;
    case CV_16UC4:
        output = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, (int)mat.step, QImage::Format_RGBA64);
        output = output.rgbSwapped(); // BRG转为RGB
        break;
    default:
        break;
    }
}

ImageMatchingResult GameScriptImageProcessor::ImageTemplateMatching(const QImage& backGround, const cv::Mat& subImageCV, QRect& recognizeRect, bool isPyrDown)
{
    cv::Mat backGroundCV, resultCV;
    QImage2cvMat(backGround, backGroundCV);

    if(subImageCV.empty() || backGroundCV.empty())
    {
        return ImageMatchingResult::IMR_FAILED_PROCEED;
    }

    if(isPyrDown)
    {
        pyrDown( backGroundCV, backGroundCV, cv::Size( backGroundCV.cols/2, backGroundCV.rows/2 ));
        pyrDown( subImageCV, subImageCV, cv::Size( subImageCV.cols/2, subImageCV.rows/2 ));
    }

    int result_cols = backGroundCV.cols - subImageCV.cols + 1;
    int result_rows = backGroundCV.rows - subImageCV.rows + 1;

    if(result_rows <= 0 || result_cols <= 0)
    {
        return ImageMatchingResult::IMR_PROCEED_NOT_MATCHED;
    }

    ImageMatchingResult imrResult;

    resultCV.create( result_rows, result_cols, CV_32FC1 );
    //resultCV = cv::Mat::zeros(result_rows, result_cols, CV_8UC4);
    matchTemplate(backGroundCV, subImageCV, resultCV, cv::TM_CCORR_NORMED);
    double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
    cv::Point matchLoc;
    minMaxLoc(resultCV, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

    if(maxVal >= 0.999f)
    {
        imrResult = ImageMatchingResult::IMR_PROCEED_FULLY_MATCHED;
    }
    else
    {
        imrResult = ImageMatchingResult::IMR_PROCEED_NOT_MATCHED;
    }

    matchLoc = maxLoc;

    if(isPyrDown)
    {
        recognizeRect.setX(matchLoc.x * 2);
        recognizeRect.setY(matchLoc.y * 2);
        recognizeRect.setWidth(subImageCV.cols * 2);
        recognizeRect.setHeight(subImageCV.rows * 2);
    }
    else
    {
        recognizeRect.setX(matchLoc.x);
        recognizeRect.setY(matchLoc.y);
        recognizeRect.setWidth(subImageCV.cols);
        recognizeRect.setHeight(subImageCV.rows);
    }

    return imrResult;
}

ImageMatchingResult GameScriptImageProcessor::ImageTemplateMatching(const QImage& backGround, const QImage& subImage, QRect& recognizeRect, bool isPyrDown)
{
    cv::Mat backGroundCV, subImageCV, resultCV;
    QImage2cvMat(backGround, backGroundCV);
    QImage2cvMat(subImage, subImageCV);

    if(subImageCV.empty() || backGroundCV.empty())
    {
        return ImageMatchingResult::IMR_FAILED_PROCEED;
    }

    if(isPyrDown)
    {
        pyrDown( backGroundCV, backGroundCV, cv::Size( backGroundCV.cols/2, backGroundCV.rows/2 ));
        pyrDown( subImageCV, subImageCV, cv::Size( subImageCV.cols/2, subImageCV.rows/2 ));
    }

    int result_cols = backGroundCV.cols - subImageCV.cols + 1;
    int result_rows = backGroundCV.rows - subImageCV.rows + 1;

    if(result_rows <= 0 || result_cols <= 0)
    {
        return ImageMatchingResult::IMR_FAILED_PROCEED;
    }

    ImageMatchingResult imrResult;

    resultCV = cv::Mat::zeros(result_rows, result_cols, CV_8UC4);
    matchTemplate(backGroundCV, subImageCV, resultCV, cv::TM_CCORR_NORMED);
    double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
    cv::Point matchLoc;
    minMaxLoc(resultCV, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

    if(maxVal >= 0.999f)
    {
        imrResult = ImageMatchingResult::IMR_PROCEED_FULLY_MATCHED;
    }
    else
    {
        imrResult = ImageMatchingResult::IMR_PROCEED_NOT_MATCHED;
    }

    matchLoc = maxLoc;

    if(isPyrDown)
    {
        recognizeRect.setX(matchLoc.x * 2);
        recognizeRect.setY(matchLoc.y * 2);
        recognizeRect.setWidth(subImageCV.cols * 2);
        recognizeRect.setHeight(subImageCV.rows * 2);
    }
    else
    {
        recognizeRect.setX(matchLoc.x);
        recognizeRect.setY(matchLoc.y);
        recognizeRect.setWidth(subImageCV.cols);
        recognizeRect.setHeight(subImageCV.rows);
    }

    return imrResult;
}


ImageMatchingResult GameScriptImageProcessor::ImageTemplateMatching(const QImage& backGround, const QImage& subImage, QRect& recognizeRect, const QString& previewImageName)
{
    cv::Mat backGroundCV, subImageCV, previewImageCV, resultCV;
    QImage2cvMat(backGround, backGroundCV);
    QImage2cvMat(subImage, subImageCV);

    if(subImageCV.empty() || backGroundCV.empty())
    {
        return ImageMatchingResult::IMR_FAILED_PROCEED;
    }

    backGroundCV.copyTo(previewImageCV);

    int result_cols = backGroundCV.cols - subImageCV.cols + 1;
    int result_rows = backGroundCV.rows - subImageCV.rows + 1;

    if(result_rows <= 0 || result_cols <= 0)
    {
        return ImageMatchingResult::IMR_FAILED_PROCEED;
    }

    ImageMatchingResult imrResult;

    resultCV = cv::Mat::zeros(result_rows, result_cols, CV_8UC4);
    matchTemplate(backGroundCV, subImageCV, resultCV, cv::TM_CCORR_NORMED);
    double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
    cv::Point matchLoc;
    minMaxLoc(resultCV, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

    if(maxVal >= 0.999f)
    {
        imrResult = ImageMatchingResult::IMR_PROCEED_FULLY_MATCHED;
    }
    else
    {
        imrResult = ImageMatchingResult::IMR_PROCEED_NOT_MATCHED;
    }

    matchLoc = maxLoc;

    rectangle( previewImageCV, matchLoc, cv::Point( matchLoc.x + subImageCV.cols , matchLoc.y + subImageCV.rows ), cv::Scalar::all(0), 2, 8, 0 );
    cv::imwrite(previewImageName.toLocal8Bit().data(), previewImageCV);

    recognizeRect.setX(matchLoc.x);
    recognizeRect.setY(matchLoc.y);
    recognizeRect.setWidth(subImageCV.cols);
    recognizeRect.setHeight(subImageCV.rows);
    return imrResult;
}

void detect_and_compute(DetectAlgorithm type, const cv::Mat& img, std::vector<cv::KeyPoint>& kpts, cv::Mat& desc)
{
    switch(type)
    {
        case DA_SURF:
        {
            cv::Ptr<cv::Feature2D> surf = cv::xfeatures2d::SURF::create(400.0);
            surf->detectAndCompute(img, cv::Mat(), kpts, desc);
            return;
        }
        case DA_SIFT:
        {
            cv::Ptr<cv::Feature2D> sift = cv::SIFT::create();
            sift->detectAndCompute(img, cv::Mat(), kpts, desc);
            return;
        }
        //case DA_ORB:
        //{
        //    cv::Ptr<cv::ORB> orb = cv::ORB::create();
        //    orb->detectAndCompute(img, cv::Mat(), kpts, desc);
        //    return;
        //}
        //case DA_BRISK:
        //{
        //    cv::Ptr<cv::BRISK> brisk = cv::BRISK::create();
        //    brisk->detectAndCompute(img, cv::Mat(), kpts, desc);
        //    return;
        //}
        case DA_KAZE:
        {
            cv::Ptr<cv::KAZE> kaze = cv::KAZE::create();
            kaze->detectAndCompute(img, cv::Mat(), kpts, desc);
            return;
        }
		//case DA_AKAZE:
		//{
		//    cv::Ptr<cv::AKAZE> akaze = cv::AKAZE::create();
		//    akaze->detectAndCompute(img, cv::Mat(), kpts, desc);
		//    return;
		//}
		//case DA_FREAK:
		//{
		//    cv::Ptr<cv::xfeatures2d::FREAK> freak = cv::xfeatures2d::FREAK::create();
		//    freak->compute(img, kpts, desc);
		//    return;
		//}
		//case DA_DAISY:
		//{
		//    cv::Ptr<cv::xfeatures2d::DAISY> daisy = cv::xfeatures2d::DAISY::create();
		//    daisy->compute(img, kpts, desc);
		//    return;
		//}
		//case DA_BRIEF:
		//{
		//    cv::Ptr<cv::xfeatures2d::BriefDescriptorExtractor> brief = cv::xfeatures2d::BriefDescriptorExtractor::create(64);
		//    brief->compute(img, kpts, desc);
		//    return;
		//}
		//case DA_ADVANCED:
		//{
		//	cv::Ptr<cv::Feature2D> sift = cv::SIFT::create();
		//	sift->detect(img, kpts);
		//    cv::Ptr<cv::Feature2D> surf = cv::xfeatures2d::SURF::create(400.0);
		//    surf->compute(img, kpts, desc);
		//    return;
		//}
    }
}

static const double kDistanceCoef = 4.0;
static const int kMaxMatchingSize = 50;

//void match(MatchAlgorithm type, const cv::Mat& desc1, const cv::Mat& desc2, std::vector<cv::DMatch>& matches, std::vector< std::vector<cv::DMatch> >& vmatches)
//{
//    matches.clear();
//    if (type == MA_BRUTE)
//    {
//        cv::BFMatcher desc_matcher(cv::NORM_L2, true);
//        desc_matcher.match(desc1, desc2, matches, cv::Mat());
//    }
//    else if (type == MA_KNN)
//    {
//        cv::BFMatcher desc_matcher(cv::NORM_L2, false);
//        desc_matcher.knnMatch(desc1, desc2, vmatches, 1);
//        for (int i = 0; i < static_cast<int>(vmatches.size()); ++i)
//        {
//            if (!vmatches[i].size())
//            {
//                continue;
//            }
//            matches.push_back(vmatches[i][0]);
//        }
//    }
//    std::sort(matches.begin(), matches.end());
//    while (matches.front().distance * kDistanceCoef < matches.back().distance)
//    {
//        matches.pop_back();
//    }
//    while (matches.size() > kMaxMatchingSize)
//    {
//        matches.pop_back();
//    }
//}

ImageMatchingResult GameScriptImageProcessor::ImageFeatureMatching(const float kNNRatioThreshold, const DetectAlgorithm detectType, const MatchAlgorithm matchType, const QImage& backGround, const ObjectImageFeature& subImage, QRect& recognizeRect)
{
    ImageMatchingResult imrResult = ImageMatchingResult::IMR_FAILED_PROCEED;
    cv::Mat sceneCV, resultCV;
	const QImage& grayScene = backGround.convertToFormat(QImage::Format_Grayscale8);
	QImage2cvMat(grayScene, sceneCV);

    if(subImage.objectImageCV.empty() || sceneCV.empty())
    {
        return imrResult;
    }
    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    cv::Mat descriptorsScene;

    m_rKeyPoints1.clear();
    detect_and_compute(detectType, sceneCV, m_rKeyPoints1, descriptorsScene);

    if(m_rKeyPoints1.empty() || descriptorsScene.empty())
    {
        qDebug() << "Background Image feature aquired failed!!!";
        return imrResult;
    }

    if(subImage.keyPoints.empty() || subImage.descriptor.empty())
    {
        qDebug() << "SubRegion Image feature aquired failed!!!";
        return imrResult;
    }
    m_rGoodMatches.clear();

    if (matchType == MA_FLANNBASED1)
    {
        m_rKNNMatches.clear();
        cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
		matcher->knnMatch(subImage.descriptor, descriptorsScene, m_rKNNMatches, 2);
		for (size_t i = 0; i < m_rKNNMatches.size(); i++)
		{
			if (m_rKNNMatches[i][0].distance < kNNRatioThreshold * m_rKNNMatches[i][1].distance)
			{
				m_rGoodMatches.push_back(m_rKNNMatches[i][0]);
			}
		}
    }
    else if (matchType == MA_FALNNBASED2)
    {
		cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
		matcher->match(subImage.descriptor, descriptorsScene, m_rGoodMatches);
    }
    else if (matchType == MA_BRUTE1)
    {
		m_rKNNMatches.clear();
        cv::Ptr<cv::BFMatcher> matcher = cv::BFMatcher::create(cv::NORM_L2, false);
		matcher->knnMatch(subImage.descriptor, descriptorsScene, m_rKNNMatches, 2);
		for (size_t i = 0; i < m_rKNNMatches.size(); i++)
		{
			if (m_rKNNMatches[i][0].distance < kNNRatioThreshold * m_rKNNMatches[i][1].distance)
			{
				m_rGoodMatches.push_back(m_rKNNMatches[i][0]);
			}
		}
    }
    else if (matchType == MA_BRUTE2)
    {
		cv::Ptr<cv::BFMatcher> matcher = cv::BFMatcher::create(cv::NORM_L2, true);
        matcher->match(subImage.descriptor, descriptorsScene, m_rGoodMatches);
    }

    if(m_rGoodMatches.empty())
    {
        qDebug() << "there is no good matches!!!";
        return imrResult;
    }

    m_rPointsOnObject.clear();
    m_rPointsOnScene.clear();

    for( size_t i = 0; i < m_rGoodMatches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        m_rPointsOnObject.push_back( subImage.keyPoints[ m_rGoodMatches[i].queryIdx ].pt );
        m_rPointsOnScene.push_back( m_rKeyPoints1[ m_rGoodMatches[i].trainIdx ].pt );
    }

    if(m_rPointsOnObject.size() <= 3 || m_rPointsOnScene.size() <= 3)
    {
        qDebug() << "there is no key points on image!!!";
        return imrResult;
    }


    cv::Mat H = findHomography( m_rPointsOnObject, m_rPointsOnScene, cv::RANSAC );

    m_rCornersOnScene.clear();
    m_rCornersOnScene.resize(4);

    m_rCornersOnObject.clear();
    m_rCornersOnObject.resize(4);
    //-- Get the corners from the image_1 ( the object to be "detected" )

    m_rCornersOnObject[0] = cv::Point2f(0, 0);
    m_rCornersOnObject[1] = cv::Point2f( (float)subImage.objectImageCV.cols, 0 );
    m_rCornersOnObject[2] = cv::Point2f( (float)subImage.objectImageCV.cols, (float)subImage.objectImageCV.rows );
    m_rCornersOnObject[3] = cv::Point2f( 0, (float)subImage.objectImageCV.rows );

    perspectiveTransform( m_rCornersOnObject, m_rCornersOnScene, H);

    recognizeRect.setX(m_rCornersOnScene[0].x);
    recognizeRect.setY(m_rCornersOnScene[0].y);
    recognizeRect.setWidth(m_rCornersOnScene[2].x - m_rCornersOnScene[0].x);
    recognizeRect.setHeight(m_rCornersOnScene[2].y - m_rCornersOnScene[0].y);

    imrResult = ImageMatchingResult::IMR_PROCEED_FULLY_MATCHED;
    return imrResult;
}

ImageMatchingResult GameScriptImageProcessor::ImageFeatureMatching(const QImage& backGround, const QImage& subImage, QRect& recognizeRect)//, QVariantList& debugPointList)
{
    ImageMatchingResult imrResult = ImageMatchingResult::IMR_FAILED_PROCEED;
    cv::Mat sceneCV, objectCV, resultCV;

    const QImage& grayScene = backGround.convertToFormat(QImage::Format_Grayscale8);
    const QImage& grayObject = subImage.convertToFormat(QImage::Format_Grayscale8);

    QImage2cvMat(grayScene, sceneCV);
    QImage2cvMat(grayObject, objectCV);

    if(objectCV.empty() || sceneCV.empty())
    {
        return imrResult;
    }

    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    cv::Mat descriptorsObject, descriptorsScene;
    m_rKeyPoints1.clear();
    m_rKeyPoints2.clear();
    detect_and_compute(DA_SIFT, objectCV, m_rKeyPoints1, descriptorsObject);
    detect_and_compute(DA_SIFT, sceneCV, m_rKeyPoints2, descriptorsScene);

    if(m_rKeyPoints1.empty() || descriptorsObject.empty())
    {
        qDebug() << "Background Image feature aquired failed!!!";
        return imrResult;
    }


    if(m_rKeyPoints2.empty() || descriptorsScene.empty())
    {
        qDebug() << "SubRegion Image feature aquired failed!!!";
        return imrResult;
    }
    m_rGoodMatches.clear();
    m_rKNNMatches.clear();

    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
    matcher->knnMatch( descriptorsObject, descriptorsScene, m_rKNNMatches, 2 );
    //-- Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.4f;
    for (size_t i = 0; i < m_rKNNMatches.size(); i++)
    {
        if (m_rKNNMatches[i][0].distance < ratio_thresh * m_rKNNMatches[i][1].distance)
        {
            m_rGoodMatches.push_back(m_rKNNMatches[i][0]);
        }
    }

    if(m_rGoodMatches.empty())
    {
        qDebug() << "there is no good matches!!!";
        return imrResult;
    }

    m_rPointsOnObject.clear();
    m_rPointsOnScene.clear();

    for( size_t i = 0; i < m_rGoodMatches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        m_rPointsOnObject.push_back( m_rKeyPoints1[ m_rGoodMatches[i].queryIdx ].pt );
        m_rPointsOnScene.push_back( m_rKeyPoints2[ m_rGoodMatches[i].trainIdx ].pt );
    }

    if(m_rPointsOnObject.size() <= 3 || m_rPointsOnScene.size() <= 3)
    {
        qDebug() << "there is no key points on image!!!";
        return imrResult;
    }


    cv::Mat H = findHomography( m_rPointsOnObject, m_rPointsOnScene, cv::USAC_ACCURATE );

    m_rCornersOnScene.clear();
    m_rCornersOnScene.resize(4);

    m_rCornersOnObject.clear();
    m_rCornersOnObject.resize(4);
    //-- Get the corners from the image_1 ( the object to be "detected" )

    m_rCornersOnObject[0] = cv::Point2f(0, 0);
    m_rCornersOnObject[1] = cv::Point2f( (float)objectCV.cols, 0 );
    m_rCornersOnObject[2] = cv::Point2f( (float)objectCV.cols, (float)objectCV.rows );
    m_rCornersOnObject[3] = cv::Point2f( 0, (float)objectCV.rows );

    perspectiveTransform( m_rCornersOnObject, m_rCornersOnScene, H);

	recognizeRect.setX(m_rCornersOnScene[0].x);
	recognizeRect.setY(m_rCornersOnScene[0].y);
	recognizeRect.setWidth(m_rCornersOnScene[2].x - m_rCornersOnScene[0].x);
	recognizeRect.setHeight(m_rCornersOnScene[2].y - m_rCornersOnScene[0].y);

    imrResult = ImageMatchingResult::IMR_PROCEED_FULLY_MATCHED;
    return imrResult;
}

bool GameScriptObjectImagePreProcessor::decodeFeatures(const DetectAlgorithm detectType,  const QImage& inputImage, std::vector<cv::KeyPoint>& rKeys, cv::Mat& rDescriptors, cv::Mat& rObjectImgCV)
{
    if(inputImage.isNull())
    {
        return false;
    }
    cv::Mat objCV;
    QImage2cvMat(inputImage, rObjectImgCV);
	const QImage& grayImage = inputImage.convertToFormat(QImage::Format_Grayscale8);
	QImage2cvMat(grayImage, objCV);

    if(objCV.empty())
    {
        return false;
    }

    cv::Mat descriptorsObject;
    detect_and_compute(detectType, objCV, rKeys, rDescriptors);

    if(rKeys.empty() || rDescriptors.empty())
    {
        return false;
    }

    return true;
}
