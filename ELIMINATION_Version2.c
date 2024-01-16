#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define N 3
#define TAILLE (N*N)

typedef struct {
    int valeur;
    bool candidats[TAILLE + 1];
    int nbCandidats;
} tCase2;


typedef int tGrille[TAILLE][TAILLE];
typedef tCase2 tGrilleCase[TAILLE][TAILLE];

void afficherStats(int *nbCasesVides,int *nbCasesVidesAvantSolver, int *nombreDeCandidats, int *nombreDeCandidatsAvantSolver,char *nomFichier);

void initCase(tCase2 *Case);

int initNbCaseVide(tGrille grille);

//prends la grille et regarde les différentas case vides
int nombreDeCandidats(tGrilleCase grilleCase);

void initCandidats(tGrilleCase grilleCase);

void chargerGrille(tGrille grille,char *nomFichier);

//estcandidats teste si une valeur est possible à mettre
bool estCandidats(tGrilleCase grilleCase, int valeur, int numLignes , int numColonne  );

//l'ajout du candidats au tablo booléen
void ajouterCandidats(tCase2 *Case, int valeur);

void ajouterValeur(tCase2 *Case, tGrilleCase grilleCase, int numLigne, int numColonne);

//retire le candidats du tablo
void retirerCandidats(tCase2 *Case, int valeur);

void retirerCandidatsColonne(tGrilleCase grilleCase, int numColonne, int valeur);
void retirerCandidatsBloc(tGrilleCase grilleCase, int numLigne, int numColonne, int valeur);
void retirerCandidatsLigne(tGrilleCase grilleCase, int numLigne, int valeur);

void affichageGrille(tGrille grille);

void affecterValeur(tGrille grille,tGrilleCase grilleCase);

void singletonCache(tGrille grille,tGrilleCase grilleCase, int numLigne, int numColonne, int valeur,int *nbCasesVides);

void singletonCacheLignes(tGrilleCase grilleCase, tCase2 *Case, int numLigne, int valeur);
void singletonCacheColonne(tGrilleCase grilleCase, tCase2 *Case, int numColonne, int valeur);
void singletonCacheBloc(tGrilleCase grilleCase, tCase2 *Case, int numLigne, int numColonne, int valeur);

int main(){
    tGrille grille;
    tGrilleCase grilleCase;
    char nomgrille[30];
    bool progression = true;
    int nbCasesVidesAvant;
    int nombresCandidats = 0,nbCandidatsAvantSolver;
    int nbCasesVides,nbCasesVidesAvantSolver;
    int nbCaseRempli = 0 ;
    int toursSansAvance = 0;

    /*float tauxDeRemplissage;
    int candidatsElimine;
    float pourcentageElemination;
    */
    chargerGrille(grille,nomgrille);
    // se deplace dans le tableau jusuqu'à trouvé une case qui à la valeur 0 et après test les candidats possisble et ajoute cela au tableau de booléen 
    // après re tester pour savoir les valeurs à enlever les candidats savoir les candidats uniques
    nbCasesVides = initNbCaseVide(grille);
    nbCasesVidesAvantSolver = initNbCaseVide(grille);
    printf(" nombre de case vides %d\n",nbCasesVides);
    printf("grille initial\n");
    affichageGrille(grille);
    affecterValeur(grille,grilleCase);
    initCandidats(grilleCase);
    
    nbCandidatsAvantSolver = nombreDeCandidats(grilleCase);
    printf("nb candidats %d\n",nombresCandidats);
    

    //se deplace dans la grille et cherche les cases qui n'ont que un seul candidats 
    while (nbCasesVides != 0 && progression == true && toursSansAvance < 5000) {
        nbCasesVidesAvant = nbCasesVides;

        for (int i = 0; i < TAILLE; i++) {
            for (int j = 0; j < TAILLE; j++) {

                if (grilleCase[i][j].nbCandidats == 1) {
                    ajouterValeur(&grilleCase[i][j], grilleCase, i, j);
                    grille[i][j] = grilleCase[i][j].valeur;
                    // Retirer les candidats dans la ligne, colonne et bloc
                    retirerCandidatsLigne(grilleCase, i, grilleCase[i][j].valeur);
                    retirerCandidatsColonne(grilleCase, j, grilleCase[i][j].valeur);
                    retirerCandidatsBloc(grilleCase, i, j, grilleCase[i][j].valeur);
                    nbCasesVides--;
                    nombresCandidats++;
                    nbCaseRempli++;
                    progression = true;
                }
                if (grilleCase[i][j].valeur == 0 && grilleCase[i][j].nbCandidats != 1)
                {
                    for (int val = 1; val <= TAILLE; val++) {
                        if (grilleCase[i][j].candidats[val] == true) {
                            singletonCache(grille, grilleCase, i, j, val , &nbCasesVides);
                        }
                    }
                }

                if (nbCasesVidesAvant == nbCasesVides) {
                    toursSansAvance++;
                } else {
                    toursSansAvance = 0;
                }
                if (toursSansAvance == 5000) {
                    progression = false;
                    printf("tours sans avance %d\n", toursSansAvance);
                }
                
  
            }
        }
        
    }
    
    printf("nb candidats %d\n",nbCandidatsAvantSolver);
    printf("nb case vide %d\n", nbCasesVides);
    
    printf("fin de prog\n");
    affichageGrille(grille);
    
    afficherStats(&nbCaseRempli,&nbCasesVidesAvantSolver,&nombresCandidats,&nbCandidatsAvantSolver,nomgrille);
    
}


void initCase(tCase2 *Case){
    for (int i = 1; i <= TAILLE; i++)
    {
        Case->candidats[i] = false;
    }
    Case->nbCandidats = 0;
    
}

int initNbCaseVide(tGrille grille){
    int nbCaseVide = 0;
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            if (grille[i][j] == 0)
            {
                nbCaseVide++;
            }
            
        }
        
    }
    return nbCaseVide;
    
}

void initCandidats(tGrilleCase grilleCase){
    bool nbEstCandidats;
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            if (grilleCase[i][j].valeur == 0)
            {
                for (int compt = 1; compt <= TAILLE; compt++) {
                    nbEstCandidats = estCandidats(grilleCase, compt, i, j);
                    if (nbEstCandidats == true) {
                        ajouterCandidats(&grilleCase[i][j], compt);
                    }
                }
            }
            
            
        }
        
    }
     
}

int nombreDeCandidats(tGrilleCase grilleCase){
    int sommeCandidats = 0;
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            sommeCandidats += grilleCase[i][j].nbCandidats;
        }
        
    }
    
    return sommeCandidats;
}

void ajouterCandidats(tCase2 *Case, int valeur){
    Case->candidats[valeur] = true;
    Case->nbCandidats++;
      
}

void retirerCandidats(tCase2 *Case, int valeur){
    if (Case->candidats[valeur] == true) {
        Case->candidats[valeur] = false;
        Case->nbCandidats--;
    }
}

void retirerCandidatsColonne(tGrilleCase grilleCase, int numColonne, int valeur) {
    for (int i = 0; i < TAILLE; i++) {
        if (grilleCase[i][numColonne].valeur == 0) {
            retirerCandidats(&grilleCase[i][numColonne], valeur);
            printf("candidats retirer %d a le place %d %d\n",valeur,i,numColonne);
        }
    }
}

void retirerCandidatsBloc(tGrilleCase grilleCase, int numLigne, int numColonne, int valeur) {
    int coinLigne = (numLigne / 3) * 3;
    int coinColonne = (numColonne / 3) * 3;

    for (int i = coinLigne; i < coinLigne + 3; i++) {
        for (int j = coinColonne; j < coinColonne + 3; j++) {
            if (grilleCase[i][j].valeur == 0) {
                retirerCandidats(&grilleCase[i][j], valeur);
                printf("candidats retirer %d a le place %d %d\n",valeur,i,j);
            }
        }
    }
}

void retirerCandidatsLigne(tGrilleCase grilleCase, int numLigne, int valeur) {
    for (int j = 0; j < TAILLE; j++) {
        if (grilleCase[numLigne][j].valeur == 0) {
            retirerCandidats(&grilleCase[numLigne][j], valeur);
            printf("candidats retirer %d a le place %d %d\n",valeur,numLigne,j);
        }
    }
}

bool estCandidats(tGrilleCase grilleCase, int valeur, int numLignes , int numColonne){
    int i, j, coinLigne, coinColonne;
    bool possible = true;
    for (j = 0; j < TAILLE; j++) {
        if (grilleCase[numLignes][j].valeur == valeur && j != numColonne) {
            possible = false;
        }
    }

    j = numColonne;
    i = 0;

    for (i = 0; i < TAILLE; i++) {
        if (grilleCase[i][numColonne].valeur == valeur && i != numLignes) {
            possible = false;
        }
    }

    coinLigne = (numLignes / 3) * 3;
    coinColonne = (numColonne / 3) * 3;

    for (int i = coinLigne; i < coinLigne + 3; i++) {
        for (int j = coinColonne; j < coinColonne + 3; j++) {
            if (grilleCase[i][j].valeur == valeur && (i != numLignes || j != numColonne)) {
                possible = false;
            }
        }
    }
    return possible;  
}

void affecterValeur(tGrille grille, tGrilleCase grilleCase) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (grille[i][j] != 0) {
                grilleCase[i][j].valeur = grille[i][j];
                initCase(&grilleCase[i][j]);
            }else{
                grilleCase[i][j].valeur = 0;
                initCase(&grilleCase[i][j]);
            }
        }
    }

    
}

void ajouterValeur(tCase2 *Case, tGrilleCase grilleCase, int numLigne, int numColonne) {
    int i = 1;
    while (Case->candidats[i] != true && i <= TAILLE) {
        i++;
    }
    Case->valeur = i;
    retirerCandidats(Case, i);

    // Mettre à jour la grilleCase et retirer les candidats associés à la nouvelle valeur
    grilleCase[numLigne][numColonne].valeur = Case->valeur;
    retirerCandidatsLigne(grilleCase, numLigne, Case->valeur);
    retirerCandidatsColonne(grilleCase, numColonne, Case->valeur);
    retirerCandidatsBloc(grilleCase, numLigne, numColonne, Case->valeur);
}


// Fonction singletonCache réintégrée
void singletonCache(tGrille grille, tGrilleCase grilleCase, int numLigne, int numColonne, int valeur, int *nbCasesVides) {
    int coinColonne, coinLigne;

    int nbCandidatsTrouves = 0;

    // Vérifie dans la ligne
    for (int j = 0; j < TAILLE; j++) {
        if (j != numColonne && grilleCase[numLigne][j].candidats[valeur] == true) {
            nbCandidatsTrouves++;
        }
    }

    // Vérifie dans la colonne
    for (int i = 0; i < TAILLE; i++) {
        if (i != numLigne && grilleCase[i][numColonne].candidats[valeur] == true) {
            nbCandidatsTrouves++;
        }
    }

    coinLigne = (numLigne / 3) * 3;
    coinColonne = (numColonne / 3) * 3;

    // Vérifie dans le bloc
    for (int i = coinLigne; i < coinLigne + 3; i++) {
        for (int j = coinColonne; j < coinColonne + 3; j++) {
            if ((i != numLigne || j != numColonne) && grilleCase[i][j].candidats[valeur] == true) {
                nbCandidatsTrouves++;
            }
        }
    }

    if (nbCandidatsTrouves == 1 ) {
        
        // Retirer le candidat seulement dans la case correspondante
        for (int val = 1; val <= TAILLE; val++) {
            if (val != valeur && grilleCase[numLigne][numColonne].candidats[val] == true) {
                retirerCandidats(&grilleCase[numLigne][numColonne], val);
            }
        }
        printf("Singleton trouvé en %d %d valeur = %d\n", numLigne, numColonne, valeur);
    }
}

void singletonCacheLignes(tGrilleCase grilleCase, tCase2 *Case, int numLigne, int valeur) {
    for (int j = 0; j < TAILLE; j++) {
        if (j != numLigne && grilleCase[numLigne][j].valeur == 0 && grilleCase[numLigne][j].candidats[valeur] == true) {
            retirerCandidats(&grilleCase[numLigne][j], valeur);
        }
    }
}



void singletonCacheColonne(tGrilleCase grilleCase, tCase2 *Case, int numColonne, int valeur) {
    for (int i = 0; i < TAILLE; i++) {
        if (i != numColonne && grilleCase[i][numColonne].valeur == 0 && grilleCase[i][numColonne].candidats[valeur] == true) {
            retirerCandidats(&grilleCase[i][numColonne], valeur);
        }
    }
}

void singletonCacheBloc(tGrilleCase grilleCase, tCase2 *Case, int numLigne, int numColonne, int valeur) {
    int coinLigne = (numLigne / 3) * 3;
    int coinColonne = (numColonne / 3) * 3;

    for (int i = coinLigne; i < coinLigne + 3; i++) {
        for (int j = coinColonne; j < coinColonne + 3; j++) {
            if ((i != numLigne || j != numColonne) && grilleCase[i][j].valeur == 0 && grilleCase[i][j].candidats[valeur] == true) {
                retirerCandidats(&grilleCase[i][j], valeur);
            }
        }
    }
}




/*
void paireNues (tCase2 *Case){
    //regarde si il y a une paire
    int compt = 0;
    for (int i = 0; i < TAILLE; i++)
    {
        if (Case->candidats == true)
        {
            compt++;
        }
        
    }
    //rajouter le fait qu'il faut stocker qu'il y une 2 paire identiaues
    //rajouter que si compt > 2 ba on enleve du compteur de paire
    //supprimer cette paire du bloc, de la lignes ou de la colonne
    

}



*/

void afficherStats(int *nbCasesVides,int *nbCasesVidesAvantSolver,int *nombreDeCandidats, int *nombreDeCandidatsAvantSolver,char *nomFichier){
    float tauxDeRemplissage = ((((float)*nbCasesVides) /  *nbCasesVidesAvantSolver) * 100);
    float pourcentageElemination = ((((float)*nombreDeCandidats) /  *nombreDeCandidatsAvantSolver) * 100);
    printf("*********   RESULTATS POUR %s  ***********\n",nomFichier);
    printf("nombres de cases remplies = %d sur %d    Taux de remplissage = %.3f%% \n ",*nbCasesVides,*nbCasesVidesAvantSolver , tauxDeRemplissage);
    printf("nombres de candidats éliminé = %d sur %d     pourcentage de elimination =  %.3f%% \n",*nombreDeCandidats,*nombreDeCandidatsAvantSolver,pourcentageElemination);
}

void chargerGrille(tGrille g,char *nomFichier) {
    FILE *f;
    printf("Nom du fichier ?\n");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");

    if (f == NULL) {
        printf("\nERREUR sur le fichier %s\n", nomFichier);
    } else {
        fread(g, sizeof(int), TAILLE * TAILLE, f);
    }
    fclose(f);
}

void affichageGrille(tGrille grille) {
    int i, j, compteur1, compteur2 = 1;
    printf("  ");
    for (compteur1 = 1; compteur1 < TAILLE + 1; compteur1++) {
        printf("%3d", compteur1);
        if (compteur1 == 6 || compteur1 == 3) {
            printf(" ");
        }
    }
    printf("\n  +---------+---------+---------+\n");
    for (i = 0; i < TAILLE; i++) {
        printf("%d |", compteur2);
        for (j = 0; j < TAILLE; j++) {
            if (grille[i][j] == 0) {
                printf(" . ");
            } else {
                printf("%2d ", grille[i][j]);
            }
            if (j == 2 || j == 5) {
                printf("|");
            }
        }
        printf("|\n");
        compteur2++;
        if (compteur2 == 4 || compteur2 == 7) {
            printf("  +---------+---------+---------+\n");
        }
    }
    printf("  +---------+---------+---------+\n");
}