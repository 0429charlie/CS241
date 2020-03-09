#include <iostream>
#include <string>
using std::string;
using std::istream;
using std::ostream;

//Skip the grammar part of the input.
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
  in >> numRules;
  skipLine(in);
  
  // skip the lines containing the production rules
  for (i = 0; i < numRules; i++) {
    skipLine(in);
  }
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

// Function that return 42 if there is and string "id" in the input or do the recursion if  the string "expr" is presented
// ISTREAM -> INT
int getnumber(istream &in) {
	int a = 0;
	string word;
	in >> word;
	if (word == "S") {
		skipLine(in);
		a = getnumber(in);
	} else if (word == "expr") { 
		in >> word;
		if (word == "expr") {
			skipLine(in);
			a = getnumber(in) - getnumber(in);
		} else {
			a = getnumber(in);
		}
	} else if (word == "term") {
		in >> word;
		if (word == "id") {
			return 42;
		} else {
			skipLine(in);
			a = getnumber(in);
		}
	}
	return a;
}

// Reads a .cfg file and prints the left-canonical
// derivation without leading or trailing spaces.
int main() {
  skipGrammar(std::cin);
  //printDerivation(std::cin, std::cout);
	std::cout << getnumber(std::cin) << '\n';
}
