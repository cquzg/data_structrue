#include "Bplus_node.h"
using namespace std;

//------------------------------------------------------

void Internal_node::Insert(long long k, node* po) {
    int n = key_number, pos = 0;
    for (int i = n - 1; i >= 0; i--) {
        if (key[i] > k) {
            key[i + 1] = key[i];
            pointer[i + 2] = pointer[i + 1];
        }
        else {
            pos = i + 1;
            break;
        }
    }

    key[pos] = k, pointer[pos + 1] = po;
    key_number++;
}

Internal_node* Internal_node::split() {
    Internal_node* tp = new Internal_node();

    int pos = key_number / 2, t = 0;

    up_key = key[pos];
    key[pos] = INF;

    for (int i = pos + 1; i < key_number; i++) {
        tp->key[t] = key[i];
        tp->pointer[t++] = pointer[i];

        key[i] = INF;
        pointer[i] = nullptr;
    }
    tp->pointer[t] = pointer[key_number];
    pointer[key_number] = nullptr;

    key_number = key_number / 2;
    tp->key_number = t;

    return tp;
}

node* Internal_node::find_next_pointer(long long k) {
    int pos = key_number;
    for (int i = 0; i < key_number; i++) if (key[i] > k) {
        pos = i;
        break;
    }
    return pointer[pos];
}

//----------------------------------------------------------

void Leaf_node::Insert(long long k, int va) {
    int n = key_number, pos = 0;
    for (int i = n - 1; i >= 0; i--) {
        if (key[i] > k) {
            key[i + 1] = key[i];
            value[i + 1] = value[i];
        }
        else {
            pos = i + 1;
            break;
        }
    }

    key[pos] = k, value[pos] = va;
    key_number++;
}

Leaf_node* Leaf_node::split() {
    Leaf_node* tp = new Leaf_node();

    int pos = (key_number + 1) / 2, t = 0;

    up_key = key[key_number / 2];

    for (int i = pos; i < key_number; i++) {
        tp->key[t] = key[i];
        tp->value[t++] = value[i];

        key[i] = INF;
        value[i] = -1;
    }

    key_number = pos;
    tp->key_number = t;

    return tp;
}
