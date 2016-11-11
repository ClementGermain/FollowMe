# OpenCV LD flags
OCV_LDFLAGS=-lopencv_highgui -lopencv_core -lopencv_legacy -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_imgproc
# Raspicam_cv LD flags
RASPICAM_CV_DIR=/home/pi/raspicam_cv/robidouille/raspicam_cv
USERLAND_LIB=/home/pi/raspicam_cv/userland/build/lib
RCAM_LDFLAGS=-L$(USERLAND_LIB) -lmmal_core -lmmal -lmmal_util -lvcos -lbcm_host \
		   -L$(RASPICAM_CV_DIR) -lraspicamcv -lpthread
# Raspicam_cv C++ flags
RCAM_CXXFLAGS=-I$(RASPICAM_CV_DIR)
# SDL flags
SDL_FLAGS=-lSDL -lSDLmain -lSDL_gfx

CXXFLAGS=-Wall -std=c++11 $(RCAM_CXXFLAGS) -MD
LDFLAGS=$(OCV_LDFLAGS) $(RCAM_LDFLAGS) $(SDL_FLAGS) -lpthread

OBJECTS=MainUI.o \
		MainView.o \
		Camera.o \
		CommandLine.o \
		KeyboardInput.o \
		Trackbar.o \
		Digital.o \
		CPULoad.o

TARGET=testUI

all: $(TARGET)

testUI: $(OBJECTS) 
	g++ $+ $(LDFLAGS) -o $@

%.o: %.cpp
	g++ -c $(CXXFLAGS) $< -o $@
