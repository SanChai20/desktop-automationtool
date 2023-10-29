#ifndef GAMESCRIPTRUNNINGTASK_H
#define GAMESCRIPTRUNNINGTASK_H
#include <QThreadPool>
#include <QObject>
#include <QList>
#include <QVariantList>
#include <QRunnable>
#include <QQueue>
#include "gamescriptimageprocessor.h"

#if defined(Q_OS_WIN)
#include <Windows.h>
#include <windef.h>
#endif

struct RunningTaskInfo
{
    bool inloop = false;
    float delayTime = 0.0f;
    float relativeClickRateX = 0.0f;
    float relativeClickRateY = 0.0f;
    int clickEvent = 0;
    ObjectImageFeature feature;
};

//dispatch threads for object images' features preprocessing
class GameScriptPreprocessingTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit GameScriptPreprocessingTask(QObject *parent = nullptr);
    ~GameScriptPreprocessingTask();
    void run() override;
    void resetTask();
    void appendPath(const QString&);
    bool isValid();
    //void setDetectType(const DetectAlgorithm);

signals:

    void taskFinishSucceed(const QList<ObjectImageFeature>&);
    void taskFinishFailed(const QList<int>&);//return failed image indexes
    void printLog(const QString& log);

private:
    QList<QString> m_rImagePathes;
    QList<ObjectImageFeature> m_rResults;
    QList<int> m_rFailedIndex;
    //DetectAlgorithm m_rDetectType = DA_SIFT;
};

//dispatch threads for single running task
class GameScriptRunningTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit GameScriptRunningTask(QObject *parent = nullptr);
    ~GameScriptRunningTask();
    void run() override;
#if defined(Q_OS_WIN)
    void receiveHandle(HWND pHandle);
#endif
    void receiveTask(const RunningTaskInfo&);

    //void setDetectType(const DetectAlgorithm rType);
    //void setMatchType(const MatchAlgorithm rType);
    //void setkNNRatioThreshold(const float fThreshold);

signals:

    void taskFinishedPolicyStop(bool);//true: succeed, false: failed
    void taskFinishedPolicyLoop(int);//1: normal proceed , 0: loop on same one, -1: exit
    void taskFinishedPolicyContinue();//Dont care its success or failed
    void gotMatchResult(float x, float y, float width, float height, float clickX, float clickY);
    void printLog(const QString& log);

private:

    bool sendRightClickMsg(float, float);
    bool sendLeftClickMsg(float, float);
    bool sendRightDoubleClickMsg(float, float);
    bool sendLeftDoubleClickMsg(float, float);

private:
#if defined(Q_OS_WIN)
    HWND m_pWindowHandle = nullptr;
#endif
    RunningTaskInfo m_rTaskInfo;
    GameScriptImageProcessor* m_pImageProcessor = nullptr;

    //DetectAlgorithm m_rDetectType = DA_SIFT;
    //MatchAlgorithm m_rMatchType = MA_FLANNBASED1;
    //float m_fRatioThreshold = 0.4f;
};

//manage tasks running logic
class GameScriptRunningTaskDispatcher : public QObject
{
    Q_OBJECT

public:
    explicit GameScriptRunningTaskDispatcher(QObject *parent = nullptr);
    ~GameScriptRunningTaskDispatcher();

#if defined(Q_OS_WIN)
    void receiveHandle(HWND pHandle);
#endif
    QQueue<RunningTaskInfo>& requestTasks();


    void resetTask();
    void startDispatching();
    void stopDispatching();

    //void setDetectType(const DetectAlgorithm rType);
    //void setMatchType(const MatchAlgorithm rType);
    //void setkNNRatioThreshold(float fThreshold);
    //void pauseDispatching();
    //void resumeDispatching();

signals:

    void forceStopped();
    void gotMatchResult(float x, float y, float width, float height, float clickX, float clickY);
    void printLog(const QString& log);
    void runningIndexUpdated(int index);
    //void forcePaused();

public slots:

    void taskFinishedPolicyStop(bool);//true: succeed, false: failed
    void taskFinishedPolicyLoop(int);//1: normal proceed , 0: loop on same one, -1: exit
    void taskFinishedPolicyContinue();//Dont care its success or failed

private: 

    void normallyProceed();
    void copyLastTask(const RunningTaskInfo& rInfo);

private:
//single GameScriptRunningTask
    GameScriptRunningTask* m_pRunningTask = nullptr;

    int m_nLoopCounter = 0;
    int m_nTotalCount = 0;
    bool m_bStopDispatching = false;
    //bool m_bPauseDispatching = false;

    //RunningTaskInfo List
    QQueue<RunningTaskInfo> m_rTaskInfos;
    //Running Loop List
    QList<RunningTaskInfo> m_rTaskLoop;

    //Last Running Task Info
    RunningTaskInfo m_rLastRunningTask;

    //index recorder
    QList<int> m_rRunningLoopCommandIndex;
};


#endif // GAMESCRIPTRUNNINGTASK_H
