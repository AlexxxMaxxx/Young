#ifndef RESOURCECONTROL_H_
#define RESOURCECONTROL_H_
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

class ResourceControl {
	public:
	static void checkMemory(float memoryLimit, ofstream &logging, bool isLogs);
	static void checkTimer(time_t startTime, time_t finishTime, int setTime); 	
};
#endif
