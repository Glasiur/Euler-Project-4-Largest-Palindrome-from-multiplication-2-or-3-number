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


//_______________________________________________________________________________________________________
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
            log_debug("%di %dj %dk %dproduct %diteration_count ++ \n",i,j,k,product,*iteration_count);
            log_statistic(";%d",product);
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
            log_debug("%di %dj %dk %dproduct %diteration_count -- \n",i,j,k,product,*iteration_count);
            log_statistic(";%d",product);
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
    log_debug("%diteration_count1 \n",*iteration_count);
    log_result("%d best palindrome for lim=%d in research_optimized_two_factors\n",max_pal,lim);
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
    log_debug("%diteration_count2 \n",*iteration_count);
    log_result("%d best palindrome for lim=%d in research_optimized_three_factors\n",max_pal,lim);
    return max_pal;
}
//_______________________________________________________________________________________________________

int research_brute_force_two_factors(int lim,int max_factor,int *iteration_count){
    int max_pal=0;
    for(int i=max_factor;i>=1&&i*max_factor>max_pal;i--){
        (*iteration_count)++;
        for(int j=max_factor;j>=i;j--){
            (*iteration_count)++;
            int product=i*j;
            if(product>max_pal&&product<lim&&is_palindrome(product)){
                log_debug("%di %dj %dproduct 1 \n",i,j,product);
                max_pal=product;
            }
        }
    }
    return max_pal;
}

int research_brute_force_three_factors(int lim,int max_factor,int *iteration_count){
    int max_pal=0;
    for(int i=max_factor;i>=1&&i*max_factor*max_factor>max_pal;i--){
        (*iteration_count)++;
        for(int j=max_factor;j>=i;j--){
            (*iteration_count)++;
            for(int k=max_factor;k>=j;k--){
                (*iteration_count)++;
                int product=i*j*k;
                if(product>max_pal&&product<lim&&is_palindrome(product)){
                    log_debug("%di %dj %dk %dproduct 2 \n",i,j,k,product);
                    max_pal=product;
                }
            }
        }
    }
    return max_pal;
}

//_______________________________________________________________________________________________________

int get_palindrome_candidate(int lim,int max_factor,int search_strategy_id,int *iteration_count){
    int palindrome=0;
    switch(search_strategy_id){
        case 2:palindrome=research_optimized_two_factors(lim,max_factor,iteration_count);break;
        case 3:palindrome=research_optimized_three_factors(lim,max_factor,iteration_count);break;
        case 4:palindrome=research_brute_force_two_factors(lim,max_factor,iteration_count);break;
        case 6:palindrome=research_brute_force_three_factors(lim,max_factor,iteration_count);break;
    }
    return palindrome;
}
//_______________________________________________________________________________________________________
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
}

//_______________________________________________________________________________________________________

void run_palindrome_tests(int max_factor,int n_factor,int brute_force,int multi_threading){
    multi_threading ? run_palindrome_tests_multithread(max_factor,n_factor,brute_force) : run_palindrome_tests_monothread(max_factor,n_factor,brute_force) ;
}
//_______________________________________________________________________________________________________

int main(){
    //run_palindrome_tests(99,2,0,0);//lim,number factor
    //run_palindrome_tests(99,3,0,0);
    run_palindrome_tests(999,2,0,0);
    return 0;
}