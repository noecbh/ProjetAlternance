#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/lutins.h"
#include "../include/plannings.h"
#include "../include/fichiers.h"
#include "../include/ihm.h"
#include <MLV/MLV_all.h>

int main() {
    int nb_lutins = 9;
    int choix2, mode, choix_menu;
    int semaine_courante = 0;
    char choix_semaine;
    int nb_semaines = MAX_SEMAINES;

    Lutin *lutins = NULL;

    LigneFabrication (*semaines)[7][MAX_LIGNES] = malloc(sizeof(LigneFabrication) * MAX_SEMAINES * 7 * MAX_LIGNES);
    if (!semaines) {
        fprintf(stderr, "Erreur allocation mémoire pour les semaines.\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    lutins = generer_lutin(nb_lutins);
    if (!lutins) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les lutins.\n");
        free(semaines);
        exit(EXIT_FAILURE);
    }

    creer_planning_semaine(lutins, nb_lutins, semaines, &nb_semaines);

    struct tm date_base;
    date_base.tm_year = 2025 - 1900;
    date_base.tm_mon = 3;
    date_base.tm_mday = 7;
    date_base.tm_hour = 0;
    date_base.tm_min = 0;
    date_base.tm_sec = 0;
    date_base.tm_isdst = -1;
    mktime(&date_base);

    printf("Choisissez votre mode d'affichage :\n");
    printf(" 1 : Version Terminal\n");
    printf(" 2 : Version Graphique\n");
    printf(" 3 : Charger un planning (.ics)\n");
    printf("Votre choix : ");
    if (scanf("%d", &mode) != 1) {
        printf("Erreur de saisie !\n");
        free(lutins);
        free(semaines);
        return 1;
    }

    if (mode == 1) {
        printf("\nListe des lutins générés :\n");
        for (int i = 0; i < nb_lutins; i++) {
            afficher_lutin(lutins[i]);
            printf("\n");
        }

        do {
            printf("\n--- Semaine %d ---\n", semaine_courante + 1);
            afficher_semaine(semaines, semaine_courante);

            printf("\nNavigation : n (suivant), p (précédent), q (quitter)\n");
            printf("Votre choix : ");
            if (scanf(" %c", &choix_semaine) != 1) break;

            if (choix_semaine == 'n') {
                if (semaine_courante < nb_semaines - 1) semaine_courante++;
                else printf("Dernière semaine.\n");
            } else if (choix_semaine == 'p') {
                if (semaine_courante > 0) semaine_courante--;
                else printf("Première semaine.\n");
            }
        } while (choix_semaine != 'q');

        printf("\nSouhaitez-vous :\n");
        printf(" 1 : Sauvegarder le planning (.ics)\n");
        printf(" 2 : Quitter\n");
        printf("Votre choix : ");
        if (scanf("%d", &choix2) != 1) {
            free(lutins);
            free(semaines);
            return 1;
        }

        if (choix2 == 1) {
            sauvegarder_semaine_ics(semaines[semaine_courante], semaine_courante);
        }

        free(lutins);
        free(semaines);
    }

    else if (mode == 2) {
        creer_fenetre();

        while (1) {
            choix_menu = menu();
            if (choix_menu == 1) {
                afficher_lutins_graph(lutins, nb_lutins, semaines);
            }
            else if (choix_menu == 2) {
                afficher_semaine_graphique(semaines, semaine_courante, date_base);
            }
            else if (choix_menu == 3) {
                charger_planning_graphique(semaines[semaine_courante], lutins, nb_lutins, semaine_courante);
            }
            else if (choix_menu == 4) {
                free(lutins);
                free(semaines);
                MLV_free_window();
                exit(EXIT_SUCCESS);
            }
        }
    }

    else if (mode == 3) {
        char nom_fichier[100];
        printf("Entrez le nom du fichier .ics à charger : ");
        if (scanf("%99s", nom_fichier)!=1){
            printf("Erreur");
        }

        charger_planning_ics(semaines[semaine_courante], lutins, nb_lutins, nom_fichier);
        afficher_semaine(semaines, semaine_courante);

        free(lutins);
        free(semaines);
    }

    else {
        printf("Mode invalide !\n");
        free(lutins);
        free(semaines);
    }

    return 0;
}
