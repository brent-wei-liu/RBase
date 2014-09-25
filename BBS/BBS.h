#ifndef BBS_H_
#define BBS_H_


class BBS{
public:
    float maxValue;
    int set_cardinality;
    float node_prob;
    string dataset_file_name;
    int dimension;
    int data_cardinality;
    char dataset_type;

    // derived
    string dataFileName; // filename of the dataset
    string dataFileNamePath; // path to dataset file
    string outDirectory; // directory to output method related files

    // constructed
    BBS_RTree *RT;

    // results
    vector<float> result;

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
    BBS(float maxValue, string dataset_file_name, int dimension, int data_cardinality, char dataset_type);
    ~BBS();



    void initialize();
    void execute();
    void skyline();

    string getStatistics();
    void printResults();
};

#endif /*BBS_H_*/
