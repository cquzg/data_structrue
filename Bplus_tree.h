#include "Bplus_node.h"
using namespace std;

class Bplus_tree {
    private:
    node* root;

    void m_insert(node* cur, int k, int v);

    void m_delete(node* cur, int k, int v);

    Leaf_node* m_find(node* cur, int k);

    int m_find_replaceOfkey(node* cur, int k);

    public:
    Bplus_tree() {
        root = nullptr;
    }
    ~Bplus_tree() {

    }

    void Inite();

    void Insert(int k, int v);

    void Delete(int k, int v);

    void Find(int k_l, int k_r);

};
