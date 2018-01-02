CXX = g++
CXXFLAGS = -std=c++14 -O3 -Wall -Wextra -pedantic -Werror=vla -MMD
EXEC = hash
OBJECTS = test.o hashtable.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
