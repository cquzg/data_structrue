#include<iostream>
using namespace std;

enum NODE_TYPE{ INTERNAL, LEAF};

#define MAX 200
#define MAX_KEY_NUMBER 100
#define INF 1000000000

class node{

public:
    NODE_TYPE m_type;
    bool splited;
    int key_number;
    long long key[MAX];
    long long up_key;


    node() {
        up_key = INF;
        key_number = 0;
        for (int i = 0; i < MAX; i++) key[i] = INF;

        splited = false;
    }
    virtual ~node() {

    }

    bool node_has_full() {
        return (key_number < MAX_KEY_NUMBER ? false : true);
    }

    NODE_TYPE type() {
        return m_type;
    }
    /*
    int Size() {
        return key_number;
    }

    void clear_up_key() {
        up_key = -1;
    }

    void set_splited(bool sp) {
        splited = sp;
    }

    bool has_splited() {
        return splited;
    }

    int get_key(int id) {
        if (id < 0 || id >= key_number) {
            cout << "key id error!" << endl;
            return -1;
        }
        return key[id];
    }

    void set_key(int id, int x) {
        if (id > key_number) {
            cout << "id is not allowed!" << endl;
            return;
        }
        if (id == key_number) key_number++;
        key[id] = x;
    }
    */
};

class Internal_node: public node {

private:
    node* pointer[MAX];
    //long long up_key;   // up to parent node

public:
    Internal_node(): node() {
        m_type = INTERNAL;
        for (int i = 0; i < MAX; i++) pointer[i] = nullptr;
    }
    ~Internal_node() {

    }

    node* get_node(int id) {
        return pointer[id];
    }

    void set_node(int id, node* tp) {
        pointer[id] = tp;
    }

    void Insert(long long k, node* po);

    Internal_node* split();

    node* find_next_pointer(long long k);


    /*
    void set_pointer(int id, node* po) {
        if (id > key_number) {
            cout << "id is not allowed!" << endl;
            return;
        }
        pointer[id] = po;
    }

    node* get_pointer(int id) {
        return pointer[id];
    }
    */
};

class Leaf_node: public node {

private:
    Leaf_node* next;
    int value[MAX];

public:
    Leaf_node(): node() {
        m_type = LEAF;
        next = nullptr;
        for (int i = 0; i < MAX; i++) value[i] = -1;
    }
    ~Leaf_node() {

    }

    Leaf_node* get_next() {
        return next;
    }

    void set_next(Leaf_node* tp) {
        next = tp;
    }

    int get_value(int id) {
        return value[id];
    }

    void set_value(int id, int x) {
        value[id] = x;
    }

    void Insert(long long k, int va);

    Leaf_node* split();
    /*

    void set_value(int id, int va) {
        if (id > key_number) {
            cout << "id is not allowed!" << endl;
            return;
        }
        value[id] = va;
    }

    int get_value(int id) {
        return value[id];
    }
    */
};


