// Casey Walker (2024)
// 24-673 Soft Robotics
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
#include <Arduino_DebugUtils.h>

#include "cal.h"


Task::Task(task_fn* fn, size_t dt) {
    this->fn          = fn;
    this->dt          = dt;
    this->t_last_call = micros();
}

void Task::operator()(uint32_t t) {
    auto dt_since_last_call = t - this->t_last_call;

    if (this->dt <= dt_since_last_call) {
        this->t_last_call = t;
        this->fn();
    }
}


bool Cal::setup() {
    return true;
}

void Cal::tick() {
    auto t = micros();
    for (auto& task : this->tasks) {
        task(t);
    }
}

void Cal::add(task_fn* fn, uint32_t dt) {
    auto task = Task(fn, dt);
    this->tasks.push_back(task);
}
