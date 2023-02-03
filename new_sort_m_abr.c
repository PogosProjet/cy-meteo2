#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
    
    int elt ;   // Id station 
    
    float m ;  // station moisture
    
    float latitude ;  // station coordonates
    float longitude ;
    
    // classics for ABR 
    struct tree* pright ; 
    struct tree* pleft ; 

}treenode ; //treenode for moisture


treenode* create_tree_m(int Id , float m , float lat , float lon ){ //create treenode for mode 1
    
    treenode* newnode = malloc(sizeof(treenode));
    if (newnode == NULL)
    {
        printf("Error allocating memory\n");
        exit(4);
    }
    
    newnode->elt = Id ; 
    
    newnode->m = m ; 
    
    newnode->latitude = lat ;
    newnode->longitude = lon ;
    
    newnode->pright = NULL ; 
    newnode->pleft = NULL ; 
    
    return newnode ;
}

float maxf(float A , float B){  //return max of 2 floats //bibliothèque ?
    if ( A >= B )
    {
        return A ; 
    }
    else
    {
        return B ;
    }
}

treenode* insertion_ABR_m(treenode* ptree, int Id, float m, float lat , float lon){ //classic insertionABR fonction (+data)
    
    if (ptree==NULL)
    {
        return create_tree_m( Id, m ,lat, lon);
    }
    
    else if ( Id == ptree->elt ) // if same station, change data
    {
	ptree->m = maxf(ptree->m , m);
	return ptree;      
    }
   
    else if (m >= ptree->m )
    {
        ptree->pright = insertion_ABR_m(ptree->pright , Id , m, lat , lon);
    }
    
    else if (m > ptree->m )
    {
        ptree->pleft = insertion_ABR_m(ptree->pleft , Id , m , lat , lon );
    }
    
return ptree ;

}

int is_empty( treenode* ptree )
	{
		return(ptree==NULL);
	}

void process(treenode* ptree , FILE* pfile){
if(!is_empty(ptree))
	{ 	
		fprintf(pfile , "%f;%05d;%f;%f\n" , ptree->m , ptree->elt ,ptree->latitude ,ptree->longitude );
	}
}

void walktrough_data_tree_infix(treenode* ptree , FILE* pfile ){
if(!is_empty(ptree))
	{
		walktrough_data_tree_infix(ptree->pright , pfile);
		process(ptree , pfile);                            //reverse infix for decreasing moisture
		walktrough_data_tree_infix(ptree->pleft , pfile);
	}
}
 
int is_end_of_file ( FILE* pfile ){
	
	int E = 1 ; 
	
	if ( fgetc(pfile) != EOF )
	{
		E = 0 ; 
	}
	
	fseek(pfile, -1 , SEEK_CUR);
	
	return E;
}


int main(int argc, char* argv[]){ 
   if(argc != 3)
	{
	    printf("Error, wrong number of arguments in main sort_m_abr\n");
	    exit(1);
	}
    
    FILE* pfile = fopen( argv[1] , "r");
    if (pfile==NULL)
    {
        printf("Error allocating memory when oppening input file in main sort_m_abr\n");
		exit(1);
	}
	
    FILE* sorted_pfile = fopen( argv[2] , "w");
    if (sorted_pfile==NULL)
    {
        printf("Error allocating memory when oppening output file in main sort_m_abr\n");
		exit(1);
	}
	
	treenode* data_tree = NULL ;
    int Id ;
    float m ;
    float latitude ;
    float longitude ;
    
    while ( is_end_of_file(pfile) == 0 )
    {
		fscanf(pfile, "%d;%f;%f;%f", &Id , &m, &latitude, &longitude);
		data_tree = insertion_ABR_m (data_tree, Id , m ,latitude, longitude);
	}
	fclose(pfile);
	
	walktrough_data_tree_infix ( data_tree , sorted_pfile );
	fclose(sorted_pfile);
	
	return 0 ; 
}
