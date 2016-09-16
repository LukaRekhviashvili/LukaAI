#!/bin/bash

gcc -Wall -std=c11 -o luka_AI \
luka.c \
File_reader.c \
 -lm && ./luka_AI
