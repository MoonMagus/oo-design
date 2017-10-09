#include <iostream>
#include <string>
#include <unordered_set>
#include <queue>
using namespace std;

enum Role {
    oprator,
    supervisor,
    director,
    unexpected
};

enum CallState {
    untaken,
    inProgress,
    finished
};

class Call {
public:
    Role role;
    int callId;
    int callTakenEmployeeId;
    int timestamp;
    CallState state;
    Call(){}
};

class CallCenter {
public:
    queue<Call> waitedCalls;
    unordered_set<Operator> operators;
    unordered_set<Supervisor> supervisors;
    unordered_set<Director> directors;

    void dispatchCall(Call& call) {
        switch (call.role)
        {
        case Role::oprator:
            // find one from oprators, if not exist, escalate this call.
        case Role::supervisor:
            // find one from supervisors, if not exist, escalate this call.
        case Role::director:
            // find one from directors, if not exist, escalate this call.
        default:
            break;
        }
    }

    // void dispatchCall(Call& call, Employee& employee){
    //     if exist, dispatch it, or escalate this call.
    //}

    void markCallFinished(Call& call) {}
    void notifyCallEscalated(Call& call) {}
};

class Employee {
public:
    int employeeId;
    string name;
    Role role;
    Call currentCall;
    CallCenter belongedCallCenter;
    bool isBusy;

public:
    Employee(int id, string _name, Role _role, CallCenter& callCenter) : employeeId(id), name(_name), role(_role), belongedCallCenter(callCenter) {}
    void takeCall(Call& call) {
        call.callTakenEmployeeId = employeeId;
        call.state = CallState::inProgress;
    }

    void finishCall() {
        currentCall.state = CallState::finished;
        belongedCallCenter.markCallFinished(currentCall);
    }

    virtual bool escalateCall() {
        currentCall.state = untaken;
        currentCall.callTakenEmployeeId = -1;
        belongedCallCenter.notifyCallEscalated(currentCall);
    }
};

class Operator: public Employee{
public:
    Operator(int id, string _name, Role _role, CallCenter& callCenter): Employee(id, _name, _role, callCenter){}

    virtual bool escalateCall() override {
        currentCall.role = Role::supervisor;
        Employee::escalateCall();
    }
};

class Supervisor: public Employee{
public:
    Supervisor(int id, string _name, Role _role, CallCenter& callCenter) : Employee(id, _name, _role, callCenter) {}

    virtual bool escalateCall() override {
        currentCall.role = Role::director;
        Employee::escalateCall();
    }
};

class Director : public Employee {
public:
    Director(int id, string _name, Role _role, CallCenter& callCenter) : Employee(id, _name, _role, callCenter) {}
};

int main() {
    return 0;
}