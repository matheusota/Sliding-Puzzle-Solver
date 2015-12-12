#include "pdb_functions.h"

int main(){
	int i, j, id;
	tree t;
	
	inserted = 0;
	strcpy(directions, "ulrd");
	
	printf("Select the puzzle you wish to create the pdb:\n");
	printf("0 -> 8-puzzle\n");
	printf("1 -> 15-puzzle\n");
	scanf("%d", &id);
	
	if(id == 0){
		SIZE = 3;
		//create pattern database
		printf("computing the pdb...\n");
		createDatabase(0);
		freeHash(0);
		freeHash(1);
		printf("pdb created successfully\n");
		
		printf("%d nodes inserted on the pdbs\n", inserted);
	}
	else{
		SIZE = 4;
		//create pattern database
		printf("computing the pdbs...\n");
		createDatabase(1);
		freeHash(0);
		freeHash(1);
		printf("pdb0 created successfully\n");
		
		createDatabase(2);
		memset(hashBlank, 0, HASH_SIZE * sizeof(hashnode));
		memset(hash, 0, HASH_SIZE * sizeof(hashnode));
		printf("pdb1 created successfully\n");
		
		createDatabase(3);
		freeHash(0);
		freeHash(1);
		printf("pdb2 created successfully\n");
		
		printf("%d nodes inserted on the pdbs\n", inserted);
	}
	
	return 0;
}
