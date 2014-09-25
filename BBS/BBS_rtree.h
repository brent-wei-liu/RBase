#ifndef BBS_RTREE_H_
#define BBS_RTREE_H_

class BBS;

class BBS_RTree : public RTree{
public:
    
    BBS *bbs; // pointer to the BBS class
    
    bool sky_dom_iter(float* _pt);
    void skyline(XxkHeap *_hp);
    
    
    // constructors
    BBS_RTree(BBS *bbs, const char *fname, int _b_length, Cache *c, int _dimension) : RTree(fname, _b_length, c, _dimension){
        this->bbs = bbs;
    };
    BBS_RTree(BBS *bbs, const char *fname, Cache *c) : RTree(fname, c){
        this->bbs = bbs;
    };
    BBS_RTree(BBS *bbs, const char *inpname, const char *fname, int _b_length, Cache *c, int _dimension) : RTree(inpname, fname, _b_length, c, _dimension){
        this->bbs = bbs;
    };
    
};


#endif /*BBS_RTREE_H_*/
