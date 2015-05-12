#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ll {
	int meter;
	struct ll *next; 
};

struct wrapper {
	struct ll *this;
	int size;
	struct wrapper *next;
};


struct node {
	struct wrapper *remaining;
	struct ll *openset;
	struct ll *closet;
	struct node *next_in_queue;
};

struct queue {
	struct node *front;
	struct node *rear;
};


void print_ll(struct ll *lls) {
        struct ll *ptr = lls;
        while (ptr!=NULL) {
                printf("%d->",ptr->meter);
                ptr=ptr->next;
        }
        printf("\n");
}

void print_all(struct wrapper *ws) {
    struct wrapper *head = ws;
    while (head) {
        print_ll(head->this);
        head=head->next;
    }
}


/*
(struct node) build_root(struct node nd) {
	nd = (struct node *) malloc(sizeof(struct node));


(struct node *) build_tree(struct ll *array,int n) {
	struct node root = build_node(root);
}*/

struct wrapper *getNewWrapper() {
	struct wrapper *ret =  malloc(sizeof(struct wrapper));
	return ret;
}


void fillWrapper(struct wrapper *ws, FILE *file, int n) {
	ws->size = n;
	ws->next = NULL;
	int i,dummy,input;
	struct ll *temp,*head;
	if ((dummy = fscanf(file,"%d",&input)) == 0) {
		printf("Error reading input\n");
		exit(1);
	}
	temp =  malloc(sizeof(struct ll));
	temp->next=NULL;
	temp->meter = input;
	ws->this = temp;
	head = temp;
	for (i=0;i<n-1;i++) {
		if ((dummy = fscanf(file,"%d",&input)) == 0) {
			printf("Error reading input\n");
			exit(1);
		}
		temp = malloc(sizeof(struct ll));
		temp->next=NULL;
		temp->meter = input;
		head->next = temp;
		head=temp;
	}
}

void insert(struct queue *q,struct node *nd) {
	if (q->rear == NULL) {
		q->rear = nd;
		q->front = nd;
		return;
	}
	(q->rear)->next_in_queue = nd;
	q->rear = nd;
}

struct node *removeq(struct queue *q) {
	struct node *ret = q->front;
	q->front = (q->front)->next_in_queue;
	return ret;
}

struct node *getNewNode() {
	struct node *ret = malloc(sizeof(struct node));
	return ret;
}

void fillNode(struct node *nd, struct ll *os,struct ll *cs, struct wrapper *rem) {
	nd->openset = malloc(sizeof(struct ll));
	nd->closet = malloc(sizeof(struct ll));
	nd->remaining = malloc(sizeof(struct wrapper));
	memcpy(nd->openset,os,sizeof(struct ll));
	memcpy(nd->closet,cs,sizeof(struct ll));
	memcpy(nd->remaining,rem,sizeof(struct wrapper));
}

struct wrapper *getWrapper(struct node *nd) {
	struct wrapper *ret;
   	memcpy(ret,nd->remaining,sizeof(struct wrapper));
	ret->next = NULL;
	return ret;
}

struct ll *addToLinkedList(struct ll *this,struct ll* new) {
	struct ll *ret;
	memcpy(ret,this,sizeof(struct ll));
	new->next = ret;
	return ret;
}

struct ll *build_tree(struct wrapper *root) {
	int i;
	struct ll *comb;
	struct wrapper wp;
	struct queue *q = malloc(sizeof(struct queue));;
	q->rear = NULL;
	struct node *start = getNewNode();
	struct node *cur,child;
	fillNode(start,malloc(sizeof(struct ll)),malloc(sizeof(struct ll)),root);
	insert(q,start);
	cur = removeq(q);
	while(1) {
		cur = removeq(q);
		if (cur->remaining ==NULL) break;
		wp = getWrapper(cur);
		for (i=0;i<size;i++) {
			comb = wp->this;
				child = getNewNode();
				fillNode(child,
			
		}
	}	
	return NULL;
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
	struct wrapper *temp,*head;
	struct wrapper *root = getNewWrapper();
	head = root;
	if ((dummy = fscanf(file,"%d",&num)) == 0) {
		printf("Error reading number first number\n");
		exit(1);
	}
	fillWrapper(root,file,num);
	for (i=0;i<dangerous-1;i++) {
		if ((dummy = fscanf(file,"%d",&num)) == 0) {
			printf("Error reading number first number\n");
			exit(1);
		}
		temp = getNewWrapper();
		fillWrapper(temp,file,num);
		head->next = temp;
		head = temp;
	}
	//print_all(root);
	struct ll *not_solution = build_tree(root);
	//print_solution(not_solution,meters);
	return 0;
}
