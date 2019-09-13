#ifndef LOGINCODE_H
#define LOGINCODE_H

#include <string>
#include <QNetworkAccessManager>
#include <QJsonObject>

class loginCode
{
public:
    loginCode();
    const std::string platformGameId = "dd4e22d6-00d1-44b9-8126-d8b40e0cd7c9";
    QNetworkRequest req;
    QNetworkAccessManager man;
    QJsonObject jObj;
    QNetworkReply *reply;

    std::string generateSessionToken(std::string tntInstallationId, std::string lang,
                                       std::string username, std::string locale, std::string password);
    std::string generatePlatformGameAccountId(std::string tntInstallationId, std::string authToken);
    std::string generateCode(std::string tntInstallationId, std::string authToken, std::string platformGameAccountId);
};

#endif // LOGINCODE_H
