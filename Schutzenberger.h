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
		static pu schutzenberger2D(lpu& tab, const bool randomize = false, dpu* sch_path = NULL, pu active = pu(0,0));
		static pc schutzenberger2D(lpc& tab, const bool randomize = false, dpc* sch_path = NULL, pc active = pc(0,0));
		static uint64 schutzenberger2D(const char* in_tab, const char* out_tab = NULL, const char* out_path = "schutzenberger2D_path.txt", 		const uint64 NUM = 0, Diagram* diagr = NULL, const bool ADD_LAST = false, const bool PRINT_NON_ACTIVE = true);
		static pu schutzenberger2D(dpu in_tab, dpu* out_tab = NULL, dpu* sch_path = NULL, pu active = pu(0,0));
		static pu schutzenberger2D_randbegin(dpu* in_tab, dpu* out_tab = NULL, dpu* sch_path = NULL, pu active = pu(0,0));
		static pu schutzenberger2D_no_lastadd(dpu in_tab, dpu& out_tab, dpu* sch_path = NULL);
		static pu schutzenberger2D_reverse(dpu in_tab, dpu &out_tab, mpp* sch_path);
		static void schutzenberger2D_reverse(dpu in_tab, dpu &out_tab, dpu& sch_path);
		static pu schutzenberger2D_reverse(dpu in_tab, dpu &out_tab);
		static void fullpath_schutzenberger2D(const char* IN_TAB, const char* OUT_TAB);
		static void lex_schutzenberger2D();
		static void schutzenberger2D_set_active(const char* IN_TAB, const char* OUT_TAB, const uint64 INIT, const uint64 LEN = 0, const uint64 CORNER = 0);
		static void nerve_asymptotics2D();

		static tc schutzenberger3D(ltc& tab, const bool randomize = false, dtc* sch_path = NULL, const bool addlast = true);
		static tu schutzenberger3D(const char* IN_TAB, const char* OUT_TAB = NULL, const uint64 NUM = 0);
		static tu schutzenberger3D(dtu in_tab, dtu &out_tab);
		static tu schutzenberger3D(dtu in_tab, dtu &out_tab, dtu &sch_path);
		static tu schutzenberger3D_randbegin(dtu in_tab, dtu &out_tab, const int SIZE = 2);
		static tu schutzenberger3D(dtu in_tab, dtu &sch_path, tu active);
		static void schutzenberger3D_reverse(dtu in_tab, dtu &out_tab, dtu& sch_path);
		static multimap<uint64, int> schutzenberger3D_cyclelengths(vvi cols);
		static multiset<int> schutzenberger2D_cyclelengths(di cols, bool print_dim = false);

		static void fullpath_schutzenberger3D(const char* IN_TAB, const char* OUT_TAB, const uint64 PATH_LEN);
		static void print_hops_to_img2D(map<pair<pu,pu>, int> hops, const string FNAME_IMG, const double LW = 1, const double THICK_FACTOR = 1.1);
		static void print_hops_to_png3D(map<pair<tu,tu>, int> hops, const char* FNAME_PNG);
		static void schutzenberger3D_seek_central_process();
		static void schutzenberger3D_seek_central_process2(const uint64 DIAGS_NUM, vvi* last_rows = NULL);
		static double schutzenberger3D_seek_central_process3(const uint64 DIAGS_NUM, dtu* init_tab = NULL);
		static void schutzenberger3D_seek_central_process3_process(const char* source);
		static void schutzenberger3D_seek_central_process3_get_diagfreqs(const vvi rows);
		static void schutzenberger3D_seek_central_process2_process(const char* source);

		static void add_min_coprob3D(Diagram3D& diag, dtu& tab, const uint64 ITER_NUM = 10000);

		static double calc_coprob3D(dtu& init_tab, const tu coord, const uint64 ITER_NUM = 10000);
		
		//____________________________________________________________________________________my

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
