CXX = g++
CXXFLAG = -std=c++17 -O2 -Wall -Wextra -fPIC
GMAEINCLUDE = ./
BUILDDIR = ./build
BONUS = -Werror -Wextra -pedantic-errors

all: gamerunner AI

AI:
	$(CXX) $(CXXFLAG) $(BONUS) -I$(GMAEINCLUDE) -shared AITemplate/Porting.cpp -o ${BUILDDIR}/AI.so
	cp ${BUILDDIR}/AI.so ${BUILDDIR}/a1.so
	cp ${BUILDDIR}/AI.so ${BUILDDIR}/a2.so

gamerunner:
	$(CXX) $(CXXFLAG) $(BONUS) -I$(GMAEINCLUDE) -pthread main.cpp -o ${BUILDDIR}/gamerunner -ldl

AI1:
	$(CXX) $(CXXFLAG) $(BONUS) -D$(AITYPE) -I$(GMAEINCLUDE) -shared AITemplate/tmpPorting.cpp -o ${BUILDDIR}/a1.so

AI2:
	$(CXX) $(CXXFLAG) $(BONUS) -D$(AITYPE) -I$(GMAEINCLUDE) -shared AITemplate/tmpPorting.cpp -o ${BUILDDIR}/a2.so

.PHONY: clean
clean:
	-rm gamerunner a1.so
