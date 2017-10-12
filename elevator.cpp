#include <iostream>
#include <queue>
using namespace std;

class IElevator {
public:
    virtual bool goUP() = 0;
    virtual bool goDown() = 0;
    virtual int getLevel() = 0;
    virtual void setLevel(int level) = 0;
    virtual bool canAddLevel(int level) = 0;
};

enum Direction {
    up,
    down
};

enum Levels {
    first,
    second,
    third,
    forth
};

// this is a normal elevator. We can also implement maintenance elevators or other kinds in similar way 
class Elevator : public IElevator {
    // Implement Go up
    // Implement Go Down
private:
    queue<int> elevatorQueue;
    Direction direction;
    int currentLevel;

public:
    // Method to set level
    void SetLevel(int level) {
        elevatorQueue.push(level);
    }

    // method to get level
    int GetLevel(int level) {
        return elevatorQueue.front();
    }

    // Method to check if a level can be added
    // we can either do this check on controller or give it to the elevators
    bool CanAddLevel(int level) {
        if (direction == Direction::up && currentLevel < level) {
            return true;
        }
        else if (direction == Direction::down && currentLevel > level) {
            return true;
        }

        return false;
    }
};

class ElevatorController {
    // contains logic to decide which elevator can be called from a particular floor
    // Contains a list of elevator
};

int main() {
    return 0;
}