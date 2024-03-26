// Casey Walker (2024)
// 24-673 Soft Robotics
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
#ifndef COMM_H
#define COMM_H

#include <Arduino.h>

#include <string>

using namespace std;


// Configurations for the serial communications.
#define BAUD    115200
#define BUF_LEN 4096


/// @brief A command message read in from serial.
///
/// If no command or arguments, the buffers will be filled with `'\0'`.
struct Cmd_msg {
    char cmd[BUF_LEN]  = {'\0'}; /// @brief The command buffer.
    char args[BUF_LEN] = {'\0'}; /// @brief The command arguments buffer.

    bool has_cmd();  /// @brief Return if the command buffer is not empty.
    bool has_args(); /// @brief Return if the arguments buffer is not empty.

    /// @brief Return if the message command matches a test string.
    ///
    /// This is useful for testing which command is read in from serial.
    bool is_cmd(const char* test);
};


/// @brief A communication object to keep track of and execute commands from
///   serial input.
///
/// Messages are split at the first space if there is one, where the command
/// is the substring before the space, and the arguments are collected as one
/// substring after the space.
class Comm {
  private:
    char buf[BUF_LEN]; /// @brief Buffer for holding serial contents.
    size_t tail;       /// @brief The last valid character in the buffer.
  public:
    /// @brief Set up the communications. For Arduino, this should be called in
    ///   the `setup` function.
    ///
    /// @return `true` if successful setup, `false` otherwise.
    bool setup();

    /// @brief Read in from the serial buffer and return the current command
    ///   if there is one.
    ///
    /// @return A command message object with the read-in command and its
    ///   arguments. If the line read in was empty, the returned object consists
    ///   of empty strings.
    Cmd_msg read();
};

#endif // COMM_H
