#ifndef GAMESCRIPTCLIPBOARD_H
#define GAMESCRIPTCLIPBOARD_H

#include <QObject>
#include <QString>

class GameScriptClipBoard : public QObject
{
    Q_OBJECT
public:
    explicit GameScriptClipBoard(QObject *parent = nullptr);

    Q_INVOKABLE void paste(QString);
    Q_INVOKABLE QString paste();

signals:

};

#endif // GAMESCRIPTCLIPBOARD_H
