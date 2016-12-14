#include <stdio.h> /* gestion des entrée sortie */
#include <stdlib.h> /* allocation mémoire types */
#include <time.h> /* tirage aléatoire */
#include <math.h> /* fonctions mathématiques */
#include <string.h> /* Manipulation des chaines de caractère. Ici pour la comparaison.*/
#include <windows.h> /* Utilisé pour afficher les couleurs sur le terminal windows */

#include "constantes.h" /* constantes du programme */
#include "utilitaire.h" /* contient les prototypes des fonctions annexes */
#include "demineur.h" /* contient les prototypes pour les fonctions relatif au jeu du démineur */

/* Variables liées au statistiques */
int statNbreCoupJouer;
int statNbreCaseNonDecouverte;
int statNbreCaseDecouverte;
int statDrapeauPose;

int main()
{
    printf("------------------- Bienvenue sur le jeu du Demineur ! ----------------- \n");
    printf("------------------- Developped By Yassin DAFIF and Yannick Kilolo Miswana ----------------- \n\n");

    char replayGame = 'o';

    /* Variables du jeu*/
    int tailleGrille = 0;
    int nbreMines = 0;
    int nbreDrapeau = 0;

    int **grilleJeu;
    int nbresCasesVides = 0;
    int nbreCaseDecouverte = 0;

    while(replayGame == 'o')
    {
        int finPartie = 0;

        /* choix du niveau */
        choixNiveau(&tailleGrille, &nbreMines, &nbreDrapeau);
        /* calcul du nombre de cases vides */
        nbresCasesVides = (tailleGrille * tailleGrille) - nbreMines;
        /* alloue un tableau en mémoire */
        grilleJeu = allouerMatrice(tailleGrille, tailleGrille);
        /* initialise la grille avec la valeur CASEVIDE */
        initialiserMatrice(grilleJeu, tailleGrille, tailleGrille, CASEVIDE);

        /* démmarre le timer */
        clock_t start = clock(), diff;
        srand(time(NULL));
        /* répartie le nombre de mines dans la grille */
        repartirMines(grilleJeu, tailleGrille, nbreMines);

        /* variable permettant de compter les drapeaux disponible */
        statDrapeauPose = nbreDrapeau;

        /* Début du jeu */
        while(finPartie == 0)
        {
            /* affiche le nombre de mines au joueur et le nombre maximum de drapeau pouvant être posé */
            printf("Mines : %d      Drapeau : %d      Case decouverte : %d", nbreMines, statDrapeauPose, nbreCaseDecouverte);

            /* affiche la grille */
            afficherGrille(tailleGrille, grilleJeu);
            /* Affiche le menu du jeu */
            menu();
            nbreCaseDecouverte = jouer(nbreCaseDecouverte, grilleJeu, tailleGrille);

            if(nbreCaseDecouverte == -2)
            {
                printf("\nAbandon de la partie :( \n");
                finPartie = 1;
            }
            if(nbreCaseDecouverte == -1)
            {
                printf("\nBOUUUUMMM !!! Vous etes mort... \n");
                finPartie = 1;
            }
            if(nbreCaseDecouverte == nbresCasesVides)
            {
                printf("\nPartie victorieuse, bravo !!! \n");
                finPartie = 1;
            }
        }

        /* Permet de savoir le nombre de case non découverte par le joueur => statistique de fin*/
        statNbreCaseNonDecouverte = ((tailleGrille * tailleGrille) - nbreMines) - statNbreCaseDecouverte;

        /* calcul le temps passé */
        diff = clock() - start;
        int msec = diff * 1000 / CLOCKS_PER_SEC;

        /* statistiques de la partie */
        statistiques(msec);

        /* laisse le choix à l'utilisateur de pouvoir recommencer la partie */
        int validChoix = 0;
        while(validChoix == 0 )
        {
            printf("\n\nVoulez-vous recommencez la partie (o/n) ? ");
            replayGame = scanf_char();

            if (replayGame != 'o' && replayGame != 'n')
            {
                printf("/!\\ Erreur de saisie. Veuillez recommencer.");
                continue;
            }

            validChoix = 1;
            statNbreCoupJouer = 0;
            statNbreCaseNonDecouverte = 0;
            statNbreCaseDecouverte = 0;
            statDrapeauPose = 0;
            viderBuffer();

            printf("\n\n");
        }
    }

    /* libère la mémoire */
    libererMatrice(grilleJeu, tailleGrille);

    printf("------------------- Merci et a bientot ! ----------------- \n");
    return 0;
}
