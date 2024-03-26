// Casey Walker (2024)
// 24-673 Soft Robotics
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
#ifndef CAL_H
#define CAL_H

#include <vector>

#include <Arduino.h>

using namespace std;


/// @brief A function that can be used in a task.
typedef void task_fn();

/// @brief A task that can be scheduled for a specific interval.
class Task {
  private:
    task_fn* fn;          /// @brief Function to call when task is due.
    uint32_t dt;          /// @brief Call interval for task, in milliseconds.
    uint32_t t_last_call; /// @brief Time of last call for this task.
  public:
    /// @brief Create a new task from a function to call at a specific time
    ///   interval.
    ///
    /// @param `fn` The function to call.
    /// @param `dt` The time interval, in microseconds, to call the funciton.
    Task(task_fn* fn, size_t dt);

    /// @brief Update the task with the current time, and call the function if
    ///   its interval has passed.
    ///
    /// @param `t` The current time to update the task with, in microseconds.
    void operator()(uint32_t t);
};


/// @brief The scheduler which can call tasks based on intervals.
class Cal {
  private:
    vector<Task> tasks; /// @brief The tasks that belong to this scheduler.

  public:
    /// @brief Set up the task scheduler. For Arduino, this should be called in
    ///   the `setup` function.
    ///
    /// @return `true` if the setup was successful, `false` otherwise.
    bool setup();

    /// @brief Force the scheduler to run an update cycle. For Arduino, this
    ///   should be called in the `loop` funciton.
    ///
    /// This will have the scheduler call each of its tasks which will possibly
    ///   also call their functions.
    void tick();

    /// @brief Add a task to the scheduler.
    ///
    /// @param `fn` The function to create the task with.
    /// @param `dt` The time interval to call the function on.
    void add(task_fn* fn, uint32_t dt);
};

#endif // CAL_H
