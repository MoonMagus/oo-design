#include <iostream>
#include <vector>
using namespace std;

enum VehicleType {
    motorcycle,
    car,
    bus
};

enum PotType {
    small,
    normal,
    large
};

class Vehicle {
public:
    int licesence_plate;
    VehicleType vehicle_type;
    vector<PotType> supported_pot_types;
    ParkingPot* parked_pot;

    virtual bool canFitInSpot(ParkingPot& pot) = 0;
    void moveInPot(ParkingPot* pot) {
        parked_pot = pot;
    }

    void moveOutPot() {
        parked_pot = nullptr;
    }
};

class Motorcycle : public Vehicle {
    bool canFitInSpot(ParkingPot& pot) {
        return true;
    }
};

class Car :public Vehicle {
    bool canFitInSpot(ParkingPot& pot) {
        return pot.pot_type == PotType::normal || pot.pot_type == PotType::large;
    }
};

class Bus :public Vehicle {
    bool canFitInSpot(ParkingPot& pot) {
        return pot.pot_type == PotType::large;
    }
};

class ParkingPot {
public:
    int level;
    int row;
    int column;
    bool empty;
    PotType pot_type;
    Vehicle* parked_vehicle;

    bool isAvailable() {
        return empty;
    }

    bool canFitVehicle(Vehicle& vehicle) {
        return isAvailable() && vehicle.canFitInSpot(*this);
    }

    void moveVehicleIn(Vehicle& vehicle) {
        parked_vehicle = &vehicle;
        vehicle.moveInPot(this);

        empty = false;
    }

    void moveVehiclesOut() {
        parked_vehicle = nullptr;
        if (parked_vehicle) {
            parked_vehicle->moveOutPot();
        }

        empty = true;
    }
};

class PotAllocationService {
public:
    ParkingPot* findAvailablePot(vector<PotType> pots) {
        for (auto potType : pots) {
        }

        return nullptr;
    }
};

struct PotPosition {
    int level;
    int row;
    int column;
};

class Ticket {
    int time_stamp;
    VehicleType vehicle_type;
    int vehicle_template;
    PotPosition position;
};

class ParkingLevel {
private:
    int floor;
    PotAllocationService pot_allocation_service;

public:
    bool try_park_vehicle(Vehicle& vehicle) {
        ParkingPot* pot = findAvailablePot(vehicle);
        if (pot == nullptr) {
            cout << "No available parking pot at level:" << floor << endl;

            return false;
        }
        else {
            pot->moveVehicleIn(vehicle);
            printTicket(*pot, vehicle);

            return true;
        }
    }

    ParkingPot* findAvailablePot(Vehicle& vehicle) {
        return pot_allocation_service.findAvailablePot(vehicle.supported_pot_types);
    }

    Ticket printTicket(ParkingPot& pot, Vehicle& vehicle) {
    }
};

class ParkingLot {
private:
    vector<ParkingLevel> levels;

public:
    void try_park_vehicle(Vehicle& vehicle) {
        for (auto& level : levels) {
            if (level.try_park_vehicle(vehicle))
                return;
        }
    }
};

int main() {
    return 0;
}