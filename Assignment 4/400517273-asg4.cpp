#include <iostream>
#include <cstdlib>
#include <string>
#include <stack>

typedef std::string Elem;
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
		Node *root;
		LinkedBinaryTree() {
			root = NULL;
			size = 0;
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
};

void LinkedBinaryTree::printExpression() {
	Node *curr = this->root;
	/*std::cout << "TEST " << curr->getElt() << std::endl;*/
	if (!curr->isLeaf()) {
		/*std::cout << "SUBTREE " << curr->getElt() << std::endl;*/

		if (curr->getElt() == "abs") {
			std::cout << "abs(";
			this->root = curr->getRight();
			this->printExpression();
			std::cout << ")";
			return;
		}

		std::cout << "(";
		this->root = curr->getLeft();
		this->printExpression();
		std::cout << curr->getElt();
		this->root = curr->getRight();
		this->printExpression();
		std::cout << ")";

		return;
	}
	/*std::cout << "LEAF NODE: " << curr->getElt() << std::endl;*/

	std::cout << curr->getElt();
	/*std::cout << "PARENT NODE: " << curr->parent->elt << std::endl;*/
}

// Should handle + - * / abs > operators and numbers may be multiple digits
LinkedBinaryTree createExpressionTree(std::string postfix) {

	// Add all elements separated by space to stack
	std::stack<std::string> s;
	std::string temp = "";
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
	/*std::cout << "STACK SIZE: " << s.size() << std::endl;*/
	int size = s.size();
	for (int i = 0; i < size; i++) {
		std::cout << "CURRENT: " << s.top() << std::endl;
		if (s.top() == "+" || s.top() == "-" || s.top() == "*" || s.top() == "/" || s.top() == ">" || s.top() == "abs") {
			std::cout << "OPERATOR" << std::endl;
			curr->elt = s.top();
			s.pop();
			tree.expandExternal(curr);
			/*std::cout << "CURR: " << curr->elt << std::endl;*/
			curr = curr->right;
		} else {
			std::cout << "NUMBER" << std::endl;
			curr->elt = s.top();
			s.pop();
			curr = curr->parent->left;
		}
	}

	return tree;
}

int main() {
	LinkedBinaryTree tree = createExpressionTree("3.7 -1.2 * abs");
	tree.printExpression();
}
