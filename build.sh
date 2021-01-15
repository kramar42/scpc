#!/bin/bash

mkdir -p build

PKGS="glfw3 gl"
CFLAGS="-Wall -std=gnu11 -Iinclude `pkg-config --cflags $PKGS`"
LIBS="`pkg-config --libs $PKGS` -lm"
SOURCES="main.c deps/*"

cc $CFLAGS $SOURCES $LIBS -o main