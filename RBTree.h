#include <string>
#include <iostream>
#include <sstream>
#include <vector>

struct RBNode {
  RBNode(int id) { this->id_ = id; }
  std::string toString();
  void addNode(RBNode *node);

  int id_ = 0;                // 节点值
  RBNode *left_ = nullptr;    // 左孩子
  RBNode *right_ = nullptr;   // 右孩子
  RBNode *parent_ = nullptr;  // 父亲
  bool isBlack_ = false;      // 是否黑色节点
};

class RBTree {
public:
  enum class InsertType {
    OTHER = 0,
    LL = 1,
    LR = 2,
    RR = 3,
    RL = 4,
  };

public:
  // 返回根节点
  RBNode *root() { return root_; }
  // 添加新节点
  void addNode(int id);
  // 前序遍历，用于判断添加节点后是否正确
  void preOrder(RBNode *node);


private:
  // 判断是否为左子节点，用于左旋右旋
  bool isLeftNode(RBNode *node);
  // 添加节点后调整
  void adjustAfterAddNode(RBNode *leafNode);
  // 节点左旋
  void leftRotate(RBNode *oldNode);
  // 节点右旋
  void rightRotate(RBNode *oldNode);
  // 判断该节点添加后属于哪种情况 LL ? LR ? RL ? RR ? NORMAL
  InsertType getRotateType(RBNode *node);
  RBNode *root_ = nullptr;
};



