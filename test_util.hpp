// Copyright (C) 2014 Martin Moene.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef TEST_UTIL_H_INCLUDED
#define TEST_UTIL_H_INCLUDED

#include "std14.hpp"

#include <sstream>
#include <vector>

template< typename T > T use( T const & x ) { return x; }

// Function approx thanks to Richard Harris (http://www.thusspakeak.com/),
// based on approximatelyEqual() by D.E. Knuth (The art of computer programming).
// true if two floating point values are considered equal given scale and epsilon.

bool approx_se( double lhs, double rhs, double scale, double epsilon )
{
    return std::abs( lhs - rhs ) < epsilon * (scale + std::max( std::abs(lhs), std::abs(rhs) ) );
}

// true if two floating point values are considered equal using scale:1 and
// epsilon:100 * epsilon|double.

bool approx( double lhs, double rhs )
{
   return approx_se( lhs, rhs, 1.0, 100.0 * std::numeric_limits<double>::epsilon() );
}

// true if vector are equal (per C++14):

template< typename T >
bool operator==( std::vector<T> const & a, std::vector<T> const & b )
{
    return std14::equal( a.begin(), a.end(), b.begin(), b.end() );
}

// non-copyable type:

struct non_copyable
{
    const int i;
    explicit non_copyable(int i) : i(i) {}
    non_copyable( non_copyable const & ) = delete;
    non_copyable & operator=( non_copyable const & ) = delete;
};

bool operator<( non_copyable const & lhs, non_copyable const & rhs )
{
	return lhs.i < rhs.i;
}

bool operator==( non_copyable const & lhs, non_copyable const & rhs )
{
	return lhs.i == rhs.i;
}

namespace lest {

std::string to_string( ::non_copyable const & nc )
{
    std::ostringstream os; os << nc.i; return os.str();
}

}

#endif // TEST_UTIL_H_INCLUDED

// end of file
