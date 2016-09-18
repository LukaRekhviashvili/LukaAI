#!/bin/bash

gcc -Wall -std=c11 -o luka_AI \
luka.c \
File_reader.c \
Neighbour.c \
Randomiser.c \
Best_match_unit.c \
Node_map.c \
Utils.c \
Input_data_vect.c \
 -lm && ./luka_AI
