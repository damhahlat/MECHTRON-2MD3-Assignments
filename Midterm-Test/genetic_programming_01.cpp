#include <math.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <stack>
#include <vector>

#include "cartCentering.h"

using namespace std;


// return a double unifomrly sampled in (0,1)
double randDouble(mt19937& rng) {
  return std::uniform_real_distribution<>{0, 1}(rng);
}
// return uniformly sampled 0 or 1
bool randChoice(mt19937& rng) {
  return std::uniform_int_distribution<>{0, 1}(rng);
}
// return a random integer uniformly sampled in (min, max)
int randInt(mt19937& rng, const int& min, const int& max) {
  return std::uniform_int_distribution<>{min, max}(rng);
}

// return true if op is a suported operation, otherwise return false
bool isOp(string op) {
  if (op == "+")
    return true;
  else if (op == "-")
    return true;
  else if (op == "*")
    return true;
  else if (op == "/")
    return true;
  else if (op == ">")
    return true;
  else if (op == "abs")
    return true;
  else
    return false;
}

int arity(string op) {
  if (op == "abs")
    return 1;
  else
    return 2;
}

typedef string Elem;

class LinkedBinaryTree {
 public:
  struct Node {
    Elem elt;
    string name;
    Node* par;
    Node* left;
    Node* right;
    Node() : elt(), par(NULL), name(""), left(NULL), right(NULL) {}
    int depth() {
      if (par == NULL) return 0;
      return par->depth() + 1;
    }
  };

  class Position {
   private:

   public:
    Node* v;
    Position(Node* _v = NULL) : v(_v) {}
    Elem& operator*() { return v->elt; }
    Position left() const { return Position(v->left); }
    void setLeft(Node* n) { v->left = n; }
    Position right() const { return Position(v->right); }
    void setRight(Node* n) { v->right = n; }
    Position parent() const  // get parent
    {
      return Position(v->par);
    }
    bool isRoot() const  // root of the tree?
    {
      return v->par == NULL;
    }
    bool isExternal() const  // an external node?
    {
      return v->left == NULL && v->right == NULL;
    }
    friend class LinkedBinaryTree;  // give tree access
  };
  typedef vector<Position> PositionList;

 public:
  LinkedBinaryTree() : _root(NULL), score(0), steps(0), generation(0) {}

  // copy constructor
  LinkedBinaryTree(const LinkedBinaryTree& t) {
    _root = copyPreOrder(t.root());
    score = t.getScore();
    steps = t.getSteps();
    generation = t.getGeneration();
  }

  // copy assignment operator
  LinkedBinaryTree& operator=(const LinkedBinaryTree& t) {
    if (this != &t) {
      // if tree already contains data, delete it
      if (_root != NULL) {
        PositionList pl = positions();
        for (auto& p : pl) delete p.v;
      }
      _root = copyPreOrder(t.root());
      score = t.getScore();
      steps = t.getSteps();
      generation = t.getGeneration();
    }
    return *this;
  }

  // destructor
  ~LinkedBinaryTree() {
    if (_root != NULL) {
      PositionList pl = positions();
      for (auto& p : pl) delete p.v;
    }
  }

  int size() const { return size(_root); }
  int size(Node* root) const;
  int depth() const;
  bool empty() const { return size() == 0; };
  Node* root() const { return _root; }
  PositionList positions() const;
  void addRoot() { _root = new Node; }
  void addRoot(Elem e) {
    _root = new Node;
    _root->elt = e;
  }
  void nameRoot(string name) { _root->name = name; }
  void addLeftChild(const Position& p, const Node* n);
  void addLeftChild(const Position& p);
  void addRightChild(const Position& p, const Node* n);
  void addRightChild(const Position& p);
  void printExpression() { printExpression(_root); }
  void printExpression(Node* v);
  double evaluateExpression(double a, double b) {
    return evaluateExpression(Position(_root), a, b);
  };
  double evaluateExpression(const Position& p, double a, double b);
  long getGeneration() const { return generation; }
  void setGeneration(int g) { generation = g; }
  double getScore() const { return score; }
  void setScore(double s) { score = s; }
  double getSteps() const { return steps; }
  void setSteps(double s) { steps = s; }
  void randomExpressionTree(Node* p, const int& maxDepth, mt19937& rng);
  void randomExpressionTree(const int& maxDepth, mt19937& rng) {
    randomExpressionTree(_root, maxDepth, rng);
  }
  void deleteSubtreeMutator(mt19937& rng);
  void addSubtreeMutator(mt19937& rng, const int maxDepth);

  Node* _root;  // pointer to the root
 protected:                                        // local utilities
  void preorder(Node* v, PositionList& pl) const;  // preorder utility
  Node* copyPreOrder(const Node* root);
  double score;     // mean reward over 20 episodes
  double steps;     // mean steps-per-episode over 20 episodes
  long generation;  // which generation was tree "born"
 private:
};

// add the tree rooted at node child as this tree's left child
void LinkedBinaryTree::addLeftChild(const Position& p, const Node* child) {
  Node* v = p.v;
  v->left = copyPreOrder(child);  // deep copy child
  v->left->par = v;
}

// add the tree rooted at node child as this tree's right child
void LinkedBinaryTree::addRightChild(const Position& p, const Node* child) {
  Node* v = p.v;
  v->right = copyPreOrder(child);  // deep copy child
  v->right->par = v;
}

void LinkedBinaryTree::addLeftChild(const Position& p) {
  Node* v = p.v;
  v->left = new Node;
  v->left->par = v;
}

void LinkedBinaryTree::addRightChild(const Position& p) {
  Node* v = p.v;
  v->right = new Node;
  v->right->par = v;
}

// return a list of all nodes
LinkedBinaryTree::PositionList LinkedBinaryTree::positions() const {
  PositionList pl;
  preorder(_root, pl);
  return PositionList(pl);
}

void LinkedBinaryTree::preorder(Node* v, PositionList& pl) const {
  pl.push_back(Position(v));
  if (v->left != NULL) preorder(v->left, pl);
  if (v->right != NULL) preorder(v->right, pl);
}

int LinkedBinaryTree::size(Node* v) const {
  int lsize = 0;
  int rsize = 0;
  if (v->left != NULL) lsize = size(v->left);
  if (v->right != NULL) rsize = size(v->right);
  return 1 + lsize + rsize;
}

int LinkedBinaryTree::depth() const {
  PositionList pl = positions();
  int depth = 0;
  for (auto& p : pl) depth = std::max(depth, p.v->depth());
  return depth;
}

LinkedBinaryTree::Node* LinkedBinaryTree::copyPreOrder(const Node* root) {
  if (root == NULL) return NULL;
  Node* nn = new Node;
  nn->elt = root->elt;
  nn->left = copyPreOrder(root->left);
  if (nn->left != NULL) nn->left->par = nn;
  nn->right = copyPreOrder(root->right);
  if (nn->right != NULL) nn->right->par = nn;
  return nn;
}

// Part 1: Question 1
void LinkedBinaryTree::printExpression(Node* v) {
	if (v == nullptr) return;

	// If the current node is a leaf, print the element
	if (v->left == nullptr && v->right == nullptr) {
		cout << v->elt;
		return;
	}

	// If the current node is an operator, print the left child, the operator, and the right child
	if (v->elt == "abs") {
		cout << "abs(";
		printExpression(v->left);
		cout << ")";
	} else {
		cout << "(";
		printExpression(v->left);
		cout << " " << v->elt << " ";
		printExpression(v->right);
		cout << ")";
	}
}

double evalOp(string op, double x, double y = 0) {
  double result;
  if (op == "+")
    result = x + y;
  else if (op == "-")
    result = x - y;
  else if (op == "*")
    result = x * y;
  else if (op == "/") {
    result = x / y;
  } else if (op == ">") {
    result = x > y ? 1 : -1;
  } else if (op == "abs") {
    result = abs(x);
  } else
    result = 0;
  return isnan(result) || !isfinite(result) ? 0 : result;
}

double LinkedBinaryTree::evaluateExpression(const Position& p, double a,
                                            double b) {
  if (!p.isExternal()) {
    auto x = evaluateExpression(p.left(), a, b);
    if (arity(p.v->elt) > 1) {
      auto y = evaluateExpression(p.right(), a, b);
      return evalOp(p.v->elt, x, y);
    } else {
      return evalOp(p.v->elt, x);
    }
  } else {
    if (p.v->elt == "a")
      return a;
    else if (p.v->elt == "b")
      return b;
    else
      return stod(p.v->elt);
  }
}

// Part 1: Question 3
void LinkedBinaryTree::deleteSubtreeMutator(mt19937& rng) {
    if (empty()) return; // Exit early if the tree is empty

    // Collect all nodes in the tree and ensure that there isn't just 1 operator and 2 leafs in the tree
    PositionList nodeList = positions();
	if (nodeList.size() <= 3) return;

    // Ensure that the node to delete is an operator and isn't the root
    Node* targetNode = nullptr;
    do {
        int randomIndex = randInt(rng, 0, nodeList.size() - 1);
        targetNode = nodeList[randomIndex].v;
    } while (!isOp(targetNode->elt) || targetNode == _root);

    // Get the parent of the selected node
    Node* parentNode = targetNode->par;

    // Determine if the target node is a left or right child
    bool isLeftChild = (parentNode->left == targetNode);

    // Unlink the target node from its parent
    if (isLeftChild) {
        parentNode->left = nullptr;
    } else {
        parentNode->right = nullptr;
    }

    // Use a stack-like thingy to iteratively delete the subtree rooted at the target node
    Node* deletionStack[1000]; // Stack to simulate recursive deletion
    int stackTop = -1;
    deletionStack[++stackTop] = targetNode;

    while (stackTop >= 0) {
        Node* currentNode = deletionStack[stackTop--];
        if (currentNode->left) deletionStack[++stackTop] = currentNode->left;
        if (currentNode->right) deletionStack[++stackTop] = currentNode->right;
        delete currentNode; // Free memory for the current node
    }

    // Handle orphaned operators after deletion
    if (isOp(parentNode->elt)) {
        vector<string> terminals = {"a", "b"}; // List of terminals

        if (arity(parentNode->elt) == 2) {
            // Binary operator: Replace missing child with a random terminal
            if (isLeftChild && parentNode->right != nullptr) {
                // Left child was deleted, replace it with a random terminal
                Node* newTerminal = new Node;
                newTerminal->elt = terminals[randInt(rng, 0, terminals.size() - 1)];
                newTerminal->par = parentNode;
                parentNode->left = newTerminal;
            } else if (!isLeftChild && parentNode->left != nullptr) {
                // Right child was deleted, replace it with a random terminal
                Node* newTerminal = new Node;
                newTerminal->elt = terminals[randInt(rng, 0, terminals.size() - 1)];
                newTerminal->par = parentNode;
                parentNode->right = newTerminal;
            } else {
                // Both children were deleted, replace both with random terminals
                Node* leftTerminal = new Node;
                leftTerminal->elt = terminals[randInt(rng, 0, terminals.size() - 1)];
                leftTerminal->par = parentNode;
                parentNode->left = leftTerminal;

                Node* rightTerminal = new Node;
                rightTerminal->elt = terminals[randInt(rng, 0, terminals.size() - 1)];
                rightTerminal->par = parentNode;
                parentNode->right = rightTerminal;
            }
        } else if (arity(parentNode->elt) == 1 && parentNode->left == nullptr) {
            // Unary operator with no child: Replace with a random terminal
            Node* newTerminal = new Node;
            newTerminal->elt = terminals[randInt(rng, 0, terminals.size() - 1)];
            newTerminal->par = parentNode;
            parentNode->left = newTerminal;
        }
    }
}

// Part 1: Question 4
void LinkedBinaryTree::addSubtreeMutator(mt19937& rng, const int maxDepth) {
    if (empty()) return; // Exit early if the tree is empty

    // Collect all nodes in the tree
    PositionList pl = positions();

    // Filter out only leaf nodes (nodes with no children)
    vector<Node*> leafNodes;
    for (auto& pos : pl) {
        Node* node = pos.v;
        if (node->left == nullptr && node->right == nullptr) { // Leaf node
            leafNodes.push_back(node);
        }
    }

    if (leafNodes.empty()) return; // If there are no leaf nodes, do nothing

    // Randomly select a leaf node to replace
    Node* leafToReplace = leafNodes[randInt(rng, 0, leafNodes.size() - 1)];

    // Define operators and terminals
    static vector<string> operators = {"+", "-", "*", "/", ">", "abs"};
    static vector<string> terminals = {"a", "b"};

    // Stack to iteratively build the subtree
    struct StackNode {
        Node* node;
        int depth;
    };

    // Initialize the stack with the root of the new subtree
    Node* subtreeRoot = new Node;
    subtreeRoot->elt = randChoice(rng) ? operators[randInt(rng, 0, operators.size() - 1)] : terminals[randInt(rng, 0, terminals.size() - 1)]; // Randomly choose an operator or terminal
    subtreeRoot->par = leafToReplace->par;

    // Replace the leaf node with the new subtree
    Node* parentNode = leafToReplace->par;
    if (parentNode == nullptr) {
        // If the leaf node is the root, replace the entire tree
        _root = subtreeRoot;
    } else {
        // Determine if the leaf node is the left or right child of its parent
        bool isLeftChild = (parentNode->left == leafToReplace);

        // Replace the leaf node with the new subtree
        delete leafToReplace; // Properly delete the existing leaf node
        if (isLeftChild) {
            parentNode->left = subtreeRoot;
        } else {
            parentNode->right = subtreeRoot;
        }
    }

    // Use a stack to iteratively build the subtree
    StackNode stack[1000]; // Stack to behave like recursion
    int stackTop = -1;

    // Push the root of the new subtree onto the stack
    stack[++stackTop] = {subtreeRoot, 1};

    while (stackTop >= 0) {
        StackNode current = stack[stackTop--];
        Node* currentNode = current.node;
        int currentDepth = current.depth;

        // If the current node is an operator and we haven't reached max depth
        if (isOp(currentNode->elt) && currentDepth < maxDepth) {
            // Generate left child
            Node* leftChild = new Node;
            leftChild->elt = randChoice(rng) || currentDepth == maxDepth - 1
                             ? terminals[randInt(rng, 0, terminals.size() - 1)]
                             : operators[randInt(rng, 0, operators.size() - 1)];
            leftChild->par = currentNode;
            currentNode->left = leftChild;

            // Generate right child if the operator is binary
            if (arity(currentNode->elt) > 1) {
                Node* rightChild = new Node;
                rightChild->elt = randChoice(rng) || currentDepth == maxDepth - 1
                                  ? terminals[randInt(rng, 0, terminals.size() - 1)]
                                  : operators[randInt(rng, 0, operators.size() - 1)];
                rightChild->par = currentNode;
                currentNode->right = rightChild;

                // Push right child onto the stack
                stack[++stackTop] = {rightChild, currentDepth + 1};
            }

            // Push left child onto the stack
            stack[++stackTop] = {leftChild, currentDepth + 1};
        }
    }
}

bool operator<(const LinkedBinaryTree& x, const LinkedBinaryTree& y) {
  return x.getScore() < y.getScore();
}

LinkedBinaryTree createExpressionTree(string postfix) {
  stack<LinkedBinaryTree> tree_stack;
  stringstream ss(postfix);
  // Split each line into words
  string token;
  while (getline(ss, token, ' ')) {
    LinkedBinaryTree t;
    if (!isOp(token)) {
      t.addRoot(token);
      tree_stack.push(t);
    } else {
      t.addRoot(token);
      if (arity(token) > 1) {
        LinkedBinaryTree r = tree_stack.top();
        tree_stack.pop();
        t.addRightChild(t.root(), r.root());
      }
      LinkedBinaryTree l = tree_stack.top();
      tree_stack.pop();
      t.addLeftChild(t.root(), l.root());
      tree_stack.push(t);
    }
  }
  return tree_stack.top();
}

// Part 1: Question 2
LinkedBinaryTree createRandExpressionTree(int max_depth, mt19937& rng) {

	// Store the operators and terminals
	static vector<string> operators = {"+", "-", "*", "/", ">", "abs"};
	static vector<string> terminals = {"a", "b"};

	LinkedBinaryTree t;

	// Base case: if max_depth is 0, add a random terminal
	if (max_depth <= 0) {
		t.addRoot(terminals[randInt(rng, 0, terminals.size() - 1)]);
		return t;
	}

	// Randomly choose between a terminal or an operator. If depth is 1, choose a terminal
	if (max_depth == 1) {
		t.addRoot(terminals[randInt(rng, 0, terminals.size() - 1)]);
	} else {
		string op = operators[randInt(rng, 0, operators.size() - 1)];
		t.addRoot(op);

		// If the operator is "abs", add only a left child
		if (op == "abs") {
			t.addLeftChild(t.root(), createRandExpressionTree(max_depth - 1, rng).root());
		} else {
			// For binary operators, add both left and right children
			t.addLeftChild(t.root(), createRandExpressionTree(max_depth - 1, rng).root());
			t.addRightChild(t.root(), createRandExpressionTree(max_depth - 1, rng).root());
		}
	}
	return t;
}

// evaluate tree t in the cart centering task
void evaluate(mt19937& rng, LinkedBinaryTree& t, const int& num_episode,
              bool animate, bool partially_observable = false) {
  cartCentering env;
  double mean_score = 0.0;
  double mean_steps = 0.0;
  for (int i = 0; i < num_episode; i++) {
    double episode_score = 0.0;
    int episode_steps = 0;
    env.reset(rng);
    while (!env.terminal()) {
      int action;
      if (partially_observable) {
        action = t.evaluateExpression(env.getCartXPos(), 0.0);
      } else {
        action = t.evaluateExpression(env.getCartXPos(), env.getCartXVel());
      }
      episode_score += env.update(action, animate);
      episode_steps++;
    }
    mean_score += episode_score;
    mean_steps += episode_steps;
  }
  t.setScore(mean_score / num_episode);
  t.setSteps(mean_steps / num_episode);
}

// Part 2: Question 1
struct LexLessThan {
    bool operator()(const LinkedBinaryTree& x, const LinkedBinaryTree& y) const {
        double scoreDiff = fabs(x.getScore() - y.getScore());
        if (scoreDiff < 0.01) {
            // Favor simpler trees (fewer nodes) when scores are similar
            return x.size() > y.size();
        } else {
            // Otherwise, compare by score
            return x.getScore() < y.getScore();
        }
    }
};

// Part 3: Question 1
// Perform crossover by swapping subtrees between two parent trees
std::pair<LinkedBinaryTree, LinkedBinaryTree> crossover(mt19937& rng, const LinkedBinaryTree& parent1, const LinkedBinaryTree& parent2) {
    // Create deep copies of the parent trees
    LinkedBinaryTree child1(parent1); // Copy constructor creates a deep copy
    LinkedBinaryTree child2(parent2);

    // Collect all nodes in both child trees
    LinkedBinaryTree::PositionList pl1 = child1.positions();
    LinkedBinaryTree::PositionList pl2 = child2.positions();

    // Exit early if either tree only has a root or only 2 leaf nodes
    if (pl1.size() <= 3 || pl2.size() <= 3) {
        return {child1, child2}; // Return unmodified copies if crossover is not feasible
    }

    // Randomly select a subtree from each child
    int randomIndex1 = randInt(rng, 1, pl1.size() - 1);
    int randomIndex2 = randInt(rng, 1, pl2.size() - 1);

    LinkedBinaryTree::Node* subtree1 = pl1[randomIndex1].v;
    LinkedBinaryTree::Node* subtree2 = pl2[randomIndex2].v;

    // Find the parent nodes of the selected subtrees
    LinkedBinaryTree::Node* parentOfSubtree1 = subtree1->par;
    LinkedBinaryTree::Node* parentOfSubtree2 = subtree2->par;

    // Determine if the subtrees are left or right children of their parents
    bool isSubtree1LeftChild = (parentOfSubtree1 && parentOfSubtree1->left == subtree1);
    bool isSubtree2LeftChild = (parentOfSubtree2 && parentOfSubtree2->left == subtree2);

    // Swap the subtrees
    if (parentOfSubtree1) {
        if (isSubtree1LeftChild) {
            parentOfSubtree1->left = subtree2;
        } else {
            parentOfSubtree1->right = subtree2;
        }
    } else {
        // If subtree1 has no parent, it must be the root of child1
        child1._root = subtree2; // Update the root pointer
    }

    if (parentOfSubtree2) {
        if (isSubtree2LeftChild) {
            parentOfSubtree2->left = subtree1;
        } else {
            parentOfSubtree2->right = subtree1;
        }
    } else {
        // If subtree2 has no parent, it must be the root of child2
        child2._root = subtree1; // Update the root pointer
    }

    // Update the parent pointers of the swapped subtrees
    subtree1->par = parentOfSubtree2;
    subtree2->par = parentOfSubtree1;

    // Return the modified child trees
    return {child1, child2};
}

int main() {
	mt19937 rng(42);
	// Experiment parameters
	const int NUM_TREE = 50;
	const int MAX_DEPTH_INITIAL = 2;
	const int MAX_DEPTH = 20;
	const int NUM_EPISODE = 20;
	const int MAX_GENERATIONS = 10;
	const bool PARTIALLY_OBSERVABLE = false;

	// Create an initial "population" of expression trees
	vector<LinkedBinaryTree> trees;
	for (int i = 0; i < NUM_TREE; i++) {
		LinkedBinaryTree t = createRandExpressionTree(MAX_DEPTH_INITIAL, rng);
		trees.push_back(t);
	}

	// Genetic Algorithm loop
	LinkedBinaryTree best_tree;
	std::cout << "generation,fitness,steps,size,depth" << std::endl;
	for (int g = 1; g <= MAX_GENERATIONS; g++) {

		// Fitness evaluation
		for (auto& t : trees) {
			if (t.getGeneration() < g - 1) continue;  // skip if not new
			evaluate(rng, t, NUM_EPISODE, false, PARTIALLY_OBSERVABLE);
		}

		// sort trees using overloaded "<" op (worst->best)
		/*std::sort(trees.begin(), trees.end());*/

		// // sort trees using comparaor class (worst->best)
		std::sort(trees.begin(), trees.end(), LexLessThan());

		// erase worst 50% of trees (first half of vector)
		trees.erase(trees.begin(), trees.begin() + NUM_TREE / 2);

		// Print stats for best tree
		best_tree = trees[trees.size() - 1];
		std::cout << g << ",";
		std::cout << best_tree.getScore() << ",";
		std::cout << best_tree.getSteps() << ",";
		std::cout << best_tree.size() << ",";
		std::cout << best_tree.depth() << std::endl;

		// Selection and mutation
		while (trees.size() < NUM_TREE) {
			// Selected random "parent" tree from survivors
			LinkedBinaryTree parent = trees[randInt(rng, 0, (NUM_TREE / 2) - 1)];

			// Create child tree with copy constructor
			LinkedBinaryTree child(parent);
			child.setGeneration(g);

			// Mutation
			// Delete a randomly selected part of the child's tree
			child.deleteSubtreeMutator(rng);
			// Add a random subtree to the child
			child.addSubtreeMutator(rng, MAX_DEPTH);

			trees.push_back(child);
		}

		// Selection, crossover, and mutation
		/*while (trees.size() < NUM_TREE) {*/
		/*	// Select two random parents*/
		/*	int index1 = randInt(rng, 0, (NUM_TREE / 2) - 1);*/
		/*	int index2 = randInt(rng, 0, (NUM_TREE / 2) - 1);*/
		/**/
		/*	auto [child1, child2] = crossover(rng, trees[index1], trees[index2]);*/
		/*	child1.setGeneration(g);*/
		/*	child2.setGeneration(g);*/
		/**/
		/*	// Mutation*/
		/*	child1.addSubtreeMutator(rng, MAX_DEPTH);*/
		/*	child2.addSubtreeMutator(rng, MAX_DEPTH);*/
		/*	child1.deleteSubtreeMutator(rng);*/
		/*	child2.deleteSubtreeMutator(rng);*/
		/**/
		/*	trees.push_back(child1);*/
		/*	trees.push_back(child2);*/
		/*}*/
	}

	// // Evaluate best tree with animation
	/*const int num_episode = 3;*/
	/*evaluate(rng, best_tree, num_episode, true, PARTIALLY_OBSERVABLE);*/

	// Print best tree info
	std::cout << std::endl << "Best tree:" << std::endl;
	best_tree.printExpression();
	std::cout << endl;
	std::cout << "Generation: " << best_tree.getGeneration() << endl;
	std::cout << "Size: " << best_tree.size() << std::endl;
	std::cout << "Depth: " << best_tree.depth() << std::endl;
	std::cout << "Fitness: " << best_tree.getScore() << std::endl << std::endl;

	/*vector<LinkedBinaryTree> trees;*/
	/*mt19937 rng(91);*/
	/*LinkedBinaryTree t;*/
	/*t = createRandExpressionTree(5, rng);*/
	/*t.printExpression();*/
	/*cout << endl;*/
	/*t.deleteSubtreeMutator(rng);*/
	/*t.printExpression();*/
	/*cout << endl;*/
	/*t.addSubtreeMutator(rng, 5);*/
	/*t.printExpression();*/
	/*cout << endl;*/
	/*t.deleteSubtreeMutator(rng);*/
	/*t.printExpression();*/
	/*cout << endl;*/
	/*t.addSubtreeMutator(rng, 5);*/
	/*t.printExpression();*/
	/*cout << endl;*/
	/*trees.push_back(t);*/
	/*LinkedBinaryTree copy(t);*/
	/*copy.printExpression();*/
	/*cout << endl;*/
	/*copy.addSubtreeMutator(rng, 5);*/
	/*copy.printExpression();*/
	/*cout << endl;*/
	/*copy.deleteSubtreeMutator(rng);*/
	/*copy.printExpression();*/
	/*cout << endl;*/
	/*trees.push_back(copy);*/
}
