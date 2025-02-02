#include <stdio.h>
#include <math.h>
#include <omp.h>

#define ENABLE_DEBUG_LOGS 0
#define ENABLE_RESULT_LOGS 0


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

int iterate_positive_3_factors(int lim,int i,int j,int min_k_increment,int *max_pal,int enforce_order,int *iteration_count,int max_factor){
    for(int k=min_k_increment;k<=max_factor&&i*j*k<lim&&(enforce_order?k<=j:1);k++){
        (*iteration_count)++;
        int product=i*j*k;
        if(product<=*max_pal)
            min_k_increment=k;
        else if(is_palindrome(product)){
            log_debug("%di %dj %dk %dproduct %diteration_count ++ \n",i,j,k,product,*iteration_count);
            *max_pal=product;
        }
    }
    return min_k_increment;
}


int iterate_negative_3_factors(int lim,int i,int j,int max_k_decrement,int *max_pal,int enforce_order,int *iteration_count,int max_factor){
    for(int k=max_k_decrement;k*i*j>*max_pal&&(enforce_order?k>=max_factor:1);k--){
        (*iteration_count)++;
        int product=k*i*j;
        if(product>=lim)
            max_k_decrement=k;
        else if(is_palindrome(product)){
            log_debug("%di %dj %dk %dproduct %diteration_count -- \n",i,j,k,product,*iteration_count);
            *max_pal=product;
        }
    }
    return max_k_decrement;
}

int adjusted_square_root(int lim,int max_factor){
    int root=sqrt(lim);
    if(root+1>max_factor) return max_factor;
    else if(root*root==lim) return root;
    return root+1;
}

int adjusted_cube_root(int lim,int max_factor){
    int root=cbrt(lim);
    if(root+1>max_factor) return max_factor;
    else if(root*root*root==lim) return root;
    return root+1;
}

int research1_optimized_two_factors(int lim,int *iteration_count){
    int max_pal=0;
    int max_factor=99;
    int sqrt_lim=adjusted_square_root(lim,max_factor);
    int min_k_increment=sqrt_lim;
    for(int j=sqrt_lim;j*max_factor>max_pal;j--){
        (*iteration_count)++;
        min_k_increment=iterate_positive_3_factors(lim,1,j,min_k_increment,&max_pal,0,iteration_count,max_factor);
        iterate_negative_3_factors(lim,1,j,sqrt_lim,&max_pal,1,iteration_count,j);
    }
    log_debug("%diteration_count1 \n",*iteration_count);
    log_result("%d best palindrome for lim=%d in research1\n",max_pal,lim);
    return max_pal;
}

int research2_optimized_three_factors(int lim,int *iteration_count){
    int max_pal=0;
    int max_factor=99;
    int sbrt_lim=adjusted_cube_root(lim,max_factor);
    int j_min_increment=sbrt_lim;
    for(int i=max_factor;i*max_factor*max_factor>max_pal;i--){
        (*iteration_count)++;
        int k_max_decrement=j_min_increment;
        for(int j=j_min_increment;j<=max_factor&&i*j*sbrt_lim<lim;j++){
            (*iteration_count)++;
            if(i*j*j<lim) k_max_decrement=j;
            if(i*j*j<max_pal) j_min_increment=j;
            else k_max_decrement=iterate_negative_3_factors(lim,i,j,k_max_decrement,&max_pal,1,iteration_count,sbrt_lim);
        }
        int min_k_increment=sbrt_lim;
        for(int j=sbrt_lim;j>=i&&i*j*max_factor>max_pal;j--){
            (*iteration_count)++;
            min_k_increment=iterate_positive_3_factors(lim,i,j,min_k_increment,&max_pal,0,iteration_count,max_factor);
            iterate_negative_3_factors(lim,i,j,sbrt_lim,&max_pal,1,iteration_count,j);
        }
    }
    log_debug("%diteration_count2 \n",*iteration_count);
    log_result("%d best palindrome for lim=%d in research2\n",max_pal,lim);
    return max_pal;
}

int research3_optimized_large_two_factors(int lim,int *iteration_count){
    int max_pal=0;
    int max_factor=999;
    int sqrt_lim=adjusted_square_root(lim,max_factor);
    int min_k_increment=sqrt_lim;
    for(int j=sqrt_lim;j*max_factor>max_pal;j--){
        (*iteration_count)++;
        min_k_increment=iterate_positive_3_factors(lim,1,j,min_k_increment,&max_pal,0,iteration_count,max_factor);
        iterate_negative_3_factors(lim,1,j,sqrt_lim,&max_pal,1,iteration_count,j);
    }
    log_debug("%diteration_count3 \n",*iteration_count);
    log_result("%d best palindrome for lim=%d in research3\n",max_pal,lim);
    return max_pal;
}

int research1_brute_force_two_factors(int lim,int *iteration_count){
    int max_pal=0;
    int max_factor=99;
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

int research2_brute_force_three_factors(int lim,int *iteration_count){
    int max_pal=0;
    int max_factor=99;
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

int research3_brute_force_large_two_factors(int lim,int *iteration_count){
    int max_pal=0;
    int max_factor=999;
    for(int i=max_factor;i>=1&&i*max_factor>max_pal;i--){
        (*iteration_count)++;
        for(int j=max_factor;j>=i;j--){
            (*iteration_count)++;
            int product=i*j;
            if(product>max_pal&&product<lim&&is_palindrome(product)){
                log_debug("%di %dj %dproduct 3 \n",i,j,product);
                max_pal=product;
            }
        }
    }
    return max_pal;
}

int get_palindrome_candidate(int lim,int search_strategy_id,int *iteration_count){
    int palindrome=0;
    switch(search_strategy_id){
        case 1:palindrome=research1_optimized_two_factors(lim,iteration_count);break;
        case 2:palindrome=research2_optimized_three_factors(lim,iteration_count);break;
        case 3:palindrome=research3_optimized_large_two_factors(lim,iteration_count);break;
        case 4:palindrome=research1_brute_force_two_factors(lim,iteration_count);break;
        case 5:palindrome=research2_brute_force_three_factors(lim,iteration_count);break;
        case 6:palindrome=research3_brute_force_large_two_factors(lim,iteration_count);break;
    }
    return palindrome;
}

void run_palindrome_tests(int lim,int search_strategy_id){
    int max_iteration_count=0;
    int max_lim=0;
    int error_count=0;
    unsigned long long int total_iterations=0;

    #pragma omp parallel
    {
        int local_max_iterations=0;
        int local_max_limit=0;
        int local_errors=0;
        unsigned long long int local_total_iterations=0;

        #pragma omp for nowait
        for(int x=lim;x>=0;x--){
            int current_iterations=0;
            int palindrome=get_palindrome_candidate(x,search_strategy_id,&current_iterations);
            int test_iterations=0;
            int test_palindrome=get_palindrome_candidate(x,search_strategy_id+3,&test_iterations);
            
            if(palindrome!=test_palindrome){
                local_errors++;
            }
            if(current_iterations>local_max_iterations){
                local_max_iterations=current_iterations;
                local_max_limit=x;
            }
            local_total_iterations+=current_iterations;
        }

        #pragma omp critical
        {
            if(local_max_iterations>max_iteration_count){
                max_iteration_count=local_max_iterations;
                max_lim=local_max_limit;
            }
            error_count+=local_errors;
            total_iterations+=local_total_iterations;
        }
    }

    printf("research%d max_ite=%d max_lim=%d moy_ite=%llu error=%d\n",
           search_strategy_id,max_iteration_count,max_lim,total_iterations/(lim+1),error_count);
}

int main(){
    run_palindrome_tests(9801,1);
    run_palindrome_tests(970299,2);
    run_palindrome_tests(998001,3);
    return 0;
}