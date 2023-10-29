#include "colorimageprovider.h"
#include <QDebug>
QPixmap ColorImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (size)
        qDebug() << size->width() << "," << size->height() << "," << requestedSize.width() << requestedSize.height();

    QPixmap map;
    map.load(id);
    return map;
}
