// Casey Walker (2024)
// 24-673 Soft Robotics
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
#include <algorithm>
#include <cstring>

#include <Arduino_DebugUtils.h>

#include "comm.h"


bool Cmd_msg::has_cmd() {
    return this->cmd[0] != '\0';
}

bool Cmd_msg::has_args() {
    return this->args[0] != '\0';
}

bool Cmd_msg::is_cmd(const char* test) {
    return strcmp(this->cmd, test) == 0;
}


bool Comm::setup() {
    Serial.begin(BAUD);
    while (!Serial)
        ; // Wait for serial to connect.

    DEBUG_INFO("Serial connected successfully.");

    return true;
}

Cmd_msg Comm::read() {
    auto msg = Cmd_msg();

    size_t available_bytes = Serial.available();
    size_t bytes_to_read   = std::min(available_bytes, BUF_LEN - this->tail);

    // Read all bytes available through the first newline.
    size_t bytes_read = 0;
    for (size_t i = 0; i < bytes_to_read; i++) {
        int c = Serial.read();
        if (c < 0)
            break;

        bytes_read++;
        this->buf[this->tail + i] = c;

        if (c == '\n')
            break;
    }

    if (bytes_read == 0)
        return msg; // Nothing read, nothing to do.

    this->tail += bytes_read;

    // Find the first space and the newline character (i.e. end of command and
    // end of message) if they exist.
    size_t msg_end_pos = BUF_LEN;
    size_t cmd_end_pos = BUF_LEN;
    for (size_t i = 0; i < this->tail; i++) {
        char c = this->buf[i];
        if (c == '\n') {
            msg_end_pos = i;
            break;
        } else if (c == ' ') {
            cmd_end_pos = i;
        }
    }

    if (msg_end_pos == BUF_LEN)
        return msg; // No newline found, so command isn't done being read.

    // Set the end of the command message to be the nul char so it acts as a
    // C-style string.
    this->buf[msg_end_pos] = '\0';

    if (cmd_end_pos == BUF_LEN) {
        size_t cmd_end_pos = msg_end_pos;
        strncpy(msg.cmd, this->buf, cmd_end_pos);
    } else {
        // Split the command message into two C-style strings, the command and
        // the arguments to the command.
        this->buf[cmd_end_pos] = '\0';

        size_t cmd_len  = cmd_end_pos;
        size_t args_len = this->tail - cmd_len;

        strncpy(msg.cmd, this->buf, cmd_end_pos);
        strncpy(msg.args, this->buf + cmd_len + 1, args_len);
    }

    // Reset buffer to beginning since the command message has been handled.
    this->tail = 0;

    return msg;
}
