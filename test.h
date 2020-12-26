class test {
private:
    int i;
public:
    test() {
        i = 0;
    }
    ~test() {

    }
};

class test1: public test {
private:
    int j;
public:
    test1(): test() {
        j = 1;
    }
    ~test1() {

    }
    int get_j() { return j; }
};


