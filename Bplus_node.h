#include "CONST_SHARE.h"
#include <iostream>
#include <string.h>
using namespace std;

/*
�ܲ��ܽ� ������cnt �� �洢��Ԫ �󶨣��������Ա����cnt��ֵ ��
*/

class node {
    private:
    int cnt, key[MAX];
    NODE_TYPE m_type;

    public:
    node() {
        cnt = 0;
        memset(key, NON, sizeof(key));
    }
    virtual ~node() {

    }

    // �������һ�� error ��
    void Error(int line) {
        cout << line << " " << "There is a error, please check >_< " << endl;
    }

    int Size() {
        return cnt;
    }

    void cnt_increase() {
        if (cnt >= MAX) {
            Error(__LINE__);
            return;
        }
        cnt ++;
    }

    void cnt_decrease() {
        if (cnt <= 0) {
            Error(__LINE__);
            return;
        }
        cnt --;
    }

    void set_key(int id, int x) {
        if (id < 0 || id >= MAX) {
            Error(__LINE__);
            return;
        }
        key[id] = x;
    }

    int get_key(int id) {
        if (id < 0 || id >= MAX) {
            Error(__LINE__);
            return -1;
        }
        return key[id];
    }

    void set_type(NODE_TYPE te) {
        m_type = te;
    }

    NODE_TYPE Type() {
        return m_type;
    }

    // ��һ�����ڻ��ߵ��� k ��λ�ã����������k��С��k���k���м�
    int Pos(int k) {
        int pos = cnt;
        int n = cnt;
        for (int i = 0; i < n; i++) if (key[i] >= k) {
            pos = i;
            break;
        }
        return pos;
    }

    // �ڵ���ѵ�ʱ���м�λ�õļ����뵽 ���ڵ�
    int get_middle_key() {
        int n = Size();
        return get_key(n >> 1);
    }

};

/*
Ҷ�ӽڵ���ڲ��ڵ��ڷ���ʱ����Ϊ��һ��

*/

class Internal_node : public node{
    private:
    node* pointer[MAX];

    public:
    Internal_node() {
        set_type(INTERNAL);
        for (int i = 0; i < MAX; i++) pointer[i] = nullptr;
    }
    ~Internal_node() {

    }

    void set_pointer(int id, node* p) {
        if (id < 0 || id > Size()) {
            Error(__LINE__);
            return;
        }
        pointer[id] = p;
    }

    node* get_pointer(int id) {
        if (id < 0 || id > Size()) {
            Error(__LINE__);
            return nullptr;
        }
        return pointer[id];
    }

    // �Ҳ���
    // ����ļ�λ��Ϊ i, �����ָ��λ��Ϊ j, ��: j = i + 1;
    void Insert(int k, node* p) {
        int pos = Pos(k);
        for (int i = Size() - 1; i >= pos; i--) {
            set_key(i + 1, get_key(i));
            set_pointer(i + 2, get_pointer(i + 1));
        }
        set_key(pos, k);
        set_pointer(pos + 1, p);
        cnt_increase();
    }

    Internal_node* Split() {
        Internal_node* new_node = new Internal_node();
        int tot = 0, n = Size();
        // ż�����ڵ�
        int i = MAX_KEY_NUMBER / 2;

        set_key(i++, -1);   // Ӧ��û����ɣ�
        cnt_decrease();

        while(i < n) {
            new_node->set_key(tot, get_key(i));
            new_node->set_pointer(tot, get_pointer(i));
            new_node->cnt_increase();

            set_key(i, NON);
            set_pointer(i, nullptr);
            cnt_decrease();

            i++, tot++;
        }
        new_node->set_pointer(tot, get_pointer(n));
        set_pointer(n, nullptr);

        return new_node;
    }

    // ����ʹ�õ�λ��Ҫ�� NON ���� nullptr
    // Ĭ�Ϻϲ� ���ֵܽڵ�
    void Merge(Internal_node* brother_node, int x) {
        int n = brother_node->Size();
        int m = Size();

        set_key(m++, x);
        cnt_increase();

        for (int i = 0; i < n; i++) {
            set_key(m, brother_node->get_key(i));
            set_pointer(m++, brother_node->get_pointer(i));
            cnt_increase();
        }
        set_pointer(m, brother_node->get_pointer(n));

        delete(brother_node);
    }

    // ��ɾ��
    // ɾ���ļ�λ��Ϊ i, ɾ����ָ��λ��Ϊ j, ��: j = i + 1;
    void Delete(int pos) {
        int n = Size();
        for (int i = pos; i < n; i++) {
            set_key(i, get_key(i + 1));
            set_pointer(i + 1, get_pointer(i + 2));
        }
        cnt_decrease();
    }

    void right_shift_one_position() {
        int n = Size();
        for (int i = n; i > 0; i--) {
            set_key(i, get_key(i - 1));
            set_pointer(i + 1, get_pointer(i));
        }
        set_pointer(1, get_pointer(0));
    }

    void left_shift_one_position() {
        int n = Size();
        for (int i = 0; i < n; i++) {
            set_key(i, get_key(i + 1));
            set_pointer(i, get_pointer(i + 1));
        }
        cnt_decrease();
    }

    void borrow_from_left_brother(Internal_node* left_brother, int x) {
        right_shift_one_position();
        int n = left_brother->Size();

        set_key(0, x);
        set_pointer(0, left_brother->get_pointer(n));
        cnt_increase();

        left_brother->Delete(n - 1);
    }

    void borrow_from_right_brother(Internal_node* right_brother, int x) {
        int n = Size();

        set_key(n, x);
        set_pointer(n + 1, right_brother->get_pointer(0));
        cnt_increase();

        right_brother->left_shift_one_position();
    }
};

class Leaf_node: public node {
    private:
    int value[MAX];
    Leaf_node* next;

    public:
    Leaf_node() {
        next = nullptr;
        set_type(LEAF);
        memset(value, -1, sizeof(value));
    };
    ~Leaf_node() {

    }

    void set_value(int id, int x) {
        if (id < 0 || id >= MAX) {
            Error(__LINE__);
            return;
        }
        value[id] = x;
    }

    int get_value(int id) {
        if (id < 0 || id >= Size()) {
            Error(__LINE__);
            return -1;
        }
        return value[id];
    }

    void set_next(Leaf_node* np) {
        next = np;
    }

    Leaf_node* get_next() {
        return next;
    }



    void Insert(int k, int va) {
        int pos = Pos(k);
        for (int i = Size() - 1; i >= pos; i--) {
            set_key(i + 1, get_key(i));
            set_value(i + 1, get_value(i));
        }
        set_key(pos, k);
        set_value(pos, va);
        cnt_increase();
    }

    Leaf_node* Split() {
        Leaf_node* new_node = new Leaf_node();
        int tot = 0, n = Size();

        //ż�����ڵ�
        int i = MAX_KEY_NUMBER / 2;

        //set_key(i, NON);
        //set_value(i++, NON);
        //cnt_decrease();

        while(i < n) {
            new_node->set_key(tot, get_key(i));
            new_node->set_value(tot, get_value(i));
            new_node->cnt_increase();

            set_key(i, NON);
            set_value(i, NON);
            cnt_decrease();

            i++, tot++;
        }

        return new_node;
    }

    void Merge(Leaf_node* brother_node) {
        int n = brother_node->Size();
        int m = Size();

        for (int i = 0; i < n; i++) {
            set_key(m, brother_node->get_key(i));
            set_value(m++, brother_node->get_value(i));
            cnt_increase();
        }

        delete(brother_node);
    }

    void Delete(int pos) {
        int n = Size();
        for (int i = pos; i < n; i++) {
            set_key(i, get_key(i + 1));
            set_value(i, get_value(i + 1));
        }
        cnt_decrease();
    }

    void right_shift_one_position() {
        int n = Size();
        for (int i = n; i > 0; i--) {
            set_key(i, get_key(i - 1));
            set_value(i, get_value(i - 1));
        }
    }

    void left_shift_one_positon() {
        int n = Size();
        for (int i = 0; i < n; i++) {
            set_key(i, get_key(i + 1));
            set_value(i, get_value(i + 1));
        }
        cnt_decrease();
    }

    void borrow_from_left_brother(Leaf_node* left_brother) {
        right_shift_one_position();
        int n = left_brother->Size();

        set_key(0, left_brother->get_key(n - 1));
        set_value(0, left_brother->get_value(n - 1));
        cnt_increase();

        left_brother->Delete(n - 1);
    }

    void borrow_from_right_brother(Leaf_node* right_brother) {
        int n = Size();

        set_key(n, right_brother->get_key(0));
        set_value(n, right_brother->get_value(0));
        cnt_increase();

        right_brother->left_shift_one_positon();
    }

};
