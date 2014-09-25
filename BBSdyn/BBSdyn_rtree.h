#ifndef BBSDYN_RTREE_H_
#define BBSDYN_RTREE_H_


class BBSdyn;

class BBSdyn_RTree : public RTree{
public:
	
	BBSdyn *bbsdyn; // pointer to the BBSdyn class
	
	int bufferSize;
	simpleBuffer *buf; // buffer

	bool sky_dom_iter(float* mbb, float* query, int domType, bool withCache);
	void skyline(XxkHeap *_hp, float* query, int domType, bool withCache);
	
	
	// constructors
	BBSdyn_RTree(BBSdyn *bbs, const char *fname, int _b_length, Cache *c, int _dimension, int bufferSize) : RTree(fname, _b_length, c, _dimension){
		this->bbsdyn = bbs;
		this->bufferSize = bufferSize;
		buf = new simpleBuffer(bufferSize);
	};
	BBSdyn_RTree(BBSdyn *bbs, const char *fname, Cache *c, int bufferSize) : RTree(fname, c){
		this->bbsdyn = bbs;
		this->bufferSize = bufferSize;
		buf = new simpleBuffer(bufferSize);
	};
	BBSdyn_RTree(BBSdyn *bbs, const char *inpname, const char *fname, int _b_length, Cache *c, int _dimension, int bufferSize) : RTree(inpname, fname, _b_length, c, _dimension){
		this->bbsdyn = bbs;
		this->bufferSize = bufferSize;
		buf = new simpleBuffer(bufferSize);
	};
	
};


#endif /*BBSDYN_RTREE_H_*/
