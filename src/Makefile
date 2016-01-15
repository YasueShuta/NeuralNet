CC = gcc
CXX = g++

OBJS = main.o
ALLOBJS = $(OBJS) plot.o plotline.o data.o

CFLAGS += -std=c++11 `pkg-config --cflags opencv`
LIBS += -O2
GPLIBS = ../gnuplot/mylib_gnuplot.so
CVLIBS = `pkg-config --libs opencv`

TARGET = NeuralNet_main
ALLTARGET = $(TARGET) plot plotline data

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS) $(CVLIBS) $(GPLIBS)

plot: plot.o
	$(CXX) -o $@ $^ $(LIBS) $(GPLIBS)

plotline: plotline.o
	$(CXX) -o $@ $^ $(LIBS) $(GPLIBS)

data: data.o
	$(CXX) -o $@ $^ $(LIBS)



.cpp.o:
	$(CXX) -c $(CFLAGS) $<

.PHONY: clean
clean:
	$(RM) $(ALLTARGET) $(ALLOBJS)

all: clean $(ALLTARGET)
