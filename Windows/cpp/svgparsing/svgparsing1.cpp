#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
const int SIZE = 60;
using namespace std;

int main(void){
    string filename = "Phi.txt";
    ifstream fin;
    ofstream fout;
    fout.open("output.txt");
    fin.open(filename);
    if (!fin.is_open()){
        cout << "Could not open the file " << filename << endl;
        cout << "Program terminating...\n";
        exit(EXIT_FAILURE);
    }

    string tempstr;
    int wordnum = 0;
    while (fin >> tempstr){
        if (fin.eof()){
            cout << "END OF FILE" << endl;
            break;
        } else if (fin.fail()){
            cout << "INPUT TERMINATED DATA MISMATCH" << endl;
            break;
        } else {
            wordnum++;
            if (tempstr == "<path"){
                cout << "Success at LINE " << wordnum << endl;
                //SVGDECODE
                char ch;
                tempstr = '\0';
                while (fin >> ch && ch != '\"'){}
                bool firstnum = true;
                while (fin >> ch && ch != '\"'){
                    switch (ch){
                        case 'C': cout << tempstr << endl; tempstr = "\0"; cout << "C" << endl; firstnum = true; break;
                        case 'c': cout << tempstr << endl; tempstr = "\0"; cout << "c" << endl; firstnum = true; break;
                        case 'V': cout << tempstr << endl; tempstr = "\0"; cout << "V" << endl; firstnum = true; break;
                        case 'v': cout << tempstr << endl; tempstr = "\0"; cout << "v" << endl; firstnum = true; break;
                        case 'H': cout << tempstr << endl; tempstr = "\0"; cout << "H" << endl; firstnum = true; break;
                        case 'h': cout << tempstr << endl; tempstr = "\0"; cout << "h" << endl; firstnum = true; break;
                        case 'L': cout << tempstr << endl; tempstr = "\0"; cout << "L" << endl; firstnum = true; break;
                        case 'l': cout << tempstr << endl; tempstr = "\0"; cout << "l" << endl; firstnum = true; break;
                        case 'Z': cout << tempstr << endl; tempstr = "\0"; cout << "Z" << endl << endl; firstnum = true; break;
                        case 'z': cout << tempstr << endl; tempstr = "\0"; cout << "z" << endl << endl; firstnum = true; break;

                        case ',':{
                            if (firstnum == true){
                                firstnum = false;
                                tempstr.push_back(ch);
                            } else {
                                cout << tempstr << endl;
                                tempstr = "\0";
                                firstnum = true;
                            }
                            break;
                        }
                        case '-':{
                            if (firstnum == true && tempstr == "\0"){
                                tempstr.push_back(ch);
                            } else if (firstnum == true && tempstr != "\0"){
                                firstnum = false;
                                tempstr.push_back(',');
                                tempstr.push_back(ch);
                            } else if (firstnum == false && tempstr != "\0"){
                                firstnum = true;
                                cout << tempstr << endl;
                                tempstr = "\0";
                                tempstr.push_back(ch);
                            } else {
                                cout << "UNKNOWN" << endl;
                            }
                            break;
                        }
                        default: tempstr.push_back(ch);
                    }
                }











            }
        }
    }
    cout << "DONE" << endl;
    return 0;
}
