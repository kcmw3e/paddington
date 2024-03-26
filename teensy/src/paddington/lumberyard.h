// Casey Walker (2024)
// 24-673 Soft Robotics
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
#ifndef LUMBERYARD_H
#define LUMBERYARD_H

#include <string>

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

using namespace std;

#define SD_CARD BUILTIN_SDCARD


/// @brief The CSV data logger.
class Lumberyard {
  private:
    File root;     /// @brief The root directory of the SD card.
    File savefile; /// @brief The current save file.
  public:
    /// @brief Whether or not to currently write data to the save file.
    bool should_save_data = false;

    /// @brief Set up the data logger. For Arduino, this should be called in
    ///   the `setup` function.
    ///
    /// @return `true` if successful setup, `false` otherwise.
    bool setup();

    /// @brief List the files and directories in the root directory of the SD
    ///   card.
    void ls();

    /// @brief Change the file to which data is saved.
    ///
    /// @param `filepath` The new path to the file to save data to.
    void change_savefile_to(const char* filepath);

    /// @brief Save a CSV line in the current save file.
    ///
    /// @param `t` The time of sensor readings.
    /// @param `val1` The value read from the first sensor.
    /// @param `val2` The value read from the second sensor.
    void save_csv_line(uint32_t t, uint32_t val1, uint32_t val2);
};

#endif // LUMBERYARD_H
