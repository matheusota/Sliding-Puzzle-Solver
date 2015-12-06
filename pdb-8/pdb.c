#include "pdb_functions.h"

int main(){
	int i, j;
	tree t;
	
	strcpy(directions, "urdl");
	
	//create pattern database
	printf("computing the pdb...\n");
	createDatabase();
	printf("pdb created successfully\n");
	printf("%d nodes inserted on the pdb\n", inserted);
	
	return 0;
}
