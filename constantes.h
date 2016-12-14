/* contantes.h
--------------

Rôle : définit des constantes pour tout le programme

*/

#ifndef DEF_CONSTANTES
#define CONSTANTES

    #define FACILETAILLE    9         /* Taille de la grille du jeu pour le niveau facile */
    #define FACILEMINES     10        /* Nombre de mines de la grille du jeu pour le niveau facile */
    #define FACILEDRAPEAU   3         /* Nombre de drapeau pour le niveau facile */

    #define NORMALTAILLE    15        /* Taille de la grille du jeu pour le niveau normal */
    #define NORMALMINES     30        /* Nombre de mines de la grille du jeu pour le niveau normal */
    #define NORMALDRAPEAU   9         /* Nombre de drapeau pour le niveau normal */

    #define DIFFICILTAILLE  21        /* Taille de la grille du jeu pour le niveau difficile */
    #define DIFFICILMINES   50        /* Nombre de mines de la grille du jeu pour le niveau difficile */
    #define DIFFICILDRAPEAU 15        /* Nombre de drapeau pour le niveau difficile*/

    #define CASEVIDE        -3        /* Représente une case vide pour le programme  */
    #define DRAPEAU         -4        /* Représente un drapeau pour le programme */
    #define MINES           9         /* Représente une mine pour le programme */

    #define CREUSER         "creu"    /* Action : creuser */
    #define PROPCASE        "auto"    /* Action : Proposer une case */
    #define ACTIONDRAPEAU   "drap"    /* Action : Poser un drapeau, Enlever un drapeau */
    #define ABDANDONNER     "aban"    /* Action : Abandonner la partie */

 #endif // DEF
