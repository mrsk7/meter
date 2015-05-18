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
	struct node *next_in_queue;
};

typedef struct node Node;

struct queue {
	Node *front;
	Node *rear;
};

typedef struct queue Queue;

Wrapper *wrapperToArray(Wrapper *start,int size) {
	Wrapper *ret = malloc(size*sizeof(Wrapper));
	int i;
	Wrapper *index = start;
	for (i=0;i<size;i++) {
		ret[i].this=index->this;
		ret[i].size=index->size;
		ret[i].next=index->next;
		index = index->next;
	}
	return ret;
}


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

int compare (const void *elem1,const void *elem2) {
	Wrapper *p1 = (Wrapper*) elem1;
	Wrapper *p2 = (Wrapper*) elem2;
	return (int) (p1->size - p2->size);
}

/*
void quicksort(Wrapper **array,int first,int last) {
	int pivot,j,i;
	Wrapper *tmp = malloc(sizeof(Wrapper));
	printf("In here\n");
		print_all(*array);
	if (first<last) {
		pivot = first;
		i = first;
		j = last;
		while(i<j) {
			while (array[i]->size<=array[pivot]->size && i<last)
				i++;
			while (array[j]->size<=array[pivot]->size)
				j--;
			if (i<j) {
				printf("In here 2\n");
				tmp->this = array[i]->this;
				tmp->size = array[i]->size;
				tmp->next = array[i]->next;
				array[i]->this = array[j]->this;
				array[i]->size = array[j]->size;
				array[i]->next = array[j]->next;
				array[j]->this =tmp->this;
				array[j]->size =tmp->size;
				array[j]->next =tmp->next;
			}
		}
		printf("In here 3\n");
		print_all(*array);
		printf("Pivot %d, j %d\n",pivot,j);
		printf("Before\n");
		print_ll(array[pivot]->this);
		print_ll(array[j]->this);
		tmp->this = array[pivot]->this;
		tmp->size = array[pivot]->size;
		tmp->next = array[pivot]->next;
		array[pivot]->this = array[j]->this;
		array[pivot]->size = array[j]->size;
		array[pivot]->next = array[j]->next;
		array[j]->this =tmp->this;
		array[j]->size =tmp->size;
		array[j]->next =tmp->next;
		printf("After\n");
		print_ll(array[pivot]->this);
		print_ll(array[j]->this);
		print_all(array[0]);
		quicksort(array,first,j-1);
		quicksort(array,j+1,last);
		free(tmp);
	}
}*/

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
	//logEntry(__FUNCTION__);
	if (q->rear == NULL) {
		q->rear = nd;
		q->front = nd;
		//logExit(__FUNCTION__);
		return;
	}
	(q->rear)->next_in_queue = nd;
	q->rear = nd;
	//logExit(__FUNCTION__);
}

Node *removeq(struct queue *q) {
	//logEntry(__FUNCTION__);
	if (q->front == NULL) {
		logExit(__FUNCTION__);
		return NULL;
	}
	Node *ret = q->front;
	q->front = (q->front)->next_in_queue;
	if (q->front == NULL) q->rear = NULL;
	//logExit(__FUNCTION__);
	return ret;
}

LinkedList *deepCopyLinkedList(LinkedList *head) {
	//logEntry(__FUNCTION__);
	LinkedList *copy;
	if (head == NULL) {
		copy = NULL;
	}
	else {
		copy = malloc(sizeof(LinkedList));
		copy->meter = head->meter;
		copy->next = deepCopyLinkedList(head->next);
	}
	//logExit(__FUNCTION__);
	return copy;
}

Wrapper *deepCopyWrapper(Wrapper *head) {
	//logEntry(__FUNCTION__);
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
	//logExit(__FUNCTION__);
	return copy;
}

void deepfreeLL(LinkedList *ll) {
	//logEntry(__FUNCTION__);
	LinkedList *head = ll;
	LinkedList *willy;
	while(head!=NULL) {
		willy = head;
		head=head->next;
		free(willy);
	}
	//logExit(__FUNCTION__);
}

void deepfreeW(Wrapper *eminem) {
	//logEntry(__FUNCTION__);
	Wrapper *head = eminem;
	Wrapper *willy;
	while (head!=NULL) {
		deepfreeLL(head->this);
		willy=head;
		head=head->next;
		free(willy);
	}
	//logExit(__FUNCTION__);
}

void deepfreeNode(Node *node) {
	//logEntry(__FUNCTION__);
	deepfreeW(node->remaining);
	deepfreeLL(node->openset);
	free(node);
	//logExit(__FUNCTION__);
}


Node *createChild(Node *parent,int key) {
	//logEntry(__FUNCTION__);
	Node *ret = malloc(sizeof(Node));
	ret->next_in_queue = NULL;
	ret->openset=deepCopyLinkedList(parent->openset);
	if (parent->remaining->next == NULL) ret->remaining = NULL;
	ret->remaining = deepCopyWrapper(parent->remaining->next);
	//logExit(__FUNCTION__);
	return ret;
}

void updateOpenset(Node *node,LinkedList *ll) {
	//logEntry(__FUNCTION__);
	LinkedList *entry = malloc(sizeof(LinkedList));
	entry->meter = ll->meter;
	entry->next = node->openset;
	node->openset = entry;
	//logExit(__FUNCTION__);
}

void updateRemaining(Node *node,LinkedList *key) {
	//logEntry(__FUNCTION__);
	Wrapper *prev,*headW,*tmp;
	LinkedList *headLL;
	if (node->remaining == NULL) {
		//logExit(__FUNCTION__);
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
	//	print_all(headW);
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
	//logExit(__FUNCTION__);
}

print_child(Node *node) {
	printf("------------\n");
	printf("Openset:\n");
	print_ll(node->openset);
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
}


LinkedList *build_tree(Wrapper *root) {
	//logEntry(__FUNCTION__);
	int i;
	struct queue *q = malloc(sizeof(struct queue));;
	q->rear = NULL;
	q->front = NULL;
	Node *start = malloc(sizeof(Node));
	start->openset = NULL;
	start->next_in_queue = NULL;
	start->remaining = root;
	//	printQSize(q);
	insert(q,start);
	//	printQSize(q);
	Node *current;			//Pointer used to point to removed Node from Queue.
	Node *child; 			//Pointer for new child-node of each Node
	LinkedList *head;
	while(1) {
		current=removeq(q);
	//	printf("Just removed node :\n");
	//	print_child(current);
		if ((current==NULL) || (current->remaining==NULL)) {
			break;
		}
		head = current->remaining->this;
		while(head!=NULL) {
			child = createChild(current,head->meter);
			updateOpenset(child,head);	//FIX
			updateRemaining(child,head);
	//		print_child(child);
	//		printf("Inserting node:\n");
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
	//print_ll(ret);
	//logExit(__FUNCTION__);
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
	Wrapper *array = wrapperToArray(root,dangerous);
	print_all(root);
	//print_all(&array[0]);
	qsort(array,dangerous,sizeof(Wrapper),compare);
	printf("edw\n");
	print_all(&array[0]);
	root = array;
	LinkedList *not_solution = build_tree(root);
	print_solution(not_solution,meters);
	return 0;
}
