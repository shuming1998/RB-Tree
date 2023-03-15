#include "avltree.h"

void AVLNode::addNode(AVLNode *newNode) {
  if (newNode->val < this->val) {
    if (!this->left) {
      this->left = newNode;
      newNode->parent = this;
    } else {
      this->left->addNode(newNode);
    }
  }
  if (newNode->val > this->val) {
    if (!this->right) {
      this->right = newNode;
      newNode->parent = this;
    } else {
      this->right->addNode(newNode);
    }
  }
}

void AVLTree::addNode(int val) {
  AVLNode *newNode = new AVLNode(val);
  if (!root_) {
    root_ = newNode;
  } else {
    root_->addNode(newNode);
    auto node = findMinNotBalance(newNode);
    // 根据最小不平衡点调整树
    adjust(node);
  }
}

void AVLTree::removeNode(int val) {
  AVLNode *delNode = findAVLNode(val);
  AVLNode *successorNode = nullptr;

  // 删除的节点有双子节点，用后继节点作为替代节点
  if (delType(delNode) == 2) {
    successorNode = findSussessor(delNode);
    delNode->val = successorNode->val;
    delNode = successorNode;
  }

  // 删除的节点有单子节点，用其叶子节点作为替代节点
  if (delType(delNode) == 1) {
    successorNode = delNode->left ? delNode->left : delNode->right;
    delNode->val = successorNode->val;
    delNode = successorNode;
  }

  // 删除的节点是叶子节点，先断开该节点与父节点的关系，再删除、调整
  if (delType(delNode) == 0) {
    AVLNode *parent = delNode->parent;
    if (isLeft(delNode)) {
      parent->left = nullptr;
    } else {
      parent->right = nullptr;
    }
    delNode->parent = nullptr;
    delete delNode;
    delNode = nullptr;
    // 删除叶子节点后，找到最小不平衡点，进行平衡调整
    AVLNode *minNbNode = findMinNotBalance(parent);
    adjust(minNbNode);
  }
}

void AVLTree::preorder(AVLNode *apex) const {
  if (!apex) {
    std::cout << "nil->";
    return;
  }
  std::cout << apex->val << "->";
  preorder(apex->left);
  preorder(apex->right);
}

void AVLTree::inorder(AVLNode *apex) const {
  if (!apex) {
    std::cout << "nil->";
    return;
  }
  preorder(apex->left);
  std::cout << apex->val << "->";
  preorder(apex->right);
}

int AVLTree::depth(AVLNode *apex) {
  if (!apex) {
    return 0;
  }
  return std::max(depth(apex->left), depth(apex->right)) + 1;
}

bool AVLTree::isBalance(AVLNode *apex) {
  if (!apex) {
    return true;
  }
  bool is = abs(depth(apex->left) - depth(apex->right)) <= 1
            && isBalance(apex->left)
            && isBalance(apex->right);
  return is;
}

AVLNode *AVLTree::findMinNotBalance(AVLNode *leaf) {
  AVLNode *temp = leaf;
  while (temp) {
    if (!isBalance(temp)) {
      break;
    }
    temp = temp->parent;
  }
  // 如果返回为 nullptr，说明没有不平衡树
  return temp;
}

void AVLTree::leftRotate(AVLNode *oldApex) {
  AVLNode *parent = oldApex->parent;
  AVLNode *newApex = oldApex->right;
  newApex->parent = parent;

  // 先处理父节点和新顶点的关系
  // 如果旋转的不是根节点
  if (parent) {
    // 如果旧顶点是左子树
    if (isLeft(oldApex)) {
      // 现在新顶点接替了旧顶点的位置，成为左子树
      parent->left = newApex;
    } else {
      parent->right = newApex;
    }
  } else {
    // 旧顶点是根节点，将新节点设置为根节点
    root_ = newApex;
  }

  // 再处理新顶点和旧顶点的关系
  AVLNode *newApexLeft = newApex->left;
  oldApex->right = newApexLeft;
  // 如果新顶点的左子树存在，变为旧顶点的右子树
  if (newApexLeft) {
    newApexLeft->parent = oldApex;
  }
  newApex->left = oldApex;
  oldApex->parent = newApex;
}

AVLTree::RotateType AVLTree::rotateType(AVLNode *apex) {
  int apexBf = getBF(apex);
  // L 型，左大右小
  if (apexBf > 0) {
    // 左子树也是左大右小 LL. 添加时 > 0 即可，这里的 = 0 主要用于删除时的情况
    // 比如删除的是右子树叶子节点，且删除后左子树本身是平衡的
    // 此时即使左子树是平衡的，对于不平衡顶点而言，也当作 LL 型
    // 如果当作 LR 型处理也可以，但要进行更多旋转操作，没有必要
    if (getBF(apex->left) >= 0) {
      return RotateType::LL;
    // 左子树是左小右大 LR
    } else {
      return RotateType::LR;
    }
  // R 型，左小右大
  } else {
    // 右子树也是左小右大 RR
    // 此处的 = 0 同理
    if (getBF(apex->right) <= 0) {
      return RotateType::RR;
    // 右子树是左大右小 RL
    } else {
      return RotateType::RL;
    }
  }
  return RotateType::OTHER;
}

void AVLTree::rightRotate(AVLNode *oldApex) {
  AVLNode *parent = oldApex->parent;
  AVLNode *newApex = oldApex->left;
  newApex->parent = parent;

  // 先处理父节点和新顶点的关系
  // 如果旋转的不是根节点
  if (parent) {
    // 如果旧顶点是左子树
    if (isLeft(oldApex)) {
      // 现在新顶点接替了旧顶点的位置，成为左子树
      parent->left = newApex;
    } else {
      parent->right = newApex;
    }
  } else {
    // 旧顶点是根节点，将新节点设置为根节点
    root_ = newApex;
  }

  // 再处理新顶点和旧顶点的关系
  AVLNode *newApexRight = newApex->right;
  oldApex->left = newApexRight;
  // 如果新顶点的左子树存在，变为旧顶点的右子树
  if (newApexRight) {
    newApexRight->parent = oldApex;
  }
  newApex->right = oldApex;
  oldApex->parent = newApex;
}

int AVLTree::delType(AVLNode *node) {
  int childNum = 0;
  if (node->left) {
    ++childNum;
  }
  if (node->right) {
    ++childNum;
  }
  return childNum;
}

void AVLTree::adjust(AVLNode *apex) {
  if (!apex) {
    return;
  }
  RotateType type = rotateType(apex);
  switch (type) {
    case RotateType::LL :
      rightRotate(apex);
      break;
    case RotateType::RR :
      leftRotate(apex);
      break;
    case RotateType::LR :
      leftRotate(apex->left);
      rightRotate(apex);
      break;
    case RotateType::RL :
      rightRotate(apex->right);
      leftRotate(apex);
      break;
    default:
      std::cout << "AVL 树平衡, 无需调整\n";
      break;
  }
}

AVLNode *AVLTree::findAVLNode(int val) {
  AVLNode *temp = root_;
  while (temp) {
    if (temp->val == val) {
      return temp;
    }
    if (temp->val < val) {
      temp = temp->right;
    } else {
      temp = temp->left;
    }
  }

  return nullptr;
}

AVLNode *AVLTree::findSussessor(AVLNode *node) {
  AVLNode *temp = node->right;
  while (temp->left) {
    temp = temp->left;
  }
  return temp;
}

bool AVLTree::isLeft(AVLNode *node) {
  AVLNode *parent = node->parent;
  if (parent && parent->left == node) {
    return true;
  }
  return false;
}

int AVLTree::getBF(AVLNode *node) {
  return depth(node->left) - depth(node->right);
}