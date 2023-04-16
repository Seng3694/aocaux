CC=gcc
AR=ar rc
RANLIB=ranlib
MKDIR=mkdir
RM=rm -f

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

TESTS:=$(wildcard test/*_test.c)

OBJS:=$(BIN)/bits.o \
  $(BIN)/filesystem.o \
  $(BIN)/md5.o \
  $(BIN)/string.o

$(BIN)/libaocaux.a: | $(BIN) $(OBJS) 
	$(SILENT) $(AR) $@ $(OBJS)
	$(SILENT) $(RANLIB) $@

$(BIN)/%_test: $(BIN)/libaocaux.a $(TESTS) 
	$(SILENT) $(CC) $(CFLAGS) -o $@ $(subst $(BIN)/,test/,$@).c -Isrc -L$(BIN) $<

$(BIN)/%.o: $(SRC)/%.c $(INCLUDE)/common.h $(INCLUDE)/%.h
	$(SILENT) $(CC) -c $(CFLAGS) $< -o $@

$(BIN):
	$(SILENT) $(MKDIR) $(BIN) 

clean:
	$(SILENT) $(RM) $(BIN)/*

.PHONY: clean test
