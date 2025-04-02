#include "../include/lutins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


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
        else {
            printf("%s : ", noms_jours[i]);
            printf("Repos \n");
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
    int i, j, temp, shift, debut, fin, index_nom, index_prenom, jour_repos1, jour_repos2;
    int indices_noms[12], indices_prenoms[12];

    debut = 0;
    fin = 0;


    Lutin *lutins = (Lutin*)malloc(n * sizeof(Lutin));
    if (!lutins) {
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }

    const char *noms[] = {
        "Ronaldo", "Noel", "Raccoon", "Mao", "Lacisaille", "Coco", 
        "Alatele", "Ammar", "Sheva", "Dupont", "Martin", "Durand"
    };

    const char *prenoms[] = {
        "Cristiano", "Leo", "Jamel", "Eric", "Anthony", "Oussamma", 
        "Bradley", "Désiré", "Franck", "Nathan", "Thomas", "Alexandre"
    };

    for (i = 0; i < 12; i++) {
        indices_noms[i] = i;
        indices_prenoms[i] = i;
    }

    for (i = 11; i > 0; i--) {
        j = rand() % (i + 1);
        temp = indices_noms[i];
        indices_noms[i] = indices_noms[j];
        indices_noms[j] = temp;
    }

    for (i = 11; i > 0; i--) {
        j = rand() % (i + 1);
        temp = indices_prenoms[i];
        indices_prenoms[i] = indices_prenoms[j];
        indices_prenoms[j] = temp;
    }

    for (i = 0; i < n; i++) {
        index_nom = indices_noms[rand() % 12];
        snprintf(lutins[i].nom, 50, "%s", noms[index_nom]);

        index_prenom = indices_prenoms[i];
        snprintf(lutins[i].prenom, 50, "%s", prenoms[index_prenom]);

        lutins[i].role = (rand() % 3) + 1;
        lutins[i].statut = 1;

        /* Sélectionne deux jours de repos aléatoires*/
        jour_repos1 = rand() % 7;
        do {
            jour_repos2 = rand() % 7;
        } while (jour_repos2 == jour_repos1);

        for (j = 0; j < 7; j++) {
            if (j == jour_repos1 || j == jour_repos2) {
                lutins[i].nb_plages[j] = 0;  /* Jour de repos */
            } else {
                shift = rand() % 3;  /* Choix aléatoire de tranche */
                switch (shift) {
                    case 0: debut = 0; fin = 8; break;
                    case 1: debut = 8; fin = 16; break;
                    case 2: debut = 16; fin = 24; break;
                }

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
    int i;
    if (l->statut == 0){

     return 0;
}

    for (i = 0; i < l->nb_plages[jour]; i++) {
        if (l->horaires[jour][i].debut < fin && l->horaires[jour][i].fin > debut) {
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
