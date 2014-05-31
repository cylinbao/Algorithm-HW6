#!/bin/bash

make clean
make
time make run1
make diff1
time make run2
make diff2
time make run3
make diff3
