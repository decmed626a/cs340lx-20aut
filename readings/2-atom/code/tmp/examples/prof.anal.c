#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

long instrTotal = 0;
long *instrPerProc = 0;

FILE *file;

void OpenFile(int number) {
  	instrPerProc = (long *) calloc(sizeof(long),number);
  	assert(instrPerProc != NULL);
  	file = fopen("prof.out","w");
  	assert(file != NULL);
  	fprintf(file,"%30s %15s %10s\n","Procedure","Instructions","%");
}

void ProcedureCount(int number, int instructions) {
  	instrTotal += instructions;
  	instrPerProc[number] += instructions;
}

void ProcedurePrint(int number, char *name) {
  	if (!instrPerProc[number] > 0) 
		return;
    	fprintf(file,"%30s %15ld %9.3f\n", name, instrPerProc[number], 
			100.0 * instrPerProc[number] / instrTotal);
}

void CloseFile() {
  	fprintf(file,"\n%30s %15ld\n", "Total", instrTotal);
  	fclose(file);
}
