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

typedef string Elem;
class Node {
	private:

	public:
		Elem elt;
		Node *parent;
		Node *left;
		Node *right;

		Node(Elem elt, Node *parent, Node *left, Node *right) {
			this->elt = elt;
			this->parent = parent;
			this->left = left;
			this->right = right;
		}

		~Node() {
			delete left;
			delete right;
		}

		Elem getElt() { return elt; }
		Node* getParent() { return parent; }
		Node* getLeft() { return left; }
		Node* getRight() { return right; }
		bool isLeaf() { return (left == NULL && right == NULL); }
		bool isRoot() { return (parent == NULL); }
		friend class LinkedBinaryTree;
};

class LinkedBinaryTree {
	private:
		int size;

	public:
		double score;
		Node *root;
		LinkedBinaryTree() {
			root = NULL;
			size = 0;
		}

		~LinkedBinaryTree() {
			delete root;
		}

		int getSize() { return size; }
		bool empty() { return size == 0; }
		Node* getRoot() { return root; }
		void addRoot(Elem e) {
			root = new Node(e, NULL, NULL, NULL);
			size = 1;
		}

		void expandExternal(Node *v) {
			Node *left = new Node("0", v, NULL, NULL);
			Node *right = new Node("0", v, NULL, NULL);
			v->left = left;
			v->right = right;
			left->parent = v;
			right->parent = v;
			size += 2;
		}

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
	Node *curr = this->root;
	if (!curr->isLeaf()) {

		if (curr->getElt() == "abs") {
			cout << "abs(";
			this->root = curr->getRight();
			this->printExpression();
			cout << ")";
			return;
		}

		cout << "(";
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

	cout << curr->getElt();

}

// Question 2
double LinkedBinaryTree::evaluateExpression(double a, double b) {
	Node *curr = this->root;
	if (!curr->isLeaf()) {
		if (curr->getElt() == "abs") {
			this->root = curr->getRight();
			double tmp = fabs(this->evaluateExpression(a, b));
			this->root = curr;
			return tmp;
		}

		this->root = curr->getLeft();
		double left = this->evaluateExpression(a, b);
		this->root = curr->getRight();
		double right = this->evaluateExpression(a, b);

		this->root = curr;
		if (curr->getElt() == "+") {
			return left + right;
		} else if (curr->getElt() == "-") {
			return left - right;
		} else if (curr->getElt() == "*") {
			return left * right;
		} else if (curr->getElt() == "/") {
			return left / right;
		} else if (curr->getElt() == ">") {
			if (left > right) {
				return 1;
			} else {
				return -1;
			}
		}
	}

	if (curr->getElt() == "a") {
		return a;
	}
	if (curr->getElt() == "b") {
		return b;
	}

	return stod(curr->getElt());
}

// Question 6
// Should handle + - * / abs > operators and numbers may be multiple digits
LinkedBinaryTree createExpressionTree(string postfix) {

	// Add all elements separated by space to stack
	stack<string> s;
	string temp = "";
	for (int i = 0; i < postfix.size(); i++) {

		if (postfix[i] != ' ') {
			temp += postfix[i];
		}

		if (postfix[i] == ' ' || i == postfix.size() - 1) {
			s.push(temp);
			temp = "";
		}
	}

	LinkedBinaryTree tree;
	tree.addRoot("a");
	Node *curr = tree.root;
	int size = s.size();
	for (int i = 0; i < size; i++) {
		if (s.top() == "+" || s.top() == "-" || s.top() == "*" || s.top() == "/" || s.top() == ">" || s.top() == "abs") {
			curr->elt = s.top();
			s.pop();
			tree.expandExternal(curr);
			curr = curr->right;
		} else {
			curr->elt = s.top();
			s.pop();
			curr = curr->parent->left;
		}
	}

	return tree;
}

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
