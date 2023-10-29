#ifndef COVERTRANSPARENTWINDOW_H
#define COVERTRANSPARENTWINDOW_H

#include <QObject>
#include <QQuickWindow>

class Covertransparentwindow : public QQuickWindow
{
    Q_OBJECT
public:
    explicit Covertransparentwindow(QQuickWindow *parent = nullptr);

    Q_INVOKABLE QQuickWindow* getWindow();


public slots:

    void updateWindowRectangle(float,float,float,float);

signals:

};

#endif // COVERTRANSPARENTWINDOW_H
