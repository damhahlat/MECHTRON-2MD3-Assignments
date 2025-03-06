// Include necessary libraries
#include <iostream>
#include <cstdlib>
#include <string>
#include <stack>
#include <math.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

// Using namespace std to simplify code
using namespace std;

// Node structure which stores strings
typedef string Elem;
class Node {
	private:


	// All the public variables and methods
	public:

		// Attributes that store the childs, parent, and the actual data inside
		Elem elt;
		Node *parent;
		Node *left;
		Node *right;

		// Constructor for creating the node
		Node(Elem elt, Node *parent, Node *left, Node *right) {
			this->elt = elt;
			this->parent = parent;
			this->left = left;
			this->right = right;
		}

		// Deleting the node can be done by deleting the childs (parents are deleted as childs of their respective parents)
		~Node() {
			delete left;
			delete right;
		}

		// Helper functions
		Elem getElt() { return elt; }
		Node* getParent() { return parent; }
		Node* getLeft() { return left; }
		Node* getRight() { return right; }
		bool isLeaf() { return (left == NULL && right == NULL); }
		bool isRoot() { return (parent == NULL); }
		friend class LinkedBinaryTree;
};

// Class for the linked binary tree
class LinkedBinaryTree {
	private:
		int size;

	public:

		// Stores the score and the root node
		double score;
		Node *root;

		// Constructor which makes the root NULL and makes the size 0
		LinkedBinaryTree() {
			root = NULL;
			size = 0;
		}

		// Add a root based on some input to the tree
		void addRoot(Elem e) {
			root = new Node(e, NULL, NULL, NULL);
			size = 1;
		}

		// Given a node, create 2 child nodes for it.
		void expandExternal(Node *v) {

			// Make 2 nodes
			Node *left = new Node("0", v, NULL, NULL);
			Node *right = new Node("0", v, NULL, NULL);

			// Set the new nodes as the childs of the parent given as a parameter
			v->left = left;
			v->right = right;

			// Set the parents of the child nodes as the current node
			left->parent = v;
			right->parent = v;

			// Increment the size accordingly
			size += 2;
		}

		// Functions prototypes for the assignment questions
		void printExpression(); // Question 1
		double evaluateExpression(double a, double b); // Question 2
		double getScore() { return score; }
		void setScore(double s) { score = s; }

		// Overload the assignment operator and compare by score
		bool operator<(const LinkedBinaryTree &t) const {
			return score < t.score;
		}
};

// Question 1
void LinkedBinaryTree::printExpression() {

	// Get the current node
	Node *curr = this->root;

	// If the current node is not a leaf which means it's an operator and not a number
	if (!curr->isLeaf()) {

		// If the operator is abs print it out so the child nodes are sandwiched between it
		if (curr->getElt() == "abs") {
			cout << "abs(";

			// Set the root to the right child (left child is empty) and recursively call it again
			this->root = curr->getRight();
			this->printExpression();

			cout << ")";
			return;
		}

		// Print the childs between parantheses with the operator between them
		cout << "(";

		// Set the root to the childs and recursively call
		this->root = curr->getLeft();
		this->printExpression();
		cout << curr->getElt();
		this->root = curr->getRight();
		this->printExpression();
		cout << ")";

		// Move this back to root
		this->root = curr;

		return;
	}

	// Should print a number since this is a leaf node
	cout << curr->getElt();
}

// Question 2
double LinkedBinaryTree::evaluateExpression(double a, double b) {

	// Make the current node the current root
	Node *curr = this->root;

	// If the current node is not a leaf
	if (!curr->isLeaf()) {

		// If the operator is an abs perform this block
		if (curr->getElt() == "abs") {

			// Only data in the right child
			this->root = curr->getRight();

			// Get the absolute value of the output from the recursion
			double tmp = fabs(this->evaluateExpression(a, b));

			// Set the root back to the current and return the output from the recursive operation
			this->root = curr;
			return tmp;
		}

		// If the operator isn't an abs, then the general procedure is more or less the same
		// Recursively get the data on the left and right nodes
		this->root = curr->getLeft();
		double left = this->evaluateExpression(a, b);
		this->root = curr->getRight();
		double right = this->evaluateExpression(a, b);

		// Move the root back to the current node
		this->root = curr;

		// Perform specific operations depending on the operator. 
		if (curr->getElt() == "+") {
			return left + right;
		} else if (curr->getElt() == "-") {
			return left - right;
		} else if (curr->getElt() == "*") {
			return left * right;
		} else if (curr->getElt() == "/") {
			return left / right;
		} else if (curr->getElt() == ">") {

			// Should output -1 if false and 1 if true instead of the default 0 and 1
			if (left > right) {
				return 1;
			} else {
				return -1;
			}
		}
	}

	// If it's a leaf node and if the Elem is either a or b, replace it with the arguments
	if (curr->getElt() == "a") {
		return a;
	}
	if (curr->getElt() == "b") {
		return b;
	}

	// Otherwise, if it's just a normal number, convert it to a double and return it
	return stod(curr->getElt());
}

// Question 6
// Should handle + - * / abs > operators and numbers may be multiple digits
LinkedBinaryTree createExpressionTree(string postfix) {

	// Add all elements separated by space to stack
	stack<string> s;
	string temp = "";
	for (int i = 0; i < postfix.size(); i++) {

		// If it's not a space, push the data to the string
		if (postfix[i] != ' ') {
			temp += postfix[i];
		}

		// If a space or the end of the string has been reached, push the temp string to the stack and reset it
		if (postfix[i] == ' ' || i == postfix.size() - 1) {
			s.push(temp);
			temp = "";
		}
	}

	// Tree structure that we'll be filling out. Insert a tentative root into it
	LinkedBinaryTree tree;
	tree.addRoot("a");

	// Set the current node to the tree's root and iterate over the size of the stack
	Node *curr = tree.root;
	int size = s.size();
	for (int i = 0; i < size; i++) {

		// If the top element on the stack is an operator, push it onto the top of the tree and create 2 child nodes
		if (s.top() == "+" || s.top() == "-" || s.top() == "*" || s.top() == "/" || s.top() == ">" || s.top() == "abs") {
			curr->elt = s.top();
			s.pop();
			tree.expandExternal(curr);

			// Be default, all data initially is inserted into the right node (hence why abs only checks on the right side)
			curr = curr->right;
		} else {

			// If the top element on the stack is a number, insert it in the current node and move the current node to the left child
			curr->elt = s.top();
			s.pop();
			curr = curr->parent->left;
		}
	}

	// Return the tree
	return tree;
}

// Default main function. No changes.
int main() {
	// Read postfix expressions into vector
	vector<LinkedBinaryTree> trees;
	ifstream exp_file("expressions.txt");
	string line;
	while (getline(exp_file, line)) {
		trees.push_back(createExpressionTree(line));
	}
	
	// Read input data into 2D vector
	vector<vector<double>> inputs;
	ifstream input_file("input.txt");
	while (getline(input_file, line)) {
		vector<double> ab_input;
		stringstream ss(line);
		string str;
		while (getline(ss, str, ' ')) {
			ab_input.push_back(stod(str));
		}
		inputs.push_back(ab_input);
	}

	// Evaluate expressions on input data
	for (auto& t : trees) {
		double sum = 0;
		for (auto& i : inputs) {
			sum += t.evaluateExpression(i[0], i[1]);
		}
		t.setScore(sum/inputs.size());
	}
	// Sort trees by their score
	sort(trees.begin(), trees.end());
	for (auto& t : trees) {
		cout << "Exp ";
		t.printExpression();
		cout << " Score " << t.getScore() << endl;
	}

}
