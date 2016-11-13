# OpenCV LD flags
OCV_LDFLAGS=-lopencv_highgui -lopencv_core -lopencv_legacy -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_imgproc
# SDL flags
SDL_FLAGS=-lSDL -lSDLmain -lSDL_gfx
# Raspicam_cv LD flags
export RASPICAM_CV_DIR:=/home/pi/raspicam_cv/robidouille/raspicam_cv
USERLAND_LIB=/home/pi/raspicam_cv/userland/build/lib
RCAM_LDFLAGS=-L$(USERLAND_LIB) -lmmal_core -lmmal -lmmal_util -lvcos -lbcm_host \
		   -L$(RASPICAM_CV_DIR) -lraspicamcv -lpthread

TARGET=barstow
export CXXFLAGS:=-Wall -std=c++11 -MD
LDFLAGS=$(OCV_LDFLAGS) $(SDL_FLAGS) -lpthread # $(RCAM_LDFLAGS) will be added if target is not 'noraspi'


OBJECTS=Main.o \
		car/Car.o \
		car/LinkSTM32.o \
		utils/Log.o
		ui/ui.a

all: LDFLAGS += $(RCAM_LDFLAGS)
all: $(TARGET)

noraspi: RASPI=noraspi
noraspi: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ $+ $(LDFLAGS) -o $@

ui/ui.a:
	make $(RASPI) -C ui/

%.o: %.cpp
	g++ -c $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS)
	make clean -C ui/
