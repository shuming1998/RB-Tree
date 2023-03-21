#include "rbtree.h"

inline std::string RBNode::toString() {
  std::stringstream ss;
  ss << this->id;
  if (isBlack) {
    ss << "(黑)";
  } else {
    ss << "(红)";
  }
  return ss.str();
}

void RBNode::addNode(RBNode *node) {
  if (node->id < this->id) {
    if (!this->left) {
      this->left = node;
      node->parent = this;
    } else {
      this->left->addNode(node);
    }
  } else {
    if (!this->right) {
      this->right = node;
      node->parent = this;
    } else {
      this->right->addNode(node);
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
    newNode->isBlack = true;
  } else {
    root_->addNode(newNode);
    adjustAfterAdd(newNode);
  }
}

/*
 * 1 没找到删除节点直接返回
 * 2 如果删除的是唯一的根节点，root 置空后返回
 * 3 删除有两个子节点的节点
 *    · 找到替换的前驱或后继节点
 *    · 将 delNode 指向替换节点，转为 4 或 5 处理
 * 4 删除只有一个子节点的节点时，删除的节点只可能是黑色的，其子节点只可能是红色
 *    · 将红色子节点的值拷贝到该删除的节点
 *    · 删除该节点转换为删除它的红色叶子节点，即 5.2
 * 5 删除叶子节点
 *    · 删除的是黑色叶子节点，需要对删除的黑色叶子节点进行调整（难）
 *    · 删除红黑两种叶子节点（黑色已经调整，红色可直接删除）
*/
RBNode *RBTree::removeNode(int id) {
  RBNode *delNode = findDelNode(id);
  // 用来返回删除的节点
  RBNode *returnNode = delNode;

  // 1 找不到该删除的节点
  if (!delNode) {
    std::cout << "can't find delete node in RB-Tree!\n";
    return nullptr;
  }

  // 2 该节点为唯一的根节点
  if (delNode == root_ && !root_->left && !root_->right) {
    root_ = nullptr;
    return returnNode;
  }

  RBNode *replaceNode = nullptr;
  // 3 该节点有两个叶子节点，需要置换为前驱或后继节点
  // 将 delNode 指向置换节点，转为 4 或 5
  if (delNode->left && delNode->right) {
    replaceNode = bestReplaceNode(delNode);
    // 将该节点置换为找到的前驱或后继节点
    delNode->id = replaceNode->id;
    // 此时需要删除的节点变为置换的那个节点
    delNode = replaceNode;
  }

  // 4 删除的是有一个红色子节点的黑色节点
  // 这种结构一定是黑色节点带一个红色子节点，不需要再判断颜色
  if (delNode->left && !delNode->right ||
     !delNode->left && delNode->right) {
    replaceNode = delNode->left ? delNode->left : delNode->right;
    // 将该节点置换为红色叶子节点
    delNode->id = replaceNode->id;
    // 此时需要删除的节点变为红色叶子节点，转为 5.2
    delNode = replaceNode;
  }

  // 5 删除的是叶子节点
  // 5.1 删除的是黑色叶子节点，需要进行平衡调整！
  if (delNode->isBlack) {
    adjustAfterRemove(delNode);
  }
  // 5.2 删除叶子节点，如果是黑色叶子节点，此时已经调整过了
  // 将需要删除的叶子节点与父节点断开
  RBNode *parent = delNode->parent;
  if (isLeft(delNode)) {
    parent->left = nullptr;
  } else {
    parent->right = nullptr;
  }
  delNode->parent = nullptr;
  delNode = nullptr;
  return returnNode;
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
void RBTree::adjustAfterAdd(RBNode *leafNode) {
  RBNode *parent = leafNode->parent;
  RBNode *grand = parent->parent;
  // 叔节点在可能在 4-节点 调整后被用到
  RBNode *uncle = isLeft(parent) ? grand->right : grand->left;

  if (!parent) {
    // 插入的是根节点，颜色会在插入时设置，这里其实不需要设置
    // leafNode->isBlack = true;
    return;
  }

  // 2-节点 的添加，不需要调整
  if (parent->isBlack) {
    return;
  }

  // 没有叔节点，或者叔节点为黑色，为 3-节点 的添加
  // 其中叔节点为黑色，说明是4-节点调整后，需要再次递归调整的情况
  if (!uncle || uncle->isBlack) {
    RBTree::AddType type = rotateTypeofAdd(leafNode);
    switch (type) {
      case RBTree::AddType::LL :
        grand->isBlack = false;
        parent->isBlack = true;
        rightRotate(grand);
        break;
      case RBTree::AddType::RR :
        grand->isBlack = false;
        parent->isBlack = true;
        leftRotate(grand);
        break;
      case RBTree::AddType::LR :
        grand->isBlack = false;
        leafNode->isBlack = true;
        leftRotate(parent);
        rightRotate(grand);
        break;
      case RBTree::AddType::RL :
        grand->isBlack = false;
        leafNode->isBlack = true;
        rightRotate(parent);
        leftRotate(grand);
        break;
    }
  } else {
    // 为 4-节点 的添加，需要将父、叔节点变黑，爷节点变红
    // 并且需要将爷节点作为新节点重新调整
    grand->isBlack = false;
    parent->isBlack = true;
    uncle->isBlack = true;

    // 爷节点变为红色后，爷节点的父节点有可能是红的
    // 此时递归调用会进入 3-节点 的调整，爷节点的叔节点此时有可能是黑的
    // 如果一直调整到了根节点，此时根节点会变红
    adjustAfterAdd(grand);
  }

  // 不管如何递归调整，最后一定要确保根节点是黑色
  root_->isBlack = true;
}

/* 删除的一定是黑色叶子节点
 * 1 是根节点，直接删除
 * 2 兄弟为黑色，以删除的是黑色右子节点为例 (L型)，R 型操作与此对称
 *    (1) 兄弟有红色子节点
 *        1) 兄弟有两个红色子节点，可看作 LL 或 LR，但看作 LL 型只需右旋一次
 *        2) 兄弟有红色左子节点，LL 型：
 *            ① 父节点右旋
 *            ② 恢复未删除前个位置的颜色：爷孙变黑，兄变父色
 *        3) 兄弟有红色右子节点，LR 型：
 *            ① 先兄节点左旋，再父节点右旋
 *            ② 恢复未删除前个位置的颜色：侄变父色，父变黑色
 *    (2) 兄弟节点为叶子节点
 *        1) 父节点为红色：
 *            ① 父变黑，兄变红
 *        2) 父节点为黑色：
 *            ① 将兄弟节点变红
 *            ① 将父节点看作是要删除的节点，向上递归调整，直到遇见红色父节点
 *              变为上面的方法 1)，或一直递归到根节点
 * 3 兄弟为红色
 *    1) 兄弟是左子树
 *        ① 将兄弟和其右儿子(当前删除节点的右侄)颜色互换
 *        ② 父节点右旋
 *    2) 兄弟是右子树
 *        ① 将兄弟和其左儿子(当前删除节点的左侄)颜色互换
 *        ② 父节点左旋
*/
void RBTree::adjustAfterRemove(RBNode *node) {
  // 如果是根节点，染黑返回(该节点可能是删除的节点，也可能是需要调整平衡的节点)
  if (root_ == node) {
    // 保险起见，染黑处理一下
    node->isBlack = true;
    return;
  }

  RBNode *parent = node->parent;
  RBNode *brother = isLeft(node) ? parent->right : parent->left;
  // 该调整节点的兄弟是黑色
  if (brother->isBlack) {
    // 根据兄弟节点的旋转类型调整
    RBTree::RemoveType type = rotateTypeofRemove(brother);
    switch (type) {
      // 黑不够，侄来凑
      case RemoveType::LL :
        // 兄染父色，接替父节点
        brother->isBlack = parent->isBlack;
        // 侄染黑色，接替兄弟点
        brother->left->isBlack = true;
        // 右旋
        rightRotate(parent);
        break;
      case RemoveType::RR :
        // 兄染父色，接替父节点
        brother->isBlack = parent->isBlack;
        // 侄染黑色，接替兄弟点
        brother->left->isBlack = true;
        // 左旋
        leftRotate(parent);
        break;
      case RemoveType::LR :
        // NR 侄染父色，父染黑色
        brother->right->isBlack = parent->isBlack;
        parent->isBlack = true;
        // 以兄左旋
        leftRotate(brother);
        // 以父右旋
        rightRotate(parent);
        break;
      case RemoveType::RL :
        // NL 侄染父色，父染黑色
        brother->left->isBlack = parent->isBlack;
        parent->isBlack = true;
        // 以兄右旋
        rightRotate(brother);
        // 以父左旋
        leftRotate(parent);
        break;
      // 兄无子，父红头
      default :
        // 父节点是红色，直接与兄节点交换颜色
        if (!parent->isBlack) {
          parent->isBlack = true;
          brother->isBlack = false;
        } else {
          // 父节点是黑色，先将兄弟节点变红
          brother->isBlack = false;
          // 将父节点作为新的删除节点(并不真的删除)向上递归，直到遇到红色节点或者递归到根节点
          adjustAfterRemove(parent);
        }
        break;
    }
  // 该删除节点的兄弟是红色
  } else {
    // 兄弟红，旋黑中；随父侄，黑变红
    // 此时必有两个侄节点，且侄节点和父节点颜色都为黑
    // 如果删除的是父节点的左子节点
    if (isLeft(node)) {
      // 兄弟节点和其左子节点交换颜色
      brother->isBlack = true;
      brother->left->isBlack = false;
      // 以父节点左旋，此时兄弟节点变为黑色父节点
      leftRotate(parent);
    // 如果删除的是父节点的右子节点
    } else {
      // 兄弟节点和其右子节点交换颜色
      brother->isBlack = true;
      brother->right->isBlack = false;
      // 以父节点右旋，此时兄弟节点变为黑色父节点
      rightRotate(parent);
    }
  }
}

inline RBTree::AddType RBTree::rotateTypeofAdd(RBNode *node) {
  RBNode *parent = node->parent;
  // 插入的节点为空或为根节点
  if (!node || !parent) {
    return AddType::OTHER;
  }

  // 插入的节点为红色，且父节点也为红色，为 3-节点添加
  if (!node->isBlack && !parent->isBlack) {
    // 父节点为左子树 => L型
    if (isLeft(parent)) {
      // 该插入的节点为左子树 => LL
      if (isLeft(node)) {
        return AddType::LL;
      } else {
        return AddType::LR;
      }
    } else {
      // 父节点为右子树 => R型
      // 该插入的节点为左子树 => RL
      if (isLeft(node)) {
        return AddType::RL;
      } else {
        return AddType::RR;
      }
    }
  }

  return AddType::OTHER;
}

RBTree::RemoveType RBTree::rotateTypeofRemove(RBNode *brother) {
  if (isLeft(brother)) {
    if (brother->left && !brother->left->isBlack) {
      return RemoveType::LL;
    }
    if (brother->right && !brother->right->isBlack) {
      return RemoveType::LR;
    }
  } else {
    if (brother->right && !brother->right->isBlack) {
      return RemoveType::RR;
    }
    if (brother->left && !brother->left->isBlack) {
      return RemoveType::RL;
    }
  }
  // 兄弟没有子节点，或者子节点是黑色的
  return RemoveType::OTHER;
}

inline bool RBTree::isLeft(RBNode *node) {
  RBNode *parent = node->parent;
  if (parent && parent->left == node) {
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
  preOrder(node->left);
  preOrder(node->right);
  std::cout << '\n';
}

/*       上层的 ⚪ 节点左旋
        |                               |
      ⚪                           ⭕
      /  \      ==>             /  \
  ⚪   ⭕                  ⚪  ⚪
           /  \                 /  \
       🔺   ⚪        ⚪   🔺
*/
void RBTree::leftRotate(RBNode *oldNode) {
  RBNode *parent = oldNode->parent;
  RBNode *newNode = oldNode->right;
  newNode->parent = parent;
  // 判断旋转的旧顶点是否为根节点(根节点的父节点为空指针)
  if (parent) {
    // 判断一下旧顶点原来是其父节点的左孩子还是右孩子
    if (isLeft(oldNode)) {
      parent->left = newNode;
    } else {
      parent->right = newNode;
    }
  } else {
    // 该顶点为根节点，根节点直接替换为新顶点
    root_ = newNode;
  }
  // 新顶点的左子树变为旧顶点的右子树
  oldNode->right = newNode->left;
  if (newNode->left) {
    // 新顶点的左子树如果存在，其父亲变为旧顶点
    newNode->left->parent = oldNode;
  }
  // 新顶点的左孩子变为旧顶点
  newNode->left = oldNode;
  // 旧顶点的父亲变为新顶点
  oldNode->parent = newNode;
}

/*          上层的 ⚪ 节点右旋
            |                         |
          ⚪                     ⭕
          /  \        ==>     /  \
      ⭕  ⚪             ⚪ ⚪
      /  \                           /  \
  ⚪   🔺                  🔺   ⚪
*/
void RBTree::rightRotate(RBNode *oldNode) {
  RBNode *parent = oldNode->parent;
  RBNode *newNode = oldNode->left;

  newNode->parent = parent;
  // 判断旋转的旧顶点是否为根节点(根节点的父节点为空指针)
  if (parent) {
    // 判断一下旧顶点原来是其父节点的左孩子还是右孩子
    if (isLeft(oldNode)) {
      parent->left = newNode;
    } else {
      parent->right = newNode;
    }
  } else {
    // 该顶点为根节点，根节点直接替换为新顶点
    root_ = newNode;
  }

  // 新顶点的右子树变为旧顶点的左子树
  oldNode->left = newNode->right;
  if (newNode->right) {
    // 新顶点的右子树如果存在，其父亲变为旧顶点
    newNode->right->parent = oldNode;
  }
  // 新顶点的右孩子变为旧顶点
  newNode->right = oldNode;
  // 旧顶点的父亲变为新顶点
  oldNode->parent = newNode;
}

RBNode *RBTree::findDelNode(int id) {
  RBNode *temp = root_;
  while (temp) {
    if (temp->id == id) {
      return temp;
    }
    if (temp->id < id) {
      temp = temp->right;
    } else {
      temp = temp->left;
    }
  }
  return nullptr;
}

// 获取前驱节点，即比当前节点值小的最大值节点
RBNode *RBTree::predecessor(RBNode *node) {
  // 先进入左子树
  RBNode *temp = node->left;
  while (temp->right) {
    temp = temp->right;
  }
  return temp;
}

// 获取后继节点，即比当前节点值大的最小值节点
RBNode *RBTree::successor(RBNode *node) {
  // 先进入右子树
  RBNode *temp = node->right;
  while (temp->left) {
    temp = temp->left;
  }
  return temp;
}

/*
* 先找前驱节点，找到以下两种前驱之一就返回：
*   · 红色叶子节点
*   · 有一个左红色叶子节点的黑色节点
*
* 否则返回后继
*/
RBNode *RBTree::bestReplaceNode(RBNode *delNode) {
  // 前驱节点或后继节点一定是叶子节点
  RBNode *predNode = predecessor(delNode);

  // 前驱是红色的叶子节点，对应情况 1.1，删除黑色叶子节点
  if (!predNode->isBlack) {
    return predNode;
  }

  // 前驱节点是黑色的（如果它有左孩子一定是红色的），对应情况 2，删除带有一个子节点的节点
  if (predNode->left) {
    return predNode;
  }

  // 前驱节点中找不到合适的，最后返回后继节点
  // 因为后继节点中仍可能有合适的替代节点
  return successor(delNode);
}
