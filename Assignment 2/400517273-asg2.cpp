#include <cstdlib>
#include <iostream>

class Node {
	private:
		std::string Name;
		int Score;
		Node* next;
		Node* prev;
		friend class DLinkedList;
};

class DLinkedList {  // doubly linked list
	public:
		DLinkedList();                 // constructor
		~DLinkedList();                // destructor
		
		int size(); // Recursively gets the size
		void AddScoreInOrder(std::string name, int score); // Adds scores in order
		void RemoveScore(int index);  // removes the score at the given index
		bool UpdateScore(std::string name, int score);  // updates the score of the player with the given name
		void Print(); // recursively prints the scores from best to worst
		DLinkedList(const DLinkedList& oldList); // Copy constructor
		DLinkedList &operator=(const DLinkedList& oldList); // Assignment operator

		// TESTING
		void PrintList() {
			Node *current = this->header->next;
			std::cout << "FIRST: " << this->header->next->Name << std::endl;
			while (current != this->trailer) {
				std::cout << "{" << current->Name << "," << current->Score << "}" << std::endl;
				current = current->next;
			}
		}

	private:                        // local type definitions
		Node* header;                 // list sentinels
		Node* trailer;
};

// Constructor
DLinkedList::DLinkedList() {
	header = new Node;
	trailer = new Node;
	header->next = trailer;
	trailer->prev = header;
	header->prev = NULL;
	trailer->next = NULL;
}

// Destructor
DLinkedList::~DLinkedList() {
	Node* current = header->next;
	while (current->next != NULL) {
		Node* next = current->next;
		delete current;
		current = next;
	}
	delete header;
	delete trailer;
}

// Part A: Function to recursively get size of linked list
int DLinkedList::size() {

	// Break case: If the current list is empty just return 0.
	if (this->header->next == this->trailer) return 0;

	// Make a copy of the front Node's elements
	std::string frontName = this->header->next->Name;
	int frontScore = this->header->next->Score;

	// Set the header to the next node
	this->header = this->header->next;
	int count = size()+1;

	// Reinsert the element
	Node *front = new Node;
	front->Name = frontName;
	front->Score = frontScore;
	front->next = this->header->next;
	front->prev = this->header;
	this->header->next->prev = front;
	this->header->next = front;

	// Return the count
	return count;
}


// Part B: adds a game score entry
void DLinkedList::AddScoreInOrder(std::string name, int score) {

	// Initialize the new node
	Node *newNode = new Node;
	newNode->Name = name;
	newNode->Score = score;

	// Find position for the new node by comparing scores
	Node *compare = this->header;
	while (true) {


		// If the new node is higher than the current node, add it before the current node
		if (this->header != compare && score > compare->Score) {
			compare->prev->next = newNode;
			newNode->prev = compare->prev;
			compare->prev = newNode;
			newNode->next = compare;
			break;
		}
		
		// If the new node is the lowest score add it to the end
		else if (compare->next == this->trailer) {
			newNode->next = this->trailer;
			compare->next = newNode;
			newNode->prev = compare;
			this->trailer->prev = newNode;
			break;
		}

		// Move to the next node
		compare = compare->next;
	}
}

// Part C: removes the score at the given index
void DLinkedList::RemoveScore(int index) {
	
	// Find the node at the given index
	Node *current = this->header->next;
	for (int i = 0; i < index; i++) current = current->next;

	// Remove the node
	current->prev->next = current->next;
	current->next->prev = current->prev;

	// Delete the node
	delete current;
}

// Part D: updates the score of the player with the given name
bool DLinkedList::UpdateScore(std::string name, int score) {

	// Find the node with the player's name
	Node *current = this->header->next;
	while (current != this->trailer) {

		// If the name is found, update the score
		if (current->Name == name) {
			current->Score = score;
			return true;
		}

		// Move to the next node
		current = current->next;
	}

	// If the name is not found, return false
	return false;
}

// Part E: recursively prints the scores from best to worst
void DLinkedList::Print() {

	// Break case: If the current list is empty just return
	if (this->header->next == this->trailer) return;

	// Make a copy of the front Node's elements
	std::string frontName = this->header->next->Name;
	int frontScore = this->header->next->Score;

	// Set the header to the next node
	this->header = this->header->next;

	// Print the current node
	std::cout << "{" << this->header->Name << "," << this->header->Score << "}" << std::endl;

	// Recursively print the rest of the list
	Print();

	// Reinsert the element
	Node *front = new Node;
	front->Name = frontName;
	front->Score = frontScore;
	front->next = this->header->next;
	front->prev = this->header;
	this->header->next->prev = front;
	this->header->next = front;
}

// Part F: Copy constructor
DLinkedList::DLinkedList(const DLinkedList& oldList) {

	// Initialize the new list with defaults
	header = new Node;
	trailer = new Node;
	header->next = trailer;
	trailer->prev = header;
	header->prev = NULL;
	trailer->next = NULL;

	// Set a pointer node to the first value of the new list that we'll insert from
	Node *currentNew = this->header;

	// Set current node and iterate through each version
	Node *currentOld = oldList.header->next;
	while (currentOld != oldList.trailer) {

		// Create a new node with the same values as the current node and insert into new list
		Node *newNode = new Node;
		newNode->Name = currentOld->Name;
		newNode->Score = currentOld->Score;

		// Insert the new node from the currentNew position
		newNode->next = this->trailer;
		newNode->prev = currentNew;
		this->trailer->prev = newNode;
		currentNew->next = newNode;

		// Move both tracker nodes forward
		currentOld = currentOld->next;
		currentNew = newNode;

	}
}

// Part G: Assignment operator
DLinkedList &DLinkedList::operator=(const DLinkedList& oldList) {


	// Check for self assignment
	if (this == &oldList) return *this;

	// Initialize the new list with defaults
	header = new Node;
	trailer = new Node;
	header->next = trailer;
	trailer->prev = header;
	header->prev = NULL;
	trailer->next = NULL;

	// Set a pointer node to the first value of the new list that we'll insert from
	Node *currentNew = this->header;

	// Set current node and iterate through each version
	Node *currentOld = oldList.header->next;
	while (currentOld != oldList.trailer) {

		// Create a new node with the same values as the current node and insert into new list
		Node *newNode = new Node;
		newNode->Name = currentOld->Name;
		newNode->Score = currentOld->Score;

		// Insert the new node from the currentNew position
		newNode->next = this->trailer;
		newNode->prev = currentNew;
		this->trailer->prev = newNode;
		currentNew->next = newNode;

		// Move both tracker nodes forward
		currentOld = currentOld->next;
		currentNew = newNode;
	}

	// Empty the old list but keep the header and trailer because the destructor will delete them
	Node* current = oldList.header->next;
	while (current != oldList.trailer) {
		Node* next = current->next;
		delete current;
		current = next;
	}
	oldList.header->next = oldList.trailer;
	oldList.trailer->prev = oldList.header;

	// Return the new list
	return *this;
}

int main() {
	DLinkedList dll;
	dll.AddScoreInOrder("Alice", 100);
	dll.AddScoreInOrder("Charlie", 600);
	dll.AddScoreInOrder("Bob", 200);
	dll.AddScoreInOrder("David", 400);
	dll.AddScoreInOrder("Eve", 50);

	// Test assignment operator
	DLinkedList dll2;
	dll2 = dll;
	dll2.Print();
	std::cout << "DLL2" << std::endl;
	dll.Print();
	std::cout << "DLL" << std::endl;
}
