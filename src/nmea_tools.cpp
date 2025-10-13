# include "nmea_tools.h"


template<> unsigned char CalculateChecksum(const std::string& sentence)
{
    unsigned char checksum = 0;

    // Exclude the starting '$' and ending '*' characters
    size_t startIndex = sentence.find('$') + 1;
    size_t endIndex = sentence.find('*');

    // Calculate the checksum
    for (size_t i = startIndex; i < endIndex; ++i)
    {
        checksum ^= sentence[i];
    }

    return checksum;
}

template<> std::string CalculateChecksum(const std::string& sentence)
{
    int checksum = 0;

    // Exclude the starting '$' and ending '*' characters
    size_t startIndex = sentence.find('$');
    size_t endIndex = sentence.rfind('*');

    if (startIndex == std::string::npos)
    {
        startIndex = 0;
    }
    else
    {
        startIndex += 1; // next char afrer $
    }
    
    
    if (endIndex == std::string::npos)
    {
        endIndex = sentence.length();
    }
    

    // Calculate the checksum
    for (size_t i = startIndex; i < endIndex; ++i)
    {
        checksum ^= sentence[i];
    }

    std::stringstream stream;
    stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << checksum;
    return stream.str();
}

bool CheckChecksum(const std::string& sentence)
{
    size_t endIndex = sentence.find('*');
    unsigned char calcChecksum = CalculateChecksum<unsigned char>(sentence);

    std::string receivedChecksumString = sentence.substr(endIndex + 1, 2);
    unsigned char receivedChecksum = std::stoi(receivedChecksumString, nullptr, 16);

    if (receivedChecksum == calcChecksum)
    {
        return true;
    }
    else
    {
        LOG(ERROR) << "Wrong checksum expect(recived): " << receivedChecksum << "Got(calc): " << calcChecksum ;
        return false;
    }
    
}

#ifndef TALKER_ID
    #define TALKER_ID "U0" // UP -is mcu U# is user defined SN -- pos sys
#endif

std::string formNMEAMessage(const std::string& sentence, const std::string talk_id) 
{
    std::string checksum = CalculateChecksum<std::string>(sentence);

    //return "$" + (std::string)TALKER_ID + sentence + "*" + checksum + "\r\n";
    std::stringstream stream;
    stream << "$" << talk_id << sentence << "*" << checksum;
    return stream.str();
}



#undef TALKER_ID

std::string getCurrentTime() 
{
    // Get the current system time
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);

    // Convert the time to a local time in struct tm
    std::tm time_info;
    localtime_r(&time_t_now, &time_info);

    // Extract the individual time components
    int hours = time_info.tm_hour;
    int minutes = time_info.tm_min;
    int seconds = time_info.tm_sec;
    int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

    // Format the time as hhmmss.ss
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours
        << std::setw(2) << minutes
        << std::setw(2) << seconds
        << '.' << std::setw(2) << milliseconds / 10;

    return oss.str();
}

int convertFloatToInteger5(float value) {
    // Map the float value to the range -5 to +5
    float scaledValue = value * 5;

    // Apply rounding using "round half to even" rule
    int integer = static_cast<int>(std::round(value));

    // Clamp the result to the range -5 to +5
    integer = std::max(-5, std::min(5, integer));

    return integer;

}





