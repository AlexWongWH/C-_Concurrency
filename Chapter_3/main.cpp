#include "main.hpp"

// if shared data is read-only -> unaffected
// data is modified

// invariants -> ground truths that holds
// broken invariants often causes issues

// race condition -> trying to modifying the same data
// Outcome depends on the order of execution (order matters) of operations on
// two or more threads

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

int main() {

  // example of locking list and modifying it
  {
    // std::list<int> some_list;
    // std::array<std::thread, 2> pool_thread;

    // // cannot gurantee the sequence of thread launch
    // int count{1};
    // for (auto i{0}; i < 2; ++i) {
    //   pool_thread[i] = std::thread(add_to_list, std::ref(some_list), count);
    //   ++count;
    // }
    // //wrapper around member function
    // std::for_each(pool_thread.begin(), pool_thread.end(),
    //               std::mem_fn(&std::thread::join));
    // {
    //   // std::thread t2([&some_list, &some_mutex]() {
    //   //   std::lock_guard<std::mutex> guard(some_mutex);
    //   //   for (int i{0}; i < 5; ++i) {
    //   //     some_list.emplace_back(i + 5);
    //   //   }
    //   // });
    //   // // creating a thread to write to list
    //   // std::thread t1([&some_list, &some_mutex]() {
    //   //   std::lock_guard<std::mutex> guard(some_mutex);
    //   //   for (int i{0}; i < 5; ++i) {
    //   //     some_list.emplace_back(i);
    //   //   }
    //   // });
    //   // t1.join();
    //   // t2.join();
    // }
    // for (auto const &v : some_list) {
    //   std::cout << "val in list " << v << std::endl;
    // }
  }

  return 0;
}