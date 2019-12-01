/*!
 * @file fichier source grille.c
 * @brief recoupant toutes les dependances permettant la creation et la gestion de grille
 */

#include "grille.h"

#ifdef MODETEXTE
// initialise la grille g à partir du fichier filename
void init_grille_from_file (char * filename, grille* g){
	FILE * pfile = NULL;
	pfile = fopen(filename, "r");
	assert (pfile != NULL);
	
	int i,j,n,l,c,vivantes=0,nonViable;
	
	fscanf(pfile, "%d", &l);
	fscanf(pfile, "%d", &c);
	
	alloue_grille(l,c,g);
	
	fscanf(pfile, "%d", &vivantes);
	for (n=0; n < vivantes; ++n){
		fscanf(pfile, "%d", &i);
		fscanf(pfile, "%d", &j);
		set_vivante(i,j,*g);
	}
    
    fscanf(pfile, "%d", &nonViable);
    for (n=0; n < nonViable; ++n){
        fscanf(pfile, "%d", &i);
        fscanf(pfile, "%d", &j);
        set_nonViable(i,j,*g);
    }
	
	fclose (pfile);
	return;
}

// copie la grille gs dans la grille gd
void copie_grille (grille gs, grille gd){
	int i, j;
	for (i=0; i<gs.nbl; ++i) for (j=0; j<gs.nbc; ++j) gd.cellules[i][j] = gs.cellules[i][j];
	return;	
}

// copie les cellules de la grille gs dans les cellules de la cellule c
void copie_grille_cellule (grille gs, cellule c){
	int i, j;
	for (i=0; i<gs.nbl; ++i) for (j=0; j<gs.nbc; ++j) c.cellules[i][j] = gs.cellules[i][j];
	return;	
}

// fais veillir la cellule
void vieillir(int i,int j,grille g,cellule c){
    
    if(c.cellules[i][j]>0){
        if(c.cellules[i][j]==8){
            set_morte(i,j,g);
            set_morte_cellule(i,j,c);
        }
        else{
            c.cellules[i][j]++;
        }
    }
}

// alloue une grille de taille l*c, et initialise toutes les cellules à mortes
void alloue_grille (int l, int c, grille* g){
    g->nbl = l;
    g->nbc = c;
    g->cellules = (int **) malloc ((l*(sizeof(int*))));
    for (int i=0;i<l;i++){
            g->cellules[i] = (int *) malloc (c*sizeof(int));
            for(int j=0;j<c;j++){
                set_morte(i,j,*g);
        }
    }

}

// alloue une grille de taille l*c, et initialise toutes les cellules à mortes
void alloue_grille_cellule (int l, int c, cellule* C){
    C->cellules = (int **) malloc ((l*(sizeof(int*))));
    for (int i=0;i<l;i++){
            C->cellules[i] = (int *) malloc (c*sizeof(int));
    }

}

// libère une grille
void libere_grille (grille* g){
    for(int i =0;i<(g->nbl);i++){
        free(g->cellules[i]);
    }
    free(g->cellules);

}

// libère une cellule
void libere_cellule (grille *g,cellule* c){
    for(int i =0;i<(g->nbl);i++){
        free(c->cellules[i]);
    }
    free(c->cellules);
}

// verifie si les grilles g1 et g2 sont égales
int grillesEgales(grille *g1, grille *g2) {
	int i, j;
	for (i = 0; i < g1->nbl; i++) {
		for (j = 0; j < g1->nbc; j++) {
			if (g1->cellules[i][j] != g2->cellules[i][j]) {
				return 0;
			}
		}
	}
	return 1;
}

// verifie si la grille g est vide
int grilleVide(grille *g) {
	int i, j;
	for (i = 0; i < g->nbl; i++) {
		for (j = 0; j < g->nbc; j++) {
			if (g->cellules[i][j] > 0) {
				return 0;
			}
		}
	}
	return 1;
}

// verifie si la grille g est oscillante
int grilleOscillante(grille *g, int (*compte_voisins_vivants) (int, int, grille), int vieillissement) {
	int tempsOscillation = 0;
	grille Initiale, Iteree, Temp;
	cellule C;
	alloue_grille (g->nbl, g->nbc, &Initiale);
	copie_grille(*g, Initiale);

	alloue_grille (g->nbl, g->nbc, &Iteree);
	copie_grille(*g, Iteree);

	alloue_grille (g->nbl, g->nbc, &Temp);
	copie_grille(*g, Temp);
	
	alloue_grille_cellule (g->nbl, g->nbc, &C);
	copie_grille_cellule(*g, C);

	int maxInterval = 1000; // On suppose qu'au-delà de 1000 évolutions, une grille ne peut pas être oscillante

	int maxDelais = 100; // On suppose qu'au-delà de 100 évolutions, il ne peut plus y avoir de comportement oscillatoire
	int i = 0;
	 do {
		while (tempsOscillation < maxInterval) {
			evolue(Iteree,Temp,C,compte_voisins_vivants,vieillissement); 
			if (grilleVide(&Initiale)) {
				return 0;
			}
			if (grillesEgales(&Initiale, &Iteree)) {
				return tempsOscillation;
			}
		}

		evolue(Initiale,Temp,C,compte_voisins_vivants,vieillissement);
		copie_grille(Initiale, Iteree);
		tempsOscillation = 0; // Réinitialisation du temps d'une oscillation
		i++;
	} while (i < maxDelais);
	return 0;
}
#else
// initialise la grille g à partir du fichier filename
void init_grille_from_file (char * filename, grille* g){
	FILE * pfile = NULL;
	pfile = fopen(filename, "r");
	assert (pfile != NULL);
	
	int i,j,n,l,c,vivantes=0,nonViable;
	
	fscanf(pfile, "%d", &l);
	fscanf(pfile, "%d", &c);
	
	alloue_grille(l,c,g);
	
	fscanf(pfile, "%d", &vivantes);
	for (n=0; n < vivantes; ++n){
		fscanf(pfile, "%d", &i);
		fscanf(pfile, "%d", &j);
		set_vivante(i,j,*g);
	}
    
    fscanf(pfile, "%d", &nonViable);
    for (n=0; n < nonViable; ++n){
        fscanf(pfile, "%d", &i);
        fscanf(pfile, "%d", &j);
        set_nonViable(i,j,*g);
    }
	
	fclose (pfile);
	return;
}

// copie la grille gs dans la grille gd
void copie_grille (grille gs, grille gd){
	int i, j;
	for (i=0; i<gs.nbl; ++i) for (j=0; j<gs.nbc; ++j) gd.cellules[i][j] = gs.cellules[i][j];
	return;	
}

// copie les cellules de la grille gs dans les cellules de la cellule c
void copie_grille_cellule (grille gs, cellule c){
	int i, j;
	for (i=0; i<gs.nbl; ++i) for (j=0; j<gs.nbc; ++j) c.cellules[i][j] = gs.cellules[i][j];
	return;	
}

// fais veillir la cellule
void vieillir(int i,int j,grille g,cellule c){
    
    if(c.cellules[i][j]>0){
        if(c.cellules[i][j]==8){
            set_morte(i,j,g);
            set_morte_cellule(i,j,c);
        }
        else{
            c.cellules[i][j]++;
        }
    }
}

// alloue une grille de taille l*c, et initialise toutes les cellules à mortes
void alloue_grille (int l, int c, grille* g){
    g->nbl = l;
    g->nbc = c;
    g->cellules = (int **) malloc ((l*(sizeof(int*))));
    for (int i=0;i<l;i++){
            g->cellules[i] = (int *) malloc (c*sizeof(int));
            for(int j=0;j<c;j++){
                set_morte(i,j,*g);
        }
    }

}

// alloue une grille de taille l*c, et initialise toutes les cellules à mortes
void alloue_grille_cellule (int l, int c, cellule* C){
    C->cellules = (int **) malloc ((l*(sizeof(int*))));
    for (int i=0;i<l;i++){
            C->cellules[i] = (int *) malloc (c*sizeof(int));
    }

}

// libère une grille
void libere_grille (grille* g){
    for(int i =0;i<(g->nbl);i++){
        free(g->cellules[i]);
    }
    free(g->cellules);

}


// libère une cellule
void libere_cellule (grille *g,cellule* c){
    for(int i =0;i<(g->nbl);i++){
        free(c->cellules[i]);
    }
    free(c->cellules);

}

// verifie si les grilles g1 et g2 sont égales
int grillesEgales(grille *g1, grille *g2) {
	int i, j;
	for (i = 0; i < g1->nbl; i++) {
		for (j = 0; j < g1->nbc; j++) {
			if (g1->cellules[i][j] != g2->cellules[i][j]) {
				return 0;
			}
		}
	}
	return 1;
}

// verifie si la grille g est vide
int grilleVide(grille *g) {
	int i, j;
	for (i = 0; i < g->nbl; i++) {
		for (j = 0; j < g->nbc; j++) {
			if (g->cellules[i][j] > 0) {
				return 0;
			}
		}
	}
	return 1;
}

// verifie si la grille g est oscillante
int grilleOscillante(grille *g, int (*compte_voisins_vivants) (int, int, grille), int vieillissement) {
	int tempsOscillation = 0;
	grille Initiale, Iteree, Temp;
	cellule C;
	alloue_grille (g->nbl, g->nbc, &Initiale);
	copie_grille(*g, Initiale);

	alloue_grille (g->nbl, g->nbc, &Iteree);
	copie_grille(*g, Iteree);

	alloue_grille (g->nbl, g->nbc, &Temp);
	copie_grille(*g, Temp);
	
	alloue_grille_cellule (g->nbl, g->nbc, &C);
	copie_grille_cellule(*g, C);

	int maxInterval = 1000; // On suppose qu'au-delà de 1000 évolutions, une grille ne peut pas être oscillante

	int maxDelais = 100; // On suppose qu'au-delà de 100 évolutions, il ne peut plus y avoir de comportement oscillatoire
	int i = 0;
	 do {
		while (tempsOscillation < maxInterval) {
			// Met à jour tempsEvolutionOscillation
			evolue(Iteree,Temp,C,compte_voisins_vivants,vieillissement);
			if (grilleVide(&Initiale)) {
				return 0;
			}
			if (grillesEgales(&Initiale, &Iteree)) {
				return tempsOscillation;
			}
		}

		evolue(Initiale,Temp,C,compte_voisins_vivants,vieillissement);
		copie_grille(Initiale, Iteree);
		tempsOscillation = 0; // Réinitialisation du temps d'une oscillation
		i++;
	} while (i < maxDelais);
	return 0;
}
#endif
