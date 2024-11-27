# Author: LemonBarsXD
# Simple but it works

all: main

main: main.cpp mainDebug.cpp
	g++ main.cpp -o bin\main -lgdi32 -Wall -Wextra -O3
	g++ mainDebug.cpp -o bin\debug -lgdi32 -Wall -Wextra -O3