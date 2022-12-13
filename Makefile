# appname := game

# CXX := g++
CXXFLAGS := -std=c++11 -pthread $(shell pkg-config --cflags sdl2) -l boost_program_options
LDFLAGS = $(shell pkg-config --libs sdl2)

gofl.exe: main.cxx src/*.cpp include/*.hpp
	g++ -o gofl.exe src/*.cpp main.cxx $(CXXFLAGS) $(LDFLAGS)

# srcfiles := $(shell find . -name "*.hpp")
# objects  := $(patsubst %.h, %.o, $(srcfiles))

# all: $(appname)

# $(appname): $(objects)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(appname) $(objects) $(LDLIBS)

# depend: .depend

# .depend: $(srcfiles)
# 	rm -f ./.depend
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -MM $^>>./.depend;

# clean:
# 	rm -f $(objects)

# dist-clean: clean
# 	rm -f *~ .depend

# include .depend
