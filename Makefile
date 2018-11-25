CC = gcc
CFLAGS = -O2
LFLAGS = -static -s
DIRS = obj bin

program = bin/ascii
objects = obj/main.o obj/ascii.o

$(shell mkdir -p $(DIRS))

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(program): $(objects)
	$(CC) $(objects) $(LFLAGS) -o $@

.PHONY : clean
clean:
	$(RM) $(program) $(objects) src/*~
