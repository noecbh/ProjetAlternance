
#ifndef LUTINS_H
#define LUTINS_H

#define NB_HORAIRES 1



typedef struct {
    int debut;
    int fin;
} PlageHoraire;

typedef struct{
    char nom[50];
    char prenom[64];
    int role; /* 1 = bricoleur, 2 = contrôleur, 3 = empaqueteur) */
    PlageHoraire horaires[7][5];
    int nb_plages[7];
    int statut; /* 1 = dispo, 0 = indispo */
} Lutin;



Lutin* generer_lutin(int n);
Lutin* supprimer_lutin_graphique(Lutin *lutins, int *nb_lutins, int courant);
Lutin* ajouter_lutin_graphique(Lutin *lutins, int *nb_lutins);
void modifier_lutin_graphique(Lutin *lutin);


#endif
