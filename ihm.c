#include "../include/ihm.h"


#include <MLV/MLV_all.h>

void creer_fenetre() {

    int hauteur = 1000, largeur = 1600;
    


    MLV_create_window("Gestionnaire de Plannings", "Gestionnaire de Plannings", largeur, hauteur);
    
   
}



int menu() {
    MLV_Image *image;
    int hauteur = 1000, largeur = 1600;
    int boutonlargeur = 600;
    int boutonhauteur = 140;
    int x, y;

    image = MLV_load_image("./ressources/Menu.jpeg");
    if (!image) {
        fprintf(stderr, "Erreur lors du chargement de l'image.\n");
        exit(EXIT_FAILURE);
    }

    MLV_resize_image_with_proportions(image, largeur, hauteur);
    MLV_draw_image(image, 0, 0);
    MLV_actualise_window();

    while (1) {
        MLV_wait_mouse(&x, &y);

        /* Bouton "Lutins" */
        int bx_lutins = 520, by_lutins = 270;
        /* Bouton "Quitter" */
        int bx_quitter = 520, by_quitter = 760;

        if (x >= bx_lutins && x <= bx_lutins + boutonlargeur &&
            y >= by_lutins && y <= by_lutins + boutonhauteur) {
            return 1;
        }

        if (x >= bx_quitter && x <= bx_quitter + boutonlargeur &&
            y >= by_quitter && y <= by_quitter + boutonhauteur) {
            MLV_free_window();
            exit(EXIT_SUCCESS);
        }
    }
}

void afficher_lutins_graph(Lutin *lutins, int nb_lutins) {
    int courant = 0;
    int x, y;
    MLV_Image *background = MLV_load_image("./ressources/AfficherLutins.jpeg");

    if (!background) {
        fprintf(stderr, "Erreur chargement image fond.\n");
        return;
    }

    while (1) {
        MLV_clear_window(MLV_COLOR_BLACK);
        MLV_draw_image(background, 0, 0);

        Lutin l = lutins[courant];

        MLV_draw_text(600, 250, "Nom : %s", MLV_COLOR_BLACK, l.nom);
        MLV_draw_text(600, 290, "Prénom : %s", MLV_COLOR_BLACK, l.prenom);
        MLV_draw_text(600, 330, "Rôle : %s", MLV_COLOR_BLACK,
            l.role == 1 ? "Bricoleur" : (l.role == 2 ? "Contrôleur" : "Empaqueteur"));
        MLV_draw_text(600, 370, "Statut : %s", MLV_COLOR_BLACK,
            l.statut ? "Actif" : "Inactif");

        int j;
        for (j = 0; j < 7; j++) {
            char jour[10];
            switch (j) {
                case 0: sprintf(jour, "Lundi"); break;
                case 1: sprintf(jour, "Mardi"); break;
                case 2: sprintf(jour, "Mercredi"); break;
                case 3: sprintf(jour, "Jeudi"); break;
                case 4: sprintf(jour, "Vendredi"); break;
                case 5: sprintf(jour, "Samedi"); break;
                case 6: sprintf(jour, "Dimanche"); break;
            }

            if (l.nb_plages[j] > 0) {
                MLV_draw_text(600, 420 + j * 30, "%s : %02dh - %02dh", MLV_COLOR_BLACK,
                    jour, l.horaires[j][0].debut, l.horaires[j][0].fin);
            } else {
                MLV_draw_text(600, 420 + j * 30, "%s : Repos", MLV_COLOR_BLACK, jour);
            }
        }

        MLV_actualise_window();

        MLV_wait_mouse(&x, &y);

        if (x >= 185 && x <= 300 && y >= 450 && y <= 560) { // flèche gauche
            if (courant > 0) courant--;
        } else if (x >= 1300 && x <= 1415 && y >= 450 && y <= 560) { // flèche droite
            if (courant < nb_lutins - 1) courant++;
        } else if (x >= 1530 && x <= 1590 && y >= 940 && y <= 990) { // bouton maison
            break;
        }
    }
    MLV_free_image(background);
}
