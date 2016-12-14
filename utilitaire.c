#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stddef.h>
#include <string.h>

#include "constantes.h" /* constantes du programme */

// permet de choisir aléatoirement un nombre compris entre 1 et Max (variable)
int choisirNomreAleatoire(int Max)
{
    return (rand() % (Max + 1));
}

// allocation d'un tableau de n ligne et p colonnes
int** allouerMatrice(int n, int p)
{
    int i = 0;
    int **tab = (int**)malloc(n*sizeof(int*));

    if(tab == NULL)
    {
        exit(0);
    }
    for(i = 0; i < n; i++)
    {
        tab[i] = (int*)malloc(p*sizeof(int));
    }
    return tab;
}

// liberation d'un tableau de n lignes
int libererMatrice(int **tab, int n)
{
    int i = 0;
    for(i = 0; i < n; i++)
    {
        free(tab[i]);
    }
    free(tab);
    return 0;
}

/* initialisation de toutes les cases d'un
   tableau de n lignes et p colonnes à la valeurs "valeurs" */
int initialiserMatrice(int **tab, int n, int p, int valeur)
{
    int i = 0, j = 0;
    // ajout de l'initialisation de toute les lignes et non seulement la première
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < p; j++)
        {
            tab[i][j] = valeur;
        }
    }
    return 0;
}

// permet de récupérer un caractère au clavier
char scanf_char()
{
    char A = 0;
    char chaine[2] = {'A', '\n'};
    scanf("%s", chaine);
    A = chaine[0];
    return A;
}

/* Permet de renvoyer seulement un entier.
   Si une chaine de caractère est entré, alors on
   affiche un message d'erreur au joueur */
int getInteger(const char *prompt, int *i)
{
  int Invalid = 0;
  int EndIndex;
  char buffer[100];

  do {
    if (Invalid)
      fputs("/!\\ Erreur de saisie. Veuillez recommencer. \n", stdout);
    Invalid = 1;
    fputs(prompt, stdout);
    if (NULL == fgets(buffer, sizeof(buffer), stdin))
      return 1;
  } while ((1 != sscanf(buffer, "%d %n", i, &EndIndex)) || buffer[EndIndex]);

  return 0;
}

/* Permet de vider le buffer.
   Récupérer depuis le site openclassroom */
void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

/* Permet de lire une chaine de caractère.
   Récupérer depuis le site openclassroom */
int lire(char *chaine, int longueur)
{
    char *positionEntree = NULL;

    if (fgets(chaine, longueur, stdin) != NULL)
    {
        positionEntree = strchr(chaine, '\n');
        if (positionEntree != NULL)
        {
            *positionEntree = '\0';
        }
        else
        {
            viderBuffer();
        }
        return 1;
    }
    else
    {
        viderBuffer();
        return 0;
    }
}
