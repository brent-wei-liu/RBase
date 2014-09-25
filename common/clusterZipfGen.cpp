/*
 * clusterZipfGen.cpp
 *
 *  Created on: Feb 25, 2010
 *      Author: dsachar
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>


#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;


int main(int argc, char** argv){

	// parameters
	float maxValue;
	int dimension;
	int cardinality;
	int numClusters;
	float clusterSpread;
	float zipf;
	long seed;


	try {
		po::options_description desc("Allowed options");
		desc.add_options()
 			("help", "produce help message")
			("numClusters,c", po::value<int>(&numClusters), "number of clusters")
			("dimension,d", po::value<int>(&dimension), "dimensionality")
			("cardinality,n", po::value<int>(&cardinality), "cardinality")
			("maxValue,M", po::value<float>(&maxValue), "maximum Value")
			("clusterSpread,s", po::value<float>(&clusterSpread)->default_value(0.001f), "cluster spread as fraction of maximum value")
			("zipf,z", po::value<float>(&zipf)->default_value(0.8f), "zipf value")
			("seed,S", po::value<long>(&seed)->default_value(123), "seed for random generator")
        ;


        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
        	cout << "Usage: options_description [options]" << endl;
        	cout << desc;
        	return 0;
        }

        if (!vm.count("numClusters")) {
        	cerr << "You must specify the number of clusters." << endl;
        }

        if (!vm.count("dimension")) {
        	cerr << "You must specify the dimensionality." << endl;
        }

        if (!vm.count("cardinality")) {
        	cerr << "You must specify the cardinality." << endl;
        }

        if (!vm.count("maxValue")) {
        	cerr << "You must specify the maxValue." << endl;
        }


	}
    catch(std::exception& e)
    {
        cout << e.what() << "\n";
        return 1;
    }


    srand(seed);
    srand48(seed);


    ostringstream filestream;

    filestream << "zD" << dimension << "M" << maxValue << "C" << cardinality << "K" << numClusters << "V" << clusterSpread << "Z" << zipf << "S" << seed;

	ofstream fout(filestream.str().c_str(), ios::out);
	if (fout.fail()) {
		cerr << "could not create file " << filestream.str() << endl;
		exit(0);
	}


	float **centers = new float*[numClusters];
	for (int i=0; i<numClusters; i++) {
		centers[i] = new float[dimension];
//		cerr << "cluster center " << i << " : ";
		for (int d=0; d<dimension; d++) {
			centers[i][d] = rand() % (int) maxValue;
//			cerr << centers[i][d] << " ";
		}
//		cerr << endl;
	}

	float clusterSide = clusterSpread * maxValue;


	float *probs = new float[numClusters];

	float sum = 0;
	for (int i=0; i<numClusters; i++){
		probs[i] = 1/pow((float) i+1, zipf);
		sum += probs[i];
	}

	// normalize so that the sum is 1
	for (int i=0; i<numClusters; i++){
		probs[i] = probs[i]/sum;
	}


	int *size = new int[numClusters];
	for (int i=0; i<numClusters; i++){
		size[i] = ((int) (probs[i] * cardinality)) + 1;
	}


	int *stats = new int[numClusters];
	for (int i=0; i<numClusters; i++){
		stats[i] = 0;
	}


	int cluster = 0;
	int count = 0;

	for (int i=0; i<cardinality; i++){

		if (count == size[cluster]) {
			cluster++;
			count = 0;
		}
		if (cluster == numClusters) cluster--;

		stats[cluster]++;

		for (int d=0; d<dimension; d++) {
			float frc = rand() * 2.0f / RAND_MAX - 1.0f;
			if (cluster == numClusters-1) clusterSide = 0;
			int value = (int) ( (float) centers[cluster][d] + frc*clusterSide);
			if (value<0) value = 0;
			if (value>maxValue) value = maxValue;
			if (d>0) fout << " ";
			fout << value;
		}

		count++;
		fout << endl;
	}


//	for (int i=0; i<numClusters; i++){
//		cerr << "cluster " << i << " size = " << stats[i] << endl;
//	}


	fout.close();

}




