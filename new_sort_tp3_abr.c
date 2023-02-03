#include <stdio.h>
#include <stdlib.h>

typedef struct tree {
    
    int Id ; // Id statio 
    
    long date_time ; //  date_time

    float data ; // temperature or pressure
    
    int counter ; // counter for average, we will make average of data
    
    // classics for ABR 
    struct tree* pright ; 
    struct tree* pleft ; 

}treenode ; //treenode for mode 3


treenode* create_tree_3(int Id, long date_time, float data){ //create treenode for mode 3
    
    treenode* newnode = malloc(sizeof(treenode));
    if (newnode == NULL)
    {
        printf("Error allocating memory\n");
        exit(4);
    }
    
    newnode->Id = Id ;
    newnode->date_time = date_time ; 
    newnode->data = data ;
    
    newnode->counter = 1 ;
    
    newnode->pright = NULL ; 
    newnode->pleft = NULL ; 
    
    return newnode ;
}

treenode* insertion_ABR_3(treenode* ptree, int Id, long date_time, float data){ //classic insertionABR fonction (+data)
    
    if (ptree==NULL)
    {
        return create_tree_3( Id, date_time, data );
    }
    
    else if ( date_time == ptree->date_time ) // if the date_time is already in the tree, sort by Id
    {
        if ( Id == ptree->Id ) // if Id is already in the tree, change data and counter value
        {
            ptree->data = ptree->data + data ;   // we decide to make average data
            ptree->counter = ptree->counter + 1 ;
            return ptree ;
        }
        else
        {
            if ( Id < ptree->Id )
            {
                ptree->pleft = insertion_ABR_3(ptree->pleft ,Id ,date_time , data);
            }
            else if ( Id > ptree->Id )
            {
                ptree->pright = insertion_ABR_3(ptree->pright ,Id ,date_time , data);   
            }
            
            return ptree ; 
            
        }
    }
    
    else if (date_time < ptree->Id )
    {
        ptree->pleft = insertion_ABR_3(ptree->pleft ,Id ,date_time , data);
    }
    
    else if (date_time > ptree->Id )
    {
        ptree->pright = insertion_ABR_3(ptree->pright ,Id ,date_time , data);
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
		fprintf(pfile , "%ld;%d;%f;\n" , ptree->date_time, ptree->Id, ptree->data );
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
	    printf("Error, wrong number of arguments in main sort_tp3_abr\n");
	    exit(1);
	}
    
    FILE* pfile = fopen(argv[1] , "r");
    if (pfile==NULL)
    {
        printf("Error allocating memory when oppening input file in main sort_tp3_abr\n");
		exit(1);
	}
	
    FILE* sorted_pfile = fopen(argv[2] , "w");
    if (sorted_pfile==NULL)
    {
        printf("Error allocating memory when oppening output file in main sort_tp3_abr\n");
		exit(1);
	}
      
    treenode* data_tree = NULL ; 
    int Id ;
    long date_time ;
    float data ;
    
    while ( is_end_of_file(pfile) == 0 ) 
    {
		fscanf(pfile, "%d;%ld;%f", &Id , &date_time , &data);   //fsanf the input file
		data_tree = insertion_ABR_3 (data_tree, Id , date_time , data); //make data_tree
	}
	fclose(pfile);
    
	walktrough_data_tree_infix ( data_tree , sorted_pfile );//walktrough data tree making output file
	fclose(sorted_pfile);
    
    return 0 ; 
}
