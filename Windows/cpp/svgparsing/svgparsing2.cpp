#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;
ifstream fin;
ofstream fout;

bool searchforpath(){
    string tempstr;
    int wordnum = 0;
    while (fin >> tempstr){
        if (fin.eof()){
            cout << "END OF FILE" << endl;
            return false;
        } else if (fin.fail()){
            cout << "INPUT TERMINATED DATA MISMATCH" << endl;
            return false;
        } else {
            wordnum++;
            cout << tempstr << endl;
        }
    }
    return true;
}

int main(void){
    string filename = "Phi.svg";
    fin.open(filename);
    if (!fin.is_open()){
        cout << "Could not open the file " << filename << endl;
        cout << "Program terminating...\n";
        fin.close();
        exit(EXIT_FAILURE);
    }

    if (searchforpath()){
        cout << "FILE READ SUCCESSFUL";
    } else {
        cout << "AN ERROR OCCURED";
    }
    fin.close();
}