
#ifndef RUBIKSSOLVER_INFOLOGGER_HPP
#define RUBIKSSOLVER_INFOLOGGER_HPP

#include <vector>

class InfoLogger {
public:
	InfoLogger();
	void logg(const std::vector<char> &moves);
	void incrementStates();
private:
	int _counter = 0;
	std::chrono::system_clock::time_point _prevPrintTime;

	bool _hasStartTime = false;
	std::chrono::system_clock::time_point _startTime;
	double _startProgress;
	unsigned long long _statesVisited = 0;
};


#endif //RUBIKSSOLVER_INFOLOGGER_HPP