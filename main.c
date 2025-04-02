#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/lutins.h"
#include "../include/plannings.h"
#include "../include/fichiers.h"
#include "../include/ihm.h"
#include <MLV/MLV_all.h>

int main() {
    int i, nb_lutins = 9, nb_lignes = MAX_LIGNES, jour = 1, debut = 0, fin = 23, choix, choix2, mode,choix_menu;
    Lutin *lutins = NULL;
    LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES];
    int nb_semaines = MAX_SEMAINES;
    int semaine_courante = 0;
    char choix_semaine;

    srand(time(NULL));

    printf("Choisissez votre mode d'exécution :\n");
    printf(" 1 : Version Terminal\n");
    printf(" 2 : Version Graphique\n");
    printf("Votre choix : ");
    if (scanf("%d", &mode) != 1) {
        printf("Erreur de saisie du mode !\n");
        exit(EXIT_FAILURE);
    }

    /* ---------------------------------- */
    /*           MODE GRAPHIQUE           */
    /* ---------------------------------- */
    if (mode == 2) {
        printf("Partie GRAPHIQUE\n");

        creer_fenetre();
        lutins = generer_lutin(nb_lutins);

        if (!lutins) {
            fprintf(stderr, "Erreur d'allocation mémoire pour les lutins.\n");
            exit(EXIT_FAILURE);
        }

        while (1) {
            choix_menu = menu(); 
            if (choix_menu == 1) {
                afficher_lutins_graph(lutins, nb_lutins);
            }
            else if (choix_menu == 2) {
                // afficher_planning_graph(); // À implémenter plus tard
            }
            else if (choix_menu == 3) {
                // charger_planning_graphique(); // À implémenter plus tard
            }
            else if (choix_menu == 4) {
                free(lutins);
                MLV_free_window();
                exit(EXIT_SUCCESS);
            }
        }
    }

    /* ---------------------------------- */
    /*           MODE TERMINAL            */
    /* ---------------------------------- */
    else if (mode == 1) {
        lutins = generer_lutin(nb_lutins);
        if (lutins == NULL) {
            printf("Erreur d'allocation mémoire des lutins.\n");
            return 1;
        }

        printf("\nListe des lutins générés :\n");
        for (i = 0; i < nb_lutins; i++) {
            afficher_lutin(lutins[i]);
            printf("\n");
        }

        printf("\nSouhaitez-vous :\n");
        printf(" 1: Générer automatiquement\n");
        printf(" 2: Remplir manuellement\n");
        printf(" 3: Charger un planning\n");
        printf(" 4: Générer un planning optimisé pour la journée\n");
        printf(" 5: Générer un planning optimisé pour la semaine\n");
        printf(" 6: Quitter\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            printf("Erreur de saisie !\n");
            free(lutins);
            exit(EXIT_FAILURE);
        }

        if (choix == 1) {
            printf("test\n");
        } else if (choix == 2) {
            LigneFabrication lignes[MAX_LIGNES];
            gerer_planning_manuel(lutins, nb_lutins, lignes, &nb_lignes, jour, debut, fin);
            afficher_planning(lignes, nb_lignes, convertir_jour_date(0));
        } else if (choix == 3) {
            LigneFabrication lignes[MAX_LIGNES];
            charger_planning(lutins, &nb_lutins, lignes, &nb_lignes);
            afficher_planning(lignes, nb_lignes, convertir_jour_date(0));
        } else if (choix == 4) {
            LigneFabrication lignes[MAX_LIGNES];
            creer_planning_optimise(lutins, nb_lutins, lignes, &nb_lignes, jour);
            afficher_planning(lignes, nb_lignes, convertir_jour_date(jour));
        } else if (choix == 5) {
            creer_planning_semaine(lutins, nb_lutins, semaines, &nb_semaines);

            do {
                printf("\n--- Semaine %d ---\n", semaine_courante + 1);
                afficher_semaine(semaines, semaine_courante);

                printf("\nQue souhaitez-vous faire ?\n");
                printf("n : Semaine suivante\n");
                printf("p : Semaine précédente\n");
                printf("q : Quitter la navigation\n");
                printf("Votre choix : ");

                if (scanf(" %c", &choix_semaine) != 1) {
                    printf("Erreur de saisie !\n");
                    break;
                }

                if (choix_semaine == 'n') {
                    if (semaine_courante < nb_semaines - 1) {
                        semaine_courante++;
                    } else {
                        printf("Vous êtes déjà à la dernière semaine.\n");
                    }
                } else if (choix_semaine == 'p') {
                    if (semaine_courante > 0) {
                        semaine_courante--;
                    } else {
                        printf("Vous êtes déjà à la première semaine.\n");
                    }
                }
            } while (choix_semaine != 'q');
        } else if (choix == 6) {
            free(lutins);
            exit(EXIT_SUCCESS);
        } else {
            printf("Choix invalide !\n");
            free(lutins);
            exit(EXIT_FAILURE);
        }

        printf("\nSouhaitez-vous :\n");
        printf(" 1 : Sauvegarder le planning (.txt)\n");
        printf(" 2 : Quitter\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix2) != 1) {
            printf("Erreur de saisie !\n");
            free(lutins);
            return 1;
        }

        if (choix2 == 1) {
            sauvegarder_semaine_ics(semaines[semaine_courante], semaine_courante);
        }

        free(lutins);
    }

    else {
        printf("Mode invalide !\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
