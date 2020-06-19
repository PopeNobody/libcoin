include etc/config.mk

try: all
	./main

all: main

MAKEFLAGS := -rR --warn-undefined-variables -j8

CXX?=g++
CC?=$(CXX)
LD:=$(CXX)
LDLIBS:=-lcoin -liberty
CPPFLAGS := -Iinclude -MD
CXXFLAGS := -ggdb3 -O0 -std=c++17
LDFLAGS := -ggdb3 -O0 -std=c++17 -Llib -L.

exe_src := main.cc
all_src := $(wildcard *.cc)
lib_src := $(filter-out $(exe_src),$(all_src))

exe_obj := $(patsubst %.cc, %.cc.o, $(exe_src))
lib_obj := $(patsubst %.cc, %.cc.o, $(lib_src))
all_obj := $(exe_obj) $(lib_obj)

exe_cpp := $(patsubst %.cc, %.cc.i, $(exe_src))
lib_cpp := $(patsubst %.cc, %.cc.i, $(lib_src))
all_cpp := $(exe_cpp) $(lib_cpp)


$(all_obj): %.cc.o: %.cc $(wildcard *.hh)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

$(all_cpp): %.cc.i: %.cc $(wildcard *.hh)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -E $<

libcoin.a: $(lib_obj)
	ar r $@ $^

main: main.cc.o libcoin.a
	$(CXX) $(LDFLAGS) -o $@ $< $(LDLIBS)
