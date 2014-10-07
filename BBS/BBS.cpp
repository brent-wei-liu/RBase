#include "../common/pch.h"


// constructor
BBS::BBS(float maxValue, string datasetR,string datasetS, int dimension, int data_cardinality, char dataset_type){
    this->maxValue = maxValue;
    this->dataset_file_R = datasetR;
    this->dataset_file_S = datasetS;
    this->dimension = dimension;
    this->data_cardinality = data_cardinality;
    this->dataset_type = dataset_type;

    dataRFileName = getFileName(dataset_file_R);
    dataRFileNamePath = getDirectory(dataset_file_R);
    outDirectoryR = getOutDirectory(dataset_file_R, "BBS");
    cerr << "dataRFileNamePath = " << dataRFileNamePath << endl;
    cerr << "dataRFileName = " << dataRFileName << endl;
    cerr << "outDirectoryR = " << outDirectoryR << endl;

    dataSFileName = getFileName(dataset_file_S);
    dataSFileNamePath = getDirectory(dataset_file_S);
    outDirectoryS = getOutDirectory(dataset_file_S, "BBS");
    cerr << "dataSFileNamePath = " << dataSFileNamePath << endl;
    cerr << "dataSFileName = " << dataSFileName << endl;
    cerr << "outDirectoryS = " << outDirectoryS << endl;

    // initialize statistics
    numResults = 0;
    accesses = 0;

    initialize();
}

BBS::~BBS(){
}

void BBS::initialize()
{

}


void BBS::execute(){

    string rtrfilename = outDirectoryR + ".rtree";
    string rtsfilename = outDirectoryS + ".rtree";

    totim = new Timing();
    totim->startClock();

    ifstream finr(rtrfilename.c_str(), ios::in);
    if (finr.fail()){
        cerr << "creating R-tree" << endl;
        Timing *tim = new Timing();
        tim->startClock();
        RTR = new BBS_RTree(this, rtrfilename.c_str(),128, NULL, dimension);
        RTR->bulkload(dataset_file_R.c_str(), 15);
        int build_time = tim->stopClock();
        delete tim;
        cerr << "build time = " << build_time << endl;
    }
    else{
        cerr << "loading R-tree" << endl;
        RTR = new BBS_RTree(this, rtrfilename.c_str(), NULL);
    }
    finr.close();
    cerr << "R-tree in memory" << endl;
    RTR->printTree();

    ifstream fins(rtsfilename.c_str(), ios::in);
    if (fins.fail()){
        cerr << "creating R-tree" << endl;
        Timing *tim = new Timing();
        tim->startClock();
        RTS = new BBS_RTree(this, rtsfilename.c_str(),128, NULL, dimension);
        RTS->bulkload(dataset_file_S.c_str(), 15);
        int build_time = tim->stopClock();
        delete tim;
        cerr << "build time = " << build_time << endl;
    }
    else{
        cerr << "loading R-tree" << endl;
        RTS = new BBS_RTree(this, rtsfilename.c_str(), NULL);
    }
    fins.close();
    cerr << "R-tree in memory" << endl;
    RTS->printTree();

    // Skyline Query
    Timing *tim = new Timing();
    tim->startClock();
//    unsigned __int64 temp = pi.cpu_time_ms();
//    skyline();
//    query_time = pi.cpu_time_ms() - temp;
    float epsilon = 1.0;
    RTR->RTreeSpatialJoin3( RTS , epsilon);
    query_time = tim->stopClock();
    delete tim;
    cerr << "query done!" << endl;

    numLeafNodes = RTR->num_of_dnodes;
    numInternalNodes = RTR->num_of_inodes;

//    cerr << "flushing R-tree to disk..." << endl;
    delete RTR;
    delete RTS;
    total_time = totim->stopClock();
    delete totim;
}



string BBS::getStatistics(){
    ostringstream sout;

    sout << "----------------- BBS -----------------" << endl;
    sout << ":: R-Tree Info ::" << endl;
    sout << "(leaf nodes, internal nodes) in R-Tree = (" << numLeafNodes << ", " << numInternalNodes << ")" << endl;

    sout << ":: Skyline Info ::" << endl;
    sout << "Number of Results = " << numResults << endl;
    sout << "Node accesses = " << accesses << endl;
    sout << "Query CPU time = " << query_time << endl;
    sout << "Total CPU time = " << total_time << endl;
    sout << "---------------------------------------" << endl;

    return sout.str();
}

void BBS::printResults(){
    int dim = dimension;
    vector<float>::iterator iti = result.begin();
    while (iti != result.end()){
        for(int j=0;j<dim;j++){
            cerr << *iti << " ";
            iti++;
        }
        cerr << endl;
    }
    cerr << "skyline size = " << result.size()/dim << endl;
}

