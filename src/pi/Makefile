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
CXXFLAGS +=-Wall -std=c++11 -I. -g
LDFLAGS +=-lpthread

# binary name and location
TARGET=barstow

UI_OBJECTS=ui/MainUI.o \
		   ui/MainView.o \
		   ui/CommandLine.o \
		   ui/CPULoad.o

VIEW_OBJECTS=ui/view/View.o \
			 ui/view/EmptyBoxView.o \
			 ui/view/PointerView.o \
			 ui/view/KeyboardInput.o \
			 ui/view/trackbar/Trackbar.o \
			 ui/view/trackbar/Trackbar_Horizontal.o \
			 ui/view/trackbar/Trackbar_Vertical.o \
			 ui/view/Digital.o \
			 ui/view/ToggleBox.o \
			 ui/view/StateBox.o \
			 ui/view/LogView.o \
			 ui/view/ImageView.o \
			 ui/view/TextView.o \
			 ui/view/Layout.o \
			 ui/view/PlotsView.o \
			 ui/view/TabView.o \
			 ui/ViewManager.o

OTHER_OBJECTS=Main.o \
			  car/Car.o \
			  car/LinkSTM32.o \
			  car/Camera.o \
			  car/Obstacle.o \
			  car/SPI_Interface.o \
			  car/MotorModel.o \
			  car/MotorDiagnostic.o \
			  car/StateRecorder.o \
			  IA/IA.o \
			  IA/Diagnostic.o \
			  utils/Log.o \
			  utils/Timer.o \
			  utils/Thread.o \
			  utils/PeriodicThread.o \
			  improc/UserDetection.o \
			  improc/KalmanFilterForUserDetection.o \
			  improc/UserDetectionThread.o \
			  improc/RoadDetection.o \
			  improc/RoadDetectionThread.o \
			  sound/Sound.o

OBJECTS=$(VIEW_OBJECTS) $(OTHER_OBJECTS) $(UI_OBJECTS)

all: LDFLAGS +=$(RASPI_LDFLAGS)
all: CXXFLAGS +=$(RASPI_CXXFLAGS)
all: $(TARGET)

noraspi: CXXFLAGS += -D__NO_RASPI__
noraspi: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo 'Link and build "$@"'
	@g++ $+ $(LDFLAGS) -o $@

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

%.o: %.cpp
	@echo -e 'Compile "$<"'
	@g++ -c $(CXXFLAGS) $< -o $@
	@g++ -MM $(CXXFLAGS) $< > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

cleanall:
	@echo "Remove all *.o and *.d files"
	@rm -f $(OBJECTS) $(OBJECTS:.o=.d)
clean:
	@echo "Remove all *.o and *.d files (excluding ui/view/)"
	@rm -f $(OTHER_OBJECTS) $(OTHER_OBJECTS:.o=.d)
	@rm -f $(UI_OBJECTS) $(UI_OBJECTS:.o=.d)
