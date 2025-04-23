#ifndef FICHIERS_H
#define FICHIERS_H

#include "lutins.h"
#include "plannings.h"


void charger_planning_ics(LigneFabrication semaine[7][MAX_LIGNES], Lutin *lutins, int nb_lutins, const char *nom_fichier);
Lutin* chercher_lutin_par_prenom(Lutin *lutins, int nb, const char *prenom);
void sauvegarder_semaine_ics(LigneFabrication semaine[7][MAX_LIGNES], int semaine_index);






#endif
