#include <iostream>
#include <vector>
#include <ctime>
#include "hashtable.h"

using namespace std;



int main() {

	const char *a = "blahblah";
	// const char *b = "blahblag";
	// const char *c = "blahblaj";
	// const char *d = "a";
	// const char *e = "AvaniAmourAnyaAlkaArvind";


	HashTable table;
	cout << HashTable::ELFHash(a) << endl;
	table = HashTable();
	table[a];
	cout << table[a] << endl;

	return 0;
}