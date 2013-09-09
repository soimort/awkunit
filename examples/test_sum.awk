#!/usr/bin/awk -f
@include "awkunit"
@include "sum"

function testSum() {
    assertEquals(0, sum("0"))
    assertEquals(3, sum("1 2"))
    assertEquals(-2, sum("3 -5"))
    assertEquals(17, sum("2 5 10"))
}

function testIO() {
    assertIO("sum.awk", "sum.in", "sum.ok")
}

BEGIN {
    testSum()
    testIO()
    exit 0
}
