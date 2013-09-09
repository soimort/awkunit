#!/usr/bin/awk -f

BEGIN {
    FS = "[^0-9+-]+"
}

function sum(input) {
    split(input, m, FS)
    ret = 0
    for (i in m)
        ret += m[i]
    return ret
}

{
    print sum($0)
}
