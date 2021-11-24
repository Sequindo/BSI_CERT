#include <iostream>
#include <cstring>
#include <vector>
#include <memory>

struct A {
    int* arrA = nullptr; //in case of bad_alloc unknown value of arrA is passed to delete[]. Nullptr is a correct value that can be returned by new[]. Refer to MEM51-CPP.
    A() noexcept(false) { //may throw std::bad_alloc. Marked as noexcept(false) - refer to ERR55-CPP.
        arrA = new int[10]{2};
    }
    ~A() { delete[] arrA; }
};

struct B {
    int* arrB = nullptr; //in case of bad_alloc unknown value of arrA is passed to delete[]. Nullptr is a correct value that can be returned by new[]. Refer to MEM51-CPP.
    B() noexcept(false) { //may throw std::bad_alloc. Marked as noexcept(false) - refer to ERR55-CPP.
        arrB = new int[10000000000]{2};
    }
    ~B() { delete[] arrB; }
};

struct C {
    int val = -1;
    C() {}
    ~C() {}
};

int g(std::unique_ptr<A> a, std::unique_ptr<B> b, std::size_t pos) noexcept(false) //prevent memory leak by using unique_ptr. Refer to MEM52-CPP.
{ //this function may throw. Marked as noexcept(false) - refer to ERR55-CPP. Check if pos is between correct bounds. Use size_t for preventing passing negative values. Refer to CTR50-CPP.
    if(pos < sizeof(b->arrB)/sizeof(b->arrB[0] || pos < sizeof(a->arrA)/sizeof(a->arrA[0])))
    {
        auto sum_pos =  a->arrA[pos] + b->arrB[pos];
        return sum_pos;
    }
    return INT32_MIN;
}

template <typename T>
void insert_in_vector(std::vector<T> &vector, std::size_t pos, T value) noexcept(false) { //size_t for iterator type to prevent passing neg. values. Refer to CTR50-CPP.
  if (pos >= vector.size()) {
    return;
  }
  vector.at(pos) = value; //vector.at() throws an exception in case of out-of-bound read. + marking this function as throwing. Refer tp ERR55-CPP.
}

int main()
{
    try
    {
        g(std::make_unique<A>(), std::make_unique<B>(), 0); //Using unique pointers to prevent memory leak in case of exception in A or B constructor. Wrapped in try-catch as this function may throw an exception.
    } //Refer to ERR55-CPP, MEM52-CPP.
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    alignas(struct C) char space_on_stack[sizeof(struct C)];
    C* c_ptr = new(&space_on_stack) C;
    c_ptr->~C(); //c_ptr is a pointer returned by placement new. The memory was allocated on the stack - we should call ~C() destructor rather than deleting() c_ptr to prevent UB.
//Refer to MEM51-CPP.
    std::vector<int> int_vector {1, 2, 3};
    try
    {
        insert_in_vector<int>(int_vector, -1, 1); //Wrapped in try-catch block because insert_in_vector may throw an exception. Refer to ERR55-CPP.
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}