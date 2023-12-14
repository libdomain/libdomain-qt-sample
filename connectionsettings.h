#ifndef LD_CONNECTION_SETTINGS_H
#define LD_CONNECTION_SETTINGS_H

#include <QString>

struct ConnectionSettings
{
    char* hostname;
    char* password;
    char* username;
    char* bind_dn;
    bool useSasl;

    ConnectionSettings(QString hostname, QString port, QString username, QString password, bool use_sasl);
    ~ConnectionSettings();
};

#endif//LD_CONNECTION_SETTINGS_H
