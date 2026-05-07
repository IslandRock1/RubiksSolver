
#include <iostream>
#include <chrono>
#include <cmath>

#include "RubiksLibrary/InfoLogger.hpp"

InfoLogger::InfoLogger() {
	_prevPrintTime = std::chrono::high_resolution_clock::now();
}

void InfoLogger::incrementStates() {
	_statesVisited++;
}

double progressPercent(const std::vector<char>& state) {
	constexpr int base = 18;
	constexpr unsigned long long totalStates = 612220032ULL; // 18^7

	unsigned long long value = 0;
	for (size_t i = 0; i < 7; ++i) {
		int digit = 0;
		if (i < state.size()) {
			digit = state[i] - 'A'; // 'A' -> 0, 'B' -> 1, ... 'R' -> 17
			if (digit < 0 || digit >= base) {
				throw std::runtime_error("Invalid character in state");
			}
		}
		value = value * base + digit;
	}

	return (100.0 * value) / (totalStates - 1);
}

void InfoLogger::logg(const std::vector<char>& moves) {
	_counter++;
	if (_counter % 1000 != 0) { return; }
	_counter = 0;

	const auto now = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - _prevPrintTime).count();

	if (duration < 500) { return; }
	_prevPrintTime = now;

	// --- Progress ---
	double percent = progressPercent(moves);

	double elapsedSec = duration;
	double progressSinceLastIter = percent - _prevProgress;
	double percentPerMS = progressSinceLastIter / static_cast<double>(duration);
	double remainingMilliSecEstimate = elapsedSec * (100.0 - percent) / percentPerMS;
	double remainingSecEstimate = remainingMilliSecEstimate / 1000.0;
	_prevProgress = percent;

	double statesPerSec = 1000.0 * static_cast<double>(_statesVisited) / static_cast<double>(duration);
	_statesVisited = 0;

	// --- Output ---
	// --- Print everything with fixed widths ---
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Progress: " << std::setw(6) << percent << "% | ";
	std::cout << "States/sec: " << std::setw(10) << statesPerSec << " | ";

	// ETA formatting
	if (remainingSecEstimate >= 0.0) {
		int h = static_cast<int>(remainingSecEstimate / 3600.0);
		int m = static_cast<int>(std::fmod(remainingSecEstimate / 60.0, 60.0));
		int s = static_cast<int>(std::fmod(remainingSecEstimate, 60.0));

		std::cout << "ETA: ";
		std::cout << std::setw(3) << std::setfill('0') << h << "h ";
		std::cout << std::setw(2) << std::setfill('0') << m << "m ";
		std::cout << std::setw(2) << std::setfill('0') << s << "s | ";
	} else {
		std::cout << "ETA: --h --m --s | ";
	}

	// Moves printing with consistent padding
	std::cout << "Moves: ";
	for (size_t i = 0; i < moves.size(); i++) {
		std::cout << moves[i];
		if (i != moves.size() - 1) std::cout << " | ";
	}

	// Fill remaining space so line length stays roughly the same
	std::cout << std::string(20, ' ') << "\r";
	std::cout.flush();
}