#ifndef GLOBALSETTINGSPROFILE_H
#define GLOBALSETTINGSPROFILE_H

#include <QObject>
#include "gamescriptimageprocessor.h"

enum PolicyType
{
    STOP_AT_FAILED,
    LOOP_AT_FAILED,
    CONTINUE_AT_FAILED,
};

enum LanguageType
{
    CHINESE,
    ENGLISH
};

struct GlobalSettings
{
    DetectAlgorithm m_rDetectType = DA_SIFT;
    int m_nDetectComboIndex = 1;
    MatchAlgorithm m_rMatchType = MA_BRUTE2;
    int m_nMatchComboIndex = 1;
    PolicyType m_rPolicyType = STOP_AT_FAILED;
    int m_nPolicyComboIndex = 0;
    LanguageType m_rLanguageType = CHINESE;
    int m_nLanguageComboIndex = 0;
    float m_fRatioThreshold = 0.4f;
};

class GlobalSettingsSingletonInstance
{
public:

    static GlobalSettingsSingletonInstance* Instance();
    void updateSettings(const GlobalSettings& settings);
    const MatchAlgorithm queryMatchType() const;
    const DetectAlgorithm queryDetectType() const;
    const PolicyType queryPolicyType() const;
    const LanguageType queryLanguageType() const;
    const float querykNNRatio() const;

private:
    GlobalSettings m_rSettings;
    static GlobalSettingsSingletonInstance* instancePtr;
};

class GlobalSettingsProfile : public QObject
{
    Q_OBJECT
public:

    //static GlobalSettingsProfile* Instance();

    explicit GlobalSettingsProfile(QObject *parent = nullptr);

    Q_INVOKABLE void switchMatchType(int index);
    Q_INVOKABLE void switchDetectType(int index);
    Q_INVOKABLE void switchPolicyType(int index);
    Q_INVOKABLE void switchLanguageType(int index);
    Q_INVOKABLE void changekNNRatio(float ratio);
    Q_INVOKABLE void show();
    Q_INVOKABLE void flip();

private:

    void readXML();
    void swap();

signals:

    void showSettings(int matchIndex, int detectIndex, int policyIndex, int languageIndex, float kNNRatio);
    void readyToShow();

private:

    

    GlobalSettings m_rReadSettings;
    GlobalSettings m_rWriteSettings;
};
#endif // GLOBALSETTINGSPROFILE_H
