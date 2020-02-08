#include<stdio.h>
#include<math.h>
#include<deque>
#include<list>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string.h>
#include<vector>
#include<array>
#include<algorithm>
#include<numeric>
#include<iterator>
#include<functional>
using namespace std;
using namespace std::placeholders;

class PersonInfo {
public:
    string name;
    vector<string> phones;
    // PersonInfo(): name("Chen"), phones(vector<string> {"666666"}) {}
    PersonInfo(const string &s = "Chen"): name(s), phones(vector<string> {"666666"}) {}
    PersonInfo(const string &s1, const string &s2): name(s1), phones(vector<string> {s2}) {}
    // You can also use this:
    // PersonInfo(const string &s1, const string &s2): name(s1), phones{s2} {}
};

class Group {
public:
    vector<PersonInfo> persons;
    Group():persons(5){}
    Group(const PersonInfo &pi): persons{pi}{}
    void fst_prsn(void);
};

void Group::fst_prsn(void) {
    cout << persons[0].name << endl;
}

void try_PersonInfo(void) {
    PersonInfo p0("Pony", "123456");
    vector<string> strs{"Chen", "Pony", "Mike"};
    // This is right.
    vector<PersonInfo> persons(strs.begin(), strs.end());
    // And this is wrong.
    // vector<PersonInfo> persons(strs);
    for (auto it = persons.begin(); it != persons.end(); it++) {
        cout << it->name << endl;
    }
    PersonInfo p;
    cout << p.name << endl;
    PersonInfo p1("Pony");
    cout << p1.name << endl;
    Group grp;
    cout << grp.persons[0].name << endl;
    Group grp1(string("Pony"));
    cout << grp1.persons[0].name << endl;
}

void try_array(void) {
    array<PersonInfo, 5> a;
    cout << a[0].name << endl;
}

void try_assign(void) {
    vector<PersonInfo> v0 {string("Mike"), string("Jack"), string("Chen")};
    vector<PersonInfo> v1 {string("Cat"), string("Dog"), string("Mouce"), string("Fish")};
    v1.assign(v0.begin(), v0.end());
    v0.assign({string("Gates"), string("Jobs")});
    for (auto it = v1.begin(); it != v1.begin() + 2; it++) {
        cout << it->name << endl;
    }
    cout << endl;
    for (auto it = v0.begin(); it != v0.end(); ++it) {
        cout << it->name << endl;
    }
}

void try_compare(void) {
    vector<string> vs1{"sun", "rise"};
    vector<string> vs2{"sun", "set"};
    cout << (vs1 < vs2) << endl;
}

void try_vector(void) {
    vector<int> v (10, 0);
    for (auto it = v.begin(); it != v.end(); it++) {
        cout << *it << endl;
    }
}
void try_sstream(void) {
    string line, word;
    vector<PersonInfo> people;
    while(getline(cin, line)) {
        PersonInfo info;
        istringstream record(line);
        record >> info.name;
        while(record >> word)
            info.phones.push_back(word);
        people.push_back(info);
    }
    line = "interesting!";
    cout << line;
    ostringstream formatted(line);
    for(const auto &entry : people) {
        formatted << entry.name;
        for(const auto &nums : entry.phones) {
            formatted << " " << nums;
        }
        formatted << endl;
    }
    cout << formatted.str() << endl;
}
void try_file(void) {
    ifstream fin("./lab/inc/stdio.h", ifstream::in);
    string s;
    cout << (int)fin.eof() << (int)fin.fail() << (int)fin.bad() << (int)fin.good() << endl;
    fin.clear();
    cout << (int)fin.eof() << (int)fin.fail() << (int)fin.bad() << (int)fin.good() << endl;
    if (!fin) {
        cout << "Open failed." << endl;
    }
    while (getline(fin, s)) {
        cout << s << endl;
        cout << (int)fin.eof() << (int)fin.fail() << (int)fin.bad() << (int)fin.good() << endl;
    }
    cout << (int)fin.eof() << (int)fin.fail() << (int)fin.bad() << (int)fin.good() << endl;
    fin.close();
}

#define N 100000000

void try_vector_speed(void) {
    vector<int> v(10000, 0);
    for (size_t i = 0; i < N; i++) {
        v.insert(v.begin(), i);
        v.pop_back();
    }
}

void try_list(void) {
    list<int> l(10000, 0);
    for (size_t i = 0; i < N; i++) {
        l.insert(l.begin(), i);
        l.erase(l.end());
    }
}

void try_deque(void) {
    deque<int> d(10000, 0);
    int a;
    for (size_t i = 0; i < N; i++) {
        d.insert(d.begin(), i);
        d.push_back(a);
    }
}

// void try_link(void) {
//     link<int> l(100, 0);
//     for (size_t i = 0; i < N; i++) {
//         l.insert(l.begin(), i);
//         l.push_back();
//     }
// }

void try_string(void) {
    string a("this is a.");
    char s[20] = "this is b.";
    string b = s;
    cout << b << endl;
    strcpy(s, "not b .");
    cout << b << endl;
}

void test_adeq(void) {
    int a = 1;
    printf("%d", a += 1);
}

// void try_ref(int& a) {
//     a = 10;
// }

void try_speed(int argc, char **argv) {
    if (argc != 2) {
        cout << "wrong" << endl;
        exit(0);
    }
    if (strncmp(argv[1], "vector", 6) == 0) {
        try_vector_speed();
    }
    else if (strncmp(argv[1], "deque", 5) == 0) {
        try_deque();
    }
    else if (strncmp(argv[1], "list", 4) == 0) {
        try_list();
    }
    else {
        cout << "wrong" << endl;
    }
}

void try_ref(void) {
    vector<int> v(5, 0);
    auto &x = v[0];
    x = 1;
    cout << v[0] << endl;
}

void print_v(vector<int> &v) {
    for (auto it = v.begin(); it != v.end(); it++) {
        cout << *it;
    }
    cout << endl;
}

void try_init(void) {
    vector<int> v0;
    vector<int> v1 {1, 2, 3, 4};
    vector<int> v2(v1);
    vector<int> v3(v1.begin(), v1.end());
    vector<int> v4(4);
    vector<int> v5(4, 1);
    vector<int> v6 = v1;
    vector<int> v7 = {1, 2, 3, 4};
    vector<float> v8(v1.begin(), v1.end());
    print_v(v0);
    print_v(v1);
    print_v(v2);
    print_v(v3);
    print_v(v4);
    print_v(v6);
    print_v(v6);
    print_v(v7);
    for (auto it = v8.begin(); it != v8.end(); it++) {
        cout << *it;
    }
    cout << endl;

}

void try_swap(void) {
    vector<string> vs1{"str1", "str2"};
    vector<string> vs2{"str3", "str4"};
    auto it = vs1.begin();
    vs1.swap(vs2);
    cout << *it << endl;

    vector<int> vi1{1, 2, 3};
    vector<int> vi2{4, 5, 6};
    auto iti = vi1.begin();
    vi1.swap(vi2);
    cout << *iti << endl;
}

void try_insert(void) {
    vector<int> v0{1, 2, 3, 4, 5};
    vector<int> v1{100, 101, 102, 103};
    v1.erase(v1.begin() + 1);
    auto it = v1.insert(v1.begin() + 1, v0.begin(), v0.end());
    for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++) {
        cout << *it << ' ';
    }
    cout << '\n';
    cout << *it;
}

void try_cap(void) {
    vector<string> v{"some", "day", "I will", "be", "gone"};
    v.resize(10, ".");
    for (vector<string>::iterator it = v.begin(); it != v.end(); it++) {
        cout << *it + ' ';
    }
    cout << endl << v.capacity() << endl;
    v.reserve(0x100000);
    cout << endl << v.capacity() << endl;
    while (v.size() != v.capacity()) {
        v.push_back("god");
    }
    cout << v.size() << endl;
    v.shrink_to_fit();
    cout << endl << v.capacity() << endl;
    PersonInfo p;
}

void try_adding(void) {
    vector<int> v(2);
    auto p = &v[0];
    cout << p << endl;
    for (int i = 0; i < 0x1000000; i++) {
        v.push_back(i);
        if ((&v[0]) != p) {
            cout << v.size() << ": " << &v[0] << endl;
            p = &v[0];
        }
    }
    cout << &v[0] << endl;
}

void loop_insert(void) {
    deque<int> dq(100000000);
    for (int i = 0; i < N; i++) {
        dq.insert(dq.begin(), i);
    }
}

void loop_push(void) {
    deque<int> dq(100000000);
    for (int i = 0; i < N; i++) {
        dq.push_front(i);
    }
}

void insert_and_loop(int argc, char **argv) {
    deque<int> dq1{1, 2, 3};
    deque<int> dq2(dq1);

    dq1.insert(dq1.begin() + 1, 5, 0);
    for (size_t i = 0; i < dq1.size(); i++) {
        cout << dq1[i] << " ";
    }
    cout << endl;
    dq1.insert(dq1.begin(), {1, 2, 3, 4});
    cout << &dq1[0] << endl;
    dq1.insert(dq1.begin(), 0);
    cout << &dq1[0] << endl;
    cout << &dq2[0] << endl;
    dq2.push_front(0);
    cout << &dq2[0] << endl;

    dq2.assign({1, 59238, 32948239, 2313});
    if (argc != 2) {
        cout << "wrong." << endl;
    }
    if (string(argv[1]) == string("insert")) {
        loop_insert();
    }
    else if (string(argv[1]) == string("push")) {
        loop_push();
    }
    else {
        cout << "wrong." << endl;
    }
}

void try_string_construct(void) {
    char s[] = "God has never played his role.";
    string s0(s);
    string s1(s0, 0, s0.size()/2);
    cout << s0 << endl << s1 << endl;
    cout << s0.substr(5, 1) << endl;
}

void try_string_find(void) {
    string s("don't go gentle into the good night.");
    cout << s.front() << endl;
    string s1;
}

void try_algorithm(void) {
    vector<string> v{"a", "b", "e", "c"};
    sort(v.begin(), v.end());
    cout << v.at(0) << v.at(1) << v.at(2) << v.at(3) << endl;
}

void try_func_compare(void) {
    string s1 = "terjwewakdfjla";
    string s2 = "kejfaklfjweknvj";
    cout << s1.compare(s1) << endl;
}

void str2num(void) {
    auto s = to_string(3.94159);
    cout << stoll(s) << endl;
    vector<int> v;
}

void try_accm(void) {
    vector<string> v{"This", "is", "accumulate()."};
    string sum = accumulate(v.cbegin(), v.cend(), string(""));
    cout << sum << endl;
}

void printString(const string &s) {
    cout << s << endl;
}

void replace(void) {
    vector<int> vec(10);
    // fill_n(vec.begin(), 10, 0);
    fill_n(back_inserter(vec), 10, 0);
    for (size_t i = 0; i != vec.size(); i++) {
        cout << vec[i];
    }
    replace(vec.begin(), vec.end(), 0, 5);
    for (size_t i = 0; i != vec.size(); i++) {
        cout << vec[i];
    }
    vector<int> vec1;
    replace_copy(vec.begin(), vec.end(), back_inserter(vec1), 5, 2);
    for (size_t i = 0; i != vec1.size(); i++) {
        cout << vec1[i];
    }
}

void sort(void) {
    ifstream fin("lab/kern/trap.c");
    if (!fin) {
        return;
    }
    vector<string> words;
    string word;
    while (fin >> word) {
        words.emplace_back(word);
    }
    cout << "The file has " << words.size() << "words." << endl;

    sort(words.begin(), words.end());
    vector<string>::iterator it(words.begin());
    while (it != words.end()) {
        word = *it;
        ++it;
        while (it != words.end() && *it == word) {
            it = words.erase(it);
        }
    }
    for (auto str : words) {
        cout << str << " ";
    }
    cout << endl;
    cout << words.size() << "individual ones." << endl;
}

bool is_same_len(const string &s1, const string &s2) {
    return s1.size() == s2.size();
}

void elim_dups(vector<string> &words) {
    sort(words.begin(), words.end());
    auto end_unique = unique(words.begin(), words.end());
    words.erase(end_unique, words.end());
}

bool is_shorter(const string &s1, const string &s2) {
    return s1.size() < s2.size();
}

void unique_len(void) {
    vector<string> words;
    ifstream fin("./lab/kern/pmap.c");
    if (!fin) {
        return;
    }
    istream_iterator<string> str_it(fin), eof;
    words.assign(str_it, eof);
    sort(words.begin(), words.end(), is_shorter);
    vector<string>::iterator wc = unique(words.begin(), words.end(), is_same_len);
    words.erase(wc, words.end());
    for (auto str : words) {
        cout << str << endl;
    }
}

void len_sort(void) {
    ifstream fin("./lab/kern/pmap.c");
    if (!fin) {
        return;
    }
    vector<string> words;
    string word;
    while (fin >> word) {
        words.emplace_back(word);
    }

    elim_dups(words);
    stable_sort(words.begin(), words.end(), is_shorter);

    size_t sz = 5;
    auto wc = find_if(words.begin(), words.end(), [sz] (const string &s)
                                             {return s.size() >= sz; });
    auto count = words.begin() - wc;
    cout << count << " " << "word(s)"
         << "of length " << sz << " or longer" << endl;

    for (const string &s : words) {
        cout << s << endl;
    }

    for_each(wc, words.end(), [](const string &s) {cout << s << " ";});
}

void lambda_exp(void) {
    int x = 45;
    auto f = [x] () mutable {return ++x;};
    for (int i = 0; i < 5; i++) {
        cout << ([x] () mutable {return ++x;})() << endl;
        ++x;
    }
}

void take_a_ref(const string &s) {
    cout << "the parameter is " << s << endl;
}

void call_use_ref(void) {
    string arg("\"this is a rvalue argument\"");
    take_a_ref(string("\"this is a lvalue argument\""));
}

ostream &print(ostream &os, const string &s, char c) {
    os << s << c;
    return os;
}

void use_bind(void) {
    vector<string> words(5, "word");
    for_each(words.begin(), words.end(), bind(print, ref(cout), _1, ' '));
}

void stream_it(void) {
    // istream_iterator<string> int_it(cin);
    // istream_iterator<string> eof;
    // vector<string> vec;
    // while (int_it != eof) {
        // vec.push_back(*int_it++);
    // }
//
    // for_each(vec.begin(), vec.end(), [] (const string &s) {cout << s << endl; });
    // istream_iterator<string> int_it(cin), eof;
    // vector<string> vec(int_it, eof);
    // for_each(vec.begin(), vec.end(), [] (const string &s) {cout << s << endl; });
    istream_iterator<int> int_iter(cin), eof;
    cout << accumulate(int_iter, eof, 0);
}

int main(/*int argc, char **argv*/) {
    // try_speed(argc, argv);
    // try_init();
    // try_PersonInfo();
    //
    try_assign();
    // try_swap();
    // try_compare();
    // try_adding();
    // insert_and_loop(argc, argv);
    // try_ref();
    // try_insert();
    // try_cap();
    // insert_and_loop(argc, argv);
    // try_string_construct();
    // try_string_find();
    // try_algorithm();
    // try_func_compare();
    // str2num();
    // try_accm();
    // printString("a");
    // replace();
    // sort();
    // len_sort();
    // lambda_exp();
    // use_bind();
    // stream_it();
    // unique_len();
}
