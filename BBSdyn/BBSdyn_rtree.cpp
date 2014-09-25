#include "../common/pch.h"

bool BBSdyn_RTree::sky_dom_iter(float* mbb, float* query, int domType, bool withCache){  // mbb is an MBB
    int dim = bbsdyn->dimension;
    int i, j;
    bool ret = false;
    vector<float>::iterator iter;


    bool isPoint = true;
    for (i=0; i<dim; i++){
        if (mbb[2*i+1] > mbb[2*i]){
            isPoint = false;
            break;
        }
    }



    // check for dynamic/orthant dominance by a skyline point
    iter = bbsdyn->result.begin();

    while (iter!=bbsdyn->result.end()){
        // Extract a result point
        float *s_pt = new float[dim];
        for (int j=0; j<(dim); j++){
            s_pt[j] = *iter;
            iter++;
        }

        if (domType == DYN) {
            // Check for dynamic dominance
            if (!PdyndomM(s_pt, mbb, query, dim)){
                delete[] s_pt;
                continue;
            }
        }
        else if (domType == ORTH) {
            // Check for orthant dominance
            if (!PorthdomM(s_pt, mbb, query, dim)){
                delete[] s_pt;
                continue;
            }
        }

        delete[] s_pt;
        return true;
    }


    // check for orthant dominance by a cached query point
    if (withCache){
//        cerr << "withCache = " << withCache << endl;

        iter = bbsdyn->cachedQueries.begin();

        while (iter!=bbsdyn->cachedQueries.end()){
            // Extract a cached query point
            float *s_pt = new float[dim];
            for (int j=0; j<(dim); j++){
                s_pt[j] = *iter;
                iter++;
            }

            // Check for orthant dominance
            if (!PorthdomM(s_pt, mbb, query, dim)){
                delete[] s_pt;
                continue;
            }

            delete[] s_pt;
            if (!isPoint){
//                cerr << "MBB pruned by cachedQueries" << endl;
            }
            return true;
        }
    }


    // check for dynamic dominance by a cached result point
    if (withCache && domType == DYN){
        iter = bbsdyn->cachedResults.begin();

        while (iter!=bbsdyn->cachedResults.end()){
            // Extract a cached query point
            float *s_pt = new float[dim];
            for (int j=0; j<(dim); j++){
                s_pt[j] = *iter;
                iter++;
            }

            // Check for dynamic dominance
            if (!PdyndomM(s_pt, mbb, query, dim)){
                delete[] s_pt;
                continue;
            }

            delete[] s_pt;
            if (!isPoint){
//                cerr << "MBB pruned by cachedResults" << endl;
            }
            return true;
        }
    }


    return false;
}

void BBSdyn_RTree::skyline(XxkHeap *heap, float* query, int domType, bool withCache){
    int i, j;

    // create a HeapEntry for root
    HeapEntry *he=new HeapEntry();
    he->dim = dimension;
    he->son1 = root;
    he->key = 0;
    he->level = 10; //this is not important but make sure it is greater than 0
    he->bounces = new float[2 * dimension];
    for (j = 0; j < dimension; j++){
        he->bounces[2*j] = minValue;
        he->bounces[2*j+1] = maxValue;
    }
    heap->push( he );



    float *child_mbb = new float[2*dimension];
    float *mbb = new float[2*dimension];




    while (heap->empty() == false){
        he = heap->top();
        heap->pop();

        int son = he->son1;
        int level = he->level;

//        cerr << "deheaped entry's mbb at level " << level << ": ";
        for (i = 0; i < 2*dimension; i ++){
            mbb[i] = he->bounces[i];
//            cerr << mbb[i] << " ";
        }
//        cerr << endl;

        if (level==0){
            if (!sky_dom_iter(mbb, query, domType, withCache)){
                for(j=0;j<dimension;j++){
                    bbsdyn->result.push_back(mbb[2*j]);
                    //cout<<bounces[2*j]<<" ";
                }
                //cout<<endl;
            }
        }
        else{
            if (!sky_dom_iter(mbb, query, domType, withCache)){
                RTNode *child = new RTNode(this, son);
                if (!buf->isHit(son, PPAGE, level)){
                    bbsdyn->accesses++;
                }

//                cerr << "number of entries in read node " << child->num_entries << endl;

                for (i = 0; i < child->num_entries; i ++) {
//                    cerr << "entry mbb: ";
                    for (j = 0; j < 2*dimension; j ++) {
                        child_mbb[j] = child->entries[i].bounces[j];
//                        cerr << child_mbb[j] << " ";
                    }

//                    if (!sky_dom_iter(child_mbb, query, domType, withCache)){
                    if (1){
                        HeapEntry *tmp_he = new HeapEntry();
                        tmp_he->init_HeapEntry( dimension );
                        tmp_he->son1 = child->entries[i].son;
                        tmp_he->level = child->level;
//                        cerr << "child->level = " << child->level << endl;

                        tmp_he->key = mindist_value(query, child_mbb, dimension);

                        memcpy( tmp_he->bounces, child->entries[i].bounces, 2*dimension*sizeof( float ) );
                        heap->push( tmp_he );
                    }
                    else {
//                        cerr << " pruned!";
                    }
//                    cerr << endl;
                }
                delete child;
            }
        }
        delete he;
    }

    delete [] mbb;
    delete [] child_mbb;

    return;
}
