#pragma once

#include "GNSSData.h"
#include "containers.hpp"

/// @brief class to store ship state for both comands and answers
class Ship_state
{
private:
    GNSSData Nav; //lon lat alt heading speed from GNSS

    double heading; // if other sourse store the filtred here.
    double speed;
    double accel[3]; //x y z if accel present (may be useful to video stab)

    double trust_factor_target;
    double trust_factor_state;
    
    double rudder_target;
    double rudder_state;
public:
    Ship_state() = default;
    virtual ~Ship_state() = default;

    double get_trust_factor_target(){return trust_factor_target;}
    double get_trust_factor_state(){return trust_factor_state;}

    double get_rudder_target(){return rudder_target;}
    double get_rudder_state(){return rudder_state;}

    double get_heading(){return heading;}
    double get_speed(){return speed;}
    double * get_accel(){return &accel[0];}
    
};


