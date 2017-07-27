CFLAGS=-w -Wall -Werror -std=c11

c1:
	$(CC) $(CFLAGS) $@.c $^ -o $@

all: clean c1

clean:
	$(RM) c1 *.o
