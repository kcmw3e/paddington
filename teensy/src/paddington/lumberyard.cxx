// Casey Walker (2024)
// 24-673 Soft Robotics
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
#include <Arduino_DebugUtils.h>

#include "lumberyard.h"

#define CSV_LINE_BUF_LEN 1024


bool Lumberyard::setup() {
    if (!SD.begin(SD_CARD))
        return false;

    this->root = SD.open("/");
    if (!this->root)
        return false;

    return true;
}

void Lumberyard::ls() {
    DEBUG_INFO("Listing files and directories:");
    for (File entry = this->root.openNextFile(); entry;
         entry      = this->root.openNextFile()) {
        if (entry.isDirectory())
            DEBUG_INFO("> |--> ['%s']", entry.name());
        else
            DEBUG_INFO("> |--> '%s'", entry.name());
    }
    this->root.rewindDirectory();
}

void Lumberyard::change_savefile_to(const char* filepath) {
    this->savefile = SD.open(filepath, FILE_WRITE);
    if (!this->savefile) {
        DEBUG_WARNING("Could not open file '%s'", filepath);
    }

    if (this->savefile.isDirectory()) {
        DEBUG_WARNING("'%s' is a directory.", filepath);
        this->savefile = File();
    }
}

void Lumberyard::save_csv_line(uint32_t t, uint32_t val1, uint32_t val2) {
    if (!this->savefile)
        return;

    if (this->savefile.isDirectory())
        return;

    char buf[CSV_LINE_BUF_LEN];
    auto n = snprintf(
        buf, sizeof(buf) / sizeof(*buf), "%u,%u,%u\n", t, val1, val2
    );
    this->savefile.write(buf, n);
    this->savefile.flush();
}
