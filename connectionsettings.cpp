#include "connectionsettings.h"

ConnectionSettings::ConnectionSettings(QString hostname, QString port, QString username, QString password, bool use_sasl)
{
    this->bind_dn = strdup("");
    this->hostname = strdup((hostname + ":" + port).toStdString().c_str());
    this->username = strdup(username.toStdString().c_str());
    this->password = strdup(password.toStdString().c_str());
    this->useSasl = use_sasl;
}

ConnectionSettings::~ConnectionSettings()
{
    free(this->bind_dn);
    free(this->hostname);
    free(this->username);
    free(this->password);
}
