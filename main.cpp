#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QVariant>
#include <QString>
#include <string>

#include <iostream>
#include <logincode.h>

//Author https://github.com/GambaX
//Credits Pumba98, https://github.com/morsisko/NosTale-Auth

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::string tntInstallationId = "";
    std::string lang = "";
    std::string username = "";
    std::string  locale = "";
    std::string  password = "";

    loginCode *loginCode = new class loginCode();
    std::string authToken = loginCode->generateSessionToken(tntInstallationId,lang,username,locale,password);

    std::cout << "Let's Start.\n\nPlain Token:" << authToken;

    std::string platformGameAccountId = loginCode->generatePlatformGameAccountId(tntInstallationId, authToken);
    std::cout << "\n\nPlain platformGameAccountId: " << platformGameAccountId;

    std::string code = loginCode->generateCode(tntInstallationId, authToken, platformGameAccountId);
    std::cout << "\n\nPlain code: " << code;

    QString qcode = QString::fromStdString(code).toLatin1().toHex();
    code = QString::fromStdString(code).toLatin1().toHex().toStdString();
    std::cout << "\n\nHexed code: " << code;


    return a.exec();
}
