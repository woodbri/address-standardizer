
CPP = g++

CPPFLAGS = -MMD -MP -fPIC -O0 -g -Wall -std=c++0x -pedantic  -fmax-errors=10 -Wextra -frounding-math -Wno-deprecated -D_FORTIFY_SOURCE=2 -D_REENTRANT  -DU_HAVE_ELF_H=1 -DU_HAVE_ATOMIC=1 

LDFLAGS =

SRCS = $(wildcard *.cpp)

OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

all: $(OBJS)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@


.PHONY: clean

clean:
	rm -f *.o *.d

-include $(DEPS)
