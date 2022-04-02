#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;
ifstream fin;
ofstream fout;

bool searchforpath();
bool ganalysis();
bool pathanalysis();

bool searchforpath(){
    string tempstr;
    int counter = 0;
    while (fin>>tempstr){
        counter++;
        if (tempstr == "<g" || tempstr == "<g>"){
            cout << "G FOUND AT STR " << counter << endl;
            ganalysis();
        }
    }
    return true;
}

bool ganalysis(){
    string tempstr;
    int counter = 0;
    while (fin>>tempstr){
        counter++;
        if (tempstr == "<path"){
            cout << "PATH FOUND AT STR " << counter << endl;
            pathanalysis();
        }
    }
    return true;
}

bool pathanalysis(){
    char ch;
    while (fin>>ch && ch!='\"'){};              //skips until the actual line starts
    string pairx[4];
    string pairy[4];
    string temp;
    char command;
    int i = 0;                                  //current index for the x-y list
    bool firstnum = true;
    while (fin>>ch && ch!='\"'){
        if ((ch>='0' && ch<='9') || ch=='.'){
            temp.push_back(ch);
        }
        if (ch==',' || ch==' '){
            if (firstnum == true){
                pairx[i] = temp;
                temp = "";
                firstnum = false;
            } else {
                pairy[i] = temp;
                i++;
                temp = "";
                firstnum = true;
            }
        }
        if (ch == 'C' || ch == 'c' || ch == 'S' || ch == 's'){
            if (pairx[i]!="\0"){
                pairy[i] = temp;
                i++;
                temp = "";
                firstnum = true;
            }
            if (i == 4){
                //CUBIC BEZIER
                cout << command << " ";
                for (int i = 0; i < 4; i++){
                    cout << pairx[i] << " " << pairy[i] << " " << endl;
                }
            }
            command = ch;
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