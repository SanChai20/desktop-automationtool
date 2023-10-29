#include "covertransparentwindow.h"

Covertransparentwindow::Covertransparentwindow(QQuickWindow *parent) : QQuickWindow(parent)
{
    this->setVisible(false);
    this->setOpacity(0.15f);
    this->setFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    //SetWindowDisplayAffinity((HWND)winId(), WDA_EXCLUDEFROMCAPTURE);
}

QQuickWindow* Covertransparentwindow::getWindow()
{
    return this;
}

void Covertransparentwindow::updateWindowRectangle(float x, float y, float width, float height)
{
    this->setVisible(true);
    this->setX(x);
    this->setY(y);
    this->setWidth(width);
    this->setHeight(height);
}
