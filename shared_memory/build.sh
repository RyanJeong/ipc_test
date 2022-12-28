#!/bin/bash
gcc test.c -o test -Wall
gcc listener.c -o server -Wall
gcc talker.c -o client -Wall
