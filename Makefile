include etc/config.mk

try: all
	./main

all: main

MAKEFLAGS := -rR --warn-undefined-variables -j8

CXX:=$(word 1, $(CXX) clang++ g++)
CC:=$(CXX)
RM_F:=rm -f
LDLIBS:=-lcoin -liberty
CPPFLAGS := -Iinclude -I../include -MD
CXXFLAGS := -ggdb3 -O0 -std=c++17
LDFLAGS  := -L. -Llib -L../lib

all_exe := main some_test
dis_exe := some_test
exe_exe := $(filter-out $(dis_exe), $(all_exe))

exe_src := $(patsubst %.cc, %, $(all_exe))
all_src := $(wildcard *.cc)
lib_src := $(filter-out $(exe_src),$(all_src))

exe_obj := $(patsubst %.cc, %.cc.o, $(exe_src))
lib_obj := $(patsubst %.cc, %.cc.o, $(lib_src))
all_obj := $(exe_obj) $(lib_obj)

exe_cpp := $(patsubst %.cc, %.cc.i, $(exe_src))
lib_cpp := $(patsubst %.cc, %.cc.i, $(lib_src))
all_cpp := $(exe_cpp) $(lib_cpp)


$(all_obj): %.cc.o: %.cc.i
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

$(all_cpp): %.cc.i: %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -E $<

libcoin.a: $(lib_obj)
	ar r $@ $^

$(exe_exe): %: %.cc.o libcoin.a etc/ldflags
	$(CXX) @etc/ldflags $(LDFLAGS) -o $@ $< $(LDLIBS)

clean:
	$(RM_F) $(wildcard *.[ioda]) 

include /dev/null $(wildcard *.d)
