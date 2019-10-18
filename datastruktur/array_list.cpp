#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>

using namespace std;

class ArrayList
{
private:
  int *data;
  int capacity;
  int growth;
  int size;

  // Resizes ArrayList according to growth factor
  void resize()
  {
    capacity *= growth;
    int *new_data = new int[capacity];
    for (int i=0; i<size; i++) {
      new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
  }

  // Shrinking storage array to smallest capacity growth^n
  void shrink_to_fit()
  {
    capacity = growth;
    while (capacity <= size) {
      capacity *= growth;
    }
    cout << capacity << endl;

    int *new_data = new int[capacity];
    for (int i=0; i<size; i++) {
      new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
  }

public:
  // Constructor, creates an empty ArrayList
  ArrayList()
  {
    size = 0;
    capacity = 1;
    growth = 2;
    data = new int[capacity];
  }

  // Overloading basic constructor with initial values
  ArrayList(vector<int> initial)
  {
    size = 0;
    capacity = size;
    growth = 2;
    data = new int[capacity];

    for (int e: initial) {
      append(e);
    }
  }

  // Destructor to free memory
  ~ArrayList()
  {
    delete[] data;
  }

  // Getting size of ArrayList
  int length()
  {
    return size;
  }

  // Printing contents of ArrayList
  void print()
  {
    cout << "[";
    for (int i=0; i<size-1; i++) {
      cout << data[i] << ", ";
    }
    cout << data[size-1] << "]" << endl;
  }

  // Appending an integer n to end of ArrayList
  void append(int n)
  {
    if (size >= capacity) {
      resize();
    }
    data[size] = n;
    size += 1;
  }

  // Overloading the []-operators
  int& operator[](int i) {
    if (0 <= i and i < size) {
      return data[i];
    } else {
      throw out_of_range("IndexError");
    }
  }

  // Inserting integer val into ArrayList at given index
  void insert(int val, int index)
  {
    if (0 <= index and index < size) {
      append(data[size]);
      for (int i=size-1; i>index; i--) {
        data[i] = data[i-1];
      }
      data[index] = val;
    } else {
      throw out_of_range("IndexError");
    }
  }

  // Remove value at given index
  void remove(int index)
  {
    if (0 <= index and index < size) {
      for (int i=index; i<size; i++) {
        data[i] = data[i+1];
      }
      size -= 1;
    } else {
      throw out_of_range("IndexError");
    }

    if (size < 0.25*capacity) {
      shrink_to_fit();
    }
  }

  // Remove value at given index and return it
  int pop(int index)
  {
    int temp = data[index];
    remove(index);

    if (size < 0.25*capacity) {
      shrink_to_fit();
    }

    return temp;
  }

  // Overloading pop method with no given index: removes last element
  int pop()
  {
    int temp = data[size-1];
    remove(size-1);

    if (size < 0.25*capacity) {
      shrink_to_fit();
    }

    return temp;
  }
};

// Function for checking if positive integer x is a prime
bool is_prime(int x)
{
  if (x < 2) return false;
  for (int i=2; i <= sqrt(x); i++) {
    if ((x%i) == 0) return false;
  }
  return true;
}

int main()
{
  cout << endl;
  cout << "---array_list.cpp---" << endl;
  cout << endl;

  ArrayList A;

  // Finding 10 primes
  int i = 1;
  while (A.length() < 10) {
    if (is_prime(i)) A.append(i);
    i += 1;
  }
  A.print();

  // Testing overloaded constructor
  ArrayList primes({2, 3, 5, 8, 11});
  primes.print();

  // Testing insert method
  primes.insert(0, 2);
  primes.print();

  // Testing remove method
  primes.remove(2);
  primes.print();

  // Testing pop method
  cout << primes.pop(2) << " ";
  primes.print();

  // Testing overloaded pop method
  cout << primes.pop() << " ";
  primes.print();

  return 0;
}
