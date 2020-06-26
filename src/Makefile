CXX = g++
CXXFLAG = -std=c++17 -O2 -Wall -Wextra -fPIC
GMAEINCLUDE = ./
BUILDDIR = ./build

all: gamerunner AI

AI:
	$(CXX) $(CXXFLAG) -I$(GMAEINCLUDE) -shared AITemplate/Porting.cpp -o ${BUILDDIR}/AI.so
	cp ${BUILDDIR}/AI.so ${BUILDDIR}/a1.so
	cp ${BUILDDIR}/AI.so ${BUILDDIR}/a2.so

gamerunner:
	$(CXX) $(CXXFLAG) -I$(GMAEINCLUDE) -pthread main.cpp -o ${BUILDDIR}/gamerunner -ldl

.PHONY: clean
clean:
	-rm gamerunner a1.so
