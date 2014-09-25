#include "../common/pch.h"


RTree::RTree(const char *fname, int _b_length, Cache *c, int _dimension){
  //use this constructor to build a new tree
    file = new BlockFile(fname, _b_length);
    cache = c;

    re_data_cands = new LinList();
    deletelist = new LinList();

    dimension = _dimension;
    root = 0;
    root_ptr = NULL;
    root_is_data = TRUE;
    num_of_data = num_of_inodes = num_of_dnodes = 0;

    root_ptr = new RTNode(this);
      //note that when a tree is constructed, the root is automatically created
      //though at this time there is no entry in the root yet.
    num_of_dnodes++;
    root_ptr -> level = 0;
    root = root_ptr -> block;
    delete root_ptr;
    root_ptr = NULL;
}

RTree::RTree(const char *fname, Cache *c)
  //use this constructor to restore a tree from a file
{
    file = new BlockFile(fname, 0);
    cache =c;

    re_data_cands = new LinList();
    deletelist = new LinList();

    char *header = new char [file->get_blocklength()];
    file -> read_header(header);
    read_header(header);
    delete [] header;

    root_ptr = NULL;
}

RTree::RTree(const char *inpname, const char *fname, int _b_length, Cache *c, int _dimension){
  // construct new R-tree from a specified input textfile with rectangles
    Entry *d;
    FILE *fp;
    file = new BlockFile(fname, _b_length);
    cache =c;

    re_data_cands = new LinList();
    deletelist = new LinList();

    char *header = new char [file->get_blocklength()];
    read_header(header);
    delete [] header;

    dimension = _dimension;
    root = 0;
    root_ptr = NULL;
    root_is_data = TRUE;
    num_of_data = num_of_inodes = num_of_dnodes = 0;

    root_ptr = new RTNode(this);
    num_of_dnodes++;
    root_ptr -> level = 0;
    root = root_ptr->block;

    int record_count = 0;

    if((fp = fopen(inpname,"r")) == NULL){
      delete this;
      error("Cannot open R-Tree text file", TRUE);
    }
    else{
      int id=0;
      while (!feof(fp)){
        record_count ++;
        id ++; //disable this variable if the id comes with the dataset

        if (record_count%1000 == 0){
            for (int i = 0; i < 79; i ++)  //clear a line
              printf("\b");
            printf("inserting object %d", record_count);
        }

        //fscanf(fp, "%d ", &id); // disable if the id does not come with the dataset
        d = new Entry(dimension, NULL);
        d -> son = id;
        for (int i=0; i<dimension; i++){
            fscanf(fp, "%f", &(d->bounces[2*i]));
            d->bounces[2*i+1] = d->bounces[2*i];
        }
        fscanf(fp, "\n");

        insert(d);
      }
    }

    fclose(fp);

    printf("\n");
    delete root_ptr;
    root_ptr = NULL;
}

RTree::~RTree(){
    char *header = new char[file -> get_blocklength()];
    write_header(header);
    file->set_header(header);
    delete [] header;

    if (root_ptr != NULL){
        delete root_ptr;
        root_ptr = NULL;
    }

    if (cache)
      cache -> flush();

    delete file;
    delete re_data_cands;
    delete deletelist;

  //printf("This R-Tree contains %d internal, %d data nodes and %d data\n",
    //    num_of_inodes, num_of_dnodes, num_of_data);
}

bool RTree::delete_entry(Entry *d){
    load_root();

    R_DELETE del_ret;
    del_ret=root_ptr->delete_entry(d);

    if (del_ret == NOTFOUND) return false;
    if (del_ret == ERASED)
        error("RTree::delete_entry--The root has been deleted\n",true);

    if (root_ptr -> level > 0 && root_ptr -> num_entries == 1){
        //there is only one entry in the root but the root
        //is not leaf.  in this case, the child of the root is exhalted to root
        root = root_ptr -> entries[0].son;
        delete root_ptr;
        root_ptr = NULL;
        load_root();
        num_of_inodes--;
    }

    //Now will reinsert the entries
    while (deletelist -> get_num() > 0){
        Linkable *e;
        e = deletelist -> get_first();
        Entry *new_e = new Entry(dimension, NULL);
        new_e -> set_from_Linkable(e);
        deletelist -> erase();
        insert(new_e);
    }

    delete root_ptr;
    root_ptr = NULL;

    return true;
}

bool RTree::FindLeaf(Entry *e){
    load_root();
    return root_ptr -> FindLeaf(e);
}

void RTree::insert(Entry* d){
    int i, j;
    RTNode *sn;
    RTNode *nroot_ptr;
    int nroot;
    Entry *de;
    R_OVERFLOW split_root;
    Entry *dc;
    float *nmbr;

    // load root into memory
    load_root();

    // no overflow occured until now
    re_level = new bool[root_ptr -> level + 1];
    for (i = 0; i <= root_ptr -> level; i++)
        re_level[i] = FALSE;

    // insert d into re_data_cands as the first entry to insert
    // make a copy of d because it should be erased later
    Linkable *new_link;
    new_link = d -> gen_Linkable();
    re_data_cands -> insert(new_link);

    delete d;  //we follow the convention that the entry will be deleted when insertion finishes

    j = -1;
    while (re_data_cands -> get_num() > 0){
        // first try to insert data, then directory entries
        Linkable *d_cand;
        d_cand = re_data_cands -> get_first();
        if (d_cand != NULL){
            // since "erase" deletes the data itself from the
            // list, we should make a copy of the data before
            // erasing it
            dc = new Entry(dimension, NULL);
            dc -> set_from_Linkable(d_cand);
            re_data_cands -> erase();

            // start recursive insert with root
            split_root = root_ptr -> insert(dc, &sn);
        }
        else
            error("RTree::insert: inconsistent list re_data_cands", TRUE);

        if (split_root == SPLIT){
        // insert has lead to split --> new root-page with two sons (i.e. root and sn)
            nroot_ptr = new RTNode(this);
            nroot_ptr -> level = root_ptr -> level + 1;
            num_of_inodes++;
            nroot = nroot_ptr -> block;

            de = new Entry(dimension, this);
            nmbr = root_ptr -> get_mbr();
            memcpy(de->bounces, nmbr, 2*dimension*sizeof(float));
            delete [] nmbr;
            de->son = root_ptr->block;
            de->son_ptr = root_ptr;
            nroot_ptr -> enter(de);

            de = new Entry(dimension, this);
            nmbr = sn -> get_mbr();
            memcpy(de -> bounces, nmbr, 2*dimension*sizeof(float));
            delete [] nmbr;
            de -> son = sn -> block;
            de -> son_ptr = sn;
            nroot_ptr->enter(de);

            root = nroot;
            root_ptr = nroot_ptr;

            root_is_data = FALSE;
        }
        j++;
    }

    num_of_data++;

    delete [] re_level;
    delete root_ptr;
    root_ptr = NULL;
}

void RTree::load_root(){
    if (root_ptr == NULL)
        root_ptr = new RTNode(this, root);
}

void RTree::NNQuery(float *QueryPoint, SortedLinList *res){
      float nearest_distanz;

      // load root node into main memory
      load_root();
      nearest_distanz = MAXREAL;
      root_ptr->NNSearch(QueryPoint,res,&nearest_distanz);

      delete root_ptr;
      root_ptr = NULL;
}

int RTree::rangeQuery(float *mbr){
    load_root();
    int cnt=root_ptr -> rangeQuery(mbr);
    delete root_ptr;
    root_ptr = NULL;
    return cnt;
}

void RTree::read_header(char *buffer){
    int i;

    memcpy(&dimension, buffer, sizeof(dimension));
    i = sizeof(dimension);

    memcpy(&num_of_data, &buffer[i], sizeof(num_of_data));
    i += sizeof(num_of_data);

    memcpy(&num_of_dnodes, &buffer[i], sizeof(num_of_dnodes));
    i += sizeof(num_of_dnodes);

    memcpy(&num_of_inodes, &buffer[i], sizeof(num_of_inodes));
    i += sizeof(num_of_inodes);

    memcpy(&root_is_data, &buffer[i], sizeof(root_is_data));
    i += sizeof(root_is_data);

    memcpy(&root, &buffer[i], sizeof(root));
    i += sizeof(root);
}

void RTree::write_header(char *buffer){
    int i;

    memcpy(buffer, &dimension, sizeof(dimension));
    i = sizeof(dimension);

    memcpy(&buffer[i], &num_of_data, sizeof(num_of_data));
    i += sizeof(num_of_data);

    memcpy(&buffer[i], &num_of_dnodes, sizeof(num_of_dnodes));
    i += sizeof(num_of_dnodes);

    memcpy(&buffer[i], &num_of_inodes, sizeof(num_of_inodes));
    i += sizeof(num_of_inodes);

    memcpy(&buffer[i], &root_is_data, sizeof(root_is_data));
    i += sizeof(root_is_data);

    memcpy(&buffer[i], &root, sizeof(root));
    i += sizeof(root);
}




bool hilCompare(hilmbr *pt1, hilmbr *pt2) {
    return (pt1->hil_v < pt2->hil_v);
}


//bulkloads the tree by sorting the data points according to the hilbert values
//fname: the name of the dataset file
//dim: the dimensionality
//number of bits to compute the hilbert values

void RTree::bulkload(const char *_fname, int _num_bit){
    int i;
    vector<hilmbr *> mbrs;

    FILE *fp = fopen(_fname, "r");
    string msg = _fname;
    msg += "\n";
    msg = "could not open data source file " + msg;
    if (!fp)
        error((char*) msg.c_str(), true);

    int cnt = 0;
    while (!feof(fp)){
        hilmbr *mbr = new hilmbr();
        mbr->bounces = new float[2*dimension];
        mbr->block = -cnt;


        for (i=0; i<dimension; i++){
            fscanf(fp, " %f", &(mbr->bounces[2*i]));
            mbr->bounces[2*i+1] = mbr->bounces[2*i];

            if (cnt == 0 && i == 0){ // first value read
                minValue = maxValue = mbr->bounces[2*i];
            }
            if (mbr->bounces[2*i] > maxValue) maxValue = mbr->bounces[2*i];
            if (mbr->bounces[2*i] < minValue) minValue = mbr->bounces[2*i];

        }
        fscanf(fp, "\n"); // dsachar

        mbrs.push_back(mbr);

        cnt ++;
    }
    fclose(fp);

//    cerr << "minValue = " << minValue << "  maxValue = " << maxValue << endl;

    // calculate the hilbert values
    for (uint i=0; i<mbrs.size(); i++){
        mbrs[i]->hil_v = cal_hilbert_mbr(_num_bit, dimension, mbrs[i]->bounces, maxValue);
        if (mbrs[i]->hil_v < 0)
            error("error: negative hilbert values obtained.\n", true);
    }

    // printf("sorting the dataset...\n");
    sort(mbrs.begin(), mbrs.end(), hilCompare);


    int level = 0;
    int mbr_num = mbrs.size();

    while (level == 0 || mbr_num > 1){
        cerr << "grouping " << mbr_num << " objects at level " << level << endl;
        int new_mbr_num = 0;

        RTNode *nd = new RTNode(this);
        nd->level = level;

        if (level == 0)
            num_of_dnodes ++;
        else
            num_of_inodes ++;


        for (i = 0; i < mbr_num; i++){
            memcpy(nd->entries[nd->num_entries].bounces, mbrs[i]->bounces, 2 * sizeof(float) * dimension);
            nd->entries[nd->num_entries].son = mbrs[i]->block;

            nd->num_entries ++;
            if (nd->num_entries == nd->capacity){
                float *ndmbr = nd->get_mbr();
                memcpy(mbrs[new_mbr_num]->bounces, ndmbr, 2 * sizeof(float) * dimension);
                mbrs[new_mbr_num]->block = nd->block;
                mbrs[new_mbr_num]->hil_v = cal_hilbert_mbr(_num_bit, dimension, mbrs[new_mbr_num]->bounces, maxValue);
                new_mbr_num ++;

                delete[] ndmbr;
                delete nd;
                nd = new RTNode(this);
                nd->level = level;
//                cerr << "nd->level = " << nd->level << endl;
                if (level == 0)
                    num_of_dnodes ++;
                else
                    num_of_inodes ++;
            }
        }
        float *ndmbr = nd->get_mbr();
        memcpy(mbrs[new_mbr_num]->bounces, ndmbr, 2 * sizeof(float) * dimension);
        mbrs[new_mbr_num]->block = nd->block;
        mbrs[new_mbr_num]->hil_v = cal_hilbert_mbr(_num_bit, dimension, mbrs[new_mbr_num]->bounces, maxValue);
        new_mbr_num ++;
        delete []ndmbr;
        delete nd;

        level ++;
        mbr_num = new_mbr_num;
        sort(mbrs.begin(), mbrs.begin()+mbr_num, hilCompare);
    }
//    cerr << "level = " << level << endl;

    root = mbrs[0]->block;
    num_of_data = mbrs.size();

    //printf("freeing up the memory...\n");
    for (i=0; i<mbrs.size(); i++){
        delete[] mbrs[i]->bounces;
        delete mbrs[i];
    }
    mbrs.clear();
}

