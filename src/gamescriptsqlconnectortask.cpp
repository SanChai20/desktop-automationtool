#include "gamescriptsqlconnectortask.h"

GameScriptSqlConnectorTask::GameScriptSqlConnectorTask(QObject *parent) : QObject(parent)
{
    m_pRawConnector = new GameScriptRDSMySQLConnector(this);
    m_pRawConnector->setThreadConnectionName(DATABASE_CURRENT_CONNECTION_THREAD1_CACHE);
}

GameScriptSqlConnectorTask::~GameScriptSqlConnectorTask()
{
    delete m_pRawConnector;
}

void GameScriptSqlConnectorTask::run()
{
    bool isCodeExisted = false;
    if (m_pRawConnector->connectToCodeTable() && m_pRawConnector->queryActivationCodeExistence(m_szActivationCode, isCodeExisted))
    {
        if (isCodeExisted)//exist then query
        {
            int addDays = 0;
            if(m_pRawConnector->queryActivationCodeAddDays(m_szActivationCode, addDays))
            {
                //query addDays success
                //makePaymentSuccess(addDays, m_szActivationCode);
                QDateTime updatedDate;
                if(m_rCurrentInternetDateTime.daysTo(m_rExpirationDateTime) >= 0)
                    updatedDate = m_rExpirationDateTime.addDays(addDays);
                else
                    updatedDate = m_rCurrentInternetDateTime.addDays(addDays);

                //notify database...
                //update database
                if(m_pRawConnector->updateUserExpirationDateTime(m_szDeviceMacAddress, updatedDate))
                {
#if 1
                    //delete activation code
                    if(m_pRawConnector->deleteActivationCodeRecord(m_szActivationCode))
                    {
                        m_pRawConnector->disconnect();
                        emit success();
                        return;
                    }
                    else
                    {
                        m_pRawConnector->updateUserExpirationDateTime(m_szDeviceMacAddress, m_rExpirationDateTime);
                        m_pRawConnector->disconnect();
                        emit failed("激活失败！同步激活码数据库存在问题，可能为网络问题，请稍后重试.");
                    }
#else
					m_pRawConnector->disconnect();
					emit success();
					return;
#endif
                }
                else
                {
                    m_pRawConnector->disconnect();
                    emit failed("激活失败！同步用户信息失败，可能为网络问题，请稍后重试.");
                }
                return;
            }
            else
            {
                //query addDays failed
                m_pRawConnector->disconnect();
                emit failed("激活失败！激活码有效但获取失败，请检查网络连接.");
                return;
            }
        }
        else//not exist then return
        {
            m_pRawConnector->disconnect();
            emit failed("激活失败！激活码无效，请联系相关人员进行处理.");
            return;
        }
    }
    m_pRawConnector->disconnect();
    emit failed("激活失败！您的网络连接存在问题，请联系相关人员进行协助.");
    return;
}

void GameScriptSqlConnectorTask::set(const QDateTime& current, const QDateTime& expiration, const QString& macAddress, const QString& activation)
{
    m_rCurrentInternetDateTime = current;
    m_rExpirationDateTime = expiration;
    m_szDeviceMacAddress = macAddress;
    m_szActivationCode = activation;
}
