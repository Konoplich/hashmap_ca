#!/bin/bash

rm ./ok

gcc main.c -o ok -L. -lhashmap -Wl,-rpath,.
