#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_set>
using namespace std;

struct Message {
    int messageId;
    int timestamp;
    string message;
};

enum RequestStatus {
    unread,
    read,
    accept,
    reject
};

struct Request {
    int fromUserId;
    int toUserId;
    int timestamp;
    RequestStatus staus;
};

class User {
public:
    int user_id;
    unordered_map<int, PrivateChat> userId_privateChat;
    unordered_map<int, GroupChat> groupId_groupChat;

    void messageWithUser(Message message, int userId) {}
    void messageWithGroup(Message message, int groupId) {}
    void sendFriendRequest(int userId) {}
    void receiveFriendRequest(int UerId) {}
    void approveFriendRequest(int UserId) {}
    void rejectFriendRequest(int UserId) {}
};

class Chat {
public:
    int chatId;
    vector<int> users;
    vector<string> history;
};

class PrivateChat :public Chat {
public:
    PrivateChat() {}
    PrivateChat(int firstUserId, int secondUserId) {
        users.push_back(firstUserId);
        users.push_back(secondUserId);
    }
};

class GroupChat :public Chat {
public:
    unordered_set<int> friend_ids;
    vector<string> history;
};

int main() {
    return 0;
}
