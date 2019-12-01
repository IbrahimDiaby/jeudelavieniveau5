/*!
 * @file fichier source io.c (Input/ouput)/ (entrées/sorties)
 * @brief recoupant toutes les dependances permettant l'affichage du jeu
 */

#include "io.h"
#include <stdlib.h>

#ifdef MODETEXTE
void affiche_trait (int c){
	int i;
	for (i=0; i<c; ++i) printf ("|---");
	printf("|\n");
	return;
}

void affiche_ligne (int c, int* ligne, int vieillissement){
	int i;
    if(vieillissement){
        for (i=0; i<c; ++i){
            if (ligne[i] == 0 ) printf ("|   ");
            else if (ligne[i] == -1 ) printf ("| X ");
            else printf ("| %d ", ligne[i]);
        }
    }
    
    else {
        for (i=0; i<c; ++i){
            if (ligne[i] == 0 ) printf ("|   ");
            else if (ligne[i] == -1 ) printf ("| X ");
            else printf ("| O ");
        }
    }
    printf("|\n");
	return;
}

void affiche_grille (grille g, cellule C, int compteur, int cyclique, int vieillissement,int tempsOscillation){
	int i, l=g.nbl, c=g.nbc;
	printf("\n");
    //printf("\e[K");
    system("clear");
    printf("Temps d'evolution : %d | ", compteur);
    printf("Mode De Voisinage : ");
    cyclique ? printf("Cyclique") : printf("Non-cyclique");
    printf(" | ");
    printf("Vieillissement : ");
    vieillissement ? printf("Oui") : printf("Non");
    printf(" | ");
    printf("Temps d'oscillation : %d ", tempsOscillation);
    printf("\n\n");
	affiche_trait(c);
	if(vieillissement){
        for (i=0; i<l; ++i) {
		    affiche_ligne(c, C.cellules[i],vieillissement);
		    affiche_trait(c);
	    }
    }

    else{
        for (i=0; i<l; ++i) {
		    affiche_ligne(c, g.cellules[i],vieillissement);
		    affiche_trait(c);
	    }
    }
	printf("\n"); 
	return;
}

void efface_grille (grille g){
	printf("\n\e[%dA",g.nbl*2 + 5); 
}

void debut_jeu(grille *g, grille *gc, cellule *C){
    char c = getchar();
    int compteur = 0, cyclique = 0, vieillissement= 0, tempsOscillation = 0;
    int (*compte_voisins_vivants) (int,int,grille)=compte_voisins_vivants_non_cyclique;
	while (c != 'q') // touche 'q' pour quitter
	{
		switch (c) {
			case '\n' : 
			{ // touche "entree" pour évoluer
                ++compteur;
				evolue(g,gc,C, compte_voisins_vivants,vieillissement);
				efface_grille(*g);
                system("clear");
				affiche_grille(*g,*C,compteur, cyclique, vieillissement, tempsOscillation);
				break;
			}
                
            case 'c' :
            { // * La touche c permet de choisir si le voisinage est cyclique ou non
                if(cyclique){
                    cyclique = 0;
                    compte_voisins_vivants=&compte_voisins_vivants_non_cyclique;
                }
                
                else{
                    cyclique = 1;
                    compte_voisins_vivants=&compte_voisins_vivants_cyclique;
                }
                
                break;
            }

            case 'v' :
            { // * La touche v permet de choisir si les cellules vieillissent ou non
                
                vieillissement=!vieillissement;

                break;
            }
            
            case 'o' :
            {
				tempsOscillation = grilleOscillante(g, compte_voisins_vivants, vieillissement);
			}

            case 'n' :
            {
				// * La touche n permet de changer de fichier
                compteur = 0;
                vieillissement =0;
                cyclique = 0;
                tempsOscillation = 0;
                
                char nom[20];
                printf("Entrer le nom de la grille : ");
                scanf("%s", nom);
                getchar();
                
                efface_grille(*g);
                system("clear");
                libere_cellule(g, C);
                libere_grille(g);
                libere_grille(gc);
                
                init_grille_from_file(nom,g);
                alloue_grille (g->nbl, g->nbc, gc);
                
                alloue_grille_cellule (g->nbl, g->nbc, C);
				copie_grille_cellule(*g,*C);
                
                affiche_grille(*g,*C,compteur, cyclique, vieillissement, tempsOscillation);
                
                break;
            }
                
			default : 
			{ // touche non traitée
				printf("\n\e[1A");
                system("clear");
				break;
			}
		}
        //printf("Voulez vous entrer le nom d'un nouveau fichier ? Press on \"n\" : ");
        c = getchar();
	}
    
	return;	
}
#else
void affiche_trait (int c){
	int i;
	for (i=0; i<c; ++i) printf ("|---");
	printf("|\n");
	return;
}

void affiche_ligne (int c, int* ligne, int vieillissement){
	int i;
    if(vieillissement){
        for (i=0; i<c; ++i){
            if (ligne[i] == 0 ) printf ("|   ");
            else if (ligne[i] == -1 ) printf ("| X ");
            else printf ("| %d ", ligne[i]);
        }
    }
    
    else {
        for (i=0; i<c; ++i){
            if (ligne[i] == 0 ) printf ("|   ");
            else if (ligne[i] == -1 ) printf ("| X ");
            else printf ("| O ");
        }
    }
    printf("|\n");
	return;
}

void affiche_grille (grille g, cellule C, int compteur, int cyclique, int vieillissement,int tempsOscillation){
	int i, l=g.nbl, c=g.nbc;
	printf("\n");
    //printf("\e[K");
    system("clear");
    printf("Temps d'evolution : %d | ", compteur);
    printf("Mode De Voisinage : ");
    cyclique ? printf("Cyclique") : printf("Non-cyclique");
    printf(" | ");
    printf("Vieillissement : ");
    vieillissement ? printf("Oui") : printf("Non");
    printf(" | ");
    printf("Temps d'oscillation : %d ", tempsOscillation);
    printf("\n\n");
	affiche_trait(c);
	if(vieillissement){
        for (i=0; i<l; ++i) {
		    affiche_ligne(c, C.cellules[i],vieillissement);
		    affiche_trait(c);
	    }
    }

    else{
        for (i=0; i<l; ++i) {
		    affiche_ligne(c, g.cellules[i],vieillissement);
		    affiche_trait(c);
	    }
    }
	printf("\n"); 
	return;
}

void efface_grille (grille g){
	printf("\n\e[%dA",g.nbl*2 + 5); 
}

void debut_jeu(grille *g, grille *gc, cellule *C){
    char c = getchar();
    int compteur = 0, cyclique = 0, vieillissement= 0, tempsOscillation = 0;
    int (*compte_voisins_vivants) (int,int,grille)=compte_voisins_vivants_non_cyclique;
	while (c != 'q') // touche 'q' pour quitter
	{
		switch (c) {
			case '\n' : 
			{ // touche "entree" pour évoluer
                ++compteur;
				evolue(g,gc,C, compte_voisins_vivants,vieillissement);
				efface_grille(*g);
                system("clear");
				affiche_grille(*g,*C,compteur, cyclique, vieillissement,tempsOscillation);
				break;
			}
                
            case 'c' :
            { // * La touche c permet de choisir si le voisinage est cyclique ou non
                if(cyclique){
                    cyclique = 0;
                    compte_voisins_vivants=&compte_voisins_vivants_non_cyclique;
                }
                
                else{
                    cyclique = 1;
                    compte_voisins_vivants=&compte_voisins_vivants_cyclique;
                }
                
                break;
            }

            case 'v' :
            { // * La touche v permet de choisir si les cellules vieillissent ou non
                
                vieillissement=!vieillissement;

                break;
            }
            
            case 'o' :
            {
				tempsOscillation = grilleOscillante(g, compte_voisins_vivants, vieillissement);
			}

            case 'n' :
            {
                compteur = 0;
                vieillissement =0;
                cyclique = 0;
                tempsOscillation = 0;
                
                char nom[20];
                printf("Entrer le nom de la grille : ");
                scanf("%s", nom);
                getchar();
                
                efface_grille(*g);
                system("clear");
                libere_grille(g);
                libere_grille(gc);
                init_grille_from_file(nom,g);
                alloue_grille (g->nbl, g->nbc, gc);
                affiche_grille(*g,*C,compteur, cyclique, vieillissement,tempsOscillation);
                
                break;
            }
                
			default : 
			{ // touche non traitée
				printf("\n\e[1A");
                system("clear");
				break;
			}
		}
        //printf("Voulez vous entrer le nom d'un nouveau fichier ? Press on \"n\" : ");
        c = getchar();
	}
    
	return;	
}
#endif
