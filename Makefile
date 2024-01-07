SHELL = /bin/sh
.SUFFIXES = .cxx .o

NAME = stockholm
CC = g++
CFLAGS = -I${incDir} -Wall -Wextra -Werror -pedantic -std=c++17

incDir = inc
srcDir = src
buildDir = build
binDir = bin

src = ${srcDir}/main.cxx
obj = ${src:${srcDir}/%.cxx=${buildDir}/%.o}
dep = ${obj:.o=.d}
