#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
    
    int elt ;   // Id station 
    
    float max_data ;
    float data ; // temperature or pressure
    float min_data ; 
    
    int counter ;// counter for average 
    
    // classics for ABR 
    struct tree* pright ; 
    struct tree* pleft ; 

}treenode ; //treenode for mode 1


treenode* create_tree_1(int Id , float data){ //create treenode for mode 1
    
    treenode* newnode = malloc(sizeof(treenode));
    if (newnode == NULL)
    {
        printf("Error allocating memory\n");
        exit(4);
    }
    
    newnode->elt = Id ; 
    
    newnode->max_data = data ; 
    newnode->data = data ;
    newnode->min_data = data ; 
    
    newnode->counter = 1 ; 
    
    newnode->pright = NULL ; 
    newnode->pleft = NULL ; 
    
    return newnode ;
}

int max( int A , int B){ //return max of 2 int 
    if ( A >= B )
    {
        return A ; 
    }
    else
    {
        return B ;
    }
}

int min( int A , int B){ // return min of 2 int 
    if ( A <= B )
    {
        return A ; 
    }
    else
    {
        return B ;
    }
}

int max_of_3 ( int A , int B , int C ){ //return max of 3 int
    return max( max(A,B), max(B,C) );
}

float maxf(float A , float B){  //return max of 2 floats 
    if ( A >= B )
    {
        return A ; 
    }
    else
    {
        return B ;
    }
}

float minf(float A , float B){  //return min of 2 floats 
    if ( A <= B )
    {
        return A ; 
    }
    else
    {
        return B ;
    }
}

treenode* insertion_ABR_1(treenode* ptree, int Id, float data){ //classic insertionABR fonction (+data)
    
    if (ptree==NULL)
    {
        return create_tree_1( Id, data );
    }
    
    else if ( Id == ptree->elt ) // if the station is already in the tree, change data and counter values
    {
        ptree->max_data = maxf(ptree->max_data , data);
        ptree->data = ptree->data + data ; 
        ptree->min_data = minf(ptree->min_data , data); 
        
        ptree->counter = ptree->counter + 1 ;
        
        return ptree ; 
    }
    
    else if (Id < ptree->elt )
    {
        ptree->pleft = insertion_ABR_1(ptree->pleft , Id , data);
    }
    
    else if (Id > ptree->elt )
    {
        ptree->pright = insertion_ABR_1(ptree->pright , Id , data);
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
		ptree->data = (ptree->data)/(ptree->counter); 	
		fprintf(pfile , "%05d;%f;%f;%f\n" , ptree->elt , ptree->max_data , ptree->min_data , ptree->data );
	}
}

void walktrough_data_tree_infix(treenode* ptree , FILE* pfile ){
if(!is_empty(ptree))
	{
		walktrough_data_tree_infix(ptree->pleft , pfile);
		process(ptree , pfile);
		walktrough_data_tree_infix(ptree->pright , pfile);
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
	    printf("Error, wrong number of arguments in main sort_tp1_abr\n");
	    exit(1);
	}
	
	FILE* pfile = fopen( argv[1] , "r");
    if (pfile==NULL)
    {
        printf("Error allocating memory when oppening input file\n");
		exit(1);
	}
	
	FILE* sorted_pfile = fopen( argv[2] , "w");
    if (pfile==NULL)
    {
        printf("Error allocating memory when oppening output file\n");
		exit(1);
	}
	
	treenode* data_tree = NULL ;
	int Id ;
    float data ;
    
    while ( is_end_of_file(pfile) == 0 )
    {
		fscanf(pfile, "%d;%f", &Id , &data);
		data_tree = insertion_ABR_1 (data_tree, Id , data);
	}
	fclose(pfile);
	
	walktrough_data_tree_infix ( data_tree , sorted_pfile );
	fclose(sorted_pfile);
	
	return 0 ; 
}
