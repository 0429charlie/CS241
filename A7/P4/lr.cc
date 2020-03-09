#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <utility>

using namespace std;

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

// The function perform the reduce operation
// Int Sting* stack<string>* stacl<string>* -> void
void reduce(int idx, string *rule, stack<string> *state, stack<string> *symbol) {
	string s;
	s = rule[idx];
	stringstream stream;
	stream << s;
	string L;
	stream >> L;
	string R;
	if (!(stream >> R)) {
		symbol->push(L);
	} else {
//		cout << "popping: " << symbol->top() << endl;
//		cout << "popping: " << state->top() << endl;
		symbol->pop();
		state->pop();
		while (stream >> R) {
//			cout << "popping: " << symbol->top() << endl;
//			cout << "popping: " << state->top() << endl;
			symbol->pop();
			state->pop();
		}
		symbol->push(L);
	}
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
// Stack<pair<string, int>>*  Stack<string>*  Stack<string>*  Vector<string>*  string* Int string* -> Int
int operation(stack<pair<string, string> > *act, stack<string> *st, stack<string> *sym, vector<string> *r, string *t, int t_size, string *ru, int nost) {
	for (int i = 0; i < r->size(); i++) {
		pair<string, string> p = searchpair(st->top(), (*r)[i], t, t_size);
		if ((p.first == "n") && (p.second == "n")) {
//			cout << "E1" << endl;
			cerr << "ERROR at " << (i+1) << endl;
			return (i+1);
		}
		act->push(p);
		if (p.first == "shift") {
			st->push(p.second);
			sym->push((*r)[i]);
		} else if (p.first == "reduce") {
			reduce(stringtoint(p.second), ru, st, sym);
//			cout << st->top() << endl;
//			cout << sym->top() << endl;
			string nextstate = updatestate(st->top(), sym->top(), t, t_size);
			if (nextstate == "n") {
//				cout << "E2" << endl;
				cerr << "ERROR at " << (i+1) << endl;
				return (i+1);
			}
			st->push(nextstate);
			i--;
		}
	}
	return 0;			
}

// main program
int main() {
	// vector and stack for each required data
	stack< pair<string, string> > Action;
	stack<string> States;
	stack<string> Symbol;
	vector<string> Remain;

	// read the terminal state and construct an array of them
	int noftstate;
	cin >> noftstate;
	string accepting[noftstate];
	skipLine(cin);
	for (int i = 0; i < noftstate; i++) {
		string t_state;
		getline(cin, t_state);
		accepting[i] = t_state;
	}

	// skip the non-terminal states and the start state
	int nofnstate;
	cin >> nofnstate;
	skipLine(cin);
	for (int i = 0; i < nofnstate; i++) {
		skipLine(cin);
	}
	skipLine(cin);

	// allocate the array and fpr the rules;
        int numRules = 0;
        cin >> numRules;
        skipLine(cin);
        string *rules = new string[numRules];

	// get the rules and save them in the array in the corresponding index
	getrules(cin, rules, numRules);

	// read the number of state
	int nofstate;
	cin >> nofstate;
	skipLine(cin);

	// read the size of the bottom-up table
	int softable;
	cin >> softable;
	skipLine(cin);
	

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
	getTable(cin, softable, table);

//------------------------------------------------------------------------------------------------
//	cout << "Table: " << endl;
//	for (int i = 0; i < (softable*4); (i = (i + 4))) {
//		cout << table[i] << " " << table[i+1] << " " << table[i+2] << " " << table[i+3] << endl;
//	}
//------------------------------------------------------------------------------------------------

	// intitialize the stacks
	States.push("0");
	string input;
	int k = 1;
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
			 // Free the allocated memory
		        delete [] rules;
    			delete [] table;
			return 0;
		}
		Remain.push_back(input);
		k++;
	}

//-----------------------------------------------------------------------------------------------------
//	cout << "Remaining stack: " << endl;
//	for (int i = 0; i < Remain.size(); i++) {
//		cout << Remain[i] << endl;
//	}

//	cout  << "State: " << endl;
//	while (!States.empty()) {
//		cout << States.top() << endl;
//		States.pop();
//	}
//-----------------------------------------------------------------------------------------------------


	// perform the bottom up operation
	int error = operation(&Action, &States, &Symbol, &Remain, table, softable, rules, nofstate);

	// Terminate the program if there was a parsing error
	if (error != 0) {
		 // Free the allocated memory
	        delete [] rules;
        	delete [] table;
		return 0;
	}

	// vector the store the output
	vector<string> out;

	// Read the Action stack and print out the corresping transitions, wich result in printing out the cfg-r
	while (!Action.empty()) {
		if (Action.top().first == "reduce") {
			out.push_back(rules[stringtoint(Action.top().second)]);
		}
//------------------------------------------------------------------------------------------------------
//		cout << Action.top().first << " " << Action.top().second <<  endl;
//------------------------------------------------------------------------------------------------------
		Action.pop();
	}	

	// Print the transition
	for (int k = (out.size() - 1); k >= 0; k--) {
		cout << out[k] << endl;
	}
	cout << rules[0] << endl;

	// Free the allocated memory
	delete [] rules;
	delete [] table;	

	return 1;	
}
