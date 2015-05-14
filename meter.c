#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ll {
	int meter;
	struct ll *next; 
};
typedef struct ll LinkedList;

struct wrapper {
	LinkedList *this;
	int size;
	struct wrapper *next;
};

typedef struct wrapper Wrapper;

struct node {
	Wrapper *remaining;
	LinkedList *openset;
	LinkedList *closet;
	struct node *next_in_queue;
};

typedef struct node Node;

struct queue {
	Node *front;
	Node *rear;
};

typedef struct queue Queue;

void print_ll(LinkedList *lls) {
        LinkedList *ptr = lls;
        while (ptr!=NULL) {
                printf("%d->",ptr->meter);
                ptr=ptr->next;
        }
        printf("\n");
}

void print_all(Wrapper *ws) {
    Wrapper *head = ws;
    while (head) {
        print_ll(head->this);
        head=head->next;
    }
}


Wrapper *getNewWrapper() {
	Wrapper *ret =  malloc(sizeof(Wrapper));
	return ret;
}


void fillWrapper(Wrapper *ws, FILE *file, int n) {
	ws->size = n;
	ws->next = NULL;
	int i,dummy,input;
	LinkedList *temp,*head;
	if ((dummy = fscanf(file,"%d",&input)) == 0) {
		printf("Error reading input\n");
		exit(1);
	}
	temp =  malloc(sizeof(LinkedList));
	temp->next=NULL;
	temp->meter = input;
	ws->this = temp;
	head = temp;
	for (i=0;i<n-1;i++) {
		if ((dummy = fscanf(file,"%d",&input)) == 0) {
			printf("Error reading input\n");
			exit(1);
		}
		temp = malloc(sizeof(LinkedList));
		temp->next=NULL;
		temp->meter = input;
		head->next = temp;
		head=temp;
	}
}

void insert(struct queue *q,Node *nd) {
	if (q->rear == NULL) {
		q->rear = nd;
		q->front = nd;
		return;
	}
	(q->rear)->next_in_queue = nd;
	q->rear = nd;
}

Node *removeq(struct queue *q) {
	if (q->front = NULL) return NULL;
	Node *ret = q->front;
	q->front = (q->front)->next_in_queue;
	return ret;
}

LinkedList *deepCopyLinkedList(LinkedList *head) {
	LinkedList *copy;
	if (head == NULL) {
		return NULL;
	}
	else {
		copy = malloc(sizeof(LinkedList));
		copy->meter = head->meter;
		copy->next = deepCopyLinkedList(head->next);
		return copy;
	}
}

Wrapper *deepCopyWrapper(Wrapper *head) {
	Wrapper *copy;
	if (head == NULL) {
		return NULL;
	}
	else {
		copy = malloc(sizeof(Wrapper));
		copy->this = deepCopyLinkedList(head->this);
		copy->size = head->size;
		copy->next = deepCopyWrapper(head->next);
		return copy;
	}
}

Node *createChild(Node *parent) {
	Node *ret = malloc(sizeof(Node));
	ret->openset=deepCopyLinkedList(parent->openset);
	ret->closet=deepCopyLinkedList(parent->closet);
	if (parent->remaining->next == NULL) ret->remaining = NULL;
	ret->remaining = deepCopyWrapper(parent->remaining->next);
	return ret;
}

void updateOpenSet(Node *node,LinkedList *ll) {
	LinkedList *entry = malloc(sizeof(LinkedList));
	entry->meter = ll->meter;
	entry->next = ll;
	ll = entry;
}

void updateClosedset(Node *node,Wrapper *list,LinkedList* key) {
	LinkedList *head = list->this;
	while(!head) {
		if (head->meter == key->meter) {
			head = head->next;
			continue;
		}
		LinkedList *entry = malloc(sizeof(LinkedList));
		entry->meter = head->meter;
		entry->next = node->closet;
		node->closet = entry
	}
}

LinkedList *build_tree(Wrapper *root) {
	int i;
	struct queue *q = malloc(sizeof(struct queue));;
	q->rear = NULL;
	Node *start = malloc(sizeof(Node));
	Node->openset = NULL;
	Node->closet = NULL;
	Node->next_in_queue = NULL;
	Node->remaining = root;
	Node *current;			//Pointer used to point to removed Node from Queue.
	Node *child; 			//Pointer for new child-node of each Node
	LinkedList *head;
	while(1) {
		current=removeq(q);
		if ((current==NULL) || (current->remaining)) break;
		head = current->remaining->this;
		while(head!=NULL) {
			child = createChild(current);
			updateOpenset(child,head);
			updateClosedset(child,current->remaining,head);
			updateRemaining(child,head);
			head=head->next;
		}
		deepfree(current);
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
	Wrapper *temp,*head;
	Wrapper *root = getNewWrapper();
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
	LinkedList *not_solution = build_tree(root);
	//print_solution(not_solution,meters);
	return 0;
}
