#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

// Structs
// This is the Failfish Struct
typedef struct failfish {
	int sequence_number;
	struct failfish *next;
	struct failfish *prev;
} failfish;

// This is the Failfish Queue (aka Pond) Struct
typedef struct failfish_queue {
	char *pondname;
	int n;
	int e;
	int th;
	failfish *head;
	failfish *tail;
} failfish_queue;

// Function Prototypes
void remove_crlf(char *s); //Remove Carriage Return
int get_next_nonblank_line(FILE *ifp, char *buf, int max_length); //Get the Next Nonblank Line from Buffer
void print_failfish_queue(failfish_queue *q); //Print the Queue of Failfish

// Constructor Prototypes
failfish *create_failfish(int sequence_number); //Constructor for Failfish
failfish_queue *create_failfish_queue(char *pondname, int n, int e, int th); //Constructor for Failfish Queues

// Destructor Prototypes
void dispose_failfish(failfish *delFailfish); //Destructor for Failfish
void dispose_failfish_queue(failfish_queue *delFailfishQueue); //Destructor for Failfish Queues

// The Main Function
int main() {
	//Report Memory Leaks When the Program Exits
	atexit(report_mem_leak);

	//Set the Input File to ifp and the Output File to ofp
	FILE *ifp = fopen("cop3502-as2-input.txt", "r");
	FILE *ofp = fopen("cop3502-as2-output-prats-christopher.txt", "w");

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

// This Function will Print a Failfish Queue
//TODO: FINISH
void print_failfish_queue(failfish_queue *q) {
	failfish *currentFailfish = q->head;

	//Print the Failfish Queue if it is Not Empty
	if (currentFailfish != NULL) {
		do {
		} while(currentFailfish != q->head);
	}
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
//TODO: FINISH
failfish_queue *create_failfish_queue(char *pondname, int n, int e, int th) {
	failfish_queue *newFailfishQueue = malloc(sizeof(failfish_queue));
	newFailfishQueue->pondname = strdup(pondname);
	newFailfishQueue->n = n;
	newFailfishQueue->e = e;
	newFailfishQueue->th = th;
	newFailfishQueue->head = NULL;
	newFailfishQueue->tail = NULL;
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
