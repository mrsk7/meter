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
	struct node *next_in_queue;
};

typedef struct node Node;

struct queue {
	Node *front;
	Node *rear;
};

typedef struct queue Queue;


Wrapper *arrayToLinkedWrapper(Wrapper *array,int size) {
    Wrapper *ret;
    int i;
    for (i=0;i<size-1;i++)  array[i].next = &array[i+1];
    array[size-1].next = NULL;
    ret = array;
    return ret;
}

int compare (const void *elem1,const void *elem2) {
	Wrapper *p1 = (Wrapper*) elem1;
	Wrapper *p2 = (Wrapper*) elem2;
	return (int) (p1->size - p2->size);
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
	if (q->front == NULL) {
		return NULL;
	}
	Node *ret = q->front;
	q->front = (q->front)->next_in_queue;
	if (q->front == NULL) q->rear = NULL;
	return ret;
}

LinkedList *deepCopyLinkedList(LinkedList *head) {
	LinkedList *copy;
	if (head == NULL) {
		copy = NULL;
	}
	else {
		copy = malloc(sizeof(LinkedList));
		copy->meter = head->meter;
		copy->next = deepCopyLinkedList(head->next);
	}
	return copy;
}

Wrapper *deepCopyWrapper(Wrapper *head) {
	Wrapper *copy;
	if (head == NULL) {
		copy = NULL;
	}
	else {
		copy = malloc(sizeof(Wrapper));
		copy->this = deepCopyLinkedList(head->this);
		copy->size = head->size;
		copy->next = deepCopyWrapper(head->next);
	}
	return copy;
}

void deepfreeLL(LinkedList *ll) {
	LinkedList *head = ll;
	LinkedList *willy;
	while(head!=NULL) {
		willy = head;
		head=head->next;
		free(willy);
	}
}

void deepfreeW(Wrapper *eminem) {
	Wrapper *head = eminem;
	Wrapper *willy;
	while (head!=NULL) {
		deepfreeLL(head->this);
		willy=head;
		head=head->next;
		free(willy);
	}
}

void deepfreeNode(Node *node) {
	deepfreeW(node->remaining);
	deepfreeLL(node->openset);
	free(node);
}


Node *createChild(Node *parent,int key) {
	Node *ret = malloc(sizeof(Node));
	ret->next_in_queue = NULL;
	ret->openset=deepCopyLinkedList(parent->openset);
	if (parent->remaining->next == NULL) ret->remaining = NULL;
	ret->remaining = deepCopyWrapper(parent->remaining->next);
	return ret;
}

void updateOpenset(Node *node,LinkedList *ll) {
	LinkedList *entry = malloc(sizeof(LinkedList));
	entry->meter = ll->meter;
	entry->next = node->openset;
	node->openset = entry;
}

void updateRemaining(Node *node,LinkedList *key) {
	Wrapper *prev,*headW,*tmp;
	LinkedList *headLL;
	if (node->remaining == NULL) {
		return;
	}
	if (node->remaining->next == NULL)	{//Check if there in only one wrapper element in the list
		headW = node->remaining;
		headLL = headW->this;
		while (headLL!=NULL) {
			if (headLL->meter==key->meter) {
				deepfreeLL(headW->this);
				node->remaining = NULL;
				free(headW);
				break;
			}
			else headLL=headLL->next;
		}
	}
	else {
		/* Because I use a prev and a headW pointer, first I must check the head
		 * of the Wrapper list alone.  */
		int flag;
		prev = NULL;
		headW = node->remaining;
		while (headW!=NULL) {			//While there are dangerous combinations
			flag = 0;
			headLL = headW->this;
			while (headLL!=NULL) {		//While there are meters in each combination
				if (headLL->meter==key->meter) {	//If I match the key, remove this combination
					deepfreeLL(headW->this);
					flag = 1;						//Update the flag so that I remove the whole Wrapper struct
					break;
				}
				headLL=headLL->next;
			}
			if (flag == 1) {
				if (prev == NULL) node->remaining = headW->next;	//If first node is to be removed, point to next;
				else prev->next = headW->next;						//Not looking at first node
				tmp = headW;
				headW = headW->next;
				free(tmp);
			}
			else {				//If flag==1, thus didn't find the key in the wrapper
				if (prev==NULL) prev = node->remaining;
				else prev = prev->next;
				headW=headW->next;
			}
		}
	}
}

LinkedList *build_tree(Wrapper *root) {
	int i;
	struct queue *q = malloc(sizeof(struct queue));;
	q->rear = NULL;
	q->front = NULL;
	Node *start = malloc(sizeof(Node));
	start->openset = NULL;
	start->next_in_queue = NULL;
	start->remaining = deepCopyWrapper(root);
	insert(q,start);
	Node *current;			//Pointer used to point to removed Node from Queue.
	Node *child; 			//Pointer for new child-node of each Node
	LinkedList *head;
	while(1) {
		current=removeq(q);
		if ((current==NULL) || (current->remaining==NULL)) {
			break;
		}
		head = current->remaining->this;
		while(head!=NULL) {
			child = createChild(current,head->meter);
			updateOpenset(child,head);	
			updateRemaining(child,head);
			insert(q,child);
			head=head->next;
		}
		deepfreeNode(current);

	}
	if (current == NULL) {
		printf("Error. Queue should never be empty\n");
		exit(-1);
	}
	LinkedList *ret = current->openset;
	return ret;
}

int inList(LinkedList *ll, int i) {
	LinkedList *head = ll;
	while (head!=NULL) {
		if (head->meter == i) return 1;
		head=head->next;
	}
	return 0;
}

void print_solution(LinkedList *ll, int size) {
	int i;
	for (i=1;i<=size;i++) {
		if (inList(ll,i)==0) {
			printf("%d ",i);
		}
	}
	printf("\n");
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
        Wrapper *array  = malloc(dangerous*sizeof(Wrapper));
        for (i=0;i<dangerous;i++) {
            if ((dummy = fscanf(file,"%d",&num)) == 0) {
		printf("Error reading number first number\n");
		exit(1);
            }
            fillWrapper(&array[i],file,num);
        }
	qsort(array,dangerous,sizeof(Wrapper),compare);
	Wrapper *root = arrayToLinkedWrapper(array,dangerous);
	LinkedList *not_solution = build_tree(root);
	print_solution(not_solution,meters);
	return 0;
}
