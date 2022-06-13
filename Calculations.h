//GNU nano 2.5.3                                      File: main.cpp                                                                           
#include "Diagram.h"
#include "StrictDiagram.h"
#include "Process.h"
#include "Enumerator.h"
#include "Partitions.h"
#include "Strategy.h"
#include "Sequence.h"
#include "Sequence3D.h"
#include "Schutzenberger.h"
#include "InputParser.h"

#include <numeric>
#include <functional>
#include <gmp.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <deque>
#include <iterator>
#include <time.h>
#include <map>
#include <chrono>
#include <gmpxx.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <iostream>
#include <ctime>
#include <list>
#include <map>

class Calculations {
	public:
	static void calculateVectorNormaliz(vector <double> &vect, oint128 dim);
	static void calculateAdjCyclesLength(vector <double> &vect, ofstream &fout);
	static double calculateMedian(vector <double> &vect);
	static double calculateDisp(vector <double> & vect, double MO, int n);
	static double calculateMO(vector <double> & vect, int n);
	static double calculateSum(vector <double> & vect);
};
