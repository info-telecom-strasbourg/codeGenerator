MAKEFLAGS += --no-print-directory

JFLAGS= -g -Xlint:unchecked
JC=javac
JVM=java

CLASSDIR=../class
SRCDIR=../src
TESTDIR=tests

SOURCES:=$(wildcard $(SRCDIR)/*.java)
TEST:=Template.java

MAIN=Template

default:
	$(JC) -d $(CLASSDIR)/ $(TEST) $(SOURCES) $(JFLAGS)

run:
	@make >/dev/null
	@$(JVM) -classpath ../class/ $(MAIN) || true

clean:
	$(RM) ../class/*.class

.PHONY: clean run classes
