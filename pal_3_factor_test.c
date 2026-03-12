#include "pal_3_factor_test.h"


int is_palindrome_3(int number) {
    if (number < 0) return 0;
    if (number % 10 == 0) return number == 0; // 0 est palindrome, mais les multiples de 10 non sauf 0
    int reversed = 0;
    while (number > reversed) {
        reversed = reversed * 10 + number % 10;
        number /= 10;
    }
    // Si le nombre a un nombre impair de chiffres, on ignore le chiffre du milieu
    return number == reversed || number == reversed / 10;
}

int adjusted_cube_root_3(int lim,int max_factor){
    int root=cbrt(lim);
    if(root>max_factor) 
        return max_factor;
    else if(root*root*root==lim) 
        return root;
    return root+1;
}

int iterate_positive_3_factors_3(int lim,int i,int j,int start_k_increment,int *max_pal,int max,int *iteration_count){
    for(int k=start_k_increment; k<=max && i*j*k<lim ; k++){
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


void iterate_negative_3_factors_3(int lim,int i,int j,int start_k_decrement,int *max_pal,int min, int *iteration_count){
    for(int k=start_k_decrement; k>=min && k*i*j>*max_pal; k--){
        (*iteration_count)++;
        int product=k*i*j;
        if(is_palindrome_3(product) ){//&& product<lim si erreur 
            *max_pal=product;
        }
    }
    return ;
}



int research_optimized_three_factors_3(int lim,int max_factor,int *iteration_count){
    if (lim <= 0) return 0; // Sécurité division par 0
    int max_pal=0;
    int sbrt_lim=adjusted_cube_root_3(lim,max_factor);
    int start_j_increment=sbrt_lim;
    for(int i=sbrt_lim; i*max_factor*max_factor>max_pal; i--){//- mais par de mf(ca marce de sbrt wtf)
        (*iteration_count)++;
        if (!(i*i*i>lim)){//
            
            for(int j=start_j_increment; j<=max_factor && i*j*sbrt_lim<lim ; j++){//&& i*max_factor*max_factor>max_pal pas très utilr car ici ca monte bcp
                (*iteration_count)++;

                int start_k_decrement =i*j*j<lim ? j : (lim-1)/(i*j);//--+ car si i*j*j est plus petit que lim, on peut utiliser start_k_decrement tel quel, sinon on le limite à j pour éviter de dépasser les limites du problème
                iterate_negative_3_factors_3(lim,i,j,start_k_decrement,&max_pal,sbrt_lim,iteration_count);//-++ car tt au dessus de sbrt_lim, descend de mf a sbrt_lim et utilsationde iteration negative car permet de mieux cibler la plage utilse avec >max_pal pour sur
                
                if(i*j*j<max_pal) // k pas plus grand que j
                    start_j_increment=j;// monte au dessus de sbrt_lim et i va que descendre 
            }

            int start_k_increment=sbrt_lim;//-- car i va que descendre et j aussi donc k doit monter pour compenser et trouver des produits plus grands que max_pal

            for(int j=sbrt_lim; j>=i && i*j*max_factor>max_pal; j--){//--
                (*iteration_count)++;

                int k_limite=(lim-1)/(i*j); 

                int start_k_decrement= k_limite>max_factor ? max_factor : k_limite;//--+ car si le start_k_decrement est plus grand que max_factor, on le limite à max_factor pour éviter de dépasser les limites du problème
                
                iterate_negative_3_factors_3(lim,i,j,start_k_decrement,&max_pal,sbrt_lim,iteration_count);//--+ utilise decrement pour optimisation

                start_k_decrement= k_limite>sbrt_lim ? sbrt_lim : k_limite;//--- 
    
                iterate_negative_3_factors_3(lim,i,j,start_k_decrement,&max_pal,j,iteration_count);//---
            }
        }
    }
    return max_pal;
}



/*int main(void){
    int lim=400000;
    int max_factor=99;
    int iteration_count = 0;
    research_optimized_three_factors_3(lim,max_factor,&iteration_count);
    log_result("Number of iterations: %d\n", iteration_count);
    return 0;
}*/