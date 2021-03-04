#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

// Structs
// This is the Failfish Struct
struct failfish_struct {
	int sequence_number;
	struct failfish_struct *next;
	struct failfish_struct *prev;
};
typedef struct failfish_struct failfish;

// This is the Failfish Queue (aka Pond) Struct
struct failfish_queue_struct {
	char *pondname;
	int n;
	int e;
	int th;
	failfish *head;
	failfish *tail;
};
typedef struct failfish_queue_struct failfish_queue;

// Function Prototypes
void remove_crlf(char *s); //Remove Carriage Return
int get_next_nonblank_line(FILE *ifp, char *buf, int max_length); //Get the Next Nonblank Line from Buffer
void initialize_ponds(FILE *ifp, failfish_queue **ponds); //Initialize the List of Ponds
void print_failfish_queue(failfish_queue *q); //Print the Queue of Failfish
void print_pond_status(failfish_queue **ponds); //Print the Current Status of All Ponds
void first_course(failfish_queue *q); //Execute the Logic for the First Course
void run_first_course(failfish_queue **ponds); //Run the First Course from a List of Ponds
void second_course(failfish_queue *q); //Execute the Logic for the Second Course
void run_second_course(failfish_queue **ponds); //Run the Second Course from a List of Ponds
int calculate_remaining_failfish(failfish_queue **ponds); //Calculate the Remaining Number of Failfish
int index_of_removal_head(failfish_queue **ponds); //Determine which Head to Remove

// Constructor Prototypes
failfish *create_failfish(int sequence_number); //Constructor for Failfish
failfish_queue *create_failfish_queue(char *pondname, int n, int e, int th); //Constructor for Failfish Queues

// Destructor Prototypes
void dispose_failfish(failfish *delFailfish); //Destructor for Failfish
void dispose_failfish_queue(failfish_queue *delFailfishQueue); //Destructor for Failfish Queues
void dispose_ponds_list(failfish_queue **ponds); //Destructor for the Ponds List

// The Main Function
int main() {
	//Report Memory Leaks When the Program Exits
	atexit(report_mem_leak);

	//Set the Input File to ifp and the Output File to ofp
	FILE *ifp = fopen("cop3502-as2-input.txt", "r");
	FILE *ofp = fopen("cop3502-as2-output-prats-christopher.txt", "w");

	//Create the List of Ponds
	failfish_queue **ponds = calloc(10, sizeof(failfish_queue));

	//Initialize the List of Ponds
	initialize_ponds(ifp, ponds);

	//Print the Initial Pond Status
	printf("Initial Pond Status\n");
	print_pond_status(ponds);

	//Run the First Course
	printf("\nFirst Course\n");
	run_first_course(ponds);

	//Print the Status After the First Course
	printf("\nEnd of Course Pond Status\n");
	print_pond_status(ponds);

	//Run the Second Course
	printf("\nSecond Course\n");
	run_second_course(ponds);

	//Dispose of the Ponds List
	dispose_ponds_list(ponds);

	//Close the Input and Output Files Prior to Exit
	fclose(ifp);
	fclose(ofp);

	return 0;
}

/*
 * Remove Carriage Return. This function is from the lectures by the professor,
 * Matthew Gerber. We are permittied to use this code in our assignments.
 * Its Purpose is to Remove Carriage Returns and Line Feeds from the End of a String
 */
void remove_crlf(char *s) {
	/*
	 * Remember that in C, a string is an array of characters ending with a '\0' character.  We
	 * are given s, a pointer to this array.  We want to find the end of the string that s points
	 * to.  We know that strlen() will give us the *length* of s, so we add s's length to its
	 * location to find the end of s.
	 */

	char *t = s + strlen(s);

	//t is now at s's null terminator.

	t--;

	//t now points to the last character of s.  (Importantly, we haven't changed s.)

	/*
	 * \n is the UNIX-like newline character.  MS-DOS and Windows occasionally add \r, which is a
	 * separate "carriage return" character.  We need to find and clobber both.
	 */

	/*
	 * We have two cases where t can end up to the left of s: if we're passed a zero-length string,
	 * or if literally the entire string is made up of \n and/or \r.  Stop everything if t ends up
	 * to the left of s to prevent bad things from happening in that case.
	 */

	//Look at the contents of t as a single character.  As long as it's still a newline or a CR...

	while((t >= s) && (*t == '\n' || *t == '\r')) {
		*t = '\0';  // ...clobber it by overwriting it with null, and...
		t--;        // decrement t *itself (i.e., change the pointed location)

		/*
		 * t now still points to the last character of s, and we keep going until it's something
		 * other than a CR or newline.
		 */
	}
}

/*
 * This function will write the next line from ifp that isn't blank into buf, removing any
 * newline characters.  It writes a zero length string at eof, and only then.  It returns
 * true if it successfully read a line and false if it didn't. This function is from the
 * lectures by the professor, Matthew Gerber.
 */
int get_next_nonblank_line(FILE *ifp, char *buf, int max_length) {
	buf[0] = '\0';

	while(!feof(ifp) && (buf[0] == '\0')) {
		fgets(buf, max_length, ifp);
		remove_crlf(buf);
	}

	if(buf[0] != '\0') {
		return 1;
	}
	else {
		return 0;
	}
}

// This Function will Initialize the List of Ponds
void initialize_ponds(FILE *ifp, failfish_queue **ponds) {
	//Get the Number of Ponds in Use
	char buf[256];
	get_next_nonblank_line(ifp, buf, 255);
	remove_crlf(buf);
	int nPonds = atoi(buf);

	//Create the Failfish Queue (Ponds) from the File
	for (int i = 0; i < nPonds; i++) {
		//Read the Failfish Queue Information into the Buffer
		get_next_nonblank_line(ifp, buf, 255);
		remove_crlf(buf);

		//Convert the Buffer's Data into Variables
		int g, n, e, th;
		char pondname[256];
		sscanf(buf, "%d %s %d %d %d", &g, pondname, &n, &e, &th);

		//Add the Failfish Queue into the Ponds List
		ponds[g - 1] = create_failfish_queue(pondname, n, e, th);
	}
}

// This Function will Print a Failfish Queue
void print_failfish_queue(failfish_queue *q) {
	failfish *currentFailfish = q->head;
	printf("%s ", q->pondname);

	//Print the Failfish Queue if it is Not Empty
	if (currentFailfish != NULL) {
		do {
			printf("%d ", currentFailfish->sequence_number);
			currentFailfish = currentFailfish->next;
		} while(currentFailfish != q->head);
	}
	printf("\n");
}

// This Function will Print the Current Status of All Ponds
void print_pond_status(failfish_queue **ponds) {
	for (int i = 0; i < 10; i++) {
		//Only Print Ponds with Failfish
		if (ponds[i] != NULL) {
			printf("%d ", i + 1);
			print_failfish_queue(ponds[i]);
		}
	}
}

// This Function will Execute the Logic on a Failgroup for the First Run
void first_course(failfish_queue *q) {
	failfish *currentFailfish = q->head;
	failfish *delFailfish;

	//Execute the Logic while the Number of Failfish is above the Threshold
	for (int i = 1; q->n > q->th; i++) {
		if (i % q->e == 0) {
			//Print the Falifish that is being Eaten
			printf("Failfish %d eaten\n", currentFailfish->sequence_number);

			//Reduce the Number of Failfish in the Queue
			q->n--;

			//Remove the Eaten Failfish from the Queue
			currentFailfish->prev->next = currentFailfish->next;
			currentFailfish->next->prev = currentFailfish->prev;
			delFailfish = currentFailfish;
			currentFailfish = delFailfish->prev;

			//Set the New Head if Needed
			if (q->head == delFailfish) {
				q->head = currentFailfish->next;
			}

			//Set the New Tail if Needed
			else if (q->tail == delFailfish) {
				q->tail = currentFailfish;
			}

			//Dispose of the Eaten Failfish
			dispose_failfish(delFailfish);
		}
		currentFailfish = currentFailfish->next;
	}
}

// This Function will Run the First Course from a List of Ponds
void run_first_course(failfish_queue **ponds) {
	for (int i = 0; i < 10; i++) {
		if (ponds[i] != NULL) {
			printf("\nPond %d: %s\n", i + 1, ponds[i]->pondname);
			first_course(ponds[i]);
		}
	}
}

// This Function Executes the Logic on a Failgroup for the Second Run
void second_course(failfish_queue *q) {
	if (q->head == q->tail) {
		printf("head is tail\n");
		q->head->sequence_number = -1000;
	}
	else {
		q->n--;
		failfish *delFailfish = q->head;
		delFailfish->prev->next = delFailfish->next;
		delFailfish->next->prev = delFailfish->prev;
		q->head = delFailfish->next;
		q->head->prev = q->tail;
		q->tail->next = q->head;
		dispose_failfish(delFailfish);
	}
}

// This Function will Run the Second Course from a List of Ponds
void run_second_course(failfish_queue **ponds) {
	//Find the Total Number of Remaining Failfish
	int remainingFailfish = calculate_remaining_failfish(ponds);
	int removeIndex;

	//Execute the Second Course until there is 1 Failfish Left
	while (remainingFailfish > 1) {
		removeIndex = index_of_removal_head(ponds);
		printf("Eaten: Failfish %d from pond %d\n", ponds[removeIndex]->head->sequence_number, removeIndex + 1);
		second_course(ponds[removeIndex]);
		remainingFailfish--;
	}

	//Print the Remaining Failfish
	for (int i = 0; i < 10; i++) {
		if (ponds[i] != NULL) {
			//From the Only Pond with Any Failfish
			if (ponds[i]->n > 0) {
				printf("Failfish %d from pond %d remains\n", ponds[i]->head->sequence_number, i + 1);
				break;
			}
		}
	}
}

// This Function will Calculate the Number of Failfish Remaining in a Pond
int calculate_remaining_failfish(failfish_queue **ponds) {
	int remainingFailfish = 0;

	//Calculate the Number of Failfish in Each Failgroup
	for (int i = 0; i < 10; i++) {
		if (ponds[i] != NULL) {
			remainingFailfish += ponds[i]->n;
		}
	}

	return remainingFailfish;
}

// This Function will Determine which Head to Remove in the Second Course
int index_of_removal_head(failfish_queue **ponds) {
	int highestNum = -1;
	int removeIndex;

	/*
	 * Start from the Lowest to the Highest Failgroup Id to Ensure that the Lowest Id is
	 * Selected in a Tie
	 */
	for (int i = 0; i < 10; i++) {
		if (ponds[i] != NULL) {
			/*
			 * If the Value of the Current Head is Larger than the Current
			 * Highest Value, Set it's Head Value to highestNum and set it's
			 * index to the Remove index.
			 */
			if (ponds[i]->head->sequence_number > highestNum) {
				highestNum = ponds[i]->head->sequence_number;
				removeIndex = i;
			}
		}
	}

	return removeIndex;
}

/*
 * This section includes the functions for constructing the various
 * objects used in this program
 */

// This Function will Create and Return a New Failfish
failfish *create_failfish(int sequence_number) {
	failfish *newFailfish = malloc(sizeof(failfish));
	newFailfish->sequence_number = sequence_number;
	newFailfish->next = NULL;
	newFailfish->prev = NULL;
	return newFailfish;
}

// This Function will Create and Return a New Failfish Queue
failfish_queue *create_failfish_queue(char *pondname, int n, int e, int th) {
	failfish_queue *newFailfishQueue = malloc(sizeof(failfish_queue));
	newFailfishQueue->pondname = strdup(pondname);
	newFailfishQueue->n = n;
	newFailfishQueue->e = e;
	newFailfishQueue->th = th;
	newFailfishQueue->head = NULL;
	newFailfishQueue->tail = NULL;

	//Create the Failfish that are Part of the Queue
	failfish *newFailfish;
	for (int i = newFailfishQueue->n; i > 0; i--) {
		//Set the Last Failfish
		if (i == newFailfishQueue->n) {
			newFailfish = create_failfish(i);
			newFailfishQueue->head = newFailfish;
			newFailfishQueue->tail = newFailfish;
			newFailfish->prev = newFailfish;
			newFailfish->next = newFailfish;
		}

		//Set the Other Failfish in the Queue
		else {
			newFailfish = create_failfish(i);
			newFailfish->next = newFailfishQueue->head;
			newFailfish->prev = newFailfishQueue->tail;
			newFailfishQueue->head->prev = newFailfish;
			newFailfishQueue->tail->next = newFailfish;
			newFailfishQueue->head = newFailfish;
		}
	}

	//Return the Created Failfish Queue
	return newFailfishQueue;
}

/*
 * This section includes the functions for destroying the various
 * objects used in this program
 */

// This Function will Destroy a Failfish that was Constructed
void dispose_failfish(failfish *delFailfish) {
	free(delFailfish);
}

// This Function will Destroy a Failfish Queue that was Constructed
void dispose_failfish_queue(failfish_queue *delFailfishQueue) {
	failfish *currentFailfish = delFailfishQueue->head;
	failfish *nextFailfish;

	//Empty the Failfish Queue if it is Not Empty
	if (currentFailfish != NULL) {
		//Empty the Failfish Queue
		do {
			nextFailfish = currentFailfish->next;
			dispose_failfish(currentFailfish);
			currentFailfish = nextFailfish;
		} while (currentFailfish != delFailfishQueue->head);
	}

	//Free the List itself Once it is Empty
	free(delFailfishQueue->pondname);
	free(delFailfishQueue);
}

// This Function will Destroy a Ponds List that was Constructed
void dispose_ponds_list(failfish_queue **ponds) {
	for (int i = 0; i < 10; i++) {
		if (ponds[i] != NULL) {
			dispose_failfish_queue(ponds[i]);
		}
	}
	free(ponds);
}
