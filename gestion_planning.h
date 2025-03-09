#ifndef GESTION_PLANNING_H
#define GESTION_PLANNING_H

#include "gestion_lutins.h"

typedef struct {
    Lutin *bricoleur;
    Lutin *controleur;
    Lutin *empaqueteur;
    PlageHoraire horaires[3];
} LigneFabrication;

void generer_planning(Lutin *lutins, int nb_lutins, LigneFabrication *lignes, int nb_lignes, int jour);
void afficher_planning(LigneFabrication *lignes, int nb_lignes, int jour);
void sauvegarder_planning(LigneFabrication *lignes, int nb_lignes, int jour);
void charger_planning(Lutin *lutins, int *nb_lutins, LigneFabrication *lignes, int *nb_lignes);
void gerer_planning_manuel(Lutin *lutins, int nb_lutins, LigneFabrication *lignes, int nb_lignes, int jour, int debut, int fin);
void afficher_horaires(Lutin *l);
void calculer_horaires_ligne(LigneFabrication *ligne);
int afficher_lutins_disponibles(Lutin *lutins, int nb_lutins, int role, int *lutin_utilise, int jour, int debut, int fin);

#endif

