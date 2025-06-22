#include <QCoreApplication>
#include <QTimer>
#include <QObject>
#include <QDateTime>

#include "NatsMessageBusThingy.h"

#include <iostream>

using json = nlohmann::json;

void mergeJson(json& target, json& patch){
  std::cout << "merge target "<< target.dump();
  std::cout << "merge patch "<< patch.dump();
  for (const auto& value : patch.items()) {
    if(value.value().is_object()){
        if(target.find(value.key()) == target.end() || !target.at(value.key()).is_object() ) {
            target[value.key()] = {};
        }
        mergeJson(target[value.key()], value.value());
    } else {
        target[value.key()] = value.value();
    }
  }
  std::cout << "After merge: "<< target.dump();
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    std::cout << "Hello NATS!\n";
    json vitalState = json::parse("{\"data\":{\"Circulation\": {\"Asystole\": \"none\",\"HR\": 90,\"Rhythm\": \"Sinus\"},\"BloodPressure\": {\"Systolic\": 120,\"Diastolic\": 90,\"AuscultationGap\": false}},\"datacontenttype\":\"application/json\",\"id\":\"01J7KG6WB3X0TPN568Z7GR91QE\",\"source\":\"/TSCAB1004736/simserver\",\"specversion\":\"1.0\",\"time\":\"2024-04-05T17:31:00Z\",\"type\":\"com.laerdal.simulation.vitalstate/v1\"}");

    NatsMessageBusThingy natsMessageBusThingy;
    natsMessageBusThingy.connect("http://localhost:4222");

    std::function<void(void)> publishState([&](){
            json sendState = vitalState;
            sendState["source"] = "/TSCAB1004736/simserver";
            sendState["time"] =  QDateTime::currentDateTime().toString(Qt::ISODate).toStdString();
            natsMessageBusThingy.publish("com.laerdal.simulation.vitalstate/v1", sendState.dump());
    });

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        publishState();
    });
    timer.start(5000);

    std::function<void(const std::string&)> cb([&](const std::string& message) {
        std::cout << "received message: " << message << "\n";

        try{
            json patch = json::parse(message);

            mergeJson(vitalState["data"], patch["data"]);

            publishState();
        } catch(std::exception e) {
            std::cout << "SHIT! " << e.what() << "\n";
        }
        
        return message;
    });

    natsMessageBusThingy.subscribe("com.laerdal.simulation.vitalstate.request/v1", cb);

    return app.exec();
}