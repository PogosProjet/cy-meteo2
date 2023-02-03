#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
    
    long elt ;   //  date_time

    float data ; // temperature or pressure

    int counter ; //counter for average
    
    // classics for ABR 
    struct tree* pright ; 
    struct tree* pleft ; 

}treenode ; //treenode for mode 2


treenode* create_tree_2(int date_time , float data){ //create treenode for mode 2
    
    treenode* newnode = malloc(sizeof(treenode));
    if (newnode == NULL)
    {
        printf("Error allocating memory\n");
        exit(4);
    }
    
    newnode->elt = date_time ; 
    
    newnode->data = data ;
    
    newnode->counter = 1 ; 
    
    newnode->pright = NULL ; 
    newnode->pleft = NULL ; 
    
    return newnode ;
}



treenode* insertion_ABR_2(treenode* ptree, long date_time, float data){ //classic insertionABR fonction (+data)
    
    if (ptree==NULL)
    {
        return create_tree_2( date_time, data );
    }
    
    else if ( date_time == ptree->elt ) // if the date_time is already in the tree, change data and counter values
    {
        ptree->data = ptree->data + data ;
        ptree->counter = ptree->counter + 1 ;
        
        return ptree ; 
    }
    
    else if (date_time < ptree->elt )
    {
        ptree->pleft = insertion_ABR_2(ptree->pleft , date_time , data);
    }
    
    else if (date_time > ptree->elt )
    {
        ptree->pright = insertion_ABR_2(ptree->pright , date_time , data);
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
		ptree->data = (ptree->data)/(ptree->counter); //data becomes average data	
		fprintf(pfile , "%ld;%f\n" , ptree->elt , ptree->data );
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
	    printf("Error, wrong number of arguments in main sort_tp2_abr\n");
	    exit(1);
	}
    
    FILE* pfile = fopen(argv[1] , "r");
    if (pfile==NULL)
    {
        printf("Error allocating memory when oppening input file in main sort_tp2_abr\n");
		exit(1);
	}
	
    FILE* sorted_pfile = fopen(argv[2] , "w");
    if (sorted_pfile==NULL)
    {
        printf("Error allocating memory when oppening output file in main sort_tp2_abr\n");
		exit(1);
	}
      
    treenode* data_tree = NULL ; 
    long date_time ;
    float data ;
    
    while ( is_end_of_file(pfile) == 0 )
    {
		fscanf(pfile, "%ld;%f", &date_time , &data);
		data_tree = insertion_ABR_2 (data_tree, date_time , data);
	}
	fclose(pfile);
    
	walktrough_data_tree_infix ( data_tree , sorted_pfile );
	fclose(sorted_pfile);
    
    return 0 ; 
}
