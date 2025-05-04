#ifndef IHM_H
#define IHM_H

#include "../include/lutins.h"
#include "../include/plannings.h"
#include "../include/fichiers.h"
#include <MLV/MLV_all.h>


typedef struct {
    int x, y, largeur, hauteur;
    int role; /* 0 : bricoleur, 1 : controleur, 2 : empaqueteur */
    SegmentPlanning* segment;
    int ligne; 
    int debut; 
    int fin; 
} ZoneModifiable;


void creer_fenetre();
int menu();
Lutin* afficher_lutins_graph(Lutin *lutins, int *nb_lutins, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES], int courant_depart);
void rechercher_lutin_graphique(Lutin *lutins, int nb_lutins, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES]);
void afficher_lutins_par_role_graphique(Lutin *lutins, int nb_lutins, int role_voulu);
void afficher_semaine_graphique_complet(LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES], int num_semaine, struct tm date_base, Lutin *lutins, int nb_lutins);
void afficher_lutins_par_date(Lutin *lutins, int nb_lutins);
void modifier_segment_lutin(LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES], int num_semaine, int jour, int ligne, int role, int heure_debut, int heure_fin, Lutin *lutins, int nb_lutins);

#endif