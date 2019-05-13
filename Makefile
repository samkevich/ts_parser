name=ts_parser

options = -g -Wall -Wextra -pedantic -std=c++11

include_path=-Iinclude/
sources=$(wildcard src/*.cpp)

default: $(name)
	
$(name): $(sources) 
	rm -rf bin && mkdir bin && g++ $(options) $(include_path) -o bin/$@ $^

 
