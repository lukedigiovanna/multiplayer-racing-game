:: Build file for windows

@echo off
cd ../../
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make