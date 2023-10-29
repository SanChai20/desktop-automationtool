#ifndef GAMEAPPLICATIONENGINEWRAPPER_H
#define GAMEAPPLICATIONENGINEWRAPPER_H

#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>

#define QT_DEVELOPMENT 1


class GameApplicationEngineWrapper : public QObject
{
    Q_OBJECT
public:
    explicit GameApplicationEngineWrapper(QGuiApplication& app, QObject *parent = nullptr);


    //Q_INVOKABLE void changeLanguage();


signals:


private:

    QQmlApplicationEngine m_rEngine;


};

#endif // GAMEAPPLICATIONENGINEWRAPPER_H
