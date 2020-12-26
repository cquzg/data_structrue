#include "Bplus_tree.h"
#include <algorithm>
using namespace std;

void Bplus_tree:: Inite() {
    if (root == nullptr) {
        root = new Leaf_node();
    }
}

void Bplus_tree:: m_insert(node* cur, int k, int v) {
    if (cur->Type() == LEAF) {
        ((Leaf_node*)cur)->Insert(k, v);
        return;
    }
    else {
        int pos = cur->Pos(k);
        node* next_pointer = ((Internal_node*)cur)->get_pointer(pos);
        m_insert(next_pointer, k, v);

        if (next_pointer->Size() > MAX_KEY_NUMBER) {
            int up_key = next_pointer->get_middle_key();
            node* new_node = nullptr;
            if (next_pointer->Type() == LEAF) {
                new_node = ((Leaf_node*)next_pointer)->Split();

                ((Leaf_node*)new_node)->set_next(((Leaf_node*)next_pointer)->get_next());
                ((Leaf_node*)next_pointer)->set_next(((Leaf_node*)new_node));
            }
            else {
                node* new_node = ((Internal_node*)next_pointer)->Split();
            }
            ((Internal_node*)cur)->Insert(up_key, new_node);
        }
    }
}

void Bplus_tree:: Insert(int k, int v) {
    if (root == nullptr) root = new Leaf_node();
    m_insert(root, k, v);
    if (root->Size() > MAX_KEY_NUMBER) {
        node* new_root = new Internal_node();
        node* new_node = nullptr;
        int up_key = root->get_middle_key();

        if (root->Type() == LEAF) {
            new_node = ((Leaf_node*)root)->Split();

            ((Leaf_node*)new_node)->set_next(((Leaf_node*)root)->get_next());
            ((Leaf_node*)root)->set_next(((Leaf_node*)new_node));
        }
        else {
            new_node = ((Internal_node*)root)->Split();
        }

        new_root->set_key(0, up_key);
        new_root->cnt_increase();
        ((Internal_node*)new_root)->set_pointer(0, root);
        ((Internal_node*)new_root)->set_pointer(1, new_node);

        root = new_root;
    }
}

int Bplus_tree:: m_find_replaceOfkey(node* cur, int k) {
    int pos = cur->Pos(k);
    if (cur->Type() == LEAF) {
        return cur->get_key(pos);
    }
    else {
        if (cur->get_key(pos) == k) pos++;
    }
    return m_find_replaceOfkey(((Internal_node*)cur)->get_pointer(pos), k);

}

void Bplus_tree:: m_delete(node* cur, int k, int v) {
    if (cur->Type() == LEAF) {
        int pos = cur->Pos(k);
        if (((Leaf_node*)cur)->get_value(pos) == v) {
            ((Leaf_node*)cur)->Delete(pos);
            return;
        }
    }
    else {
        int pos = cur->Pos(k), flag = 0, replace_key = -2;
        if (cur->get_key(pos) == k) flag = 1;

        node* next_pointer = ((Internal_node*)cur)->get_pointer(pos + flag);
        m_delete(next_pointer, k, v);

        if (next_pointer->Size() * 2 < MAX_KEY_NUMBER) {
            node* left_brother = ((pos + flag) == 0 ? nullptr : ((Internal_node*)cur)->get_pointer(pos + flag - 1));
            node* right_brother = ((pos + flag) == cur->Size() ? nullptr : ((Internal_node*)cur)->get_pointer(pos + flag + 1));

            if (left_brother->Size() * 2 > MAX_KEY_NUMBER) {
                int n = left_brother->Size();
                int down_key = cur->get_key(pos + flag - 1);

                cur->set_key(pos + flag - 1, left_brother->get_key(n - 1));

                /*
                首先判断节点类型，不同节点类型执行的操作稍微不同
                然后判断要删除的键是不是在内部节点出现过
                */
                if (left_brother->Type() == LEAF) {
                    ((Leaf_node*)next_pointer)->borrow_from_left_brother(((Leaf_node*)left_brother));
                }
                else {
                    if (flag) {
                        replace_key = m_find_replaceOfkey(cur, k);

                        if (replace_key == -1) cur->Error(__LINE__);

                        ((Internal_node*)next_pointer)->borrow_from_left_brother(((Internal_node*)left_brother), replace_key);
                    }
                    else {
                        ((Internal_node*)next_pointer)->borrow_from_left_brother(((Internal_node*)left_brother), down_key);
                    }
                }
            }
            else if (right_brother->Size() * 2 > MAX_KEY_NUMBER) {
                int down_key = cur->get_key(pos + flag);

                if (right_brother->Type() == LEAF) {

                    cur->set_key(pos + flag, right_brother->get_key(1));
                    ((Leaf_node*)next_pointer)->borrow_from_right_brother(((Leaf_node*)right_brother));

                    if (flag) {
                        cur->set_key(pos, next_pointer->get_key(0));
                    }
                }
                else {
                    if (flag) {
                        replace_key = m_find_replaceOfkey(cur, k);

                        if (replace_key == -1) cur->Error(__LINE__);

                        cur->set_key(pos + flag, right_brother->get_key(0));
                        ((Internal_node*)next_pointer)->borrow_from_right_brother(((Internal_node*)right_brother), down_key);

                        cur->set_key(pos, replace_key);
                    }
                    else {
                        cur->set_key(pos, right_brother->get_key(0));
                        ((Internal_node*)next_pointer)->borrow_from_right_brother(((Internal_node*)right_brother), down_key);
                    }
                }
            }
            else if (left_brother != nullptr) {
                if (left_brother->Type() == LEAF) {
                    ((Leaf_node*)left_brother)->set_next(((Leaf_node*)next_pointer)->get_next());
                    ((Leaf_node*)left_brother)->Merge(((Leaf_node*)next_pointer));
                }
                else {
                    if (flag) {
                        replace_key = m_find_replaceOfkey(cur, k);
                        ((Internal_node*)left_brother)->Merge(((Internal_node*)next_pointer), replace_key);
                    }
                    else {
                        int down_key = cur->get_key(pos - 1);
                        ((Internal_node*)left_brother)->Merge(((Internal_node*)next_pointer), down_key);
                    }
                }
                ((Internal_node*)cur)->Delete(pos + flag - 1);
            }
            else if (right_brother != nullptr) {
                if (next_pointer->Type() == LEAF) {
                    ((Leaf_node*)next_pointer)->set_next(((Leaf_node*)right_brother)->get_next());
                    ((Leaf_node*)next_pointer)->Merge(((Leaf_node*)right_brother));
                }
                else {
                    if (flag) {
                        replace_key = m_find_replaceOfkey(cur, k);
                        ((Internal_node*)next_pointer)->Merge(((Internal_node*)right_brother), replace_key);
                    }
                    else {
                        int down_key = cur->get_key(pos);
                        ((Internal_node*)next_pointer)->Merge(((Internal_node*)right_brother), down_key);
                    }
                }
                ((Internal_node*)cur)->Delete(pos);
            }
            else {
                cur->Error(__LINE__);
                return;
            }
        }
        else {
            if (flag) {
                replace_key = m_find_replaceOfkey(cur, k);
                cur->set_key(pos, replace_key);
            }
        }

    }
}

void Bplus_tree:: Delete(int k, int v) {
    if (root == nullptr) return;
    if (root->Size() == 0) return;
    m_delete(root, k, v);
    if (root->Size() == 0) {
        if (root->Type() != LEAF) {
            node* t_root = root;
            root = ((Internal_node*)root)->get_pointer(0);
            delete(t_root);
        }
    }
}

Leaf_node* Bplus_tree:: m_find(node* cur, int k) {
    int pos = cur->Pos(k);
    int flag = (cur->get_key(pos) == k);
    if (cur->Type()== LEAF) {
        return (flag ? ((Leaf_node*)cur) : nullptr);
    }
    else {
        return m_find(((Internal_node*)cur)->get_pointer(pos + flag), k);
    }
}

void Bplus_tree:: Find(int k_l, int k_r) {
    //if (k_l > k_r) swap(k_l, k_r);
    Leaf_node* goal = m_find(root, k_l);
    if (goal == nullptr) {
        cout << "There is no such record." << endl;
        return;
    }
    cout << "--------------------------" << endl;
    cout << "key" << "  " << "value" << endl;
    int pos = goal->Pos(k_l);
    while(goal != nullptr) {
        cout << goal->get_key(pos) << " " << goal->get_value(pos) << endl;
        if (goal->get_key(pos) == k_r) break;
        pos++;
        if (pos == goal->Size()) {
            pos = 0;
            goal = goal->get_next();
        }
    }
    cout << "--------------------------" << endl;
}
