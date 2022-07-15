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

As we measure 10_000 iterations (to reduce the influence of measurement overhead on the benchmark), the 'Ops/second' number in the results
should be multiplied by 100_000 to reach the real number.

## GCC

### -O2

```
===============================================================================
   Name (baseline is *)   |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
===============================================================================
b<original_flatmap_get> * |       1 |    56.330 |56330213 |      - |       17.8
b<latereturn_flatmap_get> |       1 |    67.031 |67030519 |  1.190 |       14.9
b<original_flatmap_get> * |       2 |    66.513 |33256372 |      - |       30.1
b<latereturn_flatmap_get> |       2 |    66.810 |33404941 |  1.004 |       29.9
b<original_flatmap_get> * |       3 |    66.840 |22279963 |      - |       44.9
b<latereturn_flatmap_get> |       3 |    69.501 |23167090 |  1.040 |       43.2
b<original_flatmap_get> * |       4 |    70.788 |17697093 |      - |       56.5
b<latereturn_flatmap_get> |       4 |    65.077 |16269238 |  0.919 |       61.5
b<original_flatmap_get> * |       5 |    69.021 |13804203 |      - |       72.4
b<latereturn_flatmap_get> |       5 |    62.058 |12411542 |  0.899 |       80.6
b<original_flatmap_get> * |       6 |    69.249 |11541485 |      - |       86.6
b<latereturn_flatmap_get> |       6 |    62.836 |10472715 |  0.907 |       95.5
b<original_flatmap_get> * |       7 |    70.711 |10101516 |      - |       99.0
b<latereturn_flatmap_get> |       7 |    61.888 | 8841206 |  0.875 |      113.1
b<original_flatmap_get> * |       8 |    70.222 | 8777731 |      - |      113.9
b<latereturn_flatmap_get> |       8 |    64.443 | 8055362 |  0.918 |      124.1
b<original_flatmap_get> * |       9 |    71.687 | 7965185 |      - |      125.5
b<latereturn_flatmap_get> |       9 |    62.844 | 6982685 |  0.877 |      143.2
b<original_flatmap_get> * |      10 |    71.914 | 7191355 |      - |      139.1
b<latereturn_flatmap_get> |      10 |    59.164 | 5916360 |  0.823 |      169.0
b<original_flatmap_get> * |      11 |    70.685 | 6425896 |      - |      155.6
b<latereturn_flatmap_get> |      11 |    62.244 | 5658549 |  0.881 |      176.7
b<original_flatmap_get> * |      12 |    69.232 | 5769325 |      - |      173.3
b<latereturn_flatmap_get> |      12 |    63.511 | 5292601 |  0.917 |      188.9
b<original_flatmap_get> * |      13 |    70.657 | 5435121 |      - |      184.0
b<latereturn_flatmap_get> |      13 |    65.040 | 5003109 |  0.921 |      199.9
b<original_flatmap_get> * |      14 |    69.007 | 4929104 |      - |      202.9
b<latereturn_flatmap_get> |      14 |    60.851 | 4346513 |  0.882 |      230.1
b<original_flatmap_get> * |      15 |    69.410 | 4627304 |      - |      216.1
b<latereturn_flatmap_get> |      15 |    61.972 | 4131452 |  0.893 |      242.0
b<original_flatmap_get> * |      16 |    71.770 | 4485603 |      - |      222.9
b<latereturn_flatmap_get> |      16 |    62.133 | 3883312 |  0.866 |      257.5
b<original_flatmap_get> * |      17 |    68.661 | 4038877 |      - |      247.6
b<latereturn_flatmap_get> |      17 |    63.869 | 3756996 |  0.930 |      266.2
b<original_flatmap_get> * |      18 |    69.107 | 3839271 |      - |      260.5
b<latereturn_flatmap_get> |      18 |    61.950 | 3441662 |  0.896 |      290.6
b<original_flatmap_get> * |      19 |    71.834 | 3780747 |      - |      264.5
b<latereturn_flatmap_get> |      19 |    62.221 | 3274801 |  0.866 |      305.4
b<original_flatmap_get> * |      20 |    65.050 | 3252489 |      - |      307.5
b<latereturn_flatmap_get> |      20 |    64.977 | 3248835 |  0.999 |      307.8
b<original_flatmap_get> * |      21 |    64.724 | 3082090 |      - |      324.5
b<latereturn_flatmap_get> |      21 |    63.161 | 3007666 |  0.976 |      332.5
b<original_flatmap_get> * |      22 |    64.267 | 2921223 |      - |      342.3
b<latereturn_flatmap_get> |      22 |    63.450 | 2884072 |  0.987 |      346.7
b<original_flatmap_get> * |      23 |    63.234 | 2749287 |      - |      363.7
b<latereturn_flatmap_get> |      23 |    59.588 | 2590785 |  0.942 |      386.0
b<original_flatmap_get> * |      24 |    64.974 | 2707238 |      - |      369.4
b<latereturn_flatmap_get> |      24 |    62.547 | 2606114 |  0.963 |      383.7
b<original_flatmap_get> * |      25 |    64.244 | 2569743 |      - |      389.1
b<latereturn_flatmap_get> |      25 |    62.408 | 2496331 |  0.971 |      400.6
b<original_flatmap_get> * |      26 |    63.604 | 2446297 |      - |      408.8
b<latereturn_flatmap_get> |      26 |    63.207 | 2431031 |  0.994 |      411.3
b<original_flatmap_get> * |      27 |    62.725 | 2323159 |      - |      430.4
b<latereturn_flatmap_get> |      27 |    62.405 | 2311307 |  0.995 |      432.7
b<original_flatmap_get> * |      28 |    64.229 | 2293887 |      - |      435.9
b<latereturn_flatmap_get> |      28 |    64.331 | 2297522 |  1.002 |      435.3
b<original_flatmap_get> * |      29 |    62.591 | 2158320 |      - |      463.3
b<latereturn_flatmap_get> |      29 |    61.013 | 2103901 |  0.975 |      475.3
b<original_flatmap_get> * |      30 |    61.924 | 2064117 |      - |      484.5
b<latereturn_flatmap_get> |      30 |    64.164 | 2138784 |  1.036 |      467.6
b<original_flatmap_get> * |      31 |    63.491 | 2048098 |      - |      488.3
b<latereturn_flatmap_get> |      31 |    65.041 | 2098103 |  1.024 |      476.6
b<original_flatmap_get> * |      32 |    61.379 | 1918097 |      - |      521.3
b<latereturn_flatmap_get> |      32 |    63.009 | 1969025 |  1.027 |      507.9
===============================================================================
```

### -O3

```
===============================================================================
   Name (baseline is *)   |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
===============================================================================
b<original_flatmap_get> * |       1 |    43.706 |43706464 |      - |       22.9
b<latereturn_flatmap_get> |       1 |    47.763 |47762792 |  1.093 |       20.9
b<original_flatmap_get> * |       2 |    54.303 |27151493 |      - |       36.8
b<latereturn_flatmap_get> |       2 |    48.319 |24159732 |  0.890 |       41.4
b<original_flatmap_get> * |       3 |    56.322 |18774151 |      - |       53.3
b<latereturn_flatmap_get> |       3 |    49.174 |16391310 |  0.873 |       61.0
b<original_flatmap_get> * |       4 |    57.768 |14441992 |      - |       69.2
b<latereturn_flatmap_get> |       4 |    47.987 |11996783 |  0.831 |       83.4
b<original_flatmap_get> * |       5 |    57.951 |11590185 |      - |       86.3
b<latereturn_flatmap_get> |       5 |    49.611 | 9922208 |  0.856 |      100.8
b<original_flatmap_get> * |       6 |    59.684 | 9947379 |      - |      100.5
b<latereturn_flatmap_get> |       6 |    49.571 | 8261895 |  0.831 |      121.0
b<original_flatmap_get> * |       7 |    60.412 | 8630263 |      - |      115.9
b<latereturn_flatmap_get> |       7 |    48.413 | 6916105 |  0.801 |      144.6
b<original_flatmap_get> * |       8 |    59.540 | 7442483 |      - |      134.4
b<latereturn_flatmap_get> |       8 |    48.771 | 6096434 |  0.819 |      164.0
b<original_flatmap_get> * |       9 |    60.208 | 6689752 |      - |      149.5
b<latereturn_flatmap_get> |       9 |    47.981 | 5331190 |  0.797 |      187.6
b<original_flatmap_get> * |      10 |    61.727 | 6172698 |      - |      162.0
b<latereturn_flatmap_get> |      10 |    47.982 | 4798184 |  0.777 |      208.4
b<original_flatmap_get> * |      11 |    62.834 | 5712154 |      - |      175.1
b<latereturn_flatmap_get> |      11 |    48.642 | 4421982 |  0.774 |      226.1
b<original_flatmap_get> * |      12 |    59.988 | 4998988 |      - |      200.0
b<latereturn_flatmap_get> |      12 |    47.887 | 3990566 |  0.798 |      250.6
b<original_flatmap_get> * |      13 |    60.325 | 4640385 |      - |      215.5
b<latereturn_flatmap_get> |      13 |    48.302 | 3715567 |  0.801 |      269.1
b<original_flatmap_get> * |      14 |    57.994 | 4142442 |      - |      241.4
b<latereturn_flatmap_get> |      14 |    47.978 | 3427025 |  0.827 |      291.8
b<original_flatmap_get> * |      15 |    56.800 | 3786640 |      - |      264.1
b<latereturn_flatmap_get> |      15 |    47.890 | 3192653 |  0.843 |      313.2
b<original_flatmap_get> * |      16 |    57.245 | 3577814 |      - |      279.5
b<latereturn_flatmap_get> |      16 |    48.153 | 3009580 |  0.841 |      332.3
b<original_flatmap_get> * |      17 |    59.262 | 3486020 |      - |      286.9
b<latereturn_flatmap_get> |      17 |    49.476 | 2910364 |  0.835 |      343.6
b<original_flatmap_get> * |      18 |    55.792 | 3099541 |      - |      322.6
b<latereturn_flatmap_get> |      18 |    51.692 | 2871799 |  0.927 |      348.2
b<original_flatmap_get> * |      19 |    54.872 | 2888008 |      - |      346.3
b<latereturn_flatmap_get> |      19 |    51.587 | 2715094 |  0.940 |      368.3
b<original_flatmap_get> * |      20 |    55.367 | 2768331 |      - |      361.2
b<latereturn_flatmap_get> |      20 |    47.903 | 2395173 |  0.865 |      417.5
b<original_flatmap_get> * |      21 |    55.052 | 2621510 |      - |      381.5
b<latereturn_flatmap_get> |      21 |    49.416 | 2353126 |  0.898 |      425.0
b<original_flatmap_get> * |      22 |    56.914 | 2587008 |      - |      386.5
b<latereturn_flatmap_get> |      22 |    47.912 | 2177836 |  0.842 |      459.2
b<original_flatmap_get> * |      23 |    53.948 | 2345567 |      - |      426.3
b<latereturn_flatmap_get> |      23 |    48.078 | 2090336 |  0.891 |      478.4
b<original_flatmap_get> * |      24 |    54.174 | 2257233 |      - |      443.0
b<latereturn_flatmap_get> |      24 |    47.987 | 1999449 |  0.886 |      500.1
b<original_flatmap_get> * |      25 |    53.506 | 2140225 |      - |      467.2
b<latereturn_flatmap_get> |      25 |    50.877 | 2035077 |  0.951 |      491.4
b<original_flatmap_get> * |      26 |    54.163 | 2083201 |      - |      480.0
b<latereturn_flatmap_get> |      26 |    48.263 | 1856257 |  0.891 |      538.7
b<original_flatmap_get> * |      27 |    52.591 | 1947797 |      - |      513.4
b<latereturn_flatmap_get> |      27 |    50.319 | 1863648 |  0.957 |      536.6
b<original_flatmap_get> * |      28 |    54.367 | 1941691 |      - |      515.0
b<latereturn_flatmap_get> |      28 |    47.766 | 1705924 |  0.879 |      586.2
b<original_flatmap_get> * |      29 |    51.557 | 1777824 |      - |      562.5
b<latereturn_flatmap_get> |      29 |    50.865 | 1753970 |  0.987 |      570.1
b<original_flatmap_get> * |      30 |    53.559 | 1785312 |      - |      560.1
b<latereturn_flatmap_get> |      30 |    47.989 | 1599643 |  0.896 |      625.1
b<original_flatmap_get> * |      31 |    51.216 | 1652115 |      - |      605.3
b<latereturn_flatmap_get> |      31 |    47.635 | 1536603 |  0.930 |      650.8
b<original_flatmap_get> * |      32 |    50.990 | 1593425 |      - |      627.6
b<latereturn_flatmap_get> |      32 |    48.204 | 1506372 |  0.945 |      663.8
===============================================================================
```

## Clang

## -O2

```
===============================================================================
   Name (baseline is *)   |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
===============================================================================
b<original_flatmap_get> * |       1 |    46.770 |46769922 |      - |       21.4
b<latereturn_flatmap_get> |       1 |    47.079 |47079060 |  1.007 |       21.2
b<original_flatmap_get> * |       2 |    55.057 |27528680 |      - |       36.3
b<latereturn_flatmap_get> |       2 |    45.343 |22671299 |  0.824 |       44.1
b<original_flatmap_get> * |       3 |    56.595 |18864985 |      - |       53.0
b<latereturn_flatmap_get> |       3 |    44.530 |14843421 |  0.787 |       67.4
b<original_flatmap_get> * |       4 |    59.735 |14933747 |      - |       67.0
b<latereturn_flatmap_get> |       4 |    46.295 |11573647 |  0.775 |       86.4
b<original_flatmap_get> * |       5 |    58.731 |11746181 |      - |       85.1
b<latereturn_flatmap_get> |       5 |    45.822 | 9164421 |  0.780 |      109.1
b<original_flatmap_get> * |       6 |    60.532 |10088606 |      - |       99.1
b<latereturn_flatmap_get> |       6 |    45.646 | 7607647 |  0.754 |      131.4
b<original_flatmap_get> * |       7 |    60.324 | 8617721 |      - |      116.0
b<latereturn_flatmap_get> |       7 |    44.903 | 6414694 |  0.744 |      155.9
b<original_flatmap_get> * |       8 |    63.901 | 7987623 |      - |      125.2
b<latereturn_flatmap_get> |       8 |    48.198 | 6024798 |  0.754 |      166.0
b<original_flatmap_get> * |       9 |    61.424 | 6824863 |      - |      146.5
b<latereturn_flatmap_get> |       9 |    45.623 | 5069218 |  0.743 |      197.3
b<original_flatmap_get> * |      10 |    63.081 | 6308123 |      - |      158.5
b<latereturn_flatmap_get> |      10 |    44.532 | 4453235 |  0.706 |      224.6
b<original_flatmap_get> * |      11 |    59.971 | 5451949 |      - |      183.4
b<latereturn_flatmap_get> |      11 |    44.966 | 4087786 |  0.750 |      244.6
b<original_flatmap_get> * |      12 |    59.304 | 4941972 |      - |      202.3
b<latereturn_flatmap_get> |      12 |    46.240 | 3853370 |  0.780 |      259.5
b<original_flatmap_get> * |      13 |    62.232 | 4787079 |      - |      208.9
b<latereturn_flatmap_get> |      13 |    45.010 | 3462342 |  0.723 |      288.8
b<original_flatmap_get> * |      14 |    58.288 | 4163404 |      - |      240.2
b<latereturn_flatmap_get> |      14 |    44.783 | 3198753 |  0.768 |      312.6
b<original_flatmap_get> * |      15 |    59.483 | 3965516 |      - |      252.2
b<latereturn_flatmap_get> |      15 |    44.452 | 2963460 |  0.747 |      337.4
b<original_flatmap_get> * |      16 |    57.496 | 3593507 |      - |      278.3
b<latereturn_flatmap_get> |      16 |    44.407 | 2775416 |  0.772 |      360.3
b<original_flatmap_get> * |      17 |    56.406 | 3318021 |      - |      301.4
b<latereturn_flatmap_get> |      17 |    44.461 | 2615374 |  0.788 |      382.4
b<original_flatmap_get> * |      18 |    56.097 | 3116484 |      - |      320.9
b<latereturn_flatmap_get> |      18 |    46.290 | 2571694 |  0.825 |      388.8
b<original_flatmap_get> * |      19 |    58.424 | 3074934 |      - |      325.2
b<latereturn_flatmap_get> |      19 |    47.107 | 2479316 |  0.806 |      403.3
b<original_flatmap_get> * |      20 |    55.735 | 2786748 |      - |      358.8
b<latereturn_flatmap_get> |      20 |    45.635 | 2281740 |  0.819 |      438.3
b<original_flatmap_get> * |      21 |    55.591 | 2647184 |      - |      377.8
b<latereturn_flatmap_get> |      21 |    45.226 | 2153620 |  0.814 |      464.3
b<original_flatmap_get> * |      22 |    56.256 | 2557104 |      - |      391.1
b<latereturn_flatmap_get> |      22 |    46.333 | 2106056 |  0.824 |      474.8
b<original_flatmap_get> * |      23 |    53.965 | 2346321 |      - |      426.2
b<latereturn_flatmap_get> |      23 |    46.151 | 2006560 |  0.855 |      498.4
b<original_flatmap_get> * |      24 |    52.934 | 2205577 |      - |      453.4
b<latereturn_flatmap_get> |      24 |    45.686 | 1903595 |  0.863 |      525.3
b<original_flatmap_get> * |      25 |    52.797 | 2111880 |      - |      473.5
b<latereturn_flatmap_get> |      25 |    46.626 | 1865024 |  0.883 |      536.2
b<original_flatmap_get> * |      26 |    52.425 | 2016337 |      - |      495.9
b<latereturn_flatmap_get> |      26 |    44.951 | 1728898 |  0.857 |      578.4
b<original_flatmap_get> * |      27 |    54.265 | 2009797 |      - |      497.6
b<latereturn_flatmap_get> |      27 |    44.992 | 1666371 |  0.829 |      600.1
b<original_flatmap_get> * |      28 |    52.115 | 1861245 |      - |      537.3
b<latereturn_flatmap_get> |      28 |    44.418 | 1586341 |  0.852 |      630.4
b<original_flatmap_get> * |      29 |    51.361 | 1771070 |      - |      564.6
b<latereturn_flatmap_get> |      29 |    44.420 | 1531740 |  0.865 |      652.9
b<original_flatmap_get> * |      30 |    51.906 | 1730188 |      - |      578.0
b<latereturn_flatmap_get> |      30 |    46.362 | 1545400 |  0.893 |      647.1
b<original_flatmap_get> * |      31 |    50.910 | 1642252 |      - |      608.9
b<latereturn_flatmap_get> |      31 |    47.629 | 1536420 |  0.936 |      650.9
b<original_flatmap_get> * |      32 |    50.166 | 1567685 |      - |      637.9
b<latereturn_flatmap_get> |      32 |    45.643 | 1426359 |  0.910 |      701.1
===============================================================================
```

## -O3

```
===============================================================================
   Name (baseline is *)   |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
===============================================================================
b<original_flatmap_get> * |       1 |    44.550 |44550364 |      - |       22.4
b<latereturn_flatmap_get> |       1 |    48.114 |48113963 |  1.080 |       20.8
b<original_flatmap_get> * |       2 |    51.308 |25653960 |      - |       39.0
b<latereturn_flatmap_get> |       2 |    46.015 |23007446 |  0.897 |       43.5
b<original_flatmap_get> * |       3 |    51.937 |17312202 |      - |       57.8
b<latereturn_flatmap_get> |       3 |    45.513 |15171084 |  0.876 |       65.9
b<original_flatmap_get> * |       4 |    55.436 |13858890 |      - |       72.2
b<latereturn_flatmap_get> |       4 |    45.146 |11286623 |  0.814 |       88.6
b<original_flatmap_get> * |       5 |    56.980 |11396079 |      - |       87.7
b<latereturn_flatmap_get> |       5 |    47.670 | 9534099 |  0.837 |      104.9
b<original_flatmap_get> * |       6 |    55.744 | 9290604 |      - |      107.6
b<latereturn_flatmap_get> |       6 |    47.407 | 7901129 |  0.850 |      126.6
b<original_flatmap_get> * |       7 |    57.951 | 8278669 |      - |      120.8
b<latereturn_flatmap_get> |       7 |    45.071 | 6438646 |  0.778 |      155.3
b<original_flatmap_get> * |       8 |    59.488 | 7436057 |      - |      134.5
b<latereturn_flatmap_get> |       8 |    45.051 | 5631414 |  0.757 |      177.6
b<original_flatmap_get> * |       9 |    59.512 | 6612438 |      - |      151.2
b<latereturn_flatmap_get> |       9 |    45.250 | 5027814 |  0.760 |      198.9
b<original_flatmap_get> * |      10 |    58.285 | 5828527 |      - |      171.6
b<latereturn_flatmap_get> |      10 |    45.396 | 4539564 |  0.779 |      220.3
b<original_flatmap_get> * |      11 |    57.873 | 5261143 |      - |      190.1
b<latereturn_flatmap_get> |      11 |    44.588 | 4053458 |  0.770 |      246.7
b<original_flatmap_get> * |      12 |    58.260 | 4854964 |      - |      206.0
b<latereturn_flatmap_get> |      12 |    48.497 | 4041418 |  0.832 |      247.4
b<original_flatmap_get> * |      13 |    57.951 | 4457792 |      - |      224.3
b<latereturn_flatmap_get> |      13 |    45.105 | 3469604 |  0.778 |      288.2
b<original_flatmap_get> * |      14 |    59.276 | 4233997 |      - |      236.2
b<latereturn_flatmap_get> |      14 |    46.210 | 3300732 |  0.780 |      303.0
b<original_flatmap_get> * |      15 |    56.763 | 3784173 |      - |      264.3
b<latereturn_flatmap_get> |      15 |    46.416 | 3094412 |  0.818 |      323.2
b<original_flatmap_get> * |      16 |    57.980 | 3623744 |      - |      276.0
b<latereturn_flatmap_get> |      16 |    47.381 | 2961290 |  0.817 |      337.7
b<original_flatmap_get> * |      17 |    55.287 | 3252162 |      - |      307.5
b<latereturn_flatmap_get> |      17 |    44.898 | 2641043 |  0.812 |      378.6
b<original_flatmap_get> * |      18 |    53.983 | 2999028 |      - |      333.4
b<latereturn_flatmap_get> |      18 |    45.237 | 2513183 |  0.838 |      397.9
b<original_flatmap_get> * |      19 |    53.759 | 2829437 |      - |      353.4
b<latereturn_flatmap_get> |      19 |    45.336 | 2386086 |  0.843 |      419.1
b<original_flatmap_get> * |      20 |    53.021 | 2651044 |      - |      377.2
b<latereturn_flatmap_get> |      20 |    45.795 | 2289730 |  0.864 |      436.7
b<original_flatmap_get> * |      21 |    52.298 | 2490403 |      - |      401.5
b<latereturn_flatmap_get> |      21 |    47.028 | 2239451 |  0.899 |      446.5
b<original_flatmap_get> * |      22 |    54.781 | 2490058 |      - |      401.6
b<latereturn_flatmap_get> |      22 |    45.306 | 2059360 |  0.827 |      485.6
b<original_flatmap_get> * |      23 |    51.684 | 2247140 |      - |      445.0
b<latereturn_flatmap_get> |      23 |    45.560 | 1980855 |  0.882 |      504.8
b<original_flatmap_get> * |      24 |    52.354 | 2181434 |      - |      458.4
b<latereturn_flatmap_get> |      24 |    44.741 | 1864213 |  0.855 |      536.4
b<original_flatmap_get> * |      25 |    52.909 | 2116362 |      - |      472.5
b<latereturn_flatmap_get> |      25 |    45.727 | 1829083 |  0.864 |      546.7
b<original_flatmap_get> * |      26 |    50.554 | 1944395 |      - |      514.3
b<latereturn_flatmap_get> |      26 |    44.848 | 1724911 |  0.887 |      579.7
b<original_flatmap_get> * |      27 |    50.692 | 1877484 |      - |      532.6
b<latereturn_flatmap_get> |      27 |    45.017 | 1667306 |  0.888 |      599.8
b<original_flatmap_get> * |      28 |    55.383 | 1977965 |      - |      505.6
b<latereturn_flatmap_get> |      28 |    47.286 | 1688776 |  0.854 |      592.1
b<original_flatmap_get> * |      29 |    49.792 | 1716960 |      - |      582.4
b<latereturn_flatmap_get> |      29 |    45.467 | 1567820 |  0.913 |      637.8
b<original_flatmap_get> * |      30 |    51.587 | 1719561 |      - |      581.5
b<latereturn_flatmap_get> |      30 |    48.422 | 1614067 |  0.939 |      619.6
b<original_flatmap_get> * |      31 |    50.444 | 1627220 |      - |      614.5
b<latereturn_flatmap_get> |      31 |    47.782 | 1541349 |  0.947 |      648.8
b<original_flatmap_get> * |      32 |    49.033 | 1532295 |      - |      652.6
b<latereturn_flatmap_get> |      32 |    45.570 | 1424053 |  0.929 |      702.2
===============================================================================
```
