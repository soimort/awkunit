#!/usr/bin/awk -f
@include "awkunit"
@include "asort"

function testIO() {
    assertIO("asort.awk", "/dev/stdin", "asort.ok")
}

BEGIN {
    testIO()
    exit 0
}
