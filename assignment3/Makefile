CC = gcc 
CFLAGS = -g -std=c99 -Wall 
LDFLAGS = -lm

help: 
	@echo "targets: help log header test clean"
timer: timer.o 
	$(CC) $(LDFLAGS) $^ -o $@
piper: piper.o
	$(CC) $(LFLAGS) $^ -o $@
timer.o: timer.c 
	$(CC) $(CFLAGS) -c  $<
piper.o: piper.c 
	$(CC) $(CFLAGS) -c  $<
timertest: 
	./timer sleep 1s
	./timer sleep 2s
	./timer sleep 3s
	./timer sleep 4s
	./timer sleep 5s
clean: 
	rm timer
	rm piper
	rm *.o
all: 
	make compile
	make test