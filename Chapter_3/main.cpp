#include "main.hpp"

// if shared data is read-only -> unaffected
// data is modified

// invariants -> ground truths that holds
// broken invariants often causes issues

// race condition -> trying to modifying the same data
// Outcome depends on the order of execution (order matters) of operations on
// two or more threads

// std::lock_guard<std::mutex> implements RAII for mutex

std::mutex some_mutex;
void add_to_list(std::list<int> &list, const int mul) {
  // some_mutex.lock();
  // unlocks when it goes out of scope
  std::lock_guard<std::mutex> lock(some_mutex);
  for (int i{0}; i < 5; ++i) {
    list.emplace_back(i * mul);
  }
  // some_mutex.unlock();
}

bool list_contains(std::list<int> &list, int value_to_find) {
  // ensures that the funtions are `MUTUALLY EXCLUSIVE` they will never see it
  // halfway
  std::lock_guard<std::mutex> guard(some_mutex);
  return std::find(list.begin(), list.end(), value_to_find) !=
         list.end(); // ensure that the list is never seen half way..
}

void print_list(std::list<int> &list) {
  std::lock_guard<std::mutex> guard(some_mutex);
  for (auto const &v : list) {
    std::cout << "val in list " << v << std::endl;
  }
}

some_data *unprotected;
data_wrapper x;

void malicious_function(some_data &protected_data) {
  unprotected = &protected_data;
}
void foo() {
  // passing the function which does the assignment
  x.process_data(malicious_function);
  unprotected->do_something();
}

// Don’t pass pointers & references to protected data outside the scope of the
// lock
// store them in externally visible mem, passing them as args to user supplied
// function

// (Interface problem) take simple stack for example
// change top() -> copy of the element

// Race conditions
// 1.
// Difficult to tell if empty() & size() is true, might be correct in the call
// but due to other threads modifying it's wrong
// -> calling top() on empty stack

// stack<int> s;
// if(!s.empty())             // -> 1
// {
//  int const value=s.top();  // -> 2
//  s.pop();                  // -> 3
//  do_something(value);
// }

// 2. with a stack of size 2
// considering both threads are running 2 & 3, due to concurrency. both threads
// might get the same s.top() element[1] , and then pop both values without
// using the result of the element[0]

// 3. combine stack `pop()` normally return value popped and remove from
// stack consider:
// if std::stack<std::vector<int>> , not enough memory throws std::bad_alloc.
//  `pop()` throws exception -> value is lost!
//  Which is why top() & pop() exist -> original problem is back

// # solution #
// 1. passing a reference
// std::vector<int> result;
// some_stack.pop(result);

// 2. No throw copy ctor & move ctor

// 3. return pointer to pop item
// manage memory (shared ptr)

int main() {

  // // example of locking list and modifying it
  // {
  //   std::list<int> some_list;
  //   std::array<std::thread, 2> pool_thread;
  //   // cannot gurantee the sequence of thread launch
  //   int count{1};
  //   for (auto i{0}; i < 2; ++i) {
  //     pool_thread[i] = std::thread(add_to_list, std::ref(some_list), count);
  //     ++count;
  //   }
  //   // wrapper around member function
  //   std::for_each(pool_thread.begin(), pool_thread.end(),
  //                 std::mem_fn(&std::thread::join));
  //   {
  //     // // creating the thread sequentially, gurantees the order
  //     // std::mutex local_mutex;
  //     // std::thread t2([&some_list, &local_mutex]() {
  //     //   std::lock_guard<std::mutex> guard(local_mutex);
  //     //   for (int i{0}; i < 5; ++i) {
  //     //     some_list.emplace_back(i + 5);
  //     //   }
  //     // });
  //     // // creating a thread to write to list
  //     // std::thread t1([&some_list, &local_mutex]() {
  //     //   std::lock_guard<std::mutex> guard(local_mutex);
  //     //   for (int i{0}; i < 5; ++i) {
  //     //     some_list.emplace_back(i);
  //     //   }
  //     // });
  //     // t2.join();
  //     // t1.join();
  //   }
  //   print_list(some_list);
  // }

  return 0;
}