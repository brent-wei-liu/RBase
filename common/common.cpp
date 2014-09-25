#include "pch.h"

string getDirectory(string dataset_file_name){
    size_t lastDir = dataset_file_name.find_last_of("/");
    if (lastDir == string::npos){
        lastDir = 0;
    }
    else {
        lastDir++;
    }
    return dataset_file_name.substr(0, lastDir);
}

string getFileName(string dataset_file_name){
    size_t lastDir = dataset_file_name.find_last_of("/");
    if (lastDir == string::npos){
        lastDir = 0;
    }
    else {
        lastDir++;
    }
    return dataset_file_name.substr(lastDir);
}


string getDirSuffix(string fileName){
    ostringstream dirSuffixStr;
    dirSuffixStr << fileName << ".dir";
//    dirSuffixStr << ""; // same directory as data set
    return dirSuffixStr.str();
}


string getOutDirectory(string directory, string prodFileName, string buyFileName, string method){
    string prodDirSuffix = getDirSuffix(prodFileName);
    string buyDirSuffix = getDirSuffix(buyFileName);

    ostringstream outDirStr;
    outDirStr << directory << prodDirSuffix;
    create_directory(outDirStr.str().c_str());
    outDirStr << "/" << buyDirSuffix;
    create_directory(outDirStr.str().c_str());
    outDirStr << "/" << method;
    create_directory(outDirStr.str().c_str());
    outDirStr << "/";

    return outDirStr.str();
}


string getOutDirectory(string dataset_file_name, string method){
    string directory, fileName, dirSuffix;
    directory = getDirectory(dataset_file_name);
    fileName = getFileName(dataset_file_name);
    dirSuffix = getDirSuffix(fileName);

    ostringstream outDirStr;
    outDirStr << directory << dirSuffix;
//    CreateDirectory(outDirStr.str().c_str(), NULL);
    create_directory(outDirStr.str().c_str());
    outDirStr << "/" << method;
//    CreateDirectory(outDirStr.str().c_str(), NULL);
    create_directory(outDirStr.str().c_str());
    outDirStr << "/";

    return outDirStr.str();
}

