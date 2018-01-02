#ifndef _BUCKET_H_
#define _BUCKET_H_

// class HashTable;


struct Bucket {

	int value;
	char *key;
	Bucket *next;

	Bucket();

	void addKey(const char *k);

	~Bucket();

};



#endif
