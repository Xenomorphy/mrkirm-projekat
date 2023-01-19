#!/bin/bash

DEBUG=-g

[ $# -eq 0 ] && DEBUG=

cd Server
g++ $DEBUG -o server *.cpp
cd ..

cd Client
g++ $DEBUG -o client *.cpp
cd ..
