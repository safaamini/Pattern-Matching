#include "pattern_matching.h"
#include <string.h>
#include <stdlib.h>

// Trouve le plus long suffixe de 'str' qui est un préfixe de 'pattern'
static int longest_prefix_suffix(char* str, int len, char* pattern, int m) {
    // Tester tous les suffixes de str en commençant par le plus long
    for (int k = len; k >= 0; k--) {
        int is_prefix = 1;
        // Vérifier si le suffixe de longueur k est un préfixe de pattern
        for (int i = 0; i < k; i++) {
            if (len - k + i >= len || str[len - k + i] != pattern[i]) {
                is_prefix = 0;
                break;
            }
        }
        if (is_prefix) {
            return k; // Longueur du plus long préfixe
        }
    }
    return 0; // Préfixe vide
}

// Crée l'AFD pour le motif donné
AFD* create_afd(char* pattern) {
    int m = strlen(pattern);
    if (m == 0 || m > MAX_PATTERN_LENGTH) {
        return NULL;
    }

    // Allouer la structure AFD
    AFD* afd = (AFD*)malloc(sizeof(AFD));
    if (!afd) return NULL;

    afd->state_count = m + 1; // Inclut le préfixe vide
    afd->final_state = m;     // État correspondant au motif complet

    // Allouer la matrice de transition
    afd->transition = (int**)malloc(afd->state_count * sizeof(int*));
    for (int i = 0; i < afd->state_count; i++) {
        afd->transition[i] = (int*)malloc(ALPHABET_SIZE * sizeof(int));
    }

    // Construire la fonction de transition
    char* temp = (char*)malloc(m + 1); // Pour construire ua
    for (int q = 0; q < afd->state_count; q++) {
        for (int a = 0; a < ALPHABET_SIZE; a++) {
            // Construire la chaîne ua (état q + caractère a)
            strncpy(temp, pattern, q);
            temp[q] = (a < 26) ? ('a' + a) : ' ';
            temp[q + 1] = '\0';
            // Calculer h(ua) : plus long suffixe de ua dans Pref(P)
            int next_state = longest_prefix_suffix(temp, q + 1, pattern, m);
            afd->transition[q][a] = next_state;
        }
    }
    free(temp);

    return afd;
}

// Libère la mémoire allouée pour l'AFD
void free_afd(AFD* afd) {
    if (!afd) return;
    for (int i = 0; i < afd->state_count; i++) {
        free(afd->transition[i]);
    }
    free(afd->transition);
    free(afd);
}

// Recherche de motifs à l'aide de l'AFD
void afd_pattern_matching(char* text, AFD* afd, int* matches, int* match_count) {
    if (!afd || !text) {
        *match_count = 0;
        return;
    }

    int n = strlen(text);
    *match_count = 0;
    int current_state = 0; // Commencer à l'état initial (préfixe vide)

    // Parcourir le texte
    for (int i = 0; i < n; i++) {
        int char_index = char_to_index(text[i]);
        if (char_index == -1) {
            current_state = 0; // Réinitialiser si caractère invalide
            continue;
        }
        // Appliquer la transition
        current_state = afd->transition[current_state][char_index];
        // Si l'état final est atteint, enregistrer une occurrence
        if (current_state == afd->final_state) {
            matches[*match_count] = i - afd->final_state + 1;
            (*match_count)++;
        }
    }
}