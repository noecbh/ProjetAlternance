
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
    int nb_plages[7]; /* Nombre de plages horaires (ex : dispo de 10-12 et 14-16 = 2 plages horaires*/
    int statut; /* 1 = dispo, 0 = indispo */
} Lutin;



void afficher_lutin(Lutin l);
void initialiser_lutin(Lutin *lutin, const char *nom, const char *prenom, int role, PlageHoraire horaires[7][5], int nb_plages[7], int statut);
Lutin* generer_lutin(int n);
void afficher_horaires(Lutin *l);
int lutin_disponible(Lutin *l, int jour, int debut, int fin);
int afficher_lutins_disponibles(Lutin *lutins, int nb_lutins, int role, int *lutin_utilise, int jour, int debut, int fin);
int comparer_lutins(const void *a, const void *b);
Lutin* supprimer_lutin_graphique(Lutin *lutins, int *nb_lutins, int courant);
void ajouter_lutin_graphique(Lutin **lutins, int *nb_lutins);
void modifier_lutin_graphique(Lutin *lutin);


#endif
