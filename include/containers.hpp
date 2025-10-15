///@brief All container files belong to here 
///@authors @Tommy0x121 @ship-ctrl

#pragma once

#include <iostream>

/// @brief container to store ETL
/// @todo make shipClass <-> ETLc converter fcn
struct ETL_container
{
    uint8_t hh;
    uint8_t mm;
    uint8_t ss;
    uint16_t ms;

    /// @brief  msg type ( Order/Answerback )
    std::string msg_type;
    /// @brief position_indicator_of_engine_telegraph
    std::string eng_tel;
    /// @brief position_indicator_of_sub_telegraph
    uint8_t sub_tel;
    /// @brief operating_location_indicator
    std::string op_location;
    /// @brief number_of_engine_or_propeller_shaft
    uint8_t shaft_num;
};