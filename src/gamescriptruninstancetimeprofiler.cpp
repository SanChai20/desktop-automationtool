#include "gamescriptruninstancetimeprofiler.h"

TimeStatsProfiler::TimeStatsProfiler(const QString& profilerName)
{
    _ProfilerName = profilerName;
    _StartTime = QTime::currentTime();
}

TimeStatsProfiler::~TimeStatsProfiler()
{
    QTime stopTime  = QTime::currentTime();
    qint64 temp = _StartTime.msecsTo(stopTime);
    QString logOutput = _ProfilerName + QString(" Time Interval is: ") + QString::number(temp / 1000.0f ,'f', 3) + QString("s");
    qDebug() << logOutput;
}
