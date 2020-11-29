#include "Bplus_tree.h"
using namespace std;

void Bplus_tree::m_insert(node* cur, long long k, int va) {
    if (cur->type() == LEAF) {
        ((Leaf_node*)cur)->Insert(k, va);

        if (cur->node_has_full()) {
            t_pointer = ((Leaf_node*)cur)->split();
            t_key = cur->up_key;

            cur->up_key = INF;

            //((Leaf_node*)t_pointer)->next = ((Leaf_node*)cur)->next;
            ((Leaf_node*)t_pointer)->set_next(((Leaf_node*)cur)->get_next());
            //((Leaf_node*)cur)->next = ((Leaf_node*)t_pointer);
            ((Leaf_node*)cur)->set_next((Leaf_node*)t_pointer);
        }
    }
    else {
        m_insert(((Internal_node*)cur)->find_next_pointer(k), k, va);

        if (t_pointer != nullptr && t_key != INF) {
            ((Internal_node*)cur)->Insert(t_key, t_pointer);
            t_key = INF, t_pointer = nullptr;

            if (cur->node_has_full()) {
                t_pointer = ((Internal_node*)cur)->split();
                t_key = cur->up_key;

                cur->up_key = INF;
            }
        }
    }
}

void Bplus_tree::Insert(long long k, int va) {
    if (root == nullptr) {
        root = new Leaf_node();
    }
    m_insert(root, k, va);

    if (t_pointer != nullptr && t_key != INF) {
        Internal_node* tp = new Internal_node();

        tp->key[0] = t_key;
        tp->set_node(0, root);
        //tp->pointer[0] = root;
        tp->set_node(1, t_pointer);
        //tp->pointer[1] = t_pointer;
        tp->key_number++;

        root = tp;
    }
}

Leaf_node* Bplus_tree::m_find(node* cur, long long k) {
    if (cur->type() == LEAF) return ((Leaf_node*)cur);
    else return m_find(((Internal_node*)cur)->find_next_pointer(k), k);
}

void Bplus_tree::range_find(long long L, long long R) {
    if (L > R) swap(L, R);
    Leaf_node* l_node = m_find(root, L);
    Leaf_node* r_node = m_find(root, R);

    cout << "------------Query result------------------" << endl;
    cout << "id" << "           " << "key" << endl;
    if (l_node == r_node) {
        print(l_node, L, R);
    }
    else {
        print(l_node, L, R);
        l_node = l_node->get_next();
        while(l_node != r_node) {
            print(l_node, L, R);
            l_node = l_node->get_next();
        }
        print(l_node, L, R);
    }
    cout << "------------------------------------------" << endl;
}

void Bplus_tree::print(Leaf_node* cur, long long L, long long R) {
    for (int i = 0; i < cur->key_number; i++) {
        long long x = cur->key[i];
        if (x >= L && x <= R) cout << cur->get_value(i) << "           " << x << endl;
    }
}

void Bplus_tree::Build(int n, pair<long long, int> data[]) {

    for (int i = 0; i < n; i++) Insert(data[i].first, data[i].second);
}


