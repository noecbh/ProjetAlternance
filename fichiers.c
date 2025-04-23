
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/plannings.h"
#include "../include/lutins.h"

Lutin* chercher_lutin_par_prenom(Lutin *lutins, int nb, const char *prenom) {
    for (int i = 0; i < nb; i++) {
        if (strcmp(lutins[i].prenom, prenom) == 0) {
            return &lutins[i];
        }
    }
    return NULL;
}


void charger_planning_ics(LigneFabrication semaine[7][MAX_LIGNES], Lutin *lutins, int nb_lutins, const char *nom_fichier) {
    FILE *f = fopen(nom_fichier, "r");
    if (!f) {
        fprintf(stderr, "Erreur ouverture fichier ICS: %s\n", nom_fichier);
        return;
    }

    /* Réinitialiser le planning */
    int j, l;
    for (j = 0; j < 7; j++) {
        for (l = 0; l < MAX_LIGNES; l++) {
            semaine[j][l].nb_segments = 0;
        }
    }

    char ligne[256];
    int jour = -1, debut = -1, fin = -1, ligne_id = 0;
    char nom_bric[50], nom_ctrl[50], nom_empq[50];

    while (fgets(ligne, sizeof(ligne), f)) {
        if (strncmp(ligne, "DTSTART:", 8) == 0) {
            int y, m, d, h;
            sscanf(ligne + 8, "%4d%2d%2dT%2d", &y, &m, &d, &h);
            struct tm base = {0};
            base.tm_year = 2025 - 1900;
            base.tm_mon = 3;  // Avril
            base.tm_mday = 7;
            base.tm_hour = 0;
            base.tm_min = 0;
            base.tm_sec = 0;
            base.tm_isdst = -1;

            time_t t_base = mktime(&base);
            struct tm actuel = {0};
            actuel.tm_year = y - 1900;
            actuel.tm_mon = m - 1;
            actuel.tm_mday = d;
            actuel.tm_hour = h;
            actuel.tm_min = 0;
            actuel.tm_sec = 0;
            actuel.tm_isdst = -1;

            time_t t_evt = mktime(&actuel);
            double diff = difftime(t_evt, t_base);
            jour = (int)(diff / (60 * 60 * 24));
            debut = h;
        }

        else if (strncmp(ligne, "DTEND:", 6) == 0) {
            int h;
            sscanf(ligne + 6, "%*8dT%2d", &h);
            fin = (h == 0) ? 24 : h;
        }

        else if (strncmp(ligne, "DESCRIPTION:", 12) == 0) {
            sscanf(ligne, "DESCRIPTION:Bricoleur=%49[^,], Controleur=%49[^,], Empaqueteur=%49[^\n]", nom_bric, nom_ctrl, nom_empq);
        }

        else if (strncmp(ligne, "X-LINE:", 7) == 0) {
            sscanf(ligne, "X-LINE:%d", &ligne_id);

            if (jour < 0 || jour >= 7 || debut < 0 || fin <= 0) continue;

            LigneFabrication *lf = &semaine[jour][ligne_id];
            SegmentPlanning *seg = &lf->segments[lf->nb_segments];
            seg->debut = debut;
            seg->fin = fin;

            /* Lier les bons lutins */
            int i;
            for (i = 0; i < nb_lutins; i++) {
                if (strcmp(lutins[i].prenom, nom_bric) == 0) {
                    seg->bricoleur = &lutins[i];
                } else if (strcmp(lutins[i].prenom, nom_ctrl) == 0) {
                    seg->controleur = &lutins[i];
                } else if (strcmp(lutins[i].prenom, nom_empq) == 0) {
                    seg->empaqueteur = &lutins[i];
                }
            }

            lf->nb_segments++;
        }
    }

    fclose(f);
    printf("Fichier ICS '%s' chargé avec succès.\n", nom_fichier);
}


void sauvegarder_semaine_ics(LigneFabrication semaine[7][MAX_LIGNES], int semaine_index) {
    FILE *f;
    char nom_fichier[64];
    int i, j, ligne;

    /* Chercher un nom de fichier non utilisé */
    for (i = 1; i < 1000; i++) {
        sprintf(nom_fichier, "sauvegarde%d.ics", i);
        f = fopen(nom_fichier, "r");
        if (!f) break;
        fclose(f);
    }

    f = fopen(nom_fichier, "w");
    if (!f) {
        fprintf(stderr, "Erreur ouverture fichier %s\n", nom_fichier);
        return;
    }

    for (j = 0; j < 7; j++) {
        for (ligne = 0; ligne < MAX_LIGNES; ligne++) {
            LigneFabrication *lf = &semaine[j][ligne];

            for (int s = 0; s < lf->nb_segments; s++) {
                SegmentPlanning seg = lf->segments[s];

                if (!seg.bricoleur || !seg.controleur || !seg.empaqueteur)
                    continue;

                int jour = j + 7 * semaine_index;
                int debut = seg.debut;
                int fin = seg.fin;

                /* Événement .ics */
                fprintf(f, "BEGIN:VEVENT\n");
                fprintf(f, "DTSTART:202504%02dT%02d\n", 7 + jour, debut);
                if (fin == 24)
                    fprintf(f, "DTEND:202504%02dT00\n", 7 + jour + 1);
                else
                    fprintf(f, "DTEND:202504%02dT%02d\n", 7 + jour, fin);
                fprintf(f, "DESCRIPTION:Bricoleur=%s, Controleur=%s, Empaqueteur=%s\n",
                    seg.bricoleur->prenom,
                    seg.controleur->prenom,
                    seg.empaqueteur->prenom);
                fprintf(f, "X-LINE:%d\n", ligne);
                fprintf(f, "END:VEVENT\n\n");
            }
        }
    }

    fclose(f);
    printf("Planning sauvegardé dans %s\n", nom_fichier);
}
