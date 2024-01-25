#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// gcc -pthread -o term term.c
void do_something(const int &i) {
  std::cout << "do_something " << i << std::endl;
}

void hello() { std::cout << "hello concurrent world\n"; }

struct func {
  int &i;
  func(int &i_) : i(i_) {}
  void operator()() {
    std::cout << "fnc operator \n";

    for (unsigned j = 0; j < 3; ++j) {
      do_something(i);
    }
  }
};

class thread_guard {
  std::thread &t_;

public:
  explicit thread_guard(std::thread &thr) : t_{thr} {}

  ~thread_guard() {
    if (t_.joinable()) // checks if thread is joinable
    {
      t_.join();
    }
  }
  thread_guard(thread_guard const &rhs) = delete;            // copy ctor
  thread_guard &operator=(thread_guard const &rhs) = delete; // copy assignment
};

void faa(int i, std::string const &s) {}
void not_oops(int some_param) {
  char buffer[1024];
  sprintf(buffer, "%i", some_param);
  std::thread t(faa, 3, std::string(buffer));
  t.detach();
}

void some_function() { std::cout << "some_function \n"; };
void some_other_function() { std::cout << "some_other_function\n"; }

// //transferring ownership out of a function
// std::thread f()
// {
//     void some_function();
//     return std::thread(some_function);
// }

// //transferring ownership into a function
// void f(std::thread t);
// void g()
// {
//     void some_func();
//     f(std::thread(some_func));
//     std::thread t(some_func);
//     f(std::move(t));
// }

// listing 2.6
class scoped_thread {
  std::thread t_;

public:
  explicit scoped_thread(std::thread t) : t_{std::move(t)} {
    // has ownership of the thread, check if this thread exist
    if (!t_.joinable()) {
      throw std::logic_error("No Thread");
    }
  }

  ~scoped_thread() { t_.join(); }
  scoped_thread(const scoped_thread &rhs) = delete;
  scoped_thread &operator=(scoped_thread const &) = delete;
};

// listing 2.7
//  With this ownership able to create a number of threads and wait for them to
//  finish

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

int main() {

  {
    int ref{4};
    func f{ref}; // creating a temp object with function call operator
    // defined
    std::thread t(f); //
    // Blocks the current thread until the thread identified by *this
    // finishes its execution
    // returns when the thread execution has completed.
    t.join();
  }

  // using lambdas
  {
      // std::thread th([](){hello();});
      // th.join(); //calling join would clean up the storage with the thread
  }

  // need to decide to `wait` or `join` the thread

  // std::threads calls std::terminate, when program is terminated
  //, gave to ensure `wait` or `join` the thread

  // Bette to have threads containing local copy instead of pointers and
  // reference
  // Have to note that if the main thread lifespan goes out of scope and it
  // child is still running,

  // blocking, waiting for thread to complete -> calling join() or detach()

  // //join() might be skipped if an exception is thrown, therefore have to join
  // when exception was thrown
  {
      //     std::cout << "Expection thrown\n";
      //     int local_state {0};
      //     std::thread s_thread(hello);
      //     try
      //     {
      //         //do something in the thread
      //     }
      //     catch(std::exception e)
      //     {
      //         s_thread.join();
      //         throw e.what();
      //     }
      //     s_thread.join();
  }

  // RAII for thread completation, since garbage collection
  {
      // int t_guard_local{24};
      // func fnc(t_guard_local);
      // std::thread t(fnc);
      // do somethine in thread;
  }

  // calling detach() -> leaves threads to run in the background
  // named:`daemon threads`
  // runs in background without explicit user interface
  // background task (monitoring file sys, clearing obj cache)

  // no longer joinable
  {
      // std::thread t1(some_function);
      // std::thread t2 = std::move(t1);
      // t1 = std::thread(some_other_function);
      // std::thread t3;
      // t3 = std::move(t2);
      // t1 = std::move(t3); // t1 already has an associated thread
      // // t1.join();
      // // t2.join();
      // // t3.join();
  }

  {
      // do_work_f();
  }

  {
    // std::thread::hardware_concurrency(); // returns num of threads that can
    // run
    //                                      // for execution of program.
    //                                      Multicore
    //                                      // sys -> CPU cores
    // std::cout << "num of threads in hardware "
    //           << std::thread::hardware_concurrency() << std::endl;
  }

  return 0;
}