#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int global;

void logEntry(const char *func) {
	int i;
	for (i=0;i<global;i++) printf("\t");
	global++;
	printf("Entry: %s\n",func);
}

void logExit(const char *func) {
	int i;
	global--;
	for (i=0;i<global;i++) printf("\t");
	printf("Exit: %s\n",func);
}

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
	logEntry(__FUNCTION__);
	if (q->rear == NULL) {
		printf("q->rear == NULL\n");
		q->rear = nd;
		q->front = nd;
		logExit(__FUNCTION__);
		return;
	}
	(q->rear)->next_in_queue = nd;
	q->rear = nd;
	logExit(__FUNCTION__);
}

Node *removeq(struct queue *q) {
	logEntry(__FUNCTION__);
	if (q->front == NULL) {
		logExit(__FUNCTION__);
		return NULL;
	}
	Node *ret = q->front;
	q->front = (q->front)->next_in_queue;
	if (q->front == NULL) q->rear = NULL;
	logExit(__FUNCTION__);
	return ret;
}

LinkedList *deepCopyLinkedList(LinkedList *head) {
	logEntry(__FUNCTION__);
	LinkedList *copy;
	if (head == NULL) {
		copy = NULL;
	}
	else {
		copy = malloc(sizeof(LinkedList));
		copy->meter = head->meter;
		copy->next = deepCopyLinkedList(head->next);
	}
	logExit(__FUNCTION__);
	return copy;
}

Wrapper *deepCopyWrapper(Wrapper *head) {
	logEntry(__FUNCTION__);
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
	logExit(__FUNCTION__);
	return copy;
}

void deepfreeLL(LinkedList *ll) {
	logEntry(__FUNCTION__);
	LinkedList *head = ll;
	LinkedList *willy;
	while(head!=NULL) {
		willy = head;
		head=head->next;
		free(willy);
	}
	logExit(__FUNCTION__);
}

void deepfreeW(Wrapper *eminem) {
	logEntry(__FUNCTION__);
	Wrapper *head = eminem;
	Wrapper *willy;
	while (head!=NULL) {
		deepfreeLL(head->this);
		willy=head;
		head=head->next;
		free(willy);
	}
	logExit(__FUNCTION__);
}

void deepfreeNode(Node *node) {
	logEntry(__FUNCTION__);
	deepfreeW(node->remaining);
	deepfreeLL(node->openset);
	deepfreeLL(node->closet);
	free(node);
	logExit(__FUNCTION__);
}


Node *createChild(Node *parent,int key) {
	logEntry(__FUNCTION__);
	LinkedList *check =parent->closet;
	/* Checking whether given meter belongs to closed set.
	 * If so, DO NOT create child, but return NULL to caller */
	while (check!=NULL) {
		if (check->meter == key) { return NULL; }
		else check=check->next;
	}
	
	Node *ret = malloc(sizeof(Node));
	ret->next_in_queue = NULL;
	ret->openset=deepCopyLinkedList(parent->openset);
	ret->closet=deepCopyLinkedList(parent->closet);
	if (parent->remaining->next == NULL) ret->remaining = NULL;
	ret->remaining = deepCopyWrapper(parent->remaining->next);
	logExit(__FUNCTION__);
	return ret;
}

void updateOpenset(Node *node,LinkedList *ll) {
	logEntry(__FUNCTION__);
	LinkedList *entry = malloc(sizeof(LinkedList));
	entry->meter = ll->meter;
	entry->next = node->openset;
	node->openset = entry;
	logExit(__FUNCTION__);
}

void updateClosedset(Node *node,LinkedList *ll,LinkedList* key) {
	logEntry(__FUNCTION__);
	LinkedList *head = ll;
	while(head!=NULL) {
		if (head->meter == key->meter) {
			head = head->next;
			continue;
		}
		LinkedList *entry = malloc(sizeof(LinkedList));
		entry->meter = head->meter;
		entry->next = node->closet;
		node->closet = entry;
		head = head->next;
	}
	logExit(__FUNCTION__);
}

void updateRemaining(Node *node,LinkedList *key) {
	logEntry(__FUNCTION__);
	Wrapper *prev,*headW,*tmp;
	LinkedList *headLL;
	if (node->remaining == NULL) {
		printf("Zero");
		logExit(__FUNCTION__);
		return;
	}
	if (node->remaining->next == NULL)	{//Check if there in only one wrapper element in the list
		printf("Only on element\n");
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
		printf("checking for %d \n");
		 * of the Wrapper list alone.  */
		/*Checking first element */
		printf("More than one element\n");
		headLL = node->remaining->this;
		while (headLL!=NULL) {
			if (headLL->meter==key->meter) {
				deepfreeLL(node->remaining->this);
				headW = node->remaining;
				node->remaining = node->remaining->next;
				free(headW);
				break;
			}
			headLL= headLL->next;
		}
		/*Now I update the rest of the list */
		prev = node->remaining;
		headW = node->remaining->next;
		int flag;
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
				prev->next = headW->next;
				tmp = headW;
				headW = headW->next;
				free(tmp);
			}
			else headW=headW->next;
		}
	}
	logExit(__FUNCTION__);
}

print_child(Node *node) {
	printf("------------\n");
	printf("Openset:\n");
	print_ll(node->openset);
	printf("------------\n");
	printf("Closed set:\n");
	print_ll(node->closet);
	printf("------------\n");
	printf("Remaining:\n");
	print_all(node->remaining);
	printf("------------\n");
}


void printQSize(struct queue *q) {
	int cnt = 0;
	Node *head = q->front;
	while (head!=NULL) {
		cnt++;
		head=head->next_in_queue;
	}
	printf("Queue size is now %d\n",cnt);
}


LinkedList *build_tree(Wrapper *root) {
	logEntry(__FUNCTION__);
	int i;
	struct queue *q = malloc(sizeof(struct queue));;
	q->rear = NULL;
	q->front = NULL;
	Node *start = malloc(sizeof(Node));
	start->openset = NULL;
	start->closet = NULL;
	start->next_in_queue = NULL;
	start->remaining = root;
		printQSize(q);
	insert(q,start);
		printQSize(q);
	Node *current;			//Pointer used to point to removed Node from Queue.
	Node *child; 			//Pointer for new child-node of each Node
	LinkedList *head;
	while(1) {
		current=removeq(q);
		printQSize(q);
		if ((current==NULL) || (current->remaining==NULL)) {
			printf("break\n");
			break;
		}
		head = current->remaining->this;
		while(head!=NULL) {
			child = createChild(current,head->meter);
			if (child == NULL) {
				head=head->next;
				continue;	//createChild returns null only if given meter belongs to closed set
			}
			updateOpenset(child,head);	//FIX
			updateClosedset(child,current->remaining->this,head);
			updateRemaining(child,head);
			printf("Created Child:\n");
			print_child(child);
			insert(q,child);
			head=head->next;
		}
		printf("Current is \n");
		print_child(current);
		deepfreeNode(current);

	}
	if (current == NULL) {
		printf("Error. Queue should never be empty\n");
		exit(-1);
	}
	LinkedList *ret = current->openset;
	print_ll(ret);
	logExit(__FUNCTION__);
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
	global = 0;
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
	LinkedList *not_solution = build_tree(root);
	print_solution(not_solution,meters);
	return 0;
}
