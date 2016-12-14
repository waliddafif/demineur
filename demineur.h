void choixNiveau(int* tailleGrille, int* nbreMines, int* nbreDrapeau);

void menu();

void repartirMines(int **grilleJeu, int tailleGrille, int nbreMines);

void afficherGrille(int tailleGrille, int **grilleJeu);

int jouer(int nbresCasesVides,int **grilleJeu, int tailleGrille);

void choixCoordonne(int **grilleJeu, char action[3], int tailleGrille, int* ligne, int* colonne, int ifAuto);

int compterNbreMineAdjacente(int **grilleJeu, int ligne, int colonne, int tailleGrille);

int decouvrirCaseAdjacente(int nbreCaseDecouverte, int **grilleJeu, int ligne, int colonne, int tailleGrille);

void statistiques(int msec);
