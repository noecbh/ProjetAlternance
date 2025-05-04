#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/lutins.h"
#include "../include/plannings.h"
#include "../include/fichiers.h"
#include "../include/ihm.h"
#include <MLV/MLV_all.h>

int main() {
    int nb_lutins = 0;
    int choix_menu;
    int semaine_courante = 0;
    int nb_semaines = MAX_SEMAINES;
    char *nom_fichier = NULL;

    Lutin *lutins = charger_lutins(&nb_lutins);

    LigneFabrication semaines[MAX_SEMAINES][7][MAX_LIGNES] = {0};

    struct tm date_base = {0};
    date_base.tm_year = 2025 - 1900;
    date_base.tm_mon = 4;  /* Mai */
    date_base.tm_mday = 12;
    mktime(&date_base);


    creer_fenetre();

    creer_planning_semaine(lutins, nb_lutins, semaines, &nb_semaines);

    while (1) {
        choix_menu = menu();

        if (choix_menu == 1) {
            lutins = afficher_lutins_graph(lutins, &nb_lutins, semaines, 0);
            creer_planning_semaine(lutins, nb_lutins, semaines, &nb_semaines);

            
        }

        else if (choix_menu == 2) {
      
            while (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED) {
                MLV_wait_milliseconds(50);
            }

            MLV_Event ev;
            int fx, fy;
            MLV_Mouse_button fb;
            MLV_Button_state fe;
            while ((ev = MLV_get_event(NULL, NULL, NULL, NULL, NULL, &fx, &fy, &fb, &fe)) != MLV_NONE) {}

            afficher_semaine_graphique_complet(semaines, semaine_courante, date_base, lutins, nb_lutins);
        }

        else if (choix_menu == 3) {
            MLV_clear_window(MLV_COLOR_BLACK);
            MLV_Image *fond = MLV_load_image("ressources/SelectionnerFichier.jpeg");
            if (fond) {
                MLV_draw_image(fond, 0, 0);
                MLV_actualise_window();
            }

            MLV_wait_input_box(
                520, 520, 500, 50,
                MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                "Nom du fichier à charger :", &nom_fichier
            );

            if (nom_fichier) {
                int semaine_trouvee = charger_semaine_ics(nom_fichier, semaines, lutins, nb_lutins, date_base);

                if (semaine_trouvee >= 0 && semaine_trouvee < MAX_SEMAINES) {
                    afficher_semaine_graphique_complet(semaines, semaine_trouvee, date_base, lutins, nb_lutins);
                } else {
                    fprintf(stderr, "Erreur de fichier ou semaine invalide\n");
                }

                free(nom_fichier);
            }

            if (fond) MLV_free_image(fond);
        }

        else if (choix_menu == 4) {
 
            sauvegarder_lutins(lutins, nb_lutins);
            free(lutins);
            MLV_free_window();
            exit(EXIT_SUCCESS);
        }

    }

    sauvegarder_lutins(lutins, nb_lutins);
    free(lutins);
    MLV_free_window();
    return 0;
}
