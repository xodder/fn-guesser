DEPS=fraction/fraction.h \
		argh/argh.cpp \
		linear_algebra/linear_algebra.cpp \
		function_guesser/function_guesser.cpp

all: main

# main: 
# 	g++ -o main main.cpp argh/argh.cpp
main: 
	g++ -o main main.cpp $(DEPS)