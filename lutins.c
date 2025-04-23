#include "../include/plannings.h"
#include "../include/lutins.h"
#include "../include/ihm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <MLV/MLV_all.h>

void afficher_lutin(Lutin l) {
    int i, j;
    const char* jours[] = {
        "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"
    };

    printf("Nom: %s\n", l.nom);
    printf("Prénom: %s\n", l.prenom);
    printf("Rôle: %s\n", 
        l.role == 1 ? "Bricoleur" :
        l.role == 2 ? "Contrôleur" :
                      "Empaqueteur");

    printf("Horaire de travail :\n");

    for (i = 0; i < 7; i++) {
        printf("%s : ", jours[i]);
        if (l.nb_plages[i] == 0) {
            printf("Repos ");
        } else {
            for (j = 0; j < l.nb_plages[i]; j++) {
                printf("[%d - %d] ", l.horaires[i][j].debut, l.horaires[i][j].fin);
            }
        }
        printf("\n");
    }

    printf("Statut : %s\n", l.statut ? "Disponible" : "Indisponible");
}



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

        /* Tirage aléatoire de 2 jours de repos différents */
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




void initialiser_lutin(Lutin *lutin, const char *nom, const char *prenom, int role, PlageHoraire horaires[7][5], int nb_plages[7], int statut) {

    int i,j;


    strcpy(lutin->nom, nom);
    strcpy(lutin->prenom, prenom);
    lutin->role = role;

    /* Initialisation des horaires de travail */
    for (i = 0; i < 7; i++) {      /* Parcours des jours de la semaine */
        for (j = 0; j < nb_plages[i]; j++) {  /* Parcours des plages horaires pour chaque jour */
            lutin->horaires[i][j] = horaires[i][j];
        }
    }

    /* Initialisation du nombre de plages horaires par jour */
    for (i = 0; i < 7; i++) {
        lutin->nb_plages[i] = nb_plages[i]; 
    }


        lutin->statut = statut;              /* 1 = disponible, 0 = indisponible */
}







void afficher_horaires(Lutin *l) {
    int i,j;
    const char* noms_jours[] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
    for (i = 0; i < 7; i++) {
        printf("%s : ", noms_jours[i]);
        for (j = 0; j < l->nb_plages[i]; j++) {
            printf("[%d - %d] ", l->horaires[i][j].debut, l->horaires[i][j].fin);
        }
        printf("\n");
    }
}

int lutin_disponible(Lutin *l, int jour, int debut, int fin) {
    if (l == NULL || jour < 0 || jour >= 7 || debut < 0 || fin > 24 || debut >= fin) {
        return 0;
    }

    for (int i = 0; i < l->nb_plages[jour]; i++) {
        int d = l->horaires[jour][i].debut;
        int f = l->horaires[jour][i].fin;
        if (d <= debut && f >= fin) {
            return 1;
        }
    }

    return 0;
}



int afficher_lutins_disponibles(Lutin *lutins, int nb_lutins, int role, int *lutin_utilise, int jour, int debut, int fin) {
    int dispo,j,k,compatible;
    dispo = 0;
    if (role == 1) {
        printf("\nLutins disponibles pour Bricoleur :\n");
    } else if (role == 2) {
        printf("\nLutins disponibles pour Contrôleur :\n");
    } else {
        printf("\nLutins disponibles pour Empaqueteur :\n");
    }
    
    for (j = 0; j < nb_lutins; j++) {
        if (!lutin_utilise[j] && lutins[j].role == role && lutins[j].statut == 1) {
            compatible = 0;
            
            /* Vérifier si le lutin a AU MOINS UN créneau dans la plage demandée*/
            for (k = 0; k < lutins[j].nb_plages[jour]; k++) {
                if ((lutins[j].horaires[jour][k].debut < fin) && 
                    (lutins[j].horaires[jour][k].fin > debut)) {
                    compatible = 1;
                }
            }

            if (compatible==1) {
                printf("[%d] %s %s (Horaires: ", j, lutins[j].prenom, lutins[j].nom);
                for (k = 0; k < lutins[j].nb_plages[jour]; k++) {
                    printf("[%d - %d] ", lutins[j].horaires[jour][k].debut, lutins[j].horaires[jour][k].fin);
                }
                printf(")\n");
                dispo = 1;
            }
        }
    }

    return dispo;
}

int comparer_lutins(const void *a, const void *b) {
    Lutin *l1 = *(Lutin **)a;
    Lutin *l2 = *(Lutin **)b;
    return l1->horaires[1][0].debut - l2->horaires[1][0].debut;
}






Lutin* supprimer_lutin_graphique(Lutin *lutins, int *nb_lutins, int courant) {
    if (*nb_lutins <= 0 || courant < 0 || courant >= *nb_lutins) {
        MLV_draw_text(600, 750, "Aucun lutin à supprimer ou index invalide.", MLV_COLOR_RED);
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

void ajouter_lutin_graphique(Lutin **lutins, int *nb_lutins) {
    char *nom = NULL, *prenom = NULL, *role_str = NULL, *statut_str = NULL;
    char *saisie = NULL;
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
        return;
    }

    /* Champ NOM */
    MLV_draw_filled_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_WHITE);
    MLV_actualise_window();
    MLV_wait_input_box(box_x, box_y, box_w, box_h,
                       MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                       "Nom :", &nom);
    MLV_draw_text(box_x + 10, box_y + 15, nom, MLV_COLOR_BLACK);

    /* Champ PRENOM */
    MLV_wait_input_box(box_x, box_y, box_w, box_h,
                       MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                       "Prénom :", &prenom);
    MLV_draw_text(box_x + 10, box_y + 15, prenom, MLV_COLOR_BLACK);

    /* Champ ROLE */
    MLV_wait_input_box(box_x, box_y, box_w, box_h,
                       MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                       "Rôle (1=Bricoleur, 2=Contrôleur, 3=Empaqueteur) :", &role_str);
    MLV_draw_text(box_x + 10, box_y + 15, role_str, MLV_COLOR_BLACK);

    /* Champ STATUT */
    MLV_wait_input_box(box_x, box_y, box_w, box_h,
                       MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                       "Statut (1=Actif, 0=Inactif) :", &statut_str);
    MLV_draw_text(box_x + 10, box_y + 15, statut_str, MLV_COLOR_BLACK);

    role = atoi(role_str);
    statut = atoi(statut_str);

    for (i = 0; i < 7; i++) {
        char message[100];
        snprintf(message, sizeof(message), "Horaire du %s (ex: 8-16 ou 0) :", jours[i]);

        MLV_wait_input_box(box_x, box_y, box_w, box_h,
                           MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                           message, &saisie);
        MLV_draw_text(box_x + 10, box_y + 15, saisie, MLV_COLOR_BLACK);

        if (strcmp(saisie, "0") == 0) {
            nb_plages[i] = 0;
        } else {
            int debut, fin;
            if (sscanf(saisie, "%d-%d", &debut, &fin) == 2 && debut < fin && debut >= 0 && fin <= 24) {
                nb_plages[i] = 1;
                horaires[i][0].debut = debut;
                horaires[i][0].fin = fin;
            } else {
                MLV_draw_text(box_x, box_y + 70, "Format invalide. Utilisez 8-16 ou 0", MLV_COLOR_RED);
                MLV_actualise_window();
                MLV_wait_seconds(2);
                free(nom); free(prenom); free(role_str); free(statut_str); free(saisie);
                return;
            }
        }
    }

    /* Vérifie qu'il y a au moins 2 jours de repos */
    int jours_repos = 0;
    for (i = 0; i < 7; i++) {
        if (nb_plages[i] == 0) jours_repos++;
    }

    if (jours_repos < 2) {
        MLV_draw_text(box_x, box_y + 70, "Il faut au moins 2 jours de repos par semaine !", MLV_COLOR_RED);
        MLV_actualise_window();
        MLV_wait_seconds(2);
        free(nom); free(prenom); free(role_str); free(statut_str); free(saisie);
        return;
    }

    Lutin *nouveaux = realloc(*lutins, sizeof(Lutin) * (*nb_lutins + 1));
    if (!nouveaux) {
        MLV_draw_text(box_x, box_y + 70, "Erreur d'allocation mémoire !", MLV_COLOR_RED);
        MLV_actualise_window();
        MLV_wait_seconds(2);
        free(nom); free(prenom); free(role_str); free(statut_str); free(saisie);
        return;
    }

    *lutins = nouveaux;
    Lutin *ajoute = &(*lutins)[*nb_lutins];

    strncpy(ajoute->nom, nom, sizeof(ajoute->nom) - 1);
    strncpy(ajoute->prenom, prenom, sizeof(ajoute->prenom) - 1);
    ajoute->nom[sizeof(ajoute->nom) - 1] = '\0';
    ajoute->prenom[sizeof(ajoute->prenom) - 1] = '\0';
    ajoute->role = role;
    ajoute->statut = statut;

    for (i = 0; i < 7; i++) {
        ajoute->nb_plages[i] = nb_plages[i];
        if (nb_plages[i] > 0)
            ajoute->horaires[i][0] = horaires[i][0];
    }

    (*nb_lutins)++;

    MLV_draw_text(box_x, box_y + 70, "Lutin ajouté avec succès !", MLV_COLOR_GREEN);
    MLV_actualise_window();
    MLV_wait_seconds(2);

    free(nom); free(prenom); free(role_str); free(statut_str); free(saisie);
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

