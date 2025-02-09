#include <stdio.h>
#include <math.h>

#define ENABLE_DEBUG_LOGS 1
#define ENABLE_RESULT_LOGS 1


#if ENABLE_DEBUG_LOGS
#define log_debug(...) printf(__VA_ARGS__)
#else
#define log_debug(...)
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

int iterate_positive_3_factors(int lim,int i,int j,int min_k_increment,int *max_pal,int max_factor){
    for(int k=min_k_increment; k<=max_factor && i*j*k<lim; k++){
        int product=i*j*k;
        if(product<=*max_pal)
            min_k_increment=k;
        else if(is_palindrome(product)){
            *max_pal=product;
        }
    }
    return min_k_increment;
}

int iterate_negative_3_factors(int lim,int i,int j,int max_k_decrement,int *max_pal,int max_factor){
    for(int k=max_k_decrement; k*i*j>*max_pal && k>=max_factor; k--){
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
    if(root>max_factor) 
        return max_factor;
    else if(root*root==lim) 
        return root;
    return root+1;
}

int research1_optimized_two_factors(int lim,int max_factor){
    int max_pal=0;
    int sqrt_lim=adjusted_square_root(lim,max_factor);
    int min_k_increment=sqrt_lim;
    for(int j=sqrt_lim;j*max_factor>max_pal;j--){
        min_k_increment=iterate_positive_3_factors(lim,1,j,min_k_increment,&max_pal,max_factor);
        iterate_negative_3_factors(lim,1,j,sqrt_lim,&max_pal,j);
    }
    log_result("%d best palindrome for lim=%d in research1\n",max_pal,lim);
    return max_pal;
}

int main(void){
    int lim=3553;
    int max_factor=99;
    research1_optimized_two_factors(lim,max_factor);
    return 0;
}