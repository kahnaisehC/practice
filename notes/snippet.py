# ======================================================================
# Python cheat sheet for competitive programming
# ----------------------------------------------------------------------
# Everything here is plain Python 3. Copy the pieces you need.
# The file itself is valid Python:  python3 snippet.py
# ======================================================================

import sys
import math
import heapq
import bisect
from collections import deque, Counter, defaultdict
from functools import lru_cache

# ----------------------------------------------------------------------
# 0. BIG INTEGERS - read this first
# ----------------------------------------------------------------------
# Python ints have UNLIMITED size. No overflow, ever:
#   2**1000, 10**100000, factorial(10000) are all fine.
#
# The only trap: Python 3.11+ limits int <-> str conversion to 4300
# digits by default. A 5000-digit input/output will crash unless you
# disable the limit BEFORE reading/printing. Keep these two lines:

try:
    sys.set_int_max_str_digits(0)   # 0 = no limit
except AttributeError:
    pass                            # Python < 3.11 has no limit

# Huge-number operations (all exact):
#   a + b, a - b, a * b
#   a // b        integer division (floor)
#   a % b         remainder
#   divmod(a, b)  -> (a // b, a % b)   (one division, faster)
#   a ** b        power, a ** 0.5 is FLOAT (don't use for big a)
#   pow(a, b, mod) fast modular exponentiation
#   math.isqrt(a) integer square root, exact
#   a.bit_length()
#   int(s)        parse from string:  int("123...")   (any length)
#   str(a)        convert to string:  len(str(a)) = digit count
#   int(s, 16)    parse base 16;  int("ff", 16) == 255
#   bin(a), hex(a), f"{a:b}"  -> strings with/without prefix
#   int(str(a)[::-1])         reverse the digits
#   sum(map(int, str(a)))     digit sum
#
# NEVER use `/` on big integers: it returns a float and loses precision.
#   a / b      -> float   WRONG for big numbers
#   a // b     -> exact int  RIGHT
#
# Decimal is only for decimal fractions; its default precision is 28
# digits, so never use it to store a huge integer:
#   from decimal import Decimal, getcontext
#   getcontext().prec = 100000        # if you really need it
#
# ----------------------------------------------------------------------
# 1. INPUT
# ----------------------------------------------------------------------
# Fastest and simplest: read the WHOLE input, split on any whitespace.
# Works whether the values are on one line or many lines.
#
#   data = sys.stdin.buffer.read().split()   # list of bytes
#   a = int(data[0])                          # int() accepts bytes too
#   b = int(data[1])
#
# If you need strings instead of bytes:
#   data = sys.stdin.read().split()           # list of str
#
# Read one line at a time:
#   n = int(sys.stdin.readline())
#   a, b = map(int, sys.stdin.readline().split())
#   arr = list(map(int, sys.stdin.readline().split()))
#
# Read n numbers, one per line:
#   arr = [int(sys.stdin.readline()) for _ in range(n)]
#
# Read n lines of two numbers:
#   pairs = [tuple(map(int, sys.stdin.readline().split())) for _ in range(n)]
#
# Tip: target problems usually put the count first, e.g.:
#   n = int(data[0]); arr = list(map(int, data[1:1+n]))
#
# ----------------------------------------------------------------------
# 2. OUTPUT
# ----------------------------------------------------------------------
# print() puts a space between arguments and a newline at the end:
#   print(a, b)              ->  "12 34\n"
#   print(a, b, sep=", ")    ->  "12, 34\n"
#   print(a, end=" ")        ->  no newline
#   print(*arr)              ->  space-separated elements
#
# Fast output for many lines (build one string, write once):
#   out = []
#   out.append(str(x))
#   sys.stdout.write("\n".join(out))
#
# Or: sys.stdout.write(" ".join(map(str, arr)))
#
# ----------------------------------------------------------------------
# 3. VARIABLES, TYPES, OPERATORS
# ----------------------------------------------------------------------
#   x = 10            int
#   y = 10.5          float
#   s = "abc"         str
#   ok = True         bool
#   z = None          "nothing"
#
# Arithmetic:  +  -  *  /  //  %  **
#   Careful:  7 / 2 = 3.5     7 // 2 = 3
#             -7 // 2 = -4    (floor, NOT truncation toward zero!)
#             -7 % 2 = 1      (result has sign of divisor)
#   For C-like truncation on negatives, use int(a / b) only on small ints,
#   or: q = a // b; if (a % b != 0) and ((a < 0) != (b < 0)): q += 1
#
# Comparison:  ==  !=  <  <=  >  >=
# Boolean:     and  or  not
# Bitwise:     &  |  ^  ~  <<  >>     (also work on big ints)
# Identity:    is / is not           (use == for numbers/strings)
#
# Chained comparison is allowed:  0 <= x < n
# Assignment shortcuts:  x += 1   x -= 1   x *= 2   x //= 2
#
# ----------------------------------------------------------------------
# 4. CONDITIONALS
# ----------------------------------------------------------------------
# if x > 0:
#     ...
# elif x == 0:
#     ...
# else:
#     ...
#
# One-line:   sign = -1 if x < 0 else 0 if x == 0 else 1
# No switch statement. Use if/elif or a dict of functions.
#
# ----------------------------------------------------------------------
# 5. LOOPS
# ----------------------------------------------------------------------
# while condition:
#     ...
#
# for i in range(n):          # 0, 1, ..., n-1
# for i in range(1, n + 1):   # 1 .. n
# for i in range(5, 15, 2):   # 5, 7, 9, 11, 13
# for i in range(n - 1, -1, -1):  # n-1 down to 0  (reverse loop)
#
# for x in arr:               # iterate values
# for i, x in enumerate(arr): # index + value
# for i, x in enumerate(arr, start=1):   # 1-based index
# for a, b in zip(A, B):      # walk two lists together
#
# break     leaves the loop
# continue  skips to the next iteration
# pass      does nothing
#
# for/while can have an `else` that runs only if the loop did NOT break:
# for x in arr:
#     if x == target:
#         break
# else:
#     print("not found")      # reached only when nothing matched
#
# Python has no do-while. Emulate it:
# while True:
#     ...
#     if condition:
#         break
#
# ----------------------------------------------------------------------
# 6. FUNCTIONS
# ----------------------------------------------------------------------
# def add(a, b=0):        # b has a default value
#     return a + b
#
# def min_max(arr):
#     return min(arr), max(arr)     # returns a tuple
#
# lo, hi = min_max([3, 1, 2])
#
# Recursion: raise the limit first (default is 1000)!
# sys.setrecursionlimit(1_000_000)
#
# Memoization (top-down DP) in one line:
# @lru_cache(maxsize=None)
# def fib(n):
#     if n < 2:
#         return n
#     return fib(n - 1) + fib(n - 2)
#
# Nested functions and closures work normally. `global` is needed only
# when a function assigns to a module-level variable.
#
# ----------------------------------------------------------------------
# 7. LISTS (dynamic arrays)
# ----------------------------------------------------------------------
# a = []                  empty
# a = [1, 2, 3]
# a = [0] * n             n zeros
# a = list(map(int, data))
#
# a.append(x)             push back
# a.pop()                 remove and return last
# a.pop(i)                remove and return index i
# a.insert(i, x)          insert at i
# a.remove(x)             remove first value equal to x
# a.extend(b)             append all of b
# a.index(x)              first position of x
# a.count(x)              occurrences
# len(a)                  size
# x in a                  membership (O(n))
# a.sort()                in-place sort
# a.sort(reverse=True)
# a.reverse()
# b = sorted(a)           returns a new sorted list
# sum(a), min(a), max(a)
#
# Indexing: a[0] first, a[-1] last.
# Slicing:  a[l:r] is indices l .. r-1 (r exclusive)
#           a[:k] first k, a[k:] from k, a[::-1] reversed copy
#           slicing always makes a copy
#
# List comprehension (very common):
# b = [x * x for x in a]
# b = [x for x in a if x > 0]
# b = [x * 2 if x > 0 else 0 for x in a]
#
# 2D grid - WRITE IT THIS WAY:
# grid = [[0] * m for _ in range(n)]
# NEVER: grid = [[0] * m] * n     (all rows are the SAME object)
#
# ----------------------------------------------------------------------
# 8. STRINGS
# ----------------------------------------------------------------------
# Strings are immutable: s[i] = 'x' is an error. Build a list and join:
#   chars = list(s); chars[i] = 'x'; s = "".join(chars)
#
# s[i], len(s), s[l:r], s[::-1]     (reverse)
# s.upper(), s.lower()
# s.split()                split on whitespace
# s.split(",")             split on comma
# " ".join(arr)            join list of strings
# s.strip()                remove whitespace at both ends
# s.startswith(p), s.endswith(p)
# s.find(p)                index or -1
# s.replace(old, new)
# s.isdigit(), s.isalpha(), s.isupper()
# ord(ch) -> code, chr(code) -> char
# Sorting characters: "".join(sorted(s))
#
# Comparing strings: s1 < s2 is lexicographic.
# Input like "abc" is already a string; single chars compare fine.
#
# ----------------------------------------------------------------------
# 9. DICTS AND SETS
# ----------------------------------------------------------------------
# d = {}                   empty dict
# d[key] = value
# d[key]                   KeyError if missing
# d.get(key, default)      safe read
# key in d                 membership (O(1))
# del d[key]
# d.keys(), d.values(), d.items()
# for k, v in d.items(): ...
#
# Counter counts things:
# c = Counter(arr)
# c[x], c.most_common(3)
#
# defaultdict gives missing keys a default (no KeyError):
# g = defaultdict(list)
# g[u].append(v)
# cnt = defaultdict(int)
# cnt[x] += 1
#
# Sets:
# s = set()
# s.add(x), s.discard(x), x in s
# A | B union, A & B intersection, A - B difference
#
# ----------------------------------------------------------------------
# 10. SORTING / SEARCHING
# ----------------------------------------------------------------------
# Sort tuples by first, then second:
# arr.sort(key=lambda p: (p[0], p[1]))
# Sort by first ascending, second descending:
# arr.sort(key=lambda p: (p[0], -p[1]))
# Sort indices by value:
# order = sorted(range(n), key=lambda i: arr[i])
#
# Binary search on a SORTED list (bisect):
# i = bisect.bisect_left(a, x)    # first index with a[i] >= x
# j = bisect.bisect_right(a, x)   # first index with a[i] > x
# count of x = j - i
# Insert keeping sorted: bisect.insort(a, x)
#
# Manual binary search:
# lo, hi = 0, n - 1
# while lo <= hi:
#     mid = (lo + hi) // 2
#     if a[mid] == x:
#         break
#     elif a[mid] < x:
#         lo = mid + 1
#     else:
#         hi = mid - 1
#
# ----------------------------------------------------------------------
# 11. USEFUL MODULES
# ----------------------------------------------------------------------
# math.gcd(a, b), math.lcm(a, b)
# math.isqrt(n)            exact floor(sqrt(n)), use for big ints
# math.inf                 infinity
# heapq: min-heap
#   h = []
#   heapq.heappush(h, x)
#   x = heapq.heappop(h)
#   heapq.heapify(arr)
#   max-heap: push -x
# deque: O(1) pops from both ends (use for BFS)
#   q = deque([start])
#   q.append(x); q.popleft()
#   q.appendleft(x); q.pop()
#
# ----------------------------------------------------------------------
# 12. TEMPLATE: solve one test case
# ----------------------------------------------------------------------
def solve():
    data = sys.stdin.buffer.read().split()
    n = int(data[0])
    arr = list(map(int, data[1:1 + n]))
    # ... your algorithm ...
    print(sum(arr))


# Template: handle T test cases first.
def solve_many():
    data = sys.stdin.buffer.read().split()
    t = int(data[0])
    pos = 1
    out = []
    for _ in range(t):
        n = int(data[pos])
        pos += 1
        arr = list(map(int, data[pos:pos + n]))
        pos += n
        out.append(str(sum(arr)))
    sys.stdout.write("\n".join(out))


# ----------------------------------------------------------------------
# 13. TEMPLATE: huge number split (the f.py problem)
# ----------------------------------------------------------------------
# Given huge a and c, find m such that:
#   right = a % 10**m, left = a // 10**m, left // right == c
# Returns (left, right) or None.
def big_split(a, c):
    lo, hi = 1, len(str(a)) - 1
    while lo <= hi:
        m = (lo + hi) // 2
        p = 10 ** m
        left, right = divmod(a, p)
        if right == 0:
            lo = m + 1
            continue
        q = left // right
        if q == c:
            return left, right
        if q < c:          # need bigger quotient -> smaller m
            hi = m - 1
        else:              # need smaller quotient -> bigger m
            lo = m + 1
    return None


# ----------------------------------------------------------------------
# 14. GOTCHAS CHECKLIST
# ----------------------------------------------------------------------
# [ ] Set sys.set_int_max_str_digits(0) before huge int read/print.
# [ ] Use // not / (never mix float with big ints).
# [ ] Python // floors toward -inf; C truncates toward zero.
# [ ] No integer overflow - don't pre-guess limits.
# [ ] Raise sys.setrecursionlimit for deep DFS.
# [ ] 2D list: [[0]*m for _ in range(n)], never [[0]*m]*n.
# [ ] Don't use a mutable default argument: def f(a=[]) is a bug.
# [ ] Reading all input with .read().split() handles newlines/spaces.
# [ ] Division by zero raises; guard it.
# [ ] `x in list` is O(n); use a set/dict when it matters.
# [ ] Copy a list with a[:] or list(a), not b = a.

if __name__ == "__main__":
    # Small self-demo that needs no input.
    huge = int("1" * 5000)
    print("digits:", len(str(huge)))
    print("floor div:", huge // (10 ** 2500) == int("1" * 2500))
    print("mod ok:", huge % (10 ** 2500) == int("1" * 2500))
    print("split ok:", big_split(huge, 1) is not None)


sys.set_int_max_str_digits(1000000)


