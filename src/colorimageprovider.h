#ifndef COLORIMAGEPROVIDER_H
#define COLORIMAGEPROVIDER_H

#include <QQuickImageProvider>

class ColorImageProvider : public QQuickImageProvider
{
public:
    ColorImageProvider(): QQuickImageProvider(QQuickImageProvider::Pixmap)
    {}

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

};

#endif // COLORIMAGEPROVIDER_H
