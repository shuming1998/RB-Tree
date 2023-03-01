#include "avltree.h"
#include <vector>

/*
 * AVL树插入删除可视化网站
 * https://www.cs.usfca.edu/~galles/visualization/AVLtree.html
*/

int main() {
  AVLTree *tree = new AVLTree();
  std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

  for (int i = 0; i < vec.size(); ++i) {
    tree->addNode(vec[i]);
  }

  tree->preorder(tree->root());
  std::cout << "\n";
  tree->removeNode(9);
  tree->preorder(tree->root());
  std::cout << "\n";
  tree->removeNode(10);
  tree->preorder(tree->root());
  std::cout << "\n";
  tree->removeNode(11);
  tree->preorder(tree->root());
  std::cout << "\n";
  return 0;
}

