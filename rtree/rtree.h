#ifndef __RTREE
#define __RTREE

#include <list>

struct hilmbr{
	int block;
	float *bounces;
	bitmask_t hil_v;
};

typedef hilmbr * hilmbr_ptr;

class RTree : public Cacheable{
public:
	int dimension;
	int num_of_data;
    int num_of_dnodes;
    int num_of_inodes;
	int root;
	bool root_is_data;

	float minValue; // minimum value (in any dimension) stored in the tree
	float maxValue; // maximum value (in any dimension) stored in the tree

	RTNode *root_ptr;
    bool *re_level;
    LinList *re_data_cands;
	LinList *deletelist;


	RTree(const char *fname, int _b_length, Cache* c, int _dimension);
    RTree(const char *fname, Cache* c);
    RTree(const char *inpname, const char *fname, int _blength, Cache* c, int _dimension);
    ~RTree();

	bool delete_entry(Entry *d);
	bool FindLeaf(Entry *e);
    int get_num() { return num_of_data; }
	void insert(Entry *d);
	void load_root();
	void NNQuery(float *QueryPoint, SortedLinList *res);
	int rangeQuery(float *mbr);
	void read_header(char *buffer);
	void write_header(char *buffer);

	void bulkload(const char *_fname, int _num_bit);



	bool sky_dom(int _dim, list<float> &result, float *_pt);
	bool sky_dom_iter(int _dim, list<float> &result, float *_pt);

	void BBS_subspace(XxkHeap *_hp, list<float> &result, bool *_active_dim);
	//bool skyband_dom( int _dim, float *_rslt, int _rsltcnt, float *_pt, int nTopK );
	//void BBS_subspace_band(XxkHeap *_hp, float *_rslt, int &_rsltcnt, bool *_active_dim, int nTopK );
	//void BBS_top_k( XxkHeap *_hp, float *_rslt, int &_rsltcnt, float *_dim_weight, int nTopK );


};
#endif // __RTREE
