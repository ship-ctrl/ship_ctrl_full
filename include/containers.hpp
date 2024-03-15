/// Containers and container_only related methods
/// @Tommy0x121 @attention @todo #2 please reorganize: all struct here and all
///

#pragma once

struct ETL_container
{
    /* data */
};



/// @brief this is proto class 4 later reorganization
class containers
{
private:
    ETL_container etl; /// @todo leave this alone 4 now just example
public:
    /// @brief just allocate mem constructor, cpp'll handle itself
    containers(/* args */);

    /// @brief use defalut destructor, we're sane
    virtual ~containers() = default;

    /// @brief it'll be collection of overloaded fcns?
    /// @param etl @typedef ETL_container
    void Set_ETL(ETL_container &etl);
};

containers::containers(/* args */)
{
}

containers::~containers()
{
}

void containers::Set_ETL(ETL_container &etl)
{

}

#define GOOGLE_STRIP_LOG 1    // this must go before the #include!
//#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include "Ship_state.h"
#include <sstream>
#include <iostream>
#include "GNSSData.h"

/// @brief Data container proto
/// @author @Tommy0x121 
struct ETL_container
{
    int hh; // @ship-ctrl so ever but why not short?
    int mm;
    int ss;
    int ms;
    std::string Message_type; // @Tommy0x121 @attention this is odd @todo
    std::string Position_indicator_of_engine_telegraph; // @Tommy0x121 @attention this probably shoud be corresponding int repr? @todo
    int Position_indicator_of_sub_telegraph;
    std::string Operating_location_indicator;
    int Number_of_engine_or_propeller_shaft;
};


/// @brief tokenizes string and dumps to contalner
/// @author @Tommy0x121 
/// @details @todo Does standard allows partial NULL fields? And do we wanna handle 'em?
/// @param sentence <ETL,hhmmss.ss, >
/// @param container ETL_container type
/// @return error token position / 1 otherwise
int parseNMEA_ETL(const std::string& sentence, ETL_container* container)
{
    short error_index = -1;
    std::istringstream iss(sentence);
    std::string token;

    if (!(std::getline(iss, token, ',') && token.substr(3) == "ETL"))
    {
        LOG(FATAL) << "Token error in " << sentence;
        return error_index;
    }


    std::string time;
    --error_index;
    if (!(std::getline(iss, time, ',') && (size(time) == 9 || size(time) == 0)))
    {
        LOG(WARNING) << "Time error in " << sentence;
        return error_index;
    }

    container->hh = stoi(time.substr(0, 2));
    container->mm = stoi(time.substr(2, 2));
    container->ss = stoi(time.substr(4, 2));
    container->ms = stoi(time.substr(7, 2));

    std::string message_type;
    --error_index;
    if (!(std::getline(iss, message_type, ',') && (message_type == "O" || message_type == "A")))
    {
        LOG(WARNING)<< "Message type error in " << sentence;
        return error_index;
    }
    
    container->Message_type = message_type;

    std::string position_indicator;
    --error_index;
    if (!(std::getline(iss, position_indicator, ',') && (position_indicator[0] == '0' || position_indicator[0] == '1') && std::stoi(position_indicator.substr(1)) > 0 && std::stoi(position_indicator.substr(1)) < 6))
    {
        LOG(WARNING) << "Position indicator of engine telegraph error in " << sentence;
        return error_index;
    }
    container->Position_indicator_of_engine_telegraph = position_indicator  ;
    
    std::string sub_telegraph_position;
    --error_index;
    if (!(std::getline(iss, sub_telegraph_position, ',') && (std::stoi(sub_telegraph_position) == 20 || std::stoi(sub_telegraph_position) == 30 || std::stoi(sub_telegraph_position) == 40)))
    {
        LOG(WARNING) << "Position indicator of sub telegraph position error" << sentence;
        return error_index;
    }
    
    container->Position_indicator_of_sub_telegraph = stoi(sub_telegraph_position);

    std::string operating_location_indicator;
    --error_index;
    if (!(std::getline(iss, operating_location_indicator, ',') && (size(operating_location_indicator) == 0 || size(operating_location_indicator) == 1 && (operating_location_indicator[0] == 'B' || operating_location_indicator[0] == 'C' || operating_location_indicator[0] == 'P' || operating_location_indicator[0] == 'S' || operating_location_indicator[0] == 'E' || operating_location_indicator[0] == 'W'))))
    {
        LOG(WARNING) << "Opertaing location indicator error in " << sentence;
        return error_index;
    }

    container->Operating_location_indicator = operating_location_indicator;

    std::string Number;
    --error_index;
    if (!(std::getline(iss, Number, '*') && (Number == "Odd" || Number == "Even" || Number == "0"))) //@Tommy0x121 @test @todo don't rly get it
    {
        LOG(WARNING) << "Number  of engine or propeller shaft error in " << sentence;
        return error_index;
    }
    
    container -> Number_of_engine_or_propeller_shaft = stoi(Number);

    return 1;
}

/// @brief 
/// @author @Tommy0x121 
/// @return 
std::string generate_ETL()
{
    std::string hh = std::to_string(rand() % 24); if (size(hh) == 1) hh = "0" + hh;
    std::string mm = std::to_string(rand() % 60); if (size(mm) == 1) mm = "0" + mm;
    std::string ss = std::to_string(rand() % 60); if (size(ss) == 1) ss = "0" + ss;
    std::string ms = std::to_string(rand() % 100); if (size(ms) == 1) ms = "0" + ms;
    std::string event_time = hh + mm + ss + "." + ms;
    std::string message_type = (rand() % 2) ? "O" : "A";
    std::string position_indicator_of_engine_telegraph = std::to_string(rand() % 2) + std::to_string(rand() % 5 + 1);
    std::string position_indicator_of_sub_telegraph = std::to_string(rand() % 3 + 2) + "0";
    std::string help[7] = { "B", "P", "S", "C", "E", "W", " "};
    std::string operating_location_indicator =  help[rand() % 7];
    std::string number_of_engine_or_propeller_shaft = std::to_string(rand() % 10);
    std::string comma = ",";
    return "$--ETL" + comma + event_time + comma + message_type + comma + position_indicator_of_engine_telegraph + comma
        + position_indicator_of_sub_telegraph + comma + operating_location_indicator + comma + number_of_engine_or_propeller_shaft + comma +
        "*hh<CR><LF>";
}

/// @brief 
/// @author @Tommy0x121 
/// @param container ETL_container type
/// FCN does not change so just passing a const link
/// @return 
std::string get_ETL(const ETL_container& container) 
{
    std::string hh = std::to_string(container.hh); if (size(hh) == 1) hh = "0" + hh;
    std::string mm = std::to_string(container.mm); if (size(mm) == 1) mm = "0" + mm;
    std::string ss = std::to_string(container.ss); if (size(ss) == 1) ss = "0" + ss;
    std::string ms = std::to_string(container.ms); if (size(ms) == 1) ms = "0" + ms;
    std::string event_time = hh + mm + ss + "." + ms;
    std::string message_type = container.message_type; // @Tommy0x121 @todo Either #define constant "ETL" or 
    std::string position_indicator_of_engine_telegraph = container.position_indicator_of_engine_telegraph;
    std::string position_indicator_of_sub_telegraph = std::to_string(container.position_indicator_of_sub_telegraph);
    std::string operating_location_indicator = container.operating_location_indicator;
    std::string number_of_engine_or_propeller_shaft = std::to_string(container.number_of_engine_or_propeller_shaft);
    std::string comma = ",";
    return "$--ETL" + comma + event_time + comma + message_type + comma + position_indicator_of_engine_telegraph + comma
        + position_indicator_of_sub_telegraph + comma + operating_location_indicator + comma + number_of_engine_or_propeller_shaft + comma +
        "*hh<CR><LF>";
}

struct HTC_HTD_container
{
    std::string Override;
    unsigned Commanded_rudder_angle;
    std::string Commanded_rudder_direction;
    std::string Selected_steering_mode;
    std::string Turn_mode;
    unsigned Commanded_rudder_limit;
    unsigned Commanded_off_heading_limit;
    unsigned Commanded_radius_of_turn;
    unsigned Commanded_rate_of_turn;
    unsigned Commanded_heading_to_steer;
    unsigned Commanded_off_track_limit;
    unsigned Commanded_track;
    std::string Heading_reference_in_use;
    std::string Rudder_status;
    std::string Off_heading_status;
    std::string Off_track_status;
    int Vessel_heading;
    std::string Sentence_status;
};

int parseNMEA_HTC(const std::string& sentence, HTC_HTD_container *storage)
{
    short error_index = -1;
std::istringstream iss(sentence);


    std::string token;
    --error_index;
    if (!(std::getline(iss, token, ',') && token.substr(3) == "HTC"))
    {
       LOG(FATAL)<< "Message type error in " << sentence;
       return error_index;
    }


    std::string override;
    --error_index;
    if(!(std::getline(iss, override, ',') && (override == "A" || override == "V")))
    {
        LOG(WARNING) << "override eroor in " << sentence;
        //std::cout << "override is " << override << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "override is " << override << "   " << sentence << std::endl;
    storage->Override = override;

    std::string commanded_rudder_angle;
    --error_index;
    if (!(std::getline(iss, commanded_rudder_angle, ',') && size(commanded_rudder_angle) != 0 && (commanded_rudder_angle.find_first_not_of("0123456789.") == commanded_rudder_angle.npos)))
    {
        LOG(WARNING) << "commanded_rudder_angle error in " << sentence;
        //std::cout << "commanded_rudder_angle is " << commanded_rudder_angle << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_rudder_angle = stof(commanded_rudder_angle);


    std::string commanded_rudder_direction;
    --error_index;
    if(!(std::getline(iss, commanded_rudder_direction, ',') && (commanded_rudder_direction == "L" || commanded_rudder_direction == "R")))
    {
        LOG(WARNING) << "commanded_rudder_direction eroor in " << sentence;
        //std::cout << "commanded_rudder_direction is " << commanded_rudder_direction << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Commanded_rudder_direction = commanded_rudder_direction;
    //std::cout << "commanded_rudder_direction is " << commanded_rudder_direction << "   " << sentence << std::endl;


    std::string selected_steering_mode;
    --error_index;
    if(!(std::getline(iss, selected_steering_mode, ',') && (selected_steering_mode == "M" || selected_steering_mode == "S" || selected_steering_mode == "H" || selected_steering_mode == "R" || selected_steering_mode == "T")))
    {
        LOG(WARNING) << "selected_steering_mode eroor in " << sentence;
        //std::cout << "selected_steering_mode is " << selected_steering_mode << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Selected_steering_mode = selected_steering_mode;
    //std::cout << "selected_steering_mode is " << selected_steering_mode << "   " << sentence << std::endl;

    
    std::string turn_mode;
    --error_index;
    if(!(std::getline(iss, turn_mode, ',') && (turn_mode == "R" || turn_mode == "T" || turn_mode == "N")))
    {
        LOG(WARNING) << "turn_mode eroor in " << sentence;
        //std::cout << "turn_mode is " << turn_mode << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "turn_mode is " << turn_mode << "   " << sentence << std::endl;
    storage->Turn_mode = turn_mode;


    std::string commanded_rudder_limit;
    --error_index;
    if (!(std::getline(iss, commanded_rudder_limit, ',') && size(commanded_rudder_limit) != 0 && (commanded_rudder_limit.find_first_not_of("0123456789.") == commanded_rudder_limit.npos)))
    {
        LOG(WARNING) << "commanded_rudder_limit error in " << sentence;
        //std::cout << "commanded_rudder_limit is " << commanded_rudder_limit << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_rudder_limit = stof(commanded_rudder_limit);


    std::string commanded_off_heading_limit;
    --error_index;
    if (!(std::getline(iss, commanded_off_heading_limit, ',') && size(commanded_off_heading_limit) != 0 && (commanded_off_heading_limit.find_first_not_of("0123456789.") == commanded_off_heading_limit.npos)))
    {
        LOG(WARNING) << "commanded_off_heading_limit error in " << sentence;
        //std::cout << "commanded_off_heading_limit is " << commanded_off_heading_limit << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_off_heading_limit = stof(commanded_off_heading_limit);


    std::string commanded_radius_of_turn;
    --error_index;
    if (!(std::getline(iss, сommanded_radius_of_turn, ',') && size(сommanded_radius_of_turn) != 0 && (сommanded_radius_of_turn.find_first_not_of("0123456789.") == сommanded_radius_of_turn.npos)))
    {
        LOG(WARNING) << "сommanded_radius_of_turn error in " << sentence;
        //std::cout << "сommanded_radius_of_turn is " << сommanded_radius_of_turn << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_radius_of_turn = stof(commanded_radius_of_turn);


    std::string commanded_rate_of_turn;
    --error_index;
    if (!(std::getline(iss, commanded_rate_of_turn, ',') && size(commanded_rate_of_turn) != 0 && (commanded_rate_of_turn.find_first_not_of("0123456789.") == commanded_rate_of_turn.npos)))
    {
        LOG(WARNING) << "commanded_rate_of_turn error in " << sentence;
        //std::cout << "commanded_rate_of_turn is " << commanded_rate_of_turn << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_rate_of_turn = stof(commanded_rate_of_turn);


    std::string commanded_heading_to_steer;
    --error_index;
    if (!(std::getline(iss, commanded_heading_to_steer, ',') && size(commanded_heading_to_steer) != 0 && (commanded_heading_to_steer.find_first_not_of("0123456789.") == commanded_heading_to_steer.npos)))
    {
        LOG(WARNING) << "commanded_heading_to_steer error in " << sentence;
        //std::cout << "commanded_heading_to_steer is " << commanded_heading_to_steer << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_heading_to_steer = stof(commanded_heading_to_steer);

    std::string сommanded_off_track_limit;
    --error_index;
    if (!(std::getline(iss, сommanded_off_track_limit, ',') && size(сommanded_off_track_limit) != 0 && (сommanded_off_track_limit.find_first_not_of("0123456789.") == сommanded_off_track_limit.npos)))
    {
        LOG(WARNING) << "сommanded_off_track_limit error in " << sentence;
        //std::cout << "сommanded_off_track_limit is " << сommanded_off_track_limit << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_off_track_limit = stof(сommanded_off_track_limit);


    std::string commanded_track;
    --error_index;
    if (!(std::getline(iss, commanded_track, ',') && size(commanded_track) != 0 && (commanded_track.find_first_not_of("0123456789.") == commanded_track.npos)))
    {
        LOG(WARNING) << "commanded_track error in " << sentence;
        //std::cout << "commanded_track is " << commanded_track << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_track = stof(commanded_track);


    std::string heading_reference_in_use;
    --error_index;
    if(!(std::getline(iss, heading_reference_in_use, ',') && (heading_reference_in_use == "T" || heading_reference_in_use == "M")))
    {
        LOG(WARNING) << "heading_reference_in_use eroor in " << sentence;
        //std::cout << "heading_reference_in_use is " << heading_reference_in_use << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Heading_reference_in_use = heading_reference_in_use;
    //std::cout << "heading_reference_in_use is " << heading_reference_in_use << "   " << sentence << std::endl;

    std::string sentence_status;
    --error_index;
    if(!(std::getline(iss, sentence_status, '*') && (sentence_status == "T" || sentence_status == "M")))
    {
        LOG(WARNING) << "sentence_status eroor in " << sentence;
        //std::cout << "sentence_status is " << sentence_status << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Sentence_status = sentence_status;

    return 1;
}


std::string get_htc(HTC_HTD_container container)
{
    std::string Override = container.Override;
    std::string Commanded_rudder_angle = std::to_string(container.Commanded_rudder_angle);
    std::string Commanded_rudder_direction = container.Commanded_rudder_direction;
    std::string Selected_steering_mode = container.Selected_steering_mode;
    std::string Turn_mode = container.Turn_mode;
    std::string Commanded_rudder_limit = std::to_string(container.Commanded_rudder_limit);
    std::string Commanded_off_heading_limit = std::to_string(container.Commanded_off_heading_limit);
    std::string Commanded_radius_of_turn = std::to_string(container.Commanded_radius_of_turn);
    std::string Commanded_rate_of_turn = std::to_string(container.Commanded_rate_of_turn);
    std::string Commanded_heading_to_steer = std::to_string(container.Commanded_heading_to_steer);
    std::string Commanded_off_track_limit = std::to_string(container.Commanded_off_track_limit);
    std::string Commanded_track = std::to_string(container.Commanded_track);
    std::string Heading_reference_in_use = container.Heading_reference_in_use;
    std::string Sentence_status = container.Sentence_status;
    std::string Comma = ",";

    return "$--HTC" + Override + Comma + Commanded_rudder_angle + Comma + Commanded_rudder_direction + Comma + Selected_steering_mode
    + Comma + Turn_mode + Comma + Commanded_rudder_limit + Comma + Commanded_off_heading_limit + Comma + Commanded_radius_of_turn + 
    Comma + Commanded_rate_of_turn + Comma + Commanded_heading_to_steer + Comma + Commanded_off_track_limit + Comma +
    Commanded_track + Comma + Heading_reference_in_use + Comma + Sentence_status + "*<CR><LF>";    
}

std::string generate_htc()
{
    std::string Override = (rand() % 2) ? "A" : "V";
    std::string Commanded_rudder_angle = std::to_string(rand() / 100);
    std::string Commanded_rudder_direction = (rand() % 2) ? "R" : "L";
    int help = rand() % 5;
    std::string Selected_steering_mode = (help == 0) ? "M" : ((help == 1) ? "S" : ((help == 2) ? "H" : ((help == 3) ? "T" : "R")));
    help = rand() % 3;
    std::string Turn_mode = (help == 0) ? "T" : ((help == 1) ? "R" : "N");
    std::string Commanded_rudder_limit = std::to_string(int(rand() % 100));
    std::string Commanded_off_heading_limit = std::to_string(int(rand() % 100));
    std::string Commanded_radius_of_turn = std::to_string(int(rand() % 100));
    std::string Commanded_rate_of_turn = std::to_string(int(rand() % 100));
    std::string Commanded_heading_to_steer = std::to_string(int(rand() % 100));
    std::string Commanded_off_track_limit = std::to_string(int(rand() % 100));
    std::string Commanded_track = std::to_string(int(rand() % 100));
    std::string Heading_reference_in_use = (help % 2) ? "T" : "M";
    std::string Sentence_status = (rand() % 2) ? "C" : "R";
    std::string Comma = ",";
    
    return "$--HTC" + Override + Comma + Commanded_rudder_angle + Comma + Commanded_rudder_direction + Comma + Selected_steering_mode
    + Comma + Turn_mode + Comma + Commanded_rudder_limit + Comma + Commanded_off_heading_limit + Comma + Commanded_radius_of_turn + 
    Comma + Commanded_rate_of_turn + Comma + Commanded_heading_to_steer + Comma + Commanded_off_track_limit + Comma +
    Commanded_track + Comma + Heading_reference_in_use + Comma + Sentence_status + "*<CR><LF>";       
}









int parseNMEA_HTD(const std::string& sentence, HTC_HTD_container* storage)
{
    short error_index = -1;
    std::istringstream iss(sentence);

    std::string token;
    --error_index;
    if (!(std::getline(iss, token, ',') && token.substr(3) == "HTD"))
    {
       LOG(FATAL)<< "Message type error in " << sentence;
       return error_index;
    }

    std::string override;
    --error_index;
    if(!(std::getline(iss, override, ',') && (override == "A" || override == "V")))
    {
        LOG(WARNING) << "override eroor in " << sentence;
        //std::cout << "override is " << override << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "override is " << override << "   " << sentence << std::endl;
    storage->Override = override;

    std::string commanded_rudder_angle;
    --error_index;
    if (!(std::getline(iss, commanded_rudder_angle, ',') && size(commanded_rudder_angle) != 0 && (commanded_rudder_angle.find_first_not_of("0123456789.") == commanded_rudder_angle.npos)))
    {
        LOG(WARNING) << "commanded_rudder_angle error in " << sentence;
        //std::cout << "commanded_rudder_angle is " << commanded_rudder_angle << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_rudder_angle = stof(commanded_rudder_angle);


    std::string commanded_rudder_direction;
    --error_index;
    if(!(std::getline(iss, commanded_rudder_direction, ',') && (commanded_rudder_direction == "L" || commanded_rudder_direction == "R")))
    {
        LOG(WARNING) << "commanded_rudder_direction eroor in " << sentence;
        //std::cout << "commanded_rudder_direction is " << commanded_rudder_direction << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Commanded_rudder_direction = commanded_rudder_direction;
    //std::cout << "commanded_rudder_direction is " << commanded_rudder_direction << "   " << sentence << std::endl;


    std::string selected_steering_mode;
    --error_index;
    if(!(std::getline(iss, selected_steering_mode, ',') && (selected_steering_mode == "M" || selected_steering_mode == "S" || selected_steering_mode == "H" || selected_steering_mode == "R" || selected_steering_mode == "T")))
    {
        LOG(WARNING) << "selected_steering_mode eroor in " << sentence;
        //std::cout << "selected_steering_mode is " << selected_steering_mode << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Selected_steering_mode = selected_steering_mode;
    //std::cout << "selected_steering_mode is " << selected_steering_mode << "   " << sentence << std::endl;

    
    std::string turn_mode;
    --error_index;
    if(!(std::getline(iss, turn_mode, ',') && (turn_mode == "R" || turn_mode == "T" || turn_mode == "N")))
    {
        LOG(WARNING) << "turn_mode eroor in " << sentence;
        //std::cout << "turn_mode is " << turn_mode << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "turn_mode is " << turn_mode << "   " << sentence << std::endl;
    storage->Turn_mode = turn_mode;


    std::string commanded_rudder_limit;
    --error_index;
    if (!(std::getline(iss, commanded_rudder_limit, ',') && size(commanded_rudder_limit) != 0 && (commanded_rudder_limit.find_first_not_of("0123456789.") == commanded_rudder_limit.npos)))
    {
        LOG(WARNING) << "commanded_rudder_limit error in " << sentence;
        //std::cout << "commanded_rudder_limit is " << commanded_rudder_limit << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_rudder_limit = stof(commanded_rudder_limit);


    std::string commanded_off_heading_limit;
    --error_index;
    if (!(std::getline(iss, commanded_off_heading_limit, ',') && size(commanded_off_heading_limit) != 0 && (commanded_off_heading_limit.find_first_not_of("0123456789.") == commanded_off_heading_limit.npos)))
    {
        LOG(WARNING) << "commanded_off_heading_limit error in " << sentence;
        //std::cout << "commanded_off_heading_limit is " << commanded_off_heading_limit << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_off_heading_limit = stof(commanded_off_heading_limit);


    std::string commanded_radius_of_turn;
    --error_index;
    if (!(std::getline(iss, сommanded_radius_of_turn, ',') && size(сommanded_radius_of_turn) != 0 && (сommanded_radius_of_turn.find_first_not_of("0123456789.") == сommanded_radius_of_turn.npos)))
    {
        LOG(WARNING) << "сommanded_radius_of_turn error in " << sentence;
        //std::cout << "сommanded_radius_of_turn is " << сommanded_radius_of_turn << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_radius_of_turn = stof(commanded_radius_of_turn);


    std::string commanded_rate_of_turn;
    --error_index;
    if (!(std::getline(iss, commanded_rate_of_turn, ',') && size(commanded_rate_of_turn) != 0 && (commanded_rate_of_turn.find_first_not_of("0123456789.") == commanded_rate_of_turn.npos)))
    {
        LOG(WARNING) << "commanded_rate_of_turn error in " << sentence;
        //std::cout << "commanded_rate_of_turn is " << commanded_rate_of_turn << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_rate_of_turn = stof(commanded_rate_of_turn);


    std::string commanded_heading_to_steer;
    --error_index;
    if (!(std::getline(iss, commanded_heading_to_steer, ',') && size(commanded_heading_to_steer) != 0 && (commanded_heading_to_steer.find_first_not_of("0123456789.") == commanded_heading_to_steer.npos)))
    {
        LOG(WARNING) << "commanded_heading_to_steer error in " << sentence;
        //std::cout << "commanded_heading_to_steer is " << commanded_heading_to_steer << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_heading_to_steer = stof(commanded_heading_to_steer);

    std::string сommanded_off_track_limit;
    --error_index;
    if (!(std::getline(iss, сommanded_off_track_limit, ',') && size(сommanded_off_track_limit) != 0 && (сommanded_off_track_limit.find_first_not_of("0123456789.") == сommanded_off_track_limit.npos)))
    {
        LOG(WARNING) << "сommanded_off_track_limit error in " << sentence;
        //std::cout << "сommanded_off_track_limit is " << сommanded_off_track_limit << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_off_track_limit = stof(сommanded_off_track_limit);


    std::string commanded_track;
    --error_index;
    if (!(std::getline(iss, commanded_track, ',') && size(commanded_track) != 0 && (commanded_track.find_first_not_of("0123456789.") == commanded_track.npos)))
    {
        LOG(WARNING) << "commanded_track error in " << sentence;
        //std::cout << "commanded_track is " << commanded_track << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Commanded_track = stof(commanded_track);


    std::string heading_reference_in_use;
    --error_index;
    if(!(std::getline(iss, heading_reference_in_use, ',') && (heading_reference_in_use == "T" || heading_reference_in_use == "M")))
    {
        LOG(WARNING) << "heading_reference_in_use eroor in " << sentence;
        //std::cout << "heading_reference_in_use is " << heading_reference_in_use << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Heading_reference_in_use = heading_reference_in_use;
    //std::cout << "heading_reference_in_use is " << heading_reference_in_use << "   " << sentence << std::endl;

    
    std::string rudder_status;
    --error_index;
    if(!(std::getline(iss, rudder_status, ',') && (rudder_status == "A" || rudder_status == "V")))
    {
        LOG(WARNING) << "rudder_status eroor in " << sentence;
        //std::cout << "rudder_status is " << rudder_status << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Rudder_status = rudder_status;
    //std::cout << "rudder_status is " << rudder_status << "   " << sentence << std::endl;

    
    std::string off_heading_status;
    --error_index;
    if(!(std::getline(iss, off_heading_status, ',') && (off_heading_status == "A" || off_heading_status == "V")))
    {
        LOG(WARNING) << "off_heading_status eroor in " << sentence;
        //std::cout << "off_heading_status is " << off_heading_status << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Off_heading_status = off_heading_status;
    //std::cout << "off_heading_status is " << off_heading_status << "   " << sentence << std::endl;


    std::string off_track_status;
    --error_index;
    if(!(std::getline(iss, off_track_status, ',') && (off_track_status == "A" || off_track_status == "V")))
    {
        LOG(WARNING) << "off_track_status eroor in " << sentence;
        //std::cout << "off_track_status is " << off_track_status << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Off_track_status = off_track_status;
    //std::cout << "off_track_status is " << off_track_status << "   " << sentence << std::endl;


    std::string vessel_heading;
    --error_index;
    if (!(std::getline(iss, vessel_heading, '*') && size(vessel_heading) != 0 && (Rate_of_turn.find_first_not_of("0123456789.") == vessel_heading.npos)))
    {
        LOG(WARNING) << "vessel_heading error in " << sentence;
        //std::cout << "vessel_heading is " << vessel_heading << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Vessel_heading = vessel_heading;

    return 1;
}


std::string generate_htd()
{
    std::string Override = (rand() % 2) ? "A" : "V";
    std::string Commanded_rudder_angle = std::to_string(rand() / 100);
    std::string Commanded_rudder_direction = (rand() % 2) ? "R" : "L";
    int help = rand() % 5;
    std::string Selected_steering_mode = (help == 0) ? "M" : ((help == 1) ? "S" : ((help == 2) ? "H" : ((help == 3) ? "T" : "R")));
    help = rand() % 3;
    std::string Turn_mode = (help == 0) ? "T" : ((help == 1) ? "R" : "N");
    std::string Commanded_rudder_limit = std::to_string(int(rand() % 100));
    std::string Commanded_off_heading_limit = std::to_string(int(rand() % 100));
    std::string Commanded_radius_of_turn = std::to_string(int(rand() % 100));
    std::string Commanded_rate_of_turn = std::to_string(int(rand() % 100));
    std::string Commanded_heading_to_steer = std::to_string(int(rand() % 100));
    std::string Commanded_off_track_limit = std::to_string(int(rand() % 100));
    std::string Commanded_track = std::to_string(int(rand() % 100));
    std::string Heading_refrerence_in_use = (help % 2) ? "T" : "M";
    std::string Rudder_status = (rand() % 2) ? "A" : "V";
    std::string Off_heading_status = (rand() % 2) ? "A" : "V";
    std::string Off_track_status = (rand() % 2) ? "A" : "V";
    std::string Vessel_heading = std::to_string((rand() % 100) + (float)(rand() / 100));
    std::string Comma = ",";

    return "$--HTD" + Comma + Override + Comma + Commanded_rudder_angle + Comma + Commanded_rudder_direction + Comma + Selected_steering_mode +
        Comma + Turn_mode + Comma + Commanded_rudder_limit + Comma + Commanded_off_heading_limit + Comma + Commanded_radius_of_turn
        + Comma + Commanded_rate_of_turn + Comma + Commanded_heading_to_steer + Comma + Commanded_off_track_limit + Comma +
        Commanded_track + Comma + Heading_refrerence_in_use + Comma + Rudder_status + Comma + Off_heading_status + Comma + Off_track_status
        + Comma + Vessel_heading + Comma + "*<CR><LF>";
}


std::string get_htd(HTC_HTD_container container)
{
    std::string Override = container.Override;
    std::string Commanded_rudder_angle = std::to_string(container.Commanded_rudder_angle);
    std::string Commanded_rudder_direction = container.Commanded_rudder_direction;
    std::string Selected_steering_mode = container.Selected_steering_mode;
    std::string Turn_mode = container.Turn_mode;
    std::string Commanded_rudder_limit = std::to_string(container.Commanded_rudder_limit);
    std::string Commanded_off_heading_limit = std::to_string(container.Commanded_off_heading_limit);
    std::string Commanded_radius_of_turn = std::to_string(container.Commanded_radius_of_turn);
    std::string Commanded_rate_of_turn = std::to_string(container.Commanded_rate_of_turn);
    std::string Commanded_heading_to_steer = std::to_string(container.Commanded_heading_to_steer);
    std::string Commanded_off_track_limit = std::to_string(container.Commanded_off_track_limit);
    std::string Commanded_track = std::to_string(container.Commanded_track);
    std::string Heading_reference_in_use = container.Heading_reference_in_use;
    std::string Rudder_status = (rand() % 2) ? "A" : "V";
    std::string Off_heading_status = (rand() % 2) ? "A" : "V";
    std::string Off_track_status = (rand() % 2) ? "A" : "V";
    std::string Vessel_heading = std::to_string((rand() % 100) + (float)(rand() / 100));
    std::string Comma = ",";
    return "$--HTC" + Override + Comma + Commanded_rudder_angle + Comma + Commanded_rudder_direction + Comma + Selected_steering_mode
    + Comma + Turn_mode + Comma + Commanded_rudder_limit + Comma + Commanded_off_heading_limit + Comma + Commanded_radius_of_turn + 
    Comma + Commanded_rate_of_turn + Comma + Commanded_heading_to_steer + Comma + Commanded_off_track_limit + Comma +
    Commanded_track + Comma + Heading_reference_in_use + Comma + Rudder_status + Comma + Off_heading_status + Comma + Off_track_status 
    + Comma + Vessel_heading + "*<CR><LF>";    
}

struct OSD_container
(
    unsigned Heading; 
    std::string Heading_status;
    unsigned Vessel_course;
    std::string Course_reference;
    unsigned Vessel_speed;
    std::string Speed_reference;
    unsigned Vesset_set;
    unsigned Vessel_drift;
    std::string Speed_units;
);

std::string generate_osd()
{
    
    int help = rand() % 5;
    std::string Heading = std::to_string(int(rand() % 100));; 
    std::string Heading_status = (rand() % 2) ? "A" : "V";;
    std::string Vessel_course = std::to_string(int(rand() % 100));
    std::string Course_reference = (help == 0) ? "M" : ((help == 1) ? "W" : ((help == 2) ? "R" : ((help == 3) ? "B" : "P")));;
    std::string Vessel_speed = std::to_string(int(rand() % 100));
    std::string Speed_reference = (help == 0) ? "M" : ((help == 1) ? "W" : ((help == 2) ? "R" : ((help == 3) ? "B" : "P")));; ;
    help = rand() % 5;
    std::string Vesset_set = std::to_string(int(rand() % 100));
    std::string Vessel_drift = std::to_string(int(rand() % 100));
    help = rand() % 3;
    std::string Speed_units = (help == 0) ? "K" : ((help == 1) ? "N" : "S");

    return "$--OSD" + Comma + Heading + Comma + Heading_status + Comma + Vessel_course + Comma + Course_reference + Comma + 
    Vessel_speed + Comma + Speed_reference + Comma + Vesset_set + Comma + Vessel_drift + Comma + Speed_units + Comma + "*<CR><LF>";

}


std::string get_osd(OSD_container container)
{
    std::string Heading = std::to_string(container.Heading); 
    std::string Heading_status = container.Heading_status;
    std::string Vessel_course = std::to_string(container.Vessel_course);
    std::string Course_reference = container.Course_reference;
    std::string Vessel_speed = std::to_string(container.Vessel_speed); 
    std::string Speed_reference = container.Speed_reference;
    std::string Vesset_set = std::to_string(container.Vesset_set);
    std::string Vessel_drift = std::to_string(container.Vessel_drift);
    std::string Speed_units =  container.Speed_units;
    std::string Comma = ",";

    return "$--OSD" + Comma + Heading + Comma + Heading_status + Comma + Vessel_course + Comma + Course_reference + Comma + 
    Vessel_speed + Comma + Speed_reference + Comma + Vesset_set + Comma + Vessel_drift + Comma + Speed_units + Comma + "*<CR><LF>";
}



int parseNMEA_OSD(const std::string& sentence, OSD_container* storage)
{
    short error_index = -1;
    std::istringstream iss(sentence);

    std::string token;
    if (!(std::getline(iss, token, ',') && token.substr(3) == "OSD"))
    {
        LOG(FATAL) << "Token error in " << sentence;
        return error_index;
    }

    std::string Heading;
    --error_index;
    if (!(std::getline(iss, Heading, ',') && size(Heading) != 0 && (Heading.find_first_not_of("0123456789.") == Heading.npos)))
    {
        LOG(WARNING) << "Heading error in " << sentence;
        //std::cout << "Heading is " << Heading << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Heading = stof(heading);


    std::string heading_status;
    --error_index;
    if(!(std::getline(iss, heading_status, ',') && (heading_status == "A" || heading_status == "V")))
    {
        LOG(WARNING) << "heading_status eroor in " << sentence;
        std::cout << "heading_status is " << heading_status << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Heading_status = heading_status;
    //std::cout << "heading_status is " << heading_status << "   " << sentence << std::endl;

    
    std::string vessel_course;
    --error_index;
    if (!(std::getline(iss, vessel_course, ',') && size(vessel_course) != 0 && (vessel_course.find_first_not_of("0123456789.") == vessel_course.npos)))
    {
        LOG(WARNING) << "vessel_course error in " << sentence;
        //std::cout << "vessel_course is " << vessel_course << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Vessel_course = stof(vessel_course);


    std::string course_reference;
    --error_index;
    if(!(std::getline(iss, course_reference, ',') && (course_reference == "B" || course_reference == "M" || course_reference == "W" || course_reference == "R" || course_reference == "P")))
    {
        LOG(WARNING) << "course_reference eroor in " << sentence;
        std::cout << "course_reference is " << course_reference << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "course_reference is " << course_reference << "   " << sentence << std::endl;
    storage->Course_reference = course_reference;


    std::string vessel_speed;
    --error_index;
    if (!(std::getline(iss, vessel_speed, ',') && size(vessel_speed) != 0 && (Rate_of_turn.find_first_not_of("0123456789.") == vessel_speed.npos)))
    {
        LOG(WARNING) << "vessel_speed error in " << sentence;
        //std::cout << "vessel_speed is " << vessel_speed << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Vessel_speed = stof(vessel_speed);


    std::string speed_reference;
    --error_index;
    if(!(std::getline(iss, speed_reference, ',') && (speed_reference == "B" || speed_reference == "M" || speed_reference == "W" || speed_reference == "R" || speed_reference == "P")))
    {
        LOG(WARNING) << "speed_reference eroor in " << sentence;
        std::cout << "speed_reference is " << speed_reference << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Speed_reference = speed_reference;
    //std::cout << "speed_reference is " << speed_reference << "   " << sentence << std::endl;

    
    std::string vesset_set;
    --error_index;
    if (!(std::getline(iss, vesset_set, ',') && size(vesset_set) != 0 && (vesset_set.find_first_not_of("0123456789.") == vesset_set.npos)))
    {
        LOG(WARNING) << "vesset_set error in " << sentence;
        //std::cout << "vesset_set is " << vesset_set << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Vesset_set = stof(vesset_set);
  

    std::string vessel_drift;
    --error_index;
    if (!(std::getline(iss, vessel_drift, ',') && size(vessel_drift) != 0 && (vessel_drift.find_first_not_of("0123456789.") == vessel_drift.npos)))
    {
        LOG(WARNING) << "vessel_drift error in " << sentence;
        //std::cout << "vessel_drift is " << vessel_drift << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Vessel_drift = stof(vessel_drift);


    std::string speed_units;
    --error_index;
    if(!(std::getline(iss, speed_units, '*') && (speed_units == "K" || speed_units == "N" || speed_units == "S")))
    {
        LOG(WARNING) << "speed_units eroor in " << sentence;
        std::cout << "speed_units is " << speed_units << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Speed_units = stof(speed_units);


    //std::cout << "speed_units is " << speed_units << "   " << sentence << std::endl;

    return 1;
}

struct PRC_container
(
    unsigned Lever_demand_position;
    std::string Lever_demand_status;
    unsigned PRM_demand_value;
    std::string PRM_mode_indicator;
    unsigned Pitch_demand_value;
    std::string Pitch_mode_indicator;
    std::string Operating_location_indicator;
    int Number_of_engine_or_propeller_shaft;
);

std::string generate_prc()
{
    int help = rand() % 3;
    std::string Lever_demand_position = std::to_string(rand() % 200 - 100);
    std::string Lever_demand_status = (rand() % 2) ? "A" : "V";
    std::string PRM_demand_value =  std::to_string(rand() % 100);
    help = rand() % 3;
    std::string PRM_mode_indicator = (help == 0) ? "V" : ((help == 1) ? "R" : "P");
    std::string Pitch_demand_value = std::to_string(rand() % 100);
    help = rand() % 3;
    std::string Pitch_mode_indicator = (help == 0) ? "V" : ((help == 1) ? "D" : "P");;
    help = rand() % 6;
    std::string Operating_location_indicator = (help == 0) ? "B" : ((help == 1) ? "P" : ((help == 2) ? "S" : ((help == 3) ? "C" : ((help == 4) ? "E" : "W"))));
    std::string Number_of_engine_or_propeller_shaft = std::to_string(rand() % 100);  
    std::string Comma = ",";

    return "$--PRC" + Comma + Lever_demand_position + Comma + Lever_demand_status + Comma + PRM_demand_value + Comma + PRM_mode_indicator
    + Comma + Pitch_demand_value + Comma + Pitch_mode_indicator + Comma + Operating_location_indicator + Comma 
    + Number_of_engine_or_propeller_shaft + "*<CR><LF>";  

}

std::string get_prc(PRC_container container)
{
    std::string Lever_demand_position = std::to_string(container.Lever_demand_position);
    std::string Lever_demand_status = container.Lever_demand_status;
    std::string PRM_demand_value =  std::to_string(container.PRM_demand_value);
    std::string PRM_mode_indicator = container.PRM_mode_indicator;
    std::string Pitch_demand_value = std::to_string(container.Pitch_demand_value);
    std::string Pitch_mode_indicator = container.Pitch_mode_indicator;
    std::string Operating_location_indicator = container.Operating_location_indicator;
    std::string Number_of_engine_or_propeller_shaft = std::to_string(container.Number_of_engine_or_propeller_shaft); 
    std::string Comma = ",";

    return "$--PRC" + Comma + Lever_demand_position + Comma + Lever_demand_status + Comma + PRM_demand_value + Comma + PRM_mode_indicator
    + Comma + Pitch_demand_value + Comma + Pitch_mode_indicator + Comma + Operating_location_indicator + Comma 
    + Number_of_engine_or_propeller_shaft + "*<CR><LF>";  
    
}

int parseNMEA_PRC(const std::string& sentence, PRC_container* storage)
{
    short error_index = -1;
    std::istringstream iss(sentence);

    std::string token;
    if (!(std::getline(iss, token, ',') && token.substr(3) == "PRC"))
    {
        LOG(FATAL) << "Token error in " << sentence;
        return error_index;
    }

    std::string Lever_demand_position;
    --error_index;
    if (!(std::getline(iss, Lever_demand_position, ',') && size(Lever_demand_position) != 0 && (Lever_demand_position.find_first_not_of("0123456789.") == Lever_demand_position.npos)))
    {
        LOG(WARNING) << "Lever_demand_position error in " << sentence;
        //std::cout << "Lever_demand_position is " << Lever_demand_position << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->Lever_demand_position = stof(Lever_demand_position);


    std::string Lever_demand_status;
    --error_index;
    if(!(std::getline(iss, Lever_demand_status, ',') && (Lever_demand_status == "A" || Lever_demand_status == "V")))
    {
        LOG(WARNING) << "Lever_demand_status eroor in " << sentence;
        std::cout << "Lever_demand_status is " << Lever_demand_status << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "Lever_demand_status is " << Lever_demand_status << "   " << sentence << std::endl;
    storage->Lever_demand_status = Lever_demand_status;


    std::string PRM_demand_value;
    --error_index;
    if (!(std::getline(iss, PRM_demand_value, ',') && size(PRM_demand_value) != 0 && (PRM_demand_value.find_first_not_of("0123456789.") == PRM_demand_value.npos)))
    {
        LOG(WARNING) << "PRM_demand_value error in " << sentence;
        //std::cout << "PRM_demand_value is " << PRM_demand_value << "   " << sentence << std::endl;
        return error_index;
    } 
    storage->PRM_demand_value = stof(PRM_demand_value);



    std::string PRM_mode_indicator;
    --error_index;
    if(!(std::getline(iss, PRM_mode_indicator, ',') && (PRM_mode_indicator == "P" || PRM_mode_indicator == "R" || PRM_mode_indicator == "V")))
    {
        LOG(WARNING) << "PRM_mode_indicator eroor in " << sentence;
        std::cout << "PRM_mode_indicator is " << PRM_mode_indicator << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "PRM_mode_indicator is " << PRM_mode_indicator << "   " << sentence << std::endl;
    storage->PRM_mode_indicator = PRM_mode_indicator;
    

    std::string Pitch_demand_value;
    --error_index;
    if (!(std::getline(iss, Pitch_demand_value, ',') && size(Pitch_demand_value) != 0 && (Pitch_demand_value.find_first_not_of("0123456789.") == Pitch_demand_value.npos)))
    {
        LOG(WARNING) << "Pitch_demand_value error in " << sentence;
        //std::cout << "Pitch_demand_value is " << Pitch_demand_value << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Pitch_demand_value = stof(Pitch_demand_value); 


    std::string Pitch_mode_indicator;
    --error_index;
    if(!(std::getline(iss, Pitch_mode_indicator, ',') && (Pitch_mode_indicator == "P" || Pitch_mode_indicator == "D" || Pitch_mode_indicator == "V")))
    {
        LOG(WARNING) << "Pitch_mode_indicator eroor in " << sentence;
        std::cout << "Pitch_mode_indicator is " << Pitch_mode_indicator << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "Pitch_mode_indicator is " << Pitch_mode_indicator << "   " << sentence << std::endl;
    storage->Pitch_mode_indicator = Pitch_mode_indicator;


    std::string Operating_location_indicator;
    --error_index;
    if(!(std::getline(iss, Operating_location_indicator, ',') && (Operating_location_indicator == "B" || Operating_location_indicator == "P" || Operating_location_indicator == "S" || Operating_location_indicator == "C" || Operating_location_indicator == "E" || Operating_location_indicator == "W")))
    {
        LOG(WARNING) << "Operating_location_indicator eroor in " << sentence;
        std::cout << "Operating_location_indicator is " << Operating_location_indicator << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "Operating_location_indicator is " << Operating_location_indicator << "   " << sentence << std::endl;
    storage->Operating_location_indicator = Operating_location_indicator;


    std::string Number_of_engine_or_propeller_shaft;
    --error_index;
    if (!(std::getline(iss, Number_of_engine_or_propeller_shaft, '*') && size(Number_of_engine_or_propeller_shaft) != 0 && (Number_of_engine_or_propeller_shaft.find_first_not_of("0123456789.") == Number_of_engine_or_propeller_shaft.npos)))
    {
        LOG(WARNING) << "Number_of_engine_or_propeller_shaft error in " << sentence;
        //std::cout << "Number_of_engine_or_propeller_shaft is " << Number_of_engine_or_propeller_shaft << "   " << sentence << std::endl;
        return error_index;
    }
     storage->Number_of_engine_or_propeller_shaft = stoi(Number_of_engine_or_propeller_shaft); 

    return 1;
}

struct PRM_container
{
    std::string Source;
    unsigned Engine_or_propeller_shaft;
    unsigned Speed;
    unsigned Propeller_pitch;
    std::string Status;
};

std::string generate_PRM()
{
    std::string Source = (rand() % 2) ? "E" : "V";
    std::string Engine_or_propeller_shaft = std::to_string(int(rand() % 100));
    std::string Speed = std::to_string(int(rand() % 100));
    std::string Propeller_pitch = std::to_string(int(rand() % 100));
    std::string Status = (rand() % 2) ? "A" : "V";
    std::string Comma = ",";

    return "$--PRM" + Comma + Source + Comma + Engine_or_propeller_shaft + Comma + Speed + Comma + Propeller_pitch + Comma + Status + 
    "*<CR><LF>";
}


std::string get_PRM(PRM_container container)
{
    std::string Source = container.Source;
    std::string Engine_or_propeller_shaft = std::to_string(container.Engine_or_propeller_shaft);
    std::string Speed = std::to_string(container.Speed);
    std::string Propeller_pitch = std::to_string(container.Propeller_pitch);
    std::string Status = container.Status
    std::string Comma = ",";

    return "$--PRM" + Comma + Source + Comma + Engine_or_propeller_shaft + Comma + Speed + Comma + Propeller_pitch + Comma + Status + 
    "*<CR><LF>";
}

int parseNMEA_PRM(const std::string& sentence, PRM_container storage)
{
    short error_index = -1;
    std::istringstream iss(sentence);

    std::string token;
    if (!(std::getline(iss, token, ',') && token.substr(3) == "PRM"))
    {
        LOG(FATAL) << "Token error in " << sentence;
        return error_index;
    }

    std::string Source;
    --error_index;
    if(!(std::getline(iss, Source, ',') && (Source == "S" || Source == "E")))
    {
        LOG(WARNING) << "Source eroor in " << sentence;
        std::cout << "Source is " << Source << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "Source is " << Source << "   " << sentence << std::endl;
    storage->Source = Source;
    


    std::string Engine_or_propeller_shaft;
    --error_index;
    if (!(std::getline(iss, Engine_or_propeller_shaft, ',') && size(Engine_or_propeller_shaft) != 0 && (Engine_or_propeller_shaft.find_first_not_of("0123456789.") == Engine_or_propeller_shaft.npos)))
    {
        LOG(WARNING) << "Engine_or_propeller_shaft error in " << sentence;
        //std::cout << "Engine_or_propeller_shaft is " << Engine_or_propeller_shaft << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Engine_or_propeller_shaft = stof(Engine_or_propeller_shaft);
    


    std::string Speed;
    --error_index;
    if (!(std::getline(iss, Speed, ',') && size(Speed) != 0 && (Speed.find_first_not_of("0123456789.") == Speed.npos)))
    {
        LOG(WARNING) << "Speed error in " << sentence;
        //std::cout << "Speed is " << Speed << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Speed = stof(Speed);
    


    std::string Propeller_pitch;
    --error_index;
    if (!(std::getline(iss, Propeller_pitch, ',') && size(Propeller_pitch) != 0 && (Propeller_pitch.find_first_not_of("0123456789.") == Propeller_pitch.npos)))
    {
        LOG(WARNING) << "Propeller_pitch error in " << sentence;
        //std::cout << "Propeller_pitch is " << Propeller_pitch << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Propeller_pitch = stof(Propeller_pitch);
    


    std::string Status;
    --error_index;
    if(!(std::getline(iss, Status, '*') && (Status == "A" || Status == "V")))
    {
        LOG(WARNING) << "Status eroor in " << sentence;
        std::cout << "Status is " << Status << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "Status is " << Status << "   " << sentence << std::endl;
    storage->Status = Status;


    return 1;
}

struct ROR_container
{
    unsigned Starboard_rudder_only;
    std::string Status1;
    unsigned Port_rudder_order;
    std::string Status2;
    std::string Command_source_location;
};


std::string generate_ror()
{
    std::string Starboard_rudder_only = std::to_string(rand() % 100);
    std::string Status1 = (rand() % 2) ? "A" : "V";
    std::string Port_rudder_order = std::to_string(rand() % 100);
    std::string Status2 = (rand() % 2) ? "A" : "V";
    std::string Command_source_location = (rand() % 2) ? "A" : "V";
    std::string Comma = ",";
    
    return  "$--ROR" + Comma + Starboard_rudder_only + Status1 + Comma + Port_rudder_order + Comma + Status2 + Comma + 
    Command_source_location + "*<CR><LF>";
}

std::string get_ror(ROR_container container)
{
    std::string Starboard_rudder_only = std::to_string(container.Starboard_rudder_only);
    std::string Status1 = container.Starboard_rudder_only;
    std::string Port_rudder_order = std::to_string(container.Port_rudder_order);
    std::string Status2 = container.Status2;
    std::string Command_source_location = container.Starboard_rudder_only;
    std::string Comma = ",";
    
    return  "$--ROR" + Comma + Starboard_rudder_only + Status1 + Comma + Port_rudder_order + Comma + Status2 + Comma + 
    Command_source_location + "*<CR><LF>";
}



int parseNMEA_ROR(const std::string& sentence, ROR_container * storage)
{
    short error_index = -1;
    std::istringstream iss(sentence);

    std::string token;
    if (!(std::getline(iss, token, ',') && token.substr(3) == "ROR"))
    {
        LOG(FATAL) << "Token error in " << sentence;
        return error_index;
    }

    std::string Starboard_rudder_only;
    --error_index;
    if (!(std::getline(iss, Starboard_rudder_only, ',') && size(Starboard_rudder_only) != 0 && (Starboard_rudder_only.find_first_not_of("0123456789.") == Starboard_rudder_only.npos)))
    {
        LOG(WARNING) << "Starboard_rudder_only error in " << sentence;
        //std::cout << "Starboard_rudder_only is " << Starboard_rudder_only << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Starboard_rudder_only = stof(Starboard_rudder_only);
    

    std::string Status1;
    --error_index;
    if(!(std::getline(iss, Status1, ',') && (Status1 == "A" || Status1 == "V")))
    {
        LOG(WARNING) << "Status1 eroor in " << sentence;
        std::cout << "Status1 is " << Status1 << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "Status1 is " << Status1 << "   " << sentence << std::endl;
    storage->Status1 = Status1;
   


    std::string Port_rudder_order;
    --error_index;
    if (!(std::getline(iss, Port_rudder_order, ',') && size(Port_rudder_order) != 0 && (Port_rudder_order.find_first_not_of("0123456789.") == Port_rudder_order.npos)))
    {
        LOG(WARNING) << "Port_rudder_order error in " << sentence;
        //std::cout << "Port_rudder_order is " << Port_rudder_order << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Port_rudder_order = stof(Port_rudder_order);
    
    
    std::string Status2;
    --error_index;
    if(!(std::getline(iss, Status2, ',') && (Status2 == "A" || Status2 == "V")))
    {
        LOG(WARNING) << "Status2 eroor in " << sentence;
        std::cout << "Status2 is " << Status2 << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "Status2 is " << Status2 << "   " << sentence << std::endl;
    storage->Status2 = Status2;


    std::string Command_source_location;
    --error_index;
    if (!(std::getline(iss, Command_source_location, '*') && size(Command_source_location) != 0 && (Command_source_location.find_first_not_of("0123456789.") == Command_source_location.npos)))
    {
        LOG(WARNING) << "Command_source_location error in " << sentence;
        //std::cout << "Command_source_location is " << Command_source_location << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Command_source_location = Command_source_location;
    
    return 1;
}

std::string generate_rot()
{
    std::string Rate_of_turn = std::to_string(int(rand() % 100));
    std::string Status = (rand() % 2) ? "A" : "V";
    std::string Comma = ",";

    return "$--ROT" + Comma + Rate_of_turn + Comma + Status + "*<CR><LF>"; 
}


std::string get_rot(ROT_conainer container)
{
    std::string Rate_of_turn = std::to_string(container.Rate_of_turn);
    std::string Status = container.Status;
    std::string Comma = ",";

    return "$--ROT" + Comma + Rate_of_turn + Comma + Status + "*<CR><LF>"; 
}


int parseNMEA_ROT(const std::string& sentence, Ship_state * storage)
{
    short error_index = -1;
    std::istringstream iss(sentence);

    std::string token;
    if (!(std::getline(iss, token, ',') && token.substr(3) == "ROT"))
    {
        LOG(FATAL) << "Token error in " << sentence;
        return error_index;
    }

    std::string Rate_of_turn;
    --error_index;
    if (!(std::getline(iss, Rate_of_turn, ',') && size(Rate_of_turn) != 0))
    {
        LOG(WARNING) << "Rate_of_turn error in " << sentence;
        //std::cout << "Rate_of_turn is " << Rate_of_turn << "   " << sentence << std::endl;
        return error_index;
    }
    try
    {
        double test = stof(Rate_of_turn);
    }
    catch (std::invalid_argument& b)
    {
        LOG(WARNING) << "Rate_of_turn error in " << sentence;
        //std::cout << "Rate_of_turn is " << Rate_of_turn << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Rate_of_turn = stof(Rate_of_turn);

    //std::cout << "Rate_of_turn is " << Rate_of_turn << "   " << sentence << std::endl;

    std::string Status;
    --error_index;а
    if (!(std::getline(iss, Status, '*') && (Status == "A" || Status == "V")))
    {
        LOG(WARNING) << "Status eroor in " << sentence;
        //std::cout << "Status is " << Status << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "Status is " << Status << "   " << sentence << std::endl;
    storage->Status = Status;


    return 1;
}

struct RSA_container
(
    unsigned Starboard_rudder_sensor;
    std::string Status1;
    unsigned Port_rudder_sensor;
    std::string Status2;
);


std::string generate_rsa()
{
    std::string Starboard_rudder_sensor = std::to_string(int(rand() % 100));
    std::string Status1 = (rand() % 2) ? "A" : "V";
    std::string Port_rudder_sensor = std::to_string(int(rand() % 100));
    std::string Status2 = (rand() % 2) ? "A" : "V";
    std::string Comma = ",";

    return "$--RSA" + Comma + Starboard_rudder_sensor + Comma + Status1 + Comma + Port_rudder_sensor + Comma + Status2 + "*<CR><LF>";   
}

std::string get_rsa(RSA_container container)
{
    std::string Starboard_rudder_sensor = std::to_string(container.Starboard_rudder_sensor);
    std::string Status1 = container.Status1;
    std::string Port_rudder_sensor = std::to_string(container.Port_rudder_sensor);
    std::string Status2= container.Status2;
    std::string Comma = ",";

    return "$--RSA" + Comma + Starboard_rudder_sensor + Comma + Status1 + Comma + Port_rudder_sensor + Comma + Status2 + "*<CR><LF>";   
}


int parseNMEA_RSA(const std::string& sentence, RSA_container storage)
{
    short error_index = -1;
    std::istringstream iss(sentence);

    std::string token;
    if (!(std::getline(iss, token, ',') && token.substr(3) == "RSA"))
    {
        LOG(FATAL) << "Token error in " << sentence;
        return error_index;
    }

    std::string Starboard_rudder_sensor;
    --error_index;
    if (!(std::getline(iss, Starboard_rudder_sensor, ',') && size(Starboard_rudder_sensor) != 0 && (Starboard_rudder_sensor.find_first_not_of("0123456789.") == Starboard_rudder_sensor.npos)))
    {
        LOG(WARNING) << "Starboard_rudder_sensor error in " << sentence;
        //std::cout << "Starboard_rudder_sensor is " << Starboard_rudder_sensor << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Starboard_rudder_sensor = stof(Starboard_rudder_sensor);
    


    std::string Status1;
    --error_index;
    if(!(std::getline(iss, Status1, ',') && (Status1 == "A" || Status1 == "V")))
    {
        LOG(WARNING) << "Status1 eroor in " << sentence;
        std::cout << "Status1 is " << Status1 << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "Status1 is " << Status1 << "   " << sentence << std::endl;
    storage->Status1 = Status1;
    

    std::string Port_rudder_sensor;
    --error_index;
    if (!(std::getline(iss, Port_rudder_sensor, ',') && size(Port_rudder_sensor) != 0 && (Port_rudder_sensor.find_first_not_of("0123456789.") == Port_rudder_sensor.npos)))
    {
        LOG(WARNING) << "Port_rudder_sensor error in " << sentence;
        //std::cout << "Port_rudder_sensor is " << Port_rudder_sensor << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Port_rudder_sensor = stof(Port_rudder_sensor);
    

    std::string Status2;
    --error_index;
    if(!(std::getline(iss, Status2, '*') && (Status2 == "A" || Status2 == "V")))
    {
        LOG(WARNING) << "Status2 eroor in " << sentence;
        std::cout << "Status2 is " << Status2 << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "Status2 is " << Status2 << "   " << sentence << std::endl;
    storage->Status2 = Status2;

    return 1;
}


struct TRC_container
{
    unsigned Number_of_thruster;
    unsigned PRM_demand_value;
    std::string PRM_mode_indicator;
    unsigned Pitch_demand_value;
    std::string Pitch_mode_indicator;
    unsigned Azimuth_demand;
    std::string Operating_location_indicator;
    std::string Sentense_status_flag;
};

std::string generate_trc()
{
    int help = rand() % 3;
    std::string Number_of_thruster =  std::to_string(int(rand() % 100));
    std::string PRM_demand_value =  std::to_string(int(rand() % 100));
    std::string PRM_mode_indicator =  (help == 0) ? "K" : ((help == 1) ? "N" : "S");
    std::string Pitch_demand_value = std::to_string(int(rand() % 100));
    help = rand() % 3;
    std::string Pitch_mode_indicator = (help == 0) ? "V" : ((help == 1) ? "D" : "P");
    std::string Azimuth_demand = std::to_string(int(rand() % 360));
    help = rand() % 6;
    std::string Operating_location_indicator = (help == 0) ? "B" : ((help == 1) ? "P" : ((help == 2) ? "S" : ((help == 3) ? "C" : ((help == 4) ? "E" : "W"))));;
    std::string Sentense_status_flag = (rand() % 2) ? "C" : "R";
    std::string Comma = ",";

    return "$--TRC" + Comma + Number_of_thruster + Comma + PRM_demand_value + Comma + PRM_mode_indicator + Comma + Pitch_demand_value
    + Comma + Pitch_mode_indicator + Comma + Azimuth_demand + Comma + Operating_location_indicator + Comma + Sentense_status_flag +
    "*<CR><LF>";
}


std::string get_trc(TRC_container container)
{
    std::string Number_of_thruster =  std::to_string(container.Number_of_thruster);
    std::string PRM_demand_value =  std::to_string(container.PRM_demand_value);
    std::string PRM_mode_indicator =  container.PRM_mode_indicator;
    std::string Pitch_demand_value = std::to_string(container.Pitch_demand_value);
    std::string Pitch_mode_indicator = container.PRM_mode_indicator;
    std::string Azimuth_demand = std::to_string(container.Azimuth_demand);
    std::string Operating_location_indicator = container.Operating_location_indicator;
    std::string Sentense_status_flag = container.Sentense_status_flag;
    std::string Comma = ",";

    return "$--TRC" + Comma + Number_of_thruster + Comma + PRM_demand_value + Comma + PRM_mode_indicator + Comma + Pitch_demand_value
    + Comma + Pitch_mode_indicator + Comma + Azimuth_demand + Comma + Operating_location_indicator + Comma + Sentense_status_flag +
    "*<CR><LF>";
}



int parseNMEA_TRC(const std::string& sentence, TRC_container * storage)
{
    short error_index = -1;
    std::istringstream iss(sentence);

    std::string token;
    if (!(std::getline(iss, token, ',') && token.substr(3) == "TRC"))
    {
        LOG(FATAL) << "Token error in " << sentence;
        return error_index;
    }

    std::string Number_of_thruster;
    --error_index;
    if (!(std::getline(iss, Number_of_thruster, ',') && size(Number_of_thruster) != 0 && (Number_of_thruster.find_first_not_of("0123456789.") == Number_of_thruster.npos)))
    {
        LOG(WARNING) << "Number_of_thruster error in " << sentence;
        //std::cout << "Number_of_thruster is " << Number_of_thruster << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Number_of_thruster = stof(Number_of_thruster);
    


    std::string PRM_demand_value;
    --error_index;
    if (!(std::getline(iss, PRM_demand_value, ',') && size(PRM_demand_value) != 0 && (PRM_demand_value.find_first_not_of("0123456789.") == PRM_demand_value.npos)))
    {
        LOG(WARNING) << "PRM_demand_value error in " << sentence;
        //std::cout << "PRM_demand_value is " << PRM_demand_value << "   " << sentence << std::endl;
        return error_index;
    }
    storage->PRM_demand_value = stof(PRM_demand_value);
    


    std::string PRM_mode_indicator;
    --error_index;
    if(!(std::getline(iss, PRM_mode_indicator, ',') && (PRM_mode_indicator == "P" || PRM_mode_indicator == "R" || PRM_mode_indicator == "V")))
    {
        LOG(WARNING) << "PRM_mode_indicator eroor in " << sentence;
        std::cout << "PRM_mode_indicator is " << PRM_mode_indicator << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "PRM_mode_indicator is " << PRM_mode_indicator << "   " << sentence << std::endl;
    storage->PRM_mode_indicator = PRM_mode_indicator;
    


    std::string Pitch_demand_value;
    --error_index;
    if (!(std::getline(iss, Pitch_demand_value, ',') && size(Pitch_demand_value) != 0 && (Pitch_demand_value.find_first_not_of("0123456789.") == Pitch_demand_value.npos)))
    {
        LOG(WARNING) << "Pitch_demand_value error in " << sentence;
        //std::cout << "Pitch_demand_value is " << Pitch_demand_value << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Pitch_demand_value = stof(Pitch_demand_value);
    


    std::string Pitch_mode_indicator;
    --error_index;
    if(!(std::getline(iss, Pitch_mode_indicator, ',') && (Pitch_mode_indicator == "P" || Pitch_mode_indicator == "D" || Pitch_mode_indicator == "V")))
    {
        LOG(WARNING) << "Pitch_mode_indicator eroor in " << sentence;
        std::cout << "Pitch_mode_indicator is " << Pitch_mode_indicator << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "Pitch_mode_indicator is " << Pitch_mode_indicator << "   " << sentence << std::endl;
    storage->Pitch_mode_indicator = Pitch_mode_indicator;
    


    std::string Azimuth_demand;
    --error_index;
    if (!(std::getline(iss, Azimuth_demand, ',') && size(Azimuth_demand) != 0 && (Azimuth_demand.find_first_not_of("0123456789.") == Azimuth_demand.npos)))
    {
        LOG(WARNING) << "Azimuth_demand error in " << sentence;
        //std::cout << "Azimuth_demand is " << Azimuth_demand << "   " << sentence << std::endl;
        return error_index;
    }
    storage->Azimuth_demand = stof(Azimuth_demand);
    


    std::string Operating_location_indicator;
    --error_index;
    if(!(std::getline(iss, Operating_location_indicator, ',') && (Operating_location_indicator == "B" || Operating_location_indicator == "P" || Operating_location_indicator == "S" || Operating_location_indicator == "C" || Operating_location_indicator == "E" || Operating_location_indicator == "W")))
    {
        LOG(WARNING) << "Operating_location_indicator eroor in " << sentence;
        std::cout << "Operating_location_indicator is " << Operating_location_indicator << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "Operating_location_indicator is " << Operating_location_indicator << "   " << sentence << std::endl;
    storage->Operating_location_indicator = Operating_location_indicator;
    


    std::string Sentense_status_flag;
    --error_index;
    if(!(std::getline(iss, Sentense_status_flag, '*') && (Sentense_status_flag == "R" || Sentense_status_flag == "C")))
    {
        LOG(WARNING) << "Sentense_status_flag eroor in " << sentence;
        std::cout << "Sentense_status_flag is " << Sentense_status_flag << "   " << sentence << std::endl;
        return error_index;
    }
    //std::cout << "Sentense_status_flag is " << Sentense_status_flag << "   " << sentence << std::endl;
    storage->Sentense_status_flag = Sentense_status_flag;


    return 1;
}    
