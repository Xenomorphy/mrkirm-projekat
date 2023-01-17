#!/bin/bash

pushd Server
gcc -o server server.c
./server
popd

pushd Client
gcc -o client client.c
popd
