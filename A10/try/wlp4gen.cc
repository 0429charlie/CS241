#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <sstream>

#include <stddef.h>

using namespace std;

// Structure of a node
struct Node {
	string lexer;
	string value;
	string kind;
	vector<Node *> children;
	Node *parent;
};

// globle variable use to keep track the number of while loop
int whileloopi = 0;
// globle variable use to keep track the number of if statement
int ifst = 0;

// The function that check if the current node is a leaf
// String -> Bool
bool Isleaf(string value) {
	if ((value == "BOF") || (value ==  "BECOMES") || (value ==  "COMMA") || (value == "ELSE") || (value == "EOF") || (value == "EQ") || (value == "GE") || (value == "GT") || (value == "ID") || (value == "IF") || (value == "INT") || (value == "LBRACE") || (value == "LE") || (value == "LPAREN") || (value == "LT") || (value == "MINUS") || (value == "NE") || (value == "NUM") || (value == "PCT") || (value == "PLUS") || (value == "PRINTLN") || (value == "RBRACE") || (value == "RETURN") || (value == "RPAREN") || (value == "SEMI") || (value == "SLASH") || (value == "STAR") || (value == "WAIN") || (value == "WHILE") || (value == "AMP") || (value == "LBRACK") || (value == "RBRACK") || (value == "NEW") || (value == "DELETE") || (value == "NULL")) {
		return true;
	} else {
		return false;
	}
}

// The function that reads in a node and update it type
// Node* Istream -> Int
// requires: the node need to be the leaf
int updatetype(Node *leaf, istream &in) {
	string lexer;
	string value;
	string s;
	stringstream stream;
	getline(in, s);
        stream << s;
        stream >> lexer;
	stream >> value;
	if (lexer == "WAIN") {
		leaf->value = "wain";
		leaf->parent->value = "wain";
	} else {
		leaf->value = value;
	}

	if (lexer == "INT") {
                leaf->kind = "int";
                for (Node *parent = leaf->parent; ((parent->lexer != "procedures") && (parent->lexer !=  "dcls") && (parent->lexer != "paramlist")); parent = parent->parent) {
                        if (parent->kind == "") {
                                parent->kind = "int";
                        } else {
                                break;
                        }
                }
        } else if (lexer == "ID") {
                leaf->kind = leaf->parent->kind;
		for (Node *parent = leaf->parent; ((parent->lexer != "procedures") && (parent->lexer != "dcls") && (parent->lexer != "paramlist") && (parent->lexer != "main")); parent = parent->parent) {
			if (parent->lexer == "term") {
				break;
			} else {
				parent->value = leaf->value;
				if ((parent->kind != "") && (leaf->kind != "")) {
					leaf->kind = parent->kind;
				}
			}
		}
        } else if (lexer == "NUM") {
                leaf->kind = "int";
                for (Node *parent = leaf->parent; parent->lexer != "term"; parent = parent->parent) {
                        if (parent->kind == "") {
                                parent->kind = "int";
                        } else {
                                break;
                        }
                }
        } else if (lexer == "NULL") {
                leaf->kind = "int*";
                for (Node *parent = leaf->parent; parent->lexer != "term"; parent = parent->parent) {
                        if (parent->kind == "") {
                                parent->kind = "int*";
                        } else {
                                break;
                        }
                }
        } else if (lexer == "STAR") {
                if ((leaf->parent->lexer == "factor") || (leaf->parent->lexer == "lvalue")) {
                        for (Node *parent = leaf->parent; parent->lexer != "term"; parent = parent->parent) {
                                if (parent->kind == "int") {
                                        parent->kind = "int*";
                                } else {
                                        parent->kind = "int";
                                }
                        }
                } else if (leaf->parent->lexer == "term") {
                        string k = leaf->parent->kind;
                        for (Node *parent = leaf->parent; parent->lexer != "expr"; parent = parent->parent) {
                                parent->kind = k;
                        }
                } else {
                        for (Node *parent = leaf->parent; ((parent->lexer != "dcls") && (parent->lexer != "main") && (parent->lexer !=  "procedures")); parent = parent->parent) {
                                parent->kind = "int*";
                        }
                }
        } else {
	}
		
	return 0;
}

// Function that perform the action procedure according to wlp4 rules
// Node* Istream -> Int
int performaction(Node *n, istream &in) {
	string s;
	getline(in, s);
	stringstream stream;
        stream << s;
	stream >> s;
	while (stream >> s) {
		Node *children = new Node;
		children->lexer = s;
		if ((s == "procedures") || (s == "dcls")|| (s == "paramlist")) {
			children->value = s;
			children->kind = s;
		} else {
			children->value = "";
			children->kind = "";
		}
		children->parent = n;
		n->children.push_back(children);
	}
	int err = 0;
	for (int i = 0; i < n->children.size(); i++) {
		err = 0;
		if (Isleaf(n->children[i]->lexer)) {
			err = updatetype(n->children[i], in);
		} else {
			err = performaction(n->children[i], in);
		}
		if (err == 1) {
			return err;
		}
	}
	return 0;
}

// Function that reads the parameter-list of a procedure and record all symbol to the symbol table
// vector<pair<string, string>*>* Node* -> Void
// requires: the children of the parameterslist can not be empty
void paramlist(vector<pair <string, string> *> *symbol_table, Node *parameterslist) {
        for (int i = 0; i < parameterslist->children.size(); i++) {
		if (parameterslist->children[i]->lexer == "dcl") {
			pair<string, string> *p = new pair<string, string>;
			p->first = parameterslist->children[i]->value;
			p->second = parameterslist->children[i]->kind;
			symbol_table->push_back(p);
		} else if (parameterslist->children[i]->lexer == "paramlist") {
			paramlist(symbol_table, parameterslist->children[i]);
		} else {
		}
	}
	return;
}

// Function that reads the parameter-list of a procedure and record all symbol to the symbol table
// vector<pair<string, string>*>* Node* -> Void
void param(vector<pair <string, string> *> *symbol_table, Node *parameterslist) {
	if (parameterslist->children.size() == 0) {
		return;
	} else {
		paramlist(symbol_table, parameterslist->children[0]);
	}
}

// Function that reads the declaration list of a procedure and record all symbol to the symbol table
// vector<pair<string, string>*>* Node* -> Void
void dcls(vector<pair<string, string> *> *symbol_table, Node *declarations) {
	if (declarations->children.size() == 0) {
		return;
	} else {
		pair<string, string> *p = new pair<string, string>;
		p->first = declarations->children[1]->value;
		p->second = declarations->children[1]->kind;
		symbol_table->push_back(p);
		dcls(symbol_table, declarations->children[0]);
		return;
	}
}

// Function use to traverse tree and contruct a symbol table
// Node* vector<vector<pair<string, string>*>*>* -> Int
int traversetree(Node *root, vector<vector<pair<string, string> *> *> *v_for_s) {
	int c = 0;
	for (int i = 0; i < root->children.size(); i++) {
		if (root->children[i]->lexer == "procedures") {
			c = traversetree(root->children[i], v_for_s);	
		} else if (root->children[i]->lexer == "procedure") {
			vector<pair<string, string> *> *procedure = new vector<pair<string, string> *>;
			v_for_s->push_back(procedure);
			pair<string, string> *p = new pair<string, string>;
			p->first = root->children[i]->value;
			p->second = "";
			procedure->push_back(p);
			param(procedure, root->children[i]->children[3]);
			dcls(procedure, root->children[i]->children[6]);
		} else if (root->children[i]->lexer == "main") {
			vector<pair <string, string> *> *wain = new vector<pair <string, string> *>;
			v_for_s->push_back(wain);
			pair<string, string> *w = new pair<string, string>;
			w->first = root->children[i]->value;
//			w->first = "wain";
			w->second = "";
			wain->push_back(w);
			pair<string, string> *dcl1 = new pair <string, string>;
			dcl1->first = root->children[i]->children[3]->value;
			dcl1->second = root->children[i]->children[3]->kind;
			pair<string, string> *dcl2 = new pair <string, string>;
			dcl2->second = root->children[i]->children[5]->kind;
			dcl2->first = root->children[i]->children[5]->value;
			wain->push_back(dcl1);
			wain->push_back(dcl2);
			dcls(wain, root->children[i]->children[8]); 
		} else {
		}
	}
	return c;
}

// Functiont that free the allocated parse-tree
// Node* -> Void
void free(Node *r) {
	if (r->children.size() == 0) {
		delete r;
	} else {
		for (int i = 0; i < r->children.size(); i++) {
			free(r->children[i]);
		}
	}
}

// Function that print out the mips code prologue
// vector<vector<pair<string, string>*>*>* -> Void
void prologue(vector<vector<pair<string, string> *> *> *s) {
	cout << ".import print" << endl;
	cout << "lis $4" << endl;
	cout << ".word 4" << endl;
	cout << "lis $11" << endl;
	cout << ".word 1" << endl;
	// save enough space for variables to be saved on RAM and update $29 and $30 respectively
	stringstream ss;
	ss << (*((*s)[s->size()-1]))[0]->second;
	int sizeofst;
	ss >> sizeofst;
	sizeofst = (4 * (sizeofst + ((*s)[s->size()-1])->size()));
	cout << "lis $3" << endl;
	cout << ".word " << sizeofst << endl;
	cout << "sub $30, $30, $3" << endl;
	cout << "add $29, $30, $0" << endl;
	cout << "add $3, $0, $0" << endl;
	cout << "sw $31, -4($30)" << endl;
	cout << "sub $30, $30, $4" << endl;
}

// Function that print out the mips code epilogue
// Void -> Void
void epilogue() {
	cout << "add $30, $30, $4" << endl;
	cout << "lw $31, -4($30)" << endl;
	cout << "jr $31" << endl;
}

// Forward declaration of the function expr
void expr(Node *e, vector<pair<string, string>*> *s);

// Function that handle all the operation that a "factor" can leads to
// Node*  vector<pair<string, string>*>* -> Void
void factor(Node *f, vector<pair<string, string>*> *s) {

// Note, modification need to be make of dealling with procedure(s) [ID is the procedure(s) name]

	if (f->children[0]->lexer == "ID") {
		string fn_n = (*s)[0]->second;
		stringstream offset;
		offset << fn_n;
		int os;
		offset >> os;
		for (int i = 0; i < s->size(); i++) {
			if ((f->children[0]->value == (*s)[i]->first) && (i != 0)) {
				os = (4*(os + i));
			}
		}
		cout << "lw $3, " << os << "($29)" << endl;
	} else if (f->children[0]->lexer == "LPAREN") {
		expr(f->children[1], s);
	} else if (f->children[0]->lexer == "NUM") {
		cout << "lis $3" << endl;
		cout << ".word " << f->children[0]->value << endl;	
	}
}

// Functio that handle all the operation that a "term" can leads to
// Node*  vector<pair<string, string>*>* -> Void
void term(Node *t, vector<pair<string, string>*> *s) {

// Note, midification need to be made if dealing with pointer

	if (t->children[0]->lexer == "factor") {
		factor(t->children[0], s);
	} else {
		/*
		push $5
		*/
		term(t->children[0], s);
		cout << "sw $3, -4($30)" << endl;
		cout << "sub $30, $30, $4" << endl;
		factor(t->children[2], s);
		cout << "add $30, $30, $4" << endl;
                cout << "lw $5, -4($30)" << endl;
		if (t->children[1]->lexer == "STAR") {
			cout << "mult $3, $5" << endl;
			cout << "mflo $3" << endl;
		} else if (t->children[1]->lexer == "SLASH") {
			cout << "div $5, $3" << endl;
			cout << "mflo $3" << endl;
		} else {
			cout << "div $5, $3" << endl;
			cout << "mfhi $3" << endl;
		}
		/*
		pop $5
		*/
	}
}

// Function that handle all the operation that an "expr" leads to
// Node*  vector<pair<string, string>*>* -> Void
void expr(Node *e, vector<pair<string, string>*> *s) {

// Note, modification need to be made if dealing with pointer

	if (e->children[0]->lexer == "term") {
		term(e->children[0], s);
	} else {
		/*
		push $5
		*/
		expr(e->children[0], s);
		cout << "sw $3, -4($30)" << endl;
		cout << "sub $30, $30, $4" << endl;
		term(e->children[2], s);
		cout << "add $30, $30, $4" << endl;
		cout << "lw $5, -4($30)" << endl;
		if (e->children[1]->lexer == "PLUS") {
			cout << "add $3, $3, $5" << endl;
                } else {
			cout << "sub $3, $5, $3" << endl;
                }
		/*
		pop $5
		*/
	}
}

// Function that handle all the operation that a "lvalue" leads to
// Node*  vector<pair<string, string>*>* -> Void
void lvalue(Node *lv, vector<pair<string, string>*> *s) {
	if (lv->children[0]->lexer == "ID") {
		/*
		push $5
		*/
		stringstream ss;
                ss << (*s)[0]->second;
                int offset;
                ss >> offset;
                for (int i = 0; i < s->size(); i++) {
                        if ((((*s)[i]->first) == lv->children[0]->value) && (i != 0)) {
				cout << "lis $5" << endl;
				cout << ".word " << (4*(offset + i)) << endl;
				cout << "add $3, $5, $29" << endl;
				break;
                        }
                }
		/*
		pop $5
		*/	
	} else if (lv->children[0]->lexer == "LPAREN") {
		lvalue(lv->children[1], s);
	}
}

// Helper functino for the test function
// Node* vector<pair<string, string>*>* -> Void
void testpushandpop(Node *tpap, vector<pair<string, string>*> *s) {
	expr(tpap->children[0], s);
	cout << "sw $3, -4($30)" << endl;
	cout << "sub $30, $30, $4" << endl;
	expr(tpap->children[2], s);
	cout << "add $30, $30, $4" << endl;
	cout << "lw $5, -4($30)" << endl;
}


// Function that handle all the operation that a "test" leads to
// Node*  vector<pair<string, string>*>* -> Void
void test(Node *tst, vector<pair<string, string>*> *s) {
	/*
	push $5, $6, $7
	*/
	if (tst->children[1]->lexer == "LT") {
		testpushandpop(tst, s);
		cout << "slt $3, $5, $3" << endl;
	} else if (tst->children[1]->lexer == "EQ") {
		testpushandpop(tst, s);
		cout << "slt $6, $5, $3" << endl;
                cout << "slt $7, $3, $5" << endl;
                cout << "add $3, $6, $7" << endl;
		cout << "sub $3, $11, $3" << endl;
	} else if (tst->children[1]->lexer == "NE") {
		testpushandpop(tst, s);
		cout << "slt $6, $5, $3" << endl;
		cout << "slt $7, $3, $5" << endl;
		cout << "add $3, $6, $7" << endl;
	} else if (tst->children[1]->lexer == "LE") {
		testpushandpop(tst, s);
		cout << "slt $3, $3, $5" << endl;
		cout << "sub $3, $11, $3" << endl;
	} else if (tst->children[1]->lexer == "GE") {
		testpushandpop(tst, s);
		cout << "slt $3, $5, $3" << endl;
		cout << "sub $3, $11, $3" << endl;
	} else if (tst->children[1]->lexer == "GT") {
		testpushandpop(tst, s);
		cout << "slt $3, $3, $5" << endl;
	}
	/*
	pop $5, $6, $7
	*/
}

// Forward declaration of statements
void statements(Node *ss, vector<pair<string, string>*> *s);

// Function that handle all the operation that a "statement" leads to
// Node*  vector<pair<string, string>*>* -> Void
void statement(Node *st, vector<pair<string, string>*> *s) {
	if (st->children[0]->lexer == "PRINTLN") {
		/*
		push $5
		*/
		expr(st->children[2], s);
		cout << "sw $1, -4($30)" << endl;
		cout << "sub $30, $30, $4" << endl;
		cout << "sw $31, -4($30)" << endl;
		cout << "sub $30, $30, $4" << endl;
		cout << "add $1, $3, $0" << endl;
		cout << "lis $5" << endl;
		cout << ".word print" << endl;
		cout << "jalr $5" << endl;
		cout << "add $30, $30, $4" << endl;
                cout << "lw $31, -4($30)" << endl;
		cout << "add $30, $30, $4" << endl;
		cout << "lw $1, -4($30)" << endl;
		/*
		pop $5
		*/
	} else if (st->children[0]->lexer == "lvalue") {
		/*
		push $5
		*/
		lvalue(st->children[0], s);
		cout << "sw $3, -4($30)" << endl;
		cout << "sub $30, $30, $4" << endl;
		expr(st->children[2], s);
		cout << "add $30, $30, $4" << endl;
		cout << "lw $5, -4($30)" << endl;
		cout << "sw $3, 0($5)" << endl;
		/*
		pop $5
		*/
	} else if (st->children[0]->lexer == "WHILE") {
		int wl = whileloopi;
		whileloopi++;
		cout << "whileloop" << wl << ":" << endl;
		test(st->children[2], s);
		cout << "beq $3, $0, endwhileloop" << wl << endl;
		statements(st->children[5], s);
		cout << "beq $0, $0, whileloop" << wl << endl;
		cout << "endwhileloop" << wl << ":" << endl;
	} else if (st->children[0]->lexer == "IF") {
		int ist = ifst;
		ifst++;
		test(st->children[2], s);
		cout << "beq $3, $0, else" << ist << endl;
		statements(st->children[5], s);
		cout << "beq $0, $0, done" << ist << endl;
		cout << "else" << ist << ":" << endl;
		statements(st->children[9], s);
		cout << "done" << ist << ":" << endl;	
	}
}


// Function that handle all the operation that "statements" leads to
// Node*  vector<pair<string, string>*>* -> Void
void statements(Node *ss, vector<pair<string, string>*> *s) {
	if (ss->children.size() == 0) {
		return;
	} else {
		statements(ss->children[0], s);
		statement(ss->children[1], s);
	}
}

// Function that handle all the operation that "dcls" leads to
// Node*  vector<pair<string, string>*>* -> Void
void e_dcls(Node *dcls, vector<pair<string, string>*> *s) {
	if (dcls->children.size() == 0) {
		return;
	} else {
		/*
		push $5
		*/
		e_dcls(dcls->children[0], s);
		stringstream ss;
		ss << (*s)[0]->second; 
		int offset;
		ss >> offset;
		for (int i = 0; i < s->size(); i++) {
			if ((((*s)[i]->first) == dcls->children[1]->value) && (i != 0)) {
				cout << "lis $5" << endl;
				cout << ".word " << dcls->children[3]->value << endl;
				cout << "sw $5, " << (4*(offset + i)) << "($29)" << endl;
				break;
			}
		}
		/*
		pop $5
		*/
	}
}

// Function that reads a parse tree and print out the correspoinding mips code
// Node*  vector<vector<pair<string, string>*>*>* -> Void
void execute(Node *r, vector<vector<pair<string, string> *> *> *s) {
	for (int i = 0; i < r->children.size(); i++) {
		if (r->children[i]->lexer == "expr") {
			expr(r->children[i], (*s)[s->size()-1]);
		} else if (r->children[i]->lexer == "statements") {
			statements(r->children[i], (*s)[s->size()-1]);
		} else if (r->children[i]->lexer == "dcls") {
			e_dcls(r->children[i], (*s)[s->size()-1]);
		} else if (!Isleaf(r->children[i]->lexer) && (r->children[i]->lexer != "dcl")) {
			execute(r->children[i], s);
		}
	}
}

// Main program
int main() {	
	// contruct the root of the tree on heap from a wlp4i file
	Node *root = new Node;
	string v;
	cin >> v;
	root->lexer = v;
	root->value = v;
	root->kind = v;
	root->parent = NULL;
	Node *c1 = new Node;
	cin >> v;
	c1->lexer = v;
	c1->value = v;
	c1->kind = v;
	c1->parent = root;
	root->children.push_back(c1);
	Node *c2 = new Node;
	cin >> v;
	c2->lexer = v;
	c2->value = v;
        c2->kind = v;
	c2->parent = root;
	root->children.push_back(c2);
	Node *c3 = new Node;
	cin >> v;
	c3->lexer = v;
	c3->value = v;
	c3->kind = v;
	c3->parent = root;
	root->children.push_back(c3);

	// Skip a line in order to use get line in the helper function
	string s;
	getline(cin, s);

	// Go throgh and update each children
	for (int i = 0; i < root->children.size(); i++) {
		if (Isleaf(root->children[i]->lexer)) {
			if (updatetype(root->children[i], cin) == 1) {
				return 0;
			}
		} else {
			if (performaction(root->children[i], cin) == 1) {
				return 0;
			}
		}
	}
	
	// vector dynamically aloocated to keep track all the symbol table
	vector<vector<pair<string, string> *> *> *f_table = new vector<vector<pair<string, string> *> *>;

	// Build a symbol table
	if (traversetree(root, f_table) == 1) {
		return 0;
	}

//-------------------------------------------------------------A9--------------------------------------------------------------------------------
	// Update the symbol table to record the size of each symbol table
	(*((*f_table)[0]))[0]->second = "0";
	for (int k = 1; k < f_table->size(); k++) {
		if (k >= f_table->size()) {
			break;
		}
		string size = (*((*f_table)[k-1]))[0]->second;
		int s1 = ((*f_table)[k])->size();
		stringstream ss;
		ss << size;
		int s2;
		ss >> s2;
		s1 = s1 + s2;
		ss << s1;
		ss >> size;
		(*((*f_table)[k]))[0]->second = size;
	}

	// Print out the prologue
	prologue(f_table);
	// Print out the mips code that save the parameters of wain onto the stack
	stringstream wain_of;
	wain_of << (*((*f_table)[f_table->size()-1]))[0]->second;
	int wain_off;
	wain_of >> wain_off;
	cout << "sw $1, " << (4*(wain_off + 1)) << "($29)" << endl;
	cout << "sw $2, " << (4*(wain_off + 2)) << "($29)" << endl;

	// Print out the equivalent mips code
	execute(root, f_table);	
	// Print out the epilpgue
	epilogue();
//-----------------------------------------------------------------------------------------------------------------------------------------------

	// Print out the symbol table and free the ememory
	for (int j = 0; j < f_table->size(); j++) {
		for (int z = 0; z < (*f_table)[j]->size(); z++) {
//			cerr << (*((*f_table)[j]))[z]->first;
			if ((*((*f_table)[j]))[z]->second == "") {
//				cerr << endl;
			} else {
//				cerr << " " << (*((*f_table)[j]))[z]->second << endl;
			}
			delete (*((*f_table)[j]))[z];
		}
		delete (*f_table)[j];
	}
	delete f_table;
	free(root);
}
