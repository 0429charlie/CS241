#include "kind.h"
#include "lexer.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
// Use only the neeeded aspects of each namespace
using std::string;
using std::vector;
using std::endl;
using std::cerr;
using std::cin;
using std::getline;
using ASM::Token;
using ASM::Lexer;

// include the requrred library
#include <utility>

// Import the Kind of the label
using namespace ASM;
using namespace std;

/*
//-----------------------------------------------------------------------------------------------------------------------------------------------
// nofintr is the number of instruction we had so far
int nofintr = 0;
// pasttwo idicate if it is first past or the second past
int pasttwo = 0;

// Helper functions for reading different type of input


// readid reads in a line that start with a label with out colum (an id) and get the address of the id
// vector<Token *> (vector<pair<string, int>> *) Int -> Void
void readid(vector<Token *> v, vector<pair<string, int> *> *pr, int index) {

        // throw error if this lable is not in the right place
	if (index == 0) {
		throw string("ERROR: colum needed in order to declare a label");
	} else if (index != (v.size() - 1)) {
                throw string("ERROR: This id should be the last word of the line");
        } else {
		if (pasttwo == 1) {
                	// Look through the symbol table
                	for (int j = 0; j < pr->size(); j++) {
                        	if ((*pr)[j]->first == v[index]->getLexeme()) {
                                	// get the address of found the label in the table
                                	int a = (*pr)[j]->second;
                                	// print out as binary representation
                                	putchar(a>>24);
                                	putchar(a>>16);
                                	putchar(a>>8);
                                	putchar(a);
                                	return;
                        	}
                	}	
                	// if not found in symbol table
                	throw string("ERROR: Label not declared");
		}
                return;
        }
}


// readword reads in a line of assembly language that has .word as the first word and print out the machine code of the line to standard output
// vector<Token *> Int vector<pair<string, int> *> * -> Void
// requires: the first element of the vector need to be the pointer to the word .word
void readword(vector<Token *> v, int t, vector<pair<string, int> *> *ptr) {
	if ((v.size() - t) != 2) {
		// throw error if number of input don't match
		throw string("ERROR: Invalid argument for .word");
	} else if (v[1+t]->getKind() == ID) {
		readid(v, ptr, (1+t));
	} else {
		if (pasttwo == 1) {
			// convert the token to integer
			Token *word = v[1+t];
			int w = word->toInt();
			// print out as binary representation
			putchar(w>>24);
			putchar(w>>16);
			putchar(w>>8);
			putchar(w);
		}
	}
	if (pasttwo == 0) {
		nofintr++;
	}
	return;
}

// readlabel reads in a line that start with a label and update the symbol table in the heap
// vector<Token *> (vector<pair<string, int>> *) Int -> Void
void readlabel(vector<Token *> v, vector<pair<string, int> *> *pr, int idx) {

	if (pasttwo == 0) {
		// variable to store name and the address of the label
		int addr;
		string s;
		// get the label name
		Token *word = v[idx];
		s = word->getLexeme();
		// drop the last character
		s = s.substr (0,s.length()-1);

		// check if it is a repeated label or not
	        for (int j = 0; j < pr->size(); j++) {
        	        if ((*pr)[j]->first == s) {
                	        throw string("ERROR: Label declared more than one time");
       			}
		}	

		// get the address of the label
		addr = 4 * nofintr;
		// allocate the memory for the new pair in the symbol table
		pair<string, int> *p = new pair<string, int>;
		p->first = s;
		p->second = addr;
		pr->push_back(p);
	}
	// read the instruction behind the label
	if (v.size() > (idx+1)) {
		int j = (idx+1);
		if (v[j]->getKind() == DOTWORD) {
			readword(v, j, pr);
		} else if (v[j]->getKind() == LABEL) {
			readlabel(v, pr, j);
		} else {
			throw string("ERROR: Invalid label");
		}
	}
	return;
}



//-----------------------------------------------------------------------------------------------------------------------------------------------
*/

// main program
int main(int argc, char* argv[]){

	 // allocate a sapce for symbol table
//        vector<pair<string, int> *> *prt = new vector<pair<string, int> *>;

  // Nested vector representing lines of Tokens
  // Needs to be used here to cleanup in the case
  // of an exception
  vector< vector<Token*> > tokLines;
  try{
    // Create a MIPS recognizer to tokenize
    // the input lines
    Lexer lexer;
    // Tokenize each line of the input
    string line;
    while(getline(cin,line)){
      tokLines.push_back(lexer.scan(line));
    }

    // Iterate over the lines of tokens and print them
    // to standard error
    vector<vector<Token*> >::iterator it;
    for(it = tokLines.begin(); it != tokLines.end(); ++it){
      vector<Token*>::iterator it2;
	//cerr << endl;
      for(it2 = it->begin(); it2 != it->end(); ++it2){
        cout << *(*it2) << endl;
      }
    }

//-------------------------------------------------------------------
	// tokLines is 2d vector that stores the mips assembly language (input)
	// first for loop get the a line of the entered essembly code.
	// second for loop get each element (instrution, value, etc).
	
	/*	
	pasttwo = 0;

	// Read a line of the input and the call the function accordingly (past 1)
        for (int i = 0; i < tokLines.size(); i++) {
                vector<Token *> token = tokLines[i];
                // read the line only if it is not an empty line
                if (token.size() != 0) {
                        if (token[0]->getKind() == DOTWORD) {
                                readword(token, 0, prt);
                        } else if (token[0]->getKind() == LABEL) {
                                readlabel(token, prt, 0);
                        } else if (token[0]->getKind() == ID) {
                                readid(token, prt, 0);
                        } else {
                                throw string("ERROR: Invalid label");
                        }
                }
        }

	// pasttwo starts
	pasttwo = 1;
	

	// Read a line of the input and the call the function accordingly (past 2)
	for (int i = 0; i < tokLines.size(); i++) {
		vector<Token *> token = tokLines[i];
		// read the line only if it is not an empty line
		if (token.size() != 0) {
			if (token[0]->getKind() == DOTWORD) {
				readword(token, 0, prt);
			} else if (token[0]->getKind() == LABEL) {
				readlabel(token, prt, 0);
			} else if (token[0]->getKind() == ID) {
				readid(token, prt, 0);
			} else {
				throw string("ERROR: Invalid label");
			}
		}
	}	
	*/

//-------------------------------------------------------------------	

  } catch(const string& msg){
    // If an exception occurs print the message and end the program
    cerr << msg << endl;
  }
  // Delete the Tokens that have been made
  vector<vector<Token*> >::iterator it;
  for(it = tokLines.begin(); it != tokLines.end(); ++it){
    vector<Token*>::iterator it2;
    for(it2 = it->begin(); it2 != it->end(); ++it2){
      delete *it2;
    }
  }

//-------------------------------------------------------------------	
	/*
	// print out the symbol table and then delete the allocated symbol table
	for (int k = 0; k < prt->size(); k++) {
		cerr << (*prt)[k]->first << " " << (*prt)[k]->second << endl;
		delete (*prt)[k];
	}
	delete prt;
	*/
}
