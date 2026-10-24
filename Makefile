CXX      := g++
CXXFLAGS := -Isrc -I/usr/local/include
LDFLAGS  := -L/usr/local/lib
VERSION  := c++20
LDLIBS   := 

GRAPH ?= grammar/graph.png

.PHONY: build clean graph

build:
	mkdir -p build
	$(CXX) -std=$(VERSION) $(CXXFLAGS) -o build/tinyLang main.cpp src/*.cpp $(LDFLAGS) $(LDLIBS)

graph:
	reflex --graphs-file=grammar/grammar.dot grammar/grammar.l
	rm lex.yy.cpp
	mkdir -p $(dir $(GRAPH))
	dot -Tpng -o $(GRAPH) grammar/grammar.dot.gv

clean:
	rm -rf build
