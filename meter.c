#include <stdio.h>
#include <stdlib.h>

struct ll {
	int meter;
	struct ll *next; 
};

struct wrapper {
	struct ll this;
	int size;
	struct wrapper *next;
};


struct node {
	struct wrapper *remaining
	struct ll *openset;
	struct ll *closed_set;
	struct node *children;
};



void print_ll(struct ll *lls,int n) {
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

void print_all(struct wrapper *


/*
(struct node) build_root(struct node nd) {
	nd = (struct node *) malloc(sizeof(struct node));


(struct node *) build_tree(struct ll *array,int n) {
	struct node root = build_node(root);
}*/

(struct wrapper *) getNewWrapper() {
	struct wrapper *ret = (struct wrapper *) malloc(sizeof(struct wrapper));
	return ret;
}


void fillWrapper(struct wrapper *ws, int n) {
	ws->size = n;
	ws->next = NULL;
	int i,dummy,input;
	struct ll *temp,*head;
	if ((dummy = fscanf(file,"%d",&input)) == 0) {
		printf("Error reading input\n");
		exit(1);
	}
	temp = (struct ll *) malloc(sizeof(struct ll));
	temp->next=NULL;
	temp->meter = input;
	ws->this = temp;
	head = temp;
	for (i=0;i<n-1;i++) {
		if ((dummy = fscanf(file,"%d",&input)) == 0) {
			printf("Error reading input\n");
			exit(1);
		}
		temp = (struct ll *) malloc(sizeof(struct ll));
		temp->next=NULL;
		temp->meter = input;
		head->next = temp;
		head=temp;
	}
}


int main (int argc, char *argv[]) {
    char *infile = argv[1] ;
	FILE *file;
    if (argc<2) {
        printf("wrong execution. try %s input_file\n",argv[0]);
        exit(1);
    }
    file = fopen(infile,"r");
	int dangerous,meters,dummy;
    if ((dummy = fscanf(file,"%d%d",&meters,&dangerous)) == 0) {
		printf("error reading number of meters and number of dangerous combinations\n");
		exit(1);
	}
	int i,num,j,input;
	struct wrapper *temp,head;
	struct wrapper *root = getNewWrapper();
	head = root;
	if ((dummy = fscanf(file,"%d",&num)) == 0) {
		printf("Error reading number first number\n");
		exit(1);
	}
	fillWrapper(root,num);
	for (i=0;i<dangerous-1;i++) {
		if ((dummy = fscanf(file,"%d",&num)) == 0) {
			printf("Error reading number first number\n");
			exit(1);
		}
		temp = getNewWrapper();
		fillWrapper(temp,num);
		head->next = temp;
		head = temp;
	}
	print_all(root);
//	root = build_tree(comb_array,dangerous);
//	struct node *set = bfs(root);
//	print_solution(set,meters);
	return 0;
}
