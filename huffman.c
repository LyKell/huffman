#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include "huffman.h"



void initialiser_tableau(Noeud tab[]) {
	/*
	BUT		Fonction permettant d'initialise le tableau tab de Noeud
	PARAM	Noeud tab[] : Le tableau de Noeud contenant l'arbre correspondant au mot
	RETOUR	void
	*/
	int i;
	for (i = 0; i < NBNOEUDS; i++) {
		tab[i].lettre = '-';
		tab[i].occur = 0;
		tab[i].fg = -1;
		tab[i].fd = -1;
	}
}



void initialiser_code(Code traduc[]) {
	/*
	BUT		Fonction permettant d'initialiser le tableau traduc de Code
	PARAM	Code traduc[] : Le tableau de Code contenant les codes des lettres dans leur arbre
	RETOUR	void
	*/
	int i;
	for (i = 0; i < NBNOEUDS; i++) {
		strcpy(traduc[i].code, "\0");
		traduc[i].taille = 0;
	}
}



void afficher_tableau(Noeud tab[]) {
	/*
	BUT		Fonction permettant d'afficher le tableau tab de Noeud
	PARAM	Noeud tab[] : Le tableau de Noeud contenant l'arbre correspondant au mot
	RETOUR	void
	*/
	int i;
	for (i = 0; i < NBNOEUDS; i++) {
		printf("%d %c %d %d %d\n", tab[i].occur, tab[i].lettre, tab[i].fg, tab[i].fd, i);
	}
}



int compare_noeud(void const *n1, void const *n2) {
	/*
	BUT		Fonction permettant de comparer l'occurrence du noeud n1 et du noeud n2
			Cette fonction permet d'avoir un critère de tri pour la fonction compteLettres
	PARAM	void const *n1 : Pointeur sur le premier noeud à comparer
			void const *n2 : Pointeur sur le deuxième noeud à comparer
	RETOUR	1 : Si l'occurrence de n1 est 0
			-1 : Si l'occurrence de n2 est 0
			a-b : Sinon
	*/
	int a, b;
	a = ((const Noeud*) n1)->occur;
	b = ((const Noeud*) n2)->occur;
	if (a == 0) {
		return 1;
	}
	if (b == 0) {
		return -1;
	}
	return a - b;
}



int compteLettres(FILE *in, Noeud tab[]) {
	/*
	BUT		Fonction permettant de compter le nombre de lettres et de trier le tableau tab dans lequel on insère les lettres
			du fichier in
	PARAM	FILE *in : Pointeur sur le fichier texte qu'on veut lire et encoder
			Noeud tab[] : Le tableau de Noeud contenant l'arbre correspondant au mot
	RETOUR	nb_lettres_dif : Le nombre de lettres différentes dans le fichier in
	*/
	unsigned char i;
	int nb_lettres_dif = 0;

	while (1) {
		i = fgetc(in);

		if (feof(in)) {	/* EOF */
			break;
		}

		if (isalpha(i) || i == 32) {
			tab[i].lettre = i;
			if (tab[i].occur == 0) {
				nb_lettres_dif++;
			}
			tab[i].occur++;
		}
	}	
	qsort(tab, NBNOEUDS, sizeof(Noeud), compare_noeud);

	return nb_lettres_dif;
}



int construitArbre(Noeud tab[], int taille) {
	/*
	BUT		Fonction permettant de construire le tableau tab de Noeud, de taille taille
	PARAM	Noeud tab[] : Le tableau de Noeud contenant l'arbre correspondant au mot
			int taille : La taille du tableau
	RETOUR	noeud_libre-1 : L'indice de la racine
	*/
	int i, j, noeud_libre;

	i = 0;
	j = taille;
	noeud_libre = taille;

	while (j < 2*taille-2) {
		if (i < taille) {
			/* 2 feuilles */
			if (tab[j].occur == 0 || (tab[i].occur < tab[j].occur && tab[i+1].occur <= tab[j].occur && i+1 != taille)) {
				tab[noeud_libre].occur = tab[i].occur + tab[i+1].occur;
				tab[noeud_libre].fg = i;
				tab[noeud_libre].fd = i+1;
				noeud_libre++;
				i += 2;
			
			/* 1 feuille 1 noeud */
			} else if ((tab[i].occur < tab[j].occur && tab[j].occur < tab[i+1].occur && i+1 != taille)
					|| (tab[i].occur == tab[j].occur)
					|| (tab[i].occur < tab[j].occur && i+1 == taille)) {
				tab[noeud_libre].occur = tab[i].occur + tab[j].occur;
				tab[noeud_libre].fg = i;
				tab[noeud_libre].fd = j;
				noeud_libre++;
				i++;
				j++;

			/* 1 noeud 1 feuille */
			} else if (tab[i].occur > tab[j].occur && ((tab[j+1].occur != 0 && tab[i].occur <= tab[j+1].occur) || tab[j+1].occur == 0)) {
				tab[noeud_libre].occur = tab[j].occur + tab[i].occur;
				tab[noeud_libre].fg = j;
				tab[noeud_libre].fd = i;
				noeud_libre++;
				j++;
				i++;

			/* 2 noeuds */
			} else if (tab[i].occur > tab[j].occur && tab[j+1].occur != 0 && tab[i].occur > tab[j+1].occur) {
				tab[noeud_libre].occur = tab[j].occur + tab[j+1].occur;
				tab[noeud_libre].fg = j;
				tab[noeud_libre].fd = j+1;
				noeud_libre++;
				j += 2;
			}

		/* Plus aucune feuille à traiter, il ne reste que des noeuds internes */
		} else if (i >= taille) {
			tab[noeud_libre].occur = tab[j].occur + tab[j+1].occur;
			tab[noeud_libre].fg = j;
			tab[noeud_libre].fd = j+1;
			noeud_libre++;
			j += 2;
		}

	}
	// afficher_tableau(tab);
	return noeud_libre - 1;
}



int construitCode(Noeud arbreH[], int racine, char* chaine, Code traduc[]) {
	/*
	BUT		Fonction permettant de construire le code des lettres de arbreH dans traduc
			chaine permet de sauvegarder le début du code pour le réutiliser
	PARAM	Noeud arbreH[] : Le tableau de noeud contenant l'arbre correspondant au mot
			int racine : La racine de l'arbre qu'on étudie
			char* chaine : La chaine de caractère dans laquelle on sauvegarde le code de la lettre
			Code traduc[] : Le tableau de Code contenant les codes des lettres dans leur arbre
	RETOUR	longueur : La longueur du texte compressé
	*/
	int longueur = 0;
	if (arbreH[racine].fg == -1 && arbreH[racine].fd == -1) {
		chaine[strlen(chaine)] = '\0';
		strcpy(traduc[arbreH[racine].lettre].code, chaine);
		traduc[arbreH[racine].lettre].taille = strlen(chaine);
		return arbreH[racine].occur * traduc[arbreH[racine].lettre].taille;
	}
	strcat(chaine, "0");
	longueur += construitCode(arbreH, arbreH[racine].fg, chaine, traduc);
	/* On cherche dans le fils droit après, donc il faut supprimer le caractère précédent, qui correspond au code d'un fils gauche */
	chaine[strlen(chaine)-1] = '\0';

	strcat(chaine, "1");
	longueur += construitCode(arbreH, arbreH[racine].fd, chaine, traduc);
	/* Idem qu'au-dessus, mais pour un fils gauche */
	chaine[strlen(chaine)-1] = '\0';

	return longueur;
}



void afficheCode(Code traduc[]) {
	/*
	BUT		Fonction permettant d'afficher le code de chaque lettre dans traduc
	PARAM	Code traduc[] : Le tableau dans lequel on a les codes des lettres
	RETOUR	void
	*/
	int i;

	for (i = 0; i < NBNOEUDS; i++) {
		if (traduc[i].taille != 0) {
			printf("[%c] : %s\n", i, traduc[i].code);
		}
	}
}



void transmission_code(Noeud tab[], int racine, FILE *out) {
	/*
	BUT		Fonction permettant d'écrire dans out l'arbre tab contenant les lettres par parcours préfixe
	PARAM	Noeud tab[] : Le tableau contenant les noeuds correspondant au texte
			int racine : La racine de l'arbre que l'on étudie
			FILE *out : Le pointeur vers le fichier dans lequel on veut écrire
	RETOUR	void
	*/
	if (tab == NULL) {
		return;
	}
	if (tab[racine].fg == -1 && tab[racine].fd == -1) {
		fprintf(out, "1[%c]", tab[racine].lettre);
		return;
	}
	fprintf(out, "0");
	transmission_code(tab, tab[racine].fg, out);
	transmission_code(tab, tab[racine].fd, out);
}



void bits_padding(FILE *out, int nbbits) {
	/*
	BUT		Fonction permettant de convertir une valeur décimale (nbbits) en binaire, et l'écrire dans le fichier out
	PARAM	FILE *out : Le pointeur vers le fichier dans lequel écrire
			int nbbits : L'entier à convertir en binaire
	RETOUR	void
	*/
	int longueur, binary;
	char nbbits_bin[3];

	for (longueur = 2; longueur >= 0; longueur--) {
		binary = nbbits >> longueur;
		if (binary & 1) {
			strcat(nbbits_bin, "1");
		} else {
			strcat(nbbits_bin, "0");
		}
	}

	fprintf(out, "%s", nbbits_bin);
}



void codeHuffmanX8(FILE *in, FILE *out) {
	/*
	BUT		Fonction permettant d'encoder le fichier in dans le fichier out
	PARAM	FILE *in : Le pointeur vers le fichier que l'on veut encoder
			FILE *out : Le pointeur vers le fichier dans lequel on veut encoder
	RETOUR	void 
	*/
	Noeud tab[NBNOEUDS];
	Code traduc[NBNOEUDS];
	char chaine[TAILLEMAX];
	int nb_lettres_dif, racine, longueur, nbbits, nbnoeuds, j;
	unsigned char i;

	initialiser_tableau(tab);
	nb_lettres_dif = compteLettres(in, tab);
	racine = construitArbre(tab, nb_lettres_dif);
	longueur = construitCode(tab, racine, chaine, traduc);
	// afficheCode(traduc);

	nbnoeuds = 2*nb_lettres_dif - 1;

	nbbits = 8 - (3 + longueur + nbnoeuds)%8;	/* Le nombre de bits du padding en base 10 */

	bits_padding(out, nbbits);

	transmission_code(tab, racine, out);

	rewind(in);	/* Permet de retourner au début du fichier à encoder */
	/* On écrit l'encodage du texte */
	while (1) {
		i = fgetc(in);

		if (feof(in)) {
			break;
		}

		if (isalpha(i)) {
			fprintf(out, "%s", traduc[i].code);
		}
	}

	/* On écrit le padding */
	for (j = 0; j < nbbits; j++) {
		fprintf(out, "0");
	}
}



