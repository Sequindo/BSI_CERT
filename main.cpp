#include <iostream>
#include <cstring>
#include <vector>

struct A {
    int* arrA = new int[10]{2};
};

struct B {
    int* arrB = new int[10000000000]{2};
};

struct C {
    int val = -1;
    C();
    ~C();
};

int g(A* a, B* b, int pos)
{
    auto sum_pos =  a->arrA[pos] + b->arrB[pos];
    return sum_pos;
}

template <typename T>
void insert_in_vector(std::vector<T> &vector, long long pos, T value) {
  if (pos >= vector.size()) {
    return;
  }
  vector[pos] = value;
}

int main()
{
    g(new A, new B, 0);

    alignas(struct C) char space_on_stack[sizeof(struct C)];
    C* c_ptr = new(&space_on_stack) C;
    delete c_ptr; //UB

    std::vector<int> int_vector {1, 2, 3};
    insert_in_vector<int>(int_vector, -1, 1);

    return 0;
}