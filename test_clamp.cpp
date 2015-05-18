// Copyright 2014-2015 Martin Moene.
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
    // test prerequisites:

    CASE( "std14::less<> works" )
    {
        EXPECT(   std14::less<>()( -6, -5 ) );
        EXPECT( ! std14::less<>()( -5, -5 ) );
        EXPECT( ! std14::less<>()( -4, -5 ) );
        EXPECT(   std14::less<>()( -3,  5 ) );
        EXPECT(   std14::less<>()(  3,  5 ) );
        EXPECT(   std14::less<>()(  4,  5 ) );
        EXPECT( ! std14::less<>()(  5,  5 ) );
        EXPECT( ! std14::less<>()(  6,  5 ) );
    },

    CASE( "std14::greater<> works" )
    {
        EXPECT( ! std14::greater<>()( -6, -5 ) );
        EXPECT( ! std14::greater<>()( -5, -5 ) );
        EXPECT(   std14::greater<>()( -4, -5 ) );
        EXPECT( ! std14::greater<>()( -3,  5 ) );
        EXPECT( ! std14::greater<>()(  3,  5 ) );
        EXPECT( ! std14::greater<>()(  4,  5 ) );
        EXPECT( ! std14::greater<>()(  5,  5 ) );
        EXPECT(   std14::greater<>()(  6,  5 ) );
    },

    // clamp( val, lo, hi ), equivalent to 
    // clamp( val, lo, hi, std::less<>() ):

    CASE( "clamp(v,lo,hi) is a valid constexpr expression" )
    {
        constexpr auto c = clamp( 7, 5, 9 );
        use(c);
    },

    CASE( "clamp(v,lo,hi) asserts !comp(hi, lo), pass" )
    {
        (void) clamp( 0, 5, 5 );
    },

    CASE( "clamp(v,lo,hi) asserts !comp(hi, lo), fail (skipped unless selected)" " [.]" )
    {
        (void) clamp( 0, 9, 5 );
    },

    CASE( "clamp(v,lo,hi) below lower boundary clamps to lower boundary" )
    {
        EXPECT(  5 == clamp(  4,  5,  9 ) );
        EXPECT( -5 == clamp( -9, -5, -1 ) );

        EXPECT(  5 == clamp( -4,  5,  9 ) );
        EXPECT( -5 == clamp( -9, -5,  9 ) );
    },

    CASE( "clamp(v,lo,hi) at lower boundary returns value" )
    {
        EXPECT(  5 == clamp(  5,  5, 9 ) );
        EXPECT( -5 == clamp( -5, -5, 9 ) );
    },

    CASE( "clamp(v,lo,hi) with non-boundary value returns value" )
    {
        EXPECT(  7 == clamp(  7,  5,  9 ) );
        EXPECT( -7 == clamp( -7, -9, -5 ) );
    },

    CASE( "clamp(v,lo,hi) at upper boundary returns value" )
    {
        EXPECT(  9 == clamp(  9,  5,  9 ) );
        EXPECT( -5 == clamp( -5, -9, -5 ) );
    },

    CASE( "clamp(v,lo,hi) above upper boundary clamps to upper boundary" )
    {
        EXPECT(  9 == clamp( 10,  5,  9 ) );
        EXPECT( -5 == clamp( -2, -9, -5 ) );
    },

    CASE( "clamp(v,lo,hi) at lower boundary returns first argument" )
    {{
        int v =  5, lo = 5, hi = 9;
        EXPECT( &v == &clamp( v, lo, hi ) );
    }{
        int v = -5, lo = -5, hi = 9;
        EXPECT( &v == &clamp( v, lo, hi ) );
    }},

    CASE( "clamp(v,lo,hi) at upper boundary returns first argument" )
    {{
        int v =  9, lo = 5, hi = 9;
        EXPECT( &v == &clamp( v, lo, hi ) );
    }{
        int v = -5, lo = -9, hi = -5;
        EXPECT( &v == &clamp( v, lo, hi ) );
    }},

    CASE( "clamp(v,lo,hi) below lower boundary fails to clamp to lower boundary (assert failure)" )
    {
        test fail[] = {{ CASE("F") { EXPECT(  3 == clamp(  3,  5,  9 ) ); } },
                       { CASE("F") { EXPECT( -3 == clamp( -3, -9, -5 ) ); } }};

        EXPECT( 2 == run( fail, dev_null ) );
    },

    CASE( "clamp(v,lo,hi) with non-boundary value returns lower boundary (assert failure)" )
    {
        test fail[] = {{ CASE("F") { EXPECT(  5 == clamp(  7,  5,  9 ) ); } },
                       { CASE("F") { EXPECT( -9 == clamp( -7, -9, -5 ) ); } }};

        EXPECT( 2 == run( fail, dev_null ) );
    },

    CASE( "clamp(v,lo,hi) with non-boundary value returns upper boundary (assert failure)" )
    {
        test fail[] = {{ CASE("F") { EXPECT(  9 == clamp(  7,  5,  9 ) ); } },
                       { CASE("F") { EXPECT( -5 == clamp( -7, -9, -5 ) ); } }};

        EXPECT( 2 == run( fail, dev_null ) );
    },

    CASE( "clamp(v,lo,hi) above upper boundary fails to clamp to upper boundary (assert failure)" )
    {
        test fail[] = {{ CASE("F") { EXPECT( 13 == clamp( 13,  5,  9 ) ); } },
                       { CASE("F") { EXPECT( -2 == clamp( -2, -9, -5 ) ); } }};

        EXPECT( 2 == run( fail, dev_null ) );
    },
    
    CASE( "clamp(v,lo,hi) works with unsigned int" )
    {
        EXPECT( 5U == clamp(  4U, 5U, 9U ) );
        EXPECT( 5U == clamp(  5U, 5U, 9U ) );
        EXPECT( 6U == clamp(  6U, 5U, 9U ) );
        EXPECT( 7U == clamp(  7U, 5U, 9U ) );
        EXPECT( 8U == clamp(  8U, 5U, 9U ) );
        EXPECT( 9U == clamp(  9U, 5U, 9U ) );
        EXPECT( 9U == clamp( 10U, 5U, 9U ) );
    },
    
    CASE( "clamp(v,lo,hi) works with double" )
    {
        EXPECT( approx( 5., clamp(  4., 5., 9. ) ) );
        EXPECT( approx( 5., clamp(  5., 5., 9. ) ) );
        EXPECT( approx( 6., clamp(  6., 5., 9. ) ) );
        EXPECT( approx( 7., clamp(  7., 5., 9. ) ) );
        EXPECT( approx( 8., clamp(  8., 5., 9. ) ) );
        EXPECT( approx( 9., clamp(  9., 5., 9. ) ) );
        EXPECT( approx( 9., clamp( 10., 5., 9. ) ) );
    },
    
#ifdef CLAMP_ACCEPTS_DIFFERENT_ARGUMENT_TYPES

    // std::min(), std::max(), clamp() do not accept different argument types:

    CASE( "std::max() does not accept different argument types" )
    {
        EXPECT( 9U == std::max(  4U, 9 ) );
        EXPECT( 9  == std::max(  4U, 9 ) );
    },

    CASE( "clamp(v,lo,hi) does not work with mixed argument types (2)" )
    {
        EXPECT( 5U == clamp(  4U, 5, 9 ) );
        EXPECT( 5U == clamp(  5U, 5, 9 ) );
        EXPECT( 6U == clamp(  6U, 5, 9 ) );
        EXPECT( 9U == clamp(  9U, 5, 9 ) );
        EXPECT( 9U == clamp( 10U, 5, 9 ) );
    },
    
    CASE( "clamp(v,lo,hi) does not work with mixed argument types (3)" )
    {
        EXPECT( 5U == clamp(  4U, 5, 9. ) );
        EXPECT( 5U == clamp(  5U, 5, 9. ) );
        EXPECT( 6U == clamp(  6U, 5, 9. ) );
        EXPECT( 9U == clamp(  9U, 5, 9. ) );
        EXPECT( 9U == clamp( 10U, 5, 9. ) );
    },
#endif

    // test clamp using a (different) predicate:
    
    CASE( "clamp(v,lo,hi,pred) works with a predicate" )
    {
        EXPECT( 5 == clamp(  4, 9, 5, std14::greater<>() ) );
        EXPECT( 5 == clamp(  5, 9, 5, std14::greater<>() ) );
        EXPECT( 6 == clamp(  6, 9, 5, std14::greater<>() ) );
        EXPECT( 7 == clamp(  7, 9, 5, std14::greater<>() ) );
        EXPECT( 8 == clamp(  8, 9, 5, std14::greater<>() ) );
        EXPECT( 9 == clamp(  9, 9, 5, std14::greater<>() ) );
        EXPECT( 9 == clamp( 10, 9, 5, std14::greater<>() ) );
    },
    
    // test clamp() with non-copyable type (see test_util.hpp):

    // test prerequisite:

    CASE( "non_copyable() compares equal (positive and negative)" )
    {
                                     EXPECT( non_copyable(2) == non_copyable(2) );
        test fail[] = {{ CASE("F") { EXPECT( non_copyable(3) == non_copyable(4) ); } }};

        EXPECT( 1 == run( fail, dev_null ) );
    },

    CASE( "clamp(v,lo,hi) can be used with a temporary non-copyable type" )
    {
        EXPECT( non_copyable(5) == clamp( non_copyable(5) , non_copyable(3), non_copyable(7) ) );
    },

    CASE( "clamp(v,lo,hi) can be used with a named non-copyable type" )
    {
        non_copyable val(5);
        non_copyable  lo(3);
        non_copyable  hi(7);

        EXPECT( val == clamp( val, lo, hi ) );
    },

    // test clamp_range():

    // test prerequisite:

    CASE( "operator==( vector ) compares equal" )
    {
        std::vector<int>       a{ 1,2,3, };
        std::vector<int> const b{ 1,2,3, };

        EXPECT( a == b );
    },

    CASE( "operator==( vector ) compares not equal" )
    {
        std::vector<int>       a{ 1,2,3, };
        std::vector<int> const b{ 1,3,3, };
        std::vector<int> const c{ 1,2,3,4 };

        EXPECT( !( a == b ) );
        EXPECT( !( a == c ) );
    },

    // clamp( first, last, out, lo, hi ), equivalent to 
    // clamp( first, last, out, lo, hi, std::less<>() ):

    CASE( "clamp( first, last, out, lo, hi ) clamps range successfully" )
    {
        std::vector<int>       a{ -7,1,2,3,4,5,6,7,8,9, };
        std::vector<int> const b{  3,3,3,3,4,5,6,7,7,7, };

        auto out = clamp_range( a.begin(), a.end(), a.begin(), 3, 7 );

        EXPECT( ( out == a.end() ) );
        EXPECT(     a == b         );
    },

    CASE( "clamp( first, last, out, lo, hi, pred ) clamps range successfully" )
    {
        std::vector<int>       a{ -7,1,2,3,4,5,6,7,8,9, };
        std::vector<int> const b{  3,3,3,3,4,5,6,7,7,7, };

        auto out = clamp_range( a.begin(), a.end(), a.begin(), 7, 3, std14::greater<>() );

        EXPECT( ( out == a.end() ) );
        EXPECT(     a == b         );
    },
};

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv /*, std::cout */ );
}

#endif //__cplusplus < 201103L

// cl -nologo -W4 -EHsc test_clamp.cpp && test_clamp
// g++ -Wall -Wextra -Weffc++ -std=c++11 -o test_clamp.exe test_clamp.cpp && test_clamp
