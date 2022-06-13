//GNU nano 2.5.3                                      File: main.cpp   
#include "ResourceControl.h"    

class Experiments4D {
	public:
	static Diagram4D createDiag4D(int sizeDiag, char method, ofstream &logging);
	static int processTabs4D(Diagram4D diag, int sizeDiag, float memoryLimit, ofstream &logging, bool isLogs, ofstream &output);
	static void calculated4D(int countDiag, int sizeDiag, string saveAddFile, char method, int setTime, float memoryLimit, ofstream &output, ofstream &logging, bool isLogs); 
};
