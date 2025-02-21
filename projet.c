#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int debut;
    int fin;
} PlageHoraire;

typedef struct{
    char nom[50];
    char prenom[50];
    int role; /* 1 = bricoleur, 2 = contrôleur, 3 = empaqueteur) */
    PlageHoraire horaires[7][5];
    int nb_plages[7]; /* Nombre de plages horaires (ex : dispo de 10-12 et 14-16 = 2 plages horaires*/
    int statut; /* 1 = dispo, 0 = indispo */
} Lutin;

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
        lutin->nb_plages[i] = nb_plages[i];  /* Assigne le nombre de plages horaires pour chaque jour */
    }

    /* Initialisation du statut du lutin */
        lutin->statut = statut;              /* 1 = disponible, 0 = indisponible */
}

Lutin* generer_lutin(int n) {
    int i, j, k;
    
    const char *noms[] = {"Ronaldo", "Noel", "Raccoon", "Mao", "Lacisaille", "Coco", "Alatele", "Ammar", "Sheva", "Pessi"};
    const char *prenoms[] = {"Cristiano", "Leo", "Jamel", "Eric", "Anthony", "Oussamma", "Bradley", "Désiré", "Franck", "Toto"};

    Lutin *lutins = (Lutin*)malloc(n * sizeof(Lutin));
    if (lutins == NULL) {
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }
    srand(time(NULL));

    for (i = 0; i < n; i++) {
        /* Choisir un nom et un prénom aléatoire dans les listes */
        snprintf(lutins[i].nom, 50, "%s", noms[rand() % 10]);
        snprintf(lutins[i].prenom, 50, "%s", prenoms[rand() % 10]);

        /* Assigner un rôle aléatoire entre 1 et 3 */
        lutins[i].role = rand() % 3 + 1;

        /* Générer des plages horaires aléatoires pour chaque jour */
        for (j = 0; j < 7; j++) {
            lutins[i].nb_plages[j] = rand() % 3;  /* Générer un nombre de plages aléatoire (0 à 2) */
            for (k = 0; k < lutins[i].nb_plages[j]; k++) {
                /* Créer des horaires aléatoires pour chaque plage */
                lutins[i].horaires[j][k].debut = rand() % 24;  /* Heure de début entre 0 et 23 */
                lutins[i].horaires[j][k].fin = lutins[i].horaires[j][k].debut + (rand() % 4 + 1);  /* Durée de 1 à 4 heures */
            }
        }

        /* Assigner un statut aléatoire (1 = dispo, 0 = indispo) */
        lutins[i].statut = rand() % 2;
    }
    return lutins;
}


int main() {
    int i;
    
    /* Exemple de plages horaires (pour chaque jour de la semaine)
       Chaque jour peut avoir jusqu'à 5 plages horaires */
    PlageHoraire horaires_exemple[7][5] = {
        {{12, 14}, {18, 22}},  /* Dimanche (jour 0) */
        {{12, 14}, {18, 22}},  /* Lundi (jour 1) */
        {{12, 14}, {18, 22}},  /* Mardi (jour 2) */
        {{12, 14}, {18, 22}},  /* Mercredi (jour 3) */
        {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},  /* Jeudi (jour 4) */
        {{12, 14}},            /* Vendredi (jour 5) */
        {{12, 14}, {18, 22}}   /* Samedi (jour 6) */
    };

    /* Nombre de plages horaires pour chaque jour */
    int nb_plages[7] = {2, 2, 2, 2, 0, 1, 2};

    Lutin lutin1;

    initialiser_lutin(&lutin1, "Lutin", "Noel", 1, horaires_exemple, nb_plages, 1);

    afficher_lutin(lutin1);

    int n = 3;
    Lutin *lutins = generer_lutin(n);

    printf("\nAffichage des lutins générés:\n");
    for (i = 0; i < n; i++) {
        afficher_lutin(lutins[i]);
        printf("\n");
    }

    free(lutins);

    return 0;
}




