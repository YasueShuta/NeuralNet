CC = gcc
CXX = g++

OBJS = main.o
CFLAGS += -std=c++11 `pkg-config --cflags opencv`
LIBS += -O2 ../gnuplot/mylib_gnuplot.so `pkg-config --libs opencv`

TARGET = NeuralNet_main

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

.cpp.o:
	$(CXX) -c $(CFLAGS) $<

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)

all: clean $(TARGET)
