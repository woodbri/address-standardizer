
CPPFLAGS = -O0 -g -Wall -std=c++0x -fPIC -frounding-math -Wno-deprecated -pedantic  -fmax-errors=10 -Wextra

EXE = t5

all: $(EXE)

t1: t1.cpp
	g++ $(CPPFLAGS) -o t1 t1.cpp ../*.o -ldl -lm `pkg-config --libs --cflags icu-uc icu-io` -L /usr/lib/x86_64-linux-gnu/ -lboost_regex

t2: t2.cpp
	g++ $(CPPFLAGS) -o t2 t2.cpp -L /usr/lib/x86_64-linux-gnu/ -lboost_regex

t3: t3.cpp
	g++ $(CPPFLAGS) -o t3 t3.cpp

t4: t4.cpp
	g++ $(CPPFLAGS) -D_FORTIFY_SOURCE=2 -D_REENTRANT  -DU_HAVE_ELF_H=1 -DU_HAVE_ATOMIC=1 -L /usr/lib/x86_64-linux-gnu/ `pkg-config --libs --cflags icu-uc icu-io` -Wl,-Bsymbolic-functions -Wl,-z,relro -o t4 t4.cpp ../*.o -ldl -lm `pkg-config --libs --cflags icu-uc icu-io` -L /usr/lib/x86_64-linux-gnu/ -lboost_regex

t5: t5.cpp
	g++ $(CPPFLAGS) -D_FORTIFY_SOURCE=2 -D_REENTRANT  -DU_HAVE_ELF_H=1 -DU_HAVE_ATOMIC=1 -L /usr/lib/x86_64-linux-gnu/ `pkg-config --libs --cflags icu-uc icu-io` -Wl,-Bsymbolic-functions -Wl,-z,relro -o t5 t5.cpp ../*.o -ldl -lm `pkg-config --libs --cflags icu-uc icu-io` -L /usr/lib/x86_64-linux-gnu/ -lboost_regex

test: $(EXE)
	./t5 lex-en.txt '11 radcliff rd, north chelmsford, ma 01863-2313 usa'


clean:
	rm -f $(EXE)
