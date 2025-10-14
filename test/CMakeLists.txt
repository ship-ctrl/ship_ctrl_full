project(tests)

find_package (Boost COMPONENTS unit_test_framework REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(test1_source test1.cpp)
set(test2_source nmea_tools_test.cpp)
set(test3_source parsers_test.cpp)

add_executable(test1 ${test1_source})
add_executable(nmea_tools_test ${test2_source})
add_executable(parsers_test ${test3_source})


target_link_libraries(test1
    ${Boost_LIBRARIES}
)

target_link_libraries(nmea_tools_test ${Boost_LIBRARIES} nmea_tools)

target_link_libraries(parsers_test
    ${Boost_LIBRARIES}
    glog::glog
    nmea_tools
)

add_test(NAME Test1 COMMAND test1)
add_test(NAME nmea_tools_test COMMAND nmea_tools_test)
add_test(NAME parsers_test COMMAND parsers_test)


# add_test(testTkInter "/usr/bin/python" "-c" "import Tkinter")