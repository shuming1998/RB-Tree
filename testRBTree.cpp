#include "RBTree.h"

int main() {
  RBTree *tree = new RBTree();
  std::vector<int> ids = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 1};
  for (int i = 0; i < ids.size(); ++i) {
    tree->addNode(ids[i]);
  }
  std::cout << "========\n";
  tree->preOrder(tree->root());
  return 0;
}