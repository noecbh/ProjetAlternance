#ifndef PLANNINGS_H
#define PLANNINGS_H

#include "lutins.h"
#include <time.h>
#define MAX_LIGNES 12
#define MAX_SEMAINES 57



typedef struct {
    Lutin *bricoleur;       /* Lutin affecté au poste de bricoleur pour ce segment */
    Lutin *controleur;       /* Lutin affecté au poste de contrôleur pour ce segment */
    Lutin *empaqueteur;      /* Lutin affecté au poste d'empaqueteur pour ce segment */
    int debut;              /* Heure de début du segment */
    int fin;                /* Heure de fin du segment */
} SegmentPlanning;

typedef struct {
    SegmentPlanning segments[10];  /* Tableau de segments de planning (nombre maximum configurable) */
    int nb_segments;               /* Nombre de segments actuellement utilisés */
} LigneFabrication;


typedef struct {
    LigneFabrication lignes[7][MAX_LIGNES];
    int nb_lignes[7];
} PlanningSemaine;



void afficher_planning(LigneFabrication *lignes, int nb_lignes, struct tm date);
void calculer_horaires_ligne(LigneFabrication *ligne, int jour);
void gerer_planning_manuel(Lutin *lutins, int nb_lutins, LigneFabrication *lignes, int *nb_lignes, int jour, int debut, int fin);
void creer_planning_optimise(Lutin *lutins, int nb_lutins, LigneFabrication *lignes, int *nb_lignes, int jour);
void creer_planning_semaine(Lutin *lutins, int nb_lutins, LigneFabrication semaine[MAX_SEMAINES][7][MAX_LIGNES], int *nb_semaines);
void generer_semaine(Lutin *lutins, int nb_lutins, LigneFabrication semaine[MAX_LIGNES]);
void afficher_semaine(LigneFabrication semaine[MAX_SEMAINES][7][MAX_LIGNES], int num_semaine);
struct tm convertir_jour_date(int jour_absolu);

#endif
