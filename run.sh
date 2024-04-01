#!/bin/bash
g++ -c DataProvider.cpp -o ./bin/DataProvider.o
g++ search_do.cpp ./bin/DataProvider.o -o ./bin/search_do  && ./bin/search_do

