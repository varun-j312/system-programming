# Defining Macros
BIN=bin/
OBJ=obj/
SRC=src/
INC=inlcude/
CFLAG=-o
OBJFLAG=-c

# default make build
all:	clean program

# program builds
program: functions
	g++ ${CFLAG} ${BIN}server9 ${SRC}server.cpp ${OBJ}func.o
	g++ ${CFLAG} ${BIN}client9 ${SRC}client.cpp ${OBJ}func.o

# object dependancies
functions:
	g++ ${OBJFLAG} ${CFLAG} ${OBJ}func.o ${SRC}func.cpp

# cleaing files
clean:
	echo "Cleaning .o and executable files"
	rm ${OBJ}func.o
	rm ${BIN}server9
	rm ${BIN}client9
