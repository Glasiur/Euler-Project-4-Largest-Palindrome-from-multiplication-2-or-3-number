#include <stdio.h>
#include <math.h>
#include <omp.h>

#define ENABLE_DEBUG_LOGS 0
#define ENABLE_RESULT_LOGS 0
#define ENABLE_STATISTIC_LOGS 1
#define ENABLE_TEST_LOGS 0

#if ENABLE_DEBUG_LOGS
#define log_debug(...) printf(__VA_ARGS__)
#else
#define log_debug(...)
#endif

#if ENABLE_RESULT_LOGS
#define log_result(...) printf(__VA_ARGS__)
#else
#define log_result(...)
#endif

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


//__________________________________your function____________________________________________________________//modifié pour le test
int is_palidrome_(int x){
    int X=x;//sauvgartde de la valeur
    int res=0;
    while (x>=1){//creation du miroir
        res*=10;
        res+=x%10;
        x=x/10;
    }
    return res==X;
}
void boucle_test_positif_3_menbre(int N, int i, int j, int k, int *max_P,int option,int *c,int lim){
    for (k; k<lim && i*j*k<N  && (option ? k<=j : 1);k++){// le test optionelle permet d'eviter la commutativité dans les bonne condition
        *c+=1;
        int X=i*j*k;
        if (is_palidrome_(X) && X>*max_P && X<N){ //test le candidat 
            log_debug("%di %dj %dk %dproduct %diteration_count ++ \n",i,j,k,X,*c);
            log_statistic(";%d",X);
            *max_P=X;
        }
    }
    return ;
}

void boucle_test_negatif_3_menbre(int N, int i, int j,int k  , int *max_P,int option,int *c){
    for(k; k*i*j>*max_P && (option ? k>=j : 1) ;k--){// le test optionelle permet d'eviter la commutativité dans les bonne condition
        *c+=1;
        int X=k*i*j;
        if (is_palidrome_(X) && X>*max_P && X<N){
            log_debug("%di %dj %dk %dproduct %diteration_count -- \n",i,j,k,X,*x);
            log_statistic(";%d",X);
            *max_P=X;
        }
    }
    return ;
}
int rc2(int N, int lim){//racine caré
    int res=sqrt(N); //recupére la racine carré de N
    if (res+1>lim)
        return lim;
    else if (res*res==N ) 
        return res;
    return res+1;
}
int rc3(int N,int lim){//racine cubique
    int res=cbrt(N);
    if (res+1>lim)//evite de depassr les limites donnée en exercice
        return lim;
    else if (res*res==N ) 
        return res;
    return res+1;
}

int research_2_factor(int N,int lim,int *c){
    int max_P=0;//meilleur palindrome trouve
    int sqrt_N=rc2(N,lim);//recupre la racine de N
    for (int i=sqrt_N; i*lim>max_P; i--){
        *c+=1;
        boucle_test_positif_3_menbre(N,1,i,sqrt_N,&max_P,0,c,lim);//test les palindrome avec une augmentation de j
        boucle_test_negatif_3_menbre(N,1,i,sqrt_N,&max_P,1,c);//test les palindrome avec une diminution de j
    }
    log_debug("%diteration_count1 \n",*c);
    log_result("%d best palindrome for lim=%d in research_2_factor\n",max_P,N);
    return max_P;
}

int research_3_factor(int N,int lim,int *c){
    int max_P=0;//equivalent a la fonction recherche 1
    int sqrt_N=rc3(N,lim);//equivalent a la fonction recherche 1
    for (int i=sqrt_N; i*lim*lim>max_P ; i--){//decroit le nombre i jusque ce qu'il soit impossible de trouver un meilleur palidrome
        *c+=1;
        for(int j=sqrt_N; j<lim && i*j*sqrt_N<N ; j++ ){// augmente le nombre j jusque de manbiére a rrester dans les limites de la recherches et de N
            *c+=1;
            boucle_test_positif_3_menbre(N,i,j,sqrt_N,&max_P,1,c,lim);//test les palindromes avec une augmentation de k
            //c+=boucle_test_negatif_3_menbre(N,i,j,sqrt_N,&max_P,0);
        }
        for (int j=sqrt_N; i*j*sqrt_N>max_P ;j-- ){// diminue le nombre j jusque il soit imposible de trouver un mailleur palindrome
            *c+=1;    
            //c+=boucle_test_positif_3_menbre(N,i,j,sqrt_N,&max_P,0,lim);
            boucle_test_negatif_3_menbre(N,i,j,sqrt_N,&max_P,1,c);//test les palindromes avec une diminution de k
        }
    }
    log_debug("%diteration_count2 \n",*iteration_count);
    log_result("%d best palindrome for lim=%d in research_2_factor\n",max_pal,lim);
    return max_P;
}

//________________________my_verificated_function________________________________________________

int is_palindrome(int number){
    int original=number;
    int reversed_number=0;
    while(number>=1){
        reversed_number*=10;
        reversed_number+=number%10;
        number=number/10;
    }
    return reversed_number==original;
}

int iterate_positive_3_factors(int lim,int i,int j,int min_k_increment,int *max_pal,int *iteration_count,int max_factor){
    for(int k=min_k_increment; k<=max_factor && i*j*k<lim ; k++){
        (*iteration_count)++;
        int product=i*j*k;
        if(product<=*max_pal)
            min_k_increment=k;
        else if(is_palindrome(product)){
            *max_pal=product;
        }
    }
    return min_k_increment;
}


int iterate_negative_3_factors(int lim,int i,int j,int max_k_decrement,int *max_pal,int *iteration_count,int max_factor){
    for(int k=max_k_decrement; k*i*j>*max_pal && k>=max_factor; k--){
        (*iteration_count)++;
        int product=k*i*j;
        if(product>=lim)
            max_k_decrement=k;
        else if(is_palindrome(product)){
            *max_pal=product;
        }
    }
    return max_k_decrement;
}

int adjusted_square_root(int lim,int max_factor){
    int root=sqrt(lim);
    if(root>max_factor) return max_factor;
    else if(root*root==lim) return root;
    return root+1;
}

int adjusted_cube_root(int lim,int max_factor){
    int root=cbrt(lim);
    if(root>max_factor) return max_factor;
    else if(root*root*root==lim) return root;
    return root+1;
}

int research_optimized_two_factors(int lim,int max_factor,int *iteration_count){
    int max_pal=0;
    int sqrt_lim=adjusted_square_root(lim,max_factor);
    int min_k_increment=sqrt_lim;
    for(int j=sqrt_lim;j*max_factor>max_pal;j--){
        (*iteration_count)++;
        min_k_increment=iterate_positive_3_factors(lim,1,j,min_k_increment,&max_pal,iteration_count,max_factor);//-+
        iterate_negative_3_factors(lim,1,j,sqrt_lim,&max_pal,iteration_count,j);//--
    }
    return max_pal;
}

int research_optimized_three_factors(int lim,int max_factor,int *iteration_count){
    int max_pal=0;
    int sbrt_lim=adjusted_cube_root(lim,max_factor);
    int j_min_increment=sbrt_lim;
    for(int i=max_factor;i*max_factor*max_factor>max_pal;i--){
        (*iteration_count)++;
        int k_max_decrement=j_min_increment;
        for(int j=j_min_increment;j<=max_factor&&i*j*sbrt_lim<lim;j++){
            (*iteration_count)++;
            if(i*j*j<lim) k_max_decrement=j;
            if(i*j*j<max_pal) j_min_increment=j;
            else k_max_decrement=iterate_negative_3_factors(lim,i,j,k_max_decrement,&max_pal,iteration_count,sbrt_lim);//-++ (real -+-)
        }
        int min_k_increment=sbrt_lim;
        for(int j=sbrt_lim;j>=i&&i*j*max_factor>max_pal;j--){
            (*iteration_count)++;
            min_k_increment=iterate_positive_3_factors(lim,i,j,min_k_increment,&max_pal,iteration_count,max_factor);//--+
            iterate_negative_3_factors(lim,i,j,sbrt_lim,&max_pal,iteration_count,j);//---
        }
    }
    return max_pal;
}


//_________________________________________________________________________________________________

int get_palindrome_candidate(int lim,int max_factor,int search_strategy_id,int *iteration_count){
    int palindrome=0;
    switch(search_strategy_id){
        case 2:palindrome=research_2_factor(lim,max_factor,iteration_count);break;
        case 3:palindrome=research_3_factor(lim,max_factor,iteration_count);break;
        case 4:palindrome=research_optimized_two_factors(lim,max_factor,iteration_count);break;
        case 6:palindrome=research_optimized_three_factors(lim,max_factor,iteration_count);break;
    }
    return palindrome;
}

void run_palindrome_tests_multithread(int max_factor,int n_factor,int brute_force){
    int max_iteration_count=0;
    int worst_lim=0;
    int error_count=0;
    unsigned long long int total_iterations=0;
    int max_lim=pow(max_factor,n_factor)+1;

    #pragma omp parallel
    {
        int local_max_iterations=0;
        int local_worst_limit=0;
        int local_errors=0;
        unsigned long long int local_total_iterations=0;

        #pragma omp for nowait
        for(int lim=max_lim ;lim>=0; lim--){
            int current_iterations=0;
            int palindrome=get_palindrome_candidate(lim,max_factor,n_factor,&current_iterations);
            if (brute_force){
                int bf_iteration=0;
                int bf_palindrome=get_palindrome_candidate(lim,max_factor,n_factor*2,&bf_iteration);
                if(palindrome!=bf_palindrome){
                    local_errors++;
                }
            }
            if(current_iterations>local_max_iterations){
                local_max_iterations=current_iterations;
                local_worst_limit=lim;
            }
            local_total_iterations+=current_iterations;
            
            log_statistic(",%d,%d,%d,%d,\n",lim,palindrome,current_iterations,adjusted_cube_root(lim,max_factor));//cube or square
        }

        #pragma omp critical
        {
            if(local_max_iterations>max_iteration_count){
                max_iteration_count=local_max_iterations;
                worst_lim=local_worst_limit;
            }
            error_count+=local_errors;
            total_iterations+=local_total_iterations;
        }
    }

    log_test("pal_from_%d_factor max_factor=%d max_ite=%d worst_lim=%d moy_ite=%llu error=%d\n",
           n_factor,max_factor,max_iteration_count,worst_lim,total_iterations/(max_lim),error_count);
}
void run_palindrome_tests_monothread(int max_factor,int n_factor,int brute_force){
    int max_iteration_count=0;
    int worst_lim=0;
    int error_count=0;
    unsigned long long int total_iterations=0;
    int max_lim=pow(max_factor,n_factor)+1;

    for(int lim=max_lim ;lim>=0; lim--){
        int current_iterations=0;
        int palindrome=get_palindrome_candidate(lim,max_factor,n_factor,&current_iterations);
        if (brute_force){
            int bf_iteration=0;
            int bf_palindrome=get_palindrome_candidate(lim,max_factor,n_factor*2,&bf_iteration);
            if(palindrome!=bf_palindrome){
                error_count++;
            }
        }
        if(current_iterations>max_iteration_count){
            max_iteration_count=current_iterations;
            worst_lim=lim;
        }
        total_iterations+=current_iterations;
        
        log_statistic(",%d,%d,%d,%d,\n",lim,palindrome,current_iterations,adjusted_cube_root(lim,max_factor));//cube or square
    }
    log_test("pal_from_%d_factor max_factor=%d max_ite=%d worst_lim=%d moy_ite=%llu error=%d\n",
        n_factor,max_factor,max_iteration_count,worst_lim,total_iterations/(max_lim),error_count);
}



void run_palindrome_tests(int max_factor,int n_factor,int brute_force,int multi_threading){
    multi_threading ? run_palindrome_tests_multithread(max_factor,n_factor,brute_force) : run_palindrome_tests_monothread(max_factor,n_factor,brute_force) ;
}


int main(){
    //run_palindrome_tests(99,2,0,0);//lim,number factor,comparaison,multi_threading
    //run_palindrome_tests(99,3,0,0);
    run_palindrome_tests(999,2,0,0);
    return 0;
}