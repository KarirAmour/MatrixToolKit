#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#define INITIAL_SIZE 256


class HashTable {
public:

	HashTable(unsigned int size = INITIAL_SIZE);

	int &operator[](const char *key);

	~HashTable();


private:
	struct Bucket;

	unsigned int table_size;
	Bucket *hashtable;
	static unsigned int ELFHash(const char *key);

};



#endif
