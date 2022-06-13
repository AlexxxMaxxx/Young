#ifndef SCHUTZENBERGER_H_
#define SCHUTZENBERGER_H_
#include "Common.h"
#include "Diagram.h"
#include "Diagram3D.h"
#include "Diagram4D.h"
#include "Sequence3D.h"
#include "Process.h"
#include <map>
#include <stdio.h>
#include <sys/sysinfo.h>
using namespace std;

class Schutzenberger {
	public:

                static QC schutzenberger4D(lqc& tab, dqc* sch_path = NULL, const bool addLast = true);
                static oint128 CucleLengths(lqu &tab, dqu& sch_path);
                
                static void CucleLengths2D(map <lpc, oint128> &allTabs, lpc &tab, ofstream &output);
                static void CucleLengths3D(map <ltc, oint128> &allTabs, ltc &tab, ofstream &output);
                static void CucleLengths4D(map <lqc, oint128> &allTabs, lqc &tab, ofstream &output);
       
                static int setCucleLengths2D(map <lpc, oint128> &allTabs, ofstream &output);
                static int setCucleLengths3D(map <ltc, oint128> &allTabs, ofstream &output);
                static int setCucleLengths4D(map <lqc, oint128> &allTabs, ofstream &output);
               
                static bool cmpDiagTab(lqu newTab,  Diagram4D form);            
                static lqu addCell(oint128 x, oint128 y, oint128 z, oint128 l, lqu tab);   

                static Diagram buildDiag2D(lpc tab);
                static Diagram3D buildDiag3D(ltc tab);
                static Diagram4D buildDiag4D(lqc tab);                            
                
                static void findOcorners2D(lpc curTabs, map <lpc, oint128> &arrayTabs, Diagram form, int sizeForm, ofstream &fout_logs, bool isLogs, float memoryLimit);  
                static void findOcorners3D(ltc curTabs, map <ltc, oint128> &arrayTabs, Diagram3D form, int sizeForm, ofstream &fout_logs, bool isLogs, float memoryLimit);  
                static void findOcorners4D(lqc curTabs, map <lqc, oint128> &arrayTabs, Diagram4D form, int sizeForm, ofstream &logging, bool isLogs, float memoryLimit);  
                
                static void createTabs2D(pc newCell, lpc curTabs, map <lpc, oint128> &arrayTabs, Diagram form, int sizeForm, ofstream &fout_logs, bool isLogs, float memoryLimit);
                static void createTabs3D(tc newCell, ltc curTabs, map <ltc, oint128> &arrayTabs, Diagram3D form, int sizeForm, ofstream &fout_logs, bool isLogs, float memoryLimit);
                static void createTabs4D(QC newCell, lqc curTabs, map <lqc, oint128> &arrayTabs, Diagram4D form, int sizeForm, ofstream &logging, bool isLogs, float memoryLimit);  
                
                static void checkMemory(float memoryLimit, ofstream &fout_logs, bool isLogs);

		static void printUchar(uchar smb);
		static void printNerve(dqu sch_path);
		 // подправить!
                static void printQU(QU active); 
                static void printPU(pu active, ofstream &fout);
                
                static void printPC(pc active, ostream &output = cout);
                static void printTC(tc active);
                static void printQC(QC active, ostream &output = cout);
                
                static void printLQU(lqu tab);
                static void printLPU(lpu tab, ofstream &fout);
                
                static void printLPC(lpc tab, ostream &output = cout);
                static void printLTC(ltc tab); 
                static void printLQC(lqc tab, ostream &output = cout);
                
              //  static lqu involution(lqu tab);
};
#endif /* SCHUTZENBERGER_H_ */
