# Benchmarking Erlang map element internals

This is related to [issue #6139](https://github.com/erlang/otp/issues/6139) on the Erlang/OTP repository.

It is an exploratory benchmark to compare different ways of iterating over a 'flatmap', which is a collection
of at most 32 keys (and 32 values).

The challenge is to look up the value for a key that matches a given input key.


# Building and running

Prerequisites: gcc, g++, clang, clang++, make.

This project uses the 'picobenchmark' microbenchmarking library, which is included as single-header in the repo.

NOTE: _Although a C++ library is used to abstract away the microbenchmarking logic, the actual implementations are written in C and compiled with a C compiler.
As such they should match how the resulting assembly would look if they were to be compiled as part of the Erlang Runtime System._

After cloning the repository, run `make` to create four executables:
- clangO3
- clangO2
- gccO3
- gccO2

These reflect the two C compilers with related optimization options.

Running each of these executables will print a benchmark results table.

# About the microbenchmark

Take the results with a huge grain of salt! 
In an actual Erlang program there are thousands of other things going on,
and cache lines will be filled with other stuff.

This benchmark only tests what happens if a program does nothing else than look up many keys in tiny maps.

The particular benchmark will, for each map size between 1 and 32, run 100_000 iterations in which a random key in _twice_ this range is picked.
This means there is a 50% chance of finding the key. **(I think this is the main point which could be considered 'artificial' about this microbenchmark (besides not running all of ERTS); 50% is an arbitrary cutoff not based on real data.)**

This procedure is repeated for each of the potential map retrieval function implementations.

By default, two more implementations which I came up with which turn out to be quite a bit slower on my machine than the original implementation
are commented in the source code (in main.cpp), but feel free to experiment.

# Results

On my computer, which has the following specs:

```
OS: Manjaro Linux x86_64 
Host: 80NV Lenovo ideapad Y700-15ISK 
Kernel: 5.18.7-1-MANJARO 
CPU: Intel i7-6700HQ (8) @ 3.500GHz 
GPU: NVIDIA GeForce GTX 960M 
GPU: Intel HD Graphics 530 
Memory: 12666MiB / 31932MiB 
```

The results on my machine seem to indicate that there is a potential for a 5-15% speed improvement depending on tuple size.
Clang works well both on O2 and O3; GCC significantly better on O3.

(Of course, it is possible to enable O3 only for a particular function using some GCC specific `__attribute__`s.)

As we measure 100_000 iterations (to reduce the influence of measurement overhead on the benchmark), the 'Total ms' number in the results should be divided by 100_000 to find the real number.

_(Note that 'ns/op' and 'Ops/second' are scaled w.r.t. `Dim` and are thus not that useful in this particular benchmark.)_

## GCC

### -O2

```
===============================================================================
   Name (baseline is *)   |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
===============================================================================
b<original_flatmap_get> * |       1 |     1.689 | 1689131 |      - |      592.0
b<latereturn_flatmap_get> |       1 |     1.106 | 1106135 |  0.655 |      904.0
b<original_flatmap_get> * |       2 |     1.797 |  898298 |      - |     1113.2
b<latereturn_flatmap_get> |       2 |     1.191 |  595597 |  0.663 |     1679.0
b<original_flatmap_get> * |       3 |     1.789 |  596290 |      - |     1677.0
b<latereturn_flatmap_get> |       3 |     1.480 |  493478 |  0.828 |     2026.4
b<original_flatmap_get> * |       4 |     1.813 |  453264 |      - |     2206.2
b<latereturn_flatmap_get> |       4 |     1.311 |  327748 |  0.723 |     3051.1
b<original_flatmap_get> * |       5 |     1.915 |  382933 |      - |     2611.4
b<latereturn_flatmap_get> |       5 |     1.406 |  281262 |  0.734 |     3555.4
b<original_flatmap_get> * |       6 |     1.983 |  330420 |      - |     3026.4
b<latereturn_flatmap_get> |       6 |     1.655 |  275865 |  0.835 |     3625.0
b<original_flatmap_get> * |       7 |     1.878 |  268281 |      - |     3727.4
b<latereturn_flatmap_get> |       7 |     1.559 |  222707 |  0.830 |     4490.2
b<original_flatmap_get> * |       8 |     1.896 |  237058 |      - |     4218.4
b<latereturn_flatmap_get> |       8 |     1.472 |  183972 |  0.776 |     5435.6
b<original_flatmap_get> * |       9 |     2.051 |  227845 |      - |     4388.9
b<latereturn_flatmap_get> |       9 |     1.649 |  183200 |  0.804 |     5458.5
b<original_flatmap_get> * |      10 |     2.126 |  212558 |      - |     4704.6
b<latereturn_flatmap_get> |      10 |     1.806 |  180586 |  0.850 |     5537.5
b<original_flatmap_get> * |      11 |     2.040 |  185489 |      - |     5391.1
b<latereturn_flatmap_get> |      11 |     1.622 |  147463 |  0.795 |     6781.3
b<original_flatmap_get> * |      12 |     2.026 |  168798 |      - |     5924.2
b<latereturn_flatmap_get> |      12 |     1.752 |  145978 |  0.865 |     6850.3
b<original_flatmap_get> * |      13 |     2.140 |  164610 |      - |     6075.0
b<latereturn_flatmap_get> |      13 |     1.945 |  149646 |  0.909 |     6682.4
b<original_flatmap_get> * |      14 |     2.265 |  161760 |      - |     6182.0
b<latereturn_flatmap_get> |      14 |     2.019 |  144224 |  0.892 |     6933.6
b<original_flatmap_get> * |      15 |     2.372 |  158143 |      - |     6323.4
b<latereturn_flatmap_get> |      15 |     1.977 |  131791 |  0.833 |     7587.7
b<original_flatmap_get> * |      16 |     2.164 |  135246 |      - |     7393.9
b<latereturn_flatmap_get> |      16 |     2.128 |  133000 |  0.983 |     7518.8
b<original_flatmap_get> * |      17 |     2.316 |  136239 |      - |     7340.0
b<latereturn_flatmap_get> |      17 |     1.882 |  110684 |  0.812 |     9034.7
b<original_flatmap_get> * |      18 |     2.219 |  123303 |      - |     8110.1
b<latereturn_flatmap_get> |      18 |     2.083 |  115701 |  0.938 |     8643.0
b<original_flatmap_get> * |      19 |     2.262 |  119044 |      - |     8400.2
b<latereturn_flatmap_get> |      19 |     2.471 |  130047 |  1.092 |     7689.5
b<original_flatmap_get> * |      20 |     2.190 |  109504 |      - |     9132.1
b<latereturn_flatmap_get> |      20 |     2.067 |  103361 |  0.944 |     9674.8
b<original_flatmap_get> * |      21 |     2.396 |  114074 |      - |     8766.2
b<latereturn_flatmap_get> |      21 |     2.469 |  117560 |  1.031 |     8506.3
b<original_flatmap_get> * |      22 |     2.227 |  101243 |      - |     9877.2
b<latereturn_flatmap_get> |      22 |     2.054 |   93361 |  0.922 |    10711.0
b<original_flatmap_get> * |      23 |     2.544 |  110596 |      - |     9041.9
b<latereturn_flatmap_get> |      23 |     2.188 |   95111 |  0.860 |    10513.9
b<original_flatmap_get> * |      24 |     2.304 |   96005 |      - |    10416.1
b<latereturn_flatmap_get> |      24 |     2.574 |  107255 |  1.117 |     9323.5
b<original_flatmap_get> * |      25 |     2.335 |   93414 |      - |    10705.0
b<latereturn_flatmap_get> |      25 |     2.139 |   85546 |  0.916 |    11689.5
b<original_flatmap_get> * |      26 |     2.311 |   88873 |      - |    11252.0
b<latereturn_flatmap_get> |      26 |     2.191 |   84263 |  0.948 |    11867.5
b<original_flatmap_get> * |      27 |     2.381 |   88184 |      - |    11339.9
b<latereturn_flatmap_get> |      27 |     2.547 |   94326 |  1.070 |    10601.5
b<original_flatmap_get> * |      28 |     2.426 |   86652 |      - |    11540.4
b<latereturn_flatmap_get> |      28 |     2.395 |   85539 |  0.987 |    11690.5
b<original_flatmap_get> * |      29 |     2.937 |  101285 |      - |     9873.0
b<latereturn_flatmap_get> |      29 |     2.259 |   77903 |  0.769 |    12836.3
b<original_flatmap_get> * |      30 |     2.938 |   97927 |      - |    10211.7
b<latereturn_flatmap_get> |      30 |     2.447 |   81559 |  0.833 |    12260.9
b<original_flatmap_get> * |      31 |     3.231 |  104209 |      - |     9596.0
b<latereturn_flatmap_get> |      31 |     2.955 |   95334 |  0.915 |    10489.4
b<original_flatmap_get> * |      32 |     3.231 |  100983 |      - |     9902.6
b<latereturn_flatmap_get> |      32 |     3.826 |  119560 |  1.184 |     8364.0
===============================================================================
```

### -O3

```
===============================================================================
   Name (baseline is *)   |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
===============================================================================
b<original_flatmap_get> * |       1 |     1.638 | 1638109 |      - |      610.5
b<latereturn_flatmap_get> |       1 |     1.007 | 1007392 |  0.615 |      992.7
b<original_flatmap_get> * |       2 |     1.764 |  882076 |      - |     1133.7
b<latereturn_flatmap_get> |       2 |     1.092 |  545935 |  0.619 |     1831.7
b<original_flatmap_get> * |       3 |     1.764 |  588048 |      - |     1700.5
b<latereturn_flatmap_get> |       3 |     1.157 |  385696 |  0.656 |     2592.7
b<original_flatmap_get> * |       4 |     1.884 |  471079 |      - |     2122.8
b<latereturn_flatmap_get> |       4 |     1.278 |  319551 |  0.678 |     3129.4
b<original_flatmap_get> * |       5 |     1.654 |  330805 |      - |     3022.9
b<latereturn_flatmap_get> |       5 |     1.021 |  204220 |  0.617 |     4896.7
b<original_flatmap_get> * |       6 |     1.921 |  320194 |      - |     3123.1
b<latereturn_flatmap_get> |       6 |     1.165 |  194180 |  0.606 |     5149.8
b<original_flatmap_get> * |       7 |     1.926 |  275072 |      - |     3635.4
b<latereturn_flatmap_get> |       7 |     1.248 |  178272 |  0.648 |     5609.4
b<original_flatmap_get> * |       8 |     1.863 |  232908 |      - |     4293.5
b<latereturn_flatmap_get> |       8 |     1.288 |  161046 |  0.691 |     6209.4
b<original_flatmap_get> * |       9 |     1.892 |  210167 |      - |     4758.1
b<latereturn_flatmap_get> |       9 |     1.306 |  145090 |  0.690 |     6892.3
b<original_flatmap_get> * |      10 |     2.011 |  201090 |      - |     4972.9
b<latereturn_flatmap_get> |      10 |     1.250 |  125008 |  0.622 |     7999.4
b<original_flatmap_get> * |      11 |     1.976 |  179657 |      - |     5566.1
b<latereturn_flatmap_get> |      11 |     1.278 |  116225 |  0.647 |     8604.0
b<original_flatmap_get> * |      12 |     1.782 |  148516 |      - |     6733.2
b<latereturn_flatmap_get> |      12 |     1.529 |  127438 |  0.858 |     7847.0
b<original_flatmap_get> * |      13 |     1.991 |  153172 |      - |     6528.6
b<latereturn_flatmap_get> |      13 |     1.649 |  126862 |  0.828 |     7882.5
b<original_flatmap_get> * |      14 |     2.191 |  156496 |      - |     6389.9
b<latereturn_flatmap_get> |      14 |     1.763 |  125943 |  0.805 |     7940.0
b<original_flatmap_get> * |      15 |     2.167 |  144440 |      - |     6923.3
b<latereturn_flatmap_get> |      15 |     1.570 |  104689 |  0.725 |     9552.1
b<original_flatmap_get> * |      16 |     2.050 |  128096 |      - |     7806.6
b<latereturn_flatmap_get> |      16 |     1.733 |  108337 |  0.846 |     9230.4
b<original_flatmap_get> * |      17 |     1.947 |  114541 |      - |     8730.5
b<latereturn_flatmap_get> |      17 |     2.019 |  118791 |  1.037 |     8418.1
b<original_flatmap_get> * |      18 |     2.199 |  122168 |      - |     8185.4
b<latereturn_flatmap_get> |      18 |     1.851 |  102834 |  0.842 |     9724.4
b<original_flatmap_get> * |      19 |     2.198 |  115690 |      - |     8643.8
b<latereturn_flatmap_get> |      19 |     1.949 |  102563 |  0.887 |     9750.1
b<original_flatmap_get> * |      20 |     2.090 |  104480 |      - |     9571.2
b<latereturn_flatmap_get> |      20 |     1.878 |   93913 |  0.899 |    10648.1
b<original_flatmap_get> * |      21 |     2.173 |  103474 |      - |     9664.2
b<latereturn_flatmap_get> |      21 |     1.806 |   85977 |  0.831 |    11631.0
b<original_flatmap_get> * |      22 |     2.266 |  102978 |      - |     9710.8
b<latereturn_flatmap_get> |      22 |     1.825 |   82966 |  0.806 |    12053.1
b<original_flatmap_get> * |      23 |     2.203 |   95797 |      - |    10438.7
b<latereturn_flatmap_get> |      23 |     1.971 |   85686 |  0.894 |    11670.4
b<original_flatmap_get> * |      24 |     2.442 |  101731 |      - |     9829.8
b<latereturn_flatmap_get> |      24 |     2.045 |   85214 |  0.838 |    11735.1
b<original_flatmap_get> * |      25 |     2.319 |   92778 |      - |    10778.4
b<latereturn_flatmap_get> |      25 |     2.131 |   85252 |  0.919 |    11729.9
b<original_flatmap_get> * |      26 |     2.267 |   87189 |      - |    11469.2
b<latereturn_flatmap_get> |      26 |     2.159 |   83041 |  0.952 |    12042.2
b<original_flatmap_get> * |      27 |     2.485 |   92019 |      - |    10867.3
b<latereturn_flatmap_get> |      27 |     2.031 |   75237 |  0.818 |    13291.2
b<original_flatmap_get> * |      28 |     2.351 |   83968 |      - |    11909.2
b<latereturn_flatmap_get> |      28 |     2.277 |   81319 |  0.968 |    12297.2
b<original_flatmap_get> * |      29 |     2.593 |   89408 |      - |    11184.6
b<latereturn_flatmap_get> |      29 |     2.197 |   75757 |  0.847 |    13200.0
b<original_flatmap_get> * |      30 |     2.945 |   98177 |      - |    10185.6
b<latereturn_flatmap_get> |      30 |     2.189 |   72954 |  0.743 |    13707.3
b<original_flatmap_get> * |      31 |     2.927 |   94434 |      - |    10589.4
b<latereturn_flatmap_get> |      31 |     2.730 |   88080 |  0.933 |    11353.3
b<original_flatmap_get> * |      32 |     3.068 |   95870 |      - |    10430.7
b<latereturn_flatmap_get> |      32 |     2.426 |   75813 |  0.791 |    13190.3
===============================================================================
```

## Clang

### -O2


```
===============================================================================
   Name (baseline is *)   |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
===============================================================================
b<original_flatmap_get> * |       1 |     1.306 | 1305617 |      - |      765.9
b<latereturn_flatmap_get> |       1 |     0.928 |  928389 |  0.711 |     1077.1
b<original_flatmap_get> * |       2 |     1.461 |  730436 |      - |     1369.0
b<latereturn_flatmap_get> |       2 |     0.935 |  467531 |  0.640 |     2138.9
b<original_flatmap_get> * |       3 |     1.423 |  474465 |      - |     2107.6
b<latereturn_flatmap_get> |       3 |     0.964 |  321196 |  0.677 |     3113.4
b<original_flatmap_get> * |       4 |     1.462 |  365568 |      - |     2735.5
b<latereturn_flatmap_get> |       4 |     1.022 |  255380 |  0.699 |     3915.7
b<original_flatmap_get> * |       5 |     1.478 |  295600 |      - |     3382.9
b<latereturn_flatmap_get> |       5 |     1.098 |  219565 |  0.743 |     4554.5
b<original_flatmap_get> * |       6 |     1.509 |  251482 |      - |     3976.4
b<latereturn_flatmap_get> |       6 |     1.116 |  185991 |  0.740 |     5376.6
b<original_flatmap_get> * |       7 |     1.591 |  227236 |      - |     4400.7
b<latereturn_flatmap_get> |       7 |     1.138 |  162636 |  0.716 |     6148.7
b<original_flatmap_get> * |       8 |     1.647 |  205825 |      - |     4858.5
b<latereturn_flatmap_get> |       8 |     1.107 |  138348 |  0.672 |     7228.1
b<original_flatmap_get> * |       9 |     1.640 |  182221 |      - |     5487.8
b<latereturn_flatmap_get> |       9 |     1.221 |  135614 |  0.744 |     7373.9
b<original_flatmap_get> * |      10 |     2.015 |  201463 |      - |     4963.7
b<latereturn_flatmap_get> |      10 |     1.255 |  125472 |  0.623 |     7969.9
b<original_flatmap_get> * |      11 |     1.987 |  180664 |      - |     5535.1
b<latereturn_flatmap_get> |      11 |     1.366 |  124213 |  0.688 |     8050.7
b<original_flatmap_get> * |      12 |     1.798 |  149865 |      - |     6672.7
b<latereturn_flatmap_get> |      12 |     1.250 |  104162 |  0.695 |     9600.4
b<original_flatmap_get> * |      13 |     1.871 |  143905 |      - |     6949.0
b<latereturn_flatmap_get> |      13 |     1.221 |   93950 |  0.653 |    10643.9
b<original_flatmap_get> * |      14 |     1.821 |  130089 |      - |     7687.0
b<latereturn_flatmap_get> |      14 |     1.250 |   89300 |  0.686 |    11198.2
b<original_flatmap_get> * |      15 |     1.809 |  120610 |      - |     8291.2
b<latereturn_flatmap_get> |      15 |     1.307 |   87154 |  0.723 |    11473.9
b<original_flatmap_get> * |      16 |     1.887 |  117922 |      - |     8480.2
b<latereturn_flatmap_get> |      16 |     1.281 |   80034 |  0.679 |    12494.6
b<original_flatmap_get> * |      17 |     1.844 |  108480 |      - |     9218.3
b<latereturn_flatmap_get> |      17 |     1.377 |   80988 |  0.747 |    12347.4
b<original_flatmap_get> * |      18 |     1.861 |  103406 |      - |     9670.5
b<latereturn_flatmap_get> |      18 |     1.431 |   79522 |  0.769 |    12575.0
b<original_flatmap_get> * |      19 |     1.883 |   99079 |      - |    10092.9
b<latereturn_flatmap_get> |      19 |     1.647 |   86678 |  0.875 |    11536.9
b<original_flatmap_get> * |      20 |     2.023 |  101147 |      - |     9886.5
b<latereturn_flatmap_get> |      20 |     1.460 |   72980 |  0.722 |    13702.3
b<original_flatmap_get> * |      21 |     1.938 |   92276 |      - |    10837.0
b<latereturn_flatmap_get> |      21 |     1.508 |   71807 |  0.778 |    13926.1
b<original_flatmap_get> * |      22 |     2.075 |   94319 |      - |    10602.2
b<latereturn_flatmap_get> |      22 |     1.588 |   72203 |  0.766 |    13849.8
b<original_flatmap_get> * |      23 |     2.350 |  102182 |      - |     9786.4
b<latereturn_flatmap_get> |      23 |     1.773 |   77106 |  0.755 |    12969.1
b<original_flatmap_get> * |      24 |     2.016 |   84012 |      - |    11903.0
b<latereturn_flatmap_get> |      24 |     1.566 |   65241 |  0.777 |    15327.7
b<original_flatmap_get> * |      25 |     2.140 |   85586 |      - |    11684.2
b<latereturn_flatmap_get> |      25 |     1.688 |   67522 |  0.789 |    14809.9
b<original_flatmap_get> * |      26 |     2.110 |   81154 |      - |    12322.3
b<latereturn_flatmap_get> |      26 |     1.680 |   64611 |  0.796 |    15477.1
b<original_flatmap_get> * |      27 |     2.131 |   78927 |      - |    12669.9
b<latereturn_flatmap_get> |      27 |     1.738 |   64352 |  0.815 |    15539.3
b<original_flatmap_get> * |      28 |     2.176 |   77714 |      - |    12867.6
b<latereturn_flatmap_get> |      28 |     1.746 |   62362 |  0.802 |    16035.2
b<original_flatmap_get> * |      29 |     2.195 |   75682 |      - |    13213.1
b<latereturn_flatmap_get> |      29 |     1.795 |   61884 |  0.818 |    16159.3
b<original_flatmap_get> * |      30 |     2.170 |   72334 |      - |    13824.6
b<latereturn_flatmap_get> |      30 |     1.824 |   60787 |  0.840 |    16450.7
b<original_flatmap_get> * |      31 |     2.438 |   78644 |      - |    12715.4
b<latereturn_flatmap_get> |      31 |     2.066 |   66640 |  0.847 |    15005.8
b<original_flatmap_get> * |      32 |     2.519 |   78716 |      - |    12703.9
b<latereturn_flatmap_get> |      32 |     1.852 |   57880 |  0.735 |    17277.1
===============================================================================
```

### -O3

```
===============================================================================
   Name (baseline is *)   |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
===============================================================================
b<original_flatmap_get> * |       1 |     1.458 | 1457689 |      - |      686.0
b<latereturn_flatmap_get> |       1 |     0.914 |  913534 |  0.627 |     1094.7
b<original_flatmap_get> * |       2 |     1.602 |  800821 |      - |     1248.7
b<latereturn_flatmap_get> |       2 |     0.942 |  471119 |  0.588 |     2122.6
b<original_flatmap_get> * |       3 |     1.435 |  478475 |      - |     2090.0
b<latereturn_flatmap_get> |       3 |     1.164 |  388036 |  0.811 |     2577.1
b<original_flatmap_get> * |       4 |     1.576 |  394080 |      - |     2537.6
b<latereturn_flatmap_get> |       4 |     1.073 |  268206 |  0.681 |     3728.5
b<original_flatmap_get> * |       5 |     1.570 |  314007 |      - |     3184.6
b<latereturn_flatmap_get> |       5 |     1.105 |  221031 |  0.704 |     4524.2
b<original_flatmap_get> * |       6 |     1.550 |  258348 |      - |     3870.7
b<latereturn_flatmap_get> |       6 |     1.085 |  180893 |  0.700 |     5528.1
b<original_flatmap_get> * |       7 |     1.550 |  221380 |      - |     4517.1
b<latereturn_flatmap_get> |       7 |     1.143 |  163243 |  0.737 |     6125.8
b<original_flatmap_get> * |       8 |     1.519 |  189832 |      - |     5267.8
b<latereturn_flatmap_get> |       8 |     1.139 |  142344 |  0.750 |     7025.2
b<original_flatmap_get> * |       9 |     1.675 |  186101 |      - |     5373.4
b<latereturn_flatmap_get> |       9 |     1.355 |  150525 |  0.809 |     6643.4
b<original_flatmap_get> * |      10 |     1.689 |  168872 |      - |     5921.6
b<latereturn_flatmap_get> |      10 |     1.257 |  125715 |  0.744 |     7954.5
b<original_flatmap_get> * |      11 |     1.658 |  150755 |      - |     6633.3
b<latereturn_flatmap_get> |      11 |     1.406 |  127801 |  0.848 |     7824.6
b<original_flatmap_get> * |      12 |     1.934 |  161181 |      - |     6204.2
b<latereturn_flatmap_get> |      12 |     1.381 |  115122 |  0.714 |     8686.4
b<original_flatmap_get> * |      13 |     1.679 |  129159 |      - |     7742.3
b<latereturn_flatmap_get> |      13 |     1.229 |   94508 |  0.732 |    10581.1
b<original_flatmap_get> * |      14 |     1.819 |  129916 |      - |     7697.3
b<latereturn_flatmap_get> |      14 |     1.449 |  103526 |  0.797 |     9659.3
b<original_flatmap_get> * |      15 |     2.012 |  134106 |      - |     7456.7
b<latereturn_flatmap_get> |      15 |     1.429 |   95277 |  0.710 |    10495.7
b<original_flatmap_get> * |      16 |     1.961 |  122590 |      - |     8157.2
b<latereturn_flatmap_get> |      16 |     1.365 |   85307 |  0.696 |    11722.3
b<original_flatmap_get> * |      17 |     2.063 |  121370 |      - |     8239.2
b<latereturn_flatmap_get> |      17 |     1.372 |   80701 |  0.665 |    12391.3
b<original_flatmap_get> * |      18 |     1.930 |  107244 |      - |     9324.5
b<latereturn_flatmap_get> |      18 |     1.574 |   87450 |  0.815 |    11435.0
b<original_flatmap_get> * |      19 |     1.884 |   99182 |      - |    10082.5
b<latereturn_flatmap_get> |      19 |     1.487 |   78239 |  0.789 |    12781.3
b<original_flatmap_get> * |      20 |     2.116 |  105790 |      - |     9452.6
b<latereturn_flatmap_get> |      20 |     1.430 |   71484 |  0.676 |    13989.1
b<original_flatmap_get> * |      21 |     1.988 |   94648 |      - |    10565.4
b<latereturn_flatmap_get> |      21 |     1.557 |   74165 |  0.784 |    13483.3
b<original_flatmap_get> * |      22 |     2.013 |   91510 |      - |    10927.7
b<latereturn_flatmap_get> |      22 |     1.753 |   79669 |  0.871 |    12551.9
b<original_flatmap_get> * |      23 |     1.986 |   86367 |      - |    11578.4
b<latereturn_flatmap_get> |      23 |     1.604 |   69735 |  0.807 |    14339.8
b<original_flatmap_get> * |      24 |     2.309 |   96220 |      - |    10392.8
b<latereturn_flatmap_get> |      24 |     1.610 |   67065 |  0.697 |    14910.8
b<original_flatmap_get> * |      25 |     2.120 |   84794 |      - |    11793.2
b<latereturn_flatmap_get> |      25 |     1.658 |   66335 |  0.782 |    15074.8
b<original_flatmap_get> * |      26 |     2.446 |   94090 |      - |    10628.1
b<latereturn_flatmap_get> |      26 |     1.830 |   70392 |  0.748 |    14206.1
b<original_flatmap_get> * |      27 |     2.148 |   79545 |      - |    12571.4
b<latereturn_flatmap_get> |      27 |     1.745 |   64615 |  0.812 |    15476.2
b<original_flatmap_get> * |      28 |     2.348 |   83871 |      - |    11923.0
b<latereturn_flatmap_get> |      28 |     1.981 |   70759 |  0.844 |    14132.4
b<original_flatmap_get> * |      29 |     2.187 |   75416 |      - |    13259.8
b<latereturn_flatmap_get> |      29 |     1.837 |   63357 |  0.840 |    15783.5
b<original_flatmap_get> * |      30 |     2.244 |   74802 |      - |    13368.6
b<latereturn_flatmap_get> |      30 |     1.970 |   65667 |  0.878 |    15228.3
b<original_flatmap_get> * |      31 |     2.617 |   84424 |      - |    11844.9
b<latereturn_flatmap_get> |      31 |     1.888 |   60904 |  0.721 |    16419.1
b<original_flatmap_get> * |      32 |     2.417 |   75518 |      - |    13241.8
b<latereturn_flatmap_get> |      32 |     1.916 |   59881 |  0.793 |    16699.6
===============================================================================
```
