#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <utility>
#include <fstream>

using namespace std;

// structure for the parse tree
struct Parsetree {
	string value;
        vector <Parsetree *> children;
};

// vector use to keep track of the input
vector <string> inp;


// Trim the given strin
// const string & -> string
string trim(const string &str) {
  size_t begin = str.find_first_not_of(" \t\n");
  if (begin == string::npos) return "";

  size_t end = str.find_last_not_of(" \t\n");

  return str.substr(begin, end - begin + 1);
}

// Skip the grammar part of the input except the rules.
// Istream -> void
void skipLine(istream &in) {
  string s;
  getline(in, s);
}

// Functioan that reads the rule of the grammar and save them in an array in the corresponding index.
// Istream *string Int -> void
void getrules(istream &in, string *ptr, int nor) {
        // string to store the rules;
	string rules = "";

        // read each line of the rules and save them into the array
        for (int i = 0; i < nor; i++) {
		getline(in, rules);
                ptr[i] = rules;
        }
}

// Function that construct the table
// ISTREAM Int string * -> void
// required: teh allocated array need to be free in future
void getTable(istream &in, int l, string *t) {

        // construct the table
        for (int i = 0; (i < (l*4)); (i = (i + 4))) {
                in >> t[i];
                in >> t[i+1];
                in >> t[i+2];
                in >> t[i+3];
                skipLine(in);
        }
}

// The function convert the string into the integer
// String -> Int
int stringtoint(string s) {
	int a;
	istringstream(s) >> a;
	return a;
}

// Function search for the pair in the table and return the corresponding pair
// string string -> pair<string int>
pair<string, string> searchpair(string st_top, string input, string *table, int size) {
	pair<string, string> p("n","n");
	for (int i = 0; (i < (size*4)); (i = (i + 4))) {
		if (table[i] == st_top) {
			if (table[i+1] == input) {
				p.first = table[i+2];
				p.second = table[i+3];
			}
		}
	}
	return p;
}

//-------------------------------------------------------------------------------------------------------------------
//void p(Parsetree *root) {
//	cout << "value: " << endl;
//	cout << root->value << endl;
//	cout << "number of children: " << endl;
//	cout << root->children.size() << endl;
//	for (int i = 0; i < root->children.size(); i++) {
//		p(root->children[i]);
//	}
//}

//-------------------------------------------------------------------------------------------------------------------

// The function perform the reduce operation
// Int Sting* stack<string>* stacl<string>* Parsetree* vector<Parsetree*>* -> void
// requires: the allocated nodes need to be free in future
void reduce(int idx, string *rule, stack<string> *state, stack<string> *symbol, Parsetree **r, vector<Parsetree *> *v) {
	string s;
	s = rule[idx];
	stringstream stream;
	stream << s;
	string L;
	stream >> L;
	string R;
//---------------------------------------------------------------------------------------------------------------------
//	cout << endl;
//	cout << endl;
//	cout << endl;
//	cout << "Reduce " << idx << endl;
//	cout << "Rule: " << s << endl;
//---------------------------------------------------------------------------------------------------------------------
	Parsetree *rn = new Parsetree;
	*r = rn;
	rn->value = L;
//---------------------------------------------------------------------------------------------------------------------
//	cout << "Constructing the parent node" << endl;
//	cout << "value: " << L << endl;
//	cout << "children empty" << endl;
//	cout << endl;
//---------------------------------------------------------------------------------------------------------------------
	if (!(stream >> R)) {
		symbol->push(L);
		v->push_back(rn);
//---------------------------------------------------------------------------------------------------------------------
//		cout << "push the parent onto the stack" << endl;
//--------------------------------------------------------------------------------------------------------------------
	} else {
//---------------------------------------------------------------------------------------------------------------------
//		cout << "First RHS: " << R << endl;
//---------------------------------------------------------------------------------------------------------------------
//		cout << "popping: " << symbol->top() << endl;
//		cout << "popping: " << state->top() << endl;
		int id = 0;
		int nd2 = 0;
		//cout << v->size() << endl;
		//return;
		for (int i = v->size()-1; i >= 0; i--) {
//---------------------------------------------------------------------------------------------------------------------
//			cout << "check the vector" << endl;
//			cout << "v[" << i << "]" << "is: " << (*v)[i]->value << endl;
//---------------------------------------------------------------------------------------------------------------------
			if ((*v)[i]->value == R) {
//---------------------------------------------------------------------------------------------------------------------
//				cout << "Push the " << i+1 << "th element of v onto the " << id+1 << "th children" << endl;
//				cout << "delete the v[" << i+1 << "]" << "result in the following v: " << endl;
//				for (int z = 0; z < v->size(); z++) {
//					cout << "|" << (*v)[z]->value << "|";
//				}
//				cout << endl;
//				cout << "to" << endl; 
//---------------------------------------------------------------------------------------------------------------------
				rn->children.push_back((*v)[i]);
				id++;
				v->erase(v->begin() + i);
//---------------------------------------------------------------------------------------------------------------------
//				for (int x = 0; x < v->size(); x++) {
  //                                      cout << "|" << (*v)[x]->value << "|";
    //                            }
      //                		cout << endl;
	//			cout << "Also, the parent have " << rn->children.size() << "children" << endl;
//---------------------------------------------------------------------------------------------------------------------
				nd2++;
				break;	
			}
		}
		if (nd2 == 0) {
//			cout << "new node" << endl;
			Parsetree *n2 = new Parsetree;
			n2->value = R;
			nd2 = 0;
			rn->children.push_back(n2);
			id++;
//----------------------------------------------------------------------------------------------------------------------
//                        cout << "make a new children if not in the v" << endl;
  //                      cout << "Value: " << R << endl;
    //                    cout << "Children empty" << endl;
      //                  cout << "The parent have " << rn->children.size() << " children" << endl;
//---------------------------------------------------------------------------------------------------------------------

		}
		symbol->pop();
		state->pop();
		while (stream >> R) {
//-------------------------------------------------------------------------------------------------------------------------
//			cout << endl;
//			cout << id+1 << "th RHS" << endl;
//------------------------------------------------------------------------------------------------------------------------		
			nd2 = 0;
			for (int j = v->size()-1; j >= 0; j--) {
//---------------------------------------------------------------------------------------------------------------------
//                        	cout << "check the vector" << endl;
  //                      	cout << "v[" << j << "]" << "is: " << (*v)[j]->value << endl;
//---------------------------------------------------------------------------------------------------------------------
                 	       if ((*v)[j]->value == R) {
//---------------------------------------------------------------------------------------------------------------------
//	                                cout << "Push the " << j << "th element of v onto the " << id+1 << "th children" << endl;
//					cout << "delete the v[" << j << "]" << "result in the following v: " << endl;
  //             		                for (int f = 0; f < v->size(); f++) {
    //     					cout << "|" << (*v)[f]->value << "|";
      //                 		        }
        //                      		cout << endl;
          //                     		cout << "to" << endl;
//---------------------------------------------------------------------------------------------------------------------
                        	        rn->children.push_back((*v)[j]);
                               		v->erase(v->begin() + j);
//---------------------------------------------------------------------------------------------------------------------
//                        	        for (int g = 0; g < v->size(); g++) {
  //              	                        cout << "|" << (*v)[g]->value << "|";
    //    	                        }
      //  	                        cout << endl;
	//                                cout << "Also, the parent have " << rn->children.size() << "children" << endl;
//---------------------------------------------------------------------------------------------------------------------
                                	id++;
					nd2++;
                               		break;
                        	}
                	}
			if (nd2 == 0) {
//				cout << "new node" << endl;
				Parsetree *n3 = new Parsetree;
				n3->value = R;
				nd2 = 0;
				rn->children.push_back(n3);
				id++;
//----------------------------------------------------------------------------------------------------------------------
//                                cout << "make a new children if not in the v" << endl;
  //                              cout << "Value: " << R << endl;
    //                            cout << "Children empty" << endl;
      //                          cout << "The parent have " << rn->children.size() << " children" << endl;
//---------------------------------------------------------------------------------------------------------------------
			}	
//			cout << "popping: " << symbol->top() << endl;
//			cout << "popping: " << state->top() << endl;
			symbol->pop();
			state->pop();
		}
//-----------------------------------------------------------------------------------------------------------------------
//		cout << "push the parent node onto v" << endl;
//		for (int y = 0; y < v->size(); y++) {
//			cout << "|" << (*v)[y]->value << "|";
  //              }
    //            cout << endl;
      //          cout << "to" << endl;
//-----------------------------------------------------------------------------------------------------------------------
		v->push_back(rn);
//-----------------------------------------------------------------------------------------------------------------------
//		for (int d = 0; d < v->size(); d++) {
  //                      cout << "|" << (*v)[d]->value << "|";
    //            }
      //          cout << endl;
//-----------------------------------------------------------------------------------------------------------------------
		symbol->push(L);
//		p(*r);
	}
//---------------------------
//	cout << endl;
//	cout << endl;
//	cout << endl;
//----------------------------
}

// The function gives the next state after reading a resuce
// String String String* Int -> string
string updatestate(string state, string symbol, string *table, int table_size) {
	string a = "n";
	for (int i = 0; (i < (table_size*4)); (i = (i + 4))) {
//		cout << "state1" << endl;
//		cout << table[i] << endl;
//		cout << state << endl;
		if (table[i] == state) {
//			cout << "symbol" << endl;
//			cout << table[i+1] << endl;;
//			cout << symbol << endl;
			if (table[i+1] == symbol) {
				a = table[i+3];
			} 
		}
	}
	return a;
}

// The function that perform the bottom-up operation
// Stack<pair<string, int>>*  Stack<string>*  Stack<string>*  Vector<string>*  string* Int string* Parsetree* vector<Parsetree*>* -> Int
int operation(stack<pair<string, string> > *act, stack<string> *st, stack<string> *sym, vector<string> *r, string *t, int t_size, string *ru, int nost, Parsetree **root, vector<Parsetree *> *tree) {
	for (int i = 0; i < r->size(); i++) {
		pair<string, string> p = searchpair(st->top(), (*r)[i], t, t_size);
		if ((p.first == "n") && (p.second == "n")) {
//			cout << "E1" << endl;
			cerr << "ERROR at " << i << endl;
			return i;
		}
		act->push(p);
		if (p.first == "shift") {
			st->push(p.second);
			sym->push((*r)[i]);
		} else if (p.first == "reduce") {
			reduce(stringtoint(p.second), ru, st, sym, root, tree);
//			cout << st->top() << endl;
//			cout << sym->top() << endl;
			string nextstate = updatestate(st->top(), sym->top(), t, t_size);
			if (nextstate == "n") {
//				cout << "E2" << endl;
				cerr << "ERROR at " << i << endl;
				return i;
			}
			st->push(nextstate);
			i--;
		}
	}
	return 0;			
}

int ix = 0;

// The function that print put the parsetree and free every node
// Parsetree* -> void
void Printtree(Parsetree *tree) {
	if ((tree->children.size() == 0) && (tree->value != "dcls") && (tree->value != "statements") && (tree->value != "params")) {
//------------------------------------------------------------------------------------------------------------------------------------------
//		cout << "*printing " << tree->value << " with "<< tree->children.size() << " children " << endl;
//------------------------------------------------------------------------------------------------------------------------------------------
			cout << tree->value << " " << inp[ix];
			ix++;
	} else {
//-------------------------------------------------------------------------------------------------------------------------------------------
//		cout << "*printing " << tree->value << " with "<< tree->children.size() << " children " << endl;
//-------------------------------------------------------------------------------------------------------------------------------------------
		cout << tree->value;
	}
	for (int j = 0; j < tree->children.size(); j++) {
		cout << " " << tree->children[j]->value;
	}
	cout << endl;
//	cout << "value: " << endl;
//        cout << tree->value << endl;
//        cout << "number of children: " << endl;
//        cout << tree->children.size() << endl;
	for (int i = 0; i < tree->children.size(); i++) {
//------------------------------------------------------------------------------------------------------------------------------------------
//		cout << "move on to the " << i+1 << "th children" << endl;
//------------------------------------------------------------------------------------------------------------------------------------------
		Printtree(tree->children[i]);
	}
	delete tree;
//------------------------------------------------------------------------------------------------------------------------------------------
//	cout << endl;
//	cout << endl;
//	cout << endl;
//------------------------------------------------------------------------------------------------------------------------------------------
	return;
}

// main program
int main() {
	// vector and stack for each required data
	stack< pair<string, string> > Action;
	stack<string> States;
	stack<string> Symbol;
	vector<string> Remain;

	ifstream wlp4grammar("WLP4.lr1");

	// read the terminal state and construct an array of them
	int noftstate;
	wlp4grammar >> noftstate;
	string accepting[noftstate];
	skipLine(wlp4grammar);
	for (int i = 0; i < noftstate; i++) {
		string t_state;
		getline(wlp4grammar, t_state);
		accepting[i] = t_state;
	}

	// skip the non-terminal states and the start state
	int nofnstate;
	wlp4grammar >> nofnstate;
	skipLine(wlp4grammar);
	for (int i = 0; i < nofnstate; i++) {
		skipLine(wlp4grammar);
	}
	skipLine(wlp4grammar);

	// allocate the array and fpr the rules;
        int numRules = 0;
        wlp4grammar >> numRules;
        skipLine(wlp4grammar);
        string *rules = new string[numRules];

	// get the rules and save them in the array in the corresponding index
	getrules(wlp4grammar, rules, numRules);

	// read the number of state
	int nofstate;
	wlp4grammar >> nofstate;
	skipLine(wlp4grammar);

	// read the size of the bottom-up table
	int softable;
	wlp4grammar >> softable;
	skipLine(wlp4grammar);
	

//------------------------------------------------------------------------------------------------
//	cout << "Rules: " << endl;
//	for (int i = 0; i < numRules; i++) {
//		cout << i << ". " << rules[i] << endl;
//	}
//	cout << "Accepting states: " << endl;
//	for (int  i = 0; i < noftstate; i++) {
//		cout << accepting[i] << endl;
//	}
//------------------------------------------------------------------------------------------------
	
	// contruct the table
	string *table = new string[softable*4];
	getTable(wlp4grammar, softable, table);

//------------------------------------------------------------------------------------------------
//	cout << "Table: " << endl;
//	for (int i = 0; i < (softable*4); (i = (i + 4))) {
//		cout << table[i] << " " << table[i+1] << " " << table[i+2] << " " << table[i+3] << endl;
//	}
//------------------------------------------------------------------------------------------------

	// close the WLP4.lr1 file
	wlp4grammar.close();

	// intitialize the stacks
	States.push("0");
	string input;
	int k = 1;
	Remain.push_back("BOF");
	while (cin >> input) {
		bool invalid = true;
		//check if the input is valid or not
		for (int i = 0; i < noftstate; i++) {
			if (trim(accepting[i]) == trim(input)) {
				invalid = false;
			}
		}
		if (invalid) {
			cerr << "ERROR at " << k << endl;
			delete [] rules;
			delete [] table;
			return 0;
		}
		Remain.push_back(input);
		k++;
		string temp;
		cin >> temp;
		inp.push_back(temp);
		getline(cin, temp);
	}
	Remain.push_back("EOF");

//-----------------------------------------------------------------------------------------------------
//	cout << "Remaining stack: " << endl;
//	for (int i = 0; i < Remain.size(); i++) {
//		cout << Remain[i] << endl;
//	}
//
//	cout  << "State: " << endl;
//	while (!States.empty()) {
//		cout << States.top() << endl;
//		States.pop();
//	}
//-----------------------------------------------------------------------------------------------------

	// vector required to build a parse tree
	vector<Parsetree *> tree;
	// pointer for the begining of the parse tree
	Parsetree *parsetree;

	// perform the bottom up operation
	int error = operation(&Action, &States, &Symbol, &Remain, table, softable, rules, nofstate, &parsetree, &tree);

	// Terminate the program if there was a parsing error
	if (error != 0) {
		delete [] rules;
		delete [] table;
		return 0;
	}

	// vector the store the output
	vector<string> out;

//------------------------------------------------------------------------------------------------------
//	cout << "Action: " << endl;
//------------------------------------------------------------------------------------------------------

	// Read the Action stack and print out the corresping transitions, wich result in printing out the cfg-r
	while (!Action.empty()) {
		if (Action.top().first == "reduce") {
			out.push_back(rules[stringtoint(Action.top().second)]);
		}
//------------------------------------------------------------------------------------------------------
//		if (Action.top().first == "reduce") {
//			cout << Action.top().first << " " << Action.top().second <<  endl;
//		}
//------------------------------------------------------------------------------------------------------
		Action.pop();
	}	

	// Print the transition
//	cout << rules[0] << endl;
//	for (int k = 0; k < out.size(); k++) {
//		cout << out[k] << endl;
//	}

//	cout << endl;


//	for (int p = 0; p < inp.size(); p++) {
//		cout << inp[p] << endl;
//	}

	// Print out the parsetree;
	cout << rules[0] << endl;
	cout << "BOF BOF" << endl;
	Printtree(parsetree);
	cout << "EOF EOF" << endl;

	// Free the rules and table
	delete [] rules;
	delete [] table;	

	return 1;	
}
