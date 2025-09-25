#include "pattern_matching.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Fonction pour lire une chaîne depuis un fichier
int read_from_file(char* filename, char* buffer, int max_len) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return 0;
    }
    if (fgets(buffer, max_len, file) == NULL) {
        printf("Erreur : fichier vide ou erreur de lecture\n");
        fclose(file);
        return 0;
    }
    // Supprimer le caractère de nouvelle ligne si présent
    buffer[strcspn(buffer, "\n")] = '\0';
    fclose(file);
    return 1;
}

// Fonction pour créer un fichier de test
int create_test_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Erreur : impossible de créer le fichier %s\n", filename);
        return 0;
    }
    fprintf(file, "%s", content);
    fclose(file);
    return 1;
}

// Fonction pour valider que le motif contient uniquement a-z ou espaces
int validate_pattern(const char* pattern) {
    for (int i = 0; pattern[i] != '\0'; i++) {
        if (!((pattern[i] >= 'a' && pattern[i] <= 'z') || pattern[i] == ' ')) {
            return 0;
        }
    }
    return 1;
}

// Fonction pour afficher les résultats
void print_matches(int* matches, int match_count, const char* algorithm) {
    printf("Résultats pour %s :\n", algorithm);
    if (match_count == 0) {
        printf("  Aucun motif trouvé.\n");
    } else {
        printf("  Motif trouvé à %d position(s) : ", match_count);
        for (int i = 0; i < match_count; i++) {
            printf("%d ", matches[i]);
        }
        printf("\n");
    }
}

// Fonction pour exécuter une recherche
void run_search(const char* text, const char* pattern, int* matches, int* match_count) {
    // Vérifier que le texte et le motif ne sont pas vides
    if (strlen(text) == 0 || strlen(pattern) == 0) {
        printf("Erreur : texte ou motif vide.\n");
        return;
    }

    // Valider le motif
    if (!validate_pattern(pattern)) {
        printf("Erreur : le motif doit contenir uniquement des lettres minuscules (a-z) ou des espaces.\n");
        return;
    }

    // Test de l'algorithme naïf
    naive_pattern_matching((char*)text, (char*)pattern, matches, match_count);
    print_matches(matches, *match_count, "l'algorithme naïf");

    // Test de l'algorithme AFD
    AFD* afd = create_afd((char*)pattern);
    if (afd) {
        afd_pattern_matching((char*)text, afd, matches, match_count);
        print_matches(matches, *match_count, "l'algorithme AFD");
        free_afd(afd);
    } else {
        printf("Erreur : échec de la création de l'AFD.\n");
    }
}

// Fonction pour exécuter les scénarios de test
void run_test_scenarios(int* matches, int* match_count) {
    char text[MAX_TEXT_LENGTH];
    char pattern[MAX_PATTERN_LENGTH];

    // Scénario 1 : Texte et motif prédéfinis (simule entrée clavier)
    printf("\n--- Scénario 1 : Texte et motif prédéfinis ---\n");
    strcpy(text, "klklfdjjkdj fmmfm");
    strcpy(pattern, "j f");
    printf("Texte : %s\n", text);
    printf("Motif : %s\n", pattern);
    run_search(text, pattern, matches, match_count);

    // Scénario 2 : Texte depuis un fichier, motif prédéfini
    printf("\n--- Scénario 2 : Texte depuis un fichier, motif prédéfini ---\n");
    if (create_test_file("text.txt", "abac aba with spaces")) {
        if (read_from_file("text.txt", text, MAX_TEXT_LENGTH)) {
            strcpy(pattern, "aba");
            printf("Texte (de text.txt) : %s\n", text);
            printf("Motif : %s\n", pattern);
            run_search(text, pattern, matches, match_count);
        }
    }

    // Scénario 3 : Texte et motif depuis des fichiers
    printf("\n--- Scénario 3 : Texte et motif depuis des fichiers ---\n");
    if (create_test_file("text.txt", "hello world this is a test") &&
        create_test_file("pattern.txt", "is a")) {
        if (read_from_file("text.txt", text, MAX_TEXT_LENGTH) &&
            read_from_file("pattern.txt", pattern, MAX_PATTERN_LENGTH)) {
            printf("Texte (de text.txt) : %s\n", text);
            printf("Motif (de pattern.txt) : %s\n", pattern);
            run_search(text, pattern, matches, match_count);
        }
    }

    // Scénario 4 : Motif invalide
    printf("\n--- Scénario 4 : Motif invalide ---\n");
    strcpy(text, "klklfdjjkdj fmmfm");
    strcpy(pattern, "j F");
    printf("Texte : %s\n", text);
    printf("Motif : %s\n", pattern);
    run_search(text, pattern, matches, match_count);
}

// Fonction pour afficher le menu et lire le choix
int display_menu() {
    int choice;
    printf("\nRecherche de motifs\n");
    printf("1. Texte et motif depuis le clavier\n");
    printf("2. Texte depuis un fichier, motif depuis le clavier\n");
    printf("3. Texte et motif depuis des fichiers\n");
    printf("4. Quitter\n");
    printf("5. Exécuter les scénarios de test\n");
    printf("Choisissez une option (1, 2, 3, 4 ou 5) : ");
    scanf("%d", &choice);
    getchar(); // Consommer le '\n'
    return choice;
}

int main() {
    char text[MAX_TEXT_LENGTH];
    char pattern[MAX_PATTERN_LENGTH];
    char filename[100];
    int choice;

    // Allouer un tableau pour stocker les positions des occurrences
    int* matches = (int*)malloc(MAX_TEXT_LENGTH * sizeof(int));
    int match_count;

    while (1) {
        choice = display_menu();

        if (choice == 4) {
            printf("Programme terminé.\n");
            break;
        }

        if (choice == 5) {
            // Option 5 : Exécuter les scénarios de test
            run_test_scenarios(matches, &match_count);
            continue;
        }

        if (choice == 1) {
            // Option 1 : Texte et motif depuis le clavier
            printf("Entrez le texte (max %d caractères) : ", MAX_TEXT_LENGTH - 1);
            fgets(text, MAX_TEXT_LENGTH, stdin);
            text[strcspn(text, "\n")] = '\0';

            printf("Entrez le motif (max %d caractères, a-z ou espace) : ", MAX_PATTERN_LENGTH - 1);
            fgets(pattern, MAX_PATTERN_LENGTH, stdin);
            pattern[strcspn(pattern, "\n")] = '\0';
        } else if (choice == 2) {
            // Option 2 : Texte depuis un fichier, motif depuis le clavier
            printf("Entrez le nom du fichier pour le texte : ");
            fgets(filename, 100, stdin);
            filename[strcspn(filename, "\n")] = '\0';
            if (!read_from_file(filename, text, MAX_TEXT_LENGTH)) {
                continue; // Retourner au menu
            }

            printf("Entrez le motif (max %d caractères, a-z ou espace) : ", MAX_PATTERN_LENGTH - 1);
            fgets(pattern, MAX_PATTERN_LENGTH, stdin);
            pattern[strcspn(pattern, "\n")] = '\0';
        } else if (choice == 3) {
            // Option 3 : Texte et motif depuis des fichiers
            printf("Entrez le nom du fichier pour le texte : ");
            fgets(filename, 100, stdin);
            filename[strcspn(filename, "\n")] = '\0';
            if (!read_from_file(filename, text, MAX_TEXT_LENGTH)) {
                continue; // Retourner au menu
            }

            printf("Entrez le nom du fichier pour le motif : ");
            fgets(filename, 100, stdin);
            filename[strcspn(filename, "\n")] = '\0';
            if (!read_from_file(filename, pattern, MAX_PATTERN_LENGTH)) {
                continue; // Retourner au menu
            }
        } else {
            printf("Choix invalide.\n");
            continue; // Retourner au menu
        }

        run_search(text, pattern, matches, &match_count);
    }

    // Libérer la mémoire
    free(matches);

    return 0;
}