#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cstdio>
#include <fstream>
#include <map>
#include <set>
#include <sstream>

using namespace std;

class StrBlobPtr;

class StrBlob
{
public:
    friend class StrBlobPtr;
    typedef std::initializer_list<std::string>::size_type size_type;
    StrBlob();
    StrBlob(std::initializer_list<std::string> il);
    size_type size() const { return data->empty(); }
    void push_back(const std::string &t) { data->push_back(t); }
    void pop_back();
    std::string &front();
    std::string &back();
    const std::string &front() const;
    const std::string &back() const;
    StrBlobPtr begin();
    StrBlobPtr end();

private:
    std::shared_ptr<std::vector<std::string>> data;
    void check(size_type i, const std::string &msg) const;
};

class StrBlobPtr
{
public:
    StrBlobPtr() : curr(0) {}
    StrBlobPtr(StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) {}
    string &deref() const;
    StrBlobPtr &incr();

private:
    shared_ptr<vector<string>> check(size_t, const string &) const;
    weak_ptr<vector<string>> wptr;
    size_t curr;
};

StrBlob::StrBlob() : data(std::make_shared<std::vector<std::string>>()) {}

StrBlob::StrBlob(std::initializer_list<std::string> il) : data(std::make_shared<std::vector<std::string>>(il)) {}

void StrBlob::check(size_type i, const std::string &msg) const
{
    if (i >= data->size())
    {
        throw std::out_of_range(msg);
    }
}

std::string &StrBlob::front()
{
    check(0, "front on empty StrBlob");
    return data->front();
}

std::string &StrBlob::back()
{
    check(0, "back on empty StrBlob");
    return data->back();
}

const std::string &StrBlob::front() const
{
    check(0, "front on empty StrBlob");
    return data->front();
}

const std::string &StrBlob::back() const
{
    check(0, "front on empty StrBlob");
    return data->back();
}

void StrBlob::pop_back()
{
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}

StrBlobPtr StrBlob::begin()
{
    return StrBlobPtr(*this);
}

StrBlobPtr StrBlob::end()
{
    return StrBlobPtr(*this, data->size());
}

shared_ptr<vector<string>> StrBlobPtr::check(size_t i, const string &msg) const
{
    auto ret = wptr.lock();
    if (!ret)
        throw runtime_error("unbound StrBlobPtr");
    if (i >= ret->size())
        throw out_of_range(msg);
    return ret;
}

string &StrBlobPtr::deref() const
{
    shared_ptr<vector<string>> p = check(curr, "dereference past end");
    return (*p)[curr];
}

StrBlobPtr &StrBlobPtr::incr()
{
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;
}

int use_shared_ptr(void)
{
}

void use_dynamic(void)
{
    // int *pi = new int;
    // std::cout << *pi << std::endl;
    std::string s("hello, world");
    auto p = new auto(s);
    std::cout << *p << std::endl;
    delete p;
}

void use_unique()
{
    unique_ptr<int> p(new int(4));
    cout << "p reffers to " << *p << endl;
    cout << "p is " << (uint64_t)p.get() << endl;
    unique_ptr<int> q(p.release());
    cout << "q reffers to " << *q << endl;
    cout << "now p is " << (uint64_t)p.get() << endl;
}

void use_weak()
{
    // auto p = make_shared<int>(42);
    // weak_ptr<int> wp(p);
    // p = nullptr;
    // if (shared_ptr<int> np = wp.lock()) {
    //     cout << *np << endl;
    // }

    // 下面这3行都行
    // 但new只能显示转换成shared_ptr
    // shared_ptr<int> sp = make_shared<int>(42);
    // shared_ptr<int> sp = shared_ptr<int>(new int(42));
    shared_ptr<int> sp(new int(42));
    cout << sp.use_count() << endl;
    shared_ptr<int> sp2(sp);
    cout << sp.use_count() << endl;
    weak_ptr<int> wp = sp;
    cout << sp.use_count() << endl;
    sp = nullptr;
    cout << sp2.use_count() << endl;
    if (shared_ptr<int> ret = wp.lock())
    {
        cout << "the number is " << *ret << endl;
        cout << ret.use_count() << endl;
    }
    sp2 = nullptr;
    if (shared_ptr<int> ret = wp.lock())
    {
        cout << "the number is " << *ret << endl;
        cout << ret.use_count() << endl;
    }
}

void what_if_realloc_vector()
{
    // 结果:容器本身的地址不随再分配改变, 元素的地址会改变.
    vector<int> v;
    cout << "capacity: " << v.capacity() << endl;
    cout << "address: " << &v << endl;
    v.push_back(0);
    cout << "capacity: " << v.capacity() << endl;
    cout << "address v: " << &v << endl;
    cout << "address v[0]: " << &v[0] << endl;
    v.resize(65536);
    cout << "capacity: " << v.capacity() << endl;
    cout << "address v: " << &v << endl;
    cout << "address v[0]: " << &v[0] << endl;
}

void use_dynamic_array()
{
    const size_t size = 7;
    // p1 is default initialized, but its values are all 0.
    int *p1 = new int[size];
    int *p2 = new int[size]();
    int *p3 = new int[size]{1, 2, 3, 4, 5};
    for (size_t i = 0; i < size; ++i)
    {
        cout << p1[i] << ' ' << p2[i] << ' ' << p3[i] << endl;
    }
    // 删除new分配的数组一定要加 []
    // 即使是通过别名定义的数组也要加, 否则是未定义行为.
    typedef int intarr_t[7];
    int *p4 = new intarr_t;
    delete[] p1;
    delete[] p2;
    delete[] p3;
    delete[] p4;
}

void smart_pointer_and_dynamic_array()
{
    unique_ptr<int[]> up(new int[10]);
    up[3] = 7;
    for (size_t i = 0; i < 10; ++i)
    {
        cout << up[i] << endl;
    }
    up.release();

    shared_ptr<int> sp(new int[10], [](int *p) { delete[] p; });
    for (size_t i = 0; i < 10; ++i)
    {
        cout << sp.get()[i] << endl;
    }
    sp.reset();
}

void use_allocator()
{
    const int n = 10;
    allocator<string> alloc;
    auto const p = alloc.allocate(n);
    // printf("%x", alloc.max_size());
    auto q = p;
    alloc.construct(q++);
    alloc.construct(q++, 10, 'c');
    alloc.construct(q++, "hi");
    for (size_t i = 0; i != 3; ++i)
    {
        cout << *(p + i) << endl;
    }

    p->push_back('a');
    cout << *p << endl;

    while (q != p)
        alloc.destroy(--q);

    alloc.deallocate(p, n);
}

void use_allocator_uninitialized()
{
    const int n = 10;
    vector<int> v{1, 2, 3, 4, 5, 1145, 7, 8, 9, 10};
    allocator<int> alloc;
    auto const p = alloc.allocate(n);
    uninitialized_copy(v.begin(), v.end(), p);
    for (auto q = p; q != p + n; ++q)
    {
        printf("%d\n", *q);
    }
}

size_t stupid_find(const string& org, const string& pattern)
{
    size_t i = 0, j = 0;
    while (true)
    {
        if (i == pattern.size())
            break;
        else if (j + i == org.size())
        {
            j = org.size();
            break;
        }
        else if (org[j + i] == pattern[i])
            ++i;
        else
        {
            ++j;
            i = 0;
        }
    }
    return (size_t)j;
}

size_t kmp_find(const string& org, const string& pattern)
{
    // [0, next[i]] 与 [i - next[i], i]全等
    vector<int> next(pattern.size(), -1);
    next[0] = 0;
    for (int i = 1; i != pattern.size(); ++i)
    {
        int j = next[i - 1];
        while (j > 0 && pattern[j + 1] != pattern[i])
            j = next[j];
        if (j > 0)
            next[i] = j + 1;
        else if (j == 0 && pattern[0] == pattern[i])
            next[i] = 0;
    }

    int i = 0, j = 0;
    while (true)
    {
        if (i == pattern.size())
            break;
        else if (i + j == org.size())
        {
            j = org.size();
            break;
        }
        else if (pattern[i] == org[j + i])
            ++i;
        else
        {
            if (next[i - 1] == -1)
            {
                j += i;
                i = 0;
            }
            else
            {
                j += next[i - 1] + 1;
                i = 0;
            }
        }
    }
    return (size_t)j;
}

void test_find()
{
    ifstream fin("../lab/kern/pmap.c");
    if (!fin)
        throw runtime_error("No such file or directory.");
    string line;
    string all;
    while (fin >> line)
    {
        all += line;
    }
    clock_t begin = clock();
    cout << (int)all.find("check_page_installed_pgdir() succeeded!") << endl;
    cout << "time1: " << clock() - begin << endl;

    begin = clock();
    cout << (int)kmp_find(all, "check_page_installed_pgdir() succeeded!") << endl;
    cout << "time2: " << clock() - begin << endl;
    
    begin = clock();
    cout << (int)stupid_find(all, "check_page_installed_pgdir() succeeded!") << endl;
    cout << "time3: " << clock() - begin << endl;
}

class QueryResult;

class TextQuery {
public:
    using line_no = std::vector<std::string>::size_type;
    // 一个默认构造函数还是挺重要
    TextQuery();
    TextQuery(std::ifstream &);
    QueryResult query(const std::string &) const;
private:
    std::shared_ptr<std::vector<string>> file;
    std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
};

TextQuery::TextQuery(ifstream &is): file(new vector<string>)
{
    string text;
    while (getline(is, text))
    {
        file->push_back(text);
        int n = file->size() - 1;
        istringstream line(text);
        string word;
        while (line >> word)
        {
            auto &lines = wm[word];
            if (!lines)
                lines.reset(new set<line_no>);
            // If a given word occurs more than once in the same line, 
            // the call to insert does nothing.
            lines->insert(n);
        }
    }
}

class QueryResult
{
friend std::ostream &print(std::ostream &, const QueryResult &);
public:
    using line_no = vector<string>::size_type;
    QueryResult(std::string s,
                std::shared_ptr<std::set<line_no>> p,
                std::shared_ptr<std::vector<string>> f):
        sought(s), lines(p), file(f) {}
private:
    std::string sought;
    std::shared_ptr<std::set<line_no>> lines;
    std::shared_ptr<std::vector<std::string>> file;
};

QueryResult TextQuery::query(const string &sought) const
{
    static shared_ptr<set<line_no>> nodata(new set<line_no>);
    auto loc = wm.find(sought);
    if (loc == wm.end())
        return QueryResult(sought, nodata, file);
    else
        return QueryResult(sought, loc->second, file);
}

ostream &print(ostream &os, const QueryResult &qr)
{
    os << qr.sought << " occurs " << qr.lines->size() << " "
        << (qr.lines->size() > 1 ? string("times") : string("time")) << endl;
    for (auto num : *qr.lines)
        os << "\t(line " << num + 1 << ") "
            << *(qr.file->begin() + num) << endl;
    return os;
}

// 先用再写
void runQueries(ifstream &infile)
{
    TextQuery tq(infile);
    while (true)
    {
        cout << "enter word to look for, or q to quit: ";
        string s;
        if (!(cin >> s) || s == "q") break;
        print(cout, tq.query(s)) << endl;
    }
}

void use_query()
{
    ifstream infile("../lab/kern/pmap.c");
    if (!infile)
        throw runtime_error("No such file.");
    runQueries(infile);
}



int main(void)
{
    // use_dynamic();
    // use_unique();
    // use_weak();
    // what_if_realloc_vector();
    // use_dynamic_array();
    // smart_pointer_and_dynamic_array();
    // use_allocator();
    // use_allocator_uninitialized();
    // test_find();
    use_query();
    return 0;
}