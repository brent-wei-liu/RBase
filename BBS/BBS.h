#ifndef BBS_H_
#define BBS_H_
class BBS{
public:
    float maxValue;
    int set_cardinality;
    float node_prob;
    string dataset_file_R;
    string dataset_file_S;
    int dimension;
    int data_cardinality;
    char dataset_type;

    // derived
    string dataRFileName; // filename of the dataset
    string dataRFileNamePath; // path to dataset file
    string dataSFileName; // filename of the dataset
    string dataSFileNamePath; // path to dataset file
    string outDirectoryR; // directory to output method related files
    string outDirectoryS; // directory to output method related files

    // constructed
    BBS_RTree *RTR;
    BBS_RTree *RTS;

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
    BBS(float maxValue, string datasetR, string datasetS, int dimension, int data_cardinality, char dataset_type);
    ~BBS();



    void initialize();
    void execute();
    void skyline();

    string getStatistics();
    void printResults();
};

#endif /*BBS_H_*/
