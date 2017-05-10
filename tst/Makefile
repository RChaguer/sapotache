TESTS_OK = $(wildcard sapo*.ok.txt)
RES_OK   = $(patsubst %.txt, %.tst, $(TESTS_OK))

TESTS_KO = $(wildcard sapo*.err.txt)
RES_KO = $(patsubst %.txt, %.tst, $(TESTS_KO))

TMP_FILE = /tmp/sapo.tmp

all:
	gcc -Wall -Wextra -std=c99 sapotache.c -o sapotache

test: all $(RES_OK) $(RES_KO)
	@echo "" # For beauty

%.ok.tst: %.ok.txt
	@./sapotache $< > $(TMP_FILE) && \
	if [ -z "$$(diff $(patsubst %.txt,%.exp, $<) $(TMP_FILE) )" ]; \
	then echo -n "."; else echo -n "[$<]"; fi

%.err.tst: %.err.txt
	@if ! ( ./sapotache $< || false ) >/dev/null 2>&1; then echo -n "."; else echo -n "[$<]"; fi

clean:
	@rm -f *~ sapotache
