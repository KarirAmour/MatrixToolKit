#include "hashtable.h"
// #include "bucket.h"


#include <cstring>
#include <cstdlib>

struct HashTable::Bucket {

	int value;
	char *key;
	Bucket *next;

	Bucket() : value{0}, key{nullptr}, next{nullptr} {}

	void addKey(const char *k) {
		key = (char *) malloc(strlen(k) + 1);
		strcpy(key, k);
	}

	~Bucket() {
		if (this != this->next) 
			delete this->next;
		delete key;
	}

};


HashTable::HashTable(unsigned int size) : table_size{size} {
	this->hashtable = new Bucket[this->table_size]();
}




int &HashTable::operator[](const char *key) {

	unsigned int hash = ELFHash(key) % this->table_size;
	Bucket *bkt = &(this->hashtable[hash]);
	if (bkt->key) {
		while (bkt->next) {
			bkt = bkt->next;
		}
		bkt = bkt->next = new Bucket();
	}

	bkt->addKey(key);
	return bkt->value;

}

HashTable::~HashTable() { delete hashtable; }




unsigned int HashTable::ELFHash(const char *key) {
	unsigned int hash = 0, x = 0;

	while (*key) {
		hash = (hash << 4) + *key;
		x = hash & 0xF0000000L;
		if (x) hash ^= (x >> 24);
		hash &= ~x;
		++key;
	}

	return hash;
}

