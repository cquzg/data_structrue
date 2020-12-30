#include "Bplus_tree.h"
using namespace std;

int main()
{
    Bplus_tree* tree = new Bplus_tree();
    tree->Inite();

    for (int i = 1; i <= 100; i++) {
        tree->Insert(i, i + 1);
    }

    tree->Delete(7, 8);
    tree->Find(1, 100);
    return 0;
}
