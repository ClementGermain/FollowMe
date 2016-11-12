# Raspicam_cv C++ flags
RCAM_CXXFLAGS=-I$(RASPICAM_CV_DIR)

CXXFLAGS += # $(RCAM_CXXFLAGS) will be added if target is not 'noraspi'

OBJECTS=MainUI.o \
		MainView.o \
		Camera.o \
		CommandLine.o \
		KeyboardInput.o \
		Trackbar.o \
		Digital.o \
		CPULoad.o

TARGET=ui.a

all: CXXFLAGS += $(RCAM_CXXFLAGS)
all: $(TARGET)

noraspi: $(TARGET)
noraspi: CXXFLAGS += -D__NO_RASPI__

$(TARGET): $(OBJECTS) 
	ar rc  $@ $^

%.o: %.cpp
	g++ -c $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS)
