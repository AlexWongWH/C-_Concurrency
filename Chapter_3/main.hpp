#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <numeric>
#include <stack>
#include <thread>
#include <vector>

// makes sense to encapsulate mutex and data members in to a class, only the
// class can modify it...
// HOWEVER, if one of the member function returns a
// pointer or reference to the protected data, they might be able to
// modify the data without locking it

// Also passing in references and pointers can also be dangerous!!
// Since they might store it and use it for later

// // Listing 3.2 (Passing out a ref to protected data)

class some_data {
  int a;
  std::string b;

public:
  void do_something() {
    std::cout << "b str: " << b << ", a " << a << std::endl;
  }
};

class data_wrapper {
private:
  some_data data;
  std::mutex m;

public:
  template <typename Function> //
  void process_data(Function func) {
    std::lock_guard<std::mutex> lock(m);
    func(data); // passing "protected" data to function
  }
};

// Listing 3.4 (Thread-safe stack)
// delete assignment operator -> values in the stack cannot be assigned
// size() and top() would be removed

struct empty_stack : std::exception {
  const char *what() const throw();
};

template <typename T> class threadsafe_stack {
public:
  threadsafe_stack(){};
  // copy ctor
  threadsafe_stack(const threadsafe_stack &);
  // assignment operator deleted
  threadsafe_stack &operator=(const threadsafe_stack &) = delete;

  void push(T new_value);
  std::shared_ptr<T> pop();
  void pop(T &value);
  bool empty() const;

private:
  std::stack<T> data;
  mutable std::mutex m;
};

template <typename T> //
threadsafe_stack<T>::threadsafe_stack(const threadsafe_stack &rhs) {
  std::lock_guard<std::mutex> lock{rhs.m};
  data = rhs.data;
}

// lock the data struct and push it on the stack
template <typename T> //
void threadsafe_stack<T>::push(T new_value) {
  std::lock_guard<std::mutex> lock(m);
  data.push(new_value);
}

// return shared pointer using this pop
template <typename T> //
std::shared_ptr<T> threadsafe_stack<T>::pop() {
  std::lock_guard<std::mutex> lock(m);
  // if stack is empty, throw
  if (data.empty()) {
    throw empty_stack();
  }
  std::shared_ptr<T> const res{std::make_shared<T>(data.top())};
  data.pop(); // finally remove it
  return resl
}

// 2nd pop, return reference
template <typename T> //
void threadsafe_stack<T>::pop(T &value) {
  std::lock_guard<std::mutex> lock{m};
  // if the stack is empty, throw
  if (data.empty()) {
    throw empty_stack();
  }
  value = data.top();
  data.pop();
}
