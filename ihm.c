#include "../include/ihm.h"
#include "../include/lutins.h"

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
        /* Bouton "Planning" */
        int bx_planning = 520, by_planning = 435;

        if (x >= bx_lutins && x <= bx_lutins + boutonlargeur &&
            y >= by_lutins && y <= by_lutins + boutonhauteur) {
            return 1;
        }

        if (x >= bx_planning && x <= bx_planning + boutonlargeur &&
            y >= by_planning && y <= by_planning + boutonhauteur) {
            return 2;
        }

        if (x >= bx_planning && x <= bx_planning + boutonlargeur &&
            y >= by_planning+boutonhauteur && y <= by_planning + (2*boutonhauteur)) {
            return 3;
        }

        if (x >= bx_quitter && x <= bx_quitter + boutonlargeur &&
            y >= by_quitter && y <= by_quitter + boutonhauteur) {
            MLV_free_window();
            exit(EXIT_SUCCESS);
        }
    }
}

void afficher_lutins_graph(Lutin *lutins, int nb_lutins, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES]) {
    int courant = 0;
    int x, y;
    int filtre_role = 0; /* 0 = tous, 1 = bricoleur, 2 = contrôleur, 3 = empaqueteur */

    MLV_Image *background = MLV_load_image("./ressources/AfficherLutins.jpeg");

    if (!background) {
        fprintf(stderr, "Erreur chargement image fond.\n");
        return;
    }

    while (1) {
        MLV_clear_window(MLV_COLOR_BLACK);
        MLV_draw_image(background, 0, 0);

        int count = 0;
        for (int i = 0; i < nb_lutins; i++) {
            if (filtre_role == 0 || lutins[i].role == filtre_role) {
                if (count == courant) {
                    Lutin l = lutins[i];

                    MLV_draw_text(600, 250, "Nom : %s", MLV_COLOR_BLACK, l.nom);
                    MLV_draw_text(600, 290, "Prénom : %s", MLV_COLOR_BLACK, l.prenom);
                    MLV_draw_text(600, 330, "Rôle : %s", MLV_COLOR_BLACK,
                        l.role == 1 ? "Bricoleur" : (l.role == 2 ? "Contrôleur" : "Empaqueteur"));
                    MLV_draw_text(600, 370, "Statut : %s", MLV_COLOR_BLACK,
                        l.statut ? "Actif" : "Inactif");

                    const char* jours[] = {
                        "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"
                    };

                    for (int j = 0; j < 7; j++) {
                        if (l.nb_plages[j] > 0) {
                            MLV_draw_text(600, 420 + j * 30, "%s : %02dh - %02dh", MLV_COLOR_BLACK,
                                jours[j], l.horaires[j][0].debut, l.horaires[j][0].fin);
                        } else {
                            MLV_draw_text(600, 420 + j * 30, "%s : Repos", MLV_COLOR_BLACK, jours[j]);
                        }
                    }
                    break;
                }
                count++;
            }
        }

        MLV_actualise_window();
        MLV_wait_mouse(&x, &y);

        if (x >= 185 && x <= 300 && y >= 450 && y <= 560) { /* flèche gauche */
            if (courant > 0) courant--;
        } else if (x >= 1300 && x <= 1415 && y >= 450 && y <= 560) { /* flèche droite */
            int total = 0;
            for (int i = 0; i < nb_lutins; i++)
                if (filtre_role == 0 || lutins[i].role == filtre_role)
                    total++;
            if (courant < total - 1) courant++;
        } else if (x >= 1530 && x <= 1590 && y >= 940 && y <= 990) { /* bouton maison */
            break;
        } else if (x >= 1295 && x <= 1350 && y >= 675 && y <= 730) { /* bouton rechercher (loupe) */
            rechercher_lutin_graphique(lutins, nb_lutins, semaines);
        } else if (x >= 1295 && x <= 1350 && y >= 735 && y <= 785) { /* bouton modifier */
            modifier_lutin_graphique(lutins);
        } else if (x >= 1295 && x <= 1350 && y >= 790 && y <= 850) { /* bouton supprimer */
            supprimer_lutin_graphique(lutins, &nb_lutins, courant);
            if (courant >= nb_lutins && courant > 0) courant--;
        } else if (x >= 1295 && x <= 1350 && y >= 855 && y <= 910) { /* bouton ajouter */
            ajouter_lutin_graphique(&lutins, &nb_lutins);
        } else if (x >= 250 && x <= 305 && y >= 670 && y <= 725) { /* bouton bricoleur */
            filtre_role = 1;
            courant = 0;
        } else if (x >= 250 && x <= 305 && y >= 735 && y <= 785) { /* bouton contrôleur */
            filtre_role = 2;
            courant = 0;
        } else if (x >= 250 && x <= 305 && y >= 790 && y <= 850) { /* bouton empaqueteur */
            filtre_role = 3;
            courant = 0;
        } else if (x >= 250 && x <= 305 && y >= 855 && y <= 910) { /* bouton reset (affiche tout) */
            filtre_role = 0;
            courant = 0;
        }
    }

    MLV_free_image(background);
}



void afficher_semaine_graphique(LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES], int num_semaine, struct tm date_base) {
    int jour = 0;
    int x, y;
    MLV_Mouse_button bouton;
    MLV_Button_state etat_bouton;
    MLV_Event event;

    while (1) {
        struct tm date_du_jour = date_base;
        date_du_jour.tm_mday = 7 + num_semaine * 7 + jour;
        mktime(&date_du_jour);

        afficher_planning_graphique_simple(semaines[num_semaine][jour], MAX_LIGNES, date_du_jour);

        // Attente d'un événement
        do {
            event = MLV_get_event(
                NULL, NULL, NULL,     // clavier
                NULL, NULL,           // texte
                &x, &y,               // position souris
                &bouton, &etat_bouton // clic souris
            );

        } while (event == MLV_NONE);

        // Clavier
        if (event == MLV_KEY) {
            if (MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT) == MLV_PRESSED) {
                if (jour < 6) jour++;
                else if (num_semaine < MAX_SEMAINES - 1) {
                    num_semaine++;
                    jour = 0;
                }
            } else if (MLV_get_keyboard_state(MLV_KEYBOARD_LEFT) == MLV_PRESSED) {
                if (jour > 0) jour--;
                else if (num_semaine > 0) {
                    num_semaine--;
                    jour = 6;
                }
            } else if (MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED) {
                break;
            }
        }

        else if (event == MLV_MOUSE_BUTTON && etat_bouton == MLV_RELEASED) {
            if (y >= 490 && y <= 580) {
                if (x >= 40 && x <= 130) {  // flèche gauche
                    if (jour > 0) jour--;
                    else if (num_semaine > 0) {
                        num_semaine--;
                        jour = 6;
                    }
                } else if (x >= 1475 && x <= 1565) {  // flèche droite
                    if (jour < 6) jour++;
                    else if (num_semaine < MAX_SEMAINES - 1) {
                        num_semaine++;
                        jour = 0;
                    }
                }
            } else if (x >= 1530 && x <= 1590 && y >= 935 && y <= 1000) {  // bouton "home"
                break;
            }
        }
    }
}


void afficher_planning_graphique_simple(LigneFabrication *lignes, int nb_lignes, struct tm date) {
    int i, h, seg;
    char nom_jour[20], nom_mois[20];
    int trouve;
    int lignes_utilisees = 0;

    nom_jour[0] = '\0';
    nom_mois[0] = '\0';

    strftime(nom_jour, sizeof(nom_jour), "%A", &date);
    strftime(nom_mois, sizeof(nom_mois), "%B", &date);

    MLV_clear_window(MLV_COLOR_WHITE);
    MLV_Image* fond = MLV_load_image("./ressources/AffichagePlanning.jpeg");
    if (fond) {
        MLV_resize_image_with_proportions(fond, 1600, 1000);
        MLV_draw_image(fond, 0, 0);
        MLV_free_image(fond);
    }

    char titre[128];
    snprintf(titre, sizeof(titre), "PLANNING DU %s %02d %s %d", nom_jour, date.tm_mday, nom_mois, 1900 + date.tm_year);
    MLV_draw_text(690, 165, titre, MLV_COLOR_WHITE);

    int base_y = 200;
    int base_x = 150;
    int decalage_x = 400; /* Décalage horizontal entre deux lignes */

    for (i = 0; i < nb_lignes; i++) {
        if (lignes[i].nb_segments == 0) continue;
        lignes_utilisees++;

        int offset_x = base_x + i * decalage_x;

        for (h = 0; h < 24; h++) {
            int y = base_y + h * 30;
            if (i == 0) {  /* Affiche l’heure une seule fois à gauche */
                char heure[8];
                snprintf(heure, sizeof(heure), "%02dh", h);
                MLV_draw_text(60, y, heure, MLV_COLOR_BLACK);
            }

            trouve = 0;

            for (seg = 0; seg < lignes[i].nb_segments; seg++) {
                if (h >= lignes[i].segments[seg].debut && h < lignes[i].segments[seg].fin) {
                    SegmentPlanning *s = &lignes[i].segments[seg];

                    char prenom_bric[64], prenom_ctrl[64], prenom_empq[64];

                    snprintf(prenom_bric, sizeof(prenom_bric), "%s", s->bricoleur && s->bricoleur->prenom ? s->bricoleur->prenom : "Personne");
                    snprintf(prenom_ctrl, sizeof(prenom_ctrl), "%s", s->controleur && s->controleur->prenom ? s->controleur->prenom : "Personne");
                    snprintf(prenom_empq, sizeof(prenom_empq), "%s", s->empaqueteur && s->empaqueteur->prenom ? s->empaqueteur->prenom : "Personne");

                    MLV_draw_text(offset_x,     y, prenom_bric, MLV_COLOR_BLACK);
                    MLV_draw_text(offset_x + 130, y, prenom_ctrl, MLV_COLOR_BLACK);
                    MLV_draw_text(offset_x + 260, y, prenom_empq, MLV_COLOR_BLACK);

                    trouve = 1;
                    break;
                }
            }

            if (!trouve) {
                MLV_draw_text(offset_x,     y, "Personne", MLV_COLOR_GRAY);
                MLV_draw_text(offset_x + 130, y, "Personne", MLV_COLOR_GRAY);
                MLV_draw_text(offset_x + 260, y, "Personne", MLV_COLOR_GRAY);
            }
        }
    }

    if (lignes_utilisees == 0) {
        MLV_draw_text(600, 500, "Planning vide", MLV_COLOR_RED);
    }

    MLV_actualise_window();
}



void charger_planning_graphique(LigneFabrication semaine[7][MAX_LIGNES], Lutin *lutins, int nb_lutins, int semaine_courante) {
    char *nom_fichier = NULL;
    (void)semaine_courante;

    MLV_wait_input_box(
        500, 400, 400, 100,
        MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_COLOR_BLACK,
        "Nom du fichier .ics à charger : ",
        &nom_fichier
    );

    if (!nom_fichier) {
        fprintf(stderr, "Aucun nom de fichier fourni.\n");
        return;
    }

    charger_planning_ics(semaine, lutins, nb_lutins, nom_fichier);
    free(nom_fichier);

    struct tm date_base;
    date_base.tm_year = 2025 - 1900;
    date_base.tm_mon = 3;
    date_base.tm_mday = 7;
    date_base.tm_hour = 0;
    date_base.tm_min = 0;
    date_base.tm_sec = 0;
    date_base.tm_isdst = -1;
    mktime(&date_base);

    int jour_courant = 0;
    while (1) {
        afficher_planning_graphique_simple(semaine[jour_courant], MAX_LIGNES, date_base);

        MLV_Keyboard_button touche;
        MLV_wait_keyboard(&touche, NULL, NULL);

        if (touche == MLV_KEYBOARD_LEFT && jour_courant > 0) {
            jour_courant--;
        }
        else if (touche == MLV_KEYBOARD_RIGHT && jour_courant < 6) {
            jour_courant++;
        }
        else if (touche == MLV_KEYBOARD_ESCAPE) {
            break;
        }
        date_base.tm_mday = 7 + jour_courant;
        mktime(&date_base);
    }
}

void afficher_lutin_graphique_unique(Lutin *lutins, int nb_lutins, int index, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES]) {
    int x, y;
    MLV_Image *background = MLV_load_image("./ressources/AfficherLutins.jpeg");

    if (!background) {
        fprintf(stderr, "Erreur chargement image fond.\n");
        return;
    }

    while (1) {
        MLV_clear_window(MLV_COLOR_BLACK);
        MLV_draw_image(background, 0, 0);

        if (nb_lutins <= 0) {
            MLV_draw_text(700, 500, "Aucun lutin à afficher.", MLV_COLOR_RED);
            MLV_actualise_window();
            MLV_wait_mouse(&x, &y);
            if (x >= 1530 && x <= 1590 && y >= 940 && y <= 990) {
                break;
            }
            continue;
        }

        Lutin l = lutins[index];

        MLV_draw_text(600, 250, "Nom : %s", MLV_COLOR_BLACK, l.nom);
        MLV_draw_text(600, 290, "Prénom : %s", MLV_COLOR_BLACK, l.prenom);
        MLV_draw_text(600, 330, "Rôle : %s", MLV_COLOR_BLACK,
                      l.role == 1 ? "Bricoleur" : (l.role == 2 ? "Contrôleur" : "Empaqueteur"));
        MLV_draw_text(600, 370, "Statut : %s", MLV_COLOR_BLACK,
                      l.statut ? "Actif" : "Inactif");

        const char *jours[] = {
            "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"
        };

        for (int j = 0; j < 7; j++) {
            if (l.nb_plages[j] > 0) {
                MLV_draw_text(600, 420 + j * 30, "%s : %02dh - %02dh", MLV_COLOR_BLACK,
                              jours[j], l.horaires[j][0].debut, l.horaires[j][0].fin);
            } else {
                MLV_draw_text(600, 420 + j * 30, "%s : Repos", MLV_COLOR_BLACK, jours[j]);
            }
        }

        MLV_actualise_window();
        MLV_wait_mouse(&x, &y);

        if (x >= 1530 && x <= 1590 && y >= 940 && y <= 990) { /* bouton maison */
            break;
        } else if (x >= 1295 && x <= 1350 && y >= 735 && y <= 790) { /* bouton modifier */
            modifier_lutin_graphique(&lutins[index]);
        } else if (x >= 1295 && x <= 1350 && y >= 795 && y <= 850) { /* bouton supprimer */
            supprimer_lutin_des_plannings(semaines, &lutins[index]);
            // Info : on ne fait PAS la suppression ici car `nb_lutins` n’est pas modifiable localement
            break;
        } else if (x >= 1295 && x <= 1350 && y >= 855 && y <= 910) { /* bouton ajouter */
            ajouter_lutin_graphique(&lutins, &nb_lutins);
        }
    }

    MLV_free_image(background);
}



void rechercher_lutin_graphique(Lutin *lutins, int nb_lutins, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES]) {
    char *nom = NULL;
    char *prenom = NULL;
    int i;

    int box_x = 700, box_y = 855, box_w = 400, box_h = 50;

    /* Saisie du NOM */
    MLV_draw_filled_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_WHITE);
    MLV_draw_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_WHITE);
    MLV_actualise_window();
    MLV_wait_input_box(box_x, box_y, box_w, box_h,
                       MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                       "Nom :", &nom);

    /* Saisie du PRÉNOM (au même endroit, efface le précédent) */
    MLV_draw_filled_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_WHITE);
    MLV_draw_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_WHITE);
    MLV_actualise_window();
    MLV_wait_input_box(box_x, box_y, box_w, box_h,
                       MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                       "Prénom :", &prenom);

    /* Recherche */
    for (i = 0; i < nb_lutins; i++) {
        if (strcmp(lutins[i].nom, nom) == 0 && strcmp(lutins[i].prenom, prenom) == 0) {
            free(nom); free(prenom);
            afficher_lutin_graphique_unique(lutins, nb_lutins, i, semaines);

            return;
        }
    }

    /* Affichage message erreur si non trouvé */
    MLV_draw_filled_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_WHITE);
    MLV_draw_rectangle(box_x, box_y , box_w, box_h, MLV_COLOR_WHITE);
    MLV_draw_text(box_x , box_y, "Lutin introuvable.", MLV_COLOR_RED);
    MLV_actualise_window();
    MLV_wait_seconds(2);

    free(nom); free(prenom);
}

void afficher_lutins_par_role_graphique(Lutin *lutins, int nb_lutins, int role_voulu, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES]) {
    int *index = malloc(sizeof(int) * nb_lutins);
    int nb_filtres = 0;
    int courant = 0;
    int x, y;
    int i;

    MLV_Image *background = MLV_load_image("./ressources/AfficherLutins.jpeg");
    if (!background) {
        fprintf(stderr, "Erreur chargement image fond.\n");
        return;
    }

    /* Récupération des indices des lutins du rôle voulu */
    for (i = 0; i < nb_lutins; i++) {
        if (lutins[i].role == role_voulu) {
            index[nb_filtres++] = i;
        }
    }

    if (nb_filtres == 0) {
        MLV_clear_window(MLV_COLOR_BLACK);
        MLV_draw_image(background, 0, 0);
        MLV_draw_text(700, 500, "Aucun lutin avec ce rôle.", MLV_COLOR_RED);
        MLV_actualise_window();
        MLV_wait_seconds(2);
        MLV_free_image(background);
        free(index);
        return;
    }

    while (1) {
        MLV_clear_window(MLV_COLOR_BLACK);
        MLV_draw_image(background, 0, 0);

        Lutin l = lutins[index[courant]];

        MLV_draw_text(600, 250, "Nom : %s", MLV_COLOR_BLACK, l.nom);
        MLV_draw_text(600, 290, "Prénom : %s", MLV_COLOR_BLACK, l.prenom);
        MLV_draw_text(600, 330, "Rôle : %s", MLV_COLOR_BLACK,
                      l.role == 1 ? "Bricoleur" : (l.role == 2 ? "Contrôleur" : "Empaqueteur"));
        MLV_draw_text(600, 370, "Statut : %s", MLV_COLOR_BLACK,
                      l.statut ? "Actif" : "Inactif");

        const char *jours[] = {
            "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"
        };

        for (int j = 0; j < 7; j++) {
            if (l.nb_plages[j] > 0) {
                MLV_draw_text(600, 420 + j * 30, "%s : %02dh - %02dh", MLV_COLOR_BLACK,
                              jours[j], l.horaires[j][0].debut, l.horaires[j][0].fin);
            } else {
                MLV_draw_text(600, 420 + j * 30, "%s : Repos", MLV_COLOR_BLACK, jours[j]);
            }
        }

        MLV_actualise_window();
        MLV_wait_mouse(&x, &y);

        if (x >= 185 && x <= 300 && y >= 450 && y <= 560) {
            if (courant > 0) courant--;
        } else if (x >= 1300 && x <= 1415 && y >= 450 && y <= 560) {
            if (courant < nb_filtres - 1) courant++;
        } else if (x >= 1530 && x <= 1590 && y >= 940 && y <= 990) {
            break;
        }
    }

    MLV_free_image(background);
    free(index);
}
