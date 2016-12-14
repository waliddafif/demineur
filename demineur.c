/* demineur.c
--------------

Rôle : classe contenant les fonctions principales du jeu

*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <windows.h> /* Utilisé pour afficher les couleurs sur le terminal windows */

#include "constantes.h" /* constantes du programme */
#include "utilitaire.h"

#include "demineur.h"

/* Variables liées au statistiques */
extern int statNbreCoupJouer;
extern int statNbreCaseNonDecouverte;
extern int statNbreCaseDecouverte;
extern int statDrapeauPose;

/* Variable permettant d'afficher les couleurs (windows) */
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
WORD saved_attributes;

// Choix du niveau de difficulter
void choixNiveau(int* tailleGrille, int* nbreMines, int* nbreDrapeau)
{
    int validChoix = 0;
    int choixNiveau = 0;
    printf("Les differents niveaux  sont : 1 (facile), 2 (normal) et 3 (difficile). \n");

    while(validChoix == 0){
       /* Fait appel à la fonction qui permet de récupérer un nombre */
       getInteger("Choix du niveau : ", &choixNiveau);

       switch(choixNiveau)
       {
           case 1:
               printf("\nNiveau Facile ! \n\n");
               validChoix = 1;
               *tailleGrille = FACILETAILLE;
               *nbreMines = FACILEMINES;
               *nbreDrapeau = FACILEDRAPEAU;
               break;
           case 2:
               printf("\nNiveau Normal ! \n\n");
               validChoix = 1;
               *tailleGrille = NORMALTAILLE;
               *nbreMines = NORMALMINES;
               *nbreDrapeau = NORMALDRAPEAU;
               break;
           case 3:
               printf("\nNiveau Difficile ! \n\n");
               validChoix = 1;
               *tailleGrille = DIFFICILTAILLE;
               *nbreMines = DIFFICILMINES;
               *nbreDrapeau = DIFFICILDRAPEAU;
               break;
           default :
               printf("/!\\ Erreur de saisie. Veuillez recommencer. \n\n");
               break;
        }
    }
}

// menu du jeu
void menu()
{
    printf("------------------- Menu du jeu -------------- \n");
    printf("Voici les commandes du jeu : \n");
    printf("        Creuser : %s \n", CREUSER);
    printf("        Proposer une case : %s \n", PROPCASE);
    printf("        Poser un drapeau : %s \n", ACTIONDRAPEAU);
    printf("        Enlever un drapeau : %s \n", ACTIONDRAPEAU);
    printf("        Abandonner la partie : %s \n", ABDANDONNER);
    printf("---------------------------------------------- \n\n");
}


/* Permet de répartir les mines dans la grille du jeu. */
void repartirMines(int **grilleJeu, int tailleGrille, int nbreMines)
{
    int i = 0;
    int ligne = 0;
    int colonne = 0;

    /* nécéssaire pour la fonction choisirNombreAleatoire
       car risque de dépasser la taille du tableau => provoquer une erreur*/
    tailleGrille--;

    for (i = 0; i < nbreMines ; i++ ){

        do {
            ligne = choisirNomreAleatoire(tailleGrille);
            colonne = choisirNomreAleatoire(tailleGrille);
        }while ( grilleJeu[ligne][colonne] == MINES ); // si il y a déja une mines sur cette case on reboucle

        grilleJeu[ligne][colonne] = MINES;
    }
}

/* Permet d'afficher la grille en fonction de la taille choisi par l'utilisateur,
   ainsi que les coups joués

   X => case non découverte,
   D => case ou est poser un drapeau
   0,1,2,3,4,5,6,7,8,9 pour le nombre(s) de mine(s) adjacente(s)
*/
void afficherGrille(int tailleGrille, int **grilleJeu)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    /* Sauvergarde des attributs existant (de la console) */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    /* couleur vert */
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

    printf("\n\n########################## Jouer ########################## \n\n" );

    /* Restaure les attributs originaux */
    SetConsoleTextAttribute(hConsole, saved_attributes);

    int i = 0, j = 0;

    for(i = 0; i < tailleGrille; i++)
    {
        // permet d'afficher les coordonnée des colonnes
        if(i == 0)
        {
            printf("    ");
            for (j = 0; j < tailleGrille; j++)
            {
                /* permet la bonne affichage des colonnes du tableau
                   (pour qu'il n'y est pas de décalage pour les chiffres supérieur a 9) */
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
                if(j < 10)
                {
                    printf("  %d  ",j);
                }
                else if(j >= 10 && j < 100 )
                {
                    printf("  %d ",j);
                }
                SetConsoleTextAttribute(hConsole, saved_attributes);
            }
            printf("\n");
        }

        /* permet d'afficher les coordonnée des lignes
           permet la bonne affichage des lignes du tableau
           (pour qu'il n'y est pas décalage pour les chiffres supérieur a 9) */
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
        if(i < 10)
        {
            printf(" %d  ",i);
        }
        else if(i >= 10 && i < 100 )
        {
            printf(" %d ",i);
        }
        SetConsoleTextAttribute(hConsole, saved_attributes);

        // permet d'afficher les le contenue de la grille
        for(j = 0; j < tailleGrille; j++)
        {
            if(grilleJeu[i][j] == DRAPEAU)
            {
                printf("  D  ");

            }else if(grilleJeu[i][j] == MINES || grilleJeu[i][j] == CASEVIDE)
            {
                printf("  X  ");

            }else if(grilleJeu[i][j] >= 0)
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                printf("  %d  ", grilleJeu[i][j]);
                SetConsoleTextAttribute(hConsole, saved_attributes);
            }

        }
        printf("\n");
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("\n\n########################## Jouer ########################## \n\n");

    SetConsoleTextAttribute(hConsole, saved_attributes);
}


/* Fonction regroupant les traitements du jeu. */
int jouer(int nbreCaseDecouverte, int **grilleJeu, int tailleGrille)
{
    int validChoix = 0;
    char action[3];
    int ligne = 0;
    int colonne = 0;
    /* variable pour savoir le nombre de mines aux alentours */
    int nbreMinesAlentour = 0;

    /* action du joueur (creuser, abandonner...)*/
    while(validChoix == 0)
    {
       printf("\nQuel action voulez-vous effectuer ? ");

       /* récupère la chaine de caractère */
       lire(action, 10);

       // si l'utilisateur n'entre pas une action valide alors reboucle
       if( strcmp(action, CREUSER) != 0 && strcmp(action, PROPCASE) !=0
          && strcmp(action, ACTIONDRAPEAU) !=0 && strcmp(action, ABDANDONNER) !=0)
       {
           printf("/!\\ Erreur de saisie. Veuillez recommencer.");
           continue;
       }

       /* si le joueur a atteint la limite des drapeau */
        if(strcmp(action, ACTIONDRAPEAU) == 0 && statDrapeauPose == 0){
            printf("/!\\ Limite des drapeaux atteinte ! Veuillez recommencer.");
            continue;
        }

       validChoix = 1;
    }

    /* Une fois que l'action est effectué on traite chaque cas */
    if(strcmp(action, ABDANDONNER) == 0)
    {
        return -2;

    }else if(strcmp(action, CREUSER) == 0)
    {
        choixCoordonne(grilleJeu, action, tailleGrille, &ligne, &colonne, 0);

        /* Si le joueur tombe sur une mines alors fin de la partie*/
        if(grilleJeu[ligne][colonne] == MINES)
        {
            return -1;
        }

        /* On incémente le nombre de case découverte a chaque coup joué
           Si le joueur arrive au total de case vide alors il gagne la partie */
        nbreCaseDecouverte++;
        statNbreCaseDecouverte++;  // statistique de fin du jeu

        /* appel a la fonction pour connaître le nombre de mine adjacente au coup joué  */
        nbreMinesAlentour = compterNbreMineAdjacente(grilleJeu, ligne, colonne, tailleGrille);

        /* on affiche le nombre de mines adjacente a l'utilisateur*/
        grilleJeu[ligne][colonne] = nbreMinesAlentour;

        /* si aucune mines aux alentours on découvre les cases adjacentes */
        if(nbreMinesAlentour == 0)
        {
           nbreCaseDecouverte = decouvrirCaseAdjacente(nbreCaseDecouverte, grilleJeu, ligne, colonne, tailleGrille);
        }

    }else if(strcmp(action, PROPCASE) == 0)
    {

        choixCoordonne(grilleJeu, action, tailleGrille, &ligne, &colonne, 1);

        /* Si le joueur tombe sur une mines alors fin de la partie*/
        if(grilleJeu[ligne][colonne] == MINES)
        {
            return -1;
        }

        nbreCaseDecouverte++;
        statNbreCaseDecouverte++;  // statistique de fin du jeu

        /* appel a la fonction pour connaître le nombre de mine adjacente au coup joué  */
        nbreMinesAlentour = compterNbreMineAdjacente(grilleJeu, ligne, colonne, tailleGrille);

        /* on affiche le nombre de mines adjacente a l'utilisateur*/
        grilleJeu[ligne][colonne] = nbreMinesAlentour;

        /* si aucune mines aux alentours on découvre les cases adjacentes */
        if(nbreMinesAlentour == 0)
        {
           nbreCaseDecouverte = decouvrirCaseAdjacente(nbreCaseDecouverte, grilleJeu, ligne, colonne, tailleGrille);
        }

    }else if(strcmp(action, ACTIONDRAPEAU) == 0)
    {
        choixCoordonne(grilleJeu, action, tailleGrille, &ligne, &colonne, 0);

        /* on affiche sur la grille le drapeau, si un drapeau est déja poser on l'enlève */
        if(grilleJeu[ligne][colonne] == DRAPEAU){
            grilleJeu[ligne][colonne] = CASEVIDE;
        }else{
            grilleJeu[ligne][colonne] = DRAPEAU;
        }

        statDrapeauPose--;
    }

    /* retourne le nombre de case découverte */
    return nbreCaseDecouverte;
}

/* Choix des coordonnée (de la grille du jeu) du coup joué. */
void choixCoordonne(int **grilleJeu, char action[3], int tailleGrille, int* ligne, int* colonne, int ifAuto)
{
    int validChoix = 0;

    /* par la suite on soustrait de 1 la taille de la grille pour vérifier la limite
       des coordonnées entrées par l'utilisateur car un tableau commence a partir de 0 */

    while(validChoix == 0){

        /* Si l'utilisateur choisi lui-même les coordonnées d'une case alors on lui affiche
           sinon selection automatique des coordonnées de la case */
        if(ifAuto == 0)
        {
            printf("\nChoix des coordonnees, limite %d.", tailleGrille - 1);

            getInteger("\nLigne : ", ligne);
            getInteger("Colonne : ", colonne);
        }else
        {
            *ligne = choisirNomreAleatoire(tailleGrille);
            *colonne = choisirNomreAleatoire(tailleGrille);
        }
        printf("\n");

        if(*ligne > (tailleGrille - 1) || *colonne > (tailleGrille - 1) )
        {
            printf("/!\\ Erreur de saisie. Veuillez recommencer. \n");
            continue;
        }

        /* Si une case déja découverte est selectionnée alors on recommence la saisie */
        if( ( strcmp(action, CREUSER) || strcmp(action, PROPCASE) || strcmp(action, ACTIONDRAPEAU))
             && (grilleJeu[*ligne][*colonne] >= 0 && grilleJeu[*ligne][*colonne] < MINES) )
        {
            printf("/!\\ Case deja decouverte. Veuillez recommencer. \n");
            continue;
        }

        /* Si un drapeau est poser sur la case et que le joueur veut la creuser alors
           on affiche un message d'erreur et on reboucle */
        if( ( strcmp(action, CREUSER) || strcmp(action, PROPCASE) )
            && (grilleJeu[*ligne][*colonne] == DRAPEAU) )
        {
            printf("/!\\ Impossible de creuser car drapeau pose sur la case. Veuillez recommencer. \n");
            continue;
        }

        validChoix = 1;
    }

    /* Statistique de coup joué */
    statNbreCoupJouer++;
}


/* Permet de compter le nombre de mines adjacente pour l'afficher au joueur. */
int compterNbreMineAdjacente(int **grilleJeu, int ligne, int colonne, int tailleGrille)
{
    int nbreMinesAlentour = 0;

    /* Pour chaque case adjacente on vérifie si il y a une mine,
    si c'est le cas on incrémente la variable nbreMinesAlentour.
    Nous teston aussi pour chaque cas si il y a un dépassement de la grille */

    if(ligne > 0 && colonne > 0 && grilleJeu[ligne - 1][colonne - 1] == MINES){ // 1er cas
        nbreMinesAlentour++;
    }
    if(ligne > 0 && grilleJeu[ligne - 1][colonne] == MINES){ // 2ème cas
        nbreMinesAlentour++;
    }
    if(ligne > 0 && (colonne < (tailleGrille - 1)) && grilleJeu[ligne - 1][colonne + 1] == MINES){ // 3ème cas
        nbreMinesAlentour++;
    }
    if((colonne < (tailleGrille - 1)) && grilleJeu[ligne][colonne + 1] == MINES){ // 4ème cas
        nbreMinesAlentour++;
    }
    if((ligne < (tailleGrille - 1)) && (colonne < (tailleGrille - 1)) // 5ème cas
        && grilleJeu[ligne + 1][colonne + 1] == MINES)
    {
        nbreMinesAlentour++;
    }
    if((ligne < (tailleGrille - 1)) && grilleJeu[ligne + 1][colonne] == MINES){ // 6ème cas
        nbreMinesAlentour++;
    }
    if((ligne < (tailleGrille - 1)) && colonne > 0     // 7ème cas
        && grilleJeu[ligne + 1][colonne - 1] == MINES)
    {
        nbreMinesAlentour++;
    }
    if(colonne > 0 && grilleJeu[ligne][colonne - 1] == MINES){ // 8ème cas
        nbreMinesAlentour++;
    }

    return nbreMinesAlentour;
}

/* Permet de découvrir chaque case adjacente dans le cas ou
   la case creusé n'est pas entouré de mines */
int decouvrirCaseAdjacente(int nbreCaseDecouverte, int **grilleJeu, int ligne, int colonne, int tailleGrille)
{
    int nbreMinesAdjacente = 0;

    /* On découvre chaque case adjacente.
       Nous teston aussi pour chaque cas si il y a un dépassement de la grille */

    if(ligne > 0 && colonne > 0) // 1er cas
    {
        // on réinitialise le nbre de mine pour chaque case adjacente
        nbreMinesAdjacente = 0;

        nbreMinesAdjacente = compterNbreMineAdjacente(grilleJeu, ligne - 1, colonne - 1, tailleGrille);
        grilleJeu[ligne - 1][colonne - 1] = nbreMinesAdjacente;

        nbreCaseDecouverte++;
        statNbreCaseDecouverte++;
    }
    if(ligne > 0) // 2ème cas
    {
        nbreMinesAdjacente = 0;

        nbreMinesAdjacente = compterNbreMineAdjacente(grilleJeu, ligne - 1, colonne, tailleGrille);
        grilleJeu[ligne - 1][colonne] = nbreMinesAdjacente;

        nbreCaseDecouverte++;
        statNbreCaseDecouverte++;
    }
    if(ligne > 0 && colonne < (tailleGrille - 1) ) // 3ème cas
    {
        nbreMinesAdjacente = 0;

        nbreMinesAdjacente = compterNbreMineAdjacente(grilleJeu, ligne - 1, colonne + 1, tailleGrille);
        grilleJeu[ligne - 1][colonne + 1] = nbreMinesAdjacente;

        nbreCaseDecouverte++;
        statNbreCaseDecouverte++;
    }
    if(colonne < (tailleGrille - 1) ) // 4ème cas
    {
        nbreMinesAdjacente = 0;

        nbreMinesAdjacente = compterNbreMineAdjacente(grilleJeu, ligne, colonne + 1, tailleGrille);
        grilleJeu[ligne][colonne + 1] = nbreMinesAdjacente;

        nbreCaseDecouverte++;
        statNbreCaseDecouverte++;
    }
    if(ligne < (tailleGrille - 1) && colonne < (tailleGrille - 1) ) // 5ème cas
    {
        nbreMinesAdjacente = 0;

        nbreMinesAdjacente = compterNbreMineAdjacente(grilleJeu, ligne + 1, colonne + 1, tailleGrille);
        grilleJeu[ligne + 1][colonne + 1] = nbreMinesAdjacente;

        nbreCaseDecouverte++;
        statNbreCaseDecouverte++;
    }
    if(ligne < (tailleGrille - 1) ) // 6ème cas
    {
        nbreMinesAdjacente = 0;

        nbreMinesAdjacente = compterNbreMineAdjacente(grilleJeu, ligne + 1, colonne, tailleGrille);
        grilleJeu[ligne + 1][colonne] = nbreMinesAdjacente;

        nbreCaseDecouverte++;
        statNbreCaseDecouverte++;
    }
    if(ligne < (tailleGrille - 1) && colonne > 0 )     // 7ème cas
    {
        nbreMinesAdjacente = 0;

        nbreMinesAdjacente = compterNbreMineAdjacente(grilleJeu, ligne + 1, colonne - 1, tailleGrille);
        grilleJeu[ligne + 1][colonne - 1] = nbreMinesAdjacente;

        nbreCaseDecouverte++;
        statNbreCaseDecouverte++;
    }
    if(colonne > 0) // 8ème cas
    {
        nbreMinesAdjacente = 0;

        nbreMinesAdjacente = compterNbreMineAdjacente(grilleJeu, ligne, colonne - 1, tailleGrille);
        grilleJeu[ligne][colonne - 1] = nbreMinesAdjacente;

        nbreCaseDecouverte++;
        statNbreCaseDecouverte++;
    }

    return nbreCaseDecouverte;
}


/* Permet d'afficher les statistiques de la partie jouée. */
void statistiques(int msec)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    /* Sauvergarde des attributs existant (de la console) */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("\n\n------------------------- Statistiques de la partie ------------------------- \n\n" );
    SetConsoleTextAttribute(hConsole, saved_attributes);

    printf("Nombre de coup joues : %d", statNbreCoupJouer);
    printf("\nNombre de case non decouverte : %d", statNbreCaseNonDecouverte);
    printf("\nNombre de case decouverte : %d", statNbreCaseDecouverte);
    printf("\nNombre de drapeau non utilise : %d", statDrapeauPose);
    printf("\nTemps de jeu : %d secondes %d milliseconde", msec/1000, msec%1000);

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("\n\n------------------------- Statistiques de la partie ------------------------- \n\n" );
    SetConsoleTextAttribute(hConsole, saved_attributes);
}
