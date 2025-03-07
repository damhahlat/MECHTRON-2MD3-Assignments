#include <iostream>
#include <cstdlib>
#include <string>
#include <stack>
#include <math.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// Define Elem as a string
typedef string Elem;

// Node class represents a node in the binary tree
class Node {
	public:
		Elem elt;       // Stores the element (operator, variable, or number)
		Node *parent;   // Pointer to the parent node
		Node *left;     // Pointer to the left child
		Node *right;    // Pointer to the right child

		// Constructor to initialize a node
		Node(Elem elt, Node *parent, Node *left, Node *right) 
			: elt(elt), parent(parent), left(left), right(right) {}

		// Destructor to delete child nodes
		~Node() {
			delete left;
			delete right;
		}

		// Getter methods
		Elem getElt() { return elt; }
		Node* getParent() { return parent; }
		Node* getLeft() { return left; }
		Node* getRight() { return right; }

		// Check if the node is a leaf (no children)
		bool isLeaf() { return (left == nullptr && right == nullptr); }

		// Check if the node is the root (no parent)
		bool isRoot() { return (parent == nullptr); }
};

// LinkedBinaryTree class represents a binary tree for storing and evaluating expressions
class LinkedBinaryTree {
	private:
		int size; // Tracks the number of nodes in the tree

		// Helper function to recursively copy a tree
		Node* copyTree(Node* node, Node* parent) {

			// Base case: if the node is null, return null
			if (node == nullptr) return nullptr;
			Node* newNode = new Node(node->elt, parent, nullptr, nullptr); // Create a new node
			newNode->left = copyTree(node->left, newNode);  // Recursively copy the left subtree
			newNode->right = copyTree(node->right, newNode); // Recursively copy the right subtree
			return newNode;
		}

	public:
		double score; // Stores the score of the tree (for questions 4 and 5)
		Node *root;  // Pointer to the root node of the tree

		// Constructor to initialize an empty tree
		LinkedBinaryTree() : root(nullptr), size(0), score(0) {}

		// Destructor to delete the entire tree
		~LinkedBinaryTree() {
			delete root; // Deleting the root recursively deletes all child nodes because destructor in node class
		}

		// Copy constructor to perform a deep copy of the tree
		LinkedBinaryTree(const LinkedBinaryTree& other) {
			root = copyTree(other.root, nullptr); // Copy the tree starting from the root
			size = other.size;
			score = other.score;
		}

		// Assignment operator to perform a deep copy of the tree
		LinkedBinaryTree& operator=(const LinkedBinaryTree& other) {

			// self assignment check
			if (this != &other) {
				delete root; // Delete the current tree
				root = copyTree(other.root, nullptr); // Copy the new tree
				size = other.size;
				score = other.score;
			}
			return *this;
		}

		// Add a root node to the tree
		void addRoot(Elem e) {
			if (root != nullptr) delete root; // Delete the existing root if it exists
			root = new Node(e, nullptr, nullptr, nullptr); // Create a new root node
			size = 1; // Set the size to 1
		}

		// Expand an external node by adding two child nodes
		void expandExternal(Node *v) {
			if (v->left != nullptr) delete v->left; // Delete existing left child if it exists
			if (v->right != nullptr) delete v->right; // Delete existing right child if it exists

			v->left = new Node("0", v, nullptr, nullptr); // Create a new left child
			v->right = new Node("0", v, nullptr, nullptr); // Create a new right child
			size += 2; // Increment the size by 2
		}

		// Print the expression represented by the tree (Question 1)
		void printExpression() {
			Node *curr = this->root;
			if (!curr->isLeaf()) { // If the current node is not a leaf (i.e., it's an operator)
				if (curr->getElt() == "abs") { // Handle the absolute value operator
					cout << "abs(";
					this->root = curr->getRight(); // Move to the right child
					this->printExpression(); // Recursively print the right subtree
					cout << ")";
					this->root = curr; // Restore the root
					return;
				}
				cout << "(";
				this->root = curr->getLeft(); // Move to the left child
				this->printExpression(); // Recursively print the left subtree
				cout << curr->getElt(); // Print the current operator
				this->root = curr->getRight(); // Move to the right child
				this->printExpression(); // Recursively print the right subtree
				cout << ")";
				this->root = curr; // Restore the root
				return;
			}
			cout << curr->getElt(); // If it's a leaf node, print the element (number or variable)
		}

		// Evaluate the expression represented by the tree (Question 2)
		double evaluateExpression(double a, double b) {
			Node *curr = this->root;

			// If the current node is not a leaf (i.e., it's an operator)
			if (!curr->isLeaf()) {

				// Handle the absolute value operator
				if (curr->getElt() == "abs") {
					this->root = curr->getRight(); // Move to the right child
					double tmp = fabs(this->evaluateExpression(a, b)); // Recursively evaluate and take absolute value
					this->root = curr; // Restore the root
					return tmp;
				}

				this->root = curr->getLeft(); // Move to the left child
				double left = this->evaluateExpression(a, b); // Recursively evaluate the left subtree
				this->root = curr->getRight(); // Move to the right child
				double right = this->evaluateExpression(a, b); // Recursively evaluate the right subtree
				this->root = curr; // Restore the root

				// Perform the operation based on the operator
				if (curr->getElt() == "+") {
					return left + right;
				} else if (curr->getElt() == "-") {
					return left - right;
				} else if (curr->getElt() == "*") {
					return left * right;
				} else if (curr->getElt() == "/") {
					return left / right;
				} else if (curr->getElt() == ">") {
					return (left > right) ? 1 : -1; // Return 1 if true, -1 if false
				}
			}

			// If it's a leaf node, return the value of 'a', 'b', or the number itself
			if (curr->getElt() == "a") {
				return a;
			}
			if (curr->getElt() == "b") {
				return b;
			}
			return stod(curr->getElt()); // Convert the string to a double and return
		}

		// Getter and setter for the score (Questions 3 and 4)
		double getScore() { return score; }
		void setScore(double s) { score = s; }

		// Overload the < operator to compare trees by their scores (Question 5)
		bool operator<(const LinkedBinaryTree &t) const {
			return score < t.score;
		}
};

// Function to create an expression tree from a postfix expression (Question 6)
LinkedBinaryTree createExpressionTree(string postfix) {

	stack<string> s; // Stack to store elements of the postfix expression

	// Empty string that will store the current element
	string temp = "";
	for (int i = 0; i < postfix.size(); i++) {

		// if the current character is not a space, add it to the current element
		if (postfix[i] != ' ') {
			temp += postfix[i]; // Build the current element
		}

		// If the current character is a space or the last character in the string
		// Push the current element to the stack and reset the temporary string
		if (postfix[i] == ' ' || i == postfix.size() - 1) {
			s.push(temp);
			temp = "";
		}
	}

	// Create a new tree with a tentative root
	LinkedBinaryTree tree;
	tree.addRoot("a");
	Node *curr = tree.root;
	int size = s.size();

	// Iterate through the stack to construct the tree
	for (int i = 0; i < size; i++) {

		// If the top element is an operator, set it as the current node and expand
		if (s.top() == "+" || s.top() == "-" || s.top() == "*" || s.top() == "/" || s.top() == ">" || s.top() == "abs") {
			curr->elt = s.top();
			s.pop();
			tree.expandExternal(curr); // Add two child nodes
			curr = curr->right; // Move to the right child
		} 

		// If the top element is a number, set it as the current node and move to the left child
		else {
			curr->elt = s.top();
			s.pop();
			curr = curr->parent->left;
		}
	}
	return tree; // Return the constructed tree
}

// Default main taken from the assignment
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
