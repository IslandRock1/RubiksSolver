//
// Stolen directly from github
// https://github.com/manashmandal/SerialPort.git

#include "SerialPort.hpp"

SerialPort::SerialPort(const char *portName)
{
    this->connected = false;

    this->handler = CreateFileA(static_cast<LPCSTR>(portName),
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);
    if (this->handler == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            std::cerr << "ERROR: Handle was not attached.Reason : " << portName << " not available\n";
        }
        else
        {
            std::cerr << "ERROR!!!\n";
        }
    }
    else
    {
        DCB dcbSerialParameters = {0};

        if (!GetCommState(this->handler, &dcbSerialParameters))
        {
            std::cerr << "Failed to get current serial parameters\n";
        }
        else
        {
            dcbSerialParameters.BaudRate = CBR_115200;
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                std::cout << "ALERT: could not set serial port parameters\n";
            }
            else
            {
                this->connected = true;
                PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(ARDUINO_WAIT_TIME);
            }
        }
    }
}

SerialPort::~SerialPort()
{
    if (this->connected)
    {
        this->connected = false;
        CloseHandle(this->handler);
    }
}

// Reading bytes from serial port to buffer;
// returns read bytes count, or if error occurs, returns 0
int SerialPort::readSerialPort(const char *buffer, unsigned int buf_size)
{
    DWORD bytesRead{};
    unsigned int toRead = 0;

    ClearCommError(this->handler, &this->errors, &this->status);

    if (this->status.cbInQue > 0)
    {
        if (this->status.cbInQue > buf_size)
        {
            toRead = buf_size;
        }
        else
        {
            toRead = this->status.cbInQue;
        }
    }

    memset((void*) buffer, 0, buf_size);

    if (ReadFile(this->handler, (void*) buffer, toRead, &bytesRead, NULL))
    {
        return bytesRead;
    }

    return 0;
}

// Sending provided buffer to serial port;
// returns true if succeed, false if not
bool SerialPort::writeSerialPort(const char *buffer, unsigned int buf_size)
{
    DWORD bytesSend;

    if (!WriteFile(this->handler, (void*) buffer, buf_size, &bytesSend, 0))
    {
        ClearCommError(this->handler, &this->errors, &this->status);
        return false;
    }

    return true;
}

// Checking if serial port is connected
bool SerialPort::isConnected()
{
    if (!ClearCommError(this->handler, &this->errors, &this->status))
    {
        this->connected = false;
    }

    return this->connected;
}

void SerialPort::closeSerial()
{
    CloseHandle(this->handler);
}

Data SerialPort::recieveData() {
    Data data;

    char recievedData[255];
    data.status = this->readSerialPort(recievedData, 255);
    data.data = recievedData;

    return data;
}

Data SerialPort::sendData(const char *data) {
    Data dataStruct;
    dataStruct.status = this->writeSerialPort(data, 255);
    dataStruct.data = data;

    return dataStruct;
}

bool isSubString(std::string mainString, std::string subString) {
    auto lMain = mainString.length();
    auto lSub = subString.length();

    for (int i = 0; i + lSub + 1 < lMain; i++) {
        // std::cout << i << " | " << l - (subString.length() + 1) << "\n";
        // std::cout << "I: " << i << " | L: " << lSub << " | sub: " << mainString.substr(i, lSub) << "\n";
        if (mainString.substr(i, lSub) == subString) {
            return true;
        }
    }

    return false;
}

Data SerialPort::waitForMoves() {
    while (true) {
        auto data = recieveData();

        if (!data.status) {
            continue;
        }

        if (isSubString(data.data, "SSS")) {
            return data;
        }
    }
}

void removePreAndTrailingS(Data &data) {
    while (true) {
        if (data.data[0] == 'S') {
            data.data.erase(data.data.begin());
        } else {
            break;
        }
    }

    while (true) {
        if (data.data[data.data.length() - 1] == 'S') {
            data.data.erase(data.data.end());
        } else {
            break;
        }
    }
}

std::vector<char> SerialPort::getMoves() {
    auto data = waitForMoves();
    removePreAndTrailingS(data);

    std::vector<char> out;
    for (auto &c : data.data) {
        out.push_back(c);
    }

    return out;
}

void SerialPort::sendMoves(std::vector<char> moves) {
    moves.push_back('S');
    moves.push_back('S');
    moves.push_back('S');
    moves.push_back('\0');
    std::cout << "Moves: " << moves.data() << " | End moves.\n";

    // waitForEspReady();
    std::cout << "Esp ready\n";
    sendData(moves.data());
}