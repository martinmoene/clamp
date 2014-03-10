Clamp algorithm
===============

A simple algorithm to limit a value or a range of values to fall between two extremes. The intention is to propose `clamp` for the C++ `<algorithm>` library.

Code to limit a value between two extremes can come by various guises.

From what we found normal some (a long) time ago:
```
int clampedValue = value;

if        ( value < minValue ) clampedValue = minValue;
[else] if ( value > maxValue ) clampedValue = maxValue;
```

via:

```
const int clampedValue = std::min( std::max( value, minValue ), maxValue );
```

to:
```
const int clampedValue = clamp( value, minValue, maxValue );
```


`clamp()` already exists in C++ libraries such as Boost[1] and Microsoft AMP[2]. This proposal is inspired on the Boost version by Marshall Clow. There are also still plenty programming language systems that do not provide it, such as C#, Java, PHP, Python (although numpy has `clip()`), Ruby [3].

Usage
-----
Clamp a single value implicitly using `std::less<>`:
```
constexpr auto v = clamp( 7, 3, 9 );
```
Note: std::less<>::operator() will become constexpr in C++14.

Clamp a single value using the given predicate:
```
constexpr auto v = clamp( 7, 3, 9, std::less<int>() );
```

Clamp a range of values implicitly using `std::less<>`:
```
std::vector<int> a{ 1,2,3,4,5,6,7,8,9 };

auto out = clamp_range( a.begin(), a.end(), a.begin(), 3, 7 );
```

Clamp a range of values using the given predicate:
```
std::vector<int> a{ 1,2,3,4,5,6,7,8,9 };

auto out = clamp_range( a.begin(), a.end(), a.begin(), 3, 7, std::less<>() );
```
Note: std::less<> defaults to void and provides a templated member operator()() in C++14.

Names
-----
Other names for `clamp_range()` could be `clamp_elements()`, or `clamp_transform()`.

Note and references
-------------------

[1] `clamp()` in the [Boost Algoritm](http://www.boost.org/libs/algorithm/doc/html/algorithm/Misc.html#the_boost_algorithm_library.Misc.clamp) library.

[2] Microsoft. [C++ Accelerated Massive Parallelism](http://msdn.microsoft.com/en-us/library/hh265137.aspx) library.

[3] Try: http://stackoverflow.com/search?q=clamp

[4] Implementation note. Why does the implementation of `std::max()` in the GNUC `<algorithm>` library use `if()` instead of the ternary operator `?:` ? Jonathan Wakely answers: That code dates back to the SGI STL. It was changed, but then reverted, you can read the saga at http://gcc.gnu.org/ml/libstdc++/2012-09/msg00128.html

[![Build Status](https://travis-ci.org/martinmoene/clamp.png?branch=master)](https://travis-ci.org/martinmoene/clamp)
