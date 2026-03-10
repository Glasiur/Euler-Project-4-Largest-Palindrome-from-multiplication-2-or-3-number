#include <stdio.h>
#include <math.h>

#define ENABLE_DEBUG_LOGS 1
#define ENABLE_RESULT_LOGS 1



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
    for(int k=min_k_increment; k<=max_factor && i*j*k<lim ; k++){
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
    for(int k=max_k_decrement; k*i*j>*max_pal && k>=max_factor ; k--){
        int product=k*i*j;
        if(product>=lim)
            max_k_decrement=k;
        else if(is_palindrome(product)){
            *max_pal=product;
        }
    }
    return max_k_decrement;
}


int adjusted_cube_root(int lim,int max_factor){
    int root=cbrt(lim);
    if(root>max_factor) 
        return max_factor;
    else if(root*root*root==lim) 
        return root;
    return root+1;
}

int research2_optimized_three_factors(int lim,int max_factor){
    int max_pal=0;
    int sbrt_lim=adjusted_cube_root(lim,max_factor);
    int min_j_increment=sbrt_lim;
    for(int i=max_factor; i*max_factor*max_factor>max_pal; i--){
        int max_k_decrement=min_j_increment;
        for(int j=min_j_increment; j<=max_factor && i*j*sbrt_lim<lim; j++){
            if(i*j*j<lim) 
                max_k_decrement=j;
            if(i*j*j<max_pal) 
                min_j_increment=j;
            else 
                max_k_decrement=iterate_negative_3_factors(lim,i,j,max_k_decrement,&max_pal,sbrt_lim);//-++
        }
        int min_k_increment=sbrt_lim;
        for(int j=sbrt_lim; j>=i && i*j*max_factor>max_pal; j--){
            min_k_increment=iterate_positive_3_factors(lim,i,j,min_k_increment,&max_pal,max_factor);//--+
            iterate_negative_3_factors(lim,i,j,sbrt_lim,&max_pal,j);//---
        }
    }
    log_result("%d best palindrome for lim=%d in research2\n",max_pal,lim);
    return max_pal;
}



int research_optimized_three_factors_(int lim, int max_factor, int *iteration_count) {
    int max_pal = 0;
    if (lim <= 0) return 0; // Sécurité division par 0

    int sbrt_lim = adjusted_cube_root(lim, max_factor);
    int j_min_increment = sbrt_lim;

    for (int i = max_factor; (long long)i * max_factor * max_factor > max_pal; i--) {
        (*iteration_count)++;

        // --- ZONE 1 : j monte ---
        int k_max_decrement = j_min_increment;
        for (int j = j_min_increment; j <= max_factor && (long long)i * j * sbrt_lim < lim; j++) {
            (*iteration_count)++;

            if ((long long)i * j * j < lim) k_max_decrement = j;
            
            if ((long long)i * j * j < max_pal) {
                j_min_increment = j;
            } else {
                // OPTIMISATION ICI : On ne laisse pas k commencer trop haut
                int k_top = (lim - 1) / (i * j);
                if (k_top < k_max_decrement) k_max_decrement = k_top;

                k_max_decrement = iterate_negative_3_factors(lim, i, j, k_max_decrement, &max_pal, iteration_count, sbrt_lim);
            }
        }

        // --- ZONE 2 : j descend ---
        int min_k_increment = sbrt_lim;
        for (int j = sbrt_lim; j >= i && (long long)i * j * i < lim; j--) {
            // Si i*j*max_factor est plus petit que max_pal, on peut break la boucle j
            if ((long long)i * j * max_factor <= max_pal) break; 
            
            (*iteration_count)++;

            // OPTIMISATION ICI : k ne peut pas dépasser lim/(i*j)
            int k_limit = (lim - 1) / (i * j);
            if (k_limit > max_factor) k_limit = max_factor;

            min_k_increment = iterate_positive_3_factors(lim, i, j, min_k_increment, &max_pal, iteration_count, k_limit);
            iterate_negative_3_factors(lim, i, j, sbrt_lim, &max_pal, iteration_count, j);
        }
    }
    return max_pal;
}

int main(void){
    int lim=400000;
    int max_factor=99;
    research2_optimized_three_factors(lim,max_factor);
    return 0;
}