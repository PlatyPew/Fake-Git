#pragma once

#include "cereal/access.hpp"

#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = filesystem;

class Blob {
    friend class cereal::access;

  private:
    string createDiff(bool genesis);

    friend void toSerial(stringstream& serial, Blob blob);
    friend void fromSerial(stringstream& serial, Blob& blob);

    template <class Archive> void serialize(Archive& archive) {
        archive(path, perms, diff, deletion);
    }

  protected:
    string path;
    fs::perms perms;
    string diff;
    bool deletion;

  public:
    Blob(){};
    Blob(string path, bool deletion = false);

    string getPath();
    fs::perms getPerms();
    string getDiff();
    bool getDeletion();
    string getContents(string prevContents);

    friend ostream& operator<<(ostream& out, const Blob& blob);
    friend istream& operator>>(istream& in, Blob& blob);
};
