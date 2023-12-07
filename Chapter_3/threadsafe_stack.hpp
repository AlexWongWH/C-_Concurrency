#include <memory>
#include <mutex>
#include <stack>

// struct just for throwing
struct empty_stack : std::exception {
  const char *what() const throw();
};

// # solution # for thread safe classes
// 1. passing a reference
// std::vector<int> result;
// some_stack.pop(result);

// 2. No throw copy ctor & move ctor

// 3. return pointer to pop item
// manage memory (shared ptr)

template <typename T> //
class threadsafe_stack {
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
  // have to get the lock from other stack (source)
  std::lock_guard<std::mutex> lock{rhs.m};
  data = rhs.data;
}

// lock the data struct and push it on the stack
template <typename T> //
void threadsafe_stack<T>::push(T new_value) {
  std::lock_guard<std::mutex> lock{m};
  data.push(new_value);
}

template <typename T> //
std::shared_ptr<T> threadsafe_stack<T>::pop() {
  std::lock_guard<std::mutex> lock{m};

  if (data.empty()) {
    throw empty_stack{};
  }
  // create a shared pointer to the top of the stack
  const std::shared_ptr<T> ptr{std::make_shared<T>(data.top())};
  data.pop();
  return ptr;
}

template <typename T> //
void threadsafe_stack<T>::pop(T &value) {
  std::lock_guard<std::mutex> lock{m};
  if (data.empty()) {
    throw empty_stack{};
  }
  value = data.top();
  data.pop();
}

template <typename T> //
bool threadsafe_stack<T>::empty() const {
  std::lock_guard<std::mutex> lock{m};
  return data.empty();
}
