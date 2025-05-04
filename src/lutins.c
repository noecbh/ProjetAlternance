#include "../include/plannings.h"
#include "../include/lutins.h"
#include "../include/ihm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <MLV/MLV_all.h>


Lutin* generer_lutin(int nb_lutins) {
    int i, j;
    Lutin* lutins = malloc(nb_lutins * sizeof(Lutin));
    if (lutins == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les lutins.\n");
        exit(EXIT_FAILURE);
    }

    const char* prenoms[] = {
        "Cristiano", "Alexandre", "Anthony",
        "Desire", "Bradley", "Oussama",
        "Franck", "Romain", "Lucie"
    };

    const char* noms[] = {
        "Ronaldo", "Dupont", "Martinez",
        "Zola", "Smith", "Benali",
        "Durand", "Gauthier", "Lemoine"
    };

    int roles[] = {
        1, 1, 1,  // Bricoleurs
        2, 2, 2,  // Contrôleurs
        3, 3, 3   // Empaqueteurs
    };

    for (i = 0; i < nb_lutins; i++) {
        snprintf(lutins[i].prenom, sizeof(lutins[i].prenom), "%s", prenoms[i]);
        snprintf(lutins[i].nom, sizeof(lutins[i].nom), "%s", noms[i]);
        lutins[i].role = roles[i];
        lutins[i].statut = 1;

        int repos1 = rand() % 7;
        int repos2;
        do {
            repos2 = rand() % 7;
        } while (repos2 == repos1);

        for (j = 0; j < 7; j++) {
            if (j == repos1 || j == repos2) {
                lutins[i].nb_plages[j] = 0;
                lutins[i].horaires[j][0].debut = 0;
                lutins[i].horaires[j][0].fin = 0;
            } else {
                int tirage = rand() % 3;
                int debut = tirage * 8;
                int fin = debut + 8;

                lutins[i].nb_plages[j] = 1;
                lutins[i].horaires[j][0].debut = debut;
                lutins[i].horaires[j][0].fin = fin;
            }
        }
    }

    return lutins;
}








Lutin* supprimer_lutin_graphique(Lutin *lutins, int *nb_lutins, int courant) {
    if (*nb_lutins <= 0 || courant < 0 || courant >= *nb_lutins) {
        MLV_draw_text(600, 750, "Aucun lutin à supprimer", MLV_COLOR_RED);
        return lutins;
    }

    char message[200];
    snprintf(message, sizeof(message), "Lutin %s %s supprimé avec succès.",
             lutins[courant].prenom, lutins[courant].nom);
    MLV_draw_text(600, 750, message, MLV_COLOR_RED);

    for (int i = courant; i < *nb_lutins - 1; i++) {
        lutins[i] = lutins[i + 1];
    }

    (*nb_lutins)--;

    return lutins;
}



void modifier_lutin_graphique(Lutin *lutin) {
    int x, y;
    int box_x = 700, box_y = 520, box_w = 500, box_h = 50;
    const char *jours[] = {
        "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"
    };
    int i;

    char buffer[100];
    char *saisie = NULL;

    MLV_Image *fond = MLV_load_image("./ressources/ModifierLutin.jpeg");
    if (fond) {
        MLV_resize_image_with_proportions(fond, 1600, 1000);
        MLV_draw_image(fond, 0, 0);
    }

    MLV_draw_text(0,0, "Cliquez sur un champ pour le modifier", MLV_COLOR_BLACK);

    int champs_y[12];
    for (i = 0; i < 12; i++) {
        champs_y[i] = 220 + i * 60;
    }

    while (1) {
        if (fond) MLV_draw_image(fond, 0, 0);

        MLV_draw_text(box_x, champs_y[0], "Nom :", MLV_COLOR_BLACK);
        MLV_draw_text(box_x + 150, champs_y[0], lutin->nom, MLV_COLOR_BLACK);

        MLV_draw_text(box_x, champs_y[1], "Prénom :", MLV_COLOR_BLACK);
        MLV_draw_text(box_x + 150, champs_y[1], lutin->prenom, MLV_COLOR_BLACK);

        snprintf(buffer, sizeof(buffer), "%d", lutin->role);
        MLV_draw_text(box_x, champs_y[2], "Rôle (1=B, 2=C, 3=E) :", MLV_COLOR_BLACK);
        MLV_draw_text(box_x + 300, champs_y[2], buffer, MLV_COLOR_BLACK);

        snprintf(buffer, sizeof(buffer), "%d", lutin->statut);
        MLV_draw_text(box_x, champs_y[3], "Statut (1=Actif, 0=Inactif) :", MLV_COLOR_BLACK);
        MLV_draw_text(box_x + 300, champs_y[3], buffer, MLV_COLOR_BLACK);

        for (i = 0; i < 7; i++) {
            char horaire[50] = "Repos";
            if (lutin->nb_plages[i] > 0) {
                snprintf(horaire, sizeof(horaire), "%d-%d", lutin->horaires[i][0].debut, lutin->horaires[i][0].fin);
            }
            snprintf(buffer, sizeof(buffer), "%s : %s", jours[i], horaire);
            MLV_draw_text(box_x, champs_y[4 + i], buffer, MLV_COLOR_BLACK);
        }


        MLV_actualise_window();

        MLV_wait_mouse(&x, &y);

        if (x >= 1530 && x <= 1590 && y >= 935 && y <= 990) {
            if (fond) MLV_free_image(fond);
            return;
        }

        for (i = 0; i < 12; i++) {
            if (y >= champs_y[i] && y <= champs_y[i] + 40) {
                MLV_wait_input_box(box_x, box_y+325, box_w, box_h,
                    MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
                    "Modifier :", &saisie);

                if (saisie == NULL) continue;

                if (i == 0) strncpy(lutin->nom, saisie, sizeof(lutin->nom) - 1);
                else if (i == 1) strncpy(lutin->prenom, saisie, sizeof(lutin->prenom) - 1);
                else if (i == 2) lutin->role = atoi(saisie);
                else if (i == 3) lutin->statut = atoi(saisie);
                else {
                    if (strcmp(saisie, "0") == 0) {
                        lutin->nb_plages[i - 4] = 0;
                    } else {
                        int debut, fin;
                        if (sscanf(saisie, "%d-%d", &debut, &fin) == 2 && debut < fin) {
                            lutin->nb_plages[i - 4] = 1;
                            lutin->horaires[i - 4][0].debut = debut;
                            lutin->horaires[i - 4][0].fin = fin;
                        }
                    }
                }
                free(saisie);
            }
        }
    }
    
}

Lutin* ajouter_lutin_graphique(Lutin *lutins, int *nb_lutins) {
    /* bug avec sizeof donc je mets juste 100 pour être large, à corriger*/
    char *nom = malloc(100);
    char *prenom = malloc(100);
    char *role_str = malloc(100);
    char *statut_str = malloc(100);
    char *saisie = malloc(100);
    int role, statut;
    PlageHoraire horaires[7][5];
    int nb_plages[7] = {0};
    int i;

    int box_x = 540, box_y = 520, box_w = 500, box_h = 50;

    const char *jours[] = {
        "lundi", "mardi", "mercredi", "jeudi", "vendredi", "samedi", "dimanche"
    };

    MLV_Image *fond = MLV_load_image("./ressources/AjouterLutin.jpeg");
    if (fond) {
        MLV_resize_image_with_proportions(fond, 1600, 1000);
        MLV_draw_image(fond, 0, 0);
    }

    int x, y;
    MLV_wait_mouse(&x, &y);
    if (x >= 1530 && x <= 1590 && y >= 935 && y <= 1000) {
        MLV_free_image(fond);
        free(nom); free(prenom); free(role_str); free(statut_str); free(saisie);
        return lutins;
    }

    MLV_wait_input_box(box_x, box_y, box_w, box_h,
                       MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                       "Nom :", &nom);
    MLV_wait_input_box(box_x, box_y, box_w, box_h,
                       MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                       "Prénom :", &prenom);
    MLV_wait_input_box(box_x, box_y, box_w, box_h,
                       MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                       "Rôle (1=Bricoleur, 2=Contrôleur, 3=Empaqueteur) :", &role_str);
    MLV_wait_input_box(box_x, box_y, box_w, box_h,
                       MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                       "Statut (1=Actif, 0=Inactif) :", &statut_str);

    role = atoi(role_str);
    statut = atoi(statut_str);

    for (i = 0; i < 7; i++) {
        char message[100];
        snprintf(message, sizeof(message), "Horaire du %s (ex: 8-16 ou 0) :", jours[i]);

        MLV_wait_input_box(box_x, box_y, box_w, box_h,
                           MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                           message, &saisie);

        if (strcmp(saisie, "0") == 0) {
            nb_plages[i] = 0;
        } else {
            int debut, fin;
            if (sscanf(saisie, "%d-%d", &debut, &fin) == 2 && debut < fin && debut >= 0 && fin <= 24) {
                nb_plages[i] = 1;
                horaires[i][0].debut = debut;
                horaires[i][0].fin = fin;
            } else {
                MLV_draw_text(box_x, box_y, "Format invalide. Utilisez 8-16 ou 0", MLV_COLOR_RED);
                MLV_actualise_window();
                MLV_wait_seconds(2);
                free(nom); free(prenom); free(role_str); free(statut_str); free(saisie);
                return lutins;
            }
        }
    }

    /* Vérifie qu'il y a au moins 2 jours de repos */
    int jours_repos = 0;
    for (i = 0; i < 7; i++) {
        if (nb_plages[i] == 0) jours_repos++;
    }

    if (jours_repos < 2) {
        MLV_draw_text(box_x, box_y, "Il faut au moins 2 jours de repos par semaine !", MLV_COLOR_RED);
        MLV_actualise_window();
        MLV_wait_seconds(2);
        free(nom); free(prenom); free(role_str); free(statut_str); free(saisie);
        return lutins;
    }

    Lutin *nouveaux = realloc(lutins, sizeof(Lutin) * (*nb_lutins + 1));
    if (!nouveaux) {
        MLV_draw_text(box_x, box_y, "Erreur d'allocation mémoire !", MLV_COLOR_RED);
        MLV_actualise_window();
        MLV_wait_seconds(2);
        free(nom); free(prenom); free(role_str); free(statut_str); free(saisie);
        return lutins;
    }
    lutins = nouveaux;

    Lutin *ajoute = &lutins[*nb_lutins];

    strncpy(ajoute->nom, nom, sizeof(ajoute->nom) - 1);
    ajoute->nom[sizeof(ajoute->nom) - 1] = '\0';
    strncpy(ajoute->prenom, prenom, sizeof(ajoute->prenom) - 1);
    ajoute->prenom[sizeof(ajoute->prenom) - 1] = '\0';
    ajoute->role = role;
    ajoute->statut = statut;

    for (i = 0; i < 7; i++) {
        ajoute->nb_plages[i] = nb_plages[i];
        if (nb_plages[i] > 0)
            ajoute->horaires[i][0] = horaires[i][0];
    }

    (*nb_lutins)++;

    MLV_draw_text(box_x, box_y, "Lutin ajouté avec succès !", MLV_COLOR_GREEN);
    MLV_actualise_window();
    MLV_wait_seconds(2);

    free(nom); free(prenom); free(role_str); free(statut_str); free(saisie);
    return lutins;
}
