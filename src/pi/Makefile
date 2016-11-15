# OpenCV LD flags
OCV_LDFLAGS=-lopencv_highgui -lopencv_core -lopencv_legacy -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_imgproc
# SDL flags
SDL_FLAGS=-lSDL -lSDLmain -lSDL_gfx
# Raspicam_cv LD flags
export RASPICAM_CV_DIR:=/home/pi/raspicam_cv/robidouille/raspicam_cv
USERLAND_LIB=/home/pi/raspicam_cv/userland/build/lib
RCAM_LDFLAGS=-L$(USERLAND_LIB) -lmmal_core -lmmal -lmmal_util -lvcos -lbcm_host \
		   -L$(RASPICAM_CV_DIR) -lraspicamcv -lpthread
# Raspicam_cv C++ flags
RCAM_CXXFLAGS=-I$(RASPICAM_CV_DIR)

#SPI lib
SPI_LDFLAGS=-lwiringPi

TARGET=barstow

CXXFLAGS=-Wall -std=c++11 -MD
LDFLAGS=$(OCV_LDFLAGS) $(SDL_FLAGS) -lpthread # $(RCAM_LDFLAGS) will be added if target is not 'noraspi'


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

all: LDFLAGS += $(RCAM_LDFLAGS)
all: LDFLAGS += $(SPI_LDFLAGS)
all: CXXFLAGS += $(RCAM_CXXFLAGS)
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
