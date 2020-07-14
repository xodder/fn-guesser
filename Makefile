all: main

# main: 
# 	g++ -o main main.cpp argh/argh.cpp
main: 
	g++ -o main main.cpp fraction/fraction.h argh/argh.cpp function_guesser/function_guesser.h