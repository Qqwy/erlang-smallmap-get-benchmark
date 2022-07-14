# Benchmarking Erlang map element internals

This is related to [issue #6139](https://github.com/erlang/otp/issues/6139) on the Erlang/OTP repository.

It is an exploratory benchmark to compare different ways of iterating over a 'flatmap', which is a collection
of at most 32 keys (and 32 values).

The challenge is to look up the value for a key that matches a given input key.


# Building and running

Prerequisites: gcc, g++, clang, clang++, make.

This project uses the 'picobenchmark' microbenchmarking library, which is included as single-header in the repo.

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

The particular benchmark will, for each map size between 1 and 32, run 1000 iterations in which a random key in _twice_ this range is picked.
This means there is a 50% chance of finding the key.

This procedure is repeated for each of the potential map retrieval function implementations.

By default, two implementations which I came up with which turn out to be quite a bit slower on my machine than the original implementation
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

I have the following results:

### ClangO2
```
===============================================================================
   Name (baseline is *)   |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
===============================================================================
b<original_flatmap_get> * |       1 |     0.432 |  431913 |      - |     2315.3
b<latereturn_flatmap_get> |       1 |     0.432 |  432137 |  1.001 |     2314.1
b<original_flatmap_get> * |       2 |     1.058 |  529152 |      - |     1889.8
b<latereturn_flatmap_get> |       2 |     0.861 |  430463 |  0.813 |     2323.1
b<original_flatmap_get> * |       3 |     1.669 |  556497 |      - |     1797.0
b<latereturn_flatmap_get> |       3 |     1.373 |  457657 |  0.822 |     2185.0
b<original_flatmap_get> * |       4 |     2.356 |  589012 |      - |     1697.8
b<latereturn_flatmap_get> |       4 |     1.859 |  464687 |  0.789 |     2152.0
b<original_flatmap_get> * |       5 |     2.984 |  596893 |      - |     1675.3
b<latereturn_flatmap_get> |       5 |     2.193 |  438596 |  0.735 |     2280.0
b<original_flatmap_get> * |       6 |     3.772 |  628674 |      - |     1590.6
b<latereturn_flatmap_get> |       6 |     2.652 |  441970 |  0.703 |     2262.6
b<original_flatmap_get> * |       7 |     4.233 |  604775 |      - |     1653.5
b<latereturn_flatmap_get> |       7 |     3.270 |  467149 |  0.772 |     2140.6
b<original_flatmap_get> * |       8 |     5.074 |  634240 |      - |     1576.7
b<latereturn_flatmap_get> |       8 |     3.550 |  443708 |  0.700 |     2253.7
b<original_flatmap_get> * |       9 |     5.593 |  621443 |      - |     1609.2
b<latereturn_flatmap_get> |       9 |     4.098 |  455388 |  0.733 |     2195.9
b<original_flatmap_get> * |      10 |     6.239 |  623852 |      - |     1602.9
b<latereturn_flatmap_get> |      10 |     4.496 |  449560 |  0.721 |     2224.4
b<original_flatmap_get> * |      11 |     6.810 |  619078 |      - |     1615.3
b<latereturn_flatmap_get> |      11 |     5.224 |  474947 |  0.767 |     2105.5
b<original_flatmap_get> * |      12 |     7.278 |  606480 |      - |     1648.9
b<latereturn_flatmap_get> |      12 |     5.227 |  435553 |  0.718 |     2295.9
b<original_flatmap_get> * |      13 |     8.412 |  647082 |      - |     1545.4
b<latereturn_flatmap_get> |      13 |     5.874 |  451874 |  0.698 |     2213.0
b<original_flatmap_get> * |      14 |     8.549 |  610653 |      - |     1637.6
b<latereturn_flatmap_get> |      14 |     6.211 |  443613 |  0.726 |     2254.2
b<original_flatmap_get> * |      15 |     9.345 |  622973 |      - |     1605.2
b<latereturn_flatmap_get> |      15 |     6.650 |  443355 |  0.712 |     2255.5
b<original_flatmap_get> * |      16 |     9.723 |  607704 |      - |     1645.5
b<latereturn_flatmap_get> |      16 |     6.990 |  436902 |  0.719 |     2288.8
b<original_flatmap_get> * |      17 |    10.844 |  637906 |      - |     1567.6
b<latereturn_flatmap_get> |      17 |     7.423 |  436651 |  0.685 |     2290.2
b<original_flatmap_get> * |      18 |    10.791 |  599516 |      - |     1668.0
b<latereturn_flatmap_get> |      18 |     7.923 |  440188 |  0.734 |     2271.8
b<original_flatmap_get> * |      19 |    11.104 |  584402 |      - |     1711.2
b<latereturn_flatmap_get> |      19 |     8.413 |  442763 |  0.758 |     2258.5
b<original_flatmap_get> * |      20 |    11.773 |  588639 |      - |     1698.8
b<latereturn_flatmap_get> |      20 |     9.148 |  457388 |  0.777 |     2186.3
b<original_flatmap_get> * |      21 |    12.252 |  583422 |      - |     1714.0
b<latereturn_flatmap_get> |      21 |     9.458 |  450383 |  0.772 |     2220.3
b<original_flatmap_get> * |      22 |    13.069 |  594043 |      - |     1683.4
b<latereturn_flatmap_get> |      22 |    10.376 |  471655 |  0.794 |     2120.2
b<original_flatmap_get> * |      23 |    13.825 |  601095 |      - |     1663.6
b<latereturn_flatmap_get> |      23 |    10.293 |  447516 |  0.745 |     2234.6
b<original_flatmap_get> * |      24 |    13.790 |  574565 |      - |     1740.4
b<latereturn_flatmap_get> |      24 |    10.765 |  448558 |  0.781 |     2229.4
b<original_flatmap_get> * |      25 |    14.720 |  588802 |      - |     1698.4
b<latereturn_flatmap_get> |      25 |    10.887 |  435481 |  0.740 |     2296.3
b<original_flatmap_get> * |      26 |    14.977 |  576057 |      - |     1735.9
b<latereturn_flatmap_get> |      26 |    11.471 |  441181 |  0.766 |     2266.6
b<original_flatmap_get> * |      27 |    15.814 |  585699 |      - |     1707.4
b<latereturn_flatmap_get> |      27 |    13.093 |  484922 |  0.828 |     2062.2
b<original_flatmap_get> * |      28 |    16.019 |  572114 |      - |     1747.9
b<latereturn_flatmap_get> |      28 |    12.322 |  440056 |  0.769 |     2272.4
b<original_flatmap_get> * |      29 |    17.145 |  591198 |      - |     1691.5
b<latereturn_flatmap_get> |      29 |    12.727 |  438872 |  0.742 |     2278.6
b<original_flatmap_get> * |      30 |    17.191 |  573038 |      - |     1745.1
b<latereturn_flatmap_get> |      30 |    14.105 |  470158 |  0.820 |     2126.9
b<original_flatmap_get> * |      31 |    18.057 |  582470 |      - |     1716.8
b<latereturn_flatmap_get> |      31 |    13.609 |  438994 |  0.754 |     2277.9
b<original_flatmap_get> * |      32 |    18.128 |  566504 |      - |     1765.2
b<latereturn_flatmap_get> |      32 |    14.035 |  438582 |  0.774 |     2280.1
===============================================================================
```


### ClangO3

```

===============================================================================
   Name (baseline is *)   |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
===============================================================================
b<original_flatmap_get> * |       1 |     0.418 |  418301 |      - |     2390.6
b<latereturn_flatmap_get> |       1 |     0.436 |  436286 |  1.043 |     2292.1
b<original_flatmap_get> * |       2 |     1.026 |  512970 |      - |     1949.4
b<latereturn_flatmap_get> |       2 |     0.946 |  472771 |  0.922 |     2115.2
b<original_flatmap_get> * |       3 |     1.614 |  537901 |      - |     1859.1
b<latereturn_flatmap_get> |       3 |     1.304 |  434798 |  0.808 |     2299.9
b<original_flatmap_get> * |       4 |     2.126 |  531403 |      - |     1881.8
b<latereturn_flatmap_get> |       4 |     1.798 |  449390 |  0.846 |     2225.2
b<original_flatmap_get> * |       5 |     2.778 |  555682 |      - |     1799.6
b<latereturn_flatmap_get> |       5 |     2.261 |  452201 |  0.814 |     2211.4
b<original_flatmap_get> * |       6 |     3.563 |  593787 |      - |     1684.1
b<latereturn_flatmap_get> |       6 |     2.763 |  460449 |  0.775 |     2171.8
b<original_flatmap_get> * |       7 |     4.100 |  585744 |      - |     1707.2
b<latereturn_flatmap_get> |       7 |     3.077 |  439580 |  0.750 |     2274.9
b<original_flatmap_get> * |       8 |     4.979 |  622429 |      - |     1606.6
b<latereturn_flatmap_get> |       8 |     3.512 |  439038 |  0.705 |     2277.7
b<original_flatmap_get> * |       9 |     5.387 |  598607 |      - |     1670.5
b<latereturn_flatmap_get> |       9 |     3.947 |  438574 |  0.733 |     2280.1
b<original_flatmap_get> * |      10 |     6.263 |  626336 |      - |     1596.6
b<latereturn_flatmap_get> |      10 |     4.413 |  441273 |  0.705 |     2266.2
b<original_flatmap_get> * |      11 |     6.694 |  608587 |      - |     1643.1
b<latereturn_flatmap_get> |      11 |     4.848 |  440748 |  0.724 |     2268.9
b<original_flatmap_get> * |      12 |     6.967 |  580615 |      - |     1722.3
b<latereturn_flatmap_get> |      12 |     6.044 |  503649 |  0.867 |     1985.5
b<original_flatmap_get> * |      13 |     7.702 |  592471 |      - |     1687.8
b<latereturn_flatmap_get> |      13 |     5.756 |  442733 |  0.747 |     2258.7
b<original_flatmap_get> * |      14 |     8.493 |  606637 |      - |     1648.4
b<latereturn_flatmap_get> |      14 |     6.277 |  448362 |  0.739 |     2230.3
b<original_flatmap_get> * |      15 |     8.780 |  585302 |      - |     1708.5
b<latereturn_flatmap_get> |      15 |     6.597 |  439798 |  0.751 |     2273.8
b<original_flatmap_get> * |      16 |     9.937 |  621056 |      - |     1610.2
b<latereturn_flatmap_get> |      16 |     7.175 |  448432 |  0.722 |     2230.0
b<original_flatmap_get> * |      17 |    10.392 |  611290 |      - |     1635.9
b<latereturn_flatmap_get> |      17 |     7.568 |  445149 |  0.728 |     2246.4
b<original_flatmap_get> * |      18 |    10.960 |  608892 |      - |     1642.3
b<latereturn_flatmap_get> |      18 |     8.051 |  447273 |  0.735 |     2235.8
b<original_flatmap_get> * |      19 |    11.149 |  586783 |      - |     1704.2
b<latereturn_flatmap_get> |      19 |     8.371 |  440558 |  0.751 |     2269.8
b<original_flatmap_get> * |      20 |    11.570 |  578493 |      - |     1728.6
b<latereturn_flatmap_get> |      20 |     8.802 |  440104 |  0.761 |     2272.2
b<original_flatmap_get> * |      21 |    12.647 |  602243 |      - |     1660.5
b<latereturn_flatmap_get> |      21 |     9.905 |  471673 |  0.783 |     2120.1
b<original_flatmap_get> * |      22 |    13.503 |  613760 |      - |     1629.3
b<latereturn_flatmap_get> |      22 |    11.190 |  508618 |  0.829 |     1966.1
b<original_flatmap_get> * |      23 |    14.558 |  632966 |      - |     1579.9
b<latereturn_flatmap_get> |      23 |    10.114 |  439726 |  0.695 |     2274.1
b<original_flatmap_get> * |      24 |    14.517 |  604872 |      - |     1653.2
b<latereturn_flatmap_get> |      24 |    11.199 |  466634 |  0.771 |     2143.0
b<original_flatmap_get> * |      25 |    14.794 |  591747 |      - |     1689.9
b<latereturn_flatmap_get> |      25 |    11.215 |  448611 |  0.758 |     2229.1
b<original_flatmap_get> * |      26 |    14.667 |  564123 |      - |     1772.7
b<latereturn_flatmap_get> |      26 |    11.495 |  442131 |  0.784 |     2261.8
b<original_flatmap_get> * |      27 |    15.536 |  575414 |      - |     1737.9
b<latereturn_flatmap_get> |      27 |    12.327 |  456571 |  0.793 |     2190.2
b<original_flatmap_get> * |      28 |    15.719 |  561397 |      - |     1781.3
b<latereturn_flatmap_get> |      28 |    12.957 |  462763 |  0.824 |     2160.9
b<original_flatmap_get> * |      29 |    16.766 |  578121 |      - |     1729.7
b<latereturn_flatmap_get> |      29 |    13.349 |  460303 |  0.796 |     2172.5
b<original_flatmap_get> * |      30 |    18.129 |  604291 |      - |     1654.8
b<latereturn_flatmap_get> |      30 |    13.465 |  448843 |  0.743 |     2227.9
b<original_flatmap_get> * |      31 |    17.305 |  558229 |      - |     1791.4
b<latereturn_flatmap_get> |      31 |    14.427 |  465379 |  0.834 |     2148.8
b<original_flatmap_get> * |      32 |    17.981 |  561907 |      - |     1779.7
b<latereturn_flatmap_get> |      32 |    14.150 |  442200 |  0.787 |     2261.4
===============================================================================
```



### gccO2

```
===============================================================================
   Name (baseline is *)   |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
===============================================================================
b<original_flatmap_get> * |       1 |     0.512 |  512433 |      - |     1951.5
b<latereturn_flatmap_get> |       1 |     0.538 |  538481 |  1.051 |     1857.1
b<original_flatmap_get> * |       2 |     1.203 |  601529 |      - |     1662.4
b<latereturn_flatmap_get> |       2 |     1.074 |  537234 |  0.893 |     1861.4
b<original_flatmap_get> * |       3 |     1.888 |  629456 |      - |     1588.7
b<latereturn_flatmap_get> |       3 |     1.611 |  536942 |  0.853 |     1862.4
b<original_flatmap_get> * |       4 |     2.607 |  651828 |      - |     1534.1
b<latereturn_flatmap_get> |       4 |     2.152 |  538105 |  0.826 |     1858.4
b<original_flatmap_get> * |       5 |     3.303 |  660655 |      - |     1513.6
b<latereturn_flatmap_get> |       5 |     2.770 |  553963 |  0.839 |     1805.2
b<original_flatmap_get> * |       6 |     4.207 |  701124 |      - |     1426.3
b<latereturn_flatmap_get> |       6 |     3.253 |  542134 |  0.773 |     1844.6
b<original_flatmap_get> * |       7 |     4.723 |  674719 |      - |     1482.1
b<latereturn_flatmap_get> |       7 |     3.810 |  544269 |  0.807 |     1837.3
b<original_flatmap_get> * |       8 |     5.793 |  724076 |      - |     1381.1
b<latereturn_flatmap_get> |       8 |     4.396 |  549560 |  0.759 |     1819.6
b<original_flatmap_get> * |       9 |     6.145 |  682771 |      - |     1464.6
b<latereturn_flatmap_get> |       9 |     4.931 |  547901 |  0.802 |     1825.1
b<original_flatmap_get> * |      10 |     6.688 |  668829 |      - |     1495.2
b<latereturn_flatmap_get> |      10 |     5.469 |  546887 |  0.818 |     1828.5
b<original_flatmap_get> * |      11 |     7.416 |  674159 |      - |     1483.3
b<latereturn_flatmap_get> |      11 |     6.040 |  549110 |  0.815 |     1821.1
b<original_flatmap_get> * |      12 |     8.140 |  678306 |      - |     1474.3
b<latereturn_flatmap_get> |      12 |     6.561 |  546742 |  0.806 |     1829.0
b<original_flatmap_get> * |      13 |     8.707 |  669786 |      - |     1493.0
b<latereturn_flatmap_get> |      13 |     7.127 |  548246 |  0.819 |     1824.0
b<original_flatmap_get> * |      14 |     9.317 |  665479 |      - |     1502.7
b<latereturn_flatmap_get> |      14 |     7.667 |  547627 |  0.823 |     1826.1
b<original_flatmap_get> * |      15 |     9.834 |  655605 |      - |     1525.3
b<latereturn_flatmap_get> |      15 |     8.312 |  554139 |  0.845 |     1804.6
b<original_flatmap_get> * |      16 |    10.451 |  653205 |      - |     1530.9
b<latereturn_flatmap_get> |      16 |     8.847 |  552933 |  0.846 |     1808.5
b<original_flatmap_get> * |      17 |    11.172 |  657181 |      - |     1521.6
b<latereturn_flatmap_get> |      17 |     9.335 |  549119 |  0.836 |     1821.1
b<original_flatmap_get> * |      18 |    11.748 |  652693 |      - |     1532.1
b<latereturn_flatmap_get> |      18 |     9.837 |  546489 |  0.837 |     1829.9
b<original_flatmap_get> * |      19 |    12.184 |  641275 |      - |     1559.4
b<latereturn_flatmap_get> |      19 |    10.431 |  548976 |  0.856 |     1821.6
b<original_flatmap_get> * |      20 |    12.597 |  629834 |      - |     1587.7
b<latereturn_flatmap_get> |      20 |    10.921 |  546072 |  0.867 |     1831.3
b<original_flatmap_get> * |      21 |    13.296 |  633134 |      - |     1579.4
b<latereturn_flatmap_get> |      21 |    12.406 |  590756 |  0.933 |     1692.7
b<original_flatmap_get> * |      22 |    13.797 |  627145 |      - |     1594.5
b<latereturn_flatmap_get> |      22 |    12.417 |  564428 |  0.900 |     1771.7
b<original_flatmap_get> * |      23 |    14.231 |  618719 |      - |     1616.2
b<latereturn_flatmap_get> |      23 |    13.028 |  566420 |  0.915 |     1765.5
b<original_flatmap_get> * |      24 |    14.769 |  615374 |      - |     1625.0
b<latereturn_flatmap_get> |      24 |    13.395 |  558110 |  0.907 |     1791.8
b<original_flatmap_get> * |      25 |    15.085 |  603408 |      - |     1657.3
b<latereturn_flatmap_get> |      25 |    13.821 |  552844 |  0.916 |     1808.8
b<original_flatmap_get> * |      26 |    16.007 |  615654 |      - |     1624.3
b<latereturn_flatmap_get> |      26 |    14.257 |  548353 |  0.891 |     1823.6
b<original_flatmap_get> * |      27 |    16.649 |  616635 |      - |     1621.7
b<latereturn_flatmap_get> |      27 |    14.755 |  546486 |  0.886 |     1829.9
b<original_flatmap_get> * |      28 |    17.047 |  608814 |      - |     1642.5
b<latereturn_flatmap_get> |      28 |    15.427 |  550957 |  0.905 |     1815.0
b<original_flatmap_get> * |      29 |    17.603 |  607008 |      - |     1647.4
b<latereturn_flatmap_get> |      29 |    16.747 |  577476 |  0.951 |     1731.7
b<original_flatmap_get> * |      30 |    18.388 |  612941 |      - |     1631.5
b<latereturn_flatmap_get> |      30 |    17.132 |  571073 |  0.932 |     1751.1
b<original_flatmap_get> * |      31 |    18.709 |  603523 |      - |     1656.9
b<latereturn_flatmap_get> |      31 |    17.133 |  552670 |  0.916 |     1809.4
b<original_flatmap_get> * |      32 |    18.993 |  593539 |      - |     1684.8
b<latereturn_flatmap_get> |      32 |    17.758 |  554931 |  0.935 |     1802.0
===============================================================================
```

### gccO3

```
===============================================================================
   Name (baseline is *)   |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
===============================================================================
b<original_flatmap_get> * |       1 |     0.426 |  426457 |      - |     2344.9
b<latereturn_flatmap_get> |       1 |     0.525 |  524715 |  1.230 |     1905.8
b<original_flatmap_get> * |       2 |     1.027 |  513449 |      - |     1947.6
b<latereturn_flatmap_get> |       2 |     1.105 |  552685 |  1.076 |     1809.3
b<original_flatmap_get> * |       3 |     1.785 |  594893 |      - |     1681.0
b<latereturn_flatmap_get> |       3 |     1.623 |  541146 |  0.910 |     1847.9
b<original_flatmap_get> * |       4 |     2.366 |  591406 |      - |     1690.9
b<latereturn_flatmap_get> |       4 |     2.119 |  529646 |  0.896 |     1888.1
b<original_flatmap_get> * |       5 |     3.055 |  611059 |      - |     1636.5
b<latereturn_flatmap_get> |       5 |     2.788 |  557651 |  0.913 |     1793.2
b<original_flatmap_get> * |       6 |     3.624 |  604040 |      - |     1655.5
b<latereturn_flatmap_get> |       6 |     3.264 |  544056 |  0.901 |     1838.0
b<original_flatmap_get> * |       7 |     4.163 |  594721 |      - |     1681.5
b<latereturn_flatmap_get> |       7 |     3.742 |  534593 |  0.899 |     1870.6
b<original_flatmap_get> * |       8 |     4.963 |  620316 |      - |     1612.1
b<latereturn_flatmap_get> |       8 |     4.689 |  586084 |  0.945 |     1706.2
b<original_flatmap_get> * |       9 |     5.377 |  597402 |      - |     1673.9
b<latereturn_flatmap_get> |       9 |     4.856 |  539548 |  0.903 |     1853.4
b<original_flatmap_get> * |      10 |     5.982 |  598223 |      - |     1671.6
b<latereturn_flatmap_get> |      10 |     6.275 |  627454 |  1.049 |     1593.7
b<original_flatmap_get> * |      11 |     6.580 |  598208 |      - |     1671.7
b<latereturn_flatmap_get> |      11 |     6.080 |  552737 |  0.924 |     1809.2
b<original_flatmap_get> * |      12 |     7.329 |  610780 |      - |     1637.2
b<latereturn_flatmap_get> |      12 |     6.457 |  538089 |  0.881 |     1858.4
b<original_flatmap_get> * |      13 |     7.731 |  594696 |      - |     1681.5
b<latereturn_flatmap_get> |      13 |     7.064 |  543404 |  0.914 |     1840.2
b<original_flatmap_get> * |      14 |     8.219 |  587041 |      - |     1703.5
b<latereturn_flatmap_get> |      14 |     7.799 |  557106 |  0.949 |     1795.0
b<original_flatmap_get> * |      15 |     8.868 |  591171 |      - |     1691.6
b<latereturn_flatmap_get> |      15 |     8.009 |  533929 |  0.903 |     1872.9
b<original_flatmap_get> * |      16 |     9.380 |  586254 |      - |     1705.7
b<latereturn_flatmap_get> |      16 |     8.819 |  551217 |  0.940 |     1814.2
b<original_flatmap_get> * |      17 |     9.953 |  585464 |      - |     1708.0
b<latereturn_flatmap_get> |      17 |     9.123 |  536656 |  0.917 |     1863.4
b<original_flatmap_get> * |      18 |    10.416 |  578672 |      - |     1728.1
b<latereturn_flatmap_get> |      18 |    10.096 |  560892 |  0.969 |     1782.9
b<original_flatmap_get> * |      19 |    10.661 |  561114 |      - |     1782.2
b<latereturn_flatmap_get> |      19 |    10.223 |  538071 |  0.959 |     1858.5
b<original_flatmap_get> * |      20 |    11.270 |  563485 |      - |     1774.7
b<latereturn_flatmap_get> |      20 |    10.946 |  547307 |  0.971 |     1827.1
b<original_flatmap_get> * |      21 |    11.881 |  565750 |      - |     1767.6
b<latereturn_flatmap_get> |      21 |    12.055 |  574068 |  1.015 |     1742.0
b<original_flatmap_get> * |      22 |    12.102 |  550090 |      - |     1817.9
b<latereturn_flatmap_get> |      22 |    11.956 |  543460 |  0.988 |     1840.1
b<original_flatmap_get> * |      23 |    12.934 |  562342 |      - |     1778.3
b<latereturn_flatmap_get> |      23 |    12.604 |  548014 |  0.975 |     1824.8
b<original_flatmap_get> * |      24 |    13.088 |  545352 |      - |     1833.7
b<latereturn_flatmap_get> |      24 |    12.973 |  540527 |  0.991 |     1850.0
b<original_flatmap_get> * |      25 |    14.285 |  571415 |      - |     1750.0
b<latereturn_flatmap_get> |      25 |    13.454 |  538164 |  0.942 |     1858.2
b<original_flatmap_get> * |      26 |    14.021 |  539265 |      - |     1854.4
b<latereturn_flatmap_get> |      26 |    14.999 |  576887 |  1.070 |     1733.4
b<original_flatmap_get> * |      27 |    14.239 |  527379 |      - |     1896.2
b<latereturn_flatmap_get> |      27 |    14.570 |  539641 |  1.023 |     1853.1
b<original_flatmap_get> * |      28 |    14.793 |  528332 |      - |     1892.7
b<latereturn_flatmap_get> |      28 |    16.144 |  576564 |  1.091 |     1734.4
b<original_flatmap_get> * |      29 |    16.194 |  558425 |      - |     1790.7
b<latereturn_flatmap_get> |      29 |    15.970 |  550702 |  0.986 |     1815.9
b<original_flatmap_get> * |      30 |    16.408 |  546917 |      - |     1828.4
b<latereturn_flatmap_get> |      30 |    16.302 |  543405 |  0.994 |     1840.2
b<original_flatmap_get> * |      31 |    16.256 |  524376 |      - |     1907.0
b<latereturn_flatmap_get> |      31 |    19.153 |  617853 |  1.178 |     1618.5
b<original_flatmap_get> * |      32 |    17.216 |  537994 |      - |     1858.8
b<latereturn_flatmap_get> |      32 |    18.697 |  584279 |  1.086 |     1711.5
===============================================================================
```
