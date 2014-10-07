#include "pch.h"





int main(int argc, char** argv){

    // parameters
    float maxValue;
    string dataset_file_R;
    string dataset_file_S;
    string query_file_name;
    int dimension;
    int data_cardinality;
    int numQueries;
    char dataset_type;
    vector<string> methods;
    int cacheSize;
    bool withCache = false;
    int bufferSize;


    bool runBBS = false;


/*
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
             ("help", "produce help message")
            ("input,I", po::value<string>(&dataset_file_name), "input dataset")
            ("query,Q", po::value<string>(&query_file_name), "query dataset")
            ("dimension,d", po::value<int>(&dimension), "dimensionality")
            ("cardinality,n", po::value<int>(&data_cardinality), "dataset cardinality")
            ("numQueries,m", po::value<int>(&numQueries), "number of queries")
            ("maxValue,M", po::value<float>(&maxValue), "dataset maximum Value")
            ("cacheSize,k", po::value<int>(&cacheSize)->default_value(0), "cache Size")
            ("distribution", po::value<char>(&dataset_type)->default_value('u'), "dataset distribution")
            ("bufferSize,b", po::value<int>(&bufferSize)->default_value(1), "R-tree buffer size")
            ("withCache,C", "turn cache on")
            ("methods", po::value< vector<string> >(&methods), "methods")
        ;

        po::positional_options_description p;
        p.add("methods", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << "Usage: options_description [options]" << endl;
            cout << desc;
            return 0;
        }

        if (vm.count("withCache")) {
            withCache = true;
        }

        if (!vm.count("input")) {
            cerr << "You must specify an input file." << endl;
            exit(0);
        }

        if (!vm.count("query")) {
            cerr << "You must specify a query file." << endl;
            exit(0);
        }

        if (!vm.count("dimension")) {
            cerr << "You must specify the dimensionality." << endl;
            exit(0);
        }

        if (!vm.count("cardinality")) {
            cerr << "You must specify the cardinality." << endl;
            exit(0);
        }

        if (!vm.count("numQueries")) {
            cerr << "You must specify the number of queries." << endl;
            exit(0);
        }

        if (!vm.count("maxValue")) {
            cerr << "You must specify the maxValue." << endl;
            exit(0);
        }

        if (vm.count("methods")) {
            vector<string>::iterator it;
            for (it=methods.begin(); it!=methods.end(); it++) {
                if ((*it).compare("BBS")==0) {
                    runBBS = true;
                }
                   if ((*it).compare("BBSdyn")==0) {
                        runBBSdyn = true;
                }
                   if ((*it).compare("BBSorth")==0) {
                        runBBSorth = true;
                }
            }
        }
        else{
            cerr << "You must specify at least one method." << endl;
            exit(0);
        }


    }
    catch(std::exception& e)
    {
        cout << e.what() << "\n";
        return 1;
    }

*/
    runBBS = true;
    maxValue = 100000;
    dataset_file_R = "./datasets/R.txt";
    dataset_file_S = "./datasets/S.txt";
    dimension = 2;
    data_cardinality = 2;
    dataset_type = 'u';






    // result files
    string resultsDir = "results"; // hard-coded
    string timeFilename, totimeFilename, iosFilename, sizeFilename, nLeavesFilename, nInternalFilename;
    timeFilename = resultsDir + "/time.txt";
    totimeFilename = resultsDir + "/totime.txt";
    iosFilename = resultsDir + "/IOs.txt";
    sizeFilename = resultsDir + "/skylineSize.txt";
    nLeavesFilename = resultsDir + "/nLeaves.txt";
    nInternalFilename = resultsDir + "/nInternal.txt";

    ofstream timeOut(timeFilename.c_str(), ios::app);
    ofstream totimeOut(totimeFilename.c_str(), ios::app);
    ofstream iosOut(iosFilename.c_str(), ios::app);
    ofstream sizeOut(sizeFilename.c_str(), ios::app);
    ofstream nLeavesOut(nLeavesFilename.c_str(), ios::app);
    ofstream nInternalOut(nInternalFilename.c_str(), ios::app);



    // BBS
    if (runBBS){
        BBS *bbs = new BBS(maxValue, dataset_file_R, dataset_file_S, dimension, data_cardinality, dataset_type);
        cerr << "Executing BBS..." << endl;
        bbs->execute();
//        cerr << "Outputing statistics..." << endl;
        cerr << bbs->getStatistics();
        timeOut << bbs->query_time << "\t";
        totimeOut << bbs->total_time << "\t";
        iosOut << bbs->accesses << "\t";
        sizeOut << bbs->numResults << "\t";
        nLeavesOut << bbs->numLeafNodes << "\t";
        nInternalOut << bbs->numInternalNodes << "\t";
        timeOut.flush();
        totimeOut.flush();
        iosOut.flush();
        sizeOut.flush();
        nLeavesOut.flush();
        nInternalOut.flush();
    }




    timeOut << endl;
    totimeOut << endl;
    iosOut << endl;
    sizeOut << endl;
    nLeavesOut << endl;
    nInternalOut << endl;

    timeOut.close();
    totimeOut.close();
    iosOut.close();
    sizeOut.close();
    nLeavesOut.close();
    nInternalOut.close();

}
