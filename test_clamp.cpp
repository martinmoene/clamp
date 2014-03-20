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

#include "test_util.hpp"
#include "lest.hpp"

#include <algorithm>
#include <iostream>

using test = lest::test;

std::ostringstream dev_null;

const test specification[] =
{
    // clamp( val, lo, hi ), equivalent to 
    // clamp( val, lo, hi, std::less<>() ):

    "clamp(v,lo,hi) is a valid constexpr expression", []
    {
        constexpr auto c = clamp( 7, 5, 9 );
        use(c);
    },

    "clamp(v,lo,hi) asserts !comp(hi, lo), pass", []
    {
        (void) clamp( 0, 5, 5 );
    },

#if 0
    "clamp(v,lo,hi) asserts !comp(hi, lo), fail (activate to try)", []
    {
        (void) clamp( 0, 9, 5 );
    },
#endif

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

    // test clamp() with non-copyable type (see test_util.hpp):

    // test prerequisite:

    "non_copyable() compares equal (positive and negative)", []
    {
                                   EXPECT( non_copyable(2) == non_copyable(2) );
        test fail[] = {{ "F", [] { EXPECT( non_copyable(3) == non_copyable(4) ); } }};

        EXPECT( 1 == run( fail, dev_null ) );
    },

    "clamp(v,lo,hi) can be used with a temporary non-copyable type", []
    {
        EXPECT( non_copyable(5) == clamp( non_copyable(5) , non_copyable(3), non_copyable(7) ) );
    },

    "clamp(v,lo,hi) can be used with a named non-copyable type", []
    {
        non_copyable val(5);
        non_copyable  lo(3);
        non_copyable  hi(7);

        EXPECT( val == clamp( val, lo, hi ) );
    },

    // test clamp_range():

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

    // clamp( first, last, out, lo, hi ), equivalent to 
    // clamp( first, last, out, lo, hi, std::less<>() ):

    "clamp( first, last, out, lo, hi ) clamps range successfully", []
    {
        std::vector<int>       a{ 1,2,3,4,5,6,7,8,9, };
        std::vector<int> const b{ 3,3,3,4,5,6,7,7,7, };

        auto out = clamp_range( a.begin(), a.end(), a.begin(), 3, 7 );

        EXPECT( ( out == a.end() ) );
        EXPECT(     a == b         );
    },
};

int main()
{
    return run( specification );
}

#endif //__cplusplus < 201103L

// cl -nologo -W4 -EHsc test_clamp.cpp && test_clamp
// g++ -Wall -Wextra -Weffc++ -std=c++11 -o test_clamp.exe test_clamp.cpp && test_clamp
