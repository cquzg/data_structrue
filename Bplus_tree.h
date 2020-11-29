#include "Bplus_node.h"
using namespace std;

class Bplus_tree {

private:
    long long t_key;
    node *t_pointer, *root;

    void m_insert(node* cur, long long k, int va);

    Leaf_node* m_find(node* cur, long long k);

public:
    Bplus_tree() {
        t_key = INF;
        t_pointer = root = nullptr;
    }
    ~Bplus_tree() {

    }

    void Insert(long long k, int va);

    void range_find(long long L, long long R);

    void Build(int n, pair<long long, int> data[]);

    void print(Leaf_node* cur, long long L, long long R);

};
