#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../include/lutins.h"
#include "../include/plannings.h"

void sauvegarder_planning(LigneFabrication *lignes, int nb_lignes, int jour) {
    FILE *fichier;
    char nom_fichier[100];
    int i, s;

    printf("Entrez le nom du fichier pour sauvegarder le planning (ex: planning_noel.txt) : ");
    if (scanf("%s", nom_fichier) == 0) {
        printf("Erreur de saisie.\n");
        return;
    }

    fichier = fopen(nom_fichier, "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s pour la sauvegarde !\n", nom_fichier);
        return;
    }

    fprintf(fichier, "PLANNING DU JOUR %d\n", jour);

    for (i = 0; i < nb_lignes; i++) {
        fprintf(fichier, "\nLigne %d:\n", i + 1);

        for (s = 0; s < lignes[i].nb_segments; s++) {
            SegmentPlanning *seg = &lignes[i].segments[s];

            fprintf(fichier, "Segment %d - [%02dh - %02dh]:\n", s + 1, seg->debut, seg->fin);

            if (seg->bricoleur)
                fprintf(fichier, "  Bricoleur: %s %s\n", seg->bricoleur->prenom, seg->bricoleur->nom);
            else
                fprintf(fichier, "  Bricoleur: Aucun\n");

            if (seg->controleur)
                fprintf(fichier, "  Contrôleur: %s %s\n", seg->controleur->prenom, seg->controleur->nom);
            else
                fprintf(fichier, "  Contrôleur: Aucun\n");

            if (seg->empaqueteur)
                fprintf(fichier, "  Empaqueteur: %s %s\n", seg->empaqueteur->prenom, seg->empaqueteur->nom);
            else
                fprintf(fichier, "  Empaqueteur: Aucun\n");
        }
    }

    fclose(fichier);
    printf("\nPlanning sauvegardé dans '%s' !\n", nom_fichier);
}



void charger_planning(Lutin *lutins, int *nb_lutins, LigneFabrication *lignes, int *nb_lignes) {
    FILE *fichier;
    char nom_fichier[100];
    char ligne[256];
    int ligne_index = -1;
    int segment_index;
    int jour;
    char role[20], prenom[50], nom[50];
    int debut, fin;
    Lutin *l;

    printf("Entrez le nom du fichier à charger (ex: planning_noel.txt) : ");
    if (scanf("%s", nom_fichier) != 1) {
        printf("Erreur de saisie !\n");
        return;
    }

    fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
        return;
    }

    *nb_lutins = 0;
    *nb_lignes = 0;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (sscanf(ligne, "PLANNING DU JOUR %d", &jour) == 1) {
            continue;
        }

        if (sscanf(ligne, "Ligne %d:", &ligne_index) == 1) {
            lignes[ligne_index].nb_segments = 0;
            (*nb_lignes)++;
            continue;
        }

        if (sscanf(ligne, "Segment %d - [%d - %d]:", &segment_index, &debut, &fin) == 3) {
            segment_index--;
            lignes[ligne_index].segments[segment_index].debut = debut;
            lignes[ligne_index].segments[segment_index].fin = fin;
            lignes[ligne_index].segments[segment_index].bricoleur = NULL;
            lignes[ligne_index].segments[segment_index].controleur = NULL;
            lignes[ligne_index].segments[segment_index].empaqueteur = NULL;
            lignes[ligne_index].nb_segments++;
            continue;
        }

        if (sscanf(ligne, "  %[^:]: %s %s", role, prenom, nom) == 3) {
            l = &lutins[*nb_lutins];
            strcpy(l->prenom, prenom);
            strcpy(l->nom, nom);
            l->statut = 1;
            l->nb_plages[jour] = 1;
            l->horaires[jour][0].debut = debut;
            l->horaires[jour][0].fin = fin;

            if (strcmp(role, "Bricoleur") == 0) {
                l->role = 1;
                lignes[ligne_index].segments[segment_index].bricoleur = l;
            } else if (strcmp(role, "Contrôleur") == 0) {
                l->role = 2;
                lignes[ligne_index].segments[segment_index].controleur = l;
            } else if (strcmp(role, "Empaqueteur") == 0) {
                l->role = 3;
                lignes[ligne_index].segments[segment_index].empaqueteur = l;
            }

            (*nb_lutins)++;
        }
    }

    fclose(fichier);
    printf("\nPlanning chargé depuis '%s' !\n", nom_fichier);
}



void exporter_planning_ics(LigneFabrication *lignes, int nb_lignes, int jour, const char *nom_fichier) {
    FILE *f = fopen(nom_fichier, "w");
    int i;
    char buffer[100];
    time_t now = time(NULL);
    struct tm *gmt = gmtime(&now);

    if (!f) {
        printf("Erreur : impossible d'ouvrir le fichier %s pour écriture.\n", nom_fichier);
        return;
    }

    fprintf(f, "BEGIN:VCALENDAR\r\n");
    fprintf(f, "VERSION:2.0\r\n");
    fprintf(f, "PRODID:-//Planning Lutins//FR\r\n");

    for (i = 0; i < nb_lignes; i++) {
        int s;
        for (s = 0; s < lignes[i].nb_segments; s++) {
            SegmentPlanning *seg = &lignes[i].segments[s];

            strftime(buffer, sizeof(buffer), "%Y%m%dT%H%M%SZ", gmt);

            char dtstart[50], dtend[50];
            snprintf(dtstart, sizeof(dtstart), "2025040%dT%02d0000Z", jour + 1, seg->debut);
            snprintf(dtend, sizeof(dtend), "2025040%dT%02d0000Z", jour + 1, seg->fin);

            char resume[200] = "";
            if (seg->bricoleur)
                strcat(resume, seg->bricoleur->prenom);
            if (seg->controleur) {
                if (strlen(resume) > 0) strcat(resume, ", ");
                strcat(resume, seg->controleur->prenom);
            }
            if (seg->empaqueteur) {
                if (strlen(resume) > 0) strcat(resume, ", ");
                strcat(resume, seg->empaqueteur->prenom);
            }

            char uid[50];
            snprintf(uid, sizeof(uid), "ligne%d-segment%d@lutins", i + 1, s + 1);

            fprintf(f, "BEGIN:VEVENT\r\n");
            fprintf(f, "UID:%s\r\n", uid);
            fprintf(f, "DTSTAMP:%s\r\n", buffer);
            fprintf(f, "DTSTART:%s\r\n", dtstart);
            fprintf(f, "DTEND:%s\r\n", dtend);
            fprintf(f, "SUMMARY:Travail ligne %d (de %dh à %dh)\r\n", i + 1, seg->debut, seg->fin);
            fprintf(f, "DESCRIPTION:Ligne %d\n%s\r\n", i + 1, resume);
            fprintf(f, "END:VEVENT\r\n");
        }
    }

    fprintf(f, "END:VCALENDAR\r\n");
    fclose(f);
    printf("Export terminé dans le fichier '%s'\n", nom_fichier);
}



void sauvegarder_semaine_ics(LigneFabrication semaine[7][MAX_LIGNES], int num_semaine) {
    FILE *fichier;
    char nom_fichier[50];
    int jour, ligne, seg;
    struct tm date_jour, debut_event, fin_event;
    char debut[20], fin[20];

    date_jour.tm_year = 2025 - 1900;
    date_jour.tm_mon = 3;
    date_jour.tm_mday = 7 + (num_semaine * 7);
    date_jour.tm_hour = 0;
    date_jour.tm_min = 0;
    date_jour.tm_sec = 0;
    date_jour.tm_isdst = -1;

    mktime(&date_jour);

    sprintf(nom_fichier, "semaine_%d.ics", num_semaine + 1);

    fichier = fopen(nom_fichier, "w");
    if (!fichier) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fprintf(fichier, "BEGIN:VCALENDAR\n");
    fprintf(fichier, "VERSION:2.0\n");
    fprintf(fichier, "PRODID:-//GestionPlannings//EN\n");

    for (jour = 0; jour < 7; jour++) {

        for (ligne = 0; ligne < MAX_LIGNES; ligne++) {

            for (seg = 0; seg < semaine[jour][ligne].nb_segments; seg++) {

                debut_event = date_jour;
                debut_event.tm_hour = semaine[jour][ligne].segments[seg].debut;
                debut_event.tm_min = 0;
                debut_event.tm_sec = 0;

                fin_event = date_jour;
                fin_event.tm_hour = semaine[jour][ligne].segments[seg].fin;
                fin_event.tm_min = 0;
                fin_event.tm_sec = 0;

                strftime(debut, sizeof(debut), "%Y%m%dT%H%M%S", &debut_event);
                strftime(fin, sizeof(fin), "%Y%m%dT%H%M%S", &fin_event);

                fprintf(fichier, "BEGIN:VEVENT\n");
                fprintf(fichier, "SUMMARY:Ligne %d : %s, %s, %s\n",
                    ligne + 1,
                    semaine[jour][ligne].segments[seg].bricoleur ? semaine[jour][ligne].segments[seg].bricoleur->prenom : "Personne",
                    semaine[jour][ligne].segments[seg].controleur ? semaine[jour][ligne].segments[seg].controleur->prenom : "Personne",
                    semaine[jour][ligne].segments[seg].empaqueteur ? semaine[jour][ligne].segments[seg].empaqueteur->prenom : "Personne"
                );
                fprintf(fichier, "DTSTART:%s\n", debut);
                fprintf(fichier, "DTEND:%s\n", fin);
                fprintf(fichier, "DESCRIPTION:Ligne %d\n", ligne + 1);
                fprintf(fichier, "END:VEVENT\n");
            }
        }

        date_jour.tm_mday++;
        mktime(&date_jour);
    }

    fprintf(fichier, "END:VCALENDAR\n");
    fclose(fichier);

    printf("Planning sauvegardé dans %s\n", nom_fichier);
}
