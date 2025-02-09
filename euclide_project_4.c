#include <stdio.h>
#include <math.h>

#define ENABLE_DEBUG_LOGS 1
#define ENABLE_RESULT_LOGS 1


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

int iterate_positive_3_factors(int lim,int i,int j,int min_k_increment,int *max_pal,int *iteration_count,int max_factor){
    for(int k=min_k_increment; k<=max_factor && i*j*k<lim ; k++){
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


int iterate_negative_3_factors(int lim,int i,int j,int max_k_decrement,int *max_pal,int *iteration_count,int max_factor){
    for(int k=max_k_decrement; k*i*j>*max_pal && k>=max_factor; k--){
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
    log_result("%d best palindrome for lim=%d in research1\n",max_pal,lim);
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
    log_result("%d best palindrome for lim=%d in research2\n",max_pal,lim);
    return max_pal;
}

int main(void){
    int c=0;
    research_optimized_two_factors(10000,99,&c);
    c=0;
    research_optimized_three_factors(400000,99,&c);
    c=0;
    research_optimized_two_factors(10000,99,&c);
    return 0;
}