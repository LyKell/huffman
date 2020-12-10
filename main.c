#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "huffman.h"



int main(int argc, char* argv[]) {
	FILE *in, *out;
	char fichier[TAILLEMAX];

	fichier[0] = '\0';

	srand(time(NULL));

	if (argc < 2) {
		printf("Argument invalide !\n");
		return EXIT_FAILURE;
	}

	strcat(fichier, argv[1]);
	fichier[strlen(argv[1])-4] = '\0';

	in = fopen(argv[1], "r");
	out = fopen(strcat(fichier, ".HUF"), "w");
	
	codeHuffmanX8(in, out);

	fclose(in);
	fclose(out);
	
	return EXIT_SUCCESS;
}