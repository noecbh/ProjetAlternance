
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/plannings.h"
#include "../include/lutins.h"
#define MAX_SEGMENTS 10


int charger_semaine_ics(const char *nom_fichier, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES],
    Lutin *lutins, int nb_lutins, struct tm date_base) {

    FILE *f = fopen(nom_fichier, "r");
    if (!f) {
        fprintf(stderr, "Erreur ouverture fichier %s\n", nom_fichier);
        return -1;
    }

    /* Variables pour lire et stocker temporairement les données ICS */
    char ligne[1024];
    struct tm debut = {0}, fin = {0};
    char summary[256], location[256];
    int lecture_event = 0; /* Booléen : 1 si on est à l’intérieur d’un événement */
    int premiere_semaine_trouvee = -1;

    /* Lecture ligne par ligne du fichier ICS */
    while (fgets(ligne, sizeof(ligne), f)) {
        if (strncmp(ligne, "BEGIN:VEVENT", 12) == 0) {
            lecture_event = 1;
            memset(&debut, 0, sizeof(struct tm));
            memset(&fin, 0, sizeof(struct tm));
            summary[0] = '\0';
            location[0] = '\0';

        } else if (strncmp(ligne, "DTSTART:", 8) == 0 && lecture_event) {
            /* Extraction de la date de début */
            strptime(ligne + 8, "%Y%m%dT%H%M%S", &debut);
            debut.tm_isdst = -1;
 

        } else if (strncmp(ligne, "DTEND:", 6) == 0 && lecture_event) {
            /* Extraction de la date de fin */
            strptime(ligne + 6, "%Y%m%dT%H%M%S", &fin);
            fin.tm_isdst = -1;


        } else if (strncmp(ligne, "SUMMARY:", 8) == 0 && lecture_event) {
            /* Récupération du rôle + prénom/nom */
            strncpy(summary, ligne + 8, sizeof(summary) - 1);
            summary[sizeof(summary) - 1] = '\0';
            summary[strcspn(summary, "\r\n")] = '\0';


        } else if (strncmp(ligne, "LOCATION:", 9) == 0 && lecture_event) {
            /* Ligne de production concernée */
            strncpy(location, ligne + 9, sizeof(location) - 1);
            location[sizeof(location) - 1] = '\0';
            location[strcspn(location, "\r\n")] = '\0';
     

        } else if (strncmp(ligne, "END:VEVENT", 10) == 0 && lecture_event) {
            lecture_event = 0;

            /* Calcul du jour écoulé depuis la date de base */
            time_t base_time = mktime(&date_base);
            time_t debut_time = mktime(&debut);
            double diff_seconds = difftime(debut_time, base_time);
            int total_jours = (int)(diff_seconds / (60 * 60 * 24));


            /* Ignore si en dehors de l’intervalle géré */
            if (total_jours < 0 || total_jours >= MAX_SEMAINES * 7) {

                continue;
            }

            /* Conversion en indices semaine/jour */
            int num_semaine = total_jours / 7;
            int jour = total_jours % 7;
            if (premiere_semaine_trouvee == -1) {
                premiere_semaine_trouvee = num_semaine;
            }

            /* Arrondi de l’horaire à nos tranches fixes */
            int heure_debut = debut.tm_hour;
            int heure_fin = fin.tm_hour;
            if (heure_fin == 0) heure_fin = 24;

            if (heure_debut >= 0 && heure_debut < 8) {
                heure_debut = 0; heure_fin = 8;
            } else if (heure_debut >= 8 && heure_debut < 16) {
                heure_debut = 8; heure_fin = 16;
            } else {
                heure_debut = 16; heure_fin = 24;
            }

            /* Détection du rôle et nom du lutin */
            int role = 0;
            char prenom[128], nom[128];
            if (sscanf(summary, "%*[^-]- %127s %127[^\n]", prenom, nom) != 2) {
                fprintf(stderr, "  !! Format summary invalide : %s\n", summary);
                continue;
            }

            if (strstr(summary, "Bricoleur")) role = 1;
            else if (strstr(summary, "Contrôleur")) role = 2;
            else if (strstr(summary, "Empaqueteur")) role = 3;
            else {
                fprintf(stderr, "  !! Rôle inconnu : %s\n", summary);
                continue;
            }

            /* Lecture du numéro de ligne */
            int ligne_num = 0;
            if (sscanf(location, "Ligne %d", &ligne_num) != 1 || ligne_num < 1 || ligne_num > MAX_LIGNES) {
                printf("  !! Ligne invalide : %s\n", location);
                continue;
            }

            /* Recherche du lutin correspondant */
            Lutin *lutin = NULL;
            for (int i = 0; i < nb_lutins; i++) {
                if (strcmp(lutins[i].prenom, prenom) == 0 && strcmp(lutins[i].nom, nom) == 0) {
                    lutin = &lutins[i];
                    break;
                }
            }

            if (!lutin) {
                printf("  !! Lutin non trouvé : %s %s\n", prenom, nom);
                continue;
            }

            /* Recherche ou création du bon segment */
            LigneFabrication *ligne = &semaines[num_semaine][jour][ligne_num - 1];
            SegmentPlanning *seg = NULL;
            for (int s = 0; s < ligne->nb_segments; s++) {
                if (ligne->segments[s].debut == heure_debut && ligne->segments[s].fin == heure_fin) {
                    seg = &ligne->segments[s];
                    break;
                }
            }

            if (!seg && ligne->nb_segments < MAX_SEGMENTS) {
                seg = &ligne->segments[ligne->nb_segments++];
                seg->debut = heure_debut;
                seg->fin = heure_fin;
                seg->bricoleur = NULL;
                seg->controleur = NULL;
                seg->empaqueteur = NULL;

            }

            /* Affectation du lutin dans le bon rôle */
            if (seg) {
                if (role == 1) seg->bricoleur = lutin;
                else if (role == 2) seg->controleur = lutin;
                else if (role == 3) seg->empaqueteur = lutin;

            }
        }
    }

    fclose(f);
    return premiere_semaine_trouvee;
}


void sauvegarder_semaine_ics(const char *nom_fichier, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES], int num_semaine, struct tm date_base) {
    int jour,l,s,r;
    FILE *f = fopen(nom_fichier, "w");
    if (!f) {
        fprintf(stderr, "Erreur ouverture fichier pour écriture : %s\n", nom_fichier);
        return;
    }

    fprintf(f, "BEGIN:VCALENDAR\n");
    fprintf(f, "VERSION:2.0\n");
    fprintf(f, "PRODID:-//Exemple//Planning Lutins//FR\n\n");

    for (jour = 0; jour < 7; jour++) {
        struct tm date_jour = date_base;
        date_jour.tm_mday += num_semaine * 7 + jour;
        mktime(&date_jour);

        for (l = 0; l < MAX_LIGNES; l++) {
            LigneFabrication *ligne = &semaines[num_semaine][jour][l];
            for (s = 0; s < ligne->nb_segments; s++) {
                SegmentPlanning *seg = &ligne->segments[s];

                Lutin *roles[3] = {seg->bricoleur, seg->controleur, seg->empaqueteur};
                const char *noms_roles[] = {"Bricoleur", "Contrôleur", "Empaqueteur"};

                for (r = 0; r < 3; r++) {
                    if (roles[r] == NULL) continue;

                    struct tm dtstart = date_jour;
                    dtstart.tm_hour = seg->debut;
                    dtstart.tm_min = 0;
                    dtstart.tm_sec = 0;
                    mktime(&dtstart);

                    struct tm dtend = date_jour;
                    dtend.tm_hour = seg->fin % 24;
                    dtend.tm_min = 0;
                    dtend.tm_sec = 0;
                    if (seg->fin == 24) {
                        dtend.tm_mday += 1;
                    }
                    mktime(&dtend);

                    char buffer_start[100], buffer_end[100];
                    strftime(buffer_start, sizeof(buffer_start), "DTSTART:%Y%m%dT%H%M%S", &dtstart);
                    strftime(buffer_end, sizeof(buffer_end), "DTEND:%Y%m%dT%H%M%S", &dtend);

                    fprintf(f, "BEGIN:VEVENT\n");
                    fprintf(f, "%s\n", buffer_start);
                    fprintf(f, "%s\n", buffer_end);
                    fprintf(f, "SUMMARY:%s - %s %s\n", noms_roles[r], roles[r]->prenom, roles[r]->nom);
                    fprintf(f, "LOCATION:Ligne %d\n", l + 1);
                    fprintf(f, "END:VEVENT\n\n");
                }
            }
        }
    }

    fprintf(f, "END:VCALENDAR\n");
    fclose(f);
}




Lutin* charger_lutins(int* nb_lutins) {
    FILE* f = fopen("lutins.txt", "r");
    if (!f) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir lutins.txt\n");
        return NULL;
    }

    int capacite = 10;
    Lutin* lutins = malloc(capacite * sizeof(Lutin));
    if (!lutins) {
        fprintf(stderr, "Erreur : Allocation mémoire impossible.\n");
        fclose(f);
        return NULL;
    }

    int i = 0, ligne_num = 1;
    char ligne[256];

    while (fgets(ligne, sizeof(ligne), f)) {
        if (i >= capacite) {
            capacite *= 2;
            Lutin* temp = realloc(lutins, capacite * sizeof(Lutin));
            if (!temp) {
                fprintf(stderr, "Erreur : Reallocation impossible.\n");
                free(lutins);
                fclose(f);
                return NULL;
            }
            lutins = temp;
        }

        Lutin* l = &lutins[i];

        /* Lecture ligne principale : prénom;nom;role;statut */
        if (sscanf(ligne, "%63[^;];%49[^;];%d;%d", l->prenom, l->nom, &l->role, &l->statut) != 4) {
            fprintf(stderr, "Erreur de format (ligne %d - infos principales) : %s", ligne_num, ligne);
            ligne_num++;
            continue;  // On ignore le lutin si ligne incorrecte
        }

        
        ligne_num++;

        for (int j = 0; j < 7; j++) {
            if (!fgets(ligne, sizeof(ligne), f)) {
                fprintf(stderr, "Erreur : fin de fichier inattendue (jour %d)\n", j);
                fclose(f);
                *nb_lutins = i;
                return lutins;
            }

            if (sscanf(ligne, "%d", &l->nb_plages[j]) != 1 || l->nb_plages[j] < 0 || l->nb_plages[j] > 5) {
                fprintf(stderr, "Erreur nb_plages jour %d (ligne %d) : %s", j, ligne_num, ligne);
                l->nb_plages[j] = 0;
            }
            ligne_num++;

            for (int p = 0; p < l->nb_plages[j]; p++) {
                if (!fgets(ligne, sizeof(ligne), f)) {
                    fprintf(stderr, "Erreur : fin de fichier inattendue (horaire jour %d plage %d)\n", j, p);
                    fclose(f);
                    *nb_lutins = i;
                    return lutins;
                }

                int tmp;  /* champ "1" inutile mais présent dans chaque ligne */
                if (sscanf(ligne, "%d;%d-%d", &tmp, &l->horaires[j][p].debut, &l->horaires[j][p].fin) != 3) {
                    fprintf(stderr, "Erreur horaire jour %d plage %d (ligne %d) : %s", j, p, ligne_num, ligne);
                    l->horaires[j][p].debut = 0;
                    l->horaires[j][p].fin = 0;
                } else {
                    
                }
                ligne_num++;
            }
        }

        i++;
    }

    fclose(f);
    *nb_lutins = i;
    return lutins;
}

void sauvegarder_lutins(Lutin *lutins, int nb_lutins) {
    int i,j,p;
    FILE *f = fopen("lutins.txt", "w");
    if (!f) {
        fprintf(stderr, "Erreur fichier invalide.\n");
        return;
    }

    for (i = 0; i < nb_lutins; i++) {
        fprintf(f, "%s;%s;%d;%d\n", lutins[i].prenom, lutins[i].nom, lutins[i].role, lutins[i].statut);

        for (j = 0; j < 7; j++) {
            fprintf(f, "%d\n", lutins[i].nb_plages[j]);

            for (p = 0; p < lutins[i].nb_plages[j]; p++) {
                fprintf(f, "1;%d-%d\n", lutins[i].horaires[j][p].debut, lutins[i].horaires[j][p].fin);
            }
        }
    }

    fclose(f);
}
