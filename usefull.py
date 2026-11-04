import sys
import math
import heapq
import bisect
from collections import deque, Counter, defaultdict
from functools import lru_cache

from decimal import Decimal, getcontext
getcontext().prec = 100000        # if you really need it

#   divmod(a, b)  -> (a // b, a % b)   (one division, faster)
#   a ** b        power, a ** 0.5 is FLOAT (don't use for big a)
#   pow(a, b, mod) fast modular exponentiation
#   math.isqrt(a) integer square root, exact
#   a.bit_length()

try:
    sys.set_int_max_str_digits(0)   # 0 = no limit
except AttributeError:
    pass                            # Python < 3.11 has no limit



# ----------------------------------------------------------------------
# 1. INPUT
# ----------------------------------------------------------------------
# Fastest and simplest: read the WHOLE input, split on any whitespace.
# Works whether the values are on one line or many lines.
#
data = sys.stdin.buffer.read().split()   # list of bytes
a = int(data[0])                          # int() accepts bytes too
b = int(data[1])
#
# If you need strings instead of bytes:
data = sys.stdin.read().split()           # list of str
#
# Read one line at a time:
n = int(sys.stdin.readline())
a, b = map(int, sys.stdin.readline().split())
arr = list(map(int, sys.stdin.readline().split()))
#
# Read n numbers, one per line:
arr = [int(sys.stdin.readline()) for _ in range(n)]
#
# Read n lines of two numbers:
pairs = [tuple(map(int, sys.stdin.readline().split())) for _ in range(n)]
#
# Tip: target problems usually put the count first, e.g.:
n = int(data[0]); arr = list(map(int, data[1:1+n]))


# 2. OUTPUT
# ----------------------------------------------------------------------
# print() puts a space between arguments and a newline at the end:
# Fast output for many lines (build one string, write once):
out = []
out.append(str(x))
sys.stdout.write("\n".join(out))
# Or: 
sys.stdout.write(" ".join(map(str, arr)))

# 5. LOOPS
# ----------------------------------------------------------------------
while condition:
...

for i in range(n):          # 0, 1, ..., n-1
for i in range(1, n + 1):   # 1 .. n
for i in range(5, 15, 2):   # 5, 7, 9, 11, 13
for i in range(n - 1, -1, -1):  # n-1 down to 0  (reverse loop)
#
for x in arr:               # iterate values
for i, x in enumerate(arr): # index + value
for i, x in enumerate(arr, start=1):   # 1-based index
for a, b in zip(A, B):      # walk two lists together


# 6. FUNCTIONS
# ----------------------------------------------------------------------
def add(a, b=0):        # b has a default value
    return a + b
#
def min_max(arr):
    return min(arr), max(arr)     # returns a tuple
#
lo, hi = min_max([3, 1, 2])
#
# Recursion: raise the limit first (default is 1000)!
sys.setrecursionlimit(1_000_000)
#
# Memoization (top-down DP) in one line:
@lru_cache(maxsize=None)
def fib(n):
    if n < 2:
        return n
    return fib(n - 1) + fib(n - 2)

gg# [ ] Copy a list with a[:] or list(a), not b = a.
