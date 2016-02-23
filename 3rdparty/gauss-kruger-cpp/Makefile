CXX       = g++
CXXFLAGS  = -pipe -Wall -W -ansi -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast
CXXFLAGS += -fPIC

default: lib
lib: libgausskruger.a libgausskruger.so
test: test_reference_points
cli: gausskruger
all: default test cli

.PHONY: clean
clean:
	rm -f *.o *.a *.so \
        gausskruger libgausskruger.a libgausskruger.so test_reference_points

# CLI tool
gausskruger: gausskruger_cli.o gausskruger.o
	$(CXX) -o $@ $^ -lboost_program_options

# Static library
libgausskruger.a: gausskruger.o
	ar rcs $@ $^

# Shared library
libgausskruger.so: gausskruger.o
	$(CXX) -shared $^ -o $@

# Tests
test_reference_points: gausskruger.o test_reference_points.o
	$(CXX) -o $@ $^

gausskruger.o: gausskruger.cpp gausskruger.h
gausskruger_cli.o: gausskruger_cli.cpp
test_reference_points.o: test_reference_points.cpp
