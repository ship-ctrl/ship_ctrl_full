#ifndef VERSION_H
#define VERSION_H

#include <string>

#define PROJECT_NAME "ship_ctrl_full"

#define MAJOR_VERSION  0
#define MINOR_VERSION  0
#define BUILD_NUMBER  76

#define VERSION_STRING  "0.0.76"
#define BUILD_DATE = __DATE__
#define BUILD_TIME = __TIME__

// Function to get full version info
inline std::string get_version_info() {
    return std::string("Version ") + std::string(VERSION_STRING) + 
           " (Build " + std::to_string(BUILD_NUMBER) + ")";
}

#endif // VERSION_H
