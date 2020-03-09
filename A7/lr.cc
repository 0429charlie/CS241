#include <iostream>
#include <string>
#include <sstream>

using namespace std;

//Skip the grammar part of the input except the rules.
void skipLine(istream &in) {
  string s;
  getline(in, s);
}
 
void skipGrammar(istream &in) {
  int i, numTerm, numNonTerm, numRules;

  // read the number of terminals and move to the next line
  in >> numTerm;
  skipLine(in);
  
  // skip the lines containing the terminals
  for (i = 0; i < numTerm; i++) {
    skipLine(in);
  }
  
  // read the number of non-terminals and move to the next line
  in >> numNonTerm;
  skipLine(in);
  
  // skip the lines containing the non-terminals
  for (i = 0; i < numNonTerm; i++) {
    skipLine(in);
  }
  
  // skip the line containing the start symbol
  skipLine(in);
  
  // read the number of rules and move to the next line
  //in >> numRules;
  //skipLine(in);
  
  // skip the lines containing the production rules
  //for (i = 0; i < numRules; i++) {
  //  skipLine(in);
  //}
}

string trim(const string &str) {
  size_t begin = str.find_first_not_of(" \t\n");
  if (begin == string::npos) return "";

  size_t end = str.find_last_not_of(" \t\n");

  return str.substr(begin, end - begin + 1);
}

// Prints the derivation with whitespace trimmed.
void printDerivation(istream &in, ostream &out) {
  string line;

  while (getline(in, line)) {
    out << trim(line) << '\n';
  }
}

// Functioan that reads the rule of the grammar and save them in an array in the corresponding index.
// ISTREAM -> *string
// require: the allocated memory need to be free in future
string * getrules(istream &in) {

	// number of available rules
	int numRules = 0;

	// read the number of rules and move to the next line
	in >> numRules;
	skipLine(in);

	// string to store the rules;
	string rules;
	
	// allocate the array to be return;
	string *ptr = new string[numRules];

	// read each line of the rules and save them into the array
  	for (int i = 0; i < numRules; i++) {
    		getline(in, rules);
		ptr[i] = rules;
  	}

	return ptr;
}

// Function that save all the transitions
// ISTREAM -> string *
// required: teh allocated array need to be free in future
string * getTransition(istream &in, int *l) {
	
	// get the number of transition and move on to the next line
	int numotrans = 0;
	in >> numotrans;
	skipLine(in);
	*l = (numotrans*4);

	// allocate the array to be return
	string *ptr = new string[numotrans*4];
	
	// skip all the transition
	for (int i = 0; i < numotrans*4; (i = (i + 4))) {
		in >> ptr[i];
		in >> ptr[i+1];
		in >> ptr[i+2];
		in >> ptr[i+3];
		skipLine(in);
	}

	return ptr;
}

// Function that read the transition and match it with the input and print out the correspinding output
// ISTREAM Int STring String* Ostream -> void
void printTransition(string *transition, string n, string x, string *ptr, int *l, ostream &out) {
	// get the number of transition
	int notrans;
	notrans = *l;
	
	// match and check the content of each transition
	for (int i = 0; i < notrans; (i = (i + 4))) {
		string a;
		a = transition[i];
		if (a == n) {
			string s;
			s = transition[i+1];
			if (s == x) {
				string sor;
				sor = transition[i+2];
				if (sor == "reduce") {
					string index;
					index = transition[i+3];
					int idx;
					istringstream (index) >> idx;
					out << sor << " " << ptr[idx] << endl;
					return;
				} else  {
					string shift;
					shift = transition[i+3];
					out << sor << " " << shift << endl;
					return;
				}
			} else {
				if (i >= (notrans - 4)) {
//					out << "error" << endl;
					cerr << "error" << endl;
				}
			}
		} else {
			if (i >= (notrans - 4)) {
//				out << "error" << endl;
				cerr << "error" << endl;
			}
		}
	}
}

// Reads lr1 file and prints the corresponduing transition
int main() {
  skipGrammar(cin);
  //printDerivation(std::cin, std::cout);
	
//	cout << "skipgrammar" << endl;
		

	// construct the array of the rules
	string *s = getrules(cin);

//	cout << "getrules" << endl;
	
	// number of available states
	int state = 0;
	
	// save the number of available states
	cin >> state;

//	cout << "state: " << state << endl;

	// length if the available trans;
	int *length = new int;

	
	
	// skip all the transition inorder to get the input
	string *trans = getTransition(cin, length);

//	cout << "getTransition" << endl;


	string n = "";
	string x = "";

	while (cin >> n) {
		if (!(cin >> x)) {
//			cout << "error" << endl;
			cerr << "error" << endl;
			return 0;
		}
//		cout << "n: " << n << endl;
//		cout << "x: " << x << endl;

		int nc;
		istringstream (n) >> nc;

		// print error and return if state don't exist
		if (nc >= state) {
//			cout << "error" << endl;
			cerr << "error" << endl;
			return 0;
		}
	
		// print the corresponding transition
		printTransition(trans, n, x, s, length, cout);
	}
	
	delete [] s;
	delete [] trans;
	delete length;

	return 1;
}
