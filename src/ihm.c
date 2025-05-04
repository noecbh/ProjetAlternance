#include "../include/ihm.h"
#include "../include/lutins.h"
#include "../include/fichiers.h"

#include <MLV/MLV_all.h>
#define MAX_LUTINS 100

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
    int bx_lutins,by_lutins,bx_quitter,by_quitter,bx_planning,by_planning;

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
        bx_lutins = 520, by_lutins = 270;
        /* Bouton "Quitter" */
        bx_quitter = 520, by_quitter = 760;
        /* Bouton "Planning" */
        bx_planning = 520, by_planning = 435;

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

Lutin* afficher_lutins_graph(Lutin *lutins, int *nb_lutins, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES], int courant_depart) {
    int courant = courant_depart;
    int x, y,i,j,total;
    int filtre_role = 0;
    int count = 0;

    const char* jours[] = {
        "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"
    };

    MLV_Image *background = MLV_load_image("./ressources/AfficherLutins.jpeg");

    if (!background) {
        fprintf(stderr, "Erreur chargement image fond.\n");
        return lutins;
    }

    while (1) {
        MLV_clear_window(MLV_COLOR_BLACK);
        MLV_draw_image(background, 0, 0);
        count=0;

        for (i = 0; i < *nb_lutins; i++) {
            if (filtre_role == 0 || lutins[i].role == filtre_role) {
                if (count == courant) {
                    Lutin l = lutins[i];
                    MLV_draw_text(600, 250, "Nom : %s", MLV_COLOR_BLACK, l.nom);
                    MLV_draw_text(600, 290, "Prénom : %s", MLV_COLOR_BLACK, l.prenom);
                    MLV_draw_text(600, 330, "Rôle : %s", MLV_COLOR_BLACK,
                        l.role == 1 ? "Bricoleur" : (l.role == 2 ? "Contrôleur" : "Empaqueteur"));
                    MLV_draw_text(600, 370, "Statut : %s", MLV_COLOR_BLACK,
                        l.statut ? "Actif" : "Inactif");


                    for (j = 0; j < 7; j++) {
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

        if (x >= 185 && x <= 300 && y >= 450 && y <= 560) { /* flèche gauche*/
            if (courant > 0) courant--;
        } else if (x >= 1300 && x <= 1415 && y >= 450 && y <= 560) { /* flèche droite */
            total = 0;
            for (i = 0; i < *nb_lutins; i++)
                if (filtre_role == 0 || lutins[i].role == filtre_role)
                    total++;
            if (courant < total - 1) courant++;
        } else if (x >= 1530 && x <= 1590 && y >= 940 && y <= 990) { /* retour*/
            break;
        } else if (x >= 1295 && x <= 1350 && y >= 675 && y <= 730) { /*recherche*/
            rechercher_lutin_graphique(lutins, *nb_lutins, semaines);
            break;
        } else if (x >= 1295 && x <= 1350 && y >= 735 && y <= 785) { /*bouton modifier*/
            modifier_lutin_graphique(&lutins[courant]);
            
        } else if (x >= 1295 && x <= 1350 && y >= 790 && y <= 850) { /*bouton supprimer*/
            lutins = supprimer_lutin_graphique(lutins, nb_lutins, courant);
            if (courant >= *nb_lutins && courant > 0) courant--;
        } else if (x >= 1295 && x <= 1350 && y >= 855 && y <= 910) { /*bouton ajouter*/
            lutins = ajouter_lutin_graphique(lutins, nb_lutins);
           
        } else if (x >= 250 && x <= 305 && y >= 670 && y <= 725) { /*bricoleur*/
            filtre_role = 1;
            courant = 0;
        } else if (x >= 250 && x <= 305 && y >= 735 && y <= 785) { /*controleur*/
            filtre_role = 2;
            courant = 0;
        } else if (x >= 250 && x <= 305 && y >= 790 && y <= 850) { /*empaqueteur*/
            filtre_role = 3;
            courant = 0;
        } else if (x >= 250 && x <= 305 && y >= 855 && y <= 910) { /* reset*/
            filtre_role = 0;
            courant = 0;
        } else if (x >= 250 && x <= 305 && y >= 610 && y <= 665) { /*calendrier*/
            MLV_free_image(background);
            afficher_lutins_par_date(lutins, *nb_lutins);
            background = MLV_load_image("./ressources/AfficherLutins.jpeg");
            if (!background) {
                fprintf(stderr, "Erreur rechargement image fond après calendrier.\n");
                return lutins;
            }
        }
    }

    MLV_free_image(background);
    sauvegarder_lutins(lutins, *nb_lutins);
    return lutins;
}



void rechercher_lutin_graphique(Lutin *lutins, int nb_lutins, LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES]) {
    char *nom = NULL;
    char *prenom = NULL;
    int i;

    MLV_Image* fond = MLV_load_image("./ressources/RechercherLutin.jpeg");
    if (fond) {
        MLV_resize_image_with_proportions(fond, 1600, 1000);
        MLV_draw_image(fond, 0, 0);
        MLV_free_image(fond);
    }
    int box_x = 540, box_y = 520, box_w = 500, box_h = 50;

    /*NOM */
    MLV_draw_filled_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_WHITE);
    MLV_draw_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_WHITE);
    MLV_actualise_window();
    MLV_wait_input_box(box_x, box_y, box_w, box_h,
                       MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                       "Nom :", &nom);

    /*PRENOM */
    MLV_draw_filled_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_WHITE);
    MLV_draw_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_WHITE);
    MLV_actualise_window();
    MLV_wait_input_box(box_x, box_y, box_w, box_h,
                       MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                       "Prénom :", &prenom);


    for (i = 0; i < nb_lutins; i++) {
        if (strcmp(lutins[i].nom, nom) == 0 && strcmp(lutins[i].prenom, prenom) == 0) {
            free(nom);
            free(prenom);
            lutins = afficher_lutins_graph(lutins, &nb_lutins, semaines, i);
            return;
        }
    }

    MLV_draw_filled_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_WHITE);
    MLV_draw_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_WHITE);
    MLV_draw_text(box_x, box_y, "Lutin introuvable.", MLV_COLOR_RED);
    MLV_actualise_window();
    MLV_wait_seconds(2);

    free(nom);
    free(prenom);
}

void afficher_lutins_par_date(Lutin *lutins, int nb_lutins) {
    char *saisie = NULL;
    int filtre_horaire = 0; /* 0 = tous, 1 = 0-8, 2 = 8-16, 3 = 16-24 */

    MLV_Image *background = MLV_load_image("./ressources/AfficherLutinsBis.jpeg");
    MLV_Image *image = MLV_load_image("./ressources/SelectionnerDate.jpeg");

    if (!background) {
        fprintf(stderr, "Erreur chargement image AfficherLutinsBis.jpeg\n");
        return;
    }

    if (!image) {
        fprintf(stderr, "Erreur chargement image SelectionnerDate.jpeg\n");
        MLV_free_image(background);
        return;
    }

 
    MLV_draw_image(image, 0, 0);
    MLV_wait_input_box(
        540, 520, 500, 50,
        MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
        "Entrez la date (JJ/MM/AAAA) :", &saisie
    );

    if (!saisie) {
        fprintf(stderr, "Erreur de saisie.\n");
        MLV_free_image(image);
        MLV_free_image(background);
        return;
    }

    MLV_free_image(image);


    int jour, mois, annee;
    if (sscanf(saisie, "%d/%d/%d", &jour, &mois, &annee) != 3) {
        fprintf(stderr, "Format de date invalide.\n");
        free(saisie);
        MLV_free_image(background);
        return;
    }
    free(saisie);

    struct tm date_recherche = {0};
    date_recherche.tm_mday = jour;
    date_recherche.tm_mon = mois - 1;
    date_recherche.tm_year = annee - 1900;
    mktime(&date_recherche);

    int jour_semaine = date_recherche.tm_wday;
    if (jour_semaine == 0) jour_semaine = 7; /* On considère dimanche comme jour 7 */

    while (1) {
        MLV_clear_window(MLV_COLOR_BLACK);
        MLV_draw_image(background, 0, 0);

        int y_depart = 200;
        int trouve = 0;

        for (int i = 0; i < nb_lutins; i++) {
            Lutin l = lutins[i];

            if (l.nb_plages[jour_semaine - 1] > 0) {
                int debut = l.horaires[jour_semaine - 1][0].debut;
                int fin = l.horaires[jour_semaine - 1][0].fin;

                int afficher = 0;

  
                if (filtre_horaire == 0)
                    afficher = 1;
                else if (filtre_horaire == 1 && debut == 0 && fin == 8)
                    afficher = 1;
                else if (filtre_horaire == 2 && debut == 8 && fin == 16)
                    afficher = 1;
                else if (filtre_horaire == 3 && debut == 16 && fin == 24)
                    afficher = 1;

                if (afficher) {
                    trouve = 1;

                    const char *role =
                        (l.role == 1) ? "Bricoleur" :
                        (l.role == 2) ? "Contrôleur" : "Empaqueteur";

                    char buffer[256];
                    snprintf(buffer, sizeof(buffer), "%.20s %.20s [%02dh-%02dh] - %s",
                             l.prenom, l.nom, debut, fin, role);

                    MLV_draw_text(350, y_depart, buffer, MLV_COLOR_BLACK);
                    y_depart += 40;

                    if (y_depart > 900) break;
                }
            }
        }

        if (!trouve) {
            MLV_draw_text(600, 500, "Aucun lutin trouvé pour ce filtre.", MLV_COLOR_RED);
        }

        MLV_actualise_window();


        int x, y_click;
        MLV_Mouse_button bouton;
        MLV_Button_state etat_bouton;
        MLV_Event event;

        do {
            event = MLV_get_event(NULL, NULL, NULL, NULL, NULL, &x, &y_click, &bouton, &etat_bouton);
        } while (event == MLV_NONE);

        if (event == MLV_MOUSE_BUTTON && etat_bouton == MLV_RELEASED) {
            if (x >= 250 && x <= 305 && y_click >= 670 && y_click <= 725) {
                filtre_horaire = 1; /* 0h-8h */
            } else if (x >= 250 && x <= 305 && y_click >= 735 && y_click <= 785) {
                filtre_horaire = 2; /* 8h-16h */
            } else if (x >= 250 && x <= 305 && y_click >= 790 && y_click <= 850) {
                filtre_horaire = 3; /* 16h-24h */
            } else if (x >= 250 && x <= 305 && y_click >= 855 && y_click <= 910) {
                filtre_horaire = 0; /* Tous */
            } else if (x >= 1530 && x <= 1590 && y_click >= 940 && y_click <= 990) {
                break; /* Bouton retour menu (Maison) */
            }
        }
    }

    MLV_free_image(background);
}



void modifier_segment_lutin(
    LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES],int num_semaine, int jour, int ligne, int role, int heure_debut, int heure_fin,Lutin *lutins, int nb_lutins) {
    int nb_disponibles = 0;
    Lutin* disponibles[MAX_LUTINS];
    int i, p;
    int dispo,deja_pris,l2,s;
    int x_depart ,y_depart,largeur_case,hauteur_case = 50;


    for (i = 0; i < nb_lutins; i++) {
        Lutin* l = &lutins[i];

        if (l->role != role) continue;

        /* Vérifie statut actif */
        if (l->statut == 0) continue;

        /* Vérifie si disponible sur un creneau */
        dispo = 0;
        for (p = 0; p < l->nb_plages[jour]; p++) {
            if (l->horaires[jour][p].debut <= heure_debut && l->horaires[jour][p].fin >= heure_fin) {
                dispo = 1;
                break;
            }
        }
        if (!dispo) continue;

        /* verifie dispo*/
        deja_pris = 0;
        for (l2 = 0; l2 < MAX_LIGNES; l2++) {
            if (l2 == ligne) continue;
            LigneFabrication lf2 = semaines[num_semaine][jour][l2];
            for (s = 0; s < lf2.nb_segments; s++) {
                SegmentPlanning seg2 = lf2.segments[s];
                if (seg2.debut == heure_debut && seg2.fin == heure_fin) {
                    if ((role == 1 && seg2.bricoleur == l) ||
                        (role == 2 && seg2.controleur == l) ||
                        (role == 3 && seg2.empaqueteur == l)) {
                        deja_pris = 1;
                        break;
                    }
                }
            }
            if (deja_pris) break;
        }

        if (!deja_pris) {
            disponibles[nb_disponibles++] = l;
        }
    }

    if (nb_disponibles == 0) {
        MLV_clear_window(MLV_COLOR_WHITE);
        MLV_draw_text(600, 500, "Aucun lutin disponible !", MLV_COLOR_RED);
        MLV_actualise_window();
        MLV_wait_mouse(NULL, NULL);
        return;
    }

    MLV_Image *fond_selection = MLV_load_image("./ressources/SelectionnerLutin.jpeg");
    if (fond_selection) {
        MLV_resize_image_with_proportions(fond_selection, 1600, 1000);
        MLV_draw_image(fond_selection, 0, 0);
    } else {
        MLV_clear_window(MLV_COLOR_WHITE);
    }

    MLV_draw_text(350, 200, "Choisissez un lutin disponible :", MLV_COLOR_BLACK);

     x_depart = 350;
    y_depart = 250;
    largeur_case = 400;
    hauteur_case = 50;

    for (i = 0; i < nb_disponibles; i++) {
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "%s %s", disponibles[i]->prenom, disponibles[i]->nom);

        MLV_draw_filled_rectangle(x_depart, y_depart + i * (hauteur_case + 10), largeur_case, hauteur_case, MLV_COLOR_WHITE);
        MLV_draw_rectangle(x_depart, y_depart + i * (hauteur_case + 10), largeur_case, hauteur_case, MLV_COLOR_BLACK);
        MLV_draw_text(x_depart + 10, y_depart + i * (hauteur_case + 10) + 15, buffer, MLV_COLOR_BLACK);
    }

    MLV_actualise_window();

    int x, y;
    MLV_Mouse_button bouton;
    MLV_Button_state etat_bouton;
    MLV_Event event;

    do {
        event = MLV_get_event(NULL, NULL, NULL, NULL, NULL, &x, &y, &bouton, &etat_bouton);
    } while (event != MLV_MOUSE_BUTTON || etat_bouton != MLV_RELEASED);


    for (i = 0; i < nb_disponibles; i++) {
        int y_case = y_depart + i * (hauteur_case + 10);
        if (x >= x_depart && x <= x_depart + largeur_case &&
            y >= y_case && y <= y_case + hauteur_case) {
            
            Lutin* selection = disponibles[i];

            for (int s = 0; s < semaines[num_semaine][jour][ligne].nb_segments; s++) {
                SegmentPlanning* seg = &semaines[num_semaine][jour][ligne].segments[s];
                if (seg->debut == heure_debut && seg->fin == heure_fin) {
                    if (role == 1) seg->bricoleur = selection;
                    else if (role == 2) seg->controleur = selection;
                    else if (role == 3) seg->empaqueteur = selection;
                    break;
                }
            }
            break;
        }
    }

    if (fond_selection) {
        MLV_free_image(fond_selection);
    }
}

void afficher_semaine_graphique_complet(LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES], int num_semaine, struct tm date_base, Lutin *lutins, int nb_lutins) {
    int jour = 0;
    int x, y, x2, y2;
    int i, j, t, r;
    int nb_lignes = 0;
    int lignes_visibles[MAX_LIGNES];
    int nb_zones = 0;
    int colonne = 0;
    double pixels_par_heure = (945.0 - 185.0) / 24.0;

    MLV_Event event, flush_event;
    MLV_Mouse_button bouton, flush_bouton;
    MLV_Button_state etat_bouton, etat_flush;

    struct tm date_du_jour;
    MLV_Image *fond = NULL;
    ZoneModifiable zones[500];

    int tranches[3][2] = {{0, 8}, {8, 16}, {16, 24}};
    MLV_Color couleurs_roles[3] = {
        MLV_rgba(200, 255, 200, 255),  /* Bricoleur */
        MLV_rgba(200, 200, 255, 255),  /* Contrôleur */
        MLV_rgba(235, 200, 255, 255)   /* Empaqueteur */
    };

    int x_depart = 180;
    int largeur_max = 1240;

    fond = MLV_load_image("ressources/AffichagePlanning.jpeg");
    if (!fond) {
        fprintf(stderr, "Erreur chargement image fond AffichagePlanning.jpeg\n");
        return;
    }

   /* pour eviter les double clic depuis le menu principal */
    while ((flush_event = MLV_get_event(NULL, NULL, NULL, NULL, NULL, &x2, &y2, &flush_bouton, &etat_flush)) != MLV_NONE) {}


    while (1) {
        nb_zones = 0;

        date_du_jour = date_base;
        date_du_jour.tm_mday += num_semaine * 7 + jour;
        mktime(&date_du_jour);

        MLV_clear_window(MLV_COLOR_BLACK);
        MLV_draw_image(fond, 0, 0);

        char titre[200];
        strftime(titre, sizeof(titre), "Planning du %A %d %B %Y", &date_du_jour);
        MLV_draw_text(700, 165, titre, MLV_COLOR_WHITE);

        /* fond planning */
        for (i = 0; i <= 24; i++) {
            int y_ligne = 185 + (int)(i * pixels_par_heure);
            MLV_draw_line(145, y_ligne, 1460, y_ligne, MLV_rgba(200, 200, 200, 255));
            char heure_txt[8];
            snprintf(heure_txt, sizeof(heure_txt), "%dh", i % 24);
            MLV_draw_text(110, y_ligne - 8, heure_txt, MLV_COLOR_BLACK);
        }

        /* Affichage des 3 tranches horaires (matin, aprem, soir) */
        for (t = 0; t < 3; t++) {
            int debut = tranches[t][0];
            int fin = tranches[t][1];
            int y_tranche = 185 + (int)(debut * pixels_par_heure);
            int hauteur_bloc = (int)((fin - debut) * pixels_par_heure);

            nb_lignes = 0;
            memset(lignes_visibles, 0, sizeof(lignes_visibles));

            /* Recherche des lignes de production actives sur cette tranche horaire */
            for (i = 0; i < MAX_LIGNES; i++) {
                LigneFabrication *ligne = &semaines[num_semaine][jour][i];
                for (j = 0; j < ligne->nb_segments; j++) {
                    if (ligne->segments[j].debut == debut && ligne->segments[j].fin == fin) {
                        lignes_visibles[i] = 1;
                        nb_lignes++;
                        break;
                    }
                }
            }

            if (nb_lignes == 0) continue;

            int largeur_colonne = largeur_max / nb_lignes;
            colonne = 0;

            /* Affichage du nom des lignes */
            for (i = 0; i < MAX_LIGNES; i++) {
                if (!lignes_visibles[i]) continue;
                int x_col = x_depart + colonne * largeur_colonne;
                char texte[64];
                snprintf(texte, sizeof(texte), "Ligne %d", i + 1);
                MLV_draw_text(x_col + 10, y_tranche - 20, texte, MLV_COLOR_BLACK);
                colonne++;
            }

            /* Affichage des blocs (bricoleur, contrôleur, empaqueteur) par ligne */
            colonne = 0;
            for (i = 0; i < MAX_LIGNES; i++) {
                if (!lignes_visibles[i]) continue;

                LigneFabrication *ligne = &semaines[num_semaine][jour][i];
                SegmentPlanning *seg = NULL;

                /* Recherche du segment correspondant à cette tranche */
                for (j = 0; j < ligne->nb_segments; j++) {
                    if (ligne->segments[j].debut == debut && ligne->segments[j].fin == fin) {
                        seg = &ligne->segments[j];
                        break;
                    }
                }

                int x_col = x_depart + colonne * largeur_colonne;

                for (r = 0; r < 3; r++) {
                    int x_bloc = x_col + r * (largeur_colonne / 3);

                    MLV_draw_filled_rectangle(x_bloc, y_tranche, largeur_colonne / 3 - 5, hauteur_bloc, couleurs_roles[r]);
                    MLV_draw_rectangle(x_bloc, y_tranche, largeur_colonne / 3 - 5, hauteur_bloc, MLV_COLOR_BLACK);

                    /* Enregistrement de la zone cliquable */
                    zones[nb_zones].x = x_bloc;
                    zones[nb_zones].y = y_tranche;
                    zones[nb_zones].largeur = largeur_colonne / 3 - 5;
                    zones[nb_zones].hauteur = hauteur_bloc;
                    zones[nb_zones].segment = seg;
                    zones[nb_zones].ligne = i;
                    zones[nb_zones].role = r + 1;
                    zones[nb_zones].debut = debut;
                    zones[nb_zones].fin = fin;
                    nb_zones++;

                    char texte[200] = "Aucun";
                    Lutin *l = NULL;
                    if (seg) {
                        if (r == 0) l = seg->bricoleur;
                        else if (r == 1) l = seg->controleur;
                        else if (r == 2) l = seg->empaqueteur;
                    }

                    if (l) {
                        strncpy(texte, l->prenom, sizeof(texte) - 1);
                        texte[sizeof(texte) - 1] = '\0';

                        int w, h;
                        MLV_get_size_of_text(texte, &w, &h);
                        while (w > (largeur_colonne / 3 - 10) && strlen(texte) > 1) {
                            texte[strlen(texte) - 1] = '\0';
                            MLV_get_size_of_text(texte, &w, &h);
                        }
                    }

                    MLV_draw_text(x_bloc + 5, y_tranche + 10, texte, MLV_COLOR_BLACK);
                }

                colonne++;
            }
        }

        MLV_actualise_window();


        do {
            event = MLV_get_event(NULL, NULL, NULL, NULL, NULL, &x, &y, &bouton, &etat_bouton);
        } while (event == MLV_NONE);

        if (event == MLV_MOUSE_BUTTON && etat_bouton == MLV_RELEASED) {
            if (x >= 40 && x <= 130 && y >= 490 && y <= 580) {
                if (jour > 0) jour--;
                else if (num_semaine > 0) { num_semaine--; jour = 6; }
            } else if (x >= 1475 && x <= 1565 && y >= 490 && y <= 580) {
                if (jour < 6) jour++;
                else if (num_semaine < MAX_SEMAINES - 1) { num_semaine++; jour = 0; }
            } else if (x >= 1530 && x <= 1590 && y >= 935 && y <= 1000) {
                break;
            } else if (x >= 10 && x <= 60 && y >= 870 && y <= 930) {
                MLV_Image *fond_saisie = MLV_load_image("ressources/SelectionnerDate.jpeg");
                if (fond_saisie) {
                    MLV_draw_image(fond_saisie, 0, 0);
                    MLV_actualise_window();
                    MLV_free_image(fond_saisie);
                }

                char *saisie = NULL;
                MLV_wait_input_box(520, 520, 200, 50, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE, "Aller à (JJ/MM) :", &saisie);

                if (saisie) {
                    int jj, mm;
                    if (sscanf(saisie, "%d/%d", &jj, &mm) == 2) {
                        struct tm cible = date_base;
                        cible.tm_mday = jj;
                        cible.tm_mon = mm - 1;
                        mktime(&cible);

                        time_t base = mktime(&date_base);
                        time_t cible_t = mktime(&cible);
                        int jours_diff = (int)(difftime(cible_t, base) / (60 * 60 * 24));

                        if (jours_diff >= 0 && jours_diff < MAX_SEMAINES * 7) {
                            num_semaine = jours_diff / 7;
                            jour = jours_diff % 7;
                        }
                    }
                    free(saisie);
                }
            } else if (x >= 10 && x <= 60 && y >= 940 && y <= 1000) {
                MLV_Image *fond_sauvegarde = MLV_load_image("ressources/SelectionnerFichier.jpeg");
                if (fond_sauvegarde) {
                    MLV_draw_image(fond_sauvegarde, 0, 0);
                    MLV_actualise_window();
                    MLV_free_image(fond_sauvegarde);
                }

                char *nom_fichier = NULL;
                MLV_wait_input_box(520, 520, 500, 50, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE, "Nom du fichier .ics :", &nom_fichier);

                if (nom_fichier) {
                    char chemin[256];
                    snprintf(chemin, sizeof(chemin), "%s.ics", nom_fichier);
                    sauvegarder_semaine_ics(chemin, semaines, num_semaine, date_base);
                    free(nom_fichier);
                }
            } else {
                for (i = 0; i < nb_zones; i++) {
                    if (x >= zones[i].x && x <= zones[i].x + zones[i].largeur &&
                        y >= zones[i].y && y <= zones[i].y + zones[i].hauteur) {
                        modifier_segment_lutin(semaines, num_semaine, jour, zones[i].ligne, zones[i].role,
                                               zones[i].debut, zones[i].fin, lutins, nb_lutins);
                        break;
                    }
                }
            }
        }
    }

    MLV_free_image(fond);
}
