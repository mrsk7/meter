#include <stdio.h>
#include <stdlib.h>

struct ll {
	int meter;
	struct ll *next; 
};

struct ll_of_ll {
	struct ll this;
	struct ll_of_ll *next;
}

struct help {
	int size;
	struct ll *next; 
};

struct node {
	struct ll_of_ll *remaining
	struct ll *openset;
	struct ll *closed_set;
	struct node *children;
};



void print_array(struct help *array,int n) {
	int i;
	for (i=0;i<n;i++) {
		struct ll *ptr=array[i].next;
		while (ptr!=NULL) {
			printf("%d->",ptr->meter);
			ptr=ptr->next;
		}
		printf("\n");
	}
}
/*
(struct node) build_root(struct node nd) {
	nd = (struct node *) malloc(sizeof(struct node));


(struct node *) build_tree(struct ll *array,int n) {
	struct node root = build_node(root);
}*/

int main (int argc, char *argv[]) {
    char *infile = argv[1] ;
	FILE *file;
    if (argc<2) {
        printf("Wrong execution. Try %s input_file\n",argv[0]);
        exit(1);
    }
    file = fopen(infile,"r");
	int dangerous,meters,dummy;
    if ((dummy = fscanf(file,"%d%d",&meters,&dangerous)) == 0) {
		printf("Error reading number of meters and number of dangerous combinations\n");
		exit(1);
	}
	int i,num,j,input;
	struct help *comb_array = (struct help *) malloc(dangerous*sizeof(struct help));
	struct ll *temp,*index;
	for (i=0;i<dangerous;i++) {
		if ((dummy = fscanf(file,"%d",&num)) == 0) {
			printf("Error reading number first number\n");
			exit(1);
		}
		comb_array[i].size=num;
		if ((dummy = fscanf(file,"%d",&input)) == 0) {
			printf("Error reading input\n");
			exit(1);
		}
		temp = (struct ll *) malloc(sizeof(struct ll));
		temp->next=NULL;
		temp->meter = input;
		comb_array[i].next = temp;
		index = temp;
		for (j=0;j<num-1;j++) {
			if ((dummy = fscanf(file,"%d",&input)) == 0) {
				printf("Error reading input\n");
				exit(1);
			}
			temp = (struct ll *) malloc(sizeof(struct ll));
			temp->next=NULL;
			temp->meter = input;
			index->next = temp;
			index=temp;
		}
	}
	print_array(comb_array,dangerous);
	root = build_tree(comb_array,dangerous);
//	struct node *set = bfs(root);
//	print_solution(set,meters);
	return 0;
}
