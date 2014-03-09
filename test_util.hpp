// Copyright (C) 2014 Martin Moene.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef TEST_UTIL_H_INCLUDED
#define TEST_UTIL_H_INCLUDED

#include <sstream>
#include <vector>

//
// emulate C++14 if necessary:
//
#if __cplusplus == 201103L

namespace std14 {

template< class InputIt1, class InputIt2 >
bool equal( InputIt1 first1, InputIt1 last1,
            InputIt2 first2, InputIt2 last2 )
{
    if ( std::distance( first1, last1 ) != std::distance( first2, last2 ) )
        return false;

    for ( ; first1 != last1; ++first1, ++first2 )
    {
        if ( ! (*first1 == *first2) )
        {
            return false;
        }
    }
    return true;
}
} // namespace std14

#else // __cplusplus

#include <functional>
namespace std14 { using std::equal; }

#endif // __cplusplus

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
