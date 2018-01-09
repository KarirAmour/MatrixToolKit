#include <iostream>
#include <vector>
#include <ctime>
#include "hashtable.h"

using namespace std;



int main() {

	const char *a = "blahblah";
	const char *b = "blahblag";
	const char *c = "blahblaj";
	const char *d = "a";
	const char *e = "AvaniAmourAnyaAlkaArvind";


	HashTable table;
	// cout << HashTable::ELFHash(a) << endl;
	// table = HashTable();
	table[a] = 15;
	table[a] = 20;
	table[b] = 69;
	table[c] = 100;
	table[d] = 3231;
	table[e] = 2131023;
	cout << table[a] << endl;
	cout << table[b] << endl;
	cout << table[c] << endl;
	cout << table[d] << endl;
	cout << table[e] << endl;

	return 0;
}