#ifndef COMMON_H_
#define COMMON_H_


#define BBSDYN 0
#define BBSORTH 1

typedef unsigned int uint;

string getDirectory(string dataset_file_name);
string getFileName(string dataset_file_name);
string getDirSuffix(string fileName);
string getOutDirectory(string directory, string prodFileName, string buyFileName, string method);
string getOutDirectory(string dataset_file_name, string method);


#endif /*COMMON_H_*/
