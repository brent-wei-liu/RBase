#ifndef SIMPLEBUFFER_H_
#define SIMPLEBUFFER_H_

//#include <sys/time.h>
#include <cassert>
#include <iostream>

using namespace std;


#define PPAGE 0
#define BPAGE 1
#define MAXPAGE 100000


//namespace simpleBuffer{

class Page{
public:
	long pageID;
	long key; // lower key is better (key=-priority)

	Page();

	void setPage(long pageID, long priority);  // higher priority is better

	bool operator==(const Page &p);

	bool operator<(const Page &p);

	bool operator>(const Page &p);

	bool operator<=(const Page &p);

	bool operator>=(const Page &p);
};


template <class T> void MaxHeapify(T *input, int index, int size);
int left(int i);
int right(int i);


class simpleBuffer{
public:
	int currSize; // current number of Buffer entries
	int bufSize;
	Page *bf;

	long globalTime;

	long pHits;
	long pMisses;

	long bHits;
	long bMisses;

	simpleBuffer(int bufSize);


	void shrink(int bufSize);
	void expand(int bufSize);

	void reset(int bufSize);

	~simpleBuffer();

	int find(long pageID);

	/// call with priority<0 to get LRU
	bool isHit(long realPageID, int type, long priority); // higher priority is better; type = {PPAGE, BPAGE}

};

ostream& operator <<(ostream &os, const simpleBuffer &c);

//}

#endif /*SIMPLEBUFFER_H_*/
