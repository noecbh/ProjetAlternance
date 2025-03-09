#ifndef GESTION_FICHIERS_H
#define GESTION_FICHIERS_H

#include "gestion_lutins.h"
#include "gestion_planning.h"

void charger_planning(Lutin *lutins, int *nb_lutins, LigneFabrication *lignes, int *nb_lignes);
void sauvegarder_planning(LigneFabrication *lignes, int nb_lignes, int jour);

#endif

