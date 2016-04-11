
CC = ../../src/tester/compile-grammar
TIME = /usr/bin/time

GMR = $(wildcard *.gmr)
TRI = $(GMR:.gmr=.tri)

%.tri: %.gmr
	$(TIME) $(CC) $< $@

all: $(TRI)

counts: $(TRI)
	$(foreach x, $(TRI), $(info $(shell /usr/bin/basename $(x) )    $(shell /bin/grep '\[' $(x) | wc -l ) ) )

clean:
	rm -f *.tri
