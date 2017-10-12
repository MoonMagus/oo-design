#include <iostream>
#include <unordered_set>
#include <string>
using namespace std;

struct Message {
    string content;
    Message() {}
    Message(string& msg) : content(msg) {}
};

class Subscriber {
private:
    NotificationServer& server;

public:
    virtual void handleMessage(NotificationServer& server, Message& msg) = 0;
};

class NotificationServer {
protected:
    unordered_set<Subscriber> subscribers;

public:
    virtual void registerObserver(Subscriber& subscriber) = 0;
    virtual void unregisterObserver(Subscriber& subscriber) = 0;
    virtual void notifyAll() = 0;
    virtual void receiveEvent() = 0;
};

class ConcreteNotificationServer : public NotificationServer {
public:
    virtual void registerObserver(Subscriber& subscriber) override {
        subscribers.insert(subscriber);
    }

    virtual void unregisterObserver(Subscriber& subscriber) override {
        if (subscribers.find(subscriber) != subscribers.end()) {
            subscribers.erase(subscriber);
        }
    }

    virtual void notifyAll() override {
        for (auto s : subscribers) {
            Message msg = Message();
            s.handleMessage(*this, msg);
        }
    }

    virtual void receiveEvent() override {
        // Handle event and call notifyAll();
    }
};

class Publisher {
private:
    NotificationServer& server;

public:
    virtual void publishEvent() = 0;
};

int main() {
    return 0;
}