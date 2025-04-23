#ifndef IHM_H
#define IHM_H

#include "../include/lutins.h"
#include "../include/plannings.h"
#include "../include/fichiers.h"
#include <MLV/MLV_all.h>

void creer_fenetre();
int menu();
void afficher_lutins_graph(Lutin *lutins, int nb_lutins, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES]);
void afficher_planning_graphique(LigneFabrication *lignes, int nb_lignes, struct tm date);
void afficher_semaine_graphique(LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES], int num_semaine, struct tm date_base);
void afficher_planning_graphique_simple(LigneFabrication *lignes, int nb_lignes, struct tm date);
void charger_planning_graphique(LigneFabrication semaine[7][MAX_LIGNES], Lutin *lutins, int nb_lutins, int semaine_courante);
void afficher_lutin_graphique_unique(Lutin *lutins, int nb_lutins, int index, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES]);

void rechercher_lutin_graphique(Lutin *lutins, int nb_lutins, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES]);
void afficher_lutins_par_role_graphique(Lutin *lutins, int nb_lutins, int role_voulu, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES]);
#endif