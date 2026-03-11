#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "pal_3_factor_test.h"
//#include "pal_3_factor.h"


#define ENABLE_STATISTIC_LOGS 0
#define ENABLE_TEST_LOGS 1


#if ENABLE_STATISTIC_LOGS
#define log_statistic(...) printf(__VA_ARGS__)
#else
#define log_statistic(...)
#endif

#if ENABLE_TEST_LOGS
#define log_test(...) printf(__VA_ARGS__)
#else
#define log_test(...)
#endif



//_________________________________________________________________________________________________

int lire_prochain_palindrome(FILE *fp, int *lim, int *max_factor, int *res) {
    if (fp == NULL) return 0;
    char ligne[256];
    if (fgets(ligne, sizeof(ligne), fp) != NULL) {
        // Extraction des trois entiers
        if (sscanf(ligne, "%d %d %d", lim, max_factor, res) == 3) {
            return 1;
        }
    }
    return 0;
}

void run_tests_multithread() {
    FILE *fp = fopen("3_factor_res", "r");
    if (!fp) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    // Première passe : compter le nombre de lignes
    int lim, maxf, res;
    int total_entries = 0;
    while (lire_prochain_palindrome(fp, &lim, &maxf, &res)) {
        total_entries++;
    }

    if (total_entries == 0) {
        printf("Aucune entrée trouvée dans le fichier.\n");
        fclose(fp);
        return;
    }

    // Allocation des tableaux pour stocker les trois colonnes
    int *lim_array   = malloc(total_entries * sizeof(int));
    int *maxf_array  = malloc(total_entries * sizeof(int));
    int *res_array   = malloc(total_entries * sizeof(int));
    if (!lim_array || !maxf_array || !res_array) {
        perror("Erreur d'allocation mémoire");
        free(lim_array);
        free(maxf_array);
        free(res_array);
        fclose(fp);
        return;
    }

    // Seconde lecture : remplissage des tableaux
    rewind(fp);
    int idx = 0;
    while (lire_prochain_palindrome(fp, &lim_array[idx], &maxf_array[idx], &res_array[idx]) && idx < total_entries) {
        idx++;
    }
    fclose(fp);

    // Variables globales pour les résultats
    int global_max_iter = 0;
    int global_worst_lim = 0;      // limite correspondant au max d'itérations
    int global_error_count = 0;
    unsigned long long global_total_iter = 0;

    // Région parallèle OpenMP
    #pragma omp parallel
    {
        int local_max_iter = 0;
        int local_worst_lim = 0;
        int local_error_count = 0;
        unsigned long long local_total_iter = 0;

        // Distribution automatique des indices
        #pragma omp for nowait
        for (int i = 0; i < total_entries; i++) {
            int current_iter = 0;
            int found = research_optimized_three_factors_3(lim_array[i], maxf_array[i], &current_iter);

            if (found != res_array[i]) {
                local_error_count++;
            }

            if (current_iter > local_max_iter) {
                local_max_iter = current_iter;
                local_worst_lim = lim_array[i];
            }

            local_total_iter += current_iter;
        }

        // Mise à jour globale en section critique
        #pragma omp critical
        {
            global_error_count += local_error_count;
            global_total_iter += local_total_iter;
            if (local_max_iter > global_max_iter) {
                global_max_iter = local_max_iter;
                global_worst_lim = local_worst_lim;
            }
        }
    }

    // Affichage du résumé (adapté : max_factor variable, donc on ne l'affiche pas dans le libellé)
    log_test("pal_from_3_factor (max_factor variable) max_ite=%d worst_lim=%d moy_ite=%llu error=%d\n", global_max_iter, global_worst_lim,
             global_total_iter / total_entries, global_error_count);

    free(lim_array);
    free(maxf_array);
    free(res_array);
}

void run_test_monothread(){
    int max_iteration_count=0;
    int worst_lim=0;
    int error_count=0;
    unsigned long long int total_iterations=0;
    int cpt_test=0;
    int max_factor;
    int current_iterations;

    FILE *fp = fopen("3_factor_res", "r");
    if (fp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return ;
    }

    int lim, res;
    while (lire_prochain_palindrome(fp, &lim,&max_factor,&res)) {
        cpt_test++;
        current_iterations=0;
        error_count+=research_optimized_three_factors_3(lim, max_factor, &current_iterations)!=res;
        if(current_iterations>max_iteration_count){
            max_iteration_count=current_iterations;
            worst_lim=lim;
        }
        total_iterations+=current_iterations;
        
        log_statistic(",%d,%d,%d,%d,\n",lim,palindrome,current_iterations,adjusted_cube_root(lim,max_factor));//cube or square
    }
    log_test("pal_from_%d_factor max_factor=%d max_ite=%d worst_lim=%d moy_ite=%llu error=%d\n",
        3,max_factor,max_iteration_count,worst_lim,total_iterations/(cpt_test),error_count);
    fclose(fp);
}

void run_palindrome_tests(int multi_threading){
    multi_threading ? run_tests_multithread() : run_test_monothread() ;
}


int main(){
    run_palindrome_tests(1);//multi_threading
    return 0;
}