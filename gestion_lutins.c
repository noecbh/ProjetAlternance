#include "../include/gestion_lutins.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void afficher_lutin(Lutin l){
    char *role_str;
    int i,j;
    const char* noms_jours[] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};

    switch (l.role){
    case 1:
        role_str="Bricoleur";
        break;
    case 2:
        role_str="Contrôleur";
        break;
    case 3:
        role_str="Empaqueteur";
        break;
    default:
        role_str="Erreur, rôle inconnu";
    }

    printf("Nom: %s\n", l.nom);
    printf("Prénom: %s\n", l.prenom);
    printf("Rôle: %s\n", role_str);


    printf("Horaire de travail :\n");
    for (i = 0; i < 7; i++) {
        if (l.nb_plages[i] > 0) {
            /* Affichage du nom du jour (Dimanche, Lundi, etc.) */
            printf("%s : ", noms_jours[i]);
            for (j = 0; j < l.nb_plages[i]; j++) {  /* Parcours des plages horaires pour ce jour */
                printf("[%d - %d] ", l.horaires[i][j].debut, l.horaires[i][j].fin);
            }
            printf("\n");
        }
    }

    switch(l.statut){
    case 0:
        printf("Statut : Indisponible \n");
        break;
    case 1:
        printf("Statut : Disponible \n");
    }

}

Lutin* generer_lutin(int n) {
    int i, j, shift, debut, fin,nb_par_role;
    nb_par_role=0;
    int horaires_dispo[3][nb_par_role];
    

    nb_par_role = n / 3;
    if (n < 9) n = 9; /* Minimum 3 lutins par rôle*/

    Lutin *lutins = (Lutin*)malloc(n * sizeof(Lutin));
    if (lutins==NULL) {
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }

    const char *noms[] = {"Ronaldo", "Noel", "Raccoon", "Mao", "Lacisaille", "Coco", "Alatele", "Ammar", "Sheva"};
    const char *prenoms[] = {"Cristiano", "Leo", "Jamel", "Eric", "Anthony", "Oussamma", "Bradley", "Désiré", "Franck"};


    

    for (i = 0; i < 3; i++) {
        for (j = 0; j < nb_par_role; j++) {
            horaires_dispo[i][j] = 0;
        }
    }

    for (i = 0; i < n; i++) {
        snprintf(lutins[i].nom, 50, "%s", noms[i % 9]);
        snprintf(lutins[i].prenom, 50, "%s", prenoms[i % 9]);
        lutins[i].role = (i % 3) + 1;

        do {
            shift = rand() % 3;  /* 0 = 00h-8h, 1 = 8h-16h, 2 = 16h-00h*/
        } while (horaires_dispo[shift][lutins[i].role - 1] >= nb_par_role / 3);

        switch (shift) {
            case 0: debut = 0; fin = 8; break;
            case 1: debut = 8; fin = 16; break;
            case 2: debut = 16; fin = 24; break;
        }

        lutins[i].nb_plages[1] = 1;
        lutins[i].horaires[1][0].debut = debut;
        lutins[i].horaires[1][0].fin = fin;

        horaires_dispo[shift][lutins[i].role - 1]++;
        lutins[i].statut = 1;
    }

    return lutins;
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




