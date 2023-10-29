#ifndef GAMESCRIPTRUNINSTANCE_H
#define GAMESCRIPTRUNINSTANCE_H

#include <QObject>
#include <QRect>
#include <QString>
#include <QImage>
#include <QVector>
#include <QQuickWindow>
#include "gamescriptimageprocessor.h"
#include "gamescriptrunningtask.h"

#if defined(Q_OS_WIN)
#include <Windows.h>
#include <windef.h>
#include <winuser.h>
#include <iostream>
#include <fstream>
#endif

struct SerializedCommand
{
    float fDelayTime;
    float fClickRateX;
    float fClickRateY;
    int nInLoop;
    QString szImagePath;
    QString szBorderImagePath;
    int nClickEvent;
};

class GameScriptRunInstance : public QObject
{
    Q_OBJECT
public:
    explicit GameScriptRunInstance(QObject *parent = nullptr);
    ~GameScriptRunInstance();

    Q_INVOKABLE void updateRectCapture(QQuickWindow* pCoverWindow);
    Q_INVOKABLE void saveImageAndGeneratePreview(int index);

    Q_INVOKABLE void resetTask();
    Q_INVOKABLE void clearImportCache();

    Q_INVOKABLE void appendCommand(float,float,float,bool,int);
    Q_INVOKABLE void appendCommand(const QString&);
    Q_INVOKABLE void appendCommand(float, float, float, bool, const QString&, const QString&, int);

    Q_INVOKABLE void packCommands(const QString&);
    Q_INVOKABLE int unpackCommands(const QString&);

    Q_INVOKABLE void stop();
    Q_INVOKABLE void run();
    //Q_INVOKABLE void pause();

    Q_INVOKABLE float queryClickRateXAt(int index);
    Q_INVOKABLE float queryClickRateYAt(int index);
    Q_INVOKABLE float queryDelayTimeAt(int index);
    Q_INVOKABLE bool queryLoopAt(int index);
    Q_INVOKABLE QString queryImagePathAt(int index);
    Q_INVOKABLE QString queryBorderImagePathAt(int index);
    Q_INVOKABLE int queryClickEventAt(int index);
    //Q_INVOKABLE void setDetectType(int index);
    //Q_INVOKABLE void setMatchType(int index);
    //Q_INVOKABLE void setkNNRatioThreshold(float ratio);

public slots:

    void preprocessingTaskFinishSucceed(const QList<ObjectImageFeature>&);
    void preprocessingTaskFinishFailed(const QList<int>&);
    void showMatchedResults(float x, float y, float width, float height, float clickX, float clickY);

signals:

    void windowRectUpdated(float rectX, float rectY, float rectWidth, float rectHeight);
    void previewImageConfigured(const QString& windowTitle, const QString& imagePath);
    void logUpdated(const QString& log);

    void runningProceed();
    void runningStopped();
    void debugMatch(float x, float y, float width, float height, float clickX, float clickY, float rectX, float rectY, float rectWidth, float rectHeight);
    void runningIndexUpdated(int runningIndex);
    //void unpackingCommand(int commandIndex, float clickRateX, float clickRateY, float delayTime, bool isLoop, const QString& imagePath, const QString& borderImagePath);

private:

#if defined(Q_OS_WIN)
    HWND m_pWindowHandle = nullptr;
#endif

    //used for preprocessing object image feature detected
    GameScriptPreprocessingTask* m_pPreprocessingTask = nullptr;
    //used for dispatching running task
    GameScriptRunningTaskDispatcher* m_pRunningTaskDispatcher= nullptr;

    QList<float> m_rDelayTimeCmdList;
    QList<float> m_rRelativeClickRateXCmdList;
    QList<float> m_rRelativeClickRateYCmdList;
    QList<int> m_rInLoopCmdList;
    QList<int> m_rClickEventList;


    QList<SerializedCommand> m_rSerializedList;
};

#endif // GAMESCRIPTRUNINSTANCE_H
