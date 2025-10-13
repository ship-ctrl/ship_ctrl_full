#pragma once

#define GOOGLE_STRIP_LOG 1    // this must go before the #include!
//#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <sstream>
#include <iostream>


struct RSA_container
{
    uint16_t Starboard_rudder_sensor;
    std::string Status1;
    uint16_t Port_rudder_sensor;
    std::string Status2;
};


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

