#include "Bplus_tree.h"

using namespace std;

const int maxn = 100000;

Bplus_tree* tree = new Bplus_tree();

int main() {

    pair<long long, int> p[maxn + 10];

    for (int i = 0; i < maxn; i++) p[i].first = i + 1, p[i].second = i + 1;

    tree->Build(maxn, p);

    tree->range_find(100, 200);

    return 0;
}
