#pragma once

#include <thread>//1
#include <chrono>//2 for tread sleep()
#include "NmeaHandler.h"

#include <iostream>
#include "AsyncQueue.h" // Assuming the async queue class is defined in async_queue.hpp

template<typename Tr>
void consumer_thread(AsyncQueue<Tr> & queue, char name) 
{
    int i = 0;
    GgaMessageHandler ggaHandler;
    RmcMessageHandler rmcHandler;
    VtgMessageHandler vtgHandler;

    // Create NMEA message dispatcher
    NmeaMessageDispatcher dispatcher;
    dispatcher.addHandler(GGA_MESSAGE, &ggaHandler);
    dispatcher.addHandler(RMC_MESSAGE, &rmcHandler);
    dispatcher.addHandler(VTG_MESSAGE, &vtgHandler);
    
    while (true) 
    {
        Tr message = queue.pop();
        i++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // just for test
        //std::chrono::seconds
        dispatcher.dispatch(message);

        std::cout << "Received by "<< name << " #(" << i << ") data: " << message << std::endl;
    }
}
