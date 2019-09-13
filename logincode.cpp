#include "logincode.h"
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

loginCode::loginCode()
{

}

std::string loginCode::generateSessionToken(std::string tntInstallationId, std::string lang,
                                              std::string username, std::string locale, std::string password)
{
    //Set the post request to send
    //QUrl is the link where to ask for auth
    //TNT-Installation-Id Installation id on local drive
    //Origin
    //Content-Type is the type of content I'm going to send
    req.setUrl(QUrl("https://spark.gameforge.com/api/v1/auth/thin/sessions"));
    req.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Safari/537.36");
    req.setRawHeader("TNT-Installation-Id", QString::fromUtf8(tntInstallationId.c_str()).toUtf8());
    req.setRawHeader("Origin", "spark://www.gameforge.com");
    req.setRawHeader("Content-Type", "application/json");

    //Here's the json who needs to be sent
    //gfLang is the language
    //identity is username
    //locale game lang
    //password
    //platformGameId is a costant
    jObj.insert("gfLang", QJsonValue::fromVariant(QString::fromUtf8(lang.c_str()).toUtf8()));
    jObj.insert("identity", QJsonValue::fromVariant(QString::fromUtf8(username.c_str()).toUtf8()));
    jObj.insert("locale", QJsonValue::fromVariant(QString::fromUtf8(locale.c_str()).toUtf8()));
    jObj.insert("password", QJsonValue::fromVariant(QString::fromUtf8(password.c_str()).toUtf8()));
    jObj.insert("platformGameId", QJsonValue::fromVariant(QString::fromUtf8(loginCode::platformGameId.c_str()).toUtf8()));

    QJsonDocument jDoc(jObj);

    //Send the request
    reply = man.post(req, jDoc.toJson());

    //Read the reply until last char
    while(!reply->isFinished()){
        qApp->processEvents();

    }

    //Read Bytes
    QByteArray responseBytes = reply->readAll();
    //qDebug()<< "\n\nConnecting... Json received!!: " <<responseBytes << endl;

    //Parse json
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseBytes);

    QJsonObject jsonObject = jsonResponse.object();

    //Return the session token
    return jsonObject["token"].toString().toLatin1().toStdString();
}

std::string loginCode::generatePlatformGameAccountId(std::string tntInstallationId, std::string authToken)
{
    //Ask for platformGameAccountId
    req.setUrl(QUrl("https://spark.gameforge.com/api/v1/user/accounts"));
    req.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36");
    req.setRawHeader("TNT-Installation-Id", QString::fromUtf8(tntInstallationId.c_str()).toUtf8());
    req.setRawHeader("Origin", "spark://www.gameforge.com");
    QString bearer = "Bearer ";
    bearer += QString::fromUtf8(authToken.c_str());
    req.setRawHeader("Authorization", bearer.toUtf8());

    //Send the GET request
    reply = man.get(req);

    //Read the reply until last char
    while(!reply->isFinished()){
        qApp->processEvents();
    }

    //Read Bytes
    QByteArray responseBytes = reply->readAll();
    //qDebug()<< "\n\nReceived content after asking for id: " <<responseBytes << endl;

    //Parse json
     QJsonDocument jsonResponse = QJsonDocument::fromJson(responseBytes);

    QJsonObject jsonObject = jsonResponse.object();

    //Saves the first key of the json receveid, the value we're looking for!
    QString platformGameAccountId = jsonObject.keys()[0];
    //qDebug() << "\n\nSo.. your platformGameAccountId is: " << jsonObject.keys()[0];

    return platformGameAccountId.toStdString();
}

std::string loginCode::generateCode(std::string tntInstallationId, std::string authToken, std::string platformGameAccountId)
{
    //Send the POST request to codes
    req.setUrl(QUrl("https://spark.gameforge.com/api/v1/auth/thin/codes"));
    req.setRawHeader("User-Agent", "GameforgeClient/2.0.34");
    req.setRawHeader("TNT-Installation-Id", QString::fromUtf8(tntInstallationId.c_str()).toUtf8());
    QString bearer = "Bearer ";
    bearer += QString::fromUtf8(authToken.c_str());
    req.setRawHeader("Authorization", bearer.toUtf8());
    req.setRawHeader("Origin", "spark://www.gameforge.com");
    req.setRawHeader("Content-Type", "application/json");

    QJsonObject jObj2;
    jObj2.insert("platformGameAccountId", QString::fromUtf8(platformGameAccountId.c_str()));
    QJsonDocument jDoc2(jObj2);


    //Send the request
    reply = man.post(req, jDoc2.toJson());

    //Read the reply until last char
    while(!reply->isFinished()){
        qApp->processEvents();

    }

    //Read Bytes
    QByteArray responseBytes = reply->readAll();
    //qDebug()<< "\n\nHere's the code we were looking for: " <<responseBytes << endl;

    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseBytes);
    QJsonObject jsonObject = jsonResponse.object();

    return jsonObject["code"].toString().toLatin1().toStdString();
}
