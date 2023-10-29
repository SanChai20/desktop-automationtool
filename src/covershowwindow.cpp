#include "covershowwindow.h"

CoverShowWindow::CoverShowWindow(QQuickWindow *parent) : QQuickWindow(parent)
{
    this->setVisible(false);
    this->setOpacity(0.5f);
    this->setFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::WindowTransparentForInput);
}
