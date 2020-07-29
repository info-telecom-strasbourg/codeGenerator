MAKEFLAGS += --no-print-directory

JFLAGS= -g -Xlint:unchecked
JC=javac
JVM=java

CLASSDIR=../class
SRCDIR=../src
TESTDIR=tests

SOURCES:=$(wildcard $(SRCDIR)/*.java)
MAIN:=TestsDemo

default:
	$(JC) -d $(CLASSDIR)/ $(MAIN).java $(SOURCES) $(JFLAGS)

run:
	@make >/dev/null
	@$(JVM) -classpath ../class/ $(MAIN) || true

clean:
	$(RM) ../class/*.class

.PHONY: clean run classes
