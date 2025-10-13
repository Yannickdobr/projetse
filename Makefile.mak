CC=gcc
CFLAGS=-Wall -Wextra -std=c99
SOURCES=main.c gestion_ressources.c
EXEC=systeme_reservation

$(EXEC): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXEC) $(SOURCES)

clean:
	rm -f $(EXEC)

run: $(EXEC)
	./$(EXEC)

debug: CFLAGS += -g
debug: $(EXEC)

.PHONY: clean run debug