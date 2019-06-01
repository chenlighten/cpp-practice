#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cstdio>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>

using namespace std;

// 一个构造函数是拷贝构造函数, 当它的第一个参数是一个引用类型,
// 并且其余参数都有默认值.
// 该引用基本上应为const, 尽管可以也定义成非const.
// 
// 这个拷贝构造函数经常被隐式地使用, 故通常不定义成explicit.
// 
// 当没有定义拷贝构造函数时, 编译器综合出一个, 即便已经定义了其他构造函数.
// 这与默认构造函数不同. 当定义了其他构造函数时, 编译器不再综合出默认构造函数.
// 编译器综合的拷贝构造函数构造函数顺序拷贝非static值.

// built-in 类型直接拷贝. 类类型调用它自己的拷贝构造函数拷贝.

class Sales_data {
public:
    Sales_data(const Sales_data &);
private:
    std::string bookNo;
    int units_sold = 0;
    double revenue = 0.0;
};

Sales_data::Sales_data(const Sales_data &org):
    bookNo(org.bookNo), units_sold(org.units_sold), revenue(org.revenue) {}

// 隐式调用非拷贝构造函数传参时发生了什么?
class A {
public:
    A():x(0), y(0) {}
    A(int i): x(i), y(i) {}
    A(const A &a): x(a.x), y(110) {}
    int getx() {return x;}
    int gety() {return y;}
private:
    int x;
    int y;
};

void print_A(A a) {
    printf("x=%d, y=%d\n", a.getx(), a.gety());
}

void how_convert() {
    // 输出5, 5: 直接把构造函数A::A(int)构建的object传给函数
    // 输出5, 110: 先用A::A(int)构建一个object, 再把它传给拷贝构造函数A::A(const A &)再构建一个object传给函数
    print_A(5);
    // 结果:输出5, 5
    // 而如果:
    print_A(A(5));
    // 依然输出5, 5
    // 而如果:
    A a(5);
    print_A(a);
    // 输出5, 110 !!
    A b = a;
    printf("%d\n", b.gety());
    // 无疑是110
    // 甚至可以这样:
    A c = 5;
    print_A(c);
    // 输出5, 110

    ////////////////////////////////////////////////////////////////////////////
    // 如果把A::A(int)声明为explicit和把A::A(const A &)声明为explicit分别导致什么结果?
    // 忘了就试试!
}

/*
class HasPtr {
public:
    HasPtr() {}
    HasPtr(const std::string &s = std::string()):
        ps(new std::string(s)), i(0) {}
    HasPtr(const HasPtr &hp):
        ps(new std::string(*hp.ps)), i(hp.i) {}
    HasPtr &operator=(const HasPtr & hp) {
        ps = new string(*hp.ps);
        i = hp.i;
    }
    ~HasPtr() {
        delete ps;
    }
private:
    std::string *ps;
    int i;
};
*/

// class that act like values
// each object has to have its own copy of the resource that the class manages
// to implement this, we need
// - a copy constructor that copies the resource, not just the pointer
// - a destructor to free the resource
// - a copy-assignment operator to free the object's existing resource and
//   copy the resource from it's right-hand operand
class HasPtr {
    friend void swap(HasPtr &, HasPtr &);
public:
    HasPtr() {}
    HasPtr(const std::string &s = std::string()):
        ps(new std::string(s)), i(0) {}
    HasPtr(const HasPtr &hp):
        ps(new std::string(*hp.ps)), i(0) {}
    HasPtr &operator=(const HasPtr &);
    ~HasPtr() {delete ps;}
private:
    std::string *ps;
    int i;
};

HasPtr &HasPtr::operator=(const HasPtr &hp) {
    /////////////////////////////////////////
    // 当左, 右操作数的ps指向相同的区域时出bug
    if (false) {
        delete ps;
        ps = new std::string(*hp.ps);
        i = hp.i;
        return *this;
    }
    // 正确做法
    else {
        auto newps = new string(*hp.ps);
        delete ps;
        ps = newps;
        i = hp.i;
        return *this;
    }
}

inline void swap(HasPtr &lhp, HasPtr &rhp) {
    using std::swap;
    // 不应用std::swap()而应用swap()
    // 这样编译器会自动为定义了swap()函数的类匹配它自己的swap()函数
    swap(lhp.ps, rhp.ps);
    swap(lhp.i, rhp.i);
}

class HasPtrRef {
public:
    HasPtrRef(const std::string &s = std::string()):
        ps(new std::string(s)), i(0), use(new std::size_t(1)) {}
    HasPtrRef(const HasPtrRef &hpr):
        ps(hpr.ps), i(hpr.i), use(hpr.use) { ++*use; }
    HasPtrRef &operator=(const HasPtrRef &);
    ~HasPtrRef();
private:
    std::string *ps;
    int i;
    std::size_t *use;
};

HasPtrRef::~HasPtrRef() {
    if (--*use == 0) {
        delete ps;
        delete use;
    }
}

HasPtrRef &HasPtrRef::operator=(const HasPtrRef &hpr) {
    ++*hpr.use;
    if (--*use == 0) {
        delete ps;
        delete use;
    }
    ps = hpr.ps;
    i = hpr.i;
    use = hpr.use;
    return *this;
}

class Folder;
class Message {
    friend class Folder;
    friend void swap(Message &, Message &);
public:
    explicit Message(const std::string &s):
        contents(s) {}
    Message(const Message&);
    Message &operator=(const Message &);
    ~Message();
    void save(Folder &);
    void remove(Folder &);
private:
    std::string contents;
    std::set<Folder *> folders;
    void add_to_Folders_of(const Message &);
    void remove_from_Folders();
};

void Message::save(Folder &f) {
    folders.insert(&f);
    f.addMsg(this);
}

void Message::remove(Folder &f) {
    folders.erase(&f);
    f.remMsg(this);
}

void Message::add_to_Folders_of(const Message &msg) {
    for (auto f : msg.folders) {
        f->addMsg(this);
    }
}

void Message::remove_from_Folders() {
    for (auto f : folders) {
        f->remMsg(this);
    }
}

Message::Message(const Message &msg):
    contents(msg.contents), folders(msg.folders)
{
    add_to_Folders_of(msg);
}

Message::~Message() {
    remove_from_Folders();
}

Message &Message::operator=(const Message &rhs) {
    remove_from_Folders();
    contents = rhs.contents;
    folders = rhs.folders;
    add_to_Folders_of(rhs);
    return *this;
}

void swap(Message &lhs, Message &rhs) {
    using std::swap;
    for (auto f : lhs.folders)
        f->remMsg(&lhs);
    for (auto f:rhs.folders)
        f->remMsg(&rhs);
    swap(lhs.folders, rhs.folders);
    swap(lhs.contents, rhs.contents);
    for (auto f : lhs.folders)
        f->addMsg(&lhs);
    for (auto f : rhs.folders)
        f->addMsg(&rhs);
}

class Folder {
public:
    Folder() {}
    void addMsg(Message *msg);
    void remMsg(Message *msg);
private:
    std::set<Message *> messages;
};

void Folder::addMsg(Message *msg) {
    messages.insert(msg);
}

void Folder::remMsg(Message *msg) {
    messages.erase(msg);
}

int main() {
    // vector<int> v = {1, 2, 3, 4, 2, 1, 6, 100, 3, 7};
    // sort(v.begin(), v.end());
    // for (auto i : v)
    //     cout << i << '\t';
    printf("hello, world\n");
    how_convert();
    return 0;
}
