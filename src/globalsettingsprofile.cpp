#include "globalsettingsprofile.h"
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
GlobalSettingsSingletonInstance* GlobalSettingsSingletonInstance::instancePtr = nullptr;
GlobalSettingsSingletonInstance* GlobalSettingsSingletonInstance::Instance()
{
    if (instancePtr == nullptr)
    {
        instancePtr = new GlobalSettingsSingletonInstance();
        return instancePtr;
    }
    else
    {
        return instancePtr;
    }
}
void GlobalSettingsSingletonInstance::updateSettings(const GlobalSettings& settings)
{
    m_rSettings.m_fRatioThreshold = settings.m_fRatioThreshold;
    m_rSettings.m_rMatchType = settings.m_rMatchType;
    m_rSettings.m_rDetectType = settings.m_rDetectType;
    m_rSettings.m_rPolicyType = settings.m_rPolicyType;
    m_rSettings.m_rLanguageType = settings.m_rLanguageType;
    m_rSettings.m_nMatchComboIndex = settings.m_nMatchComboIndex;
    m_rSettings.m_nDetectComboIndex = settings.m_nDetectComboIndex;
    m_rSettings.m_nPolicyComboIndex = settings.m_nPolicyComboIndex;
    m_rSettings.m_nLanguageComboIndex = settings.m_nLanguageComboIndex;
}

const MatchAlgorithm GlobalSettingsSingletonInstance::queryMatchType() const
{
    return m_rSettings.m_rMatchType;
}

const DetectAlgorithm GlobalSettingsSingletonInstance::queryDetectType() const
{
    return m_rSettings.m_rDetectType;
}

const PolicyType GlobalSettingsSingletonInstance::queryPolicyType() const
{
    return m_rSettings.m_rPolicyType;
}

const LanguageType GlobalSettingsSingletonInstance::queryLanguageType() const
{
    return m_rSettings.m_rLanguageType;
}

const float GlobalSettingsSingletonInstance::querykNNRatio() const
{
    return m_rSettings.m_fRatioThreshold;
}

GlobalSettingsProfile::GlobalSettingsProfile(QObject *parent) : QObject(parent)
{
    readXML();
}



void GlobalSettingsProfile::readXML()
{
    QFile file("globalsettings.xml");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error: Cannot read file";
        return;
    }
    int detectTypeIndex = 1, matchTypeIndex = 1, policyTypeIndex = 0, languageTypeIndex = 0;
    float kNNRatio = 0.4f;
    QXmlStreamReader xmlReader(&file);
    xmlReader.readNext();
    while(!xmlReader.atEnd())
    {
        if(xmlReader.isStartElement())
        {
            if(xmlReader.name() == "GlobalSettingsProfile")
            {
                xmlReader.readNext();
            }
            else if (xmlReader.name() == "detectType")
            {
                detectTypeIndex = xmlReader.readElementText().toInt();
                switchDetectType(detectTypeIndex);
                xmlReader.readNext();
            }
            else if (xmlReader.name() == "matchType")
            {
                matchTypeIndex = xmlReader.readElementText().toInt();
                switchMatchType(matchTypeIndex);
                xmlReader.readNext();
            }
            else if (xmlReader.name() == "policyType")
            {
                policyTypeIndex = xmlReader.readElementText().toInt();
                switchPolicyType(policyTypeIndex);
                xmlReader.readNext();
            }
            else if (xmlReader.name() == "languageType")
            {
                languageTypeIndex = xmlReader.readElementText().toInt();
                switchLanguageType(languageTypeIndex);
                xmlReader.readNext();
            }
            else if (xmlReader.name() == "kNNRatio")
            {
                kNNRatio = xmlReader.readElementText().toFloat();
                changekNNRatio(kNNRatio);
                qDebug() << "kNN: " << kNNRatio;
                xmlReader.readNext();
            }
            else
            {
                xmlReader.raiseError(QObject::tr("Not a options file"));
            }
        }
        else
        {
            xmlReader.readNext();
        }
    }
    file.close();
    if (xmlReader.hasError())
    {
        qDebug() << "Error: Failed to parse file";
        return;
    }
    else if (file.error() != QFile::NoError)
    {
        qDebug() << "Error: Cannot read file";
        return;
    }
    swap();
}

void GlobalSettingsProfile::show()
{
    emit showSettings(m_rReadSettings.m_nMatchComboIndex,
                      m_rReadSettings.m_nDetectComboIndex,
                      m_rReadSettings.m_nPolicyComboIndex,
                      m_rReadSettings.m_nLanguageComboIndex,
                      m_rReadSettings.m_fRatioThreshold);
}

void GlobalSettingsProfile::swap()
{
    m_rReadSettings.m_fRatioThreshold = m_rWriteSettings.m_fRatioThreshold;
    m_rReadSettings.m_rMatchType = m_rWriteSettings.m_rMatchType;
    m_rReadSettings.m_rDetectType = m_rWriteSettings.m_rDetectType;
    m_rReadSettings.m_rPolicyType = m_rWriteSettings.m_rPolicyType;
    m_rReadSettings.m_rLanguageType = m_rWriteSettings.m_rLanguageType;
    m_rReadSettings.m_nMatchComboIndex = m_rWriteSettings.m_nMatchComboIndex;
    m_rReadSettings.m_nDetectComboIndex = m_rWriteSettings.m_nDetectComboIndex;
    m_rReadSettings.m_nPolicyComboIndex = m_rWriteSettings.m_nPolicyComboIndex;
    m_rReadSettings.m_nLanguageComboIndex = m_rWriteSettings.m_nLanguageComboIndex;
    GlobalSettingsSingletonInstance::Instance()->updateSettings(m_rReadSettings);
}

void GlobalSettingsProfile::flip()
{
    swap();
    //Write out to xml file
    QFile file("globalsettings.xml");
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("GlobalSettingsProfile");
    xmlWriter.writeTextElement("detectType", QString::number(m_rWriteSettings.m_nDetectComboIndex));
    xmlWriter.writeTextElement("matchType", QString::number(m_rWriteSettings.m_nMatchComboIndex));
    xmlWriter.writeTextElement("policyType", QString::number(m_rWriteSettings.m_nPolicyComboIndex));
    xmlWriter.writeTextElement("languageType", QString::number(m_rWriteSettings.m_nLanguageComboIndex));
    xmlWriter.writeTextElement("kNNRatio", QString::number(m_rWriteSettings.m_fRatioThreshold));
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();

    qDebug() << "swap!!";

}

void GlobalSettingsProfile::switchMatchType(int index)
{
    m_rWriteSettings.m_nMatchComboIndex = index;
    if (index == 0)
        m_rWriteSettings.m_rMatchType = MA_BRUTE1;
    else if (index == 1)
        m_rWriteSettings.m_rMatchType = MA_BRUTE2;
    else if (index == 2)
        m_rWriteSettings.m_rMatchType = MA_FLANNBASED1;
    else if (index == 3)
        m_rWriteSettings.m_rMatchType = MA_FALNNBASED2;
    else
        m_rWriteSettings.m_rMatchType = MA_BRUTE2;
}
void GlobalSettingsProfile::switchDetectType(int index)
{
    m_rWriteSettings.m_nDetectComboIndex = index;
    if (index == 0)
        m_rWriteSettings.m_rDetectType = DA_SURF;
    else if (index == 1)
        m_rWriteSettings.m_rDetectType = DA_SIFT;
    else if (index == 2)
        m_rWriteSettings.m_rDetectType = DA_KAZE;
    else
        m_rWriteSettings.m_rDetectType = DA_SIFT;
}
void GlobalSettingsProfile::switchPolicyType(int index)
{
    m_rWriteSettings.m_nPolicyComboIndex = index;
    if(index == 0)
        m_rWriteSettings.m_rPolicyType = STOP_AT_FAILED;
    else if(index == 1)
        m_rWriteSettings.m_rPolicyType = LOOP_AT_FAILED;
    else if(index == 2)
        m_rWriteSettings.m_rPolicyType = CONTINUE_AT_FAILED;
    else
        m_rWriteSettings.m_rPolicyType = STOP_AT_FAILED;
}
void GlobalSettingsProfile::switchLanguageType(int index)
{
    m_rWriteSettings.m_nLanguageComboIndex = index;
    if(index == 0)
        m_rWriteSettings.m_rLanguageType = CHINESE;
    else if(index == 1)
        m_rWriteSettings.m_rLanguageType = ENGLISH;
    else
        m_rWriteSettings.m_rLanguageType = CHINESE;
}
void GlobalSettingsProfile::changekNNRatio(float ratio)
{
    m_rWriteSettings.m_fRatioThreshold = ratio;
}
