#include "RBTree.h"

inline std::string RBNode::toString() {
  std::stringstream ss;
  ss << this->id_;
  if (isBlack_) {
    ss << "(黑)";
  } else {
    ss << "(红)";
  }
  return ss.str();
}

void RBNode::addNode(RBNode *node) {
  if (node->id_ < this->id_) {
    if (!this->left_) {
      this->left_ = node;
      node->parent_ = this;
    } else {
      this->left_->addNode(node);
    }
  } else {
    if (!this->right_) {
      this->right_ = node;
      node->parent_ = this;
    } else {
      this->right_->addNode(node);
    }
  }
}

void RBTree::addNode(int id) {
  RBNode *newNode = new RBNode(id);
  // 如果根节点为空
  if (!root_) {
    // 此节点插入后即为根节点
    root_ = newNode;
    // 根节点一定为黑色
    newNode->isBlack_ = true;
  } else {
    root_->addNode(newNode);
    adjustAfterAddNode(newNode);
  }
}

/*
 * RBTree <==> 2-3-4-Tree
 * 如果添加的是根节点，变为黑色并返回
 *
 * 如果父节点是黑色，为 2-节点 的添加：
 *    · 不需要调整，直接返回
 *
 * 如果父节点是红色，叔节点是空的或黑色，为 3-节点 的添加：
 *    · 判断 LL/LR/RL/RR 并作出对应调整
 *        · LL 爷节点右旋，右旋后新父节点和原爷节点变色
 *        · LR 先父节点左旋变为 LL，再爷节点右旋
 *        · RR 爷节点左旋，左旋后新父节点和原爷节点变色
 *        · RL 先父节点右旋变为 RR，再爷节点左旋
 *        · 其余情况中，如果为左中右型，不需要调整
 *
 * 如果父、叔节点都是红色，为 4-节点 的添加：
 *    · 父、叔节点变为黑色，爷节点变为红色
 *    · 如果爷节点的父节点也为红色 => 递归调用 让爷节点以上的节点继续旋转变色
*/
void RBTree::adjustAfterAddNode(RBNode *leafNode) {
  RBNode *parent = leafNode->parent_;

  if (!parent) {
    // 插入的是根节点，颜色会在插入时设置，这里其实不需要设置
    // leafNode->isBlack_ = true;
    return;
  }

  // 2-节点 的添加，不需要调整
  if (parent->isBlack_) {
    return;
  }

  RBNode *grandpa = parent->parent_;
  RBNode *uncle = isLeftNode(parent) ? grandpa->right_ : grandpa->left_;

  // 没有叔节点，或者叔节点为黑色，为 3-节点 的添加
  if (!uncle || uncle->isBlack_) {
    RBTree::InsertType type = getRotateType(leafNode);
    switch (type) {
      case RBTree::InsertType::LL :
        grandpa->isBlack_ = false;
        parent->isBlack_ = true;
        rightRotate(grandpa);
        break;
      case RBTree::InsertType::RR :
        grandpa->isBlack_ = false;
        parent->isBlack_ = true;
        leftRotate(grandpa);
        break;
      case RBTree::InsertType::LR :
        grandpa->isBlack_ = false;
        leafNode->isBlack_ = true;
        leftRotate(parent);
        rightRotate(grandpa);
        break;
      case RBTree::InsertType::RL :
        grandpa->isBlack_ = false;
        leafNode->isBlack_ = true;
        rightRotate(parent);
        leftRotate(grandpa);
        break;
    }
  } else {
    // 为 4-节点 的添加，需要将父、叔节点变黑，爷节点变红
    // 并且需要将爷节点作为新节点重新调整

    grandpa->isBlack_ = false;
    parent->isBlack_ = true;
    uncle->isBlack_ = true;
    // 爷节点变为红色后，其父节点有可能是红的
    // 此时递归调用会进入 3-节点 的调整，爷节点的叔节点此时有可能是黑的
    // 如果一直调整到了根节点，此时根节点会变红
    adjustAfterAddNode(grandpa);
  }

  // 不管如何递归调整，最后一定要确保根节点是黑色
  root_->isBlack_ = true;
}

inline RBTree::InsertType RBTree::getRotateType(RBNode *node) {
  RBNode *parent = node->parent_;
  // 插入的节点为空或为根节点
  if (!node || !parent) {
    return InsertType::OTHER;
  }

  // 插入的节点为红色，且父节点也为红色，为 3-节点添加
  if (!node->isBlack_ && !parent->isBlack_) {
    // 父节点为左子树 => L型
    if (isLeftNode(parent)) {
      // 该插入的节点为左子树 => LL
      if (isLeftNode(node)) {
        return InsertType::LL;
      } else {
        return InsertType::LR;
      }
    } else {
      // 父节点为右子树 => R型
      // 该插入的节点为左子树 => RL
      if (isLeftNode(node)) {
        return InsertType::RL;
      } else {
        return InsertType::RR;
      }
    }
  }

  return InsertType::OTHER;
}

inline bool RBTree::isLeftNode(RBNode *node) {
  RBNode *parent = node->parent_;
  if (parent && parent->left_ == node) {
    return true;
  }
  return false;
}

void RBTree::preOrder(RBNode *node) {
  if (!node) {
    std::cout << "nil ";
    return;
  }
  std::cout << node->toString() << "-->";
  preOrder(node->left_);
  preOrder(node->right_);
  std::cout << '\n';
}

/*            节点左旋
      |                      |
     ⚪                     ⭕
    /  \        ==>         /  \
  ⚪   ⭕                 ⚪  ⚪
       /  \               /  \
     🔺   ⚪           ⚪   🔺
*/
void RBTree::leftRotate(RBNode *oldNode) {
  RBNode *parent = oldNode->parent_;
  RBNode *newNode = oldNode->right_;
  newNode->parent_ = parent;
  // 判断旋转的旧顶点是否为根节点(根节点的父节点为空指针)
  if (parent) {
    // 判断一下旧顶点原来是其父节点的左孩子还是右孩子
    if (isLeftNode(oldNode)) {
      parent->left_ = newNode;
    } else {
      parent->right_ = newNode;
    }
  } else {
    // 该顶点为根节点，根节点直接替换为新顶点
    root_ = newNode;
  }
  // 新顶点的左子树变为旧顶点的右子树
  oldNode->right_ = newNode->left_;
  if (newNode->left_) {
    // 新顶点的左子树如果存在，其父亲变为旧顶点
    newNode->left_->parent_ = oldNode;
  }
  // 新顶点的左孩子变为旧顶点
  newNode->left_ = oldNode;
  // 旧顶点的父亲变为新顶点
  oldNode->parent_ = newNode;
}

/*            节点右旋
        |                      |
       ⚪                     ⭕
      /  \        ==>         /  \
     ⭕  ⚪                ⚪   ⚪
    /  \                         /  \
  ⚪   🔺                     🔺   ⚪
*/
void RBTree::rightRotate(RBNode *oldNode) {
  RBNode *parent = oldNode->parent_;
  RBNode *newNode = oldNode->left_;

  newNode->parent_ = parent;
  // 判断旋转的旧顶点是否为根节点(根节点的父节点为空指针)
  if (parent) {
    // 判断一下旧顶点原来是其父节点的左孩子还是右孩子
    if (isLeftNode(oldNode)) {
      parent->left_ = newNode;
    } else {
      parent->right_ = newNode;
    }
  } else {
    // 该顶点为根节点，根节点直接替换为新顶点
    root_ = newNode;
  }

  // 新顶点的右子树变为旧顶点的左子树
  oldNode->left_ = newNode->right_;
  if (newNode->right_) {
    // 新顶点的右子树如果存在，其父亲变为旧顶点
    newNode->right_->parent_ = oldNode;
  }
  // 新顶点的右孩子变为旧顶点
  newNode->right_ = oldNode;
  // 旧顶点的父亲变为新顶点
  oldNode->parent_ = newNode;
}
