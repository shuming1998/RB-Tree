#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>

template <typename K, typename V>
class HashNode {
public:
  HashNode() = default;
  ~HashNode() = default;
  HashNode(K key, V val) : key_(key), val_(val) {}
  std::string toString() {
    std::stringstream ss;
    ss << "{" << key_ << ": " << val_ << "}";
    return ss.str();
  }

  void setVal(V val) { this->val_ = val; }
  K key() const { return key_; }
  V val() const { return val_; }

  HashNode<K, V> *prev_ = nullptr;
  HashNode<K, V> *next_ = nullptr;

private:
  K key_;
  V val_;
};

template <typename K, typename V>
class HashLinkList {
public:
  HashLinkList(){
    head_ = new HashNode<K, V>();
  }
  ~HashLinkList() = default;

  // 添加或修改结点
  void append(K key, V val) {
    HashNode<K, V> *node = head_;
    HashNode<K, V> *prev = nullptr;

    while (node) {
      if (key == node->key()) {
        node->setVal(val);
        return;
      }
      prev = node;
      node = node->next_;
    }

    // 尾插法，此时 prev 已经到了队尾
    HashNode<K, V> *newNode = new HashNode<K, V>(key, val);
    prev->next_ = newNode;
    newNode->prev_ = prev;
    ++size_;
  }

  // 根据键寻找结点
  HashNode<K, V> *findNode(K key) {
    HashNode<K, V> *node = head_;
    while (node) {
      if (key == node->key()) {
        return node;
      }
      node = node->next_;
    }
    return nullptr;
  }

  // 根据键删除结点
  HashNode<K, V> *removeNode(K key) {
    HashNode<K, V> *node = findNode(key);
    if (!node) {
      std::cout << "can't find key in linklist!\n";
      return nullptr;
    }
    node->prev_->next_ = node->next_;
    node->prev_ = nullptr;
    if (node->next_) {
      node->next_->prev_ = node->prev_;
    }
    --size_;
    return node;
  }

  void showList() {
    HashNode<K, V> *temp = head_->next_;
    while (temp) {
      std::cout << temp->toString() << "-->";
      temp = temp->next_;
    }
  }

  HashNode<K, V> *head_;

private:
  int size_ = 0;
};

template <typename K, typename V>
class HashTable {
public:
  ~HashTable() {
    for (int i = 0; i < table_.size(); ++i) {
      delete table_[i];
    }
  }
  // 初始化数组中的链表
  void init() {
    for (int i = 0; i < table_.size(); ++i) {
      table_[i] = new HashLinkList<K, V>();
    }
  }
  // 计算哈希值
  int getHash(K key) {
    std::hash<K> hashK;
    size_t hashVal = hashK(key);
    return hashVal % size_;
  }

  HashTable(int size = 10) : size_(size) {
    table_.resize(size_);
    init();
  }
  // 将数据放入哈希表
  void put(K key, V val) {
    int hashVal = getHash(key);
    table_[hashVal]->append(key, val);
  }
  // 查找哈希表中的结点
  HashNode<K, V> *findNode(K key) {
    int hashVal = getHash(key);
    return table_[hashVal]->findNode(key);
  }
  // 删除哈希表中的结点
  HashNode<K, V> *removeNode(K key) {
    int hashVal = getHash(key);
    return table_[hashVal]->removeNode(key);
  }
  // 打印哈希表元素
  void showHashTable() {
    for (int i = 0; i < table_.size(); ++i) {
      std::cout << "[" << i << "]: ";
      table_[i]->showList();
    }
  }

private:
  int size_;
  std::vector<HashLinkList<K, V> *> table_;
};