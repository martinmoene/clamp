// Copyright (C) 2014 Martin Moene.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// The idea and interface is based on the Boost.Algorithm library
// authored by Marshall Clow.

#if __cplusplus < 201103L
#error Compile as C++11 or newer.
#else // __cplusplus < 201103L

#include "clamp.hpp"
#include "lest.hpp"

#include <algorithm>
#include <iostream>
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

using test = lest::test;

std::ostringstream dev_null;

template< typename T > T use( T const & x ) { return x; }

template< typename T >
bool operator==( std::vector<T> const & a, std::vector<T> const & b )
{
    return std14::equal( a.begin(), a.end(), b.begin(), b.end() );
}

const test specification[] =
{
    // clamp( val, lo, hi ):

    "clamp(v,lo,hi) is a valid constexpr expression", []
    {
        constexpr auto c = clamp( 7, 5, 9 );
        use(c);
    },

    "clamp(v,lo,hi) below lower boundary clamps to lower boundary", []
    {
        EXPECT( 5 == clamp( 4, 5, 9 ) );
    },

    "clamp(v,lo,hi) at lower boundary returns value", []
    {
        EXPECT( 5 == clamp( 5, 5, 9 ) );
    },

    "clamp(v,lo,hi) with non-boundary value returns value", []
    {
        EXPECT( 7 == clamp( 7, 5, 9 ) );
    },

    "clamp(v,lo,hi) at upper boundary returns value", []
    {
        EXPECT( 9 == clamp( 9, 5, 9 ) );
    },

    "clamp(v,lo,hi) above upper boundary clamps to upper boundary", []
    {
        EXPECT( 9 == clamp( 10, 5, 9 ) );
    },

    "clamp(v,lo,hi) below lower boundary fails to clamp to lower boundary (negative)", []
    {
        test fail[] = {{ "F", [] { EXPECT( 3 == clamp( 3, 5, 9 ) ); } }};

        EXPECT( 1 == run( fail, dev_null ) );
    },

    "clamp(v,lo,hi) with non-boundary value returns lower boundary (negative)", []
    {
        test fail[] = {{ "F", [] { EXPECT( 5 == clamp( 7, 5, 9 ) ); } }};

        EXPECT( 1 == run( fail, dev_null ) );
    },

    "clamp(v,lo,hi) with non-boundary value returns upper boundary (negative)", []
    {
        test fail[] = {{ "F", [] { EXPECT( 9 == clamp( 7, 5, 9 ) ); } }};

        EXPECT( 1 == run( fail, dev_null ) );
    },

    "clamp(v,lo,hi) above upper boundary fails to clamp to upper boundary (negative)", []
    {
        test fail[] = {{ "F", [] { EXPECT( 13 == clamp( 13, 5, 9 ) ); } }};

        EXPECT( 1 == run( fail, dev_null ) );
    },

    // clamp( val, lo, hi, pred ) is used by clamp( val, lo, hi ).

    // test prerequisite:

    "operator==( vector ) compares equal", []
    {
        std::vector<int>       a{ 1,2,3, };
        std::vector<int> const b{ 1,2,3, };

        EXPECT( a == b );
    },

    "operator==( vector ) compares not equal", []
    {
        std::vector<int>       a{ 1,2,3, };
        std::vector<int> const b{ 1,3,3, };

        EXPECT( !( a == b ) );
    },

    // clamp( first, last, out, lo, hi ):

    "clamp( first, last, out, lo, hi ) clamps range successfully", []
    {
        std::vector<int>       a{ 1,2,3,4,5,6,7,8,9, };
        std::vector<int> const b{ 3,3,3,4,5,6,7,7,7, };

        auto out = clamp_range( a.begin(), a.end(), a.begin(), 3, 7 );

        EXPECT( ( out == a.end() ) );
        EXPECT(     a == b         );
    },

    // clamp( first, last, out, lo, hi ) uses
    // clamp( first, last, out, lo, hi, pred ):
};

int main()
{
    return run( specification );
}

#endif //__cplusplus < 201103L

// cl -nologo -W4 -EHsc test_clamp.cpp && test_clamp
// g++ -Wall -Wextra -Weffc++ -std=c++11 -o test_clamp.exe test_clamp.cpp && test_clamp
