#include "../include/gestion_lutins.h"
#include "../include/gestion_planning.h"
#include "../include/gestion_fichiers.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int i, choix, choix2;
    int nb_lutins = 9;
    int nb_lignes = 3;
    int jour = 1;  /* Lundi */
    int debut = 0, fin = 23;  /* Créneau horaire à tester */
    srand(time(NULL));

    /* Allocation et génération des lutins */
    Lutin *lutins = generer_lutin(nb_lutins);
    LigneFabrication lignes[nb_lignes];

    if (lutins==NULL) {
        printf("Erreur d'allocation mémoire des lutins.\n");
        return 1;
    }

    /* Affichage des lutins générés */
    printf("\nListe des lutins générés :\n");
    for (i = 0; i < nb_lutins; i++) {
        afficher_lutin(lutins[i]);
        printf("\n");
    }

    /* Choix du mode de génération */
    printf("\nSouhaitez-vous :\n 1: Générer automatiquement\n 2: Remplir manuellement\n 3: Charger un planning\nVotre choix : ");
    if (scanf("%d", &choix) != 1) {
        printf("Erreur de saisie !\n");
        free(lutins);
        exit(EXIT_FAILURE);
    }

    if (choix == 1) {
        generer_planning(lutins, nb_lutins, lignes, nb_lignes, jour);
    } else if (choix == 2) {
        gerer_planning_manuel(lutins, nb_lutins, lignes, nb_lignes, jour, debut, fin);
    } else if (choix == 3) {
        charger_planning(lutins, &nb_lutins, lignes, &nb_lignes);
    } else {
        printf("Choix invalide !\n");
        free(lutins);
        exit(EXIT_FAILURE);
    }

    afficher_planning(lignes, nb_lignes, jour);

    /* Demande de sauvegarde */
    printf("\nSouhaitez-vous :\n 1 : Sauvegarder le planning\n 2: Quitter\nVotre choix : ");
    if (scanf("%d", &choix2) != 1) {
        printf("Erreur de saisie !\n");
        free(lutins);
        return 1;
    }

    if (choix2 == 1) {
        sauvegarder_planning(lignes, nb_lignes, jour);
    }

    /* Libération de la mémoire */
    free(lutins);
    
    exit(EXIT_SUCCESS);
}

