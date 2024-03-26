// Casey Walker (2024)
// 24-673 Soft Robotics
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
#include <string>

#include <Arduino_DebugUtils.h>

#include "cal.h"
#include "comm.h"
#include "lumberyard.h"

using namespace std;


// Pins to read the sensor voltages with.
#define SENSOR_PIN_1 40
#define SENSOR_PIN_2 41

// Intervals at which to call each task, in microseconds.
#define HEARTBEAT_INTERVAL 1000000
#define SAVE_INTERVAL      20000
#define SERIAL_INTERVAL    10000


/// @brief The data logger object.
Lumberyard ly;

/// @brief The task scheduler object.
Cal cal;

/// @brief The object used for reading in commands from serial.
Comm comm;


void setup() {
    Serial.begin(BAUD);

    if (!cal.setup()) {
        DEBUG_ERROR("Could not set up scheduler.");
        while (true)
            ;
    }
    DEBUG_INFO("Scheduler set up.");

    if (!comm.setup()) {
        DEBUG_ERROR("Could not set up communications.");
        while (true)
            ;
    }
    DEBUG_INFO("Serial connected.");

    if (!ly.setup()) {
        DEBUG_ERROR("Could not set up logging.");
        while (true)
            ;
    }
    DEBUG_INFO("SD card connected.");

    if (!setup_tasks()) {
        DEBUG_ERROR("Could not set up tasks.");
        while (true)
            ;
    }
    DEBUG_INFO("Tasks setup.");

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(SENSOR_PIN_1, INPUT);
    pinMode(SENSOR_PIN_2, INPUT);
}

void loop() {
    cal.tick();
}

/// @brief Set up the task scheduler, adding the tasks to be called.
bool setup_tasks() {
    cal.add(heartbeat, HEARTBEAT_INTERVAL);
    cal.add(check_serial, SERIAL_INTERVAL);
    cal.add(save, SAVE_INTERVAL);

    return true;
}

/// @brief Blink the built-in LED to show everything is still running smoothly.
void heartbeat() {
    static uint32_t beat = LOW;
    digitalWrite(LED_BUILTIN, beat);
    beat = !beat;
}

/// @brief Read in commands from serial and do any necessary actions.
///
/// Actions are limited to:
///   "ls"
///     List files and directories in the SD card's root directory.
///   "start"
///     Start data logging and write to serial the sensor readings.
///   "stop"
///     Stop data logging and stop writing sensor readings to serial.
///   "new [filename]"
///     Create a new file and use it for logging.
///
/// See "comm.h" and "comm.cxx" for details on how commands are read in.
void check_serial() {
    auto msg = comm.read();

    if (!msg.has_cmd())
        return;

    if (msg.is_cmd("ls")) {
        ly.ls();
    } else if (msg.is_cmd("start")) {
        DEBUG_INFO("Starting data collection.");
        ly.should_save_data = true;
    } else if (msg.is_cmd("stop")) {
        DEBUG_INFO("Stopping data collection.");
        ly.should_save_data = false;
    } else if (msg.is_cmd("new")) {
        if (!msg.has_args()) {
            DEBUG_ERROR("No filepath specified.");
        } else {
            DEBUG_INFO("Changing save file to '%s'", msg.args);
            ly.change_savefile_to(msg.args);
        }
    }
}

/// @brief Save the sensor readings to the SD card.
void save() {
    uint32_t t = micros();
    uint32_t val1 = analogRead(SENSOR_PIN_1);
    uint32_t val2 = analogRead(SENSOR_PIN_2);

    if (ly.should_save_data) {
        DEBUG_INFO("%u, %u, %u", t, val1, val2);
    }

    ly.save_csv_line(t, val1, val2);
}
