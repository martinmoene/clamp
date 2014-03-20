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

bool operator<( const non_copyable& lhs, const non_copyable& rhs )
{
	return lhs.i < rhs.i;
}

bool operator==( const non_copyable& lhs, const non_copyable& rhs )
{
	return lhs.i == rhs.i;
}

namespace lest {

std::string to_string( const ::non_copyable & nc )
{
    std::ostringstream os; os << nc.i; return os.str();
}

}

#endif // TEST_UTIL_H_INCLUDED

// end of file
