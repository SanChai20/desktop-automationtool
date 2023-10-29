#include "gamescriptrunningtask.h"
#include "toolheader.h"
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QPixmap>
#include <QImage>
#include "globalsettingsprofile.h"

//Preprocessing LINE-------------------------------------------------------------------------------------
GameScriptPreprocessingTask::GameScriptPreprocessingTask(QObject *parent):QObject(parent)
{

}
GameScriptPreprocessingTask::~GameScriptPreprocessingTask()
{

}

void GameScriptPreprocessingTask::run()
{
    m_rResults.clear();
    if(m_rImagePathes.empty())
    {
        emit printLog(LOG_MAKE_WARNING(QString("The list of target image paths is empty. Please ensure its valid before go next.")));
        emit taskFinishSucceed(m_rResults);
        return;
    }
    m_rFailedIndex.clear();
    for(int index = 0;index < (int)m_rImagePathes.size(); ++index)
    {
        QImage currentImage;
        //qDebug() << m_rImagePathes[index];
        currentImage.load(m_rImagePathes[index]);
        ObjectImageFeature feature;
        emit printLog(LOG_MAKE_COMMON(QString("Current processing ") + m_rImagePathes[index]));
        emit printLog(LOG_MAKE_COMMON(QString("Detect Type is: ") + QString::number(GlobalSettingsSingletonInstance::Instance()->queryDetectType())));
        if (!GameScriptObjectImagePreProcessor::decodeFeatures(GlobalSettingsSingletonInstance::Instance()->queryDetectType(),
                                                               currentImage,
                                                               feature.keyPoints,
                                                               feature.descriptor,
                                                               feature.objectImageCV))
        {
            emit printLog(LOG_PREFIX_ERROR + QString("Proceed failed on image decoding at command number ") + QString::number(index));
            m_rFailedIndex.append(index);
            continue;
        }
        m_rResults.append(feature);
    }
    if(!m_rFailedIndex.empty())
    {
        m_rImagePathes.clear();
        m_rResults.clear();
        emit taskFinishFailed(m_rFailedIndex);
        return;
    }
    emit printLog(LOG_MAKE_COMMON(QString("Proceed success.")));
    emit taskFinishSucceed(m_rResults);
    return;
}

void GameScriptPreprocessingTask::resetTask()
{
    m_rFailedIndex.clear();
    m_rResults.clear();
    m_rImagePathes.clear();
    emit printLog(LOG_MAKE_COMMON(QString("Preprocessing tasks has been reset.")));
}
void GameScriptPreprocessingTask::appendPath(const QString& rPath)
{
    m_rImagePathes.append(rPath);
}

//void GameScriptPreprocessingTask::setDetectType(const DetectAlgorithm rType)
//{
//    m_rDetectType = rType;
//    emit printLog(LOG_MAKE_COMMON(QString("Preprocessing detect algorithm changed.")));
//}

bool GameScriptPreprocessingTask::isValid()
{
    if(m_rImagePathes.empty())
    {
        return false;
    }
    else if(m_rImagePathes.size() == 1 && m_rImagePathes[0] == "")
    {
        return false;
    }
    else
    {
        bool result = false;
        for(int index = 0; index < m_rImagePathes.size();++index)
        {
            if(m_rImagePathes[index] != "")
                result = true;
        }
        return result;
    }
}

//Running LINE--------------------------------------------------------------------------------------------

GameScriptRunningTask::GameScriptRunningTask(QObject *parent):QObject(parent)
{
    m_pImageProcessor = new GameScriptImageProcessor();
}

GameScriptRunningTask::~GameScriptRunningTask()
{
    if(m_pImageProcessor)
        delete m_pImageProcessor;
}

void GameScriptRunningTask::receiveTask(const RunningTaskInfo& rInfo)
{
    m_rTaskInfo = rInfo;
}

//void GameScriptRunningTask::setDetectType(const DetectAlgorithm rType)
//{
//    m_rDetectType = rType;
//    emit printLog(LOG_MAKE_COMMON(QString("Running task detect algorithm changed.")));
//}
//
//void GameScriptRunningTask::setMatchType(const MatchAlgorithm rType)
//{
//    m_rMatchType = rType;
//    emit printLog(LOG_MAKE_COMMON(QString("Running task match algorithm changed.")));
//}
//
//void GameScriptRunningTask::setkNNRatioThreshold(const float fThreshold)
//{
//    m_fRatioThreshold = fThreshold;
//    emit printLog(LOG_MAKE_COMMON(QString("Running task kNN ratio threshold changed.")));
//}

bool GameScriptRunningTask::sendRightClickMsg(float X, float Y)
{
    int lResult = ::SendMessage(m_pWindowHandle, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(X, Y));
    if (lResult == 0)
        emit printLog(LOG_MAKE_COMMON(QString("Send right-click button down event success.")));
    else
    {
        emit printLog(LOG_MAKE_ERROR(QString("Send right-click button down event failed.")));
        return false;
    }
    lResult = ::SendMessage(m_pWindowHandle, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(X, Y));
    if (lResult == 0)
        emit printLog(LOG_MAKE_COMMON(QString("Send right-click button up event success.")));
    else
    {
        emit printLog(LOG_MAKE_ERROR(QString("Send right-click button up event failed.")));
        return false;
    }
    return true;
}

bool GameScriptRunningTask::sendLeftClickMsg(float X, float Y)
{
    int lResult = ::SendMessage(m_pWindowHandle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(X, Y));
    if (lResult == 0)
        emit printLog(LOG_MAKE_COMMON(QString("Send left-click button down event success.")));
    else
    {
        emit printLog(LOG_MAKE_ERROR(QString("Send left-click button down event failed.")));
        return false;
    }
    lResult = ::SendMessage(m_pWindowHandle, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(X, Y));
    if (lResult == 0)
        emit printLog(LOG_MAKE_COMMON(QString("Send left-click button up event success.")));
    else
    {
        emit printLog(LOG_MAKE_ERROR(QString("Send left-click button up event failed.")));
        return false;
    }
    return true;
}

bool GameScriptRunningTask::sendRightDoubleClickMsg(float X, float Y)
{
    int lResult = ::SendMessage(m_pWindowHandle, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(X, Y));
    if (lResult == 0)
        emit printLog(LOG_MAKE_COMMON(QString("Send right-click button down event success.")));
    else
    {
        emit printLog(LOG_MAKE_ERROR(QString("Send right-click button down event failed.")));
        return false;
    }
    lResult = ::SendMessage(m_pWindowHandle, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(X, Y));
    if (lResult == 0)
        emit printLog(LOG_MAKE_COMMON(QString("Send right-click button up event success.")));
    else
    {
        emit printLog(LOG_MAKE_ERROR(QString("Send right-click button up event failed.")));
        return false;
    }


    lResult = ::SendMessage(m_pWindowHandle, WM_RBUTTONDBLCLK, MK_RBUTTON, MAKELPARAM(X, Y));
    if (lResult == 0)
        emit printLog(LOG_MAKE_COMMON(QString("Send right-click button down event success.")));
    else
    {
        emit printLog(LOG_MAKE_ERROR(QString("Send right-click button down event failed.")));
        return false;
    }
    lResult = ::SendMessage(m_pWindowHandle, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(X, Y));
    if (lResult == 0)
        emit printLog(LOG_MAKE_COMMON(QString("Send right-click button up event success.")));
    else
    {
        emit printLog(LOG_MAKE_ERROR(QString("Send right-click button up event failed.")));
        return false;
    }

    return true;
}

bool GameScriptRunningTask::sendLeftDoubleClickMsg(float X, float Y)
{
    int lResult = ::SendMessage(m_pWindowHandle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(X, Y));
    if (lResult == 0)
        emit printLog(LOG_MAKE_COMMON(QString("Send left-click button down event success.")));
    else
    {
        emit printLog(LOG_MAKE_ERROR(QString("Send left-click button down event failed.")));
        return false;
    }
    QThread::msleep(50);
    lResult = ::SendMessage(m_pWindowHandle, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(X, Y));
    if (lResult == 0)
        emit printLog(LOG_MAKE_COMMON(QString("Send left-click button up event success.")));
    else
    {
        emit printLog(LOG_MAKE_ERROR(QString("Send left-click button up event failed.")));
        return false;
    }

    QThread::msleep(100);

    lResult = ::SendMessage(m_pWindowHandle, WM_LBUTTONDBLCLK, MK_LBUTTON, MAKELPARAM(X, Y));
    if (lResult == 0)
        emit printLog(LOG_MAKE_COMMON(QString("Send left-click button down event success.")));
    else
    {
        emit printLog(LOG_MAKE_ERROR(QString("Send left-click button down event failed.")));
        return false;
    }
        QThread::msleep(50);
    lResult = ::SendMessage(m_pWindowHandle, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(X, Y));
    if (lResult == 0)
        emit printLog(LOG_MAKE_COMMON(QString("Send left-click button up event success.")));
    else
    {
        emit printLog(LOG_MAKE_ERROR(QString("Send left-click button up event failed.")));
        return false;
    }
    return true;
}

void GameScriptRunningTask::run()
{
    const PolicyType policyType = GlobalSettingsSingletonInstance::Instance()->queryPolicyType();
    if(m_pWindowHandle == nullptr)
    {
        emit printLog(LOG_MAKE_WARNING(QString("Please ensure window is captured.")));
        if (policyType == STOP_AT_FAILED)
            emit taskFinishedPolicyStop(false);
        else if (policyType == LOOP_AT_FAILED)
            emit taskFinishedPolicyLoop(-1);
        else if (policyType == CONTINUE_AT_FAILED)
            emit taskFinishedPolicyContinue();

        return;
    }
    //first deal with template checking
    QScreen *screen = QGuiApplication::primaryScreen();
    const QImage& sceneImage = screen->grabWindow((WId)m_pWindowHandle).toImage();

    QRect matchResult;
    ImageMatchingResult templateMatchResult = (ImageMatchingResult)m_pImageProcessor->ImageTemplateMatching(sceneImage,
                                                                                                            m_rTaskInfo.feature.objectImageCV,
                                                                                                            matchResult);
    if(templateMatchResult == ImageMatchingResult::IMR_PROCEED_FULLY_MATCHED)
    {
        emit printLog(LOG_MAKE_COMMON(QString("Template matching succeed.")));
        float clickX = matchResult.x() + matchResult.width() * m_rTaskInfo.relativeClickRateX;
        float clickY = matchResult.y() + matchResult.height() * m_rTaskInfo.relativeClickRateY;
        emit gotMatchResult(matchResult.x(), matchResult.y(), matchResult.width(), matchResult.height(), matchResult.width() * m_rTaskInfo.relativeClickRateX, matchResult.height() * m_rTaskInfo.relativeClickRateY);
        bool isClickEventSuccess = false;
        if(m_rTaskInfo.clickEvent == 0)//left single
        {
            isClickEventSuccess = sendLeftClickMsg(clickX, clickY);
            emit printLog(LOG_MAKE_COMMON(QString("Mouse Left-Button Single Click...")));
        }
        else if(m_rTaskInfo.clickEvent == 1)//left double
        {
            isClickEventSuccess = sendLeftDoubleClickMsg(clickX, clickY);
            emit printLog(LOG_MAKE_COMMON(QString("Mouse Left-Button Double Click...")));
        }
        else if(m_rTaskInfo.clickEvent == 2)//right single
        {
            isClickEventSuccess = sendRightClickMsg(clickX, clickY);
            emit printLog(LOG_MAKE_COMMON(QString("Mouse Right-Button Single Click...")));
        }
        else if(m_rTaskInfo.clickEvent == 3)//right double
        {
            isClickEventSuccess = sendRightDoubleClickMsg(clickX, clickY);
            emit printLog(LOG_MAKE_COMMON(QString("Mouse Right-Button Double Click...")));
        }

        if(isClickEventSuccess)
        {
            //wait for delayTime
            emit printLog(LOG_MAKE_COMMON(QString("Enter delay time area...")));
            QThread::sleep(m_rTaskInfo.delayTime + 1);
            emit printLog(LOG_MAKE_COMMON(QString("Exit delay time area...")));
            if (policyType == STOP_AT_FAILED)
                emit taskFinishedPolicyStop(true);
            else if (policyType == LOOP_AT_FAILED)
                emit taskFinishedPolicyLoop(1);
            else if (policyType == CONTINUE_AT_FAILED)
                emit taskFinishedPolicyContinue();

            return;
        }
        else
        {
            emit printLog(LOG_MAKE_ERROR(QString("Sending click command failed.")));
            if (policyType == STOP_AT_FAILED)
                emit taskFinishedPolicyStop(false);
            else if (policyType == LOOP_AT_FAILED)
                emit taskFinishedPolicyLoop(-1);
            else if (policyType == CONTINUE_AT_FAILED)
                emit taskFinishedPolicyContinue();

            return;
        }
    }
    else if(templateMatchResult == ImageMatchingResult::IMR_PROCEED_NOT_MATCHED)
    {
        //try feature detecting
        //QVariantList debugPointList;  
        emit printLog(LOG_MAKE_COMMON(QString("Detect Type is: ") + QString::number(GlobalSettingsSingletonInstance::Instance()->queryDetectType())));
        emit printLog(LOG_MAKE_COMMON(QString("Match Type is: ") + QString::number(GlobalSettingsSingletonInstance::Instance()->queryMatchType())));
        emit printLog(LOG_MAKE_COMMON(QString("kNN Ratio is: ") + QString::number(GlobalSettingsSingletonInstance::Instance()->querykNNRatio())));

        ImageMatchingResult featureMatchResult = m_pImageProcessor->ImageFeatureMatching(GlobalSettingsSingletonInstance::Instance()->querykNNRatio(),
                                                                                         GlobalSettingsSingletonInstance::Instance()->queryDetectType(),
                                                                                         GlobalSettingsSingletonInstance::Instance()->queryMatchType(),
                                                                                         sceneImage,
                                                                                         m_rTaskInfo.feature,
                                                                                         matchResult);// , debugPointList);

        if(featureMatchResult == ImageMatchingResult::IMR_PROCEED_FULLY_MATCHED)
        {
            emit printLog(LOG_MAKE_COMMON(QString("Feature matching succeed.")));
            float clickX = matchResult.x() + m_rTaskInfo.relativeClickRateX * matchResult.width();
            float clickY = matchResult.y() + m_rTaskInfo.relativeClickRateY * matchResult.height();
            emit gotMatchResult(matchResult.x(), matchResult.y(), matchResult.width(), matchResult.height(), m_rTaskInfo.relativeClickRateX * matchResult.width(), m_rTaskInfo.relativeClickRateY * matchResult.height());

            bool isClickEventSuccess = false;
            if(m_rTaskInfo.clickEvent == 0)//left single
            {
                isClickEventSuccess = sendLeftClickMsg(clickX, clickY);
                emit printLog(LOG_MAKE_COMMON(QString("Mouse Left-Button Single Click...")));
            }
            else if(m_rTaskInfo.clickEvent == 1)//left double
            {
                isClickEventSuccess = sendLeftDoubleClickMsg(clickX, clickY);
                emit printLog(LOG_MAKE_COMMON(QString("Mouse Left-Button Double Click...")));
            }
            else if(m_rTaskInfo.clickEvent == 2)//right single
            {
                isClickEventSuccess = sendRightClickMsg(clickX, clickY);
                emit printLog(LOG_MAKE_COMMON(QString("Mouse Right-Button Single Click...")));
            }
            else if(m_rTaskInfo.clickEvent == 3)//right double
            {
                isClickEventSuccess = sendRightDoubleClickMsg(clickX, clickY);
                emit printLog(LOG_MAKE_COMMON(QString("Mouse Right-Button Double Click...")));
            }

            if(isClickEventSuccess)
            {
                //wait for delayTime
                emit printLog(LOG_MAKE_COMMON(QString("Enter delay time area...")));
                QThread::sleep(m_rTaskInfo.delayTime + 1);
                emit printLog(LOG_MAKE_COMMON(QString("Exit delay time area...")));
                if (policyType == STOP_AT_FAILED)
                    emit taskFinishedPolicyStop(true);
                else if (policyType == LOOP_AT_FAILED)
                    emit taskFinishedPolicyLoop(1);
                else if (policyType == CONTINUE_AT_FAILED)
                    emit taskFinishedPolicyContinue();

                return;
            }
            else
            {
                emit printLog(LOG_MAKE_ERROR(QString("Sending click command failed.")));
                if (policyType == STOP_AT_FAILED)
                    emit taskFinishedPolicyStop(false);
                else if (policyType == LOOP_AT_FAILED)
                    emit taskFinishedPolicyLoop(-1);
                else if (policyType == CONTINUE_AT_FAILED)
                    emit taskFinishedPolicyContinue();
                return;
            }
        }
        else
        {
            emit printLog(LOG_MAKE_ERROR(QString("Image Matching failed: not found at current screen window or the captured window has been minimized.")));
            //failed .. think other stratgy
            if (policyType == STOP_AT_FAILED)
                emit taskFinishedPolicyStop(false);
            else if (policyType == LOOP_AT_FAILED)
                emit taskFinishedPolicyLoop(0);
            else if (policyType == CONTINUE_AT_FAILED)
                emit taskFinishedPolicyContinue();
            return;
        }
    }
    else//failed
    {
        emit printLog(LOG_MAKE_ERROR(QString("Image Matching failed: please check target image format or use bigger clearer image instead.")));
        //simply failed
        if (policyType == STOP_AT_FAILED)
            emit taskFinishedPolicyStop(false);
        else if (policyType == LOOP_AT_FAILED)
            emit taskFinishedPolicyLoop(-1);
        else if (policyType == CONTINUE_AT_FAILED)
            emit taskFinishedPolicyContinue();

        return;
    }
}


#if defined(Q_OS_WIN)

void GameScriptRunningTask::receiveHandle(HWND pHandle)
{
    m_pWindowHandle = pHandle;
}

//Dispatcher LINE-------------------------------------------------------------------------------------

void GameScriptRunningTaskDispatcher::receiveHandle(HWND pHandle)
{
    if(m_pRunningTask)
        m_pRunningTask->receiveHandle(pHandle);
}

#endif

GameScriptRunningTaskDispatcher::GameScriptRunningTaskDispatcher(QObject *parent): QObject(parent)
{
    m_pRunningTask = new GameScriptRunningTask(this);
    m_pRunningTask->setAutoDelete(false);
    connect(m_pRunningTask, &GameScriptRunningTask::taskFinishedPolicyStop, this, &GameScriptRunningTaskDispatcher::taskFinishedPolicyStop);
    connect(m_pRunningTask, &GameScriptRunningTask::taskFinishedPolicyLoop, this, &GameScriptRunningTaskDispatcher::taskFinishedPolicyLoop);
    connect(m_pRunningTask, &GameScriptRunningTask::taskFinishedPolicyContinue, this, &GameScriptRunningTaskDispatcher::taskFinishedPolicyContinue);
    connect(m_pRunningTask, &GameScriptRunningTask::gotMatchResult, this, &GameScriptRunningTaskDispatcher::gotMatchResult);
    connect(m_pRunningTask, &GameScriptRunningTask::printLog, this, &GameScriptRunningTaskDispatcher::printLog);
}

GameScriptRunningTaskDispatcher::~GameScriptRunningTaskDispatcher()
{
    if(m_pRunningTask)
    {
        delete m_pRunningTask;
    }
}

void GameScriptRunningTaskDispatcher::copyLastTask(const RunningTaskInfo& rInfo)
{
    m_rLastRunningTask.clickEvent = rInfo.clickEvent;
    m_rLastRunningTask.delayTime = rInfo.delayTime;
    m_rLastRunningTask.feature = rInfo.feature;
    m_rLastRunningTask.inloop = rInfo.inloop;
    m_rLastRunningTask.relativeClickRateX = rInfo.relativeClickRateX;
    m_rLastRunningTask.relativeClickRateY = rInfo.relativeClickRateY;
}

void GameScriptRunningTaskDispatcher::normallyProceed()
{
    //succeed!
    if (m_rTaskInfos.isEmpty())
    {
        if (m_rTaskLoop.empty())
        {
            m_nLoopCounter = 0;
            emit printLog(LOG_MAKE_COMMON(QString("All commands have been executed and no one left.")));
            emit forceStopped();
            return;
        }
        //enter loop
        int index = m_nLoopCounter % m_rTaskLoop.size();
        emit runningIndexUpdated(m_rRunningLoopCommandIndex[index]);
        m_rLastRunningTask = m_rTaskLoop[index];
        //copyLastTask(m_rTaskLoop[index]);
        m_pRunningTask->receiveTask(m_rLastRunningTask);
        m_nLoopCounter++;
        emit printLog(LOG_MAKE_COMMON(QString("Proceed the next command in loops.")));
        QThreadPool::globalInstance()->start(m_pRunningTask);
    }
    else
    {
        //first running time
        int runningIndex = m_nTotalCount - m_rTaskInfos.count();
        emit runningIndexUpdated(runningIndex);
        m_rLastRunningTask = m_rTaskInfos.dequeue();
        //const RunningTaskInfo& rInfo = m_rTaskInfos.dequeue();
        //copyLastTask(rInfo);
        if (m_rLastRunningTask.inloop)
        {
            m_rRunningLoopCommandIndex.append(runningIndex);
            m_rTaskLoop.append(m_rLastRunningTask);
        }
        m_pRunningTask->receiveTask(m_rLastRunningTask);
        emit printLog(LOG_MAKE_COMMON(QString("Proceed the next command in first circle.")));
        QThreadPool::globalInstance()->start(m_pRunningTask);
    }
}

void GameScriptRunningTaskDispatcher::taskFinishedPolicyStop(bool result)
{
    emit printLog(LOG_MAKE_COMMON(QString("Current task finished and failed-policy is 'stop at fail'.")));
    if(m_bStopDispatching || !result)
    {
        m_bStopDispatching = false;
        //m_bPauseDispatching = false;
        m_rTaskLoop.clear();
        m_rTaskInfos.clear();
        m_rRunningLoopCommandIndex.clear();
        m_nTotalCount = 0;
        m_nLoopCounter = 0;
        emit printLog(LOG_MAKE_WARNING(QString("Force Stopped: stopped in manual or running into errors.")));
        emit forceStopped();
        return;
    }
    //else if(m_bPauseDispatching)
    //{
    //    m_bPauseDispatching = false;
    //    emit forcePaused();
    //    return;
    //}
    else
        normallyProceed();
}

void GameScriptRunningTaskDispatcher::taskFinishedPolicyLoop(int state)
{
    emit printLog(LOG_MAKE_COMMON(QString("Current task finished and failed-policy is 'single loop at fail'.")));
    if (m_bStopDispatching || (state == -1))
    {
        m_bStopDispatching = false;
        //m_bPauseDispatching = false;
        m_rTaskLoop.clear();
        m_rTaskInfos.clear();
        m_rRunningLoopCommandIndex.clear();
        m_nTotalCount = 0;
        m_nLoopCounter = 0;
        emit printLog(LOG_MAKE_WARNING(QString("Force Stopped: stopped in manual or running into errors.")));
        emit forceStopped();
        return;
    }
    else if (state == 1)//success/ normally proceed
        normallyProceed();
    else if (state == 0)//failed and enter single loop
    {
        m_pRunningTask->receiveTask(m_rLastRunningTask);
        emit printLog(LOG_MAKE_COMMON(QString("Proceed the last failed command in loops.")));
        QThreadPool::globalInstance()->start(m_pRunningTask);
    }
}

void GameScriptRunningTaskDispatcher::taskFinishedPolicyContinue()
{
    emit printLog(LOG_MAKE_COMMON(QString("Current task finished and failed-policy is 'continue at fail'.")));
    if (m_bStopDispatching)
    {
        m_bStopDispatching = false;
        //m_bPauseDispatching = false;
        m_rTaskLoop.clear();
        m_rTaskInfos.clear();
        m_rRunningLoopCommandIndex.clear();
        m_nTotalCount = 0;
        m_nLoopCounter = 0;
        emit printLog(LOG_MAKE_WARNING(QString("Force Stopped: stopped in manual or running into errors.")));
        emit forceStopped();
        return;
    }
    else
        normallyProceed();
}

QQueue<RunningTaskInfo>& GameScriptRunningTaskDispatcher::requestTasks()
{
    return m_rTaskInfos;
}

void GameScriptRunningTaskDispatcher::stopDispatching()
{
    m_bStopDispatching = true;
}

//void GameScriptRunningTaskDispatcher::setDetectType(const DetectAlgorithm rType)
//{
//    m_pRunningTask->setDetectType(rType);
//}
//
//void GameScriptRunningTaskDispatcher::setMatchType(const MatchAlgorithm rType)
//{
//    m_pRunningTask->setMatchType(rType);
//}
//
//void GameScriptRunningTaskDispatcher::setkNNRatioThreshold(float fThreshold)
//{
//    m_pRunningTask->setkNNRatioThreshold(fThreshold);
//}

void GameScriptRunningTaskDispatcher::resetTask()
{
    m_bStopDispatching = false;
    //m_bPauseDispatching = false;
    m_nLoopCounter = 0;
    m_nTotalCount = 0;
    m_rTaskLoop.clear();
    m_rTaskInfos.clear();
    m_rRunningLoopCommandIndex.clear();

    emit printLog(LOG_MAKE_COMMON(QString("Command dispatcher tasks has been reset.")));
}

void GameScriptRunningTaskDispatcher::startDispatching()
{
    //make sure list is not empty
    if(m_rTaskInfos.isEmpty())
    {
        emit printLog(LOG_MAKE_WARNING(QString("Force Stopped: please ensure all commands have been configured properly.")));
        emit forceStopped();
        return;
    }

    if(m_pRunningTask == nullptr)
    {
        emit printLog(LOG_MAKE_ERROR(QString("Force Stopped: running task not created correctly.")));
        emit forceStopped();
        return;
    }

    m_bStopDispatching = false;
    //m_bPauseDispatching = false;
    m_nLoopCounter = 0;
    m_rTaskLoop.clear();
    m_rRunningLoopCommandIndex.clear();
    m_nTotalCount = m_rTaskInfos.count();
    m_rLastRunningTask = m_rTaskInfos.dequeue();
    //copyLastTask(rInfo);
    if (m_rLastRunningTask.inloop)
    {
        m_rRunningLoopCommandIndex.append(0);
        m_rTaskLoop.append(m_rLastRunningTask);
    }
    m_pRunningTask->receiveTask(m_rLastRunningTask);
    emit printLog(LOG_MAKE_COMMON(QString("Start executing...")));
    emit runningIndexUpdated(0);
    QThreadPool::globalInstance()->start(m_pRunningTask);
}
