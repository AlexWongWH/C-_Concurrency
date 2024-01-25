#include "chpt_2.hpp"
#include <array>
#include <mutex>

std::thread::id master_thread_id;

std::mutex some_mutex;
// master thread should populate the array, rest of threads adds it
std::array<int, 16> My_array;

void do_work(unsigned id) { std::cout << "doing work\n"; }

void do_work_f() {
  std::vector<std::thread> threads;
  for (auto i{0}; i < 10; ++i) {
    threads.push_back(std::thread(do_work, i)); // spawning threads
  }
  // std::mem_fn, generates wrapper obj for pointers to members. To store and
  // copy and involve mem to functions
  std::for_each(threads.begin(), threads.end(),
                std::mem_fn(&std::thread::join));
}

void core_algo() {
  if (std::this_thread::get_id() == master_thread_id) {
    int i{0};
  } else {
  }
}

int main() {

  std::cout << " master thread id: " << master_thread_id << std::endl;
  std::cout << " std::this_thread::get_id(): " << std::this_thread::get_id()
            << std::endl;

  // std::thread t(core_algo);
  // t.join();

  // std::thread common_thread(some_core_part_of_algorithm);
  // common_thread.join();

  int data[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  // int para_accumulate(int first, int end, int *data, int &acc) {
  int acc{};

  int myacc = para_accumulate(0, 10, data, acc);

  std::cout << " Accumulate value: " << myacc << std::endl;

  return 0;
}
