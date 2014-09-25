#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


typedef unsigned int uint;


using namespace std;

void Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


ofstream fout;

void insertParameterValue(string value){
	fout << "echo " << value << " \t >> results/falseHits.txt" << endl;
	fout << "echo " << value << " \t >> results/time.txt" << endl;
	fout << "echo " << value << " \t >> results/totime.txt" << endl;
	fout << "echo " << value << " \t >> results/IOs.txt" << endl;
	fout << "echo " << value << " \t >> results/skylineSize.txt" << endl;
	fout << "echo " << value << " \t >> results/nLeaves.txt" << endl;
	fout << "echo " << value << " \t >> results/nInternal.txt" << endl;
}

void insertSetting(string parameter, string methodsToRun){
	fout << "echo " << parameter << " \t " << methodsToRun << " >> results/falseHits.txt" << endl;
	fout << "echo " << parameter << " \t " << methodsToRun << " >> results/time.txt" << endl;
	fout << "echo " << parameter << " \t " << methodsToRun << " >> results/totime.txt" << endl;
	fout << "echo " << parameter << " \t " << methodsToRun << " >> results/IOs.txt" << endl;
	fout << "echo " << parameter << " \t " << methodsToRun << " >> results/skylineSize.txt" << endl;
	fout << "echo " << parameter << " \t " << methodsToRun << " >> results/nLeaves.txt" << endl;
	fout << "echo " << parameter << " \t" << methodsToRun << " >> results/nInternal.txt" << endl;
}


int main(int argc, char** argv){

	if (argc!=4){
		cerr << "Usage: " << argv[0] << " [executable] [generatorExec] [parameterFile]" << endl;
		exit(0);
	}

	string executable = argv[1];
	string generatorExec = argv[2];
	string parameterFilename = argv[3];

	ifstream fin(parameterFilename.c_str(), ios::in);

	ofstream foutd("createDatasets.bat", ios::out);
	fout.open("batchRun.bat", ios::out);

	// which methods to run
	string methodsToRun;

	// maxValue in each dimension
	int maxValue;

	// dataset correlation type
	vector<string> correl;

	// dataset cardinality
	string defCard;
	vector<string> card;

	// lattice set cardinality
	string defSetCard;
	vector<string> setCard;

	// lattice node probability
	string defProb;
	vector<string> prob;

	// TO dimensionality
	int defTOdim;
	vector<int> TOdim;

	// PO dimensionality
	int defPOdim;
	vector<int> POdim;


	// 1. read parameters
	getline(fin, methodsToRun); // methods
	string temp;

	getline(fin, temp); // maxValue
	maxValue = atoi(temp.c_str());

	getline(fin, temp); // correlation
	Tokenize(temp, correl);

	getline(fin, temp); // cardinalities
	Tokenize(temp, card);
	defCard = card[0];
	card.erase(card.begin());

	getline(fin, temp); // lattice set cardinalities
	Tokenize(temp, setCard);
	defSetCard = setCard[0];
	setCard.erase(setCard.begin());

	getline(fin, temp); // lattice node probabilities
	Tokenize(temp, prob);
	defProb = prob[0];
	prob.erase(prob.begin());

	getline(fin, temp); // TO dimensionalities
	vector<string> dummy;
	Tokenize(temp, dummy);
	defTOdim = atoi(dummy[0].c_str());
	for (int i=1; i<dummy.size(); i++){
		TOdim.push_back(atoi(dummy[i].c_str()));
	}
	dummy.clear();

	getline(fin, temp); // PO dimensionalities
	Tokenize(temp, dummy);
	defPOdim = atoi(dummy[0].c_str());
	for (int i=1; i<dummy.size(); i++){
		POdim.push_back(atoi(dummy[i].c_str()));
	}
	dummy.clear();

	fin.close();


	// 2. create data sets
	for (uint i=0; i<correl.size(); i++){
		// create default data set
		foutd << generatorExec << " -" << correl[i] << " -d " << (defTOdim+defPOdim) << " -t 1 -M " << maxValue << " -n " << defCard << " -P" << endl;

		// vary cardinality
		for (uint j=0; j<card.size(); j++){
			foutd << generatorExec << " -" << correl[i] << " -d " << (defTOdim+defPOdim) << " -t 1 -M " << maxValue << " -n " << card[j] << " -P" << endl;
		}

		// vary TO dim
		for (uint j=0; j<TOdim.size(); j++){
			foutd << generatorExec << " -" << correl[i] << " -d " << (TOdim[j]+defPOdim) << " -t 1 -M " << maxValue << " -n " << defCard << " -P" << endl;
		}

		// vary PO dim
		for (uint j=0; j<POdim.size(); j++){
			foutd << generatorExec << " -" << correl[i] << " -d " << (defTOdim+POdim[j]) << " -t 1 -M " << maxValue << " -n " << defCard << " -P" << endl;
		}
	}
	foutd.close();



	// 3. execute!
	for (uint i=0; i<correl.size(); i++){
		ostringstream sout;
		fout << "echo DISTRIBUTION " << correl[i] << endl;
		sout << "DISTRIBUTION " << correl[i];
		insertSetting(sout.str(), methodsToRun);
		// execute default scenario
		insertSetting("DEFAULT SCENARIO", methodsToRun);
		ostringstream dataFileName;
		dataFileName << "datasets/" << correl[i] << "D" << (defTOdim+defPOdim) << "T1M" << maxValue << "C" << defCard;
		fout << "echo DEFAULT scenario;" << endl;
		fout << executable << " " << dataFileName.str() << " " << maxValue << " " << defSetCard << " " << defProb << " ";
		fout << defTOdim << " " << defPOdim << " " << defCard << " " << correl[i] << " def " << methodsToRun << endl;

		if (card.size()>0){
			fout << "echo VARY CARDINALITY;" << endl;
			insertSetting("N", methodsToRun);
		}
		for (uint j=0; j<card.size(); j++){
//			insertParameterValue(card[j]);
			ostringstream dataFileName;
			dataFileName << "datasets/" << correl[i] << "D" << (defTOdim+defPOdim) << "T1M" << maxValue << "C" << card[j];
			fout << executable << " " << dataFileName.str() << " " << maxValue << " " << defSetCard << " " << defProb << " ";
			fout << defTOdim << " " << defPOdim << " " << card[j] << " " << correl[i] << " " << card[j] << " " << methodsToRun << endl;
		}

		if (TOdim.size()>0){
			fout << "echo VARY TO_dim;" << endl;
			insertSetting("TO_dim", methodsToRun);
		}
		for (uint j=0; j<TOdim.size(); j++){
			ostringstream theTOdim;
			theTOdim << TOdim[j];
//			insertParameterValue(theTOdim.str());
			ostringstream dataFileName;
			dataFileName << "datasets/" << correl[i] << "D" << (TOdim[j]+defPOdim) << "T1M" << maxValue << "C" << defCard;
			fout << executable << " " << dataFileName.str() << " " << maxValue << " " << defSetCard << " " << defProb << " ";
			fout << TOdim[j] << " " << defPOdim << " " << defCard << " " << correl[i] << " " << TOdim[j] << " " << methodsToRun << endl;
		}

		if (POdim.size()>0){
			fout << "echo VARY PO_dim;" << endl;
			insertSetting("PO_dim", methodsToRun);
		}
		for (uint j=0; j<POdim.size(); j++){
			ostringstream thePOdim;
			thePOdim << POdim[j];
//			insertParameterValue(thePOdim.str());
			ostringstream dataFileName;
			dataFileName << "datasets/" << correl[i] << "D" << (defTOdim+POdim[j]) << "T1M" << maxValue << "C" << defCard;
			fout << executable << " " << dataFileName.str() << " " << maxValue << " " << defSetCard << " " << defProb << " ";
			fout << defTOdim << " " << POdim[j] << " " << defCard << " " << correl[i] << " " << POdim[j] << " " << methodsToRun << endl;
		}

		if (setCard.size()>0){
			fout << "echo VARY SET_CARD;" << endl;
			insertSetting("sc", methodsToRun);
		}
		for (uint j=0; j<setCard.size(); j++){
//			insertParameterValue(setCard[j]);
			ostringstream dataFileName;
			dataFileName << "datasets/" << correl[i] << "D" << (defTOdim+defPOdim) << "T1M" << maxValue << "C" << defCard;
			fout << executable << " " << dataFileName.str() << " " << maxValue << " " << setCard[j] << " " << defProb << " ";
			fout << defTOdim << " " << defPOdim << " " << defCard << " " << correl[i] << " " << setCard[j] << " " << methodsToRun << endl;
		}

		if (prob.size()>0){
			fout << "echo VARY NODE_PROB;" << endl;
			insertSetting("p", methodsToRun);
		}
		for (uint j=0; j<prob.size(); j++){
//			insertParameterValue(prob[j]);
			ostringstream dataFileName;
			dataFileName << "datasets/" << correl[i] << "D" << (defTOdim+defPOdim) << "T1M" << maxValue << "C" << defCard;
			fout << executable << " " << dataFileName.str() << " " << maxValue << " " << defSetCard << " " << prob[j] << " ";
			fout << defTOdim << " " << defPOdim << " " << defCard << " " << correl[i] << " " << prob[j] << " " << methodsToRun << endl;
		}


	}
	fout.close();


}
