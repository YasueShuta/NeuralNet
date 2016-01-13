TARGET = mylib_gnuplot.so
OBJS = Gnuplot.o

${TARGET}: ${OBJS}
	g++ -shared -o $@ $^
	${RM} $^

.cpp.o:
	g++ -c $< -std=c++11 -fPIC

.PHONY: clean
clean: 
	${RM} ${TARGET} ${OBJS}

all: clean ${TARGET}


