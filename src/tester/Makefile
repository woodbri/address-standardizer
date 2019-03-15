
CPPFLAGS = -O0 -g -Wall -std=c++0x -fPIC -frounding-math -Wno-deprecated -pedantic  -fmax-errors=10 -Wextra -Werror=conversion -I ..

OBJS = ../grammar.o ../inclass.o ../lexentry.o ../lexicon.o ../metarule.o ../metasection.o ../outclass.o ../rule.o ../rulesection.o ../search.o ../token.o ../tokenizer.o ../utils.o ../trieutf8.o ../utf8iterator.o ../md5.o

EXE = t2 read-dump-grammar read-dump-lexicon t4 t5 regex-tester compile-lexicon

all: $(EXE)

compile-lexicon: compile-lexicon.cpp $(OBJS)
	g++ $(CPPFLAGS) -o compile-lexicon compile-lexicon.cpp $(OBJS) -ldl -lm `pkg-config --libs --cflags icu-uc icu-io` -L /usr/lib/x86_64-linux-gnu/ -lboost_regex -lboost_wserialization -lboost_serialization

read-dump-lexicon: read-dump-lexicon.cpp $(OBJS)
	g++ $(CPPFLAGS) -o read-dump-lexicon read-dump-lexicon.cpp $(OBJS) -ldl -lm `pkg-config --libs --cflags icu-uc icu-io` -L /usr/lib/x86_64-linux-gnu/ -lboost_regex -lboost_wserialization -lboost_serialization

t2: t2.cpp $(OBJS)
	g++ $(CPPFLAGS) -D_FORTIFY_SOURCE=2 -D_REENTRANT  -DU_HAVE_ELF_H=1 -DU_HAVE_ATOMIC=1 -L /usr/lib/x86_64-linux-gnu/ `pkg-config --libs --cflags icu-uc icu-io` -Wl,-Bsymbolic-functions -Wl,-z,relro -o t2 t2.cpp $(OBJS) -ldl -lm `pkg-config --libs --cflags icu-uc icu-io` -L /usr/lib/x86_64-linux-gnu/ -lboost_regex -lboost_serialization

read-dump-grammar: read-dump-grammar.cpp $(OBJS)
	g++ $(CPPFLAGS) -D_FORTIFY_SOURCE=2 -D_REENTRANT  -DU_HAVE_ELF_H=1 -DU_HAVE_ATOMIC=1 -L /usr/lib/x86_64-linux-gnu/ `pkg-config --libs --cflags icu-uc icu-io` -Wl,-Bsymbolic-functions -Wl,-z,relro -o read-dump-grammar read-dump-grammar.cpp $(OBJS) -ldl -lm `pkg-config --libs --cflags icu-uc icu-io` -L /usr/lib/x86_64-linux-gnu/ -lboost_regex

t4: t4.cpp $(OBJS)
	g++ $(CPPFLAGS) -D_FORTIFY_SOURCE=2 -D_REENTRANT  -DU_HAVE_ELF_H=1 -DU_HAVE_ATOMIC=1 -L /usr/lib/x86_64-linux-gnu/ `pkg-config --libs --cflags icu-uc icu-io` -Wl,-Bsymbolic-functions -Wl,-z,relro -o t4 t4.cpp $(OBJS) -ldl -lm `pkg-config --libs --cflags icu-uc icu-io` -L /usr/lib/x86_64-linux-gnu/ -lboost_regex

t5: t5.cpp $(OBJS)
	g++ $(CPPFLAGS) -D_FORTIFY_SOURCE=2 -D_REENTRANT  -DU_HAVE_ELF_H=1 -DU_HAVE_ATOMIC=1 -L /usr/lib/x86_64-linux-gnu/ `pkg-config --libs --cflags icu-uc icu-io` -Wl,-Bsymbolic-functions -Wl,-z,relro -o t5 t5.cpp $(OBJS) -ldl -lm `pkg-config --libs --cflags icu-uc icu-io` -L /usr/lib/x86_64-linux-gnu/ -lboost_regex

regex-tester: regex-tester.cpp ../utils.o
	g++ $(CPPFLAGS) -D_FORTIFY_SOURCE=2 -D_REENTRANT  -DU_HAVE_ELF_H=1 -DU_HAVE_ATOMIC=1 -L /usr/lib/x86_64-linux-gnu/ `pkg-config --libs --cflags icu-uc icu-io ` -Wl,-Bsymbolic-functions -Wl,-z,relro -o regex-tester regex-tester.cpp ../utils.o -ldl -lm `pkg-config --libs --cflags icu-uc icu-io` -L /usr/lib/x86_64-linux-gnu/ -lboost_regex

lex-serial-usa.txt: compile-lexicon lex-usa.txt
	./compile-lexicon lex-usa.txt lex-serial-usa.txt

test: $(EXE)
	./t2 lex-serial-usa.txt ../../data/sample/usa.gmr '11 radcliff rd, north chelmsford, ma 01863-2313 usa'

valgrind: $(EXE)
	valgrind -v --track-origins=yes ./t2 lex-serial-usa.txt ../../data/sample/usa.gmr '11 radcliff rd, north chelmsford, ma 01863-2313 usa'

callgrind: $(EXE)
	valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./t2 lex-serial-usa.txt ../../data/sample/usa.gmr '11 radcliff rd, north chelmsford, ma 01863-2313 usa'
	callgrind_annotate callgrind.out | tee callgrind.txt

test1: $(EXE)
	./t4 lex-en.txt '11 radcliff rd, north chelmsford, ma 01863-2313 usa'
	./read-dump-grammar ./test.grammar
	./t2 lex-usa.txt ./test.grammar '11 radcliff rd, north chelmsford, ma 01863-2313 usa'
	./t2 lex-test.txt test.grammar  'a b c d e'


clean:
	rm -f $(EXE) lex-serial-usa.txt
