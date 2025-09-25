#include "pattern_matching.h"
#include <string.h>
#include <stdlib.h>

// Convertit un caractère (a-z ou espace) en indice (a=0, b=1, ..., z=25, espace=26)
int char_to_index(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    }
    if (c == ' ') {
        return 26;
    }
    return -1; // Caractère invalide
}

// Algorithme naïf de recherche de motifs
void naive_pattern_matching(char* text, char* pattern, int* matches, int* match_count) {
    int n = strlen(text);
    int m = strlen(pattern);
    *match_count = 0;

    // Vérifier si les longueurs sont valides
    if (m == 0 || n < m) {
        return;
    }

    // Parcourir le texte avec une fenêtre de taille m
    for (int i = 0; i <= n - m; i++) {
        int j;
        // Comparer le motif avec la fenêtre courante
        for (j = 0; j < m; j++) {
            if (text[i + j] != pattern[j]) {
                break;
            }
        }
        // Si toute la fenêtre correspond au motif
        if (j == m) {
            matches[*match_count] = i;
            (*match_count)++;
        }
    }
}