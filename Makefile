# Définition des constantes
CC = gcc
CFLAGS = -Wall -ansi -pedantic
EXEC = main
OBJ = main.o huffman.o


# Définition des commandes à effectuer
main : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

main.o : main.c huffman.h

huffman.o : huffman.c huffman.h

%.o : %.c
	$(CC) -c $< $(CFLAGS)


# Définition des commandes de nettoyage
clean :
	rm -f *.o

mrproper : clean
	rm -f $(EXEC)


# Définition des commandes à appeler
install : $(EXEC)
	mkdir ../bin
	mv $(EXEC) ../bin/$(EXEC)
	make mrproper

uninstall : mrproper
	rm -f ../bin/$(EXEC)
	rm -rf ../bin
