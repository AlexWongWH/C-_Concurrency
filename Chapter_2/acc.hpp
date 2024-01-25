#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <thread>
#include <vector>

template <typename Iterator, typename T> struct accumulate_block {
  void operator()(Iterator first, Iterator last, T &result) {
    result = std::accumulate(first, last, result);
  }
};

// naive way of parallel accumulate
template <typename Iterator, typename T> //
T parallel_accumulate(Iterator first, Iterator last, T init) {

  // Must have forward iterators
  // T must be default constructible

  // number of elements
  unsigned long const length = std::distance(first, last);
  if (!length) {
    return init; // 1. return if input range is empty
  }

  // num or elements to be processed by the thread
  unsigned long const min_per_thread{25};

  // div num of elemets to process by min block size
  unsigned long const max_thread =
      (length + min_per_thread - 1) / min_per_thread; // 2.

  unsigned long const hardware_threads = std::thread::hardware_concurrency();

  // 3.
  // more threads then hardware (oversubscription)
  // too many threads -> context switching
  // too little -> less concurrency
  unsigned long const num_threads =
      std::min(hardware_threads != 0 ? hardware_threads : 2, max_thread);

  // 4. num of elements to process by a thread
  // -> (length of elements / num of threads)
  unsigned long const block_size = length / num_threads;
  std::vector<T> result(num_threads);                // intermediate result
  std::vector<std::thread> threads(num_threads - 1); // 5. holds num of threads

  Iterator block_start = first;
  // thread 0~4, each thread manages 25 elements, with 100 elements:
  // [0] -> 0~24 , [1] ->25~49
  for (unsigned long i = 0; i < (num_threads - 1); ++i) {
    // set the iterator to the beginning of the element block
    Iterator block_end = block_start;

    // 6. Advance the iterator by the block(elements per block)
    std::advance(block_end, block_size);

    // 7. launch new threads 0~4, to accumulate the result
    threads[i] = std::thread(
        accumulate_block<Iterator, T>(), block_start, block_end,
        std::ref(result[i])); // can't return value from thread, have to pass a
                              // reference from threads
    // 8. next block index -> Start of the block  [1,2] [3,4] [5,6]
    block_start = block_end;
  }

  // 9. Launching the final block
  accumulate_block<Iterator, T>()(block_start, last, result[num_threads - 1]);

  std::for_each(threads.begin(), threads.end(),
                std::mem_fn(&std::thread::join)); // 10.

  return std::accumulate(result.begin(), result.end(), init); // 11.
}
