#pragma once

#define GOOGLE_STRIP_LOG 1    // this must go before the #include!
//#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include "Ship_state.h"
#include <sstream>
#include <iostream>
//#include "GNSSData.h"

/// @brief container to store ETL
/// @todo make shipClass <-> ETLc converter fcn
struct ETL_container
{
    int hh;
    int mm;
    int ss;
    int ms;
    /// @brief  msg type ( Order/Answerback )
    std::string message_type;
    std::string position_indicator_of_engine_telegraph;
    int position_indicator_of_sub_telegraph;
    std::string operating_location_indicator;
    int number_of_engine_or_propeller_shaft;
};


int parseNMEA_ETL(const std::string& sentence, ETL_container* container)

/// @brief 
/// @param container 
/// @return 
std::string get_ETL(ETL_container container)

/// @brief test purpose generator
/// @return kinda valid ETL msg
/// @test ${1: test will be somehow like nmea_tools.formNMEAMessage(etlObj.generate_ETL())
std::string generate_ETL()
