all : clean main
.PHONY : all

CXX = g++
override CXXFLAGS += -std=c++2a -Wall -Wno-sign-compare -Werror -fdiagnostics-color=always
OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
main: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS)
clean:
	rm -f $(OBJS) a.out