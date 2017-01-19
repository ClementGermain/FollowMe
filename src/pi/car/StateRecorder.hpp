#ifndef __STATE_RECORDER_HPP__
#define __STATE_RECORDER_HPP__

#define MAX_RECORDING_DURATION	(45*1000) // Recorder keeps the last 45 sec
#define RECORDER_SAMPLE_PERIOD (25) // Create sample every 25 ms

#include "utils/PeriodicThread.hpp"
#include "utils/Log.hpp"
#include <string>
#include <vector>
#include <chrono>

class StateRecorder : public PeriodicThread {
	public:
		StateRecorder(unsigned int maxRecordingMilliseconds, int samplePeriodMillis);
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

