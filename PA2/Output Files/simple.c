#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct person {
	int day;
	int month;
	int year;
	struct person* next;
};

struct person* GLOBAL_HEADPTR;

// Function declarations
struct person* createNode(void);
void fillHead(struct person* nextPtr);
void fillNode(struct person* personPtr, int i);
void createList(void);
void printList(void);
void destroyList(void);

/* This function is called when the module is loaded. */
int simple_init(void)
{
		printk(KERN_INFO "Loading Module\n");
		createList();
		printList();
		
		return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) 
{
	printk(KERN_INFO "Destroying List");
	destroyList();
	printk(KERN_INFO "Removing Module\n");
}

struct person* createNode(void)
{
	return ((struct person*) kmalloc(sizeof(struct person*), GFP_KERNEL));
}

void fillHead(struct person* nextPtr)
{
	struct person* headtemp;
	
	GLOBAL_HEADPTR = (struct person*) kmalloc(sizeof(struct person*), GFP_KERNEL);
	headtemp = GLOBAL_HEADPTR;
	
	headtemp->day = 1;
	headtemp->month = 1;
	headtemp->year = 1800;
	headtemp->next = nextPtr;
	
	headtemp = NULL;
}

void fillNode(struct person* personPtr, int i) 
{	
	(personPtr -> month) = (i + 3200 ) % 12; if (personPtr -> month == 0) { personPtr -> month = 1; }
	(personPtr -> day) = ((i + 3200) % 30);
	(personPtr -> year) = ((((i + 3200) % 2000) + 1800) % 2000) + 1000;
	(personPtr -> next) = NULL;
}

void createList(void)
{
	int i; // init counter
	struct person* currentptr; // tracker
	struct person* nextptr = createNode(); // create new ref node
	
	fillHead(nextptr); // feed ref node to GLOBAL_HEADPTR and INIT GLOBAL_HEADPTR
	currentptr = nextptr; // begin fill process
	nextptr = NULL; // manage ptrs
	
	for (i = 1; i <= 4; i++)
	{
		fillNode(currentptr, i); // fill data
		if (i <= 3)
		{
			currentptr -> next = createNode(); // creat new node
			currentptr = currentptr -> next; // increment currentptr
		}
		else
		{
			currentptr -> next = NULL;
		}
	}
	
	currentptr = NULL; // manage ptrs
}

void printList(void)
{
	int i = 0;
	struct person* currentptr = GLOBAL_HEADPTR; // init currentptr
	
	printk(KERN_INFO "Linked List Contents:");
	
	for (i = 0; i < 5; i++)
	{
		printk(KERN_INFO "Index: %d\n", i);
		printk(KERN_INFO "Month: %d\n", (currentptr -> month));
		printk(KERN_INFO "Day: %d\n", (currentptr -> day));
		printk(KERN_INFO "Year: %d\n", (currentptr -> year));
		printk(KERN_INFO " ");
		
		currentptr = currentptr -> next;
	}
}

void destroyList(void)
{
	int i = 0;
	struct person* delptr = GLOBAL_HEADPTR; // start at head
	
	printk(KERN_INFO "i		M		D		Y");
	
	for (i = 0; i < 5; i++)
	{
		printk(KERN_INFO "%d		%d		%d		%d\n", i, delptr->month, delptr->day, delptr->year);
		delptr = delptr -> next;
	}
	
	delptr = NULL;
	GLOBAL_HEADPTR = NULL;
	printk(KERN_INFO "GLOBAL_HEADPTR NULLIFIED");
	
	printk(KERN_INFO "All nodes deleted.");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

