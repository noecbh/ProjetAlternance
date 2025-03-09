#include "../include/gestion_fichiers.h"
#include "../include/gestion_lutins.h"
#include "../include/gestion_planning.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sauvegarder_planning(LigneFabrication *lignes, int nb_lignes, int jour) {
    FILE *fichier;
    char nom_fichier[100];
    int i;

    /* Demander à l'utilisateur le nom du fichier */
    printf("Entrez le nom du fichier pour sauvegarder le planning (ex: planning_noel.txt) : ");
    if (scanf("%s", nom_fichier)==0){
        printf("Erreur !\n");
    }

    fichier = fopen(nom_fichier, "w");
    if (fichier==NULL) {
        printf("Erreur lors de l'ouverture du fichier %s pour la sauvegarde !\n", nom_fichier);
        return;
    }

    /* Écriture du planning */
    fprintf(fichier, "PLANNING DU JOUR %d\n", jour);
    for (i = 0; i < nb_lignes; i++) {
        fprintf(fichier, "\nLigne %d:\n", i + 1);
        fprintf(fichier, "Bricoleur: %s %s\n", lignes[i].bricoleur->prenom, lignes[i].bricoleur->nom);
        fprintf(fichier, "Contrôleur: %s %s\n", lignes[i].controleur->prenom, lignes[i].controleur->nom);
        fprintf(fichier, "Empaqueteur: %s %s\n", lignes[i].empaqueteur->prenom, lignes[i].empaqueteur->nom);
    }
    
    fclose(fichier);
    printf("\nPlanning sauvegardé dans '%s' !\n", nom_fichier);
}

void charger_planning(Lutin *lutins, int *nb_lutins, LigneFabrication *lignes, int *nb_lignes) {
    FILE *fichier;
    char nom_fichier[100];
    char prenom[50], nom[50], role[20];
    int debut, fin, ligne_num, id_lutin;

    printf("Entrez le nom du fichier à charger (ex: planning_noel.txt) : ");
    if (scanf("%s", nom_fichier) == 0) {
        printf("Erreur\n");
    }

    fichier = fopen(nom_fichier, "r");
    if (fichier==NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
        return;
    }


    while (fscanf(fichier, "%s %s %s %d-%d %d", prenom, nom, role, &debut, &fin, &ligne_num) == 6) {
        strcpy(lutins[*nb_lutins].prenom, prenom);
        strcpy(lutins[*nb_lutins].nom, nom);
        lutins[*nb_lutins].statut = 1;
        lutins[*nb_lutins].nb_plages[1] = 1;
        lutins[*nb_lutins].horaires[1][0].debut = debut;
        lutins[*nb_lutins].horaires[1][0].fin = fin;

        if (strcmp(role, "Bricoleur") == 0) {
            lutins[*nb_lutins].role = 1;
        } else if (strcmp(role, "Contrôleur") == 0) {
            lutins[*nb_lutins].role = 2;
        } else if (strcmp(role, "Empaqueteur") == 0) {
            lutins[*nb_lutins].role = 3;
        }

        printf("Chargé: %s %s (%s) %dh-%dh, Ligne %d\n", prenom, nom, role, debut, fin, ligne_num);

        if (ligne_num > *nb_lignes) {
            *nb_lignes = ligne_num;
        }

        id_lutin = *nb_lutins;

        if (lutins[id_lutin].role == 1) {
            lignes[ligne_num - 1].bricoleur = &lutins[id_lutin];
        } else if (lutins[id_lutin].role == 2) {
            lignes[ligne_num - 1].controleur = &lutins[id_lutin];
        } else if (lutins[id_lutin].role == 3) {
            lignes[ligne_num - 1].empaqueteur = &lutins[id_lutin];
        }

        (*nb_lutins)++;
    }

    fclose(fichier);
    printf("\nPlanning chargé depuis '%s' !\n", nom_fichier);
}


