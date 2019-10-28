#!/bin/bash
echo 'Client working ...'
gcc client.c -o client -std=c17 -Wall
./client
echo ''
echo 'Server working ...'
gcc server.c -o server -std=c17 -Wall
./server
