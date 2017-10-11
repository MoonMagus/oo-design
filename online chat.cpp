#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

struct Date {};

class Message {
private:
    string content;
    Date timestamp;

public:
    Message(string& _content, Date& _date) {
        content = _content;
        timestamp = _date;
    }

    string getContent() {
        return content;
    }

    Date getDate() {
        return timestamp;
    }
};

class ChatBase {
protected:
    int chatId;
    unordered_set<User> participants;
    vector<Message> messages;

public:
    vector<Message> getMessages() {
        return messages;
    }

    bool addMessage(Message& m) {
        messages.push_back(m);

        return true;
    }

    int getChatId() {
        return chatId;
    }
};

class PrivateChat : public ChatBase {
private:
    User userAnother;

public:
    PrivateChat(User& _userAnother) : userAnother(_userAnother) {}

    User getOtherParticipant() {
        return userAnother;
    }
};

class GroupChat : public ChatBase {
public:
    void addParticipant(User& user) {
        participants.insert(user);
    }

    void removeParticipant(User& user) {
        participants.erase(user);
    }
};

enum RequestStatus {
    unread,
    read,
    accepted,
    rejected
};

class AddRequest {
private:
    User fromUser;
    User toUser;
    Date date;
    RequestStatus status;

public:
    AddRequest(User from, User to, Date _date) {
        fromUser = from;
        toUser = to;
        date = _date;
        status = RequestStatus::unread;
    }

    RequestStatus getStatus() {
        return status;
    }

    void setStatus(RequestStatus _status) {
        status = _status;
    }

    User getFromUser() {
        return fromUser;
    }

    User getToUser() {
        return toUser;
    }

    Date getDate() {
        return date;
    }
};

class User {
private:
    int id;
    string accountName;
    string fullName;
    UserStatus status;
    unordered_map<int, PrivateChat> userId_to_privateChats;
    unordered_map<int, GroupChat> groupId_to_groupChats;
    unordered_map<int, AddRequest> receivedFriendRequests;
    unordered_map<int, AddRequest> sentFriendRequests;
    unordered_map<int, User> contacts;

public:
    User() {}
    User(int _id, string _accountName, string _fullName) : id(_id), accountName(_accountName), fullName(_fullName) {}

    bool sendMessageToPrivateChat(User& toUser, string& content) {
        if (userId_to_privateChats.find(toUser.getUserId()) == userId_to_privateChats.end()) {
            userId_to_privateChats[toUser.getUserId()] = PrivateChat(toUser);
        }
        Message message(content, Date());

        return userId_to_privateChats[toUser.getUserId()].addMessage(message);
    }

    bool sendMessageToGroupChat(int groupId, string content) {
        if (groupId_to_groupChats.find(groupId) != groupId_to_groupChats.end()) {
            Message message(content, Date());

            return groupId_to_groupChats[groupId].addMessage(message);
        }

        return false;
    }

    void setStatus(UserStatus& _status) {
        status = _status;
    }

    UserStatus getStatus() {
        return status;
    }

    bool addContact(User user) {
        if (contacts.find(user.getUserId()) != contacts.end()) {
            return false;
        }
        else {
            contacts[user.getUserId()] = user;

            return true;
        }
    }

    void receivedFriendRequest(AddRequest req) {
        int friendId = req.getFromUser().getUserId();
        if (receivedFriendRequests.find(friendId) == receivedFriendRequests.end()) {
            receivedFriendRequests[friendId] = req;
        }
    }

    void sentFriendRequest(AddRequest req) {
        int friendId = req.getFromUser().getUserId();
        if (sentFriendRequests.find(friendId) == sentFriendRequests.end()) {
            sentFriendRequests[friendId] = req;
        }
    }

    void withdrawFriendRequest(AddRequest req) {
        // If current user is the receiver, then delete request from current user's receive map.
        if (req.getToUser().getUserId() == this->getUserId()) {
            receivedFriendRequests.erase(req.getToUser().getUserId());
        }
        // If current user is the sender, then delete request from current user's send map.
        else if (req.getFromUser().getUserId() == this->getUserId()) {
            sentFriendRequests.erase(req.getToUser().getUserId());
        }
    }

    // What's this mean?
    void requestAddUser(string accountName) {
        UserManager::getInstance()->addUser(*this, accountName);
    }

    void createPrivateChat(PrivateChat& privateChat) {
        User otherUser = privateChat.getOtherParticipant();
        userId_to_privateChats[otherUser.getUserId()] = privateChat;
    }

    void createGroupChat(GroupChat& groupChat) {
        groupId_to_groupChats[groupChat.getChatId()] = groupChat;
    }

    int getUserId() {
        return id;
    }

    string getAccountName() {
        return accountName;
    }

    string getFullName() {
        return fullName;
    }
};

enum UserStatusType {
    offline,
    away,
    idle,
    available,
    busy
};

class UserStatus {
private:
    UserStatusType type;
    string message;

public:
    UserStatus() {}
    UserStatus(UserStatusType _type, string _message) :type(_type), message(_message) {}

    UserStatusType getStatusType() {
        return type;
    }

    string getMessage() {
        return message;
    }
};

/* UserManager serves as the central place for the core user actions. */
class UserManager {
private:
    static UserManager* instance;
    unordered_map<int, User> usersById;
    unordered_map<string, User> usersByAccountName;
    unordered_map<int, User> onlineUsers;

public:
    static UserManager* getInstance() {
        if (instance == nullptr) {
            instance = new UserManager();
        }

        return instance;
    }

    void addUser(User& fromUser, string& toAccountName) {
        User toUser = usersByAccountName[toAccountName];
        AddRequest req(fromUser, toUser, Date());
        toUser.receivedFriendRequest(req);
        fromUser.sentFriendRequest(req);
    }

    void approveAddRequest(AddRequest req) {
        req.setStatus(RequestStatus::accepted);
        User from = req.getFromUser();
        User to = req.getToUser();
        from.addContact(to);
        to.addContact(from);
    }

    void rejectAddRequest(AddRequest req) {
        req.setStatus(RequestStatus::rejected);
        User from = req.getFromUser();
        User to = req.getToUser();
        from.withdrawFriendRequest(req);
        to.withdrawFriendRequest(req);
    }

    void userSignedOn(string accountName) {
        User user = usersByAccountName[accountName];
        if (&user != nullptr) {
            user.setStatus(UserStatus(UserStatusType::available, ""));
            onlineUsers[user.getUserId()] = user;
        }
    }

    void userSignedOff(string accountName) {
        User user = usersByAccountName[accountName];
        if (&user != nullptr) {
            user.setStatus(UserStatus(UserStatusType::offline, ""));
            onlineUsers.erase(user.getUserId());
        }
    }
};