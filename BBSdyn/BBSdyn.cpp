#include "../common/pch.h"


// constructor
BBSdyn::BBSdyn(string dataset_file_name, string query_file_name, int numQueries, int cacheSize, int dimension, int bufferSize){
    this->dataset_file_name = dataset_file_name;
    this->query_file_name = query_file_name;
    this->dimension = dimension;
    this->numQueries = numQueries;
    this->cacheSize = cacheSize;
    this->bufferSize = bufferSize;

    dataFileName = getFileName(dataset_file_name);
    dataFileNamePath = getDirectory(dataset_file_name);
    outDirectory = getOutDirectory(dataset_file_name, "BBS");
    cerr << "dataFileNamePath = " << dataFileNamePath << endl;
    cerr << "dataFileName = " << dataFileName << endl;
    cerr << "outDirectory = " << outDirectory << endl;

    // initialize statistics
    numResults = 0;
    accesses = 0;

    initialize();
}

BBSdyn::~BBSdyn(){
}

void BBSdyn::initialize(){

}

void BBSdyn::execute(int method, bool withCache){

    string rtreefilename = outDirectory + ".rtree";
    cerr << "rtreefilename = " << rtreefilename << endl;

    totim = new Timing();
    totim->startClock();

    ifstream fin(rtreefilename.c_str(), ios::in);
    if (fin.fail()){
        cerr << "creating R-tree" << endl;
        Timing *tim = new Timing();
        tim->startClock();
        RT = new BBSdyn_RTree(this, rtreefilename.c_str(), 4096, NULL, dimension, bufferSize);
        RT->bulkload(dataset_file_name.c_str(), 15);
        int build_time = tim->stopClock();
        delete tim;
        cerr << "build time = " << build_time << endl;
    }
    else{
        cerr << "loading R-tree" << endl;
        RT = new BBSdyn_RTree(this, rtreefilename.c_str(), NULL, bufferSize);
    }
    fin.close();
    cerr << "R-tree in memory" << endl;


    ifstream qin(query_file_name.c_str(), ios::in);

    float* query = new float[dimension];


    // initialize cache
    for (int i=0; i<cacheSize; i++){

        for (int d=0; d<dimension; d++){
            qin >> query[d];
        }

        if (withCache){

            // cache query point
            for (int d=0; d<dimension; d++){
                cachedQueries.push_back(query[d]);
            }

            if (method == DYN){
                // execute query
                result.clear();
                skyline(query, method, false);

                // cache up to K result points
                int K = 0;
                vector<float>::iterator iter = result.begin();
                while (K>0 && iter!=result.end()){
                    for (int d=0; d<dimension; d++){
                        cachedResults.push_back(*iter);
                        iter++;
                    }
                    K--;
                }
            }
        }
    }


    resetStatistics();

    // Skyline Queries
    Timing *tim = new Timing();
    tim->startClock();

    for (int i=0; i<numQueries; i++){
//        cerr << "query " << (i+1) << endl;
        result.clear();
//        result = cachedResults;

        for (int d=0; d<dimension; d++){
            qin >> query[d];
        }

        skyline(query, method, withCache);
//        printResults();

    }

    query_time = tim->stopClock();
    delete tim;
    cerr << "queries done!" << endl;

    qin.close();

    delete query;

    numLeafNodes = RT->num_of_dnodes;
    numInternalNodes = RT->num_of_inodes;

//    cerr << "flushing R-tree to disk..." << endl;
    delete RT;

    total_time = totim->stopClock();
    delete totim;
}


void BBSdyn::skyline(float* query, int domType, bool withCache){

//    cerr << "query: ";
//    for (uint i=0; i<dimension; i++){
//        cerr << query[i] << " ";
//    }
//    cerr << endl;

    XxkHeap *hp = new XxkHeap();
    RT->skyline(hp, query, domType, withCache);
    numResults += result.size()/(dimension);
//    cerr << "numResults = " << numResults << endl;
    delete hp;
//    printResults();
}


string BBSdyn::getStatistics(int domType){
    ostringstream sout;

    if (domType == DYN){
        sout << "----------------- BBSdyn -----------------" << endl;
    }
    else if (domType == ORTH){
        sout << "----------------- BBSorth -----------------" << endl;
    }
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

void BBSdyn::resetStatistics(){
    numResults = 0;
    accesses = 0;
}


void BBSdyn::printResults(){
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

