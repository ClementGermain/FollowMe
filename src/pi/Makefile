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
RASPI_LDFLAGS=-lwiringPi

# other flags
CXXFLAGS +=-Wall -std=c++11 -I. -MD
LDFLAGS +=-lpthread

# binary name and location
TARGET=barstow

UI_OBJECTS=ui/MainUI.o \
		   ui/MainView.o \
		   ui/Camera.o \
		   ui/CommandLine.o \
		   ui/KeyboardInput.o \
		   ui/Trackbar.o \
		   ui/Digital.o \
		   ui/LogView.o \
		   ui/CPULoad.o

OBJECTS=Main.o \
		car/Car.o \
		car/LinkSTM32.o \
		utils/Log.o \
		SPI_Interface/SPI_Interface.o \
		$(UI_OBJECTS)

all: LDFLAGS +=$(RASPI_LDFLAGS)
all: CXXFLAGS +=$(RCAM_CXXFLAGS)
all: $(TARGET)

noraspi: RASPI=noraspi
noraspi: CXXFLAGS += -D__NO_RASPI__
noraspi: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ $+ $(LDFLAGS) -o $@

%.o: %.cpp
	g++ -c $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS)
