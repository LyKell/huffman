#ifndef __HUFFMAN__
#define __HUFFMAN__

#include <stdio.h>

#define NBLETTRES 256
#define NBNOEUDS (NBLETTRES * 2 - 1)
#define TAILLEMAX 32



typedef struct {
	unsigned char lettre;
	int occur;
	int fg;
	int fd;
} Noeud;



typedef struct {
	char code[TAILLEMAX];
	int taille;
} Code;



void initialiser_tableau(Noeud tab[]);
void initialiser_code(Code traduc[]);
void afficher_tableau(Noeud tab[]);
int compare_noeud(void const *n1, void const *n2);
int compteLettres(FILE *in, Noeud tab[]);
int construitArbre(Noeud tab[], int taille);
int construitCode(Noeud arbreH[], int racine, char* chaine, Code traduc[]);
void afficheCode(Code traduc[]);
void transmission_code(Noeud tab[], int racine, FILE *out);
void bits_padding(FILE *out, int nbbits);
void codeHuffmanX8(FILE *in, FILE *out);


#endif