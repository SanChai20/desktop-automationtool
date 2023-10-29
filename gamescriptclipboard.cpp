#include "gamescriptclipboard.h"
#include <QClipboard>
#include <QMimeData>
#include <QGuiApplication>
GameScriptClipBoard::GameScriptClipBoard(QObject *parent) : QObject(parent)
{

}

void GameScriptClipBoard::paste(QString sourceText)
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(sourceText, QClipboard::Clipboard);
    if (clipboard->supportsSelection()) {
        clipboard->setText(sourceText, QClipboard::Selection);
    }
}

QString GameScriptClipBoard::paste()
{
    const QClipboard *clipboard = QGuiApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData->hasText())
    {
        return mimeData->text();
    }
    return "无效内容";
}
