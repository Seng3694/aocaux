CC=gcc
AR=ar rc
RANLIB=ranlib
MKDIR=mkdir
RM=rm -rf
CP=cp -r

CFLAGS:=-g \
  -O0 \
  -Wall \
  -std=c99 \
  -fsanitize=address \
  -fsanitize=undefined

ifdef release
CFLAGS:=-O2 \
  -Wall \
  -std=c99 \
  -DNDEBUG
endif

ifndef verbose
SILENT=@
endif

BIN:=bin
SRC:=src
INCLUDE:=$(SRC)/aoc

SRCS:=$(wildcard src/*.c)
OBJS:=$(patsubst $(SRC)/%.c, $(BIN)/%.o, $(SRCS))

TESTS:=$(wildcard test/*.c)
TEST_BINS:=$(patsubst test/%.c, $(BIN)/% , $(TESTS))

$(BIN)/libaocaux.a: | $(BIN) $(OBJS) 
	$(SILENT) $(AR) $@ $(OBJS)
	$(SILENT) $(RANLIB) $@

run_tests: $(TEST_BINS)
	@for TEST in $(TEST_BINS); do \
		./$$TEST; \
	done

$(BIN)/%_test: $(BIN)/libaocaux.a test/%_test.c 
	$(SILENT) $(CC) $(CFLAGS) -o $@ $(subst $(BIN)/,test/,$@).c -Isrc -L$(BIN) $<

$(BIN)/%.o: $(SRC)/%.c $(INCLUDE)/common.h $(INCLUDE)/%.h
	$(SILENT) $(CC) -c $(CFLAGS) $< -o $@

$(BIN):
	$(SILENT) $(MKDIR) $(BIN) 

install: $(BIN)/libaocaux.a
	$(CP) $(BIN)/libaocaux.a /usr/local/lib/
	$(CP) $(SRC)/aoc /usr/local/include/

uninstall:
	$(RM) /usr/local/lib/libaocaux.a
	$(RM) /usr/local/include/aoc

clean:
	$(SILENT) $(RM) $(BIN)/*

.PHONY: clean run_tests install uninstall
