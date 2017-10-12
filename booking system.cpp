#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Use case:
// 1. Client order a room through online system for a specified city.

// Classes.
// 1. client
// 2. order system(Transaction server which located at server API layer)
// 3. room
// 4. city
// 5. facaility

enum RoomStatus {
    empty,
    not_empty
};

enum RoomType {
    single,
    doubles,
    triple
};

enum PaymentStatus {
    paid,
    unpaid
};

struct Date {
    int year;
    int day;
    int hour;
    int miniutes;
};

// Layer: Client side, Data layer(Related database table).
//        # Client side metadata is retrived from Data layer. 
//        # Related object are assembled or functions are implemented at client side to interact with service API layer.
class Client {
    int userId;
    string name;
    Date birthday;
    string mobileNumber;
    string email;
    string sex;

public:
    vector<Room> viewRoomsOfSpecifiedCity(City& city) {}
    bool bookRoom(int roomId) {}
};

// Layer: API server.
class Server {
public:
    vector<Room> viewRoomsOfSpecifiedCity(City& city) {}
    bool bookRoom(int roomId) {}
};

class City {
public:
    int cityId;
    string name;
    vector<int> roomIds;

    vector<Room> viewRooms() {}
    bool bookingRoom(int RoomId) {}
    bool CheckoutRoom(int RoomId) {}
};

// For the room in any hotel.
class Room {
    int roomId;
    int hotelId;
    RoomType roomType;
    RoomStatus roomStatus;
};

class Hotel {
    int hotelId;
    string hotelName;
    Address adress;
    vector<Room> rooms; // hotel contains the list of rooms.
    float rating;
    Facilities facilities;
};

// a new transaction is created for each booking done by any user
class Transaction {
    int bookingId;
    int userId;
    int hotelId;
    vector<Room> bookedRooms;
    int amount;
    PaymentStatus status_of_payment;
    Date bookingTime;
    Duration duration;
};

class Address {
    string city;
    string pinCode;
    string streetNo;
    string landmark;
};

class Duration {
    Date from;
    Date to;
};

class Facilities {
    bool provides_lift;
    bool provides_power_backup;
    bool provides_hot_water;
    bool provides_breakfast_free;
};

int main() {
    return 0;
}