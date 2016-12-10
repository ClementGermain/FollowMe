# OpenCV lib
LDFLAGS +=-lopencv_highgui -lopencv_core -lopencv_legacy -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_imgproc
# SDL lib
LDFLAGS +=-lSDL -lSDLmain -lSDL_gfx
# Raspicam_cv include
RASPI_CXXFLAGS +=-I/home/pi/raspicam_cv/robidouille/raspicam_cv
# Raspicam_cv lib
RASPI_LDFLAGS +=-L/home/pi/raspicam_cv/robidouille/raspicam_cv \
		   		-L/home/pi/raspicam_cv/userland/build/lib \
				-lraspicamcv -lpthread -lmmal_core -lmmal -lmmal_util -lvcos -lbcm_host

#SPI lib
RASPI_LDFLAGS +=-lwiringPi

# other flags
CXXFLAGS +=-Wall -std=c++11 -I. -MD -g
LDFLAGS +=-lpthread

# binary name and location
TARGET=barstow

UI_OBJECTS=ui/MainUI.o \
		   ui/MainView.o \
		   ui/CommandLine.o \
		   ui/view/View.o \
		   ui/view/EmptyBoxView.o \
		   ui/view/PointerView.o \
		   ui/view/KeyboardInput.o \
		   ui/view/trackbar/Trackbar.o \
		   ui/view/trackbar/Trackbar_Horizontal.o\
		   ui/view/trackbar/Trackbar_Vertical.o\
		   ui/view/Digital.o \
		   ui/view/ToggleBox.o\
		   ui/view/LogView.o \
		   ui/view/ImageView.o \
		   ui/view/TextView.o \
		   ui/view/Layout.o \
		   ui/view/PlotsView.o \
		   ui/ViewManager.o \
		   ui/CPULoad.o

OBJECTS=Main.o \
		car/Car.o \
		car/LinkSTM32.o \
		car/Camera.o \
		car/Obstacle.o \
		car/SPI_Interface.o \
		car/MotorModel.o\
		car/MotorDiagnostic.o\
		IA/IA.o\
		utils/Log.o \
		utils/Timer.o \
		improc/UserPatternDetection.o \
		improc/KalmanFilterForUserDetection.o \
		improc/UserPatternDetectionTest.o \
		improc/RoadDetection.o \
		improc/RoadDetectionTest.o \
		sound/Sound.o \
		$(UI_OBJECTS)

all: LDFLAGS +=$(RASPI_LDFLAGS)
all: CXXFLAGS +=$(RASPI_CXXFLAGS)
all: $(TARGET)

noraspi: CXXFLAGS += -D__NO_RASPI__
noraspi: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ $+ $(LDFLAGS) -o $@

%.o: %.cpp
	g++ -c $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS)
