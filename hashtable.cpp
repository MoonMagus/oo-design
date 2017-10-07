#include <vector>
#include <list>
using namespace std;

struct Node {
    int key;
    int value;
    Node() {}
    Node(int _key, int _value) : key(_key), value(_value) {}
};

class HashTable {
private:
    int size;
    vector<list<Node>>& table;

    // Use directive mode method.
    int hashMethod(int key) {
        return key % size;
    }

public:
    HashTable(int table_size = 1) : size(table_size), table(vector<list<Node>>(size)) {}

    void insert(int key, int value) {
        int pos = hashMethod(key);
        for (auto it = table[pos].begin(); it != table[pos].end(); ++it) {
            if (it->key == key) {
                it->value = value;
                return;
            }
        }

        table[pos].push_back(Node(key, value));
    }

    int get(int key) {
        int pos = hashMethod(key);
        for (auto it = table[pos].begin(); it != table[pos].end(); ++it) {
            if (it->key == key) {
                return it->value;
            }
        }

        throw exception("Value not exist in current hash table");
    }

    void remove(int key) {
        int pos = hashMethod(key);
        for (auto it = table[pos].begin(); it != table[pos].end(); ++it) {
            if (it->key == key) {
                table[pos].erase(it);
                return;
            }
        }

        throw exception("Value not exist in hash table");
    }
};

int main() {
    return 0;
}