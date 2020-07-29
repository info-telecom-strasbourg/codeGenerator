# Do not print when we change of directory
MAKEFLAGS += --no-print-directory

# Options and JVM options
JFLAGS= -g -Xlint:unchecked
JC=javac
JVM=java

# Depends on your structure project
CLASSDIR=class
SRCDIR=src
TESTDIR=tests

SOURCES:=$(wildcard $(SRCDIR)/*.java)
CLASSES:=$(SOURCES:$(SRCDIR)/%.java=$(CLASSDIR)/%.class)

# The name of the fail that contains the main method
MAIN=Main

default:
	$(JC) -d $(CLASSDIR)/ $(SOURCES) $(JFLAGS)

run:
	@make >/dev/null
	@$(JVM) -classpath $(CLASSDIR)/ $(MAIN) || true

tests:
	@cd ./tests $(MAKE) && make run

clean:
	$(RM) class/*.class

.PHONY: clean run classes tests

