import sys



def bsearch(a, c):
    l = int(1)
    r = len(str(a)) - 1
    while l <= r:
        m = (r-l)//2 + l
        p = 10 ** m
        right = a % p
        left = a // p


        if right == 0:
            l = m+1
            continue



        res = left//right
        if(res == c):
            return [(left), (right)]

        if(res < c):
            r = m-1
            continue
        else:
            l = m+1
            continue








[a, b] = list(map(int, input().split()))

[left, right] = bsearch(a, b)
print(left, right)


