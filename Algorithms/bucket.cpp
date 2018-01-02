#include "bucket.h"

#include <cstring>
#include <cstdlib>



HashTable::Bucket::Bucket() : value{0}, key{nullptr}, next{nullptr} {}


void HashTable::Bucket::addKey(const char *k) {
	key = (char *) malloc(strlen(k) + 1);
	strcpy(key, k);
}


HashTable::Bucket::~Bucket() {
	if (this != this->next) 
		delete this->next;
	delete key;
}