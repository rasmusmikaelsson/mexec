CC = gcc
CFLAGS = -g -std=gnu11 -Werror -Wall -Wextra -Wpedantic \
         -Wmissing-declarations -Wmissing-prototypes -Wold-style-definition

OBJ = mexec.o pipeline.o error_handler.o command.o parse.o
DEPS = pipeline.h error_handler.h command.h parse.h

mexec: $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJ) mexec
