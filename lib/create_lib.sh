#!/bin/bash

FN="libhashmap";

gcc -shared -o "${FN}.so" -fPIC "${FN}.c"

cp "${FN}.so" ./..
cp "${FN}.h" ./..

