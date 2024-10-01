#include "NatsMessageBusThingy.h"

#include <string>
#include <functional>

#include <QUrl>

void NatsMessageBusThingy::connect(const std::string& address){
    qDebug() << "connecting to: " << QString::fromStdString(address);
    try{
        m_client.connectToServer(QString::fromStdString(address));
    }catch(std::exception& e){
        qDebug() << "Error connecting to server: " << e.what();
    }
}
    
void NatsMessageBusThingy::publish(const std::string& address, const std::string& message){
    qDebug() << "publishing message: " << QString::fromStdString(message);
    m_client.publish(QtNats::Message(QString::fromStdString(address).toUtf8(), QString::fromStdString(message).toUtf8()));
}

void NatsMessageBusThingy::subscribe(const std::string& address, std::function<void(const std::string&)> cb){
    qDebug() << "subscribe to: " << QString::fromStdString(address);
    QtNats::Subscription* subscription = m_client.subscribe(QString::fromStdString(address).toUtf8());
    QObject::connect(subscription, &QtNats::Subscription::received, [&](const QtNats::Message& message) {
        cb(QString(message.data).toStdString());
    });
}