#include <iostream>
#include <string>

class AVLNode {
public:
  AVLNode(int val = 0) { this->val_ = val; }
  void addNode(AVLNode *newNode);

  int val_;
  AVLNode *left_;
  AVLNode *right_;
  AVLNode *parent_;
};

class AVLTree {
public:
  // 添加节点时树的结构类型
  enum class RotateType {
    OTHER = 0,
    LL = 1,
    LR = 2,
    RL = 3,
    RR = 4,
  };

public:
  // 添加节点
  void addNode(int val);
  // 删除节点
  void removeNode(int val);
  // 前序遍历
  void preorder(AVLNode *apex) const;
  // 中序遍历
  void inorder(AVLNode *apex) const;
  // 根据 *最小不平衡顶点 apex* 判断需要旋转的类型
  AVLTree::RotateType rotateType(AVLNode *apex);

  AVLNode *root() { return this->root_; }

private:
  // 是否为左子节点
  bool isLeft(AVLNode *node);
  // 计算平衡因子（这里定义为左子树深度 - 右子树深度）
  int getBF(AVLNode *node);
  // 左旋
  void leftRotate(AVLNode *oldApex);
  // 右旋
  void rightRotate(AVLNode *oldApex);
  // 获得删除节点的类型，判断子节点个数
  int delType(AVLNode *node);
  // 计算该节点作为顶点时树的深度
  int depth(AVLNode *apex);
  // 不平衡点调整
  void adjust(AVLNode *apex);
  // 查询指定 key 的节点
  AVLNode *findAVLNode(int val);
  // 查找子树中的后继节点，即比当前节点值大的最小值(在该节点同时有左右孩子的前提下)
  AVLNode *findSussessor(AVLNode *node);
  // 判断树是否平衡
  bool isBalance(AVLNode *apex);
  // 根据插入节点，回溯找到最小不平衡树
  AVLNode *findMinNotBalance(AVLNode *leaf);

  AVLNode *root_;
};