# ship_ctrl_full
[![CMake](https://github.com/ship-ctrl/ship_ctrl_full/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/ship-ctrl/ship_ctrl_full/actions/workflows/cmake.yml)

assemble multiple prjs to finaly become ship ctrl app

# prepare

install libboost-all
```bash 
sudo apt-get install libboost-all-dev
``` 

install glog

```bash
git clone https://github.com/google/glog.git
cd glog

cmake -S . -B build -G "Unix Makefiles"

cmake --build build

cmake --build build --target test

sudo cmake --build build --target install
```

building main code

```bash

cmake -S . -B build -G "Unix Makefiles"
cmake --build build

cmake --build build --target test
```
you should probably run tests on main

______

# Project structure

- [NMEA_COM]() — main comm module currently only accepting and distribute incoming messages
- [handler](https://github.com/ship-ctrl/ship_ctrl_full/blob/ship-ctrl/issue11/src/NmeaHandler.cpp) — gets propper handler for every message based on it's content
- [tools]() — common methods to form/check/fill msgs
- Msg's parsers and containers — see standard for more description 
