#ifndef __STATE_RECORDER_HPP__
#define __STATE_RECORDER_HPP__

#define MAX_RECORDING_DURATION	(45*1000) // Recorder keeps the last 45 sec
#define RECORDER_SAMPLE_PERIOD (25) // Create sample every 25 ms

#include "utils/PeriodicThread.hpp"
#include "utils/Log.hpp"
#include <string>
#include <vector>
#include <chrono>

/**
 * This class is an observer that save continously everything that happens in the car.
 * It save commands, sensors values, user and road detection, logs, and even more in csv text format.
 */
class StateRecorder : public PeriodicThread {
	public:
		//! Configure the sampling rate and the maximum time to remember
		StateRecorder(unsigned int maxRecordingMilliseconds, int samplePeriodMillis);
		//! save the recorded value in a file. The file name is automatically prepended with ../../out/ and appended with a timestamp and '.csv'
		void save(std::string const& filename="records");
	private:
		void loop();
		void begin();

		int maxSamplesCount;
		int firstSampleIndex;
		int samplePeriod;
		std::vector<std::string> samplesBuffer;
		std::chrono::system_clock::time_point dateStart;
		LogStream::Cursor logCursor;
		std::string newSample;
};

extern StateRecorder stateRecorder;

#endif

