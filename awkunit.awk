#!/usr/bin/awk -f
@load "awkunit"

function assert(condition, string)
{
    if (!condition) {
        printf("Assertion failed: %s\n",
               string) > "/dev/stderr"
        _assert_exit = 1
        exit 1
    }
}

function assertEquals(value1, value2)
{
    if (value1 != value2) {
        printf("Assertion failed: %s is not equal to %s\n",
               value1, value2) > "/dev/stderr"
        _assert_exit = 1
        exit 1
    }
}

END {
    if (_assert_exit)
        exit 1
}
