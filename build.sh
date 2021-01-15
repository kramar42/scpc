#!/bin/bash

mkdir -p build

PKGS="glfw3 gl"
CFLAGS="-Wall -std=gnu11 -Iinclude `pkg-config --cflags $PKGS`"
LIBS="`pkg-config --libs $PKGS` -lm -ldl"
SOURCES="src/main.c deps/*"

cc $CFLAGS $SOURCES $LIBS -o build/main
