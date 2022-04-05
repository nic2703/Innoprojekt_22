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
            char ch1, ch2;
            while (fin>>std::noskipws>>ch1){
                if (ch1 == '=' && ch2 == 'd'){
                    cout << "D FOUND" << endl;
                    pathanalysis();
                }
                ch2 = ch1;
            }
        }
    }
    return true;
}

bool pathanalysis(){
    char ch;
    while (fin>>ch && ch!='\"'){};              //skips until the actual line starts
    string pairx[5];
    string pairy[5];
    string temp;
    string pairxspecial;
    string pairyspecial;
    char command;
    int i = 0;                                  //current index for the x-y list
    bool firstnum = true;
    while (fin>>std::noskipws>>ch && ch!='\"'){
        if ((ch>='0' && ch<='9') || ch=='.'){
            temp.push_back(ch);
        }
        if (ch==',' || ch==' '){
            if (firstnum == true && command!='Z'){
                pairx[i] = temp;
                temp = "";
                firstnum = false;
            } else if (firstnum == false && command!='Z'){
                pairy[i] = temp;
                i++;
                temp = "";
                firstnum = true;
            } else {
                //ignore
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
            if (pairx[i]=="\0" && i == 4){
                pairx[i] = temp;
                temp = "";
                firstnum = true;
            }
            if (i == 4 && (command == 'C')){
                //CUBIC BEZIER
                cout << "C ";
                for (int i = 0; i < 4; i++){
                    cout << pairx[i] << ", " << pairy[i] << ", ";
                }
                cout << endl;
                pairx[0] = pairx[3];
                pairy[0] = pairy[3];
                pairxspecial = pairx[2];
                pairyspecial = pairy[2];
                for (int i = 1; i <= 3; i++){
                    pairx[i] = pairy[i] = "";
                }
                i = 1;
            }
            if (i == 4 && (command == 'c')){
                //CUBIC BEZIER points shifted
                cout << "C ";
                pairx[1] = to_string(stod(pairx[0])+stod(pairx[1]));
                pairy[1] = to_string(stod(pairy[0])+stod(pairy[1]));
                pairx[2] = to_string(stod(pairx[0])+stod(pairx[2]));
                pairy[2] = to_string(stod(pairy[0])+stod(pairy[2]));
                pairx[3] = to_string(stod(pairx[0])+stod(pairx[3]));
                pairy[3] = to_string(stod(pairy[0])+stod(pairy[3]));
                for (int i = 0; i < 4; i++){
                    cout << pairx[i] << ", " << pairy[i] << ", ";
                }
                cout << endl;
                pairx[0] = pairx[3];
                pairy[0] = pairy[3];
                pairxspecial = pairx[2];
                pairyspecial = pairy[2];
                for (int i = 1; i <= 3; i++){
                    pairx[i] = pairy[i] = "";
                }
                i = 1;
            }
            if (i == 3 && (command == 'S')){
                //QUADRATIC SMOOTH BEZIER
                cout << "C ";
                cout << pairx[0] << ", " << pairy[0] << ", ";
                cout << pairxspecial << ", " << pairyspecial << ", ";
                cout << pairx[1] << ", " << pairy[1] << ", ";
                cout << pairx[2] << ", " << pairy[2] << ", ";
                cout << endl;
                pairx[0] = pairx[2];
                pairy[0] = pairy[2];
                pairxspecial = pairx[1];
                pairyspecial = pairy[1];
                for (int i = 1; i <= 3; i++){
                    pairx[i] = pairy[i] = "";
                }
                i = 1;
            }
            if (i == 3 && (command == 's')){
                //QUADRATIC SMOOTH BEZIER POSITIONS SHIFTED
                cout << "C ";
                pairx[1] = to_string(stod(pairx[0])+stod(pairx[1]));
                pairy[1] = to_string(stod(pairy[0])+stod(pairy[1]));
                pairx[2] = to_string(stod(pairx[0])+stod(pairx[2]));
                pairy[2] = to_string(stod(pairy[0])+stod(pairy[2]));
                cout << pairx[0] << ", " << pairy[0] << ", ";
                cout << pairxspecial << ", " << pairyspecial << ", ";
                cout << pairx[1] << ", " << pairy[1] << ", ";
                cout << pairx[2] << ", " << pairy[2] << ", ";
                cout << endl;
                pairx[0] = pairx[2];
                pairy[0] = pairy[2];
                pairxspecial = pairx[1];
                pairyspecial = pairy[1];
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
                pairxspecial = pairx[1];
                pairyspecial = pairy[1];
                for (int i = 1; i <= 3; i++){
                    pairx[i] = pairy[i] = "";
                }
                i = 1;
            }
            if (i == 3 && (command == 'c' || command == 'q')){
                //QUADRATIC BEZIER points shifted
                cout << "Q ";
                pairx[1] = to_string(stod(pairx[0])+stod(pairx[1]));
                pairy[1] = to_string(stod(pairy[0])+stod(pairy[1]));
                pairx[2] = to_string(stod(pairx[0])+stod(pairx[2]));
                pairy[2] = to_string(stod(pairy[0])+stod(pairy[2]));
                for (int i = 0; i < 3; i++){
                    cout << pairx[i] << ", " << pairy[i] << ", ";
                }
                cout << endl;
                pairx[0] = pairx[2];
                pairy[0] = pairy[2];
                pairxspecial = pairx[1];
                pairyspecial = pairy[1];
                for (int i = 1; i <= 3; i++){
                    pairx[i] = pairy[i] = "";
                }
                i = 1;
            }
            if (i == 2 && (command == 'S' || command == 'T')){
                //QUADRATIC SMOOTH BEZIER
                cout << "Q ";
                cout << pairx[0] << ", " << pairy[0] << ", ";
                cout << pairxspecial << ", " << pairyspecial << ", ";
                cout << pairx[1] << ", " << pairy[1] << ", ";
                for (int i = 0; i < 3; i++){
                    cout << pairx[i] << ", " << pairy[i] << ", ";
                }
                cout << endl;
                pairx[0] = pairx[2];
                pairy[0] = pairy[2];
                pairxspecial = pairx[1];
                pairyspecial = pairy[1];
                for (int i = 1; i <= 3; i++){
                    pairx[i] = pairy[i] = "";
                }
                i = 1;
            }
            if (i == 2 && (command == 's' || command == 't')){
                //QUADRATIC SMOOTH BEZIER
                cout << "Q ";
                pairx[1] = to_string(stod(pairx[0])+stod(pairx[1]));
                pairy[1] = to_string(stod(pairy[0])+stod(pairy[1]));
                cout << pairx[0] << ", " << pairy[0] << ", ";
                cout << pairxspecial << ", " << pairyspecial << ", ";
                cout << pairx[1] << ", " << pairy[1] << ", ";
                for (int i = 0; i < 3; i++){
                    cout << pairx[i] << ", " << pairy[i] << ", ";
                }
                cout << endl;
                pairx[0] = pairx[2];
                pairy[0] = pairy[2];
                pairxspecial = pairx[1];
                pairyspecial = pairy[1];
                for (int i = 1; i <= 3; i++){
                    pairx[i] = pairy[i] = "";
                }
                i = 1;
            }
            
            if (i == 1 && (command == 'H' || command == 'V')){
                //H and V commands
                cout << command << " " << temp << endl;
                if (command == 'H'){
                    pairx[0] = temp;
                    cout << command << " " << pairx[0] << ", " << pairy[0] << ", " << endl;
                } else {
                    pairy[0] = temp;
                    cout << command << " " << pairx[0] << ", " << pairy[0] << ", " << endl;
                }
                pairxspecial = pairx[0];
                pairyspecial = pairy[0];
                temp = "";
                i = 1;
            }
            if (i == 1 && (command == 'h' || command == 'v')){
                //h and v commands shifted
                if (command == 'h'){
                    pairx[0] = to_string(stod(pairx[0])+stod(temp));
                    cout << command << " " << pairx[0] << ", " << pairy[0] << ", " << endl;
                } else {
                    pairy[0] = to_string(stod(pairy[0])+stod(temp));
                    cout << command << " " << pairx[0] << ", " << pairy[0] << ", " << endl;
                }
                pairxspecial = pairx[0];
                pairyspecial = pairy[0];
                temp = "";
                i = 1;
            }
            if (i == 2 && command == 'L'){
                //L command line
                pairx[0] = pairx[1];
                pairy[0] = pairy[1];
                cout << command << " " << pairx[0] << ", " << pairy[0] << endl;
                for (int i = 1; i <= 3; i++){
                    pairx[i] = pairy[i] = "";
                }
                pairxspecial = pairx[0];
                pairyspecial = pairy[0];
                i = 1;
            }
            if (i == 2 && command == 'l'){
                //L command line shifted
                pairx[0] = to_string(stod(pairx[0])+stod(pairx[1]));
                pairy[0] = to_string(stod(pairy[0])+stod(pairy[1]));
                cout << command << " " << pairx[0] << ", " << pairy[0] << endl;
                for (int i = 1; i <= 3; i++){
                    pairx[i] = pairy[i] = "";
                }
                pairxspecial = pairx[0];
                pairyspecial = pairy[0];
                i = 1;
            }

            if (i == 4 && command == 'A'){
                //A ellipse command using dx dy
                cout << command << " " << pairx[0] << ", " << pairy[0] << ", " << to_string(stod(pairx[0])+stod(pairy[3])) << ", " << to_string(stod(pairy[0])+stod(pairx[4])) << ", " << pairx[2] << ", " << pairy[2] << ", " << pairx[3] << ", " << endl;
                pairx[0] = to_string(stod(pairx[0])+stod(pairy[3]));
                pairy[0] = to_string(stod(pairy[0])+stod(pairx[4]));
                for (int i = 1; i<=4; i++){
                    pairx[i] = pairy[i] = "";
                }
                pairxspecial = pairx[0];
                pairyspecial = pairy[0];
                i = 1;
                firstnum = true;
            }

            if (command == 'Z'){
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
            if (ch == 'C'){      //cubic bezier, occasionally used for quadratic
                command = 'C';
            }
            if (ch == 'S'){     //cubic smooth bezier
                command = 'S';
            }
            if (ch == 'c'){     //cubic bezier, occasionally quadratic, relative positions
                command = 'c';
            }
            if (ch == 's'){     //cubic smooth bezier, occasionally quadratic, relative positions
                command = 's';
            }
            if (ch == 'Q' || ch == 'q' || ch == 'T' || ch == 't'){      //quadratic bezier
                command = 'Q';
            }
            if (ch == 'H'){                                             //horizontal line, to x= temp
                command = 'H';
            }
            if (ch == 'h'){                                             //horizontal line, to pairx[0]+temp
                command = 'h';
            }
            if (ch == 'V'){                                             //vertical line, to y=temp
                command = 'V';
            }
            if (ch == 'v'){                                             //vertical line, to pairy[0]+temp
                command = 'v';
            }
            if (ch == 'Z' || ch == 'z'){                                //lift pen, pairx and pairy (all) reset
                command = 'Z';
                cout << "End of line reached." << endl;
            }
            if (ch == 'M' || ch == 'm'){                                //put down pen
                command = 'M';
            }
            if (ch == 'L'){                                             //straight line, to pairx[1] and pairy[1]
                command = 'L';
            }
            if (ch == 'l'){                                             //straight line, pairx[0] and pairy[0] shifted by pairx[1] and pairy[1]
                command = 'l';
            }
            if (ch == 'A' || ch == 'a'){
                command = 'A';
            }
            if (ch == 'D' || ch == 'd'){                                //just for debugging
                for (int j = 0; j <= 4; j++){
                    cout << "     pairx[" << j << "]=" << pairx[j] << " | pairy[" << j << "]=" << pairy[j] << endl;
                }
            }

        }

    }

    return true;
}

int main(void){
    string filename = "omega-svgrepo-com.svg";
    fin.open(filename);
    if (!fin.is_open()){
        cout << "Could not open the file " << filename << endl;
        cout << "Program terminating...\n";
        fin.close();
        exit(EXIT_FAILURE);
    }

    if (ganalysis()){
        cout << "FILE READ SUCCESSFUL";
    } else {
        cout << "AN ERROR OCCURED";
    }
    fin.close();
}