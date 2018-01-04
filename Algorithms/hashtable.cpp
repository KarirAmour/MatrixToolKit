#include "hashtable.h"
// #include "bucket.h"


#include <cstring>
#include <cstdlib>
#include <iostream>

struct HashTable::Bucket {

	int value;
	char *key;
	Bucket *next;

	Bucket() : value{0}, key{nullptr}, next{nullptr} {}

	void addKey(const char *k) {
		std::cout << "BKT addKey Enter" << std::endl;
		key = (char *) malloc(strlen(k) + 1);
		strcpy(key, k);
		std::cout << "BKT addKey Exit" << std::endl;

	}

	~Bucket() {
		if (this != this->next) delete this->next;
		free(key);
	}

};


HashTable::HashTable(unsigned int size) : table_size{size} {
	std::cout << "HT Ctor Enter" << std::endl;
	this->hashtable = new Bucket[this->table_size]();
		std::cout << "HT ctor Exit" << std::endl;

}




int &HashTable::operator[](const char *key) {
	std::cout << "HT[] Enter" << std::endl;

	unsigned int hash = ELFHash(key) % this->table_size;
	Bucket *bkt = &(this->hashtable[hash]);

	if (bkt->key) {
		do {
			if (strcmp(bkt->key, key) == 0) { // If key exists.
				std::cout << key << " strcmp(bkt->key, key)" << std::endl;
				return bkt->value;
			}
			std::cout << "bkt = bkt->next" << std::endl;
			bkt = bkt->next;
		} while (bkt->next);
		bkt = bkt->next = new Bucket();
	}

	bkt->addKey(key);
	std::cout << "HT[] Exit" << std::endl;

	return bkt->value;

}

HashTable::~HashTable() { 	
	std::cout << "HT Dtor Enter." << std::endl;
	delete[] hashtable; 
	std::cout << "HT Dtor Exit." << std::endl;

}




unsigned int HashTable::ELFHash(const char *key) {
	std::cout << "ELF Enter." << std::endl;
	// ELF Hash Algorithm
	unsigned int hash = 0, x = 0;
	while (*key) {
		hash = (hash << 4) + *key;
		x = hash & 0xF0000000L;
		if (x) hash ^= (x >> 24);
		hash &= ~x;
		++key;
	}
	std::cout << "ELF Exit." << std::endl;
	return hash;
}

