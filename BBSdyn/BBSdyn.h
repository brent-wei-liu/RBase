#ifndef BBSDYN_H_
#define BBSDYN_H_



class BBSdyn{
public:
    int set_cardinality;
    float node_prob;
    string dataset_file_name;
    string query_file_name;
    int numQueries;
    int cacheSize;
    int dimension;
    int bufferSize;

    // derived
    string dataFileName; // filename of the dataset
    string dataFileNamePath; // path to dataset file
    string outDirectory; // directory to output method related files

    // constructed
    BBSdyn_RTree *RT;

    // results
    vector<float> result;

    // cache
    vector<float> cachedQueries;
    vector<float> cachedResults;


    // statistics
    Timing *totim;
    int numResults;
    int accesses;
    long query_time;
    long total_time;
    int numLeafNodes;
    int numInternalNodes;


    // METHODS

    // con- (de-) structors
    BBSdyn(string dataset_file_name, string query_file_name, int numQueries, int cacheSize, int dimension, int bufferSize);
    ~BBSdyn();



    void initialize();
    void execute(int method, bool withCache);
    void skyline(float* query, int domType, bool withCache);

    void resetStatistics();

    string getStatistics(int domType);
    void printResults();
};

#endif /*BBSDYN_H_*/
