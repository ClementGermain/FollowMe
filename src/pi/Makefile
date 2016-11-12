TARGET=barstow
CXXFLAGS=-Wall -std=c++11 -MD
LDFLAGS=-lpthread

OBJECTS=Main.o \
		car/Car.o \
		car/LinkSTM32.o \
		ui/ui.a

all: $(TARGET)


$(TARGET): $(OBJECTS)
	g++ $+ $(LDFLAGS) -o $@

ui/ui.a:
	make -C ui/

%.o: %.cpp
	g++ -c $(CXXFLAGS) $< -o $@
