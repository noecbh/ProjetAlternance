#ifndef FICHIERS_H
#define FICHIERS_H

#include "lutins.h"
#include "plannings.h"

void sauvegarder_planning(LigneFabrication *lignes, int nb_lignes, int jour);
void charger_planning(Lutin *lutins, int *nb_lutins, LigneFabrication *lignes, int *nb_lignes);
void exporter_planning_ics(LigneFabrication *lignes, int nb_lignes, int jour, const char *nom_fichier);
void sauvegarder_semaine_ics(LigneFabrication semaine[7][MAX_LIGNES], int num_semaine);
#endif
