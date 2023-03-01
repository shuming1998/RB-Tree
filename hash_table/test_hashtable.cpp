#include "hashtable.h"
#include <string>
#include <time.h>

class Emp {
public:
  Emp() = default;
  Emp(int empno, std::string empName, std::string empAddress) {
    empno_ = empno;
    empName_ = empName;
    empAddress_ = empAddress;
  }

  int empno() const { return empno_; }
  std::string const empName() { return empName_; }
  std::string const empAddress() { return empAddress_; }

  std::string toString() {
    std::stringstream ss;
    ss << "Emp{" << empno_ << "," << empName_ << "," << empAddress_ << "}";
    return ss.str();
  }

  friend std::ostream &operator<<(std::ostream &output, Emp &e){
    output << "Emp{" << e.empno_ << "," << e.empName_ << "," << e.empAddress_ << "}";
    return output;
  }

private:
  int empno_;
  std::string empName_;
  std::string empAddress_;
};



int main() {
  HashTable<int, Emp> *table = new HashTable<int, Emp>(4);
  Emp emp1 = Emp(1001, "武大", "美国");
  Emp emp2 = Emp(1002, "武二", "日本");
  Emp emp3 = Emp(1003, "武三", "中国");
  Emp emp4 = Emp(1004, "王一", "澳大利亚");
  Emp emp5 = Emp(1005, "王二", "法国");
  Emp emp6 = Emp(1006, "王三", "俄罗斯");
  Emp emp7 = Emp(1007, "张一", "泰国");
  Emp emp8 = Emp(1008, "张二", "韩国");
  Emp emp9 = Emp(1009, "张三", "朝鲜");
  Emp emp10 = Emp(1010, "李一", "梵蒂冈");
  Emp emp11 = Emp(1011, "李二", "维京");
  table->put(1001, emp1);
  table->put(1002, emp2);
  table->put(1003, emp3);
  table->put(1004, emp4);
  table->put(1005, emp5);
  table->put(1006, emp6);
  table->put(1007, emp7);
  table->put(1008, emp8);
  table->put(1009, emp9);
  table->put(1010, emp10);
  table->put(1011, emp11);
  table->showHashTable();

  auto people = table->findNode(1005);
  table->removeNode(people->val().empno());
  std::cout << "\n\n==========remove==========\n";
  table->showHashTable();
  std::cout << "\n";
  return 0;
}