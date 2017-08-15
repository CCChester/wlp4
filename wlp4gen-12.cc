#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <vector>
using namespace std;


struct tree {
    string rule;
    vector <string> tokens;
    vector <tree*> children;
    ~tree() {
        for(vector<tree*>::iterator i = children.begin(); i != children.end(); i++) {
            delete (*i);
        }
    }

};


void combine(tree *);


void getfrontdata(map <string, string> &symtable1, int &errorc){
    string temp, token, name, type = "";
    int a = 0, b = 0 , c = 0;
    while (getline(cin, temp)){
        if (temp == "procedures main") return;
        istringstream iss(temp);
        iss >> token;
        if (token == "procedure"){
            a = 0;
            b = 0;
            iss >> token;
            if (token == "INT"){
                iss >> token;
                while(getline(cin, temp)){
                    istringstream iss(temp);
                    iss >> token;
                    if (token == "ID"){
                        iss >> token;
                        name = token;
                        a = 1;
                        break;
                    }
                }
            }
        }
        if (token == "LPAREN") {
            c = 1;
            while(getline(cin, temp)){
                istringstream iss(temp);
                iss >> token;
                if (token == "type"){
                    iss >> token;
                    if (iss >> token){
                        type += "int* ";
                        b = 1;
                    }else{
                        type += "int ";
                        b = 1;
                    }
                }
                if (token == "RPAREN"){
                    break;
                }
            }
        }
        if ((a == 1 && b == 1) || (a == 1 && b == 0 && c == 1)){
            if (symtable1.count(name) == 0){
                symtable1.insert(pair <string, string> (name, type));
                a = 0;
                b = 0;
            }
            else{
                cerr << "ERROR: duplicate func defined " << endl;
                a = 0;
                b = 0;
                errorc = 1;
                return;
            }
        }
    }
}




void savedata(tree *root, int &numchildren){
    string input;
    getline(cin, input);
    root->rule = input;
    istringstream iss(input);
    string token;
    iss >> token;                                       // the first token of every line
    if (token == "") return;
    if (token == "BOF" || token == "BECOMES" || token == "COMMA" || token == "ELSE" ||
        token == "EOF" || token == "EQ" || token == "GE"  || token == "ID" || token == "IF" ||
        token == "INT" || token == "LBRACE" || token == "LE" || token == "LPAREN" || token == "WAIN" ||
        token == "LT" || token == "MINUS" || token == "NE" || token == "PCT" || token == "NUM" ||
        token == "PCT" || token == "PLUS" || token == "PRINTLN" || token == "RBRACE" || token == "RETURN" ||
        token == "RPAREN" || token == "SEMI" || token == "SLASH" || token == "STAR" || token == "WHILE" ||
        token == "AMP" || token == "LBRACK" || token == "RBRACK" || token == "NEW" || token == "DELETE" ||
        token == "NULL"){
        root->tokens.push_back(token);
        iss >> token;
        root->tokens.push_back(token);
    }else{
        while(iss >> token){
            numchildren++;
            root->tokens.push_back(token);
        }
    }
}

void addchildren(tree *root, int numchildren){
    if (numchildren == 0){
        root->children.push_back(NULL);
    }else{
        for(int i = 0; i < numchildren; i++){
            tree *child = new tree();
            root->children.push_back(child);
        }
        for (vector<tree*>::iterator i = root->children.begin(); i != root->children.end(); i++){
            combine(*i);
        }
    }
}


void combine(tree *root){
    int t = 0;
    savedata(root, t);
    addchildren(root, t);
}


void createsymtable(tree *root, map <string, string> &symtable){
    if (root->children[0] == NULL){
        string temp;
        istringstream iss(root->rule);
        iss >> temp;
        if (temp == "WAIN"){
            iss >> temp;
            if (symtable.count(temp) == 0){
                symtable.insert(pair <string, string> (temp, ""));
            }else{
                cerr << "ERROR: duplicate wain" << endl;
                return;
            }
        }
    } else {
        if (root->rule == "dcl type ID"){
            istringstream iss (root->children[0]->rule);
            string temp, temp1;
            iss >> temp >> temp;
            if (iss >> temp1){
                temp = "int*";
            }else{
                temp = "int";
            }
            string ID;
            istringstream iss1 (root->children[1]->rule);
            iss1 >> ID >> ID;
            if (symtable.count(ID) == 0){
                symtable.insert(pair <string, string> (ID, temp));
            }else{
                cerr << "ERROR: duplicate variables" << endl;
                return;
            }
        }else{
            for(int i = 0; i < root->children.size() ; i++){
                createsymtable(root->children[i], symtable);
            }
        }
    }
}



void checkerror (tree *root, map <string, string> &symtable, int &errorcheck){
    if (root->children[0] == NULL){
            return;
    }else{
        if (root->rule == "factor ID"){
            istringstream iss(root->children[0]->rule);
            string type, name;
            iss >> type >> name;
            if (symtable.count(name) == 0){
                cerr << "ERROR: undefined variables" << endl;
                errorcheck = 1;
                return;
            }
        }else{
            for (int i = 0; i < root->children.size(); i++){
                checkerror(root->children[i], symtable, errorcheck);
            }
        }
    }
}

void print(map <string, string> &symtable, map <string, string> &symtable1, string addstr){
    for(map<string, string>::iterator i = symtable1.begin(); i != symtable1.end(); i++){
        cerr << i->first << " " << i->second << endl;
    }
    cerr << endl;
    
    map<string, string>::iterator i = symtable.find("wain");
    symtable.erase(i);
    symtable.insert(pair <string, string> ("wain", addstr));
    cerr << symtable.find("wain")->first << " " << symtable.find("wain")->second << endl;
    map<string, string>::iterator q = symtable.find("wain");
    symtable.erase(q);

    
    for(map<string, string>::iterator i = symtable.begin(); i != symtable.end(); i++){
        cerr << i->first << " " << i->second << endl;
    }

}

void addwainp(tree *root, string &addstring){
    for (int i = 0; i < root->children.size(); i++){
        if (root->tokens[i] == "dcl"){
            string temp, type, name;
            istringstream iss(root->children[i]->children[0]->rule);
            iss >> temp >> type;
            if (iss >> type){
                addstring += "int* ";
            }else{
                addstring += "int ";
            }
        }
        else if (root->tokens[i] == "RPAREN"){
        break;
        }
    }
}



int main(int argc, const char * argv[]) {
    map <string, string> symtable;
    map <string, string> symtable1;
    string addstr = "";
    int errorc = 0;
    tree *root = new tree();
    getfrontdata(symtable1, errorc);
    if (errorc == 1) return 0;
    combine(root);
    createsymtable(root, symtable);
    addwainp(root, addstr);
    int errorcheck = 0;
    checkerror(root, symtable, errorcheck);
    if (errorcheck == 0){
        print(symtable, symtable1, addstr);
    }
    delete root;
    return 0;
}
