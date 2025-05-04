#include "../include/plannings.h"
#include "../include/lutins.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_SEMAINES 57
#define MAX_LUTINS 50


void creer_planning_optimise(Lutin *lutins, int nb_lutins, LigneFabrication *lignes, int *nb_lignes, int jour) {
    int i, j, l; 
    int lutins_utilises[nb_lutins]; 
    int lignes_creees = 0;

    int tranches[3][2] = {{0, 8}, {8, 16}, {16, 24}};

    for (i = 0; i < nb_lutins; i++) {
        lutins_utilises[i] = 0;
    }

    for (i = 0; i < 3; i++) {
        int debut = tranches[i][0];
        int fin = tranches[i][1];

        while (1) {
            /* On cherche un trio de lutins disponible pour cette tranche */
            Lutin *bricoleur = NULL;
            Lutin *controleur = NULL;
            Lutin *empaqueteur = NULL;

            for (j = 0; j < nb_lutins; j++) {
                if (lutins_utilises[j]) continue; /* Lutin déjà utilisé */
                if (lutins[j].nb_plages[jour] != 1) continue; 

                PlageHoraire dispo = lutins[j].horaires[jour][0];

                
                if (dispo.debut != debut || dispo.fin != fin) continue;

                /* On affecte le lutin à son rôle s’il est disponible et pas encore trouvé */
                if (lutins[j].role == 1 && !bricoleur) {
                    bricoleur = &lutins[j];
                } else if (lutins[j].role == 2 && !controleur) {
                    controleur = &lutins[j];
                } else if (lutins[j].role == 3 && !empaqueteur) {
                    empaqueteur = &lutins[j];
                }
            }

            /* Si on a trouvé un trio complet, on crée un segment */
            if (bricoleur && controleur && empaqueteur) {
                SegmentPlanning segment;
                segment.bricoleur = bricoleur;
                segment.controleur = controleur;
                segment.empaqueteur = empaqueteur;
                segment.debut = debut;
                segment.fin = fin;

                int ligne_placee = 0;

                /* On essaie d’ajouter le segment dans une ligne existante compatible */
                for (l = 0; l < lignes_creees; l++) {
                    int libre = 1;
                    for (int s = 0; s < lignes[l].nb_segments; s++) {
                        SegmentPlanning *exist = &lignes[l].segments[s];

             
                        if (!(segment.fin <= exist->debut || segment.debut >= exist->fin)) {
                            libre = 0;
                            break;
                        }
                    }

                    if (libre && lignes[l].nb_segments < 10) {
                        lignes[l].segments[lignes[l].nb_segments++] = segment;
                        ligne_placee = 1;
                        break;
                    }
                }

                /* Si aucune ligne existante n’est compatible, on crée une nouvelle ligne */
                if (!ligne_placee && lignes_creees < *nb_lignes) {
                    lignes[lignes_creees].segments[0] = segment;
                    lignes[lignes_creees].nb_segments = 1;
                    lignes_creees++;
                }

                /* On marque les lutins comme utilisés */
                lutins_utilises[bricoleur - lutins] = 1;
                lutins_utilises[controleur - lutins] = 1;
                lutins_utilises[empaqueteur - lutins] = 1;
            } else {
                /* On ne peut pas former une nouvelle ligne pour cette tranche, on passe à la suivante */
                break;
            }
        }
    }

    *nb_lignes = lignes_creees;
}





void creer_planning_semaine(Lutin *lutins, int nb_lutins, LigneFabrication semaine[MAX_SEMAINES][7][MAX_LIGNES], int *nb_semaines) {
    int num_semaine, jour;

    for (num_semaine = 0; num_semaine < *nb_semaines; num_semaine++) {
        for (jour = 0; jour < 7; jour++) {

            if (semaine[num_semaine][jour][0].segments[0].bricoleur != NULL ||
                semaine[num_semaine][jour][0].segments[0].controleur != NULL ||
                semaine[num_semaine][jour][0].segments[0].empaqueteur != NULL) {
                continue;
            }

            int nb_lignes = MAX_LIGNES;
            creer_planning_optimise(lutins, nb_lutins, semaine[num_semaine][jour], &nb_lignes, jour);
        }
    }
}



void generer_planning_test(LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES]) {
     Lutin lutins[90];
     int i,jour,ligne,s;

    for (i = 0; i < 90; i++) {
        snprintf(lutins[i].prenom, sizeof(lutins[i].prenom), "Lutin%d", i + 1);
        snprintf(lutins[i].nom, sizeof(lutins[i].nom), "Test%d", i + 1);
        lutins[i].role = (i % 3) + 1;  /* 1 = bricoleur, 2 = contrôleur, 3 = empaqueteur*/
    }

    for (jour = 0; jour < 7; jour++) {
        for (ligne = 0; ligne < 6; ligne++) { 
            semaines[0][jour][ligne].nb_segments = 3;

            for (s = 0; s < 3; s++) {
                SegmentPlanning *seg = &semaines[0][jour][ligne].segments[s];

                seg->bricoleur = &lutins[(jour * 18 + ligne * 3 + s * 3 + 0) % 90];
                seg->controleur = &lutins[(jour * 18 + ligne * 3 + s * 3 + 1) % 90];
                seg->empaqueteur = &lutins[(jour * 18 + ligne * 3 + s * 3 + 2) % 90];

                seg->debut = s == 0 ? 0 : (s == 1 ? 8 : 16);
                seg->fin = s == 0 ? 8 : (s == 1 ? 16 : 24);
            }
        }
    }
}

void vider_semaine(LigneFabrication semaine[7][MAX_LIGNES]) {
    int jour, ligne, s;
    for (jour = 0; jour < 7; jour++) {
        for (ligne = 0; ligne < MAX_LIGNES; ligne++) {
            semaine[jour][ligne].nb_segments = 0;
            for (s = 0; s < 10; s++) {
                semaine[jour][ligne].segments[s].bricoleur = NULL;
                semaine[jour][ligne].segments[s].controleur = NULL;
                semaine[jour][ligne].segments[s].empaqueteur = NULL;
                semaine[jour][ligne].segments[s].debut = 0;
                semaine[jour][ligne].segments[s].fin = 0;
            }
        }
    }
}
