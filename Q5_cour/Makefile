CC=gcc
CFLAGS=-Wall -O -g
LDFLAGS=-pthread
EXEC=test_lecteurs_redacteurs

all: $(EXEC)
			rm *.o
			./test_lecteurs_redacteurs 10 10 3
			
test_lecteurs_redacteurs: lectred.o test_lecteurs_redacteurs.o
			$(CC) -o $@ $^ $(LDFLAGS)

lectred.o: lectred.c
			$(CC) -o $@ -c $< $(CFLAGS)
			
test_lecteurs_redacteurs.o : test_lecteurs_redacteurs.c lectred.h
			$(CC) -o $@ -c $< $(CFLAGS)

clean:
			rm -f *.o *~

mrproper: clean
			rm -f $(EXEC) core


