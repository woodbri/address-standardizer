
CPP = g++

CPPFLAGS = -MMD -MP -fPIC -O0 -g -Wall -std=c++0x -pedantic  -fmax-errors=10 -Wextra -frounding-math -Wno-deprecated -D_FORTIFY_SOURCE=2 -D_REENTRANT  -DU_HAVE_ELF_H=1 -DU_HAVE_ATOMIC=1 -I ..

UPOBJS = ../grammar.o ../inclass.o ../lexentry.o ../lexicon.o ../metarule.o ../metasection.o ../outclass.o ../rule.o ../rulesection.o ../search.o ../token.o ../tokenizer.o ../utils.o ../trieutf8.o ../utf8iterator.o


LDFLAGS = $(UPOBJS) -L /usr/lib/x86_64-linux-gnu/ -ldl -lm `pkg-config --libs --cflags icu-uc icu-io` -Wl,-Bsymbolic-functions -Wl,-z,relro -L /usr/lib/x86_64-linux-gnu/ -lboost_regex -lboost_unit_test_framework

SRCS = $(wildcard *.cpp)

OBJS  = $(SRCS:.cpp=.o)
DEPS  = $(SRCS:.cpp=.d)
TESTS = $(SRCS:.cpp=)

all: $(TESTS)

$(TESTS): $(UPOBJS)
	$(CPP) $(CPPFLAGS) -o $@ $@.cpp $(LDFLAGS)

.PHONY: test

test: $(TESTS)
	./test-runner.sh $(TESTS)
#	$(foreach x,$(TESTS),echo Running $(x);./$(x);)

.PHONY: clean

clean:
	rm -f *.o *.d $(TESTS)

-include $(DEPS)
 
