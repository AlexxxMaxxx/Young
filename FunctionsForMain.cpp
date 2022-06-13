#include "FunctionsForMain.h"

std::vector<std::string> split(const std::string& s, const char * delim) {

    std::string::size_type size = s.length();
    char * const buffer = new char[size + 1];
    std::vector<std::string> res;
    
    strcpy(buffer, s.c_str());
    
    char * p = strtok(buffer, delim);
    while (p)
    {
        res.push_back(std::string(p));
        p = strtok(NULL, delim);
    }
    delete[]buffer;
    return res;
}

int getCountDiag(int sizeDiag, int dimension) { //переименовать?

	std::ifstream finRead;
	string fileName;
	
	switch(dimension) {
		case 2: {
			fileName = "info_files/2D/2D_maxNumbDiag.txt";
			break;
		}
		case 3: {
			fileName = "info_files/3D/3D_maxNumbDiag.txt";
			break;
		}
		case 4: {
			fileName = "info_files/4D/4D_maxNumbDiag.txt";
			break;
		}
		default:
			break;
	}
	
	finRead.open(fileName); 
	string sLine;
	int lineNumb;
	
	sLine = "";
	lineNumb = 0;
	
	while(lineNumb != sizeDiag && getline(finRead, sLine)) { ++lineNumb; }
	
	std::vector<std::string> res = split(sLine, " ");
	return(stoi(res[1]));
}

string convIntStr(int value) {

	std::ostringstream ost;
  	ost << value;
  	return ost.str();
}

string getNameFile(char method, int sizeDiag, int dimension, char mode, int numbStr, int sizeDiag2) { //доделать?

	string extension;
	string path;
	
	extension = ".txt";

	switch(method) {
		case 'p': {
			path = "tests/" +  convIntStr(dimension) + "D/plansh/";
			break;
		}
		case 'r': {
			path = "tests/" +  convIntStr(dimension) + "D/rich/";
			break;
		}
		default: break;	
	}
	
	switch(mode) { 
		case 'a': { path += "dop_inf/di_"; break; }
		
		case 'c': { 
			return path + "cols/" + convIntStr(sizeDiag) + "/diag_" + convIntStr(numbStr) + extension; 
			break;
		}
		
		case 'r': { 
		return path + "meanCL_serialNumb/meanCl_" + convIntStr(sizeDiag) + "_" + convIntStr(sizeDiag2) + "_" + convIntStr(dimension) + "D" + extension; 
		break; 
		}
		
		case 'd': { path += "dimensions/d_"; break; }
		
		case 'g': { path += "cycle_lengths/cl_"; break; }
		
		case 'o':  { path += "cycle_lengths_limit/cll_"; break; }
		
		case '0': { path += "dop_inf_limit/dil_"; break; }
		
		case 'l': { path += "logs/l_"; break; }
		
		case '1': { path += "calculated/1/c1_"; break; }
		
		case '2': { path += "calculated/2/cl/cl_"; break; }
		
		case '3': { path += "calculated/2/acl/acl_"; break; }
		
		case 'w': { path += "meanCL_serialNumb/meanCl_"; break; }
		
		default: break;	
	}
	
	return path + convIntStr(dimension) + "D_" + convIntStr(sizeDiag) + extension;
}
// mode u
void firstCycleAndDim(char method, int dimension, int sizeDiagBegin, int sizeDiagEnd) {
	string cycleLengthFile, cycleLengthFileAdd, calcFile;
	string result = "tests/" +  convIntStr(dimension) + "D/rich/" + "firstCycleDim_" + convIntStr(dimension) + "D.txt";
	
	int countLineDop = 0;
	int countLineCalc = 0;
	int countLine = 0;
		
	string strDop = "";
	string strCalc = "";
	string str = "";
	
	int countDiag = 0;
	
	vector <double> str_vect;
	
	ofstream foutWrite;
    	foutWrite.open(result);
    	std::vector<pair <double, int>> all;
	
	for (int sizeDiag = sizeDiagBegin; sizeDiag <= sizeDiagEnd; sizeDiag++) {
		cycleLengthFileAdd = getNameFile(method, sizeDiag, dimension, 'a');
	
		ifstream finReadDop;
    		finReadDop.open(cycleLengthFileAdd); 
    		
    		countLineDop = 0;
    		
    		while(!finReadDop.eof()) {
    			strDop = "";
    			std::getline(finReadDop, strDop);
    			countLineDop++;
    			if (countLineDop == 2) {
    				countDiag = stoi(strDop);
    			}
    		}
    		
    		std::vector<pair <double, int>> firstCycleDim (countDiag);
    		finReadDop.close();
		
		calcFile = getNameFile(method, sizeDiag, dimension, '1');
		
		ifstream finReadCalc;
    		finReadCalc.open(calcFile);
    		
    		countLineCalc = 0; 
    		double elem = 0.0;
    		int index1 = 0;
    		
		while(!finReadCalc.eof()) {
			strCalc = "";
			std::getline(finReadCalc, strCalc);
			countLineCalc++;
			if (countLineCalc % 6 == 2) {
    				split(str_vect, strCalc, " ");
    				elem = str_vect[0];
    				index1 = (countLineCalc / 6);
    				firstCycleDim[index1] = make_pair(elem, -1);
    			}
    			str_vect.clear();
		} 

		finReadCalc.close();
		
		cycleLengthFile = getNameFile(method, sizeDiag, dimension, 'g');
    		
    		ifstream finRead;
    		finRead.open(cycleLengthFile); 
    		
    		int dim = 0;
    		int index = 0;
    		countLine = 0;
    		string diag = "";
    		
    		while(!finRead.eof()) {
    			str = "";
    			std::getline(finRead, str);
    			countLine++;
    			if (countLine % 4 == 1) {
    				diag = str;
    			}
    			if (countLine % 4 == 3) {
    				dim = stoi(str);
    				index = (countLine / 4);
    				//firstCycleDim[index].first = (firstCycleDim[index].first * dim) / sizeDiag;
    				//firstCycleDim[index].first = (firstCycleDim[index].first) / sizeDiag;
    		
    				firstCycleDim[index].second = dim;
   				if (firstCycleDim[index].first >= 0.3 && firstCycleDim[index].first <= 0.5) {
   					std::cout << "max cycle = " << firstCycleDim[index].first << endl;
   					std::cout << "dim = " << dim << endl;
   					std::cout << "diag = " << diag << endl;
   					std::cout << "--------------------------------" << endl;
   				}
    				all.push_back(make_pair(firstCycleDim[index].first, firstCycleDim[index].second));
    			}
    		}
    		
    		
    		finRead.close();	
		firstCycleDim.clear();
	}

	std::sort(all.begin(), all.end());
	for (int i = 0; i < all.size(); i++) {
		foutWrite << all[i].first << " " << all[i].second << endl;
	}
	all.clear();
	foutWrite.close();
}

// mode h
void calcAveCycles(char method, int dimension, int sizeDiagBegin, int sizeDiagEnd) {
	string cycleLengthFile, cycleLengthFileAdd;	
	string result = "tests/" +  convIntStr(dimension) + "D/rich/" + "meanCountCycles.txt";
	ofstream foutWrite;
    	foutWrite.open(result, ofstream::app);
	
	int countLineDop = 0;
    	string strDop = "";
    	
    	int countLine = 0;
    	string str = "";
    	
    	int countDiag = 0;
    	vector <double> str_vect;
	
	for (int sizeDiag = sizeDiagBegin; sizeDiag <= sizeDiagEnd; sizeDiag++) {
    		cycleLengthFileAdd = getNameFile(method, sizeDiag, dimension, 'a');
    		
    		ifstream finReadDop;
    		finReadDop.open(cycleLengthFileAdd); 
    		
    		countDiag = 0;
    		
    		while(!finReadDop.eof()) {
    		strDop = "";
    		std::getline(finReadDop, strDop);
    		countLineDop++;
    		if (countLineDop == 2) {
    			countDiag = stoi(strDop);
    			}
    		}
    		
    		vector <double> countCycles (countDiag);

    		finReadDop.close();
    		countLineDop = 0;
    		
    		cycleLengthFile = getNameFile(method, sizeDiag, dimension, 'g');
    		
    		ifstream finRead;
    		finRead.open(cycleLengthFile); 
    		int index = 0;
    		int min = 999999999;
    		int max = 0;
    		double ave = 0;
    		
    		while(!finRead.eof()) {
    			str = "";
    			std::getline(finRead, str);
    			countLine++;
    			if (countLine % 4 == 0) {
    				
    				split(str_vect, str, " ");
    				index = (countLine / 4) - 1;
    				countCycles[index] = str_vect.size();
    				if (countCycles[index] < min)
    					min = countCycles[index];
    				if (countCycles[index] > max)
    					max = countCycles[index];
    				str_vect.clear();
    			}
    		}
    		
    		finRead.close();
    		countLine = 0;
    		
		Calculations calc;
    		ave = calc.calculateSum(countCycles) / countDiag;

    		foutWrite << "sizeDiag = " << sizeDiag << endl;
    		foutWrite << "min = " << min << endl;
    		foutWrite << "ave = " << ave << endl;
    		foutWrite << "max = " << max << endl;
    		foutWrite << "---------------------------------" << endl;
    		
    		min = 999999999;
    		ave = 0;
    		max = 0;
    		countCycles.clear();
    	}
	foutWrite.close();
	/*
	- в цикле по размерам +
	- открыть доп файл и прочесть 2 строку - сохранить в переменную +
	- открыть файл с длинами циклов конкретного размера +
	- начать читать +
	- каждую 4 строку - перевести в строку и посчитать количество пробелов - есть количество различных циклов +
	- сохранить в массив размера (общее количество 2 строка доп файла) +
	- искать мин и макс +
	- после просуммировать и разделить на кол-во (общее количество 2 строка доп файла) +
	*/
}

// mode e
void getMeanFirstCL(char method, int dimension, int sizeDiagBegin, int sizeDiagEnd) {
	string cycleLengthFile, cycleLengthFileAdd;
	double sum = 0;
	string result = "tests/" +  convIntStr(dimension) + "D/rich/" + "meanFirstCl.txt";
	ofstream foutWrite;
    	foutWrite.open(result);
    	
	for (int sizeDiag = sizeDiagBegin; sizeDiag < sizeDiagEnd; sizeDiag++) {
    		cycleLengthFileAdd = getNameFile(method, sizeDiag, dimension, 'a');
    				
    		int del = 0;
    		int countLine = 0;
    		string str;
    		
    		vector <double> cyclesLength;
    		vector <double> str_;
    				
    		ifstream finRead;
    		finRead.open(cycleLengthFileAdd); 
    				
    		while(!finRead.eof()) {
    		str = "";
    		std::getline(finRead, str);
    		countLine++;
    		if (countLine == 1) {
    			del = stoi(str);
    			}
    		}
    				
    		finRead.close();
    		countLine = 0;	
    		cycleLengthFile = getNameFile(method, sizeDiag, dimension, '2');
    		cyclesLength.reserve(del);
    		str_.reserve(del);
    				
    		ifstream finRead2;
    		finRead2.open(cycleLengthFile); 
    		
    		while(!finRead2.eof()) {
    			str = "";
    			std::getline(finRead2, str);
    			countLine++;
    			split(str_, str, " ");
    			sum = sum + str_[0];
    			str_.clear();
    		}
    		sum = sum / del;
    		foutWrite << sum << endl;
    				
    		finRead2.close();
    		sum = 0;
    	}
    	foutWrite.close();
}

//mode z
void getDublicateCL(char method, int dimension, int sizeDiagBegin, int sizeDiagEnd){
	string tempFile, result;
	string str;
	std::vector<std::string> CL;
	std::vector<std::string> dublicate;
	std::vector<pair <int, int>> numbBlock;
	int countLine = 0;
	int maxNumbCl = 0;
	result = "tests/" +  convIntStr(dimension) + "D/rich/" + "dublicateCl.txt";
	ofstream foutWrite;
    	foutWrite.open(result);
	
	
	for (int sizeDiag = sizeDiagBegin; sizeDiag <= sizeDiagEnd; sizeDiag = sizeDiag + 1) {

		foutWrite << sizeDiag << " -- size\n";
		tempFile = getNameFile(method, sizeDiag, dimension, '1');
		
		ifstream finRead;
    		finRead.open(tempFile); 
    		
    		if(!finRead.is_open()) std::cout << "<< Не получилось открыть файл с информацией о максимальной длине циклов! >>" << endl;
		else {
    			while(!finRead.eof()) {
    				str = "";
    				std::getline(finRead, str);
    				countLine++;
    				if (countLine % 6 == 2 && str.size() > 0) {
    					CL.push_back(str);
    				}
    			}
    		}
    		
		for (size_t i = 0; i < CL.size(); ++i) {
        		for (size_t j = i+1; j < CL.size(); ++j) {
            			if (CL[i] == CL[j]) {
            				dublicate.push_back(CL[i]);
            				numbBlock.push_back(make_pair(i + 1, j + 1));
            			}
            		}
            	}

		for (int i = 0; i < dublicate.size(); i++) {
			foutWrite << dublicate[i] << " ";
			//foutWrite << numbBlock[i].first << " " << numbBlock[i].second << " ";

			int st1 = ((numbBlock[i].first * 4) - 4) + 1;
			int end1 = (numbBlock[i].first * 4);
			
			int st2 = ((numbBlock[i].second * 4) - 4) + 1;
			int end2 = (numbBlock[i].second * 4);
			
			string cycleLengthFile = getNameFile(method, sizeDiag, dimension, 'g');
			ifstream finRead2;
    			finRead2.open(cycleLengthFile); 
			
			foutWrite << st1 << " - " << end1 << "; " << st2 << " - " << end2 << " ";
			
			Diagram diag, diag2;
    			deque<uint64> cols_;
    			vector <double> strToInt;
			
			while(!finRead2.eof()) {
    				str = "";
    				std::getline(finRead2, str);
    				countLine++;
    				if (countLine == st1) {
    		
    					split(strToInt, str, " ");
    					
    					for (int k = 0; k < strToInt.size(); k++) {
    						cols_.push_back(strToInt[k]);
    					} 
    					diag.init_by_cols(cols_);
    					strToInt.clear();
    					cols_.clear();
    				}
    				else if (countLine == st2) {
    					split(strToInt, str, " ");
    					
    					for (int k = 0; k < strToInt.size(); k++) {
    						cols_.push_back(strToInt[k]);
    					}
    					diag2.init_by_cols(cols_);
    					if (diag2.is_symmetric_to(&diag)) 
    						foutWrite << "SIMM\n";
    					else foutWrite << "NOT SIMM!!!\n";
    				}
    				
    			}
    			countLine = 0;
			cols_.clear();
    			strToInt.clear();
    			finRead2.close();
			
		}
		
		CL.clear();
		dublicate.clear();
		numbBlock.clear();
		
		countLine = 0;
		finRead.close();
	}
	
	foutWrite.close();
}

//mode l
void getDiagMaxNumbCl(char method, int dimension, int sizeDiagBegin, int sizeDiagEnd) {
	string tempFile, tempFileAdd;
	string str;
	int countLine = 0;
	int maxNumbCl = 0;
	
	string path = "tests/" +  convIntStr(dimension) + "D/rich/" + "maxNumbCl.txt";
     		
    	ofstream foutWrite;
    	foutWrite.open(path);
	
	for (int sizeDiag = sizeDiagBegin; sizeDiag <= sizeDiagEnd; sizeDiag = sizeDiag + 1) {
		tempFile = getNameFile(method, sizeDiag, dimension, 'g');
    		tempFileAdd = getNameFile(method, sizeDiag, dimension, 'a');	

    		ifstream finReadDop;
    		finReadDop.open(tempFileAdd); 
    		
    		if(!finReadDop.is_open()) std::cout << "<< Не получилось открыть файл с информацией о максимальной длине циклов! >>" << endl;
    		else {
    			while(!finReadDop.eof()) {
    				str = "";
    				std::getline(finReadDop, str);
    				countLine++;
    				if (countLine == 1) {
    					maxNumbCl = stoi(str);
    				}
    			}
    		}
    		
    		string diag;
    		ifstream finRead;
    		finRead.open(tempFile); 
    		countLine = 0;
    		int counterCl = 0; 
    		
    		
    		if(!finRead.is_open()) std::cout << "<< Не получилось открыть файл с информацией о максимальной длине циклов! >>" << endl;
    		else {
    			while(!finRead.eof()) {
    				str = "";
    				std::getline(finRead, str);
    				countLine++;
    				if (countLine % 4 == 1 ) {
    					diag = str;
    				}
    				if (countLine % 4 == 0) {
    					counterCl = std::count(str.begin(), str.end(), ' ') + 1;
    					if (maxNumbCl == counterCl)
    				     		foutWrite << diag << " " << maxNumbCl << endl;
    				}
    			}
    		}
    		maxNumbCl = 0;
  		countLine = 0;
      		finReadDop.close();
    		finRead.close();
	}
	foutWrite.close();
}

//mode q
void getMean_MeanCL_forDiagDifSizes(char method, int dimension, int sizeDiagBegin, int sizeDiagEnd) {
	string tempFile;
	string result;
	vector <double> meanMeanCl (1);
	
	for (int sizeDiag = sizeDiagBegin; sizeDiag <= sizeDiagEnd; sizeDiag = sizeDiag + 1) {
		tempFile = getNameFile(method, sizeDiag, dimension, 'w');
	
		calcMeanMeanCL_forOneSize(getNameFile(method, sizeDiag, dimension, '1') , getNameFile(method, sizeDiag, dimension, 'a'), tempFile, meanMeanCl, sizeDiag);
		
	}
	// обработали все размеры по отдельности, сводим к одному результату
	result = getNameFile(method, sizeDiagBegin, dimension, 'r', 1, sizeDiagEnd);
	
	ofstream foutWrite;
    	foutWrite.open(result);
    
    	if(!foutWrite.is_open()) std::cout << "<< Не получилось открыть файл, используемый для сохранения длин циклов! >>" << endl;
    	
    	for (int numbCycle = 0; numbCycle < meanMeanCl.size(); numbCycle = numbCycle + 1) {
		foutWrite << meanMeanCl[numbCycle] << endl;
    	}
    	
    	foutWrite.close();
}

void calcMeanMeanCL_forOneSize(string cycleLengths, string dopFile, string tempFile, vector <double> & meanMeanCl, int sizeDiag) // void потом заменить
{    

    ifstream finReadDop;
    finReadDop.open(dopFile);   
    
    string str;
    int modulo;
    int countLine;
    
    int maxCountCycles;
    int numbDiag;
    int countDiag;
    	    	
    modulo = 0;
    countLine = 0;
    
    if(!finReadDop.is_open()) std::cout << "<< Не получилось открыть файл! >>" << endl;
    else {
    	while(!finReadDop.eof()) {
    	 	str = "";
    		std::getline(finReadDop, str);
    		countLine = countLine + 1;
    		if (countLine == 1) { 
    			maxCountCycles = stoi(str);
    			//std::cout << "максимальное количество циклов " << maxCountCycles << " ";
    		}
    		else if (countLine == 2) {
    			numbDiag = stoi(str);
    			//std::cout << "количество диаграмм " << numbDiag << " \n";
    		}
    	}
    } 
    
    finReadDop.close();
    
    //массив numbDiag - строк (кол-во диаграмм), maxCountCycles - столбцов (кол-во циклов - макс)
    vector< vector<double> > matrixCL(numbDiag, vector<double> (maxCountCycles, 0));
    
    ifstream finRead;
    finRead.open(cycleLengths);    
    
    ofstream foutWrite;
    foutWrite.open(tempFile);
    
    if(!foutWrite.is_open()) std::cout << "<< Не получилось открыть файл, используемый для сохранения длин циклов! >>" << endl;
	//std::cout << tempFile << endl;
	
    
    const int SIZE_BLOCK = 6;	
    str = ""; 	
    modulo = 0;
    countLine = 0;
    countDiag = 0;
    
    if(!finRead.is_open()) std::cout << "<< Не получилось открыть файл! >>" << endl;
    else {
    	while(!finRead.eof()) {
    	 	initParamRead(str, finRead, SIZE_BLOCK, modulo, countLine);
    		switch(modulo) {			
    			case 2: {
    				//std::cout << str << endl;
    				split(matrixCL[countDiag], str, " ", false);
    				countDiag = countDiag + 1;
   				break;
    			}
    			default: 
    				break;
    		}
    	}
    }
    
/*std::cout << "Полученная матрица" << "\n";  
for (int i = 0; i < matrixCL.size(); i++) {
    for (int j = 0; j < matrixCL[i].size(); j++) {
    	std::cout << matrixCL[i][j] << " ";
    }
    std::cout << endl;
}*/
    
    vector <double> summatorCols (maxCountCycles);
    //summatorCols.reserve(maxCountCycles);
    int counterElem = 0;
    
    for (int col = 0; col < maxCountCycles; col = col + 1) {
    	for (int row = 0; row < numbDiag; row = row + 1) {
    		summatorCols[col] = summatorCols[col] + matrixCL[row][col];
    		if (matrixCL[row][col] != 0) counterElem = counterElem + 1;
    	}
    	summatorCols[col] = summatorCols[col] / counterElem; 
    	summatorCols[col] = summatorCols[col] / sizeDiag; 
    	//summatorCols[col] = summatorCols[col] / sizeDiag; 
    	counterElem = 0;
    }
    
   // for (int col = 0; col < maxCountCycles; col = col + 1) {
   // 	foutWrite << summatorCols[col] << endl;
   // }
//std::cout << "meanMeanCl.size() = " << meanMeanCl.size() << endl << std::flush;;
//std::cout << "summatorCols.size() = " << summatorCols.size() << endl << std::flush;;
    if (meanMeanCl.size() < summatorCols.size()) 
    { 
    	meanMeanCl.resize(summatorCols.size());
    }
//std::cout << "meanMeanCl.size() = " << meanMeanCl.size() << endl << std::flush;;
//std::cout << "LOOK" << endl;
  
    for (int numbCycle = 0; numbCycle < summatorCols.size(); numbCycle = numbCycle + 1) {
//std::cout << "do - " << endl;
//std::cout << "do - " << meanMeanCl[numbCycle] << " ";
    	meanMeanCl[numbCycle] = meanMeanCl[numbCycle] + summatorCols[numbCycle]; //нужно нормировать на размер диаграммы?
//std::cout << "posle - " << meanMeanCl[numbCycle] << "\n";
    }

    summatorCols.clear();
    finRead.close();
    foutWrite.close();

}

void launchGeneration(string cycleLengthFile, string cycleLengthFileAdd, oint128 countDiag, int sizeDiag, char method, int dimension, int setTime, float memoryLimit, ofstream &logging, bool isLogs) {
	Experiments2D exp2D;
	Experiments3D exp3D;
	Experiments4D exp4D;
	
	
	ofstream foutWrite;
	foutWrite.open(cycleLengthFile);
	
	if(!foutWrite.is_open()) logging << "<< Не получилось открыть файл, используемый для сохранения длин циклов! >>" << endl;
	
	switch(dimension) {
		case 2: {
			exp2D.calculated2D(countDiag, sizeDiag, foutWrite, cycleLengthFileAdd, method, setTime, memoryLimit, logging, isLogs); 
			break;
		}
		case 3: {
			exp3D.calculated3D(countDiag, sizeDiag, foutWrite, cycleLengthFileAdd, method, setTime, memoryLimit, logging, isLogs); 
			break;
		}
		case 4: {
			exp4D.calculated4D(countDiag, sizeDiag, cycleLengthFileAdd, method, setTime, memoryLimit, foutWrite, logging, isLogs);
			break;
		}
		default: break;		
	}
	
	foutWrite.close();
}

int getStringAdd(string cycleLengthFileAdd, char mode, ofstream &logging) {

	ifstream finRead;
	string str;
	
    	int retVal;
    	int countLine;
    	
    	finRead.open(cycleLengthFileAdd);
    	countLine = 0;
    	    		
    	if(!finRead.is_open()) logging << "<< Не получилось открыть файл с информацией о максимальной длине циклов! >>" << endl;
    	else {
    		while(!finRead.eof()) {
    			str = "";
    			std::getline(finRead, str);
    			countLine++;
    			if(mode == '1' & countLine == 1) retVal = stoi(str);
    			else if (mode == '2' & countLine == 2) retVal = stoi(str);
    		}
    	}
    	
    	finRead.close();  
    	
    	return retVal;
}

void initParamRead(string &str, ifstream &finRead, const int SIZE_BLOCK, int &modulo, int &countLine) {

	str = "";
    	std::getline(finRead, str);
    	countLine = countLine + 1;
    	modulo = countLine % SIZE_BLOCK;
}
    		 	
// обработка файла с длинами циклов - для первого этапа вычислений
void getProcessedCyclesLength(string cycleLengthFile, string calc1File, int maxSizeVec, ofstream &logging) {

	ofstream foutWrite;
	ifstream finRead;
	
    	foutWrite.open(calc1File);
    	foutWrite.close();
    	
    	finRead.open(cycleLengthFile);
    	
    	vector <double> cyclesLength;

    	oint128 dimDiag;
    		
    	const int SIZE_BLOCK = 4;
    	string str;
    	int modulo;
    	int countLine;

    	cyclesLength.reserve(maxSizeVec);
    	
    	modulo = 0;
    	countLine = 0;
    	
    	if(!finRead.is_open()) logging << "<< Не получилось открыть файл с длинами циклов! >>" << endl;
    	else {
    		while(!finRead.eof()) {
    		 	initParamRead(str, finRead, SIZE_BLOCK, modulo, countLine);

    			switch(modulo) {
    			
    				case 3: {
    					dimDiag = stoi(str);
    					break;
    				}
    				
    				case 0: {
    					split(cyclesLength, str, " ");
    					computeCyclesLength(calc1File, cyclesLength, dimDiag);
    					cyclesLength.clear();
    					break; 
    				}
    				default: 
    					break;
    			}
    		}
    	}
    	
    	finRead.close();
}


void split(std::vector<double>& vect, const std::string &str, const std::string delims, bool flag) {
	
	std::istringstream iss(str);
	std::vector<std::string> strs {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
	int index = 0;
	
	for (auto& i : strs) { 
		if (flag) { vect.push_back(stod(i)); }
		else 
		{ vect[index] = stod(i);
		index = index + 1; }
	}
}

void printData(ofstream &fout, vector <double> &data) {

	int i;
	i = 0;
	
	while(i < data.size()) {
		fout << data[i] << " ";
		i = i + 1;
	} fout << endl;
}

void computeCyclesLength(string calc1File, vector <double> &cyclesLength, oint128 dimDiag) {
	
	Calculations calc;
	
	ofstream foutWrite;
	foutWrite.open(calc1File, ofstream::app);
	 
	std::sort(cyclesLength.begin(), cyclesLength.end(), [](const double &a, const double &b) {return a > b;});
	
	// 1 - отсортированные длины циклов
	printData(foutWrite, cyclesLength);
	
	// 2 - отнормированные длины циклов
	calc.calculateVectorNormaliz(cyclesLength, dimDiag);
	printData(foutWrite, cyclesLength);
	
	// 3 - отношения соседних длин циклов
	calc.calculateAdjCyclesLength(cyclesLength, foutWrite);
	
	// 4 - max
	foutWrite << cyclesLength.front() * dimDiag << " ";
	foutWrite << cyclesLength.front() << endl;
	
	// 5 - min
	foutWrite << cyclesLength.back() * dimDiag << " ";
	foutWrite << cyclesLength.back() << endl;
	
	// 6 - median
	foutWrite << calc.calculateMedian(cyclesLength) * dimDiag << " ";
	foutWrite << calc.calculateMedian(cyclesLength) << endl;
	
	foutWrite.close();
}


void getParamsCycleLengths(char method, int sizeDiag, int dimension, int maxSizeVec, int numbDiag, string fileForRead, int numbStr, char mode, ofstream &logging) {

	std::vector<std::vector<double> > cycleLengths (maxSizeVec, std::vector<double>(numbDiag));
	readCycleLengths(fileForRead, maxSizeVec, numbStr, cycleLengths, logging);
	
	string fileForWrite;
	fileForWrite = getNameFile(method, sizeDiag, dimension, mode);
	
	writeResultsCalc(fileForWrite, maxSizeVec, numbDiag, cycleLengths);
	
	cycleLengths.clear();
}

void readCycleLengths(string fileForRead, int lengthRow, int numbStr, std::vector<std::vector<double>> & cycleLengths, ofstream &logging) {

	ifstream finRead;
	finRead.open(fileForRead);
	
	vector <double> tempVect;
	
	const int SIZE_BLOCK = 6;
    	int modulo;
    	int countLine;
    	int index;
	int i;
    	string str;
    	
    	modulo = 0;
    	countLine = 0;
    	
    	if(!finRead.is_open()) logging << "<< Не получилось открыть файл с длинами циклов! >>" << endl;
    	else {
    		while(!finRead.eof()) { //до конца файла
    			initParamRead(str, finRead, SIZE_BLOCK, modulo, countLine);
    			
    			// считываем строку с длинами циклов (отношениями соседних длин циклов)
			if(modulo == numbStr) { 
				split(tempVect, str, " ");
				
				// добавляем 0: 1 2 3 4 | 1 2 0 0 | 1 0 0 0 
				while(tempVect.size() != lengthRow) { tempVect.push_back(0.0);}
				
				index = countLine / SIZE_BLOCK;
				i = 0;
				
				while(i < cycleLengths.size()) {
					cycleLengths[i][index] = tempVect[i];
					i = i + 1;
				}
				tempVect.clear();
			}
    		}
    	}
    	
	finRead.close();
}		
void writeResultsCalc(string fileForWrite, int lengthRow, int lengthCol, std::vector<std::vector<double>> & cycleLengths) {

	Calculations calc;
	
	ofstream foutWrite;
	foutWrite.open(fileForWrite);
	
	vector <double> expectedValue (lengthRow); 	//res MO
	vector <double> dispersion (lengthRow);
	
	int i;
	i = 0;
	
	while(i < lengthRow) {
		expectedValue[i] = calc.calculateMO(cycleLengths[i], lengthCol);
		foutWrite << expectedValue[i] << " ";
		
		dispersion[i] = calc.calculateDisp(cycleLengths[i], expectedValue[i], lengthCol);
		foutWrite << dispersion[i] << " ";
				
		foutWrite << sqrt(dispersion[i]) << " ";
		std::sort(cycleLengths[i].begin(), cycleLengths[i].end(), [](const double &a, const double &b) {return a > b;});
		foutWrite << cycleLengths[i].front() << " "; //max

		while(cycleLengths[i].back() == 0) {
			cycleLengths[i].pop_back();
		}
		
		foutWrite << cycleLengths[i].back() << endl; //min
		i = i + 1;
	}
    	
	foutWrite.close();
}

void writeFile(char method, int sizeDiag, oint128 countDiag, int dimension, string allDimensions, ofstream &logging) {

	Experiments2D exp2D;
	Experiments3D exp3D;

	ofstream foutWrite;
	foutWrite.open(allDimensions, ofstream::app);
	
	if(!foutWrite.is_open()) logging << "<< Не получилось открыть файл, используемый для сохранения размерностей диаграмм! >>" << endl;
	
	switch(dimension) {
		case 2: {
			exp2D.calculated2D(method, sizeDiag, countDiag, dimension, foutWrite);
			break;
		}
		case 3: {	 
			exp3D.calculated3D(method, sizeDiag, countDiag, dimension, foutWrite);
			break;
		}
		/*case 4: {
			break;
		}*/
		default: break;		
	}
	
	foutWrite.close();
}

void checkFileCorrect(string fileForRead, string fileAdd, ofstream &logging) {

	string tempFile;
	ifstream finRead;
	ofstream foutWrite;
	
	tempFile = "tests/temp.txt";
	foutWrite.open(tempFile);
	finRead.open(fileForRead);
	
	const int SIZE_BLOCK = 4;
	bool isEdit;
	string str;
	
	int oldSize;
    	int modulo;
    	int dimension;
	int countLine;
	int countDiag;
	int maxCountCycle;
	
	isEdit = false;
	oldSize = 0;
    	modulo = 0;
    	dimension = 0;
    	countLine = 0;  
    	countDiag = 0;
    	maxCountCycle = 0;
    	
    	std::vector<double> temp;
	vector <string> buffer;
    	map <string, oint128> dublicate;
    	
    	if(!finRead.is_open()) logging << "<< Не получилось открыть файл с длинами циклов! >>" << endl;
    	else {
    		while(!finRead.eof()) { //до конца файла
    			initParamRead(str, finRead, SIZE_BLOCK, modulo, countLine);	
    			readBlock(modulo, countLine, oldSize, dimension, countDiag, maxCountCycle, isEdit, str, temp, buffer, dublicate, foutWrite);
    		}
    	}
	
	overwrite(fileAdd, maxCountCycle, countDiag, logging);
	
	if(isEdit) {
		logging << "<< Обнаружение ошибок в файлах >>" << endl;
		overwrite(fileForRead, tempFile, logging); 
	}
	
	finRead.close();
	foutWrite.close();
	
	if (remove("/home/parallels/Young/tests/temp.txt") == -1) logging << "<< Ошибка удаления временного файла >>" << endl;
}

void readBlock(int modulo, int countLine, int & oldSize, int & dimension, int & countDiag, int & maxCountCycle, bool & isEdit, string str, std::vector<double> & temp, vector <string> & buffer, map <string, oint128> & dublicate, ofstream & foutWrite) {

	bool flagError; 	
	flagError = false;
	
	buffer.push_back(str);
	
	switch(modulo) {
    		case 1: {
    			oldSize = dublicate.size();
    			dublicate.insert(make_pair(str, countLine));
    			break;
    		}
    		
    		case 3: {
    			dimension = stoi(str);
    			break;
    		}
    		
    		case 0: {
  			split(temp, str, " ");	
  			checkBlock(oldSize, dimension, maxCountCycle, isEdit, flagError, temp, dublicate);
  					
  			if(flagError == false) {
  				countDiag = countDiag + 1;
  				printBlock(buffer, foutWrite);
  			}			
  	
  			flagError = false;
  			buffer.clear();
  			temp.clear();
  			break;
    		}
    	}
}

void checkBlock(int oldSize, int dimension, int & maxCountCycle, bool & isEdit, bool & flagError, std::vector<double> temp, map <string, oint128> dublicate) {

	Calculations calc;
	
	if(temp.size() > maxCountCycle) 
  		maxCountCycle = temp.size();
  		
  	if(oldSize == dublicate.size() || calc.calculateSum(temp) != dimension) {
  		flagError = true;
  		isEdit = true;
  	}
}

void printBlock(vector <string> buffer, ofstream & foutWrite) {

	int index;
	index = 0;   
	
	while(index < buffer.size()) {
  		foutWrite << buffer[index] << endl;
  		index = index + 1;
	}
}

void overwrite(string fileWrite, string fileRead, ofstream &logging) {

	ifstream finRead;
	ofstream foutWrite;
	
	finRead.open(fileRead);
	foutWrite.open(fileWrite);
	
	string str;
	
	if(!finRead.is_open()) logging << "<< Не получилось открыть временный файл с длинами циклов! >>" << endl;
    	else {
    		while(!finRead.eof()) { //до конца файла
    			str = "";
    			std::getline(finRead, str);
    			foutWrite << str << endl;
    		}
    	}
	
	finRead.close();
	foutWrite.close();
}

/*
void initParamRead(string &str, ifstream &finRead, const int SIZE_BLOCK, int &modulo, int &countLine) {

	str = "";
    	std::getline(finRead, str);
    	countLine = countLine + 1;
    	modulo = countLine % SIZE_BLOCK;
}
*/
void cutCols(int numbStr, string fileWrite, string fileRead, ofstream &logging) {
	
	ifstream finRead;
	ofstream foutWrite;
	
	finRead.open(fileRead);
	foutWrite.open(fileWrite);
	
	const int SIZE_BLOCK = 6;
	
	string str;
	int i;
	int modulo;
	int countLine;
	int newNumbStr;
	
	i = 0;
	modulo = 0;
	countLine = 0;
	newNumbStr = ((numbStr / 4) * SIZE_BLOCK) + 1;
	
	vector <double> cycleLengths;
	vector <double> adjCycleLengths;
	
	if(!finRead.is_open()) logging << "<< Не получилось открыть файл для чтения! >>" << endl;
    	else {
    		while(!finRead.eof()) { //до конца файла
    			initParamRead(str, finRead, SIZE_BLOCK, modulo, countLine);
    			if(countLine == newNumbStr + 1) split(cycleLengths, str, " ");
    			else if(countLine == newNumbStr + 2) split(adjCycleLengths, str, " ");
    		}
    	}
    	
    	while(i < adjCycleLengths.size()) {
    		foutWrite << cycleLengths[i] << " " << adjCycleLengths[i] << endl;
    		i = i + 1;
    	}
	
	while(i < cycleLengths.size()) {
		foutWrite << cycleLengths[i] << endl;
		i = i + 1;
	}
	
	finRead.close();
	foutWrite.close();
}

void overwrite(string fileAdd, int maxCountCycle, int countDiag, ofstream &logging) {

	ofstream foutWrite;
	foutWrite.open(fileAdd);
	
	if(!foutWrite.is_open()) logging << "<< Не получилось открыть доп. файл! >>" << endl;
    	else { foutWrite << maxCountCycle << endl << countDiag << endl; }
    	
	foutWrite.close();
}

void printMemory() { /* Представление информации в понятном виде. */

	struct sysinfo si;
	sysinfo(&si);
	
	long double totalRAM = si.totalram; // 100 %
	long double freeRAM = si.freeram;

	const double percent = totalRAM / 100.0;
	double freePercent = freeRAM / percent;
	
	const double gigabyte = 1024 * 1024 * 1024;

	printf("total RAM : %5.1ld Byte \n", si.totalram );
	printf("free RAM : %5.1ld Byte \n", si.freeram );
	printf("free RAM in percent : %5.1f \n", freePercent );
	
	printf("total RAM : %5.1f GB \n", si.totalram / gigabyte);
	printf("free RAM : %5.1f GB \n", si.freeram / gigabyte);
}

void cutBlocks(string fileWrite, string fileRead, string fileAdd, oint128 maxDim, float alpha, ofstream &logging) {
	
	ofstream foutWrite;
	ifstream finRead;
	
	foutWrite.open(fileWrite);
	finRead.open(fileRead);

	const int SIZE_BLOCK = 4;
	string str;
	
	int modulo;
    	int dimension;
	int countLine;
	int countDiag;
	int maxCountCycle;
	
	modulo = 0;
    	dimension = 0;
    	countLine = 0;  
    	countDiag = 0;
    	maxCountCycle = 0;
    	
    	vector <string> buffer;
    	std::vector<double> temp;
	
	if(!finRead.is_open()) logging << "<< Не получилось открыть файл с длинами циклов! >>" << endl;
    	else {
    		while(!finRead.eof()) { //до конца файла
    			initParamRead(str, finRead, SIZE_BLOCK, modulo, countLine);	
    			readBlock(modulo, dimension, countDiag, maxCountCycle, maxDim, alpha, str, buffer, temp, foutWrite);
    		}
    	}
    	
    	overwrite(fileAdd, maxCountCycle, countDiag, logging);

	foutWrite.close();
	finRead.close();
}

void readBlock(int modulo, int & dimension, int & countDiag, int & maxCountCycle, oint128 maxDim, float alpha, string str, vector <string> & buffer, std::vector<double> & temp, ofstream & foutWrite) {

	buffer.push_back(str);
	
	switch(modulo) { 
		case 3: { // запоминаем размерность
    			dimension = stoi(str);
    			break;
    		}
    		case 0: { //  если выполняется условие - печатем блок
    			if(dimension >= (int)(alpha * maxDim)) {
    				printBlock(buffer, foutWrite);
    				
    				countDiag = countDiag + 1;
    				
    				split(temp, str, " ");	
				if(temp.size() > maxCountCycle) 
  					maxCountCycle = temp.size();
  				temp.clear();
    			}
    			
    			buffer.clear();
    			break;
    		}
	}
}

/*
void updateFile(string oldFile, string newFile, char mode) {
	ofstream foutNew;
	ifstream finOld;
	
	foutNew.open(newFile);
	finOld.open(oldFile);
	
	int sizeBlock;
    	int modulo;
    	int countLine;
    	string str;
    	
    	modulo = 0;
    	countLine = 0;
	
	if(finOld.is_open()) {
    		while(!finOld.eof()) { //до конца файла
  			str = "";
    			std::getline(finOld, str);
    			countLine = countLine + 1;
    			
    			switch(mode) {
				case 'm': sizeBlock = 6; break;
				case 'd': sizeBlock = 2; break;	
			}
			modulo = countLine % sizeBlock; // m: (1 2 3 4) 5 0; d: (1) 0 
			
			switch(mode) {
				case 'm': {
					switch(modulo) {
						case 1:
						case 2:
						case 3:
						case 4:
							foutNew << str << endl; break;	
					}
					break;
				}
				case 'd': {
					switch(modulo) { case 1: foutNew << str << endl; break; }				
					break;
					}	
			}
    		}
    	}
	foutNew.close();
	finOld.close();
}
*/

/*
void readFile(string fileForRead, int numbColVect, int numbColFile, std::vector<std::vector<double> > &data) {
	ifstream finRead;
	finRead.open(fileForRead);
	
    	int countLine;
	string str;

    	countLine = 0;  
    	
    	std::vector<double> temp;
    	
    	if(!finRead.is_open()) std::cout << "<< Не получилось открыть файл с длинами циклов! >>" << endl;
    	else {
    		while(!finRead.eof()) { //до конца файла
    			str = "";
    			std::getline(finRead, str);
    			split(temp, str, " ");
    			
    			if(!temp.empty()) data[countLine][numbColVect] = temp[numbColFile - 1];
    			temp.clear();
    			countLine = countLine + 1;
    		}
    	}  	
	
	finRead.close();
}

void readFile(string fileForRead, string fileForWrite, oint128 maxDim, float alpha, int &maxNumbCycle, int &numbDiag, ofstream &logging) {
	ifstream finRead;
	ofstream foutWrite;
	
	finRead.open(fileForRead);
	foutWrite.open(fileForWrite);
	
	const int SIZE_BLOCK = 4;
    	int modulo;
    	int countLine;
    	int i;
    	string str;
    	
    	modulo = 0;
    	countLine = 0;
 	i = 0;
    	
    	vector <string> buffer;
    	vector <double> tempVect;
    	
    	if(!finRead.is_open()) logging << "<< Не получилось открыть файл с длинами циклов! >>" << endl;
    	else {
    		while(!finRead.eof()) { //до конца файла
			initParamRead(str, finRead, SIZE_BLOCK, modulo, countLine);
    			
    			switch(modulo) {
    				case 1:
    				case 2:
    				case 3:
    					buffer.push_back(str);
    					break;
    				case 0:
    					buffer.push_back(str);	
    					if(stoi(buffer[2]) > (int)(alpha * maxDim)) { //подходит под условие
    					numbDiag = numbDiag + 1;
    						while(i < buffer.size()) {
    							foutWrite << buffer[i] << endl;
    							i = i + 1;
    						}
    					i = 0;	
    					split(tempVect, str, " ");
    					if (tempVect.size() > maxNumbCycle) maxNumbCycle = tempVect.size();
    					tempVect.clear();
    					}
    					
    					buffer.clear();
    					break;
    			}
    			
    		}
    	}

	finRead.close();
	foutWrite.close();
}

int findMax(string cycleLengthFileAdd) {
	ifstream finRead;
	finRead.open(cycleLengthFileAdd);
	
	string str;
	int countLine;
	int retVal;
	    	
	countLine = 0;
	
	if(!finRead.is_open()) std::cout << "<< Не получилось открыть файл с информацией о максимальной длине циклов! >>" << endl;
    	else {
    		while(!finRead.eof()) { 
    			str = "";
    			std::getline(finRead, str);
    			countLine++;
    			if(countLine == 1) retVal = stoi(str);
    		}
    	}
	
	finRead.close();
	return retVal;
}*/
