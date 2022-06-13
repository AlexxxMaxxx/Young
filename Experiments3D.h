//GNU nano 2.5.3                                      File: main.cpp   
#include "ResourceControl.h"   

class Experiments3D {
	public:
	static Diagram3D createDiag3D(int sizeDiag, char method, ofstream &fout_logs);
	static int processTabs3D(Diagram3D diag, int sizeDiag, float memoryLimit, ofstream &fout_logs, bool isLogs, ofstream &output);
	static void calculated3D(char method, int sizeDiag, oint128 countDiag, int dimension, ofstream &fout);
	static void calculated3D(int countDiag, int sizeDiag, ofstream &output, string saveAdd_file, char method, int set_time, float memoryLimit, ofstream &fout_logs, bool isLogs); 
};
