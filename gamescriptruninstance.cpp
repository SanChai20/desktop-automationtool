#include "gamescriptruninstance.h"
#include "toolheader.h"
#include <QCursor>
#include <QGuiApplication>
#include <QScreen>
#include <QPixmap>
#include <QThreadPool>
#include "gamescriptruninstancetimeprofiler.h"


GameScriptRunInstance::GameScriptRunInstance(QObject *parent) : QObject(parent)
{
    m_pPreprocessingTask = new GameScriptPreprocessingTask(this);
    m_pPreprocessingTask->setAutoDelete(false);
    m_pRunningTaskDispatcher = new GameScriptRunningTaskDispatcher(this);

    QObject::connect(m_pPreprocessingTask, &GameScriptPreprocessingTask::taskFinishSucceed, this, &GameScriptRunInstance::preprocessingTaskFinishSucceed);
    QObject::connect(m_pPreprocessingTask, &GameScriptPreprocessingTask::taskFinishFailed, this, &GameScriptRunInstance::preprocessingTaskFinishFailed);
    QObject::connect(m_pPreprocessingTask, &GameScriptPreprocessingTask::printLog, this, &GameScriptRunInstance::logUpdated);

    QObject::connect(m_pRunningTaskDispatcher, &GameScriptRunningTaskDispatcher::forceStopped, this, &GameScriptRunInstance::runningStopped);
    QObject::connect(m_pRunningTaskDispatcher, &GameScriptRunningTaskDispatcher::gotMatchResult, this, &GameScriptRunInstance::showMatchedResults);
    QObject::connect(m_pRunningTaskDispatcher, &GameScriptRunningTaskDispatcher::printLog, this, &GameScriptRunInstance::logUpdated);
    QObject::connect(m_pRunningTaskDispatcher, &GameScriptRunningTaskDispatcher::runningIndexUpdated, this, &GameScriptRunInstance::runningIndexUpdated);

    //m_pPreprocessingTask->setDetectType(DA_SIFT);
    //m_pRunningTaskDispatcher->setDetectType(DA_SIFT);
    //m_pRunningTaskDispatcher->setMatchType(MA_BRUTE2);
}

void GameScriptRunInstance::showMatchedResults(float x, float y, float width, float height, float clickX, float clickY)
{
    RECT lrect;
    bool ret = ::GetWindowRect(m_pWindowHandle, &lrect);     //获取窗口位置
    if (!ret)
    {
        emit logUpdated(LOG_MAKE_ERROR(QString("Failed to get window rectangle. Please try another one.")));
        return;
    }

    int rectX = lrect.left;
    int rectY = lrect.top;
    int rectWidth = lrect.right - lrect.left;
    int rectHeight = lrect.bottom - lrect.top;

    if (rectWidth == 0 || rectHeight == 0)
    {
        emit logUpdated(LOG_MAKE_ERROR(QString("The window you captured has invalid width and height.")));
        return;
    }

    emit logUpdated(LOG_MAKE_COMMON(QString("The debug cover window updated.")));
    emit debugMatch(x, y, width, height, clickX, clickY, rectX, rectY, rectWidth, rectHeight);
}

GameScriptRunInstance::~GameScriptRunInstance()
{
    if (m_pPreprocessingTask != nullptr)
        delete m_pPreprocessingTask;

    if (m_pRunningTaskDispatcher != nullptr)
        delete m_pRunningTaskDispatcher;
}

void GameScriptRunInstance::resetTask()
{
    if(m_pPreprocessingTask)
        m_pPreprocessingTask->resetTask();

    if(m_pRunningTaskDispatcher)
        m_pRunningTaskDispatcher->resetTask();

    m_rDelayTimeCmdList.clear();
    m_rRelativeClickRateXCmdList.clear();
    m_rRelativeClickRateYCmdList.clear();
    m_rInLoopCmdList.clear();
    m_rClickEventList.clear();

    emit logUpdated(LOG_MAKE_COMMON(QString("Current running instance has been reset.")));
}
void GameScriptRunInstance::appendCommand(float delayTime, float clickRateX, float clickRateY, bool inLoop, int clickEvent)
{
    m_rDelayTimeCmdList.append(delayTime);
    m_rRelativeClickRateXCmdList.append(clickRateX);
    m_rRelativeClickRateYCmdList.append(clickRateY);
    m_rInLoopCmdList.append(inLoop ? 1 : 0);
    m_rClickEventList.append(clickEvent);
    emit logUpdated(LOG_MAKE_COMMON(QString("One command has been recorded...")));
}

void GameScriptRunInstance::appendCommand(const QString& rPath)
{
    if(m_pPreprocessingTask)
    {
        m_pPreprocessingTask->appendPath(rPath);
    }
}

//QML 选择更新使用

void GameScriptRunInstance::updateRectCapture(QQuickWindow* pCoverWindow)
{
    QPoint point = QCursor::pos();  // 获取鼠标当前位置
    WINDOW_INSTANCE_TIME_PROFILER(updateRectCapture);
    {
#if defined(Q_OS_WIN)
        POINT pos;
        pos.x = point.x();
        pos.y = point.y();

        HWND pTempHandle = ::WindowFromPoint(pos);   // 通过鼠标位置获取窗口句柄
        if (pTempHandle == nullptr)
        {
            emit logUpdated(LOG_MAKE_ERROR(QString("The window under mouse is invalid to capture. Please try another one.")));
            return;
        }


        if(pTempHandle != (HWND)pCoverWindow->winId())
        {
            m_pWindowHandle = pTempHandle;
            RECT lrect;
            bool ret = ::GetWindowRect(m_pWindowHandle, &lrect);     //获取窗口位置
            if (!ret)
            {
                emit logUpdated(LOG_MAKE_ERROR(QString("Failed to get window rectangle. Please try another one.")));
                return;
            }


            int rectX = lrect.left;
            int rectY = lrect.top;
            int rectWidth = lrect.right - lrect.left;
            int rectHeight = lrect.bottom - lrect.top;

            if (rectWidth == 0 || rectHeight == 0)
            {
                emit logUpdated(LOG_MAKE_ERROR(QString("The window you captured has invalid width and height.")));
                return;
            }

            emit windowRectUpdated(rectX, rectY, rectWidth, rectHeight);
        }
#endif
    }
}
#if defined(Q_OS_WIN)
void requestWindowTitle(HWND handle, QString& result)
{
    int length = GetWindowTextLengthA( handle ) + 1;
    char *caption = new char[length];
    ::GetWindowTextA( handle, caption, length );
    result = QString::fromLocal8Bit(caption);
    delete[] caption;
}
#endif

//QML 点击截图使用

void GameScriptRunInstance::saveImageAndGeneratePreview(int index)
{
#if defined(Q_OS_WIN)
    if(m_pWindowHandle == nullptr)
        return;

    WINDOW_INSTANCE_TIME_PROFILER(saveImageAndGeneratePreview);
    {
        QString windowTitle;
        requestWindowTitle(m_pWindowHandle, windowTitle);
        emit logUpdated(LOG_MAKE_COMMON(QString("The window has been captured successfully and the title is ") + windowTitle));

        QScreen *screen = QGuiApplication::primaryScreen();
        QPixmap imageOutput = screen->grabWindow((WId)m_pWindowHandle);
        emit logUpdated(LOG_MAKE_COMMON(QString("The captured window handle is ") + QString::number((qlonglong)m_pWindowHandle)));

        QString imageName = QString::number(index);
        imageName = "CapturePreview" + imageName + ".PNG";
        imageOutput.save(imageName, "PNG");

        emit logUpdated(LOG_MAKE_COMMON(QString("The window preview image saved success.")));
        emit logUpdated(LOG_MAKE_COMMON(QString("The path is ") + imageName));

        emit previewImageConfigured(windowTitle, QString(imageName));
    }
#endif
}

void GameScriptRunInstance::preprocessingTaskFinishSucceed(const QList<ObjectImageFeature>& rFeatures)
{
    if(rFeatures.empty())
    {
        emit runningStopped();
        return;
    }

    if(m_pRunningTaskDispatcher)
    {
#if defined(Q_OS_WIN)
        m_pRunningTaskDispatcher->receiveHandle(m_pWindowHandle);
        if(m_pWindowHandle == nullptr)
        {
            emit logUpdated(LOG_MAKE_WARNING(QString("Please capture window first before running tasks.")));
            emit runningStopped();
            return;
        }
#endif
        QQueue<RunningTaskInfo>& taskQueue = m_pRunningTaskDispatcher->requestTasks();
        for(int index = 0;index < (int)rFeatures.size();++index)
        {
            RunningTaskInfo taskInfo;
            taskInfo.feature = rFeatures[index];
            taskInfo.delayTime = m_rDelayTimeCmdList[index];
            taskInfo.inloop = m_rInLoopCmdList[index] == 1;
            taskInfo.relativeClickRateX = m_rRelativeClickRateXCmdList[index];
            taskInfo.relativeClickRateY = m_rRelativeClickRateYCmdList[index];
            taskInfo.clickEvent = m_rClickEventList[index];
            taskQueue.enqueue(taskInfo);
        }
        emit runningProceed();
        emit logUpdated(LOG_MAKE_COMMON(QString("Target images preprocessing finished successfully and start to do tasks.")));
        m_pRunningTaskDispatcher->startDispatching();
    }
}
void GameScriptRunInstance::preprocessingTaskFinishFailed(const QList<int>& rResults)
{
    //qDebug() << "failed";
    emit logUpdated(LOG_MAKE_ERROR(QString("Target images preprocessing finished but something is wrong.")));
    for(int index =0;index < rResults.size();++index)
    {
        emit logUpdated(LOG_MAKE_WARNING(QString("Please Check command number " + QString::number(rResults[index]))));
        //qDebug() << rResults[index];
    }
    emit runningStopped();
    return;
}

void GameScriptRunInstance::stop()
{
    if(m_pRunningTaskDispatcher)
        m_pRunningTaskDispatcher->stopDispatching();
}
void GameScriptRunInstance::run()
{
    if(m_pPreprocessingTask)
    {
        if(!m_pPreprocessingTask->isValid())
        {
            emit logUpdated(LOG_MAKE_ERROR(QString("The path of target match image in following command list is not valid.")));
            emit runningStopped();
            return;
        }
        QThreadPool::globalInstance()->start(m_pPreprocessingTask);
    }
}

void GameScriptRunInstance::appendCommand(float delayTime, float clickRateX, float clickRateY, bool isLoop, const QString& imagePath, const QString& borderImagePath, int clickEvent)
{
    SerializedCommand command;
    command.fClickRateX = clickRateX;
    command.fClickRateY = clickRateY;
    command.fDelayTime = delayTime;
    command.nInLoop = isLoop ? 1 : 0;
    command.szImagePath = imagePath;
    command.szBorderImagePath = borderImagePath;
    command.nClickEvent = clickEvent;
    m_rSerializedList.append(command);
}

void GameScriptRunInstance::packCommands(const QString& filePath)
{
    if (m_rSerializedList.isEmpty() || filePath.isEmpty())
    {
        m_rSerializedList.clear();
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        m_rSerializedList.clear();
        return;
    }
    QDataStream out(&file);   // we will serialize the data into the file
    out << m_rSerializedList.count();
    for (int index = 0; index < m_rSerializedList.count(); ++index)
    {
        out << m_rSerializedList[index].fClickRateX
            << m_rSerializedList[index].fClickRateY
            << m_rSerializedList[index].fDelayTime
            << m_rSerializedList[index].nInLoop
            << m_rSerializedList[index].szImagePath
            << m_rSerializedList[index].szBorderImagePath
            << m_rSerializedList[index].nClickEvent;

        QFile file(m_rSerializedList[index].szImagePath);
        if (m_rSerializedList[index].szImagePath.isEmpty() || !file.exists())
        {
            emit logUpdated(LOG_MAKE_ERROR(QString("The image path at command number " + QString::number(index) + " is invalid which would cause errors when importing.")));
        }
        
        emit logUpdated(LOG_MAKE_COMMON(QString("Packing command number " + QString::number(index) + "...")));
    }
    m_rSerializedList.clear();
}

int GameScriptRunInstance::unpackCommands(const QString& filePath)
{
    m_rSerializedList.clear();
    if (filePath.isEmpty())
        return 0;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return 0;
    QDataStream in(&file);    // read the data serialized from the file
    int listCount;
    in >> listCount;
    m_rSerializedList.reserve(listCount);
    for (int index = 0; index < listCount; ++index)
    {
        SerializedCommand command;
        in >> command.fClickRateX
            >> command.fClickRateY
            >> command.fDelayTime
            >> command.nInLoop
            >> command.szImagePath
            >> command.szBorderImagePath
            >> command.nClickEvent;

        QFile file(command.szImagePath);
        if (!file.exists())
        {
            emit logUpdated(LOG_MAKE_ERROR(QString("The image file doesn't exist anymore.")));
            emit logUpdated(LOG_MAKE_ERROR(QString("Please check " + command.szImagePath)));
        }
        m_rSerializedList.append(command);
        emit logUpdated(LOG_MAKE_COMMON(QString("Unpacking command number " + QString::number(index) + "...")));
    }
    return listCount;
}


float GameScriptRunInstance::queryClickRateXAt(int index) 
{
    return m_rSerializedList[index].fClickRateX;
}
float GameScriptRunInstance::queryClickRateYAt(int index)
{
    return m_rSerializedList[index].fClickRateY;
}
float GameScriptRunInstance::queryDelayTimeAt(int index)
{
    return m_rSerializedList[index].fDelayTime;
}
bool GameScriptRunInstance::queryLoopAt(int index)
{
    return m_rSerializedList[index].nInLoop == 1;
}
QString GameScriptRunInstance::queryImagePathAt(int index)
{
    return m_rSerializedList[index].szImagePath;
}
QString GameScriptRunInstance::queryBorderImagePathAt(int index)
{
    return m_rSerializedList[index].szBorderImagePath;
}
int GameScriptRunInstance::queryClickEventAt(int index)
{
    return m_rSerializedList[index].nClickEvent;
}

void GameScriptRunInstance::clearImportCache()
{
    m_rSerializedList.clear();
}

//enum MatchAlgorithm
//{
//	MA_BRUTE1 = 0,//knn
//	MA_BRUTE2,//simple match
//	MA_FLANNBASED1,//with knn
//	MA_FALNNBASED2//with simple match
//};
//void GameScriptRunInstance::setDetectType(int index)
//{
//    if (index == 0)
//    {
//		m_pPreprocessingTask->setDetectType(DA_SURF);
//		m_pRunningTaskDispatcher->setDetectType(DA_SURF);
//    }
//    else if (index == 1)
//    {
//		m_pPreprocessingTask->setDetectType(DA_SIFT);
//		m_pRunningTaskDispatcher->setDetectType(DA_SIFT);
//    }
//    //else if (index == 2)
//    //{
//	//	m_pPreprocessingTask->setDetectType(DA_ORB);
//	//	m_pRunningTaskDispatcher->setDetectType(DA_ORB);
//    //}
//	//else if (index == 3)
//	//{
//	//	m_pPreprocessingTask->setDetectType(DA_BRISK);
//	//	m_pRunningTaskDispatcher->setDetectType(DA_BRISK);
//	//}
//	else if (index == 2)
//	{
//		m_pPreprocessingTask->setDetectType(DA_KAZE);
//		m_pRunningTaskDispatcher->setDetectType(DA_KAZE);
//	}
//	//else if (index == 5)
//	//{
//	//	m_pPreprocessingTask->setDetectType(DA_AKAZE);
//	//	m_pRunningTaskDispatcher->setDetectType(DA_AKAZE);
//	//}
//	//else if (index == 6)
//	//{
//	//	m_pPreprocessingTask->setDetectType(DA_FREAK);
//	//	m_pRunningTaskDispatcher->setDetectType(DA_FREAK);
//	//}
//	//else if (index == 7)
//	//{
//	//	m_pPreprocessingTask->setDetectType(DA_DAISY);
//	//	m_pRunningTaskDispatcher->setDetectType(DA_DAISY);
//	//}
//	//else if (index == 8)
//	//{
//	//	m_pPreprocessingTask->setDetectType(DA_BRIEF);
//	//	m_pRunningTaskDispatcher->setDetectType(DA_BRIEF);
//	//}
//	//else if (index == 9)
//	//{
//	//	m_pPreprocessingTask->setDetectType(DA_ADVANCED);
//	//	m_pRunningTaskDispatcher->setDetectType(DA_ADVANCED);
//	//}
//    else
//    {
//		m_pPreprocessingTask->setDetectType(DA_SIFT);
//		m_pRunningTaskDispatcher->setDetectType(DA_SIFT);
//    }
//}
//
//void GameScriptRunInstance::setMatchType(int index)
//{
//    if (index == 0)
//    {
//        m_pRunningTaskDispatcher->setMatchType(MA_BRUTE1);
//    }
//    else if (index == 1)
//    {
//        m_pRunningTaskDispatcher->setMatchType(MA_BRUTE2);
//    }
//	else if (index == 2)
//	{
//		m_pRunningTaskDispatcher->setMatchType(MA_FLANNBASED1);
//	}
//	else if (index == 3)
//	{
//		m_pRunningTaskDispatcher->setMatchType(MA_FALNNBASED2);
//	}
//    else
//    {
//        m_pRunningTaskDispatcher->setMatchType(MA_FLANNBASED1);
//    }
//}
//
//void GameScriptRunInstance::setkNNRatioThreshold(float ratio)
//{
//    m_pRunningTaskDispatcher->setkNNRatioThreshold(ratio);
//    //emit logUpdated(LOG_MAKE_COMMON(QString("kNN ratio threshold updated " + QString::number(ratio) + ".")));
//}
