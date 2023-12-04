#include "chpt_2.hpp"
#include <array>

std::thread::id master_thread;

// void some_core_part_of_algorithm() {
//   if (std::this_thread::get_id() == master_thread) {
//     // do_master_thread_work();
//   }
//   // do_common_work();
// }

// master thread should populate the array, rest of threads adds it

std::array<int, 16> My_array;

void core_algo() {
  std::cout << "inside core algo: " << std::this_thread::get_id() << std::endl;

  if (std::this_thread::get_id() == master_thread) {
  }
}

int main() {

  // core_algo();
  std::cout << "in main " << std::this_thread::get_id() << std::endl;

  std::thread t(core_algo);
  t.join();

  return 0;
}
