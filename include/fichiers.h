#ifndef FICHIERS_H
#define FICHIERS_H

#include "lutins.h"
#include "plannings.h"


int charger_semaine_ics(const char *nom_fichier, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES], Lutin *lutins, int nb_lutins, struct tm date_base);
void sauvegarder_semaine_ics(const char *nom_fichier, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES], int num_semaine, struct tm date_base);
void sauvegarder_lutins(Lutin *lutins, int nb_lutins);
Lutin* charger_lutins(int *nb_lutins);





#endif
