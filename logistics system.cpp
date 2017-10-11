#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Use case:
// 1. A client could submit a order to get his items delivered to a given destination.
// 2. User could track staus of order.

// Classes design.
// 1. client.
// 2. order.
// 3. item
// 4. vehicle. which extended by motorcycle, car or trunk.
// 5. location. E.g. longtitude, gratitude.
// 6. admin

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

// For geographical position of any point
class Location {
    float longitude;
    float latitude;
};

// Desciption: client which could afford client use cases.
// Layer: Client side, Data layer(Related database table).
//        # Client side metadata is retrived from Data layer. 
//        # Related object are assembled or functions are implemented at client side to interact with service API layer.
class Client {
    int client_id;
    string name;
    Location adress;
};

// Layer: Client side, Data layer. 
// Layer: Client side, Data layer(Related database table).
//        # Related object are assembled or functions are implemented at client side to interact with service API layer.
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

// Layer: Client side, Data layer. 
// Layer: Client side, Data layer(Related database table).
//        # Related object are assembled or functions are implemented at client side to interact with service API layer.
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

// Layer: Client side, Data layer. 
// Layer: Client side, Data layer(Related database table).
//        # Related object are assembled or functions are implemented at client side to interact with service API layer.
class Item {
    int itemId;
    string name;
    int price;
    int volume;
    int weight;
};

// Layer: Web service layer.
class Admin {
public:
    // Take order from client side.
    void take_order(Order order) {}

    // Process order at service layer and dispatch vehicles.
    void process_order(Order order) {}

    // Retrieve data from DB and sent status to client side.
    Location track_order(int order_id) {}
};