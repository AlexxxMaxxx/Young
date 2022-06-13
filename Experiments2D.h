//GNU nano 2.5.3                                      File: main.cpp     
#include "ResourceControl.h"               

class Experiments2D {
	public:
	static Diagram createDiag2D(int sizeDiag, char method, ofstream &fout_logs);
	static int processTabs2D(Diagram diag, int sizeDiag, float memoryLimit, ofstream &fout_logs, ofstream &output, bool isLogs); 
	static void calculated2D(char method, int sizeDiag, oint128 countDiag, int dimension, ofstream &fout); 
	static void calculated2D(int countDiag, int sizeDiag, ofstream &output, string saveAdd_file, char method, int set_time, float memoryLimit, ofstream &logging, bool isLogs); 
};
