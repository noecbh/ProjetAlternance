#ifndef GESTION_LUTINS_H
#define GESTION_LUTINS_H

struct LigneFabrication;

typedef struct {
    int debut;
    int fin;
} PlageHoraire;

typedef struct {
    char nom[50];
    char prenom[50];
    int role; 
    PlageHoraire horaires[7][5];
    int nb_plages[7]; 
    int statut; 
} Lutin;

Lutin* generer_lutin(int n);
void afficher_lutin(Lutin l);
void initialiser_lutin(Lutin *lutin, const char *nom, const char *prenom, int role, PlageHoraire horaires[7][5], int nb_plages[7], int statut);
int lutin_disponible(Lutin *l, int jour, int debut, int fin);
int afficher_lutins_disponibles(Lutin *lutins, int nb_lutins, int role, int *lutin_utilise, int jour, int debut, int fin);
#endif

