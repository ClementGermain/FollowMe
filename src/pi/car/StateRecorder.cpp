#include <iostream>
#include <fstream>
#include "StateRecorder.hpp"
#include "car/Car.hpp"
#include "improc/UserDetectionThread.hpp"
#include "improc/RoadDetectionThread.hpp"

using namespace std;
using namespace std::chrono;

StateRecorder stateRecorder(MAX_RECORDING_DURATION, RECORDER_SAMPLE_PERIOD);

StateRecorder::StateRecorder(unsigned int maxRecordingMilliseconds, int samplePeriodMillis) :
	PeriodicThread(samplePeriodMillis, "State Recorder"),
	maxSamplesCount(maxRecordingMilliseconds/samplePeriodMillis),
	firstSampleIndex(0),
	samplePeriod(samplePeriodMillis),
	dateStart(system_clock::now())
{
	samplesBuffer.reserve(maxSamplesCount);
	newSample.reserve(300);
}

void StateRecorder::begin() {
	dateStart = system_clock::now();
	logCursor = Log.getCursor(true);
}

void StateRecorder::loop() {
	double time = duration_cast<milliseconds>(system_clock::now() - dateStart).count() / 1000.0;

	// Control
	BarstowControl_Typedef control;
	Car::getControlStructure(control);
	float cmdMotP = control.propulsionMotor.speed * control.propulsionMotor.direction;
	float cmdMotD = control.directionMotor.speed * control.directionMotor.direction;
	int cmdGyro = control.gyro;

	// Model
	BarstowModel_Typedef model;
	Car::getModelStructure(model);
	uint32_t * modMotPL = &model.leftWheelMotor.current;
	uint32_t * modMotPR = &model.rightWheelMotor.current;
	uint32_t * modMotD = &model.directionMotor.current;
	uint32_t modEOCL = model.leftEocSensor.endOfCourse;
	uint32_t modEOCR = model.rightEocSensor.endOfCourse;

	// User
	float userDist = userDetectionThread.detector.getDistance();
	float userDir = userDetectionThread.detector.getDirection() * 180 / M_PI;
	bool userDetect = userDetectionThread.detector.isDetected();
	bool userVis = userDetectionThread.detector.isVisible();

	// Road
	float roadUserDist = roadDetectionThread.detector.Target.y * 180 / M_PI;
	float roadUserDir = roadDetectionThread.detector.Target.x;

	// Logs
	string concatenatedLogs;
	while(Log.hasNext(logCursor)) {
		LogLine ll(Log.readNext(logCursor));
		// get line and remove double quotes
		string l(ll.formatedString(true));
		for(int i = l.size(); --i >= 0;)
			if(l[i] == '\"')
				l[i] = '\'';
		concatenatedLogs += "\""+l+"\" ";
	}

	// Put all data in one string
	newSample.clear();
	newSample += std::to_string(time) + " ";
	newSample += std::to_string(cmdMotP) + " ";
	newSample += std::to_string(cmdMotD) + " ";
	newSample += std::to_string(cmdGyro) + " ";
	newSample += std::to_string(modMotPL[0]) + " ";
	newSample += std::to_string(modMotPL[1]) + " ";
	newSample += std::to_string(modMotPL[2]) + " ";
	newSample += std::to_string(modMotPR[0]) + " ";
	newSample += std::to_string(modMotPR[1]) + " ";
	newSample += std::to_string(modMotPR[2]) + " ";
	newSample += std::to_string(modMotD[0]) + " ";
	newSample += std::to_string(modMotD[1]) + " ";
	newSample += std::to_string(modMotD[2]) + " ";
	newSample += std::to_string(modEOCL) + " ";
	newSample += std::to_string(modEOCR) + " ";
	newSample += std::to_string(userDist) + " ";
	newSample += std::to_string(userDir) + " ";
	newSample += std::to_string(userDetect) + " ";
	newSample += std::to_string(userVis) + " ";
	newSample += std::to_string(roadUserDist) + " ";
	newSample += std::to_string(roadUserDir) + " ";
	newSample += concatenatedLogs + " ";

	// push new sample in the cyclic buffer
	if((int)samplesBuffer.size() >= maxSamplesCount) {
		// case of buffer is full, erase the first sample and consider that it is now the last one
		samplesBuffer[firstSampleIndex] = newSample;
		firstSampleIndex = (firstSampleIndex+1) % maxSamplesCount;
	}
	else {
		// buffer is not full, push the new sample at the end
		samplesBuffer.push_back(newSample);
	}
}

void StateRecorder::save(string const& name) {
	// Create a new filename with date format: "../../out/filename_HH-MM-SS.sss.csv"
	string filename("../../out/"+name+"_");
	char time[18];
	system_clock::time_point date(system_clock::now());
	int ms = duration_cast<milliseconds>(date.time_since_epoch()).count() % 1000;
	time_t rawtime = system_clock::to_time_t(date);
	size_t len = strftime(time, 9, "%H-%M-%S", localtime(&rawtime));
	snprintf(time+len, 17, ".%03d.csv", ms);
	filename += time;
	
	cout << "Save records in \"" << filename << "\"." << endl;

	ofstream file(filename);

	// write column names
	file << "Time(s) CmdMotP(1) CmdMotD(1) CmdGyro(bool) ModMotPLCur(mA) ModMotPLVolt1(mV) ModMotPLVolt2(mV) ModMotPRCur(mA) ModMotPRVolt1(mV) ModMotPRVolt2(mV) ModMotDCur(mA) ModMotDVolt1(mV) ModMotDVolt2(mV) ModEOCL(bool) ModEOCR(bool) UserDist(m) UserDir(deg) UserDetect(bool) UserVis(bool) RoadUserDist(m) RoadUserDir(deg) Logs" << endl;

	// write Samples
	for(int i = 0; i < (int) samplesBuffer.size(); i++)
		file << samplesBuffer[i%samplesBuffer.size()] << endl;

	file.close();
}


