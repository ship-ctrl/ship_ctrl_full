#pragma once

#define GOOGLE_STRIP_LOG 1    // this must go before the #include!
//#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
//#include "Ship_state.hpp"
#include <sstream>
#include <iostream>
#include "containers.hpp"
//#include "GNSSData.h"

/// @brief tokenizes a sentence to store in a container
/// @param sentence msg itself except $TALKERIDMSGTYPE and *CHKSUM witch are already cuted and checked in HANDLER class
/// @param container see containers.hpp 4 details
/// @return 0 on OK, err position of error otherwise
uint8_t parseNMEA_ETL(const std::string& sentence, ETL_container* container);

/// @brief get ETL from container 
/// @note once ship class is formed the message get method may be rewrited to directly read from SHIP CLASS
/// @param container container with fields translating from message in 1:1 without modification
/// @return (std::string) all $TALKERID and *CHKSUM
std::string get_ETL(const ETL_container& container);

/// @brief test purpose generator
/// @return kinda valid ETL msg
/// @test ${1: test will be somehow like nmea_tools.formNMEAMessage(etlObj.generate_ETL())
std::string generate_ETL();
