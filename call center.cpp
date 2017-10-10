#include <iostream>
#include <vector>
#include <string>
#include <queue>
using namespace std;

enum Rank {
    responder = 0,
    manager = 1,
    director = 2,
    //
    //private:
    //    int value;
    //    void Rank(int v) {
    //        value = v;
    //    }
    //
    //public:
    //    int getValue() {
    //        return value;
    //    }
};

class Caller {
private:
    string name;
    int userId;
public:
    Caller() {}
    Caller(int id, string nm) {
        name = nm;
        userId = id;
    }
};

/* Represents a call from a user. Calls have a minimum rank and are assigned to the
* first employee who can handle that call.
*/
class Call {
    /* Minimal rank of employee who can handle this call. */
private:
    Rank rank;

    /* Person who is calling. */
    Caller caller;

    /* Employee who is handling call. */
    Employee handler;

public:
    Call(Caller c) {
        rank = Rank::responder;
        caller = c;
    }

    /* Set employee who is handling call. */
    void setHandler(Employee e) {
        handler = e;
    }

    /* Play recorded message to the customer. */
    void reply(string message) {
        cout << message << endl;
    }

    Rank getRank() {
        return rank;
    }

    void setRank(Rank r) {
        rank = r;
    }

    Rank incrementRank() {
        if (rank == Rank::responder) {
            rank = Rank::manager;
        }
        else if (rank == Rank::manager) {
            rank = Rank::director;
        }

        return rank;
    }

    /* Disconnect call. */
    void disconnect() {
        reply("Thank you for calling");
    }
};

/* Employee is a super class for the Director, Manager, and Respondent classes. It is implemented as an
* abstract class, since there should be no reason to instantiated an Employee type directly.
*/
class Employee {
private:
    Call* currentCall = nullptr;

protected:
    Rank rank;

public:
    Employee() {}

    /* Start the conversation */
    void receiveCall(Call* call) {
        currentCall = call;
    }

    /* the issue is resolved, finish the call */
    void callCompleted() {
        if (currentCall != nullptr) {
            /* Disconnect the call. */
            currentCall->disconnect();

            /* Free the employee */
            currentCall = nullptr;
        }

        /* Check if there is a call waiting in queue */
        assignNewCall();
    }

    /*
    * The issue has not been resolved. Escalate the call, and assign a new call
    * to the employee.
    */
    void escalateAndReassign() {
        if (currentCall != nullptr) {
            /* escalate call */
            currentCall->incrementRank();
            CallHandler::getInstance()->dispatchCall(currentCall);

            /* free the employee */
            currentCall = nullptr;
        }

        /* assign a new call */
        assignNewCall();
    }

    /* Assign a new call to an employee, if the employee is free. */
    bool assignNewCall() {
        if (!isFree()) {
            return false;
        }

        return CallHandler::getInstance()->assignCall(this);
    }

    /* Returns whether or not the employee is free. */
    bool isFree() {
        return currentCall == nullptr;
    }

    Rank getRank() {
        return rank;
    }
};

class Respondent : public Employee {
public:
    Respondent() {
        rank = Rank::responder;
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

/* CallHandler is implement as a singleton class. It represents the body of the program,
* and all calls are funneled first through it.
*/
class CallHandler {
private:
    static CallHandler* instance;

    /* We have 3 levels of employees: respondents, managers, directors. */
    int LEVELS = 3;

    /* Initialize with 10 respondents, 4 managers, and 2 directors. */
    int NUM_RESPONDENTS = 10;
    int NUM_MANAGERS = 4;
    int NUM_DIRECTORS = 2;

    /* List of employees, by level.
    * employeeLevels[0] = respondents
    * employeeLevels[1] = managers
    * employeeLevels[2] = directors
    */
    vector<vector<Employee>> employeeLevels;

    /* queues for each call’s rank */
    vector<queue<Call>> callQueues;

    int getLevel(Rank rank) {
        switch (rank) {
        case Rank::responder:
            return 0;
        case Rank::director:
            return 1;
        case Rank::manager:
            return 2;
        default:
            return -1;
        }
    }

protected:
    CallHandler() : employeeLevels(vector<vector<Employee>>(LEVELS)), callQueues(vector<queue<Call>>(LEVELS)) {
        // Create respondents, 10.
        vector<Employee> respondents(NUM_RESPONDENTS);
        for (int k = 0; k < NUM_RESPONDENTS - 1; k++) {
            respondents.push_back(Respondent());
        }
        employeeLevels.push_back(respondents);

        // Create managers, 4.
        vector<Employee> managers(NUM_MANAGERS);
        for (int k = 0; k < NUM_MANAGERS - 1; k++) {
            managers.push_back(Manager());
        }
        employeeLevels.push_back(managers);

        // Create directors, 2.
        vector<Employee> directors(NUM_DIRECTORS);
        for (int k = 0; k < NUM_DIRECTORS - 1; k++) {
            managers.push_back(Director());
        }
        employeeLevels.push_back(directors);
    }

public:
    /* Get instance of singleton class. */
    static CallHandler* getInstance() {
        if (instance == nullptr) {
            instance = new CallHandler();
        }

        return instance;
    }

    /* Gets the first available employee who can handle this call. */
    Employee* getHandlerForCall(Call* call) {
        for (int level = getLevel(call->getRank()); level < LEVELS - 1; level++) {
            for (Employee emp : employeeLevels[level]) {
                if (emp.isFree()) {
                    return &emp;
                }
            }
        }
        return nullptr;
    }

    /* Routes the call to an available employee, or saves in a queue if no employee available. */
    void dispatchCall(Caller& caller) {
        Call call = Call(caller);
        dispatchCall(&call);
    }

    /* Routes the call to an available employee, or saves in a queue if no employee available. */
    void dispatchCall(Call* call) {
        /* Try to route the call to an employee with minimal rank. */
        Employee* emp = getHandlerForCall(call);
        if (emp != nullptr) {
            emp->receiveCall(call);
            call->setHandler(*emp);
        }
        else {
            /* Place the call into corresponding call queue according to its rank. */
            call->reply("Please wait for free employee to reply");
            callQueues[getLevel(call->getRank())].push(*call);
        }
    }

    /* An employee got free. Look for a waiting call that he/she can serve. Return true
    * if we were able to assign a call, false otherwise. */
    bool assignCall(Employee* emp) {
        /* Check the queues, starting from the highest rank this employee can serve. */
        for (int rank = getLevel(emp->getRank()); rank >= 0; rank--) {
            queue<Call> que = callQueues[rank];

            /* Remove the first call, if any */
            if (que.size() > 0) {
                Call call = que.front();
                que.pop();
                if (&call != nullptr) {
                    emp->receiveCall(&call);
                    return true;
                }
            }
        }
        return false;
    }
};
