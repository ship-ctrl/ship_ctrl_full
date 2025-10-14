///test Parsers
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Parsers_TestSuite
#include <boost/test/included/unit_test.hpp>
//#include <boost/test/unit_test.hpp>

#include <iostream>
#include <cmath>
#include <allHandlers.h>
#include <nmea_tools.h>

BOOST_AUTO_TEST_SUITE(ParserTests)

BOOST_AUTO_TEST_CASE(GAA_in_Test)
{
    //Calculator calc;
    GNSSData navD;
    int rc = parseNMEA_GAA("$GPGGA,202530.00,5109.0262,N,11401.8407,W,5,40,0.5,1097.36,M,-17.00,M,18,TSTR*61", &navD);
    double dif = std::fabs(51.0 + 9.0262/60 - navD.getLatitude());
    BOOST_CHECK_EQUAL( rc , 0);

    BOOST_CHECK_MESSAGE( dif < .0001, " dif calc is " << dif << " and should be less than" << .0001 << " regained lat is" << navD.getLatitude());
    //BOOST_CHECK_EQUAL( std::fabs(51.0d + 9.0262/60 - navD.getLatitude()) < .0001 , true);
}

BOOST_AUTO_TEST_CASE(GAA_in_S_Test)
{
    //Calculator calc;
    GNSSData navD;
    int rc = parseNMEA_GAA("$GPGGA,202530.00,5109.0262,S,11401.8407,W,5,40,0.5,1097.36,M,-17.00,M,18,TSTR*61", &navD);
    double dif = std::fabs(-51.0 - 9.0262/60 - navD.getLatitude());
    BOOST_CHECK_EQUAL( rc , 0);

    BOOST_CHECK_MESSAGE( dif < .0001, " dif calc is " << dif << " and should be less than" << .0001 << "regained lat is" << navD.getLatitude());
    //BOOST_CHECK_EQUAL( std::fabs(51.0d + 9.0262/60 - navD.getLatitude()) < .0001 , true);
}

BOOST_AUTO_TEST_CASE(ETL_in_Test)
{
    //Calculator calc;
    BOOST_CHECK_EQUAL( 2 - 1 , 1);
}

BOOST_AUTO_TEST_SUITE_END()