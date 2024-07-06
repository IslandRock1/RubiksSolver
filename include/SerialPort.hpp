//
// Stolen directly from github on 06/07/24
// https://github.com/manashmandal/SerialPort.git

#ifndef RUBIKSSOLVER_SERIALPORT_HPP
#define RUBIKSSOLVER_SERIALPORT_HPP

#pragma once

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#include <windows.h>
#include <iostream>
#include <vector>

struct Data {
    bool status;
    std::string data;
};

class SerialPort {
private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;

    Data waitForMoves();
    Data sendData(const char *data);
    Data recieveData();
public:
    explicit SerialPort(const char *portName);
    ~SerialPort();

    int readSerialPort(const char *buffer, unsigned int buf_size);
    bool writeSerialPort(const char *buffer, unsigned int buf_size);
    bool isConnected();
    void closeSerial();

    std::vector<char> getMoves();
    void sendMoves(std::vector<char> moves);
};


#endif //RUBIKSSOLVER_SERIALPORT_HPP
