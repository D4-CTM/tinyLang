CXX      := g++
CXXFLAGS := -Isrc -I/usr/local/include
LDFLAGS  := -L/usr/local/lib
LDLIBS   := -lreflex

.PHONY: build clean graph

build:
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o build/tinyLang main.cpp src/*.cpp $(LDFLAGS) $(LDLIBS)

graph:
	reflex --graphs-file=Grammar/grammar.dot Grammar/grammar.l 
	rm lex.yy.cpp
	dot -Tpng Grammar/grammar.dot.gv >> Grammar/graph.png

clean:
	rm -rf build
