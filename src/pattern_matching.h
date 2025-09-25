#ifndef PATTERN_MATCHING_H
#define PATTERN_MATCHING_H

#define ALPHABET_SIZE 27  // Taille de l'alphabet latin minuscule {a, ..., z} + espace
#define MAX_PATTERN_LENGTH 100  // Longueur maximale du motif
#define MAX_TEXT_LENGTH 1000  // Longueur maximale du texte

// Structure pour représenter l'AFD
typedef struct {
    int state_count;  // Nombre d'états (préfixes du motif)
    int** transition; // Matrice de transition : état x caractère -> état
    int final_state;  // État final (motif complet)
} AFD;

// Prototypes des fonctions pour l'algorithme naïf
void naive_pattern_matching(char* text, char* pattern, int* matches, int* match_count);

// Prototypes des fonctions pour l'algorithme AFD
AFD* create_afd(char* pattern);
void free_afd(AFD* afd);
void afd_pattern_matching(char* text, AFD* afd, int* matches, int* match_count);

// Fonction utilitaire pour convertir un caractère en indice (a=0, b=1, ..., z=25, espace=26)
int char_to_index(char c);

#endif