#ifndef COVERSHOWWINDOW_H
#define COVERSHOWWINDOW_H

#include <QObject>
#include <QQuickWindow>

class CoverShowWindow : public QQuickWindow
{
    Q_OBJECT
public:
    explicit CoverShowWindow(QQuickWindow *parent = nullptr);

signals:

};

#endif // COVERSHOWWINDOW_H
