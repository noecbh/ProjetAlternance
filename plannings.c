#include "../include/plannings.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_SEMAINES 57
#define MAX_LUTINS 50

struct tm convertir_jour_date(int jour_absolu) {
    struct tm date_base;
    date_base.tm_year = 2025 - 1900;
    date_base.tm_mon = 3;
    date_base.tm_mday = 7 + jour_absolu;
    date_base.tm_hour = 0;
    date_base.tm_min = 0;
    date_base.tm_sec = 0;
    date_base.tm_isdst = -1;
    mktime(&date_base);
    return date_base;
}


void afficher_planning(LigneFabrication *lignes, int nb_lignes, struct tm date) {
    int i, h, seg;
    char nom_jour[20];
    char nom_mois[20];
    int trouve;

    nom_jour[0] = '\0';
    nom_mois[0] = '\0';

    strftime(nom_jour, sizeof(nom_jour), "%A", &date);
    strftime(nom_mois, sizeof(nom_mois), "%B", &date);

    printf("\nPLANNING DU %s %02d %s %d\n\n", nom_jour, date.tm_mday, nom_mois, 1900 + date.tm_year);

    int lignes_utilisees = 0;

    for (i = 0; i < nb_lignes; i++) {
        if (lignes[i].nb_segments == 0)
            continue;  /* Ne pas afficher les lignes vides */

        lignes_utilisees++;

        printf("PLANNING DE LA LIGNE %d\n", i + 1);
        printf("+------------------------------------------------------+\n");
        printf("| Heure | Bricoleur       | Contrôleur       | Empaqueteur       |\n");
        printf("+------------------------------------------------------+\n");

        for (h = 0; h < 24; h++) {
            printf("|  %02dh  ", h);
            trouve = 0;

            for (seg = 0; seg < lignes[i].nb_segments; seg++) {
                if (h >= lignes[i].segments[seg].debut && h < lignes[i].segments[seg].fin) {
                    printf("| %-15s ", lignes[i].segments[seg].bricoleur ? lignes[i].segments[seg].bricoleur->prenom : "Personne");
                    printf("| %-15s ", lignes[i].segments[seg].controleur ? lignes[i].segments[seg].controleur->prenom : "Personne");
                    printf("| %-15s |\n", lignes[i].segments[seg].empaqueteur ? lignes[i].segments[seg].empaqueteur->prenom : "Personne");
                    trouve = 1;
                    break;
                }
            }

            if (!trouve) {
                printf("| Personne        | Personne        | Personne        |\n");
            }
        }

        printf("+------------------------------------------------------+\n\n");
    }

    if (lignes_utilisees == 0) {
        printf("Planning vide\n");
    }
}




void gerer_planning_manuel(Lutin *lutins, int nb_lutins, LigneFabrication *lignes, int *nb_lignes, int jour, int debut, int fin) {
    int i, id_bricoleur, id_controleur, id_empaqueteur;
    int lutin_utilise[nb_lutins];
    int segment_index;
    
    for (i = 0; i < nb_lutins; i++) {
        lutin_utilise[i] = 0;
    }

    *nb_lignes = 0;

    printf("\nCombien de lignes de fabrication voulez-vous créer ? ");
    if (scanf("%d", nb_lignes) != 1 || *nb_lignes <= 0) {
        printf("Entrée invalide.\n");
        return;
    }

    for (i = 0; i < *nb_lignes; i++) {
        printf("\nConfiguration de la ligne %d\n", i + 1);
        lignes[i].nb_segments = 0;
        segment_index = 0;

        /* Afficher les lutins disponibles pour chaque rôle */

        if (!afficher_lutins_disponibles(lutins, nb_lutins, 1, lutin_utilise, jour, debut, fin)) {
            printf("\nERREUR : Plus de bricoleur disponible ! Annulation du planning.\n");
            return;
        }
        do {
            printf("Choisissez un bricoleur (ID) : ");
            if ((scanf("%d", &id_bricoleur))!=1){
                printf("Erreur");
            };
        } while (id_bricoleur < 0 || id_bricoleur >= nb_lutins || lutin_utilise[id_bricoleur] || lutins[id_bricoleur].role != 1);
        lutin_utilise[id_bricoleur] = 1;

        if (!afficher_lutins_disponibles(lutins, nb_lutins, 2, lutin_utilise, jour, debut, fin)) {
            printf("\nERREUR : Plus de contrôleur disponible ! Annulation du planning.\n");
            return;
        }
        do {
            printf("Choisissez un contrôleur (ID) : ");
            if ((scanf("%d", &id_controleur)) != 1){
                printf("Erreur");
            };
        } while (id_controleur < 0 || id_controleur >= nb_lutins || lutin_utilise[id_controleur] || lutins[id_controleur].role != 2);
        lutin_utilise[id_controleur] = 1;

        if (!afficher_lutins_disponibles(lutins, nb_lutins, 3, lutin_utilise, jour, debut, fin)) {
            printf("\nERREUR : Plus d'empaqueteur disponible ! Annulation du planning.\n");
            return;
        }
        do {
            printf("Choisissez un empaqueteur (ID) : ");
            if ((scanf("%d", &id_empaqueteur)) != 1){
                printf("Erreur");
            };
        } while (id_empaqueteur < 0 || id_empaqueteur >= nb_lutins || lutin_utilise[id_empaqueteur] || lutins[id_empaqueteur].role != 3);
        lutin_utilise[id_empaqueteur] = 1;

        /* Création du segment de planning pour la ligne */
        lignes[i].segments[segment_index].bricoleur = &lutins[id_bricoleur];
        lignes[i].segments[segment_index].controleur = &lutins[id_controleur];
        lignes[i].segments[segment_index].empaqueteur = &lutins[id_empaqueteur];
        lignes[i].segments[segment_index].debut = debut;
        lignes[i].segments[segment_index].fin = fin;
        lignes[i].nb_segments++;

        printf("\nLigne %d configurée !\n", i + 1);
    }
}




void calculer_horaires_ligne(LigneFabrication *ligne, int jour) {
    int seg, min_debut, max_fin;
    for (seg = 0; seg < ligne->nb_segments; seg++) {
        min_debut = 24;
        max_fin = 0;
        /* Vérifier le lutin bricoleur du segment */
        if (ligne->segments[seg].bricoleur != NULL && ligne->segments[seg].bricoleur->statut == 1) {
            if (ligne->segments[seg].bricoleur->horaires[jour][0].debut < min_debut)
                min_debut = ligne->segments[seg].bricoleur->horaires[jour][0].debut;
            if (ligne->segments[seg].bricoleur->horaires[jour][0].fin > max_fin)
                max_fin = ligne->segments[seg].bricoleur->horaires[jour][0].fin;
        }
        /* Vérifier le lutin contrôleur du segment */
        if (ligne->segments[seg].controleur != NULL && ligne->segments[seg].controleur->statut == 1) {
            if (ligne->segments[seg].controleur->horaires[jour][0].debut < min_debut)
                min_debut = ligne->segments[seg].controleur->horaires[jour][0].debut;
            if (ligne->segments[seg].controleur->horaires[jour][0].fin > max_fin)
                max_fin = ligne->segments[seg].controleur->horaires[jour][0].fin;
        }
        /* Vérifier le lutin empaqueteur du segment */
        if (ligne->segments[seg].empaqueteur != NULL && ligne->segments[seg].empaqueteur->statut == 1) {
            if (ligne->segments[seg].empaqueteur->horaires[jour][0].debut < min_debut)
                min_debut = ligne->segments[seg].empaqueteur->horaires[jour][0].debut;
            if (ligne->segments[seg].empaqueteur->horaires[jour][0].fin > max_fin)
                max_fin = ligne->segments[seg].empaqueteur->horaires[jour][0].fin;
        }
        /* Affecter la plage horaire recalculée au segment */
        ligne->segments[seg].debut = min_debut;
        ligne->segments[seg].fin = max_fin;
    }
}

void creer_planning_optimise(Lutin *lutins, int nb_lutins, LigneFabrication *lignes, int *nb_lignes, int jour) {
    int i, j, l;
    int utilises[nb_lutins];
    int lignes_creees = 0;
    int tranches[3][2] = {{0, 8}, {8, 16}, {16, 24}};

    for (i = 0; i < nb_lutins; i++) {
        utilises[i] = 0;
    }

    for (i = 0; i < 3; i++) {
        int debut = tranches[i][0];
        int fin = tranches[i][1];

        while (1) {
            Lutin *bricoleur = NULL;
            Lutin *controleur = NULL;
            Lutin *empaqueteur = NULL;

            for (j = 0; j < nb_lutins; j++) {
                if (utilises[j]) continue;
                if (lutins[j].nb_plages[jour] != 1) continue;

                PlageHoraire h = lutins[j].horaires[jour][0];
                if (h.debut != debut || h.fin != fin) continue;

                if (lutins[j].role == 1 && !bricoleur)
                    bricoleur = &lutins[j];
                else if (lutins[j].role == 2 && !controleur)
                    controleur = &lutins[j];
                else if (lutins[j].role == 3 && !empaqueteur)
                    empaqueteur = &lutins[j];
            }

            if (bricoleur && controleur && empaqueteur) {
                SegmentPlanning seg;
                seg.bricoleur = bricoleur;
                seg.controleur = controleur;
                seg.empaqueteur = empaqueteur;
                seg.debut = debut;
                seg.fin = fin;

                /* Cherche une ligne existante compatible */
                int ligne_placee = 0;
                for (l = 0; l < lignes_creees; l++) {
                    int libre = 1;
                    for (int s = 0; s < lignes[l].nb_segments; s++) {
                        SegmentPlanning *s_exist = &lignes[l].segments[s];
                        if (!(seg.fin <= s_exist->debut || seg.debut >= s_exist->fin)) {
                            libre = 0;
                            break;
                        }
                    }

                    if (libre && lignes[l].nb_segments < 10) {
                        lignes[l].segments[lignes[l].nb_segments++] = seg;
                        ligne_placee = 1;
                        break;
                    }
                }

                /* Si aucune ligne ne convient, crée une nouvelle ligne */
                if (!ligne_placee && lignes_creees < *nb_lignes) {
                    lignes[lignes_creees].segments[0] = seg;
                    lignes[lignes_creees].nb_segments = 1;
                    lignes_creees++;
                }

                utilises[bricoleur - lutins] = 1;
                utilises[controleur - lutins] = 1;
                utilises[empaqueteur - lutins] = 1;
            } else {
                break;
            }
        }
    }

    *nb_lignes = lignes_creees;

}





void generer_semaine(Lutin *lutins, int nb_lutins, LigneFabrication semaine[MAX_LIGNES]) {
    int jour;
    for (jour = 0; jour < 7; jour++) {
        int nb_lignes = MAX_LIGNES;
        creer_planning_optimise(lutins, nb_lutins, &semaine[jour], &nb_lignes, jour);
    }
}


void creer_planning_semaine(Lutin *lutins, int nb_lutins, LigneFabrication semaine[MAX_SEMAINES][7][MAX_LIGNES], int *nb_semaines) {
    int num_semaine, jour;

    /* Pour chaque semaine */
    for (num_semaine = 0; num_semaine < *nb_semaines; num_semaine++) {
        for (jour = 0; jour < 7; jour++) {
            int nb_lignes = MAX_LIGNES;

            /* Appel à la fonction d'optimisation réelle */
            creer_planning_optimise(lutins, nb_lutins, semaine[num_semaine][jour], &nb_lignes, jour);
        }
    }
}



void afficher_semaine(LigneFabrication semaine[MAX_SEMAINES][7][MAX_LIGNES], int num_semaine) {
    int jour;
    struct tm date_du_jour;

    for (jour = 0; jour < 7; jour++) {
        /* Initialisation de la date du jour */
        date_du_jour.tm_year = 2025 - 1900;
        date_du_jour.tm_mon = 3; /* Avril = 3 car janvier = 0 */
        date_du_jour.tm_mday = 7 + num_semaine * 7 + jour;
        date_du_jour.tm_hour = 0;
        date_du_jour.tm_min = 0;
        date_du_jour.tm_sec = 0;
        date_du_jour.tm_isdst = -1;
        mktime(&date_du_jour); /* Pour que la date soit correcte */

        afficher_planning(semaine[num_semaine][jour], MAX_LIGNES, date_du_jour);
    }
}


void supprimer_lutin_des_plannings(LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES], Lutin *lutin_supprime) {
    int s, j, l, seg;

    for (s = 0; s < MAX_SEMAINES; s++) {
        for (j = 0; j < 7; j++) {
            for (l = 0; l < MAX_LIGNES; l++) {
                LigneFabrication *ligne = &semaines[s][j][l];

                for (seg = 0; seg < ligne->nb_segments; seg++) {
                    SegmentPlanning *segment = &ligne->segments[seg];

                    if (segment->bricoleur == lutin_supprime) segment->bricoleur = NULL;
                    if (segment->controleur == lutin_supprime) segment->controleur = NULL;
                    if (segment->empaqueteur == lutin_supprime) segment->empaqueteur = NULL;
                }
            }
        }
    }
}
