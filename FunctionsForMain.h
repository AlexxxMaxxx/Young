#include "Experiments2D.h"
#include "Experiments3D.h"
#include "Experiments4D.h"
#include "Calculations.h"
#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::string> split(const std::string& s, const char * delim);
int getCountDiag(int sizeDiag, int dimension); 

string convIntStr(int value);
string getNameFile(char method, int sizeDiag, int dimension, char mode, int numbStr = 0, int sizeDiag2 = 0);
//mode u
void firstCycleAndDim(char method, int dimension, int sizeDiagBegin, int sizeDiagEnd);
// mode h
void calcAveCycles(char method, int dimension, int sizeDiagBegin, int sizeDiagEnd);
// mode e
void getMeanFirstCL(char method, int dimension, int sizeDiagBegin, int sizeDiagEnd);
//mode z
void getDublicateCL(char method, int dimension, int sizeDiagBegin, int sizeDiagEnd);

//mode l
void getDiagMaxNumbCl(char method, int dimension, int sizeDiagBegin, int sizeDiagEnd);

//mode q
void getMean_MeanCL_forDiagDifSizes(char method, int dimension, int sizeDiagBegin, int sizeDiagEnd);
void calcMeanMeanCL_forOneSize(string cycleLengths, string dopFile, string tempFile, vector <double> & meanMeanCl, int sizeDiag); // void потом заменить?

// mode g
void launchGeneration(string cycleLengthFile, string cycleLengthFileAdd, oint128 countDiag, int sizeDiag, char method, int dimension, int setTime, float memoryLimit, ofstream &logging, bool isLogs); 

int getStringAdd(string cycleLengthFileAdd, char mode, ofstream &logging);

// mode c1
void initParamRead(string &str, ifstream &finRead, const int SIZE_BLOCK, int &modulo, int &countLine);
void getProcessedCyclesLength(string cycleLengthFile, string calc1File, int maxSizeVec, ofstream &logging);
void split(std::vector<double>& vect, const std::string &str, const std::string delims, bool flag = true);
void printData(ofstream &file, vector <double> &data);
void computeCyclesLength(string calc1File, vector <double> &cyclesLength, oint128 dimDiag);

//mode c2
void getParamsCycleLengths(char method, int sizeDiag, int dimension, int maxSizeVec, int numbDiag, string fileForRead, int numbStr, char mode, ofstream &logging);
void readCycleLengths(string fileForRead, int lengthRow, int numbStr, std::vector<std::vector<double>> & cycleLengths, ofstream &logging);
void writeResultsCalc(string fileForWrite, int lengthRow, int lengthCol, std::vector<std::vector<double>> & cycleLengths);

// mode d
void writeFile(char method, int sizeDiag, oint128 countDiag, int dimension, string allDimensions, ofstream &logging); 

// mode p
void checkFileCorrect(string fileForRead, string fileAdd, ofstream &logging);
void readBlock(int modulo, int countLine, int & oldSize, int & dimension, int & countDiag, int & maxCountCycle, bool & isEdit, string str, std::vector<double> & temp, vector <string> & buffer, map <string, oint128> & dublicate, ofstream & foutWrite);
void checkBlock(int oldSize, int dimension, int & maxCountCycle, bool & isEdit, bool & flagError, std::vector<double> temp, map <string, oint128> dublicate);
void printBlock(vector <string> buffer, ofstream & foutWrite); 
void overwrite(string fileWrite, string fileRead, ofstream &logging);
void overwrite(string fileAdd, int maxCountCycle, int countDiag, ofstream &logging);

// mode m
void printMemory(); 

// mode t
void cutCols(int numbStr, string fileWrite, string fileRead, ofstream &logging);

// mode o
void cutBlocks(string fileWrite, string fileRead, string fileAdd, oint128 maxDim, float alpha, ofstream &logging);
void readBlock(int modulo, int & dimension, int & countDiag, int & maxCountCycle, oint128 maxDim, float alpha, string str, vector <string> & buffer, std::vector<double> & temp, ofstream & foutWrite) ;
 
//void updateFile(string oldFile, string newFile, char mode); // перезапись из файла в файл
//void readFile(string fileForRead, int numbColVect, int numbColFile, std::vector<std::vector<double> > &data);
//void readFile(string fileForRead, string fileForWrite, oint128 maxDim, float alpha, int &maxNumbCycle, int &numbDiag, ofstream &logging); 
//int findMax(string cycleLengthFileAdd);

