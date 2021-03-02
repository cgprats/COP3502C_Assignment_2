#include <stdio.h>
#include <stdlib.h>
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
void print_failfish_queue(failfish_queue *q);

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

// This Function will Print a Failfish Queue
void print_failfish_queue(failfish_queue *q) {
}

/*
 * This section includes the functions for constructing the various
 * objects used in this program
 */

// This Function will Create and Return a New Failfish
failfish *create_failfish(int sequence_number) {
}

// This Function will Create and Return a New Failfish Queue
failfish_queue *create_failfish_queue(char *pondname, int n, int e, int th) {
}

/*
 * This section includes the functions for destroying the various
 * objects used in this program
 */

// This Function will Destroy a Failfish that was Constructed
void dispose_failfish(failfish *delFailfish) {
}

// This Function will Destroy a Failfish Queue that was Constructed
void dispose_failfish_queue(failfish_queue *delFailfishQueue) {
}
