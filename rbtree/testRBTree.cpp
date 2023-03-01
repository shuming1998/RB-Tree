#include "RBTree.h"

/*
 * 红黑树插入删除可视化网站
 * https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
*/


int main() {
//------------------插入节点后删除------------------
  RBTree *tree = new RBTree();
  std::vector<int> ids = {20, 3, 44, 5, 6, 75, 8, 19, 60, 50, 12, 1};
  for (int i = 0; i < ids.size(); ++i) {
    tree->addNode(ids[i]);
  }
  std::cout << "========\n";
  tree->preOrder(tree->root());

  tree->removeNode(75);
  std::cout << "===delete 75====\n";
  tree->preOrder(tree->root());

  tree->removeNode(60);
  std::cout << "===delete 60====\n";
  tree->preOrder(tree->root());
  return 0;
}