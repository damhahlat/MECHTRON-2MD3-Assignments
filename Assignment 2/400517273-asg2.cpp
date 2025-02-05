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
		void OrderByNames(); // Orders the list by names
		void OrderByScores(); // Orders the list by scores
		void Append(DLinkedList& L); // Appends the list L to the end of the current list
		void Reverse(); // Reverses the list

	// List sentients
	private:
		Node* header;
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

// Part H: Orders the list by names alphabetically
void DLinkedList::OrderByNames() {

	// Use bubble sort to order the list by names
	Node *current = this->header->next;

	// Iterate through the list
	while (current != this->trailer) {

		// Compare the current node to the next node
		Node *compare = current->next;
		while (compare != this->trailer) {

			// If the current node is greater than the next node, swap their values
			if (current->Name > compare->Name) {
				std::string tempName = current->Name;
				int tempScore = current->Score;
				current->Name = compare->Name;
				current->Score = compare->Score;
				compare->Name = tempName;
				compare->Score = tempScore;
			}

			// Move to the next node
			compare = compare->next;
		}

		// Move to the next node
		current = current->next;
	}
}

// Part I: Orders the list by scores
void DLinkedList::OrderByScores() {

	// Use bubble sort to order the list by scores
	Node *current = this->header->next;

	// Iterate through the list
	while (current != this->trailer) {

		// Compare the current node to the next node
		Node *compare = current->next;
		while (compare != this->trailer) {

			// If the current node is less than the next node, swap their values
			if (current->Score < compare->Score) {
				std::string tempName = current->Name;
				int tempScore = current->Score;
				current->Name = compare->Name;
				current->Score = compare->Score;
				compare->Name = tempName;
				compare->Score = tempScore;
			}

			// Move to the next node
			compare = compare->next;
		}

		// Move to the next node
		current = current->next;
	}
}

// Part J: Appends the list L to the end of the current list
void DLinkedList::Append(DLinkedList& L) {

	// Set a pointer node to the first value of the new list that we'll insert from
	Node *currentNew = L.header->next;

	// Iterate through each value in L and put it before trailer of this list
	while (currentNew != L.trailer) {

		// Get the next node because we'll be moving the current node
		Node *next = currentNew->next;

		// Point trailer prev to the new node
		this->trailer->prev->next = currentNew;
		currentNew->prev = this->trailer->prev;
		this->trailer->prev = currentNew;
		currentNew->next = this->trailer;

		// Move to the next node
		currentNew = next;

	}

	// Point the header of L to the trailer of L because the destructor will handle deleting them
	L.header->next = L.trailer;
	L.trailer->prev = L.header;
}

// Part K: Reverses the list
void DLinkedList::Reverse() {

	// Temporary list that's a copy of the current list
	DLinkedList temp = *this;

	// Empty the current list
	Node* current = this->header->next;
	while (current != this->trailer) {
		Node* next = current->next;
		delete current;
		current = next;
	}
	this->header->next = this->trailer;
	this->trailer->prev = this->header;

	// Set a pointer to the trailer of the temporary list and copy the values to the current list
	Node *currentNew = temp.trailer->prev;
	Node *thisPos = this->header;
	while (currentNew != temp.header) {

		// Create a new node with the same values as the current node and insert into new list
		Node *newNode = new Node;
		newNode->Name = currentNew->Name;
		newNode->Score = currentNew->Score;

		// Insert the new node from the currentNew position
		newNode->next = this->trailer;
		newNode->prev = thisPos;
		this->trailer->prev = newNode;
		thisPos->next = newNode;

		// Move to the next node
		currentNew = currentNew->prev;
		thisPos = newNode;
	}
}

int main() {
	DLinkedList dll;
	dll.AddScoreInOrder("Alice", 100);
	dll.AddScoreInOrder("Charlie", 600);
	dll.AddScoreInOrder("Bob", 200);
	dll.AddScoreInOrder("David", 400);
	dll.AddScoreInOrder("Eve", 50);

	std::cout << "Original List:" << std::endl;
	dll.Print();
	std:: cout << std::endl;

	std::cout << "Size of List: " << dll.size() << std::endl;
	std::cout << "List after size() call:" << std::endl;
	dll.Print();
	std::cout << std::endl;

	std::cout << "Sorted by names:" << std::endl;
	dll.OrderByNames();
	dll.Print();
	std::cout << std::endl;

	std::cout << "Sorted by scores:" << std::endl;
	dll.OrderByScores();
	dll.Print();
	std::cout << std::endl;

	DLinkedList dll2;
	dll2.AddScoreInOrder("Frank", 300);
	dll2.AddScoreInOrder("George", 700);
	dll2.AddScoreInOrder("Hank", 800);
	dll2.AddScoreInOrder("Ivan", 900);
	dll2.AddScoreInOrder("Jack", 1000);
	std::cout << "Second List:" << std::endl;
	dll2.Print();
	std::cout << std::endl;

	std::cout << "Appending Second List to First List:" << std::endl;
	dll.Append(dll2);
	dll.Print();
	std::cout << std::endl;

	std::cout << "Reversing the List:" << std::endl;
	dll.Reverse();
	dll.Print();
	std::cout << std::endl;
}
