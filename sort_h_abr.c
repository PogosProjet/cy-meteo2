#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
    
    int elt ;   // Id station 
    
    float h ;  // station height
    
    // classics for ABR 
    struct tree* pright ; 
    struct tree* pleft ; 

}treenode ; //treenode for height


treenode* create_tree_h(int Id , float h){ //create treenode for mode 1
    
    treenode* newnode = malloc(sizeof(treenode));
    if (newnode == NULL)
    {
        printf("Error allocating memory\n");
        exit(4);
    }
    
    newnode->elt = Id ; 
    
    newnode->h = h ; 
    
    newnode->pright = NULL ; 
    newnode->pleft = NULL ; 
    
    return newnode ;
}

treenode* insertion_ABR_h(treenode* ptree, int Id, float h){ //classic insertionABR fonction (+data)
    
    if (ptree==NULL)
    {
        return create_tree_h( Id, h);
    }
    
    else if ( Id == ptree->elt ) // if same station,same height, return tree
    {
        return ptree;
    }
    
    else if (h >= ptree->h )
    {
        ptree->pright = insertion_ABR_h(ptree->pright , Id , h);
    }
    
    else if (h < ptree->h )
    {
        ptree->pleft = insertion_ABR_h(ptree->pleft , Id , h);
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
		fprintf(pfile , "%f;%05d\n" , ptree->h , ptree->elt );
	}
}

void walktrough_data_tree_infix(treenode* ptree , FILE* pfile ){
if(!is_empty(ptree))
	{
		walktrough_data_tree_infix(ptree->pright , pfile);
		process(ptree , pfile);                           //reverse infix for decreassing
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
	    printf("Error, wrong number of arguments in main sort_h_abr\n");
	    exit(1);
	}
	
    FILE* pfile = fopen("filtered_height.csv" , "r");
    if (pfile==NULL)
    {
        printf("Error allocating memory when oppening input file in main sort_h_abr\n");
		exit(12);
	}
	FILE* sorted_pfile = fopen("sorted_height.csv" , "w");
    if (pfile==NULL)
    {
        printf("Error allocating memory when oppening output file in main sort_h_abr\n");
		exit(13);
	}
	
	treenode* data_tree = NULL ;
	int Id ;
    float h ;
    
    while ( is_end_of_file(pfile) == 0 )
    {
		fscanf(pfile, "%d;%f", &Id , &h);
		data_tree = insertion_ABR_h (data_tree, Id , h);
	}
	fclose(pfile);
	
	walktrough_data_tree_infix ( data_tree , sorted_pfile );
	fclose(sorted_pfile);
	
	return 0 ; 
}
