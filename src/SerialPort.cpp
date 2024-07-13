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

    std::string out;
    for (auto &val : recievedData) {
        if (isalnum(val)) {
            out.push_back(val);
        }
    }

    if (!out.empty()) {
        std::cout << out << "\n";
    }

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

bool formatDataAttemptTwo(Data &data) {
    bool seenS = false;
    int countS = 0;
    while (!data.data.empty()) {

        if (seenS and (data.data[0] != 'S') and (countS == 3)) {
            break;
        }

        if (data.data[0] == 'S') {
            seenS = true;
            countS++;
        } else {
            countS = false;
        }

        data.data.erase(data.data.begin());
    }


    // Should be left with string like ABSDJHDSSSKFHKHBFNSSS
    std::string out;
    while ((!data.data.empty()) and (data.data[0] != 'S')) {
        out.push_back(data.data[0]);
        data.data.erase(data.data.begin());
    }

    auto ends = data.data[0] == 'S';
    data.data = out;
    return ends;
}

bool removePreAndTrailingS(Data &data) {
    std::string out;
    bool began = false;
    int countStart = 0;
    int countEnd = 0;
    for (auto &c : data.data) {
        if (began) {
            if (c != 'S') {
                out.push_back(c);
                countEnd = 0;
            } else {
                countEnd++;
                if (countEnd == 2) {
                    break;
                }
            }
        } else {
            if (c == 'S') {
                countStart++;

                if (countStart == 2) {
                    began = true;
                }
            }
        }
    }

    data.data = out;
    return !out.empty();
}

std::vector<char> SerialPort::getMoves() {

    Data data;
    while (true) {
        data = waitForMoves();
        auto correctFormat = formatDataAttemptTwo(data);

        if (correctFormat) {
            break;
        }
    }

    std::vector<char> out;
    for (auto &c : data.data) {
        out.push_back(c);
    }

    return out;
}

void SerialPort::sendMoves(std::vector<char> moves) {
    std::vector<char> chars = {'S', 'S', 'S'};
    for (auto val : moves) {
        chars.push_back(val);
    }

    chars.push_back('S'); //ESP is using "SSS" as end terminator
    chars.push_back('S');
    chars.push_back('S');
    chars.push_back('\0'); // Null terminator??

    std::cout << "Moves: " << chars.data() << " | End moves.\n";
    auto data = sendData(chars.data());

    if (!data.status) {
        std::cout << "Failed to send data" << "\n";
    }
}