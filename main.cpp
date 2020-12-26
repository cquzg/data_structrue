#include "Bplus_tree.h"
using namespace std;

int main()
{
    Bplus_tree* tree = new Bplus_tree();
    tree->Inite();
    tree->Insert(1,2);

    return 0;
}
