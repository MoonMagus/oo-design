#include <iostream>
#include <vector>
#include <queue>
using namespace std;

enum Rank {
    respondent = 0,
    director = 1,
    manager = 2
};

class Call {
public:
    Rank rank;
    int caller_id;
    int handler_id;

    void reply(string message) {}
    void disconnect() {}
};

class Employee {
public:
    int employee_id;
    Rank rank;
    bool isBusy;

    void receiveCall(Call& call) {}
    void disconnect() {}
    bool isFree() {
        return !isBusy;
    }

    void escalateCall(Call call) {
        // escalate this call and request call center to add this call to queue. 
    }

    Rank getRank() {
        return rank;
    }
};

class Respondent : public Employee {
public:
    Respondent() {
        rank = Rank::respondent;
    }
};

class Director : public Employee {
public:
    Director() {
        rank = Rank::director;
    }
};

class Manager : public Employee {
public:
    Manager() {
        rank = Rank::manager;
    }
};

class CallCenter {
private:
    const int respondent_num = 50;
    const int director_num = 50;
    const int manager_num = 50;

    vector<Respondent> respondents;
    vector<Director> directors;
    vector<Manager> managers;

    queue<Respondent> wait_for_respondents;
    queue<Director> wait_for_directors;
    queue<Manager> wait_for_managers;

    queue<Respondent> free_for_respondents;
    queue<Director> free_for_directors;
    queue<Manager> free_for_managers;

    void dispatchCall(Call& call) {
        Employee* em = getHandlerForCall(call);
        if (em) {
            em->receiveCall(call);
            call.handler_id = em->employee_id;
        }
        else {
            storeCallIntoQueue(call);
        }
    }

    Employee* getHandlerForCall(Call& call) {
        switch (call.rank) {
        case Rank::respondent:
            // Try to find one.
        case Rank::director:
            // Try to find one.
        case Rank::manager:
            // Try to find one.
        default:
            break;
        }
    }

    bool storeCallIntoQueue(Call& call) {
    }

    bool assignCall(Call& call, Employee& em) {}
};

int main() {
    return 0;
}
