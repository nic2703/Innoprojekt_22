#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;
ifstream fin;
ofstream fout;

int main(void){
    string filename = "Phi.svg";
    fin.open(filename);
    if (!fin.is_open()){
        cout << "Could not open the file " << filename << endl;
        cout << "Program terminating...\n";
        fin.close();
        exit(EXIT_FAILURE);
    }

    if (fileread()){
        cout << "FILE READ SUCCESSFUL";
    } else {
        cout << "AN ERROR OCCURED";
    }
    fin.close();
}