#include <iostream>
#include <vector>

using namespace std;


// A class for a nood.
class Node {
	public:
	Node(int val, int child);
	~Node();
	void printvalueandchild();
	int getnchild();
	Node * getidxchild(int idx);
	void pushchild(Node *next);
	
	private:
	int value;
	int child;
	vector<Node *> children;
};

// Constructor of the Node
Node::Node(int val ,int child) {
	this->value = val;
	this->child = child;
}

// Destructor of the Node
Node::~Node() {
	for (int a = 0; a < children.size(); a++) {
		delete this->children[a];
	}
}

// Function to push a Node to a vector
void Node::pushchild(Node *next) {
	children.push_back(next);
}

// Get the pointer to the child indicated in idx
Node *Node::getidxchild(int idx) {
	return children[idx];
}

// Get the number of child of the node
int Node::getnchild() {
	return child;
}

// Print out the value of teh node and its number of childs
void Node::printvalueandchild() {
	cout << value << ' ' << child << endl;
	return;
}

// A function that construct a tree by reading a pre-order traversal of a non-empty tree from standard input
// Void -> Node*
Node *maketree() {
	// declare the variable for reading input
	int vf;
        int cf;
        // read the input
        cin >> vf;
	cin >> cf;
	// construct the tree
	Node *p = new Node(vf, cf);
	for (int j = 0; j < cf; j++) {
		Node *p2 = maketree();
		p->pushchild(p2);
	}
	return p;
}

// Print the value and the number of child of the current node
// Node* -> Void
void print(Node *n) {
	if (n->getnchild() == 0) {
		n->printvalueandchild();
		//delete n;
		return;
	} else {
		for (int k = 0; k < n->getnchild(); k++) {
			Node *n2 = n->getidxchild(k);
			print(n2);
		}
		n->printvalueandchild();
		//delete n;
		return;
	}
}

int main() {
	// construct the tree
	Node *root = maketree();

	// read and print out the tree in post-order traversal
	print(root);
	return 1;
}


