#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QObject>

#include "NatsMessageBusThingy.h"

#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    qDebug() << "Hello NATS!";


    NatsMessageBusThingy natsMessageBusThingy;
    natsMessageBusThingy.connect("http://localhost:4222");

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        qDebug() << "sending!";
        natsMessageBusThingy.publish("com.laerdal.simulation.vitalstate/v1", R"({"hello":"again"})");
    });
    timer.start(5000);

    std::function<void(const std::string&)> cb([](const std::string& message) {
        qDebug() << "received message: " << QString::fromStdString(message);
        return message;
    });

    natsMessageBusThingy.subscribe("com.laerdal.simulation.vitalstate/v1", cb);

    return app.exec();
}