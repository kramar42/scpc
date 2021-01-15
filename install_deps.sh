#!/usr/bin/env sh

# requires package to be installed: pip3 install glad
# generate glad files
python3 -m glad --profile core --out-path /tmp/glad --api gl=3.2 --generator c --spec gl

mkdir -p include/glad
mv /tmp/glad/include/glad/* include/glad
mkdir -p deps
mv /tmp/glad/src/* deps
