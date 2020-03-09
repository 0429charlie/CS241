#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <sstream>

#include <stddef.h>

using namespace std;

// Structure of a node
struct Node {
	string value;
	vector<Node *> children;
	Node *parent;
	string type;;
};

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
	string value;
	string type;
	string s;
	stringstream stream;
	getline(in, s);
        stream << s;
        stream >> value;
	if (value != leaf->value) {
		cerr << "ERROR: wrong format for wlp4i file" << endl;
		return 1;
	}
	stream >> type;
	leaf->type = type;	
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
	if (s != n->value) {
		cerr << "ERROR: wrong format for wlp4i file" << endl;
		return 1;
	} else {
		while (stream >> s) {
			Node *children = new Node;
			children->value = s;
			children->parent = n;
			children->type = "";
			n->children.push_back(children);
		}
	}
	int err;
	for (int i = 0; i < n->children.size(); i++) {
		err = 0;
		if (Isleaf(n->children[i]->value)) {
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

// Function use to traverse tree and contruct a symbol table
// Node* vector<vector<pair<string, string>>*>* vector<pair<string, string>>* -> Int
int traversetree(Node *root, vector<vector<pair<string, string> > *> *v_for_s, vector<pair<string, string> > *curr_s) {
	int cc;
	for (int i = 0; i < root->children.size(); i++) {
		if (root->children[i]->value == "WAIN") {
			pair<string, string> p; 
			p.first = root->children[i]->type;
			p.second = root->children[i-1]->type;
			curr_s->push_back(p);
		} else if (root->children[i]->value == "ID") {
			pair<string, string> p;
			if (root->value == "factor" || root->value == "lvalue") {
                                int c = 0;
                                for (int j = 0; j < curr_s->size(); j++) {
                                        if ((*curr_s)[j].first == root->children[i]->type) {
                                                c = 1;
                                        }
                                }
                                if (c == 0) {
                                        cerr << "ERROR: undeclared ID" << endl;
                                        return 1;
                                }
                        } else if (root->children[i-1]->value == "type") {
				if (root->children[i-1]->children.size() == 2) {
					p.second = "int*";
					p.first = root->children[i]->type;
				} else {
					p.first = root->children[i]->type;
					p.second = "int";
				}
				curr_s->push_back(p);
			} else {
				p.first = root->children[i]->type;
				p.second = root->children[i-1]->type;
				curr_s->push_back(p);
			}
		} else if (!Isleaf(root->children[i]->value)) {
			cc = traversetree(root->children[i], v_for_s, curr_s);	
		} else if (root->children[i]->value == "procesure" || root->children[i]->value == "main"){
			vector<pair<string, string> > *curr_s = new vector<pair<string, string> >;
        		v_for_s->push_back(curr_s);
			cc = traversetree(root->children[i], v_for_s, curr_s);
		}
	}
	return cc;
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

// Main program
int main() {	
	// contruct the root of the tree on heap from a wlp4i file
	Node *root = new Node;
	string v;
	cin >> v;
	root->value = v;
	root->type = "";
	root->parent = NULL;
	Node *c1 = new Node;
	cin >> v;
	c1->value = v;
	c1->type = "";
	c1->parent = root;
	root->children.push_back(c1);
	Node *c2 = new Node;
	cin >> v;
	c2->type = "";
	c2->value = v;
	c2->parent = root;
	root->children.push_back(c2);
	Node *c3 = new Node;
	cin >> v;
	c3->type = "";
	c3->value = v;
	c3->parent = root;
	root->children.push_back(c3);

	// Skip a line in order to use get line in the helper function
	string s;
	getline(cin, s);
	

	// Go throgh and update each children
	for (int i = 0; i < root->children.size(); i++) {
		if (Isleaf(root->children[i]->value)) {
			if (updatetype(root->children[i], cin) == 1) {
				return 0;
			}
		} else {
			if (performaction(root->children[i], cin) == 1) {
				return 0;
			}
		}
	}

	
	// vector to keep track all the symbol table
	vector<vector<pair<string, string> > *> v_s(0);
	vector<vector<pair<string, string> > *> *ptr = &v_s;

	// first dynamically located symbol table
	vector<pair<string, string> > *s_table = new vector<pair<string, string> >;
	v_s.push_back(s_table);

	// Build a symbol table
	if (traversetree(root, ptr, s_table) == 1) {
		return 0;
	}


	int wain = 0;
	
/*	// Print out the symbol table and free th ememory
	for (int j = 0; j < v_s.size(); j++) {
		for (int z = 0; z < v_s[j]->size(); z++) {
			if (((*(v_s[j]))[z].first) == "wain") {
				cerr << (*(v_s[j]))[z].first << endl;
				wain = 1;
			} else if (wain == 1) {
				cerr << (*(v_s[j]))[z].first << " " <<  (*(v_s[j]))[z].second << endl;
			} else {

			}
		}
		wain = 0;
		delete v_s[j];
	}*/

	for (int j = 0; j < v_s.size(); j++) {
               for (int z = 0; z < v_s[j]->size(); z++) {
                        if (z == 0) {
                                cerr << (*(v_s[j]))[z].first << " " << (*(v_s[j]))[z+1].second << " " << (*(v_s[j]))[z+1].second << endl;
				cerr << endl;
			} else if (((*(v_s[j]))[z].first) == "wain") {
				cerr << (*(v_s[j]))[z].first << " " << (*(v_s[j]))[z+1].second << " " << (*(v_s[j]))[z+1].second << endl;
                                wain = 1;
                        } else if (wain == 1) {
                                cerr << (*(v_s[j]))[z].first << " " <<  (*(v_s[j]))[z].second << endl;
                        } else {

                       }
                }
                delete v_s[j];
        }

//	for (int j = 0; j < v_s.size(); j++) {
  //              for (int z = 0; z < v_s[j]->size(); z++) {
//			if ((z == 0) && (j == 0)) {
//				cerr << (*(v_s[j]))[z].first << " " << (*(v_s[j]))[z+1].second << " " << (*(v_s[j]))[z+1].second << endl;
//			} if (z == 0) {
//				cerr << endl;
 //                               cerr << (*(v_s[j]))[z].first << " " << (*(v_s[j]))[z+1].second << " " << (*(v_s[j]))[z+1].second << endl;
  //                      } else if (z == v_s[j]->size()-1) {
   //                             cerr << (*(v_s[j]))[z].first << " " <<  (*(v_s[j]))[z].second << endl;
    //                    } else {
//				cerr << (*(v_s[j]))[z].first << " " <<  (*(v_s[j]))[z].second << endl;
//			}
 //               }
  //              delete v_s[j];
   //     }


	// Free the allocated memory
	free(root);
}
