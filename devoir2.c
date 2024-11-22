#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#define MODE_BINAIRE 0
#define MODE_TEXTE 1

// Fonction pour afficher une matrice
void afficher_matrice(int *matrice, int dimension) 
{
 int i,j;
    for (i = 0; i < dimension; i++) 
   {
        for (j = 0; j < dimension; j++) 
        {
            printf("%d ", matrice[i * dimension + j]);
        }
        printf("\n");
   }
}
// Fonction pour créer une matrice aléatoire
void creer_matrice(int *matrice, int dimension) 
{
    int i;
    srand(time(NULL)); 
    for (i = 0; i < dimension * dimension; i++)
    {
        matrice[i] = rand() % 100; // Valeurs aléatoires entre 0 et 99
    }
}
// Fonction pour enregistrer une matrice
void enregistrer_matrice(const char *nom_fichier, int *matrice, int dimension, int mode)
 {
    int fichier;
    int i, j;
    
    // Mode binaire
if (mode == MODE_BINAIRE) 
    {
        fichier = open(nom_fichier, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fichier == -1) 
          {
                perror("Erreur lors de la création du fichier binaire");
                exit(EXIT_FAILURE);
          }
        if (write(fichier, matrice, sizeof(int) * dimension * dimension) == -1) 
           {
                 perror("Erreur lors de l'écriture dans le fichier binaire");
                 close(fichier);
                 exit(EXIT_FAILURE);
           }
     }
    // Mode texte
else if (mode == MODE_TEXTE) 
{
     fichier = open(nom_fichier, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
     if (fichier == -1) 
              {
                  perror("Erreur lors de la création du fichier texte");
                  exit(EXIT_FAILURE);
               }
for (i = 0; i < dimension; i++) 
     {
              for (j = 0; j < dimension; j++)
               {
                 char buffer[12];
                 sprintf(buffer, "%d ", matrice[i * dimension + j]);
                 if (write(fichier, buffer, strlen(buffer)) == -1) 
                  {
                    perror("Erreur lors de l'écriture dans le fichier texte");
                    close(fichier);
                    exit(EXIT_FAILURE);
                  }
               }
         if (write(fichier, "\n", 1) == -1) 
                     {
                         perror("Erreur lors de l'écriture de la fin de ligne");
                         close(fichier);
                        exit(EXIT_FAILURE);
                     }
      }
    }
    close(fichier);
}
// Fonction pour lire une matrice
void lire_matrice(const char *nom_fichier, int *matrice, int dimension, int mode) {
    int fichier;
    int index;
    // Mode binaire
    if (mode == MODE_BINAIRE) 
       {
           fichier = open(nom_fichier, O_RDONLY);
           if (fichier == -1) {
                               perror("Erreur lors de l'ouverture du fichier binaire");
                               exit(EXIT_FAILURE);
                              }
            if (read(fichier, matrice, sizeof(int) * dimension * dimension) == -1) 
                        {
                            perror("Erreur lors de la lecture du fichier binaire");
                            close(fichier);
                            exit(EXIT_FAILURE);
                         }
       }
    // Mode texte
    else if (mode == MODE_TEXTE) 
     {
                 fichier = open(nom_fichier, O_RDONLY);
                  if (fichier == -1) 
                    {
                      perror("Erreur lors de l'ouverture du fichier texte");
                      exit(EXIT_FAILURE);
                    }
        char buffer[12];
        index = 0;
     while (read(fichier, buffer, sizeof(char)) > 0)
          {
             if (sscanf(buffer, "%d", &matrice[index]) == 1)  
                {
                   index++;
                 }
          }
     }
    close(fichier);
}

// Fonction principale
int main(int argc, char *argv[])
{
    int dimension = 0;
    int mode = MODE_BINAIRE; // Par défaut : mode binaire
    int *matrice = NULL;
    char *nom_fichier = NULL;
    int creer = 0, afficher = 0;
    int i;
// Lecture des arguments
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            creer = 1;
        } else if (strcmp(argv[i], "-a") == 0) {
            afficher = 1;
        } else if (strcmp(argv[i], "-d") == 0) {
            if (i + 1 < argc) {
                dimension = atoi(argv[++i]);
            }
     } else if (strcmp(argv[i], "-b") == 0) {
            mode = MODE_BINAIRE;
        } else if (strcmp(argv[i], "-t") == 0) {
            mode = MODE_TEXTE;
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                nom_fichier = argv[++i];
            }
        }
    }
// Validation des paramètres
    if (dimension <= 0 || nom_fichier == NULL) 
    {
        fprintf(stderr, "Usage : %s [-c | -a] -d dimension -f fichier [-b | -t]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Allocation de la matrice
    matrice = (int *)malloc(sizeof(int) * dimension * dimension);
    if (matrice == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }
// Création de la matrice
    if (creer) 
     {
        creer_matrice(matrice, dimension);
        enregistrer_matrice(nom_fichier, matrice, dimension, mode);
        printf("Matrice créée et enregistrée dans le fichier '%s'.\n", nom_fichier);
     }

    // Lecture et affichage de la matrice
    if (afficher) 
     {
        lire_matrice(nom_fichier, matrice, dimension, mode);
        printf("Matrice lue depuis le fichier '%s' :\n", nom_fichier);
        afficher_matrice(matrice, dimension);
     }

    // Libération de la mémoire
    free(matrice);
    return 0;
}