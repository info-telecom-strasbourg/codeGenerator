CC		:= g++
C_FLAGS := -std=c++11 -g -pthread -pedantic -fstrict-overflow

LIBRARIES	:=


EXECUTABLE	:= tests

all: 	$(EXECUTABLE)

$(EXECUTABLE): *.cpp
	$(CC) $(C_FLAGS) -I -L $^ -o $@ $(LIBRARIES)


.PHONY: clean mrproper


clean:
	$(RM) $(EXECUTABLE)

mrproper: clean
	rm -rf $(EXECUTABLE)

run: all
	$(EXECUTABLE)

