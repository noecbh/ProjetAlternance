#ifndef IHM_H
#define IHM_H

#include "../include/lutins.h"
#include "../include/plannings.h"
#include "../include/fichiers.h"
#include <MLV/MLV_all.h>

void creer_fenetre();
int menu();
void afficher_lutins_graph(Lutin *lutins, int nb_lutins);


#endif