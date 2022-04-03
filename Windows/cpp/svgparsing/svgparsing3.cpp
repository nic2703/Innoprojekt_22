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

//IMPORTANT NOTICE
//WHEN MODIFYING THIS FILE, PLEASE MAKE A SEPARATE COPY FIRST!!

//IMPORTANT NOTICE


bool isletter(char ch){
    if ((ch>='a' && ch<='z')||(ch>='A'&&ch<='Z')){
        return true;
    }
    return false;
}

bool isnumeric(char ch){
    if (ch>='0' && ch<='9'){
        return true;
    }
    return false;
}

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
        if (ch=='-'){
            if (firstnum == true && temp == "\0"){
                temp.push_back(ch);
            } else if (firstnum == true && temp != "\0"){
                pairx[i] = temp;
                temp = "";
                firstnum = false;
                temp.push_back(ch);
            } else if (firstnum == false && temp == "\0"){
                temp.push_back(ch);
            } else if (firstnum == false && temp != "\0"){
                pairy[i] = temp;
                i++;
                temp = "";
                firstnum = true;
                temp.push_back(ch);
            }
        }

        if (isletter(ch)){
            if (pairx[i]!="\0"){                            //finish the last pair, letter was delimiter
                pairy[i] = temp;
                i++;
                temp = "";
                firstnum = true;
            }
            if (i == 4 && (command == 'C')){
                //CUBIC BEZIER
                cout << command << " ";
                for (int i = 0; i < 4; i++){
                    cout << pairx[i] << ", " << pairy[i] << ", ";
                }
                cout << endl;
                pairx[0] = pairx[3];
                pairy[0] = pairy[3];
                for (int i = 1; i <= 3; i++){
                    pairx[i] = pairy[i] = "";
                }
                i = 1;
            }
            if (i == 3 && (command == 'C' || command == 'Q')){
                //QUADRATIC BEZIER
                cout << "Q ";
                for (int i = 0; i < 3; i++){
                    cout << pairx[i] << ", " << pairy[i] << ", ";
                }
                cout << endl;
                pairx[0] = pairx[2];
                pairy[0] = pairy[2];
                for (int i = 1; i <= 3; i++){
                    pairx[i] = pairy[i] = "";
                }
                i = 1;
            }
            if (i == 1 && (command == 'H' || command == 'V')){
                cout << command << " " << temp << endl;
                if (command == 'H'){
                    pairx[0] = to_string(stod(pairx[0])+stod(temp));
                } else {
                    pairy[0] = to_string(stod(pairy[0])+stod(temp));
                }
                temp = "";
                i = 1;
            }
            if (i == 2 && command == 'L'){
                cout << command << " " << pairx[1] << ", " << pairy[1] << endl;
                pairx[0] = to_string(stod(pairx[0])+stod(pairx[1]));
                pairy[0] = to_string(stod(pairy[0])+stod(pairy[1]));
                for (int i = 1; i <= 3; i++){
                    pairx[i] = pairy[i] = "";
                }
                i = 1;
            }

            if (command == 'Z'){
                cout << "End of line reached." << endl;
                for (int i = 0; i <= 3; i++){
                    pairx[i] = pairy[i] = "";
                }
                i = 0;
            }
            if (command == 'M'){
                cout << "Start of line reached." << endl;
            }


            //command = ch;         no, as must be normalized to uppercase in if statements below

            //POST
            if (ch == 'C' || ch == 'c' || ch == 'S' || ch == 's'){      //cubic bezier, occasionally used for quadratic
                command = 'C';
            }
            if (ch == 'Q' || ch == 'q' || ch == 'T' || ch == 't'){      //quadratic bezier
                command = 'Q';
            }
            if (ch == 'H' || ch == 'h'){                                //horizontal line, pairx[0] shifted
                command = 'H';
            }
            if (ch == 'V' || ch == 'v'){                                //vertical line, pairy[0] shifted
                command = 'V';
            }
            if (ch == 'Z' || ch == 'z'){                                //lift pen, pairx and pairy (all) reset
                command = 'Z';
            }
            if (ch == 'M' || ch == 'm'){                                //put down pen
                command = 'M';
            }
            if (ch == 'L' || ch == 'l'){                                //straight line, pairx[0] and pairy[0] shifted
                command = 'L';
            }

        }

    }

    return true;
}

int main(void){
    string filename = "Phi.txt";
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