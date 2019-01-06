/* 
 * File:   main.c
 * Author: Jules
 *
 * Created on 27 décembre 2018, 18:02
 */
// Puissance4.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define NB_COL (7)
#define NB_LGN (6)
#define PLAYER1 (1) // player 1 joue avec les X
#define PLAYER2 (2) // Player 2 joue avec les O
#define EMPTY_VALUE (' ')

// Stockage de la grille du Puissance 4
static char Grid [NB_LGN][NB_COL];

// intialise la grille à ' '
static void initGrid(void)
{
    int col, lgn;
    for (lgn = 0; lgn < NB_LGN; ++lgn)
            for (col = 0; col < NB_COL; ++col)
                    Grid[lgn][col] = EMPTY_VALUE;
}

// Affiche la grille
static void displayGrid(void)
{
    int col, lgn;
    printf("\n\n");

    // affichage de l'entête de colonnes
    printf("   ");
    for (col = 0; col < NB_COL; col++)
    {
            printf("  %d ",col+1);
    }
    printf("\n   ");

    for (col = 0; col < NB_COL; col++)
    {
            printf("----");
    }
    printf("-\n");

    for (lgn = 0; lgn < NB_LGN; lgn++)
    {
            printf(" %d | ",lgn+1);
            for (col = 0; col < NB_COL; col++)
            {
                    printf("%c", Grid[lgn][col]);
                    printf(" | ");
            }
            printf("\n");
    }
    printf("   ");
    for (col = 0; col < NB_COL; col++)
    {
            printf("----");
    }
    printf("-");
}


// La grille est remplie si la ligne du haut est pleine
int isGridFilled()
{
    int i;
    for(i = 0; i < NB_COL; i++)
    {
            if (Grid[0][i] == EMPTY_VALUE)
                    return 0;
    }
    return 1;
}


// La fonction retourne la colonne saisie par le joueur courant
int askPlayerMovement(int currentPlayer)
{
    int col;

    printf("\n\n PLAYER %d, please enter your choice (1..7): ",currentPlayer);
    scanf("%d", &col); // L'utilisateur entre la colonne à jouer

    // on vérifie que la saisie est correcte sinon on lui redemande de jouer
    // TODO : Amélioration, si l'utilisateur tape 0, il pourrait sortir et quitter le jeu
    // Si la colonne saisie est en dehors des bornes ou que la colonne est pleine, on redemande de saisir une colonne
    while (col-1 < 0 || col-1 >= NB_COL || Grid[0][col-1] != EMPTY_VALUE)
    {
            printf("\n Bad entry, player %d. Try again. Enter your choice (1..7): ",currentPlayer);
            scanf("%d", &col); // L'utilisateur entre de nouveau la colonne à jouer
    }
    return col-1;
}

// Remplissage de la grille avec la colonne choisie par l'utilisateur.
// on détermine la première ligne dispo dans la colonne choisie
// Cette fonction considère que le mouvement est valide

int fillingGridWithUserMovement(int userCol, int currentPlayer)
{
    int lgn = NB_LGN - 1;
    while (Grid[lgn][userCol] != EMPTY_VALUE)
            lgn--;

    Grid[lgn][userCol] = (currentPlayer == PLAYER1) ? 'X' : 'O';

    // on retourne la ligne courante où nous avons inséré le jeton
    return lgn;
}

// on analyse si la position donnée en paramètre est une position valide qui ne sort pas de la grid 
// et qui contient une valeur saisie par l'utilisateur

int isMovementValid(int Lgn, int Col)
{
    // Si la colonne saisie est en dehors des bornes, on retourne 0, mouvement invalide
    if (Col < 0 || Col > NB_COL) return 0;

    // Si la ligne saisie est en dehors des bornes, on retourne 0, mouvement invalide
    if (Lgn < 0 || Lgn > NB_LGN) return 0;

    // Si l'emplacement est vide on retourne 0
    if (Grid[Lgn][Col] == EMPTY_VALUE) return 0;

    // le mouvement est valide
    return 1;
}

// On analyse une ligne horizontale, verticale ou les diagonales à partir d'une position passée en paramètre
int analyseLine(int userLgn, int userCol, int offsetLgn, int offsetCol, int token)
{
    int lgn = userLgn + offsetLgn;
    int col = userCol + offsetCol;
    int nbTokens = 0;

    while (isMovementValid(lgn, col))
    {
            if (Grid[lgn][col] == token)
            {
                    nbTokens++;
                    lgn += offsetLgn;
                    col += offsetCol;
            }
            else
                    break;
    }
    return nbTokens;
}
	

// On cherche à savoir si le joueur a gagné en jouant ce dernier coup
// UserLgn et userCol représente la position dans la grille du dernbier coup de l'utilisateur
int DoesthePlayerWin(int userLgn, int userCol, char token)
{
    int nbTokens = 0;
    // On doit analyser tous les coups autours de la position courante
    // Et chercher si on a 4 jetons identiques en ligne, colonne ou diagonale

    // 1 On analyse la colonne courante, en dessous du jeton
    nbTokens = analyseLine(userLgn, userCol, 1, 0, token);
    if (nbTokens + 1 >= 4) return 1; // on fait +1 pour prendre en compte le jeton joué par l'utilisateur

    // 2 On analyse la ligne courante, à droite et à gauche du jeton
    nbTokens = analyseLine(userLgn, userCol, 0, 1, token) + analyseLine(userLgn, userCol, 0, -1, token);
    if (nbTokens + 1 >= 4) return 1;

    // 3 On analyse la diagonale de gauche à droite
    nbTokens = analyseLine(userLgn, userCol, 1, -1, token) + analyseLine(userLgn, userCol, -1, 1, token);
    if (nbTokens + 1 >= 4) return 1;

    // 4 On analyse la diagonale de droite à gauche
    nbTokens = analyseLine(userLgn, userCol, -1, -1, token) + analyseLine(userLgn, userCol, 1, 1, token);
    if (nbTokens + 1 >= 4) return 1;


    return 0;
}

int main(int argc, char** argv) 
{
    int userCol,userLgn;
    int currentPlayer = PLAYER1; // On initialise avec le premier player
    int status;

    printf("Playing Puissance4...\n\n"); 
    initGrid(); 
    displayGrid();
    while (!isGridFilled())
    {
            //on demande de jouer au player courant	
            userCol = askPlayerMovement(currentPlayer);
            // On ajoute l'entrée dans la grille
            userLgn= fillingGridWithUserMovement(userCol, currentPlayer);
            //On affiche la grille
            displayGrid();
            // On teste si le joueur courant a gagné
            status = DoesthePlayerWin(userLgn, userCol, (currentPlayer == PLAYER1) ? 'X' : 'O');
            // si l'utilisateur a gagné on sort de la boucle
            if (status == 1)
                    break;

            // on change de player
            currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
    }

    // Affichage du résultat
    if (status == 1)
    {
            printf("\n\n\n************************************\n");
            printf("  Well done! The player %d HAS WON.", currentPlayer);
            printf("\n************************************\n\n\n");

    }		
    else
    {
            printf("\n\n\n********************************\n");
            printf("     Game Over, TIE GAME!", currentPlayer);
            printf("\n********************************\n\n\n");
    }
    return (EXIT_SUCCESS);
}
