#include "diff.hpp"
#include "init.hpp"

#include "blob.hpp"

#include <iostream>

using namespace std;

void runSubCmd(string subCmd) {
    if (subCmd == "init")
        Init::init();
}

int main(int argc, char** argv) {
    if (argc >= 2) {
        string subcmd(argv[1]);

        runSubCmd(subcmd);
    }

    Blob blob("./CMakeLists.txt");
    createBlob(blob);
    cout << blob;

    return 0;
}
