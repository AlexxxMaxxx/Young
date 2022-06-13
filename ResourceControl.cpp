#include "ResourceControl.h"

void ResourceControl::checkTimer(time_t startTime, time_t finishTime, int setTime) {
	if ((int)(finishTime - startTime) >= setTime) exit(0);
}

void ResourceControl::checkMemory(float memoryLimit, ofstream &logging, bool isLogs) {
	struct sysinfo si;
	sysinfo(&si);

	long double totalRAM; 			// 100 %
	long double freeRAM;
	
	totalRAM = si.totalram;
	freeRAM = si.freeram;

	const double percent = totalRAM / 100.0;
	double freePercent;
	
	freePercent = freeRAM / percent;

	if(freePercent <= (double)memoryLimit) {
		logging << "<< ПРЕВЫШЕНИЕ ОГРАНИЧЕНИЯ ПО ПАМЯТИ >>" << endl;
		exit(0);
	}
	else{
		if(isLogs) logging <<  freePercent << " %" << endl;
	}
}


