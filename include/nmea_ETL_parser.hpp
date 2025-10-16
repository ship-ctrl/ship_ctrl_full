#pragma once

#define GOOGLE_STRIP_LOG 1    // this must go before the #include!
//#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
//#include "Ship_state.hpp"
#include <sstream>
#include <iostream>
#include "containers.hpp"
//#include "GNSSData.h"


uint8_t parseNMEA_ETL(const std::string& sentence, ETL_container* container);

/// @brief 
/// @param container 
/// @return 
std::string get_ETL(const ETL_container& container);

/// @brief test purpose generator
/// @return kinda valid ETL msg
/// @test ${1: test will be somehow like nmea_tools.formNMEAMessage(etlObj.generate_ETL())
std::string generate_ETL();
