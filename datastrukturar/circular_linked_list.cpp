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


class CircLinkedList
{
private:
  Node* head;
  Node* tail;
  int size;

public:
  // Basic constructor
  CircLinkedList()
  {
    size = 0;
    head = nullptr;
  }

  // Overloaded constructor
  CircLinkedList(int n)
  {
    head = nullptr;
    for (int i=1; i<=n; i++) {
      append(i);
    }
    size = n;
  }

  // Appending element to end of list
  void append(int val)
  {
    // if empty list: insert value and point at iself
    if (head == nullptr) {
      head = new Node(val, head);
      size = 1;
      return;
    }
    Node* last = new Node(val, head);
    Node* current = head;

    // If two or more elements in list: cycle until end of list
    if (size > 1) {
      while (current->next != head) {
        current = current->next;
      }
    }
    current->next = last;
    size += 1;
  }

  // Overloading []-operator to access element by index
  int operator[](int index)
  {
    if (head == nullptr) {
      throw out_of_range("IndexError: List is empty");
    }
    Node* current = head;
    for (int i=0; i<index; i++) {
      current = current->next;
    }
    return current->value;
  }

  // Printing elements in list once on a single line
  void print()
  {
    Node* current = head;
    cout << "[";
    while (current->next != head) {
      cout << current->value;
      cout << ", ";
      current = current->next;
    }
    cout << current->value << ", ...]" << endl;
  }

  // Finding the Josephus Sequence
  vector<int> josephus_sequence(int k)
  {
    Node* current = nullptr;
    Node* prev;
    Node* temp;
    vector<int> seq;
    while (size != 0) {

      // Iterating over k elements
      for (int i=0; i<k; i++) {

        // Starting iterations at first element
        if (current == nullptr) {
          current = head;
          continue;
        }
        temp = current;
        current = current->next;
        prev = temp;
      }
      seq.push_back(current->value);
      prev->next = current->next;
      delete current;
      size -= 1;
    }
    return seq;
  }
};

// Function for solving the Josephus problem
int last_man_standing(int n, int k)
{
  CircLinkedList list(n);
  vector<int> sequence = list.josephus_sequence(k);
  return sequence.back();
}


int main()
{
  cout << endl;
  cout << "---circular_linked_list.cpp---" << endl;
  cout << endl;

  CircLinkedList clist;
  cout << setw(20) << left << "CircLinkedList clist;" << endl;
  clist.append(0); cout << "clist.append(0);" << endl;
  clist.append(2); cout << "clist.append(2);" << endl;
  clist.append(4); cout << "clist.append(4);" << endl;
  cout << "clist.print();" << endl;

  cout << endl;
  clist.print();

  cout << endl;
  cout << "L(68, 7) = " << last_man_standing(68, 7) << endl;
  cout << endl;
  
  return 0;
}
