#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum SpotSize {
    Motorcycle,
    Compact,
    Large
};

class Vehicle {
protected:
    vector<ParkingSpot> parkingSpots;
    string licensePlate;
    SpotSize size;
    int spotsNeeded;

public:
    int getSpotsNeeded() {
        return spotsNeeded;
    }

    SpotSize getSize() {
        return size;
    }

    /* Park vehicle in this spot (among others, potentially) */
    void parkInSpot(ParkingSpot& spot) {
        parkingSpots.push_back(spot);
    }

    /* Remove car from spot, and notify spot that it's gone */
    void clearSpots() {
        for (int i = 0; i < parkingSpots.size(); i++) {
            parkingSpots[i].removeVehicle();
        }
        parkingSpots.clear();
    }

    virtual bool canFitInSpot(ParkingSpot spot) = 0;
    virtual void print() = 0;
};

class Motorcycle : public Vehicle {
public:
    Motorcycle() {
        spotsNeeded = 1;
        size = SpotSize::Motorcycle;
    }

    bool canFitInSpot(ParkingSpot& spot) {
        return true;
    }

    void print() {
        cout << "M";
    }
};

class Car : public Vehicle {
public:
    Car() {
        spotsNeeded = 1;
        size = SpotSize::Compact;
    }

    bool canFitInSpot(ParkingSpot& spot) {
        return spot.getSize() == SpotSize::Large || spot.getSize() == SpotSize::Compact;
    }

    void print() {
        cout << "C";
    }
};

class Bus : public Vehicle {
public:
    Bus() {
        spotsNeeded = 5;
        size = SpotSize::Large;
    }

    bool canFitInSpot(ParkingSpot spot) {
        return spot.getSize() == SpotSize::Large;
    }

    void print() {
        cout << "B";
    }
};

/* Represents a level in a parking garage */
class Level {
private:
    int floor;
    vector<ParkingSpot> spots;
    int availableSpots = 0; // number of free spots
    const int SPOTS_PER_ROW = 10;

public:
    Level() {}
    Level(int flr, int numberSpots) {
        floor = flr;
        spots = vector<ParkingSpot>();
        int largeSpots = numberSpots / 4;
        int  compactSpots = numberSpots / 4;
        int motorcycleSpots = numberSpots - largeSpots - motorcycleSpots;
        for (int i = 0; i < numberSpots; i++) {
            SpotSize sz;
            if (i < largeSpots) {
                sz = SpotSize::Large;
            }
            else if (i < largeSpots + compactSpots) {
                sz = SpotSize::Compact;
            }
            else {
                sz = SpotSize::Motorcycle;
            }

            int row = i / SPOTS_PER_ROW;
            spots.push_back(ParkingSpot(this, row, i, sz));
        }
        availableSpots = numberSpots;
    }

    int availableSpots() {
        return availableSpots;
    }

    /* Try to find a place to park this vehicle. Return false if failed. */
    bool parkVehicle(Vehicle& vehicle) {
        if (availableSpots < vehicle.getSpotsNeeded()) {
            return false;
        }

        int spotNumber = findAvailableSpots(vehicle);
        if (spotNumber < 0) {
            return false;
        }

        return parkStartingAtSpot(spotNumber, vehicle);
    }

    void print() {
        int lastRow = -1;
        for (int i = 0; i < spots.size(); i++) {
            ParkingSpot spot = spots[i];
            if (spot.getRow() != lastRow) {
                cout << "  ";
                lastRow = spot.getRow();
            }
            spot.print();
        }
    }

    /* When a car was removed from the spot, increment availableSpots */
    void spotFreed() {
        availableSpots++;
    }

private:
    /* Park a vehicle starting at the spot spotNumber, and continuing until vehicle.spotsNeeded. */
    bool parkStartingAtSpot(int spotNumber, Vehicle& vehicle) {
        vehicle.clearSpots();
        bool success = true;
        for (int i = spotNumber; i < spotNumber + vehicle.getSpotsNeeded(); i++) {
            success &= spots[i].park(&vehicle);
        }

        availableSpots -= vehicle.getSpotsNeeded();

        return success;
    }

    /* find a spot to park this vehicle. Return index of spot, or -1 on failure. */
    int findAvailableSpots(Vehicle& vehicle) {
        int spotsNeeded = vehicle.getSpotsNeeded();
        int previousRow = -1;
        int spotsFound = 0;
        for (int i = 0; i < spots.size(); i++) {
            ParkingSpot spot = spots[i];
            // if current row and previous row are not the same row, set spotsFound to zero. 
            // Because we could not park a vehicle in two rows.
            if (previousRow != spot.getRow()) {
                spotsFound = 0;
                previousRow = spot.getRow();
            }

            // if current pot is not suitable for vehicle, we will reset spotsFound to zero.
            // Because we could not found a contious region to park this vehicle.
            if (spot.canFitVehicle(&vehicle)) {
                spotsFound++;
            }
            else {
                spotsFound = 0;
            }

            if (spotsFound == spotsNeeded) {
                return i - (spotsNeeded - 1);
            }
        }

        return -1;
    }
};

class ParkingSpot {
private:
    Vehicle* vehicle;
    SpotSize spotSize;
    int row;
    int spotNumber;
    Level* level;

public:
    ParkingSpot(Level* lvl, int r, int n, SpotSize sz) {
        level = lvl;
        row = r;
        spotNumber = n;
        spotSize = sz;
    }

    bool isAvailable() {
        return vehicle == nullptr;
    }

    /* Checks if the spot is big enough for the vehicle (and is available). This compares
    * the SIZE only. It does not check if it has enough spots. */
    bool canFitVehicle(Vehicle* vehicle) {
        return isAvailable() && vehicle->canFitInSpot(*this);
    }

    /* Park vehicle in this spot. */
    bool park(Vehicle* v) {
        if (!canFitVehicle(v)) {
            return false;
        }

        vehicle = v;
        vehicle->parkInSpot(*this);
        return true;
    }

    int getRow() {
        return row;
    }

    int getSpotNumber() {
        return spotNumber;
    }

    SpotSize getSize() {
        return spotSize;
    }

    /* Remove vehicle from spot, and notify level that a new spot is available */
    void removeVehicle() {
        level->spotFreed();
        vehicle = nullptr;
    }

    void print() {
        if (vehicle == nullptr) {
            switch (spotSize) {
            case SpotSize::Motorcycle:
                cout << "m" << endl;
            case SpotSize::Compact:
                cout << "c" << endl;
            case SpotSize::Large:
                cout << "l" << endl;
            default:
                break;
            }
        }
        else {
            vehicle->print();
        }
    }
};

int main() {
    return 0;
}