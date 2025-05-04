#ifndef PLANNINGS_H
#define PLANNINGS_H

#include "lutins.h"
#include <time.h>
#define MAX_LIGNES 12
#define MAX_SEMAINES 57



typedef struct {
    Lutin *bricoleur;       
    Lutin *controleur;      
    Lutin *empaqueteur;      
    int debut;          
    int fin;               
} SegmentPlanning;

typedef struct {
    SegmentPlanning segments[10];  
    int nb_segments;               
} LigneFabrication;


typedef struct {
    LigneFabrication lignes[7][MAX_LIGNES];
    int nb_lignes[7];
} PlanningSemaine;



void creer_planning_optimise(Lutin *lutins, int nb_lutins, LigneFabrication *lignes, int *nb_lignes, int jour);
void creer_planning_semaine(Lutin *lutins, int nb_lutins, LigneFabrication semaine[MAX_SEMAINES][7][MAX_LIGNES], int *nb_semaines);
void generer_planning_test(LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES]);
void vider_semaine(LigneFabrication semaine[7][MAX_LIGNES]);
#endif
