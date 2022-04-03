#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;
ifstream fin;
ofstream fout;

bool searchdequal();
bool svgparse();

bool isletter(char ch){
    if ((ch>='a' && ch<='z')||(ch>='A' && ch<='Z')){
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

int main(void){
    string filename = "Phi.txt";
    fin.open(filename);
    if (!fin.is_open()){
        cout << endl <<"ERROR 01: Could not open the file " << filename << endl;
        cout << "Program terminating..." << endl;
        fin.close();
        exit(EXIT_FAILURE);
    }
    if (searchdequal()){
        cout << "FILE READ SUCCESSFUL";
    } else {
        cout << endl << "ERROR 02: SearchDEqual returned false." << endl;
    }
    fin.close();
    return 0;
}

bool searchdequal(){
    char ch1, ch2;
    int dfound = 0;
    while (fin>>std::noskipws>>ch1){
        if (ch1 == '=' && ch2 == 'd'){
            dfound++;
            cout << "DEBUG 01: D FOUND" << endl;
            svgparse();
        }
        ch2 = ch1;
    }
    cout << "DEBUG 02: #D FOUND: " << dfound << endl;
    if (dfound>0){
        return true;
    }
    return false;
}

bool svgparse(){
    char ch;
    while (fin>>ch && ch!='\"'){};
    string num[11];
    string temp;
    char command;
    int index = 0;
    while (fin>>std::noskipws>>ch && ch!='\"'){
        if (isnumeric(ch)||ch=='.'){
            temp.push_back(ch);
        }
        if (ch==','||ch==' '){
            if (command!='Z'){
                num[index] = temp;
                temp = "";
                index++;
            } else {
                //ignore
            }
        }
        if (ch=='-'){
            if (temp == "\0"){                  //if temp is empty
                temp.push_back(ch);
            } else {
                num[index] = temp;
                temp = "";
                temp.push_back(ch);
                index++;
            }
        }
        if (isletter(ch)){
            cout << "ch=" << ch << " index=" << index << " command=" << command << endl;
            if (temp!="\0"){
                num[index] = temp;
                temp = "";
                index++;
            }
            if (command == 'C'){
                if (index==6 || index==8){
                    if (index == 6){
                        cout << "Q ";
                    } else {
                        cout << "C ";
                    }
                    for (int j = 0; j<index; j++){
                        cout << num[j] << ", ";
                    }
                    cout << endl;
                } else {
                    cout << "ERROR 03: C-Bezier has incorrect number of inputs: " << index << " instead of 6 (Q) or 8(C)" << endl;
                    return false;
                }
                for (int j = 4; j<11; j++){
                    num[j] = "";
                }
                index = 4;
            }

            if (command == 'c'){
                if (index==6 || index==8){
                    if (index == 6){
                        cout << "Q ";
                    } else {
                        cout << "C ";
                        num[6] = to_string(stod(num[6])+stod(num[0]));
                        num[7] = to_string(stod(num[7])+stod(num[1]));
                    }
                    num[2] = to_string(stod(num[2])+stod(num[0]));
                    num[3] = to_string(stod(num[3])+stod(num[1]));
                    num[4] = to_string(stod(num[4])+stod(num[0]));
                    num[5] = to_string(stod(num[5])+stod(num[1]));
                    for (int j = 0; j<index; j++){
                        cout << num[j] << ", ";
                    }
                    cout << endl;
                } else {
                    cout << "ERROR 03: C-Bezier has incorrect number of inputs: " << index << " instead of 6 (Q) or 8(C)" << endl;
                    return false;
                }
                for (int j = 4; j<11; j++){
                    num[j] = "";
                }
                index = 4;
            }

            if (command == 'Q'){
                if (index == 6){
                    cout << "Q ";
                    for (int j = 0; j < index; j++){
                        cout << num[j] << ", ";
                    }
                    cout << endl;
                } else {
                    cout << "ERROR 04: Q-Bezier has incorrect number of inputs: " << index << " instead of 6 (Q)" << endl;
                }
                for (int j = 4; j<11; j++){
                    num[j] == "";
                }
                index = 4;
            }



            if (ch == 'D'){
                for (int j = 0; j<11; j++){
                        cout << "     DEBUG num[" << j << "]=" << num[j] << endl;
                }
            }
            if (ch == 'C'){
                if (index == 4){
                    num[0] = num[2];
                    num[1] = num[3];
                    num[2] = num[3] = "";
                    index = 2;
                }
                command = ch;
            }
            if (ch == 'c'){
                if (index == 4){
                    num[0] = num[2];
                    num[1] = num[3];
                    num[2] = num[3] = "";
                    index = 2;
                }
                command = ch;
            }
            if (ch == 'Q'){
                if (index == 4){
                    num[0] = num[2];
                    num[1] = num[3];
                    num[2] = num[3] = "";
                    index = 2;
                }
                command = ch;
            }



        }
    }
    return true;
}