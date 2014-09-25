/*
 * zipfGen.cpp
 *
 *  Created on: Feb 24, 2010
 *      Author: dsachar
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>


using namespace std;

int main(int argc, char **argv){

	if (argc!=6){
		cout << "Usage: " << argv[0] << " [dimensionality] [cardinality] [maxValue] [skew] [seed]" << endl;
		exit(0);
	}


	int dimension = atoi(argv[1]);
	int cardinality = atoi(argv[2]);
	int maxValue = atoi(argv[3]);
	float skew = atof(argv[4]);
	int seed = atoi(argv[5]);

	ostringstream filename;
	filename << "zD" << dimension << "M" << maxValue << "C" << cardinality << "Z" << skew << "S" << seed;

	ofstream fout;
	fout.open (filename.str().c_str(), ofstream::out);

	float *probs = new float[maxValue];

	float sum = 0;
	for (int i=0; i<maxValue; i++){
		probs[i] = 1/pow((float) i+1, skew);
		sum += probs[i];
	}

	// normalize so that the sum is 1
	for (int i=0; i<maxValue; i++){
		probs[i] = probs[i]/sum;
	}

	vector<int>* permutations = new vector<int>[dimension];
	for (int i=0; i<maxValue; i++){
		for (int j=0; j<dimension; j++){
			permutations[j].push_back(i);
		}
	}
	for (int j=0; j<dimension; j++){
		random_shuffle(permutations[j].begin(), permutations[j].end());
	}


	srand48(seed); // random seed

	for (int t=0; t<cardinality; t++){
		if (t>0) fout << endl;
		for (int d=0; d<dimension; d++){
			double rnd = drand48();
			int index = 0;
			float sum = probs[0];
			while(sum<rnd && index<maxValue){
				sum += probs[index];
				index++;
			}
			if (index==maxValue) index--;
			if (d==0){
				fout << permutations[d][index];
			}
			else{
				fout << " " << permutations[d][index];
			}
		}
	}


}

