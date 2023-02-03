#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct tree {
    
    int elt ;   // Id station 
    
    float x ;   // wind speed coordonates
    float y ; 
   
    int counter ;// counter for average 
    
    // classics for ABR 
    struct tree* pright ; 
    struct tree* pleft ; 

}treenode ; //treenode for wind sort


double radian( double d ){  //transform degres in radians
    return (d*3.1415)/180 ;
}

treenode* create_tree_w(int Id , double direction , float speed){ //create treenode for wind
    
    treenode* newnode = malloc(sizeof(treenode));
    if (newnode == NULL)
    {
        printf("Error allocating memory\n");
        exit(4);
    }
    
    newnode->elt = Id ; 
    
    newnode->x = speed*cos(radian(direction));
    newnode->y = speed*sin(radian(direction));
     
    newnode->counter = 1 ; 
    
    newnode->pright = NULL ; 
    newnode->pleft = NULL ; 
    
    return newnode ;
}

treenode* insertion_ABR_w(treenode* ptree, int Id, double direction ,float speed){ //classic insertionABR fonction (+data)
    
    if (ptree==NULL)
    {
        return create_tree_w( Id, direction, speed);
    }
    
    else if ( Id == ptree->elt ) // if the station is already in the tree, change all data and counter values
    {
        
        ptree->x = ptree->x + speed*cos(radian(direction)) ; 
        ptree->y = ptree->y + speed*sin(radian(direction)) ;
        
        ptree->counter = ptree->counter + 1 ;
        
        return ptree ; 
    }
    
    else if (Id < ptree->elt )
    {
        ptree->pleft = insertion_ABR_w(ptree->pleft , Id, direction, speed);
    }
    
    else if (Id > ptree->elt )
    {
        ptree->pright = insertion_ABR_w(ptree->pright , Id, direction, speed);
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
		ptree->x = (ptree->x)/(ptree->counter);  //speeds becomes average speeds 
		ptree->y = (ptree->y)/(ptree->counter);
		
		ptree->counter = ptree->x ; //using counter as temporary variable
		
		ptree->x = sqrtf((ptree->x)*(ptree->x) + (ptree->y)*(ptree->x));   // x and y are transform in polar
		if(ptree->counter = 0)
			{
				ptree->y = 90 ;
			}
			else
			{
				ptree->y = atanf ( (ptree->y)/(ptree->x) );
			}
		fprintf(pfile , "%05d;%f;%f\n" , ptree->elt , ptree->x , ptree->y);
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
	    printf("Error, wrong number of arguments in main sort_w_abr\n");
	    exit(1);
	}
    FILE* pfile = fopen( argv[1] , "r");
    if (pfile==NULL)
    {
        printf("Error allocating memory when oppening input file in main sort_w_abr\n");
		exit(1);
	}
	FILE* sorted_pfile = fopen( argv[2] , "w");
    if (pfile==NULL)
    {
        printf("Error allocating memory when oppening output file in main sort_w_abr\n");
		exit(1);
	}
	
	treenode* data_tree = NULL ;
	int Id ;
    double direction ;
    float speed ;
    
    while ( is_end_of_file(pfile) == 0 )
    {
		fscanf(pfile, "%d;%lf;%f", &Id , &direction, &speed);
		data_tree = insertion_ABR_w (data_tree, Id , direction, speed);
	}
	fclose(pfile);
	
	walktrough_data_tree_infix ( data_tree , sorted_pfile );
	fclose(sorted_pfile);
    
return 0 ; 

}
