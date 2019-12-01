/*!
 * @file fichier principale
 * @brief recoupant toutes les dependances permettant la réalisation du projet
 */

#include <stdio.h>
#include <stdlib.h>

#include "grille.h"
#include "io.h"
#include "jeu.h"

#define SIZEX 600
#define SIZEY 600

/*!
 * \author Diaby Ibrahim
 * \version 0.5.1
 * \date 01-12-2019
 * \mainpage Jeu De La Vie
 * \section Consignes
 Voici les consignes que doivent respecter chacun de vos rendus.

Le dépôt git doit être partagé avec l’enseignant qui s’occupe de votre groupe de TP à qui vous donnerez le rôle Reporter.
make dist produit une archive (tarball). Cette archive en .tar.xz doit être mise en ligne sur Moodle dans la section prévue à cet effet (attention à respecter votre groupe).
make compile le code extrait en version debug.
doxygen crée la documentation.
Votre historique git doit contenir des étiquettes (tag) pour indiquer vos versions. Ainsi, le travail correspondant au niveau 1 aura une étiquette comme 0.1 (ou 1.0, ou autre selon vos préférences) et les archives déposées sur Moodle auront dans leur nom l’étiquette correspondante. Vous pouvez publier plusieurs versions d’un niveau en ajoutant des étiquettes pour cette série (0.1.x, 1.0.x…). Votre fichier README expliquera votre système de versionnement.
Un projet qui ne compile pas sera pénalisé. Un exécutable qui plante sera pénalisé. Tout ce qui entrave les tests par l’enseignant sera pénalisé.
 */

#ifdef MODETEXTE

/*!
 * @brief fonction principale
 * @param[in] argc nombre d'argument passé en paramètre
 * @param[in] argv[] tableau de chaines de caractères correspondant aux nombres d'arguments passés en paramètre
 * @param[out] renvoie 0 si pas d'erreur et 1 sinon
 */
int main (int argc, char ** argv) {
	
	if (argc != 2 )
	{
		printf("usage : main <fichier grille>\n");
		return 1;
	}

    grille g, gc;
    cellule C;
    init_grille_from_file(argv[1],&g);
    alloue_grille (g.nbl, g.nbc, &gc);

    alloue_grille_cellule (g.nbl, g.nbc, &C);
    copie_grille_cellule(g,C);
    
    //affiche_grille(g,C, compteur, cyclique, vieillessement, tempsOscillation);
    affiche_grille(g, C, 0, 0, 0, 0);
    
    debut_jeu(&g, &gc,&C);

    libere_cellule(&g, &C);
    libere_grille(&g);
    libere_grille(&gc);
    
	return EXIT_SUCCESS;
}

#else

#include <cairo.h>
#include <cairo-xlib.h>
#include <X11/Xlib.h>

/**
 * @brief dessine la grille sous forme GUI(Graphic User Interface)
 * @param[in]  surface pointeur de cairo_surface_t
 * @param[in]  g grille
 * @param[in]  C cellule
 * @param[in]  vieillissement : activé (1) / desactivé (0) 
 */
void paint(cairo_surface_t *surface,grille g,cellule C, int vieillissement)
{
	// create cairo mask
	cairo_t *cr;
	cr=cairo_create(surface);
 
	// background
	cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
	cairo_paint(cr);
	
	for(int i=0;i<g.nbl;i++){
		for(int j=0;j<g.nbc;j++){

			if(g.cellules[i][j] == 1 || (C.cellules[i][j] > 0 && C.cellules[i][j] <= 8)){
				// filled rectangle
				cairo_rectangle(cr,(SIZEY/g.nbc)*j,(SIZEX/g.nbl)*i,(SIZEY/g.nbc),(SIZEX/g.nbl));
				cairo_set_source_rgb (cr, (8-C.cellules[i][j])/10.0, (8-C.cellules[i][j])/10.0, (8-C.cellules[i][j])/10.0);
				cairo_fill(cr);
			}
			
			if(g.cellules[i][j] == 1 && C.cellules[i][j] > 8){
				// filled rectangle
				cairo_rectangle(cr,(SIZEY/g.nbc)*j,(SIZEX/g.nbl)*i,(SIZEY/g.nbc),(SIZEX/g.nbl));
				cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
				cairo_fill(cr);
			}
			else if(g.cellules[i][j] == -1){
				// filled rectangle
				cairo_rectangle(cr,(SIZEY/g.nbc)*j,(SIZEX/g.nbl)*i,(SIZEY/g.nbc),(SIZEX/g.nbl));
				cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
				cairo_fill(cr);
			}
			else{
				cairo_fill(cr);
			}
		}
	}
	
	// line
	for(int i=0; i <= g.nbl; i++){
		cairo_set_source_rgb (cr, 0.1, 0.4, 0.7);
		cairo_move_to (cr, 0.0, ((SIZEX/g.nbl)*i));
		cairo_line_to(cr, SIZEY,((SIZEX/g.nbl)*i));
		cairo_set_line_width (cr, 1);
		cairo_stroke (cr);
	}
	
	for(int i=0; i <= g.nbc; i++){
		cairo_set_source_rgb (cr, 0.1, 0.4, 0.7);
		cairo_move_to (cr, ((SIZEY/g.nbc)*i), 0.0);
		cairo_line_to(cr, ((SIZEY/g.nbc)*i), SIZEX);
		cairo_set_line_width (cr, 2);
		cairo_stroke (cr);
	}

	cairo_destroy(cr); // destroy cairo mask
}
/*!
 * @brief fonction principale
 * @param[in] argc nombre d'argument passé en paramètre
 * @param[in] argv[] tableau de chaines de caractères correspondant aux nombres d'arguments passés en paramètre
 * @param[out] renvoie 0 si pas d'erreur et 1 sinon
 */
int main (int argc, char *argv[]){
	
	 if (argc != 2 )
	{
		printf("usage : main <fichier grille>\n");
		return 1;
	}

    grille g, gc;
    cellule C;
    
    int compteur = 0, cyclique = 0, vieillissement= 0;
    int *tempsOscillation = &compteur;
    int (*compte_voisins_vivants) (int,int,grille)=compte_voisins_vivants_non_cyclique;
    
    init_grille_from_file(argv[1],&g);
    alloue_grille (g.nbl, g.nbc, &gc);

    alloue_grille_cellule (g.nbl, g.nbc, &C);
    copie_grille_cellule(g,C);
    
	// X11 display
	Display *dpy;
	Window rootwin;
	Window win;
	XEvent e;
	int scr;
	
	// init the display
	if(!(dpy=XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: Could not open display\n");
		exit(1);
	}

	scr=DefaultScreen(dpy);
	rootwin=RootWindow(dpy, scr);

	win= XCreateSimpleWindow(dpy, rootwin, 1, 1, SIZEX, SIZEY, 0, BlackPixel(dpy, scr), BlackPixel(dpy, scr));

	XStoreName(dpy, win, "jeu de la vie");
	XSelectInput(dpy, win, ExposureMask|ButtonPressMask|KeyPressMask);
	XMapWindow(dpy, win);
	
	// create cairo surface
	cairo_surface_t *cs; 
	cs=cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy, 0), SIZEX, SIZEY);

	// run the event loop
	while(1) {
		XNextEvent(dpy, &e);
		if(e.type==Expose && e.xexpose.count<1) {
			paint(cs,g,C,vieillissement);
		}
		
		else if (e.type==KeyPress && e.xkey.keycode == 54){ // C cyclique
			if(cyclique){
                    cyclique = 0;
                    compte_voisins_vivants=&compte_voisins_vivants_non_cyclique;
                }
                
                else{
                    cyclique = 1;
                    compte_voisins_vivants=&compte_voisins_vivants_cyclique;
                }
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
			printf("Temps d'oscillation : %d ", *tempsOscillation-compteur);
			printf("\n\n");
		}
		
		else if (e.type==KeyPress && e.xkey.keycode == 55){ // V vieillessement
			vieillissement=!vieillissement;
			
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
			printf("Temps d'oscillation : %d ", *tempsOscillation-compteur);
			printf("\n\n");
		}
		
		else if (e.type==KeyPress && e.xkey.keycode == 57){ // N nouvelle grille
			    compteur = 0;
                char nom[20];
                printf("Entrer le nom de la grille : ");
                scanf("%s", nom);
                getchar();
                libere_cellule(&g, &C);
                libere_grille(&g);
                libere_grille(&gc);
                init_grille_from_file(nom,&g);
                alloue_grille (g.nbl, g.nbc, &gc);
                alloue_grille_cellule (g.nbl, g.nbc, &C);
				copie_grille_cellule(g,C);
                paint(cs,g,C,vieillissement);
				
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
				printf("Temps d'oscillation : %d ", *tempsOscillation-compteur);
				printf("\n\n");
		}
		
		else if (e.type==KeyPress && e.xkey.keycode == 32) { // O (oscillation)
			*tempsOscillation = grilleOscillante(&g, compte_voisins_vivants, vieillissement);
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
			printf("Temps d'oscillation : %d ", *tempsOscillation-compteur);
			printf("\n\n");
		}
		
		else if(e.type==ButtonPress  && e.xbutton.button == 1){ // left click 1 // middle click 1
			++compteur;
			evolue(&g,&gc,&C, compte_voisins_vivants,vieillissement);
			paint(cs,g,C,vieillissement);
			
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
			printf("Temps d'oscillation : %d ", *tempsOscillation-compteur);
			printf("\n\n");
		}
		else if(e.type==ButtonPress  && e.xbutton.button == 3){ // right click 3
			cairo_surface_destroy(cs); // destroy cairo surface
			XCloseDisplay(dpy); // close the display
			libere_cellule(&g, &C);
			libere_grille(&g);
			libere_grille(&gc);
		}
	}
		
	return EXIT_SUCCESS;
}
#endif
