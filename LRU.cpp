#include <iostream>
#include <unordered_map>
#include <list>
using namespace std;

class LRUCache {
private:
    struct Node {
        int key;
        int value;
        Node() {}
        Node(int k, int v) :key(k), value(v) {}
    };
    int cache_capacity;
    unordered_map<int, list<Node>::iterator> m;
    list<Node> cache;

public:
    LRUCache(int capacity) {
        cache_capacity = capacity;
    }

    int get(int key) {
        if (m.find(key) != m.end()) {
            cache.splice(cache.begin(), cache, m[key]);
            m[key] = cache.begin();

            return cache.begin()->value;
        }

        return -1;
    }

    void put(int key, int value) {
        if (m.find(key) != m.end()) {
            m[key]->value = value;
            cache.splice(cache.begin(), cache, m[key]);
            m[key] = cache.begin();
        }
        else {
            if (cache_capacity == m.size()) {
                m.erase(cache.end()->key);
                cache.pop_back();
            }
            
            cache.push_front(Node(key, value));
            m[key] = cache.begin();
        }
    }
};

/**
* Your LRUCache object will be instantiated and called as such:
* LRUCache obj = new LRUCache(capacity);
* int param_1 = obj.get(key);
* obj.put(key,value);
*/