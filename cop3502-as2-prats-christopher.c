#include <stdio.h>
#include <stdlib.h>
#include "leak_detector_c.h"

// Function Prototypes

// Constructor Prototypes

// Destructor Prototypes

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
