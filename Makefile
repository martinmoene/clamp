# Copyright 2014 by Martin Moene
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

CXXFLAGS   := -Wall -Wextra -Weffc++ -Wno-missing-braces
CXXFLAGS03 := $(CXXFLAGS) -std=c++03
CXXFLAGS11 := $(CXXFLAGS) -std=c++11

all: test_clamp

test_lest: test_clamp.cpp clamp.hpp
	$(CXX) $(CXXFLAGS11) -o test_clamp test_clamp.cpp
	./test_clamp

clean:
	rm test_clamp


