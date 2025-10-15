///@brief All container files belong to here 
///@authors @Tommy0x121 @ship-ctrl

#pragma once

#include <iostream>

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