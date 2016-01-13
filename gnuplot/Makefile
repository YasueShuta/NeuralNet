TARGET = mylib_gnuplot.so

${TARGET}: Gnuplot.o
	g++ -shared -o $@ $^

.cpp.o:
	g++ -c $< -std=c++11 -fPIC

.PHONY: clean
clean:
	${RM} ${TARGET} Gnuplot.o

all: clean ${TARGET}


