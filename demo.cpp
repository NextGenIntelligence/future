#include <iostream>
#include <future>
#include <thread>

struct base
{
};

std::ostream& operator<<(std::ostream& os, const base&)
{
  return os << "base";
}

struct derived : base
{
};

std::ostream& operator<<(std::ostream& os, const derived&)
{
  return os << "derived";
}

int main()
{
  std::packaged_task<int()> call_me([]
  {
    std::cout << "returning 13 from thread " << std::this_thread::get_id() << std::endl;
    return 13;
  });

  auto f1 = call_me.get_future();

  std::thread t1(std::move(call_me));

  std::cout << "Thread 1 result: " << f1.get() << std::endl;

  t1.join();

  std::packaged_task<int()> throw_exception([]
  {
    std::cout << "throwing exception from thread " << std::this_thread::get_id() << std::endl;

    throw 13;

    return 13;
  });

  auto f2 = throw_exception.get_future();

  std::thread t2(std::move(throw_exception));

  try
  {
    std::cout << "Thread 2 result: " << f2.get() << std::endl;
  }
  catch(int x)
  {
    std::cout << "Thread 2 exception: " << x << std::endl;
  }

  t2.join();

  std::packaged_task<derived()> return_derived([]
  {
    std::cout << "returning derived from thread " << std::this_thread::get_id() << std::endl;
    return derived();
  });

  std::future<base> f3 = return_derived.get_future();

  std::thread t3(std::move(return_derived));

  std::cout << "Thread 3 result: " << f3.get() << std::endl;

  t3.join();

  return 0;
}

