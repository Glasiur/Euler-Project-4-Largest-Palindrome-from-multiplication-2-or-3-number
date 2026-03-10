#include <pal_3_factor.h>


int is_palindrome_3(int number){
    if (number%10==0 ) return 0;
    int original=number;
    int reversed_number=0;
    while(number>=1){
        reversed_number*=10;
        reversed_number+=number%10;
        number=number/10;
    }
    return reversed_number==original;
}


int iterate_positive_3_factors_3(int lim,int i,int j,int start_k_increment,int *max_pal,int max_factor,int *iteration_count){
    for(int k=start_k_increment; k<=max_factor && i*j*k<lim ; k++){
        (*iteration_count)++;
        int product=i*j*k;
        if(product<=*max_pal)
            start_k_increment=k;
        else if(is_palindrome_3(product)){
            *max_pal=product;
        }
    }
    return start_k_increment;
}


void iterate_negative_3_factors_3(int lim,int i,int j,int start_k_decrement,int *max_pal,int min_k_limit, int *iteration_count){
    for(int k=start_k_decrement; k*i*j>*max_pal && k>=min_k_limit ; k--){
        (*iteration_count)++;
        int product=k*i*j;
        if(is_palindrome_3(product)){
            *max_pal=product;
        }
    }
    return ;
}


int adjusted_cube_root_3(int lim,int max_factor){
    int root=cbrt(lim);
    if(root>max_factor) 
        return max_factor;
    else if(root*root*root==lim) 
        return root;
    return root+1;
}

int research_optimized_three_factors_3(int lim,int max_factor,int *iteration_count){
    if (lim <= 0) return 0; // Sécurité division par 0
    int max_pal=0;
    int sbrt_lim=adjusted_cube_root_3(lim,max_factor);
    int start_j_increment=sbrt_lim;
    for(int i=max_factor; i*max_factor*max_factor>max_pal; i--){
        (*iteration_count)++;
        for(int j=start_j_increment; j<=max_factor && i*j*sbrt_lim<lim; j++){
            (*iteration_count)++;

            int start_k_decrement;
            if (i * j * j < lim) // eviter le doublon k*j et j*k si j monte a mf et k descend de mf ainsi que respecter le -+- 
                start_k_decrement = j;
            else 
                start_k_decrement=(lim-1)/(i*j);

            iterate_negative_3_factors_3(lim,i,j,start_k_decrement,&max_pal,sbrt_lim,iteration_count);//-++ car tt au dessus de sbrt_lim, descend de mf a sbrt_lim et utilsationde iteration negative car permet de mieux cibler la plage utilse avec >max_pal pour sur
            
            if(i*j*j<max_pal) 
                start_j_increment=j;// monte au dessus de sbrt_lim et i va que descendre 
        }

        int start_k_increment=sbrt_lim;

        for(int j=sbrt_lim; j>=i && i*j*max_factor>max_pal; j--){
            (*iteration_count)++;
            start_k_increment=iterate_positive_3_factors_3(lim,i,j,start_k_increment,&max_pal,max_factor,iteration_count);//--+
            iterate_negative_3_factors_3(lim,i,j,sbrt_lim,&max_pal,j,iteration_count);//---
        }
    }
    log_result("%d best palindrome for lim=%d in research2\n",max_pal,lim);
    return max_pal;
}

/*

int research___optimized_three_factors_(int lim, int max_factor, int *iteration_count) {
    if (lim <= 0) return 0; // Sécurité division par 0
    int max_pal = 0;
    int sbrt_lim = adjusted_cube_root_3(lim, max_factor);
    int j_min_increment = sbrt_lim;//initla racine de lim

    for (int i = max_factor; i * max_factor * max_factor > max_pal; i--) {//i-
        (*iteration_count)++;

        // --- ZONE 1 : j monte ---
        int k_max_decrement = j_min_increment;//initla racine de lim
        for (int j = j_min_increment; j <= max_factor && i * j * sbrt_lim < lim; j++) {
            (*iteration_count)++;

            if (i * j * j < lim) k_max_decrement = j;
            
            if (i * j * j < max_pal) {
                j_min_increment = j;
            } else {
                // OPTIMISATION ICI : On ne laisse pas k commencer trop haut
                int k_top = (lim - 1) / (i * j);
                if (k_top < k_max_decrement) k_max_decrement = k_top;

                k_max_decrement = iterate_negative_3_factors_3(lim, i, j, k_max_decrement, &max_pal, iteration_count, sbrt_lim);
            }
        }

        // --- ZONE 2 : j descend ---
        int min_k_increment = sbrt_lim;
        for (int j = sbrt_lim; j >= i && i * j * i < lim; j--) {
            // Si i*j*max_factor est plus petit que max_pal, on peut break la boucle j
            if (i * j * max_factor <= max_pal) break; 
            
            (*iteration_count)++;

            // OPTIMISATION ICI : k ne peut pas dépasser lim/(i*j)
            int k_limit = (lim - 1) / (i * j);
            if (k_limit > max_factor) k_limit = max_factor;

            min_k_increment = iterate_positive_3_factors_3(lim, i, j, min_k_increment, &max_pal, iteration_count, k_limit);
            iterate_negative_3_factors_3(lim, i, j, sbrt_lim, &max_pal, iteration_count, j);
        }
    }
    return max_pal;
}
*/

/*int main(void){
    int lim=400000;
    int max_factor=99;
    int iteration_count = 0;
    research_optimized_three_factors_3(lim,max_factor,&iteration_count);
    log_result("Number of iterations: %d\n", iteration_count);
    return 0;
}*/