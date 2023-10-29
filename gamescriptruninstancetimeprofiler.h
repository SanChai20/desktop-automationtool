#ifndef GAMESCRIPTRUNINSTANCETIMEPROFILER_H
#define GAMESCRIPTRUNINSTANCETIMEPROFILER_H

#include <QTime>
#include <QDebug>
#include <QString>

class TimeStatsProfiler
{
public:
    TimeStatsProfiler(){}
    TimeStatsProfiler(const QString&);
    ~TimeStatsProfiler();

private:

    QTime _StartTime;
    QString _ProfilerName;
};

#define WINDOW_INSTANCE_TIME_PROFILER(name) TimeStatsProfiler _profiler##name(#name);


#endif // GAMESCRIPTRUNINSTANCETIMEPROFILER_H
