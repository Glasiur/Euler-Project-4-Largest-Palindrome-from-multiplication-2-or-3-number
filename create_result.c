#include <stdio.h>
#include <stdlib.h>

int is_palindrome(int number) {
    int original = number;
    int reversed_number = 0;
    while (number >= 1) {
        reversed_number *= 10;
        reversed_number += number % 10;
        number = number / 10;
    }
    return reversed_number == original;
}

int research_brute_force_three_factors(int lim, int max_factor, int *iteration_count) {
    int max_pal = 0;
    for (int i = max_factor; i >= 1 && i * max_factor * max_factor > max_pal; i--) {
        (*iteration_count)++;
        for (int j = max_factor; j >= i; j--) {
            (*iteration_count)++;
            for (int k = max_factor; k >= j; k--) {
                (*iteration_count)++;
                int product = i * j * k;
                if (product > max_pal && product < lim && is_palindrome(product)) {
                    max_pal = product;
                }
            }
        }
    }
    return max_pal;
}

int main(void) {
    int max_factor = 99;
    int iteration_count = 0;
    int pal;

    // Ouverture du fichier en écriture
    FILE *fp = fopen("3_factor_res", "w");
    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    for (int lim = max_factor * max_factor * max_factor + 1; lim >= 0; lim--) {
        pal = research_brute_force_three_factors(lim, max_factor, &iteration_count);
        fprintf(fp, "%d %d %d\n", lim, max_factor, pal);  // Écriture dans le fichier
    }

    fclose(fp);  // Fermeture du fichier
    return 0;
}