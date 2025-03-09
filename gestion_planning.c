#include "../include/gestion_planning.h"
#include "../include/gestion_lutins.h"
#include <stdio.h>

void generer_planning(Lutin *lutins, int nb_lutins, LigneFabrication *lignes, int nb_lignes, int jour) {
    int i, heure, j, k;
    for (i = 0; i < nb_lignes; i++) {
        lignes[i].bricoleur = NULL;
        lignes[i].controleur = NULL;
        lignes[i].empaqueteur = NULL;
    }

    for (heure = 0; heure < 24; heure++) {
        for (i = 0; i < nb_lignes; i++) {
            Lutin *bricoleur = NULL, *controleur = NULL, *empaqueteur = NULL;

            for (j = 0; j < nb_lutins; j++) {
                Lutin *l = &lutins[j];

                if (l->statut == 1) {
                    for (k = 0; k < l->nb_plages[jour]; k++) {
                        if (heure >= l->horaires[jour][k].debut && heure < l->horaires[jour][k].fin) {
                            if (l->role == 1 && !bricoleur) bricoleur = l;
                            else if (l->role == 2 && !controleur) controleur = l;
                            else if (l->role == 3 && !empaqueteur) empaqueteur = l;
                        }
                    }
                }
            }

            if (bricoleur!=NULL) lignes[i].bricoleur = bricoleur;
            if (controleur!=NULL) lignes[i].controleur = controleur;
            if (empaqueteur!=NULL) lignes[i].empaqueteur = empaqueteur;
        }
    }
}

void afficher_planning(LigneFabrication *lignes, int nb_lignes, int jour) {
    int heure, role, lutin_trouve, ligne_num, k, debut, fin;
    const char *jours[] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};

    printf("\nPLANNING DU %s\n", jours[jour]);

    for (ligne_num = 0; ligne_num < nb_lignes; ligne_num++) {
        printf("\nPLANNING DE LA LIGNE %d\n", ligne_num + 1);
        printf("+------------------------------------------------------+\n");
        printf("| Heure | Bricoleur       | Contrôleur       | Empaqueteur       |\n");
        printf("+------------------------------------------------------+\n");

        for (heure = 0; heure < 24; heure++) {
            printf("|  %dh  ", heure);

            for (role = 1; role <= 3; role++) {
                lutin_trouve = 0;
                Lutin *lutin = NULL;

                if (role == 1){
                 lutin = lignes[ligne_num].bricoleur;
                 }
                if (role == 2){
                 lutin = lignes[ligne_num].controleur;
                 }
                if (role == 3){ 
                lutin = lignes[ligne_num].empaqueteur;
                }

                if (lutin != NULL) {
                    for (k = 0; k < lutin->nb_plages[jour]; k++) {
                        debut = lutin->horaires[jour][k].debut;
                        fin = lutin->horaires[jour][k].fin;

                        if (heure >= debut && heure < fin) {
                            printf("| %s ", lutin->prenom);
                            lutin_trouve = 1;
                        }

                        if (debut > fin) {
                            if (heure >= debut || heure < fin) {
                                printf("| %s ", lutin->prenom);
                                lutin_trouve = 1;
                            }
                        }
                    }
                }

                if (lutin_trouve==0) {
                    printf("| Personne       ");
                }
            }
            printf("|\n");
        }

        printf("+------------------------------------------------------+\n");
    }
}

void gerer_planning_manuel(Lutin *lutins, int nb_lutins, LigneFabrication *lignes, int nb_lignes, int jour, int debut, int fin) {
    int lutin_utilise[nb_lutins]; 
    int i, id_bricoleur, id_controleur, id_empaqueteur;
    for (i = 0; i < nb_lutins; i++) lutin_utilise[i] = 0; 

    for (i = 0; i < nb_lignes; i++) {
        printf("\nConfiguration de la ligne %d\n", i + 1);

        /* Choix du bricoleur*/
        if (!afficher_lutins_disponibles(lutins, nb_lutins, 1, lutin_utilise, jour, debut, fin)) {
            printf("\nERREUR : Plus de bricoleur disponible ! Annulation du planning.\n");
            return;
        }
        do {
            printf("\nChoisissez un bricoleur (ID) : ");
            if (scanf("%d", &id_bricoleur)==1){
                printf("OK");
            };
        } while (id_bricoleur < 0 || id_bricoleur >= nb_lutins || lutin_utilise[id_bricoleur] || lutins[id_bricoleur].role != 1);
        lignes[i].bricoleur = &lutins[id_bricoleur];
        lutin_utilise[id_bricoleur] = 1;

        /*Choix du contrôleur*/
        if (!afficher_lutins_disponibles(lutins, nb_lutins, 2, lutin_utilise, jour, debut, fin)) {
            printf("\nERREUR : Plus de contrôleur disponible ! Annulation du planning.\n");
            return;
        }
        do {
            printf("\nChoisissez un contrôleur (ID) : ");
            if (scanf("%d", &id_controleur)==1){
                printf("OK");
            };
        } while (id_controleur < 0 || id_controleur >= nb_lutins || lutin_utilise[id_controleur] || lutins[id_controleur].role != 2);
        lignes[i].controleur = &lutins[id_controleur];
        lutin_utilise[id_controleur] = 1;

        /*Choix de l'empaqueteur*/
        if (!afficher_lutins_disponibles(lutins, nb_lutins, 3, lutin_utilise, jour, debut, fin)) {
            printf("\nERREUR : Plus d'empaqueteur disponible ! Annulation du planning.\n");
            return;
        }
        do {
            printf("\nChoisissez un empaqueteur (ID) : ");
            if (scanf("%d", &id_empaqueteur)==1){
                printf("OK");
            };
        } while (id_empaqueteur < 0 || id_empaqueteur >= nb_lutins || lutin_utilise[id_empaqueteur] || lutins[id_empaqueteur].role != 3);
        lignes[i].empaqueteur = &lutins[id_empaqueteur];
        lutin_utilise[id_empaqueteur] = 1;

        printf("\nLigne %d configurée !\n", i + 1);
    }
}


