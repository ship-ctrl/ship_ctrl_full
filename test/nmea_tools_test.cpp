///test Nmea tools Checksumm 1 & 2, form
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE NmeaChecksum_test
#include <boost/test/included/unit_test.hpp>
//#include <boost/test/unit_test.hpp>

#include <iostream>
#include "Message.hpp"

BOOST_AUTO_TEST_SUITE(NmeaToolsTests)

BOOST_AUTO_TEST_CASE(FiniteMSG_checksumm_Test)
{
    /// @todo implement
    RMCMessage msg;
    BOOST_CHECK_EQUAL( msg.CalculateChecksum<unsigned char>("$GPRMC,222548.00,V,5959.4246,N,03019.1743,E,0.0000,0.000,310523,11.1,E*4D") , 0x4D);
}

BOOST_AUTO_TEST_CASE(EmptyMSG_checksumm_Test)
{
    /// @todo implement
    VTGMessage msg;
    BOOST_CHECK_EQUAL( msg.CalculateChecksum<unsigned char>("$GPVTG,089.0,T,,,15.2,N,,,*53") , 0x53);
}

BOOST_AUTO_TEST_CASE(FiniteMSG_checksumm_Teststr)
{
    /// @todo implement
    RMCMessage msg;
    std::string s = msg.CalculateChecksum<std::string>("$GPRMC,222548.00,V,5959.4246,N,03019.1743,E,0.0000,0.000,310523,11.1,E*4D");
    char  c[] = "4D";
    //Calculator calc;
    BOOST_CHECK_MESSAGE( strcmp(s.c_str() , c)==0, "calc is " << s << " and should be " << c);
}

BOOST_AUTO_TEST_CASE(EmptyMSG_checksumm_Teststr)
{
    /// @todo implement
    GSAMessage msg;
    std::string s = msg.CalculateChecksum<std::string>("$GPGSA,A,1,,,,,,,,,,,,,,,,*32");
    char  c[] = "32";
    //strcmp(s.c_str(),c)==0
    BOOST_CHECK_MESSAGE( strcmp(s.c_str() , c)==0, "calc is " << s << " and should be " << c);
}

BOOST_AUTO_TEST_CASE(FiniteMSG_form_Test_str)
{
    //std::string st = "GAA,123,45,6,7";
    /// @todo implement and may-be rewrite this test to meet new paradigm
    GAAMessage msg;
    std::string s = msg.formNMEAMessage("GAA,12,45,6,7,15");
    char  c[] = "$U0GAA,12,45,6,7,15*6C";
    //Calculator calc;
    BOOST_CHECK_MESSAGE( strcmp(s.c_str() , c)==0, "calc is " << s << " and should be " << c);
}

BOOST_AUTO_TEST_CASE(EmptyMSG_form_Test_str)
{
    //std::string st = "GAA,123,,,";
    /// @todo implement and may-be rewrite this test to meet new paradigm
    GAAMessage msg;
    std::string s = msg.formNMEAMessage("GAA,123,,,");
    char  c[] = "$U0GAA,123,,,*77";
    //strcmp(s.c_str(),c)==0
    BOOST_CHECK_MESSAGE( strcmp(s.c_str() , c)==0, "calc is " << s << " and should be " << c);
}

BOOST_AUTO_TEST_CASE(FiniteMSG_form_talker_Test_str)
{
    //std::string st = "GAA,123,45,6,7";
    std::string s = formNMEAMessage("GAA,12,45.66,6,7,15","U1");
    char  c[] = "$U1GAA,12,45.66,6,7,15*42";
    //Calculator calc;
    BOOST_CHECK_MESSAGE( strcmp(s.c_str() , c)==0, "calc is " << s << " and should be " << c);
}

BOOST_AUTO_TEST_CASE(EmptyMSG_form_talker_Test_str)
{
    //std::string st = "GAA,123,,,";
    std::string s = formNMEAMessage("GAA,12,,,","U1");
    char  c[] = "$U1GAA,12,,,*44";
    //strcmp(s.c_str(),c)==0
    BOOST_CHECK_MESSAGE( strcmp(s.c_str() , c)==0, "calc is " << s << " and should be " << c);
}

BOOST_AUTO_TEST_CASE(CurrentTime)
{
    std::string currentTime = getCurrentTime();
    char  c[] = "000000.00";
    BOOST_CHECK_MESSAGE( strcmp(currentTime.c_str() , c)!=0, "calc is " << currentTime << " and shouldn't be " << c);
    //std::cout << "Current time: " << currentTime << std::endl;
}
BOOST_AUTO_TEST_CASE(floatintconv)
{
    int i = convertFloatToInteger5(5.0f);
    BOOST_CHECK_MESSAGE( i==5, "calc is " << i << " and shouldn't be " << 5);
}
BOOST_AUTO_TEST_SUITE_END()