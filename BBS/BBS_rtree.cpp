#include "../common/pch.h"

bool BBS_RTree::sky_dom_iter(float* _pt){  // _pt is an MBR
	int dim = bbs->dimension;
	int i, j;
	bool ret = false;
	vector<float>::iterator iter;

	iter = bbs->result.begin();

	// Extract point
	float *point = new float[dim];
	for(i=0;i<dim;i++)
		point[i] = _pt[2*i];

	while (iter!=bbs->result.end()){
		// Extract a result point
		float *s_pt = new float[dim];
		for (int j=0; j<(dim); j++){
			s_pt[j] = *iter;
			iter++;
		}

		// Check for dominance
		if (!dominate(s_pt, point, dim)){
			delete[] s_pt;
			continue;
		}

		delete[] s_pt;
		ret = true;
		break;
	}

	delete[] point;
	return ret;
}

void BBS_RTree::skyline(XxkHeap *_hp){
	int i, j;

	HeapEntry *he=new HeapEntry();
	he->dim = dimension;
	he->son1 = root;
	he->key = 0;
	he->level = 1; //this is not important but make sure it is greater than 0
	he->bounces = new float[2 * dimension];
	for (j = 0; j < 2 * dimension; j++)
		he->bounces[j] = 0.0;
	_hp->push( he );

	float *b = new float[2*dimension];
	float *bounces = new float[2*dimension];

	while (_hp->empty() == false){
		he = _hp->top();
		_hp->pop();

		int son = he->son1;
		int level = he->level;

		for (i = 0; i < 2*dimension; i ++){
			bounces[i] = he->bounces[i];
		}

		if (level==0){
			if (!sky_dom_iter(bounces)){
				for(j=0;j<dimension;j++){
					bbs->result.push_back(bounces[2*j]);
					//cout<<bounces[2*j]<<" ";
				}
				//cout<<endl;
			}
		}
		else{
			if (!sky_dom_iter(bounces)){
				RTNode *child = new RTNode(this, son);
				bbs->accesses++;

				for (i = 0; i < child->num_entries; i ++){
					for (j = 0; j < 2*dimension; j ++)
							b[j] = child->entries[i].bounces[j];

					if (!sky_dom_iter(b)){
						HeapEntry *tmp_he = new HeapEntry();
						tmp_he->init_HeapEntry( dimension );
						tmp_he->son1 = child->entries[i].son;
						tmp_he->level = child->level;
						tmp_he->key = 0;

						for (j = 0; j < dimension; j ++)
							tmp_he->key += b[2*j]; // Manhattan distance

						memcpy( tmp_he->bounces, child->entries[i].bounces, 2 * dimension * sizeof( float ) );
						_hp->push( tmp_he );
					}
				}
				delete child;
			}
		}
		delete he;
	}

	delete [] bounces;
	delete [] b;

	return;
}
