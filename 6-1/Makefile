CC = g++

SRC = 0010108_hw6-1_v1.cpp

TARGET = hw6-1

LIB = -O2 -lm

all:
	${CC} ${SRC} ${LIB} -o ${TARGET}

run1:
	./${TARGET} < 6-1-small.in.txt > output1

run2:
	./${TARGET} < 6-1-med.in.txt > output2

run3:
	./${TARGET} < 6-1-large.in.txt > output3

diff1:
	diff output1 6-1-small.out.txt

diff2:
	diff output2 6-1-med.out.txt

diff3:
	diff output3 6-1-large.out.txt

clean:
	rm -rf ${TARGET}
