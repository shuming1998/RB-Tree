#include <string>
#include <iostream>
#include <sstream>
#include <vector>

struct RBNode {
  RBNode(int id) { this->id = id; }
  // 打印节点
  std::string toString();
  // 添加节点
  void addNode(RBNode *node);

  int id = 0;                // 节点值
  RBNode *left = nullptr;    // 左孩子
  RBNode *right = nullptr;   // 右孩子
  RBNode *parent = nullptr;  // 父亲
  bool isBlack = false;      // 是否黑色节点
};

class RBTree {
public:
  // 红节点的位置
  enum class AddType {
    OTHER = 0,
    LL = 1,
    LR = 2,
    RR = 3,
    RL = 4,
  };

  // 兄弟节点的红色子节点相对父节点的位置
  enum class  RemoveType {
    OTHER = 0,
    LL = 1,   // 兄弟为左子树，红色子节点为左子树
    LR = 2,   // 兄弟为左子树，红色子节点为右子树
    RR = 3,   // 兄弟为右子树，红色子节点为右子树
    RL = 4,   // 兄弟为右子树，红色子节点为左子树
  };

public:
  // 返回根节点
  RBNode *root() { return root_; }
  // 添加新节点
  void addNode(int id);
  // 删除指定节点
  RBNode *removeNode(int id);
  // 前序遍历，用于判断添加节点后是否正确
  void preOrder(RBNode *node);

private:
  // 判断是否为左子节点，用于左旋右旋
  bool isLeft(RBNode *node);
  // 添加节点后调整平衡
  void adjustAfterAdd(RBNode *leafNode);
  // 删除节点后调整平衡
  void adjustAfterRemove(RBNode *node);
  // 节点左旋
  void leftRotate(RBNode *oldNode);
  // 节点右旋
  void rightRotate(RBNode *oldNode);
  // 判断添加节点属于哪种情况
  AddType rotateTypeofAdd(RBNode *node);
  // 判断删除节点属于哪种情况
  RemoveType rotateTypeofRemove(RBNode *node);
  // 查找需要删除的节点
  RBNode *findDelNode(int id);
  // 获取前驱节点，即比当前节点值小的最大值(仅向下查找，并非严格意义的前驱)
  RBNode *predecessor(RBNode *node);
  // 获取后继节点，即比当前节点值大的最小值(仅向下查找，并非严格意义的后继)
  RBNode *successor(RBNode *node);
  // 获取使调整次数最小的替代节点
  RBNode *bestReplaceNode(RBNode *delNode);

  RBNode *root_ = nullptr;
};



