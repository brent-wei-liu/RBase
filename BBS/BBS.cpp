#include "../common/pch.h"


// constructor
BBS::BBS(float maxValue, string dataset_file_name, int dimension, int data_cardinality, char dataset_type){
	this->maxValue = maxValue;
	this->dataset_file_name = dataset_file_name;
	this->dimension = dimension;
	this->data_cardinality = data_cardinality;
	this->dataset_type = dataset_type;

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

BBS::~BBS(){
}

void BBS::initialize(){
}

void BBS::execute(){

	string rtreefilename = outDirectory + ".rtree";

	totim = new Timing();
	totim->startClock();

	ifstream fin(rtreefilename.c_str(), ios::in);
	if (fin.fail()){
		cerr << "creating R-tree" << endl;
		Timing *tim = new Timing();
		tim->startClock();
		RT = new BBS_RTree(this, rtreefilename.c_str(), 4096, NULL, dimension);
		RT->bulkload(dataset_file_name.c_str(), 15);
		int build_time = tim->stopClock();
		delete tim;
		cerr << "build time = " << build_time << endl;
	}
	else{
		cerr << "loading R-tree" << endl;
		RT = new BBS_RTree(this, rtreefilename.c_str(), NULL);
	}
	fin.close();
	cerr << "R-tree in memory" << endl;


	// Skyline Query
	Timing *tim = new Timing();
	tim->startClock();
//	unsigned __int64 temp = pi.cpu_time_ms();
	skyline();
//	query_time = pi.cpu_time_ms() - temp;
	query_time = tim->stopClock();
	delete tim;
	cerr << "query done!" << endl;

	numLeafNodes = RT->num_of_dnodes;
	numInternalNodes = RT->num_of_inodes;

//	cerr << "flushing R-tree to disk..." << endl;
	delete RT;

	total_time = totim->stopClock();
	delete totim;
}

void BBS::skyline(){
	XxkHeap *hp = new XxkHeap();
	RT->skyline(hp);
	numResults = result.size()/(dimension);
	delete hp;
//	printResults();
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

