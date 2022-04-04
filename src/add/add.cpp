#include "add.hpp"
#include "commit.hpp"
#include "diff.hpp"
#include "status.hpp"
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <defaults.hpp>
using namespace std;

void Add::add() {
    cout << "Please input filename" << endl;
}

void Add::add(string fileName) {
    cout << "constructor with filename " << fileName << endl;
    
    bool isDelete = false;
    bool isBinary = checkIfBinary(fileName);
    bool isStageable = status::checkIfFileInLocal(fileName);
    
    // TODO: Add git status code here , get values for isDelete and isBinary
    // TODO: Implement Daryl's helper function for the isStageable

    if (isStageable) {
        this->staged.addToStaged(fileName, isDelete, isBinary);
        // this->staged.printStaged();
    }

    else {
        cout << "Nothing to be staged";
    }
}
bool Add::checkIfBinary(string fileName) {
    int c;
    std::ifstream ifs(fileName);
    if (ifs.fail()) {
        return 0;
    }
    while ((c = ifs.get()) != EOF && c <= 127)
        ;
    if (c == EOF) {
        return true;
    } else {
        return false;
    }
}

Staged::Staged() {
    this->stagedFile = "staged";
}
void Staged::addToStaged(string fileName, bool isDelete, bool isBinary) {
    ofstream fout;
    this->paths.push_back(fileName);
    fout.open(this->stagedFile, ios::app);
    fout << fileName << "," << isDelete << "," << isBinary << endl;
    fout.close();

    this->printStaged();
}

void Staged::printStaged() {
    ifstream fin;
    string line;
    fin.open(this->stagedFile);
    while (fin) {
        getline(fin, line);
        if (line.compare("") != 0)
            cout << line << " Staged" << endl;
    }

    fin.close();
}

vector<string> Staged::getAllFilesFromDirectory(const char* subPath) {
    struct dirent* de; // Pointer for directory entry
    std::vector<string> cacheFiles;
    // opendir() returns a pointer of DIR type.
    DIR* dr = opendir(subPath);

    if (dr == NULL) // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory");
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    while ((de = readdir(dr)) != NULL)
        if (de->d_type == DT_REG) {

            cacheFiles.push_back(de->d_name);
        }
    closedir(dr);
    return cacheFiles;
}

bool Staged::ifModifiedOrAdded(string filePath) {
    string CachePath;
    CachePath.assign(Defaults::fgitCaches);
    vector<string> FgitPaths = Staged::getAllFilesFromDirectory(Defaults::fgitCaches.c_str());
    bool isExist = false;
    string fgitCachePath = CachePath;
    for (int i = 0; i < FgitPaths.size(); i++) {
        if (FgitPaths[i].compare(filePath) == 0) {
            CachePath = fgitCachePath + FgitPaths[i];
            isExist = true;
            cout << "FILE EXIST";
            break;
        }
    }
    if (isExist) {
        string FgitContent = readFileIntoString(CachePath);
        string currentContent = readFileIntoString(filePath);
        if (Diff::isDiff(currentContent, FgitContent)) {
            return true;
        } else {
            return false;
        }
        return Diff::isDiff(FgitContent, currentContent);
    } 
    else {
        return true;
    }
}

string Staged::readFileIntoString(const string& path) {
    auto ss = ostringstream{};
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '" << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}
