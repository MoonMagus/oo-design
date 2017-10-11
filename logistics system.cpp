#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum VehicleStatus {
    free,
    busy,
    not_working
};

enum OrderStatus {
    delivered,
    processing,
    cancelled
};

enum PaymentStatus {
    paid,
    unpaid
};  

enum OrderPriority {
    low,
    medium,
    high
};

// for geographical position of any point
class Location {
    float longitude;
    float latitude;
};

// for the vehicles used for transportation
class Vehicle {
    int id;
    string vehicleNo;
    int capacity;
    Location current_position;
    VehicleStatus current_condition;
};

class Truck : public Vehicle {
};

class Bike : public Vehicle {
};

// a new order is created for each order place by any client
class Order {
    int order_id;
    OrderPriority priority_of_order;
    Client owner_of_order;
    Location destination;
    int amount_of_charge;
    vector<Item> items;
    int total_weight;
    OrderStatus current_status;
    PaymentStatus status_of_payment;
    int time_of_order_placed;
    int time_of_delivery;
};

// An order is List of Items
class Item {
    int itemId;
    string name;
    int price;
    int volume;
    int weight;
};

class Client {
    int client_id;
    string name;
    Location adress;
};

class Admin {
public:
    void take_order(Order order) {}
    void process_order(Order order) {}
    Location track_order(int order_id) {}
};