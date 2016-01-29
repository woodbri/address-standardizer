
CPP = g++

CPPFLAGS = -MMD -MP -fPIC -O0 -g -Wall -std=c++0x -pedantic  -fmax-errors=10 -Wextra -frounding-math -Wno-deprecated -D_FORTIFY_SOURCE=2 -D_REENTRANT  -DU_HAVE_ELF_H=1 -DU_HAVE_ATOMIC=1

LDFLAGS = ../*.o -L /usr/lib/x86_64-linux-gnu/ -ldl -lm `pkg-config --libs --cflags icu-uc icu-io` -Wl,-Bsymbolic-functions -Wl,-z,relro -L /usr/lib/x86_64-linux-gnu/ -lboost_regex -lboost_unit_test_framework

SRCS = $(wildcard *.cpp)

OBJS  = $(SRCS:.cpp=.o)
DEPS  = $(SRCS:.cpp=.d)
TESTS = $(SRCS:.cpp=)

all: $(TESTS)

$(TESTS):
	$(CPP) $(CPPFLAGS) -o $@ $@.cpp $(LDFLAGS)

test: $(TESTS)
	$(foreach x,$(TESTS),echo Running $(x);./$(x);)

.PHONY: clean

clean:
	rm -f *.o *.d $(TESTS)

-include $(DEPS)
 
