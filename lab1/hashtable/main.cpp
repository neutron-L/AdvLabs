#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <unordered_set>
#include <cassert>
#include "hashtable.h"
using std::ifstream;
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::unordered_set;

constexpr unsigned int kDictSize = 1034189;
constexpr unsigned int kWordsSize = 123417;

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int alpha, type, idx;
    cout << "input the item type: " << endl;
    cout << "1.linkedlist\t2.avltree\t3.rbtree\t4.skiplist\n";
    cin >> type;
    cout << "input the hash function: " << endl;
    cout << "1.RSHash\t2.JSHash\t3.PJWHash\t4.ELFHash\t5.BKDRHash\n"
            "6.SDBMHash\t7.DJBHash\t8.DEKHash\t9.BPHash\t10.FNVHash\t11.APHash\n";
    cin >> idx;
    cout << "input load factor: ";
    cin >> alpha;
    --idx;
    --type;
    clock_t start, mid, end;
    start = clock();
    HashTable<string> hashTable(kDictSize / alpha, idx, type);

    ifstream fr;
    std::ofstream fw;
//    insert
    fr.open("dict.txt");
    assert(fr.is_open());
    string str;
    while (fr >> str)
        hashTable.insert(str);
    fr.close();
    mid = clock();
//    print
//    hashTable.printTable();

//    search
    unsigned int sum = 0;
    unsigned int times;
    int hits = 0;
    fr.open("words.txt");
    fw.open("result.txt");
    assert(fr.is_open());
    assert(fw.is_open());
    while (fr >> str) {
        if (hashTable.search(str, times))
        {
            fw << str << endl;
            hits++;
            sum += times;
        }
//        sum += times;
    }
    fr.close();
    fw.close();
    end = clock();
//    cout << "Total time: " << (end - start) << "ms" << endl;
    cout << "Hashtable build time: " << (mid - start) << "ms" << endl;
    cout << "Search time: " << (end - mid) << "ms" << endl;
    cout << "Total matches: " << sum << endl;
    cout << "Average number of comparisons: " << 1.0 * sum / hits << endl;
    cout << "Hits: " << hits << endl;

    return 0;
}
