// I am implementing a singly linked list

#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <vector>

using namespace std;

struct Node
{
  int value;
  Node* next;

  // Constructor with no pointer input
  Node(int n)
  {
    value = n;
    next = nullptr;
  }

  // Constructor with call-by-pointer as input
  Node(int n, Node* p)
  {
    value = n;
    next = p;
  }
};


class LinkedList
{
private:
  Node* head;
  Node* tail;
  int size;

  // Getting node at given index
  Node* get_node(int index) {
    if (index < 0 or index >= size) {
      throw out_of_range("IndexError");
    }

    Node* current = head;
    for (int i=0; i<index; i++) {
      current = current->next;
    }
    return current;
  }

public:
  // Constructor for empty list: head and tail points at nullptr
  LinkedList()
  {
    size = 0;
    head = nullptr;
    tail = nullptr;
  }

  // Overloaded constructor to initialize list with elements
  LinkedList(vector<int> initial)
  {
    size = 0;
    head = nullptr;

    for (int val: initial) {
      append(val);
    }
  }

  // Getting number of elements in list
  int length()
  {
    return size;
  }

  // Adding new element to end of list
  void append(int val)
  {
    size += 1;

    if (head == nullptr) {
      head = new Node(val);
      tail = head;
      return;
    }

    tail->next = new Node(val);
    tail = tail->next;
  }

  // Printing elements on single line
  void print()
  {
    Node* current = head;
    cout << "[";
    while (current->next != nullptr) {
      cout << current->value;
      cout << ", ";
      current = current->next;
    }
    cout << current->value << "]" << endl;
  }

  // Destructor to free memory
  ~LinkedList()
  {
    Node* current;
    Node* next;

    current = head;

    while (current != nullptr) {
      next = current->next;
      delete current;
      current = next;
    }
  }

  // Overloading []-operator to access element by index
  int& operator[](int index)
  {
    return get_node(index)->value;
  }

  // Inserting element into list at given index
  void insert(int val, int index)
  {
    Node* prev = get_node(index-1);
    Node* next = prev->next;
    prev->next = new Node(val, next);

    size += 1;
  }

  // Removing element at given index
  void remove(int index)
  {
    Node* prev = get_node(index-1);
    Node* current = get_node(index);
    prev->next = current->next;
    delete current;

    size -= 1;
  }

  // Removing element at given index and returning it
  int pop(int index)
  {
    int temp = get_node(index)->value;
    remove(index);
    return temp;
  }

  // Overloading pop if no given index: pops last element
  int pop()
  {
    int temp = get_node(size-1)->value;
    remove(size-1);
    return temp;
  }
};


int main()
{
  cout << endl;
  cout << "---linked_list.cpp---" << endl;
  cout << endl;

  LinkedList A;
  cout << setw(20) << left << "LinkedList A;" << endl;
  A.append(1); cout << setw(20) << "A.append(1)"; A.print();
  A.append(2); cout << setw(20) << "A.append(2)"; A.print();
  A.append(3); cout << setw(20) << "A.append(3)"; A.print();
  A.insert(4, 2); cout << setw(20) << "A.insert(4, 2)"; A.print();
  A.remove(3); cout << setw(20) << "A.remove(3)"; A.print();
  cout << setw(20) << "A.pop(1)" << A.pop(1) << " "; A.print();
  cout << setw(20) << "A.pop()" << A.pop() << " "; A.print();

  cout << endl;
  LinkedList B({5, 4, 3, 2, 1});
  cout << setw(20) << left << "LinkedList B({5, 4, 3, 2, 1});" << endl;
  cout << setw(20) << "B.print()"; B.print();
  cout << setw(20) << "B[2]" << B[2] << endl;
  cout << setw(20) << "B.length()" << B.length() << endl;
  cout << endl;

  return 0;
}
