#ifndef GAMESCRIPTIMAGEPROCESSOR_H
#define GAMESCRIPTIMAGEPROCESSOR_H

#include <QImage>
#include <QString>
#include "imgcodecs.hpp"
#include "highgui.hpp"
#include "imgproc.hpp"
#include "core.hpp"
#include "features2d.hpp"
//#include "xfeatures2d.hpp"

struct ObjectImageFeature
{
    //feature points
    std::vector<cv::KeyPoint> keyPoints;
    //feature descriptor
    cv::Mat descriptor;
    //object image opencv interp
    cv::Mat objectImageCV;
};

enum class ImageMatchingResult
{
    //match processing go through
    IMR_PROCEED_FULLY_MATCHED = 0,
    IMR_PROCEED_NOT_MATCHED,
    //returned in advance
    IMR_FAILED_PROCEED
};

enum DetectAlgorithm
{
	DA_SURF = 0,
	DA_SIFT,
	//DA_ORB,
	//DA_BRISK,
	DA_KAZE,
	//DA_AKAZE,
	//DA_FREAK,
	//DA_DAISY,
	//DA_BRIEF,
	//DA_ADVANCED
};

enum MatchAlgorithm
{
	MA_BRUTE1 = 0,//knn
    MA_BRUTE2,//simple match
    MA_FLANNBASED1,//with knn
    MA_FALNNBASED2//with simple match
};

//deal with object image feature refining, each GameScriptCommandPackInstance should have one this processor
class GameScriptObjectImagePreProcessor
{
public:
    static bool decodeFeatures(const DetectAlgorithm detectType, const QImage&, std::vector<cv::KeyPoint>&, cv::Mat&, cv::Mat&);
};

class GameScriptImageProcessor
{
public:

    GameScriptImageProcessor();
    ~GameScriptImageProcessor();
    ImageMatchingResult ImageTemplateMatching(const QImage& backGround, const cv::Mat& subImage, QRect& recognizeRect, bool isPyrDown = false);
    ImageMatchingResult ImageTemplateMatching(const QImage& backGround, const QImage& subImage, QRect& recognizeRect, bool isPyrDown = false);
    ImageMatchingResult ImageTemplateMatching(const QImage& backGround, const QImage& subImage, QRect& recognizeRect, const QString& previewImageName);


    ImageMatchingResult ImageFeatureMatching(const QImage& backGround, const QImage& subImage, QRect& recognizeRect);// , QVariantList& debugPointList);
    ImageMatchingResult ImageFeatureMatching(const float kNNRatioThreshold, const DetectAlgorithm detectType, const MatchAlgorithm matchType,  const QImage& backGround, const ObjectImageFeature& subImage, QRect& recognizeRect);// , QVariantList& debugPointList);
    //Input,Input,Output
    //bool findAt(const QImage& sceneImage, const GameScriptObjectImagePreProcessor* subImagePPtr, QRect& recognizeRect, QString& logOutput);

private:

    //cv::Ptr<cv::xfeatures2d::SURF> m_pDetector = nullptr;
    //cv::Ptr<cv::DescriptorMatcher> m_pMatcher = nullptr;
    std::vector<cv::KeyPoint> m_rKeyPoints1;
    std::vector<cv::KeyPoint> m_rKeyPoints2;
    std::vector<std::vector<cv::DMatch>> m_rKNNMatches;
    std::vector<std::vector<cv::DMatch>> m_rKNNMatchesOptional;
    std::vector<cv::DMatch> m_rGoodMatches;
    std::vector<char> m_rMatchMask;

    std::vector<cv::Point2f> m_rPointsOnScene;//main Image
    std::vector<cv::Point2f> m_rPointsOnObject;//sub Image

    std::vector<cv::Point2f> m_rCornersOnScene;//main Image
    std::vector<cv::Point2f> m_rCornersOnObject;//sub Image
};


#endif // GAMESCRIPTIMAGEPROCESSOR_H
