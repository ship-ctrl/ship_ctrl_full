/// @brief ETL message parse / generate / form instructions
/// @author @Tommy0x121

#pragma once

#include "nmea_ETL_parser.hpp"



uint8_t parseNMEA_ETL(const std::string& sentence, ETL_container* container)
{
    uint8_t error_index = -1;
    std::istringstream iss(sentence);

    //std::string token;

    ///@note on this point U0ETL (talkerID and msg type are already cuted while determining a parser object at nmea_handler common class)
    //if (!(std::getline(iss, token, ',') && token.substr(3) == "ETL"))
    //{
        //LOG(FATAL) << "Token error in " << sentence;
        //return error_index;
    //}

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

    std::string msg_type;
    --error_index;

    if (!(std::getline(iss, msg_type, ',') && (msg_type == "O" || msg_type == "A")))
    {
        LOG(WARNING)<< "Message type error in " << sentence;
        return error_index;
    }
    container->msg_type = msg_type;

    std::string position_indicator;
    --error_index;
    if (!(std::getline(iss, position_indicator, ',') && (position_indicator[0] == '0' || position_indicator[0] == '1') && std::stoi(position_indicator.substr(1)) > 0 && std::stoi(position_indicator.substr(1)) < 6))
    {
        LOG(WARNING) << "Position indicator of engine telegraph error in " << sentence;
        return error_index;
    }
    container->eng_tel = position_indicator;

    std::string sub_telegraph_position;
    --error_index;
    if (!(std::getline(iss, sub_telegraph_position, ',') && (std::stoi(sub_telegraph_position) == 20 || std::stoi(sub_telegraph_position) == 30 || std::stoi(sub_telegraph_position) == 40)))
    {
        LOG(WARNING) << "Position indicator of sub telegraph position error" << sentence;
        return error_index;
    }
    container->sub_tel = stoi(sub_telegraph_position);

    std::string op_location;
    --error_index;
    if (!(std::getline(iss, op_location, ',') && (size(op_location) == 0 || size(op_location) == 1 && (op_location[0] == 'B' || op_location[0] == 'C' || op_location[0] == 'P' || op_location[0] == 'S' || op_location[0] == 'E' || op_location[0] == 'W'))))
    {
        LOG(WARNING) << "Opertaing location indicator error in " << sentence;
        return error_index;
    }
    container->op_location = op_location;

    std::string Number;
    --error_index;
    if (!(std::getline(iss, Number, '*') && (Number == "Odd" || Number == "Even" || Number == "0")))
    {
        LOG(WARNING) << "Number  of engine or propeller shaft error in " << sentence;
        return error_index;
    }
    container -> shaft_num = stoi(Number);

    return 1;
}

std::string get_ETL(const ETL_container& container)
{
    std::string hh = std::to_string(container.hh); if (size(hh) == 1) hh = "0" + hh;
    std::string mm = std::to_string(container.mm); if (size(mm) == 1) mm = "0" + mm;
    std::string ss = std::to_string(container.ss); if (size(ss) == 1) ss = "0" + ss;
    std::string ms = std::to_string(container.ms); if (size(ms) == 1) ms = "0" + ms;
    std::string event_time = hh + mm + ss + "." + ms;
    std::string msg_type = container.msg_type;
    std::string eng_tel = container.eng_tel;
    std::string sub_tel = std::to_string(container.sub_tel);
    std::string op_location = container.op_location;
    std::string shaft_num = std::to_string(container.shaft_num);
    std::string comma = ",";
    return "ETL" + comma + event_time + comma + msg_type + comma + eng_tel + comma
        + sub_tel + comma + op_location + comma + shaft_num + comma;
}


std::string generate_ETL()
{
    std::string hh = std::to_string(rand() % 24); if (size(hh) == 1) hh = "0" + hh;
    std::string mm = std::to_string(rand() % 60); if (size(mm) == 1) mm = "0" + mm;
    std::string ss = std::to_string(rand() % 60); if (size(ss) == 1) ss = "0" + ss;
    std::string ms = std::to_string(rand() % 100); if (size(ms) == 1) ms = "0" + ms;
    std::string event_time = hh + mm + ss + "." + ms;
    // O — order, A — answerback
    std::string msg_type = (rand() % 2) ? "O" : "A";
    std::string eng_tel = std::to_string(rand() % 2) + std::to_string(rand() % 5 + 1);
    std::string sub_tel = std::to_string(rand() % 3 + 2) + "0";
    std::string help[7] = { "B", "P", "S", "C", "E", "W", " "};
    std::string op_location =  help[rand() % 7];
    std::string shaft_num = std::to_string(rand() % 10);
    std::string comma = ",";
    return "ETL" + comma + event_time + comma + msg_type + comma + eng_tel + comma
        + sub_tel + comma + op_location + comma + shaft_num + comma;
}