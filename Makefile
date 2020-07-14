all: main

# main: 
# 	g++ -o main main.cpp argh/argh.cpp
main: 
	g++ -o main main.cpp fraction/fraction.h argh/argh.h function_guesser/function_guesser.cpp