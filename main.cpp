#include "FunctionsForMain.h"
using namespace std;

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "ru");
	
	string cycleLengthFile;
	string cycleLengthFileAdd;
	string logsFile;
	
	oint128 countDiag;
	
	int dimension;
	int sizeDiag;
	int sizeDiagBegin, sizeDiagEnd;
	int setTime;
	
	char mode;
	char modeAdd;
	char method;
	
	float memoryLimit;
	bool isLogs;

	ofstream foutLogs;
	
	countDiag = 0;
	dimension = 0;
	sizeDiag = 0;
	sizeDiagBegin = 0, sizeDiagEnd = 0;
	setTime = 0;	
	
	mode = argv[1][0];
	
	memoryLimit = 0.0;
	isLogs = false;
	
	switch(mode) {
		case 'l':
		case 'c':
			modeAdd = argv[1][1];
		case 'd':
		case 'g':
		case 'p':
		case 't':
		case 'o':
			method = argv[2][0];
			dimension = atoi(argv[3]);
			sizeDiag = atoi(argv[4]);

			countDiag = getCountDiag(sizeDiag, dimension);

			logsFile = getNameFile(method, sizeDiag, dimension, 'l');
			foutLogs.open(logsFile, ofstream::app);
			
			/*если будем создавать диаграммы больше 121 размера 2D
			или больше 45 размера 3D, 
			или больше 30 размера 4D, нужно будет дополнить*/
			break;
			
		default:
			break;
	}
	
	switch(mode) {
		case 'u': {
			method = argv[2][0];
			dimension = atoi(argv[3]); // 2-3-4D
			sizeDiagBegin = atoi(argv[4]);
			sizeDiagEnd = atoi(argv[5]);
			
			firstCycleAndDim(method, dimension, sizeDiagBegin, sizeDiagEnd);
			break;
		}
		case 'h': { // минимальное-максимальное-среднее количество циклов
			method = argv[2][0];
			dimension = atoi(argv[3]); // 2-3-4D
			sizeDiagBegin = atoi(argv[4]);
			sizeDiagEnd = atoi(argv[5]);
			
			calcAveCycles(method, dimension, sizeDiagBegin, sizeDiagEnd);
			break;
		}
		case 'e':
		{
			method = argv[2][0];
			dimension = atoi(argv[3]); // 2-3-4D
			sizeDiagBegin = atoi(argv[4]);
			sizeDiagEnd = atoi(argv[5]);
			
			getMeanFirstCL(method, dimension, sizeDiagBegin, sizeDiagEnd);
			
			break;
		}
		case 'z': // поиск дубликатов (по разбиению на длины циклов)
		{
			method = argv[2][0];
			dimension = atoi(argv[3]); // 2-3-4D
			sizeDiagBegin = atoi(argv[4]);
			sizeDiagEnd = atoi(argv[5]);
			
			getDublicateCL(method, dimension, sizeDiagBegin, sizeDiagEnd);
		
			break;
		}
		case 'l':{ // получить список диаграмм с максимальным количеством циклов
			method = argv[2][0];
			dimension = atoi(argv[3]); // 2-3-4D
			sizeDiagBegin = atoi(argv[4]);
			sizeDiagEnd = atoi(argv[5]);
			
			getDiagMaxNumbCl(method, dimension, sizeDiagBegin, sizeDiagEnd);
		
			break;
		case 'q': { // вычисление среднего значения длин i-ого цикла по таблиц юнга dimension-D размера от sizeDiagBegin до sizeDiagEnd
		}
			method = argv[2][0];
			dimension = atoi(argv[3]); // 2-3-4D
			sizeDiagBegin = atoi(argv[4]);
			sizeDiagEnd = atoi(argv[5]);

			getMean_MeanCL_forDiagDifSizes(method, dimension, sizeDiagBegin, sizeDiagEnd);
			
			break;
		}
		case 'g': {	// генерация диаграмм (2D, 3D)
			setTime = atoi(argv[5]);
   			memoryLimit = atof(argv[6]);
   			isLogs = atoi(argv[7]);

			cycleLengthFile = getNameFile(method, sizeDiag, dimension, 'g');
    			cycleLengthFileAdd = getNameFile(method, sizeDiag, dimension, 'a');

			launchGeneration(cycleLengthFile, cycleLengthFileAdd, countDiag, sizeDiag, method, dimension, setTime, memoryLimit, foutLogs, isLogs);
			break;
		}
		
		case 'c': {
			string calc1File;
			int maxSizeVec;
			
			cycleLengthFileAdd = getNameFile(method, sizeDiag, dimension, 'a');
			calc1File = getNameFile(method, sizeDiag, dimension, '1');
			maxSizeVec = getStringAdd(cycleLengthFileAdd, '1', foutLogs);

			switch(modeAdd) { 
				case '1': {
					cycleLengthFile = getNameFile(method, sizeDiag, dimension, 'g');
					getProcessedCyclesLength(cycleLengthFile, calc1File, maxSizeVec, foutLogs);
					break;
				}
				
				case '2': {
					int numbDiag;
					numbDiag = getStringAdd(cycleLengthFileAdd, '2', foutLogs);
					
					getParamsCycleLengths(method, sizeDiag, dimension, maxSizeVec, numbDiag, calc1File, 2, '2', foutLogs);			
					getParamsCycleLengths(method, sizeDiag, dimension, maxSizeVec - 1, numbDiag, calc1File, 3, '3', foutLogs);
					
					break;
				}
				
				default: 
					break;
			}
			
			break;
		}
		
		case 'p': {	// удаление повторяющихся или недописанных блоков с перезаписью
			cycleLengthFile = getNameFile(method, sizeDiag, dimension, 'g');
			cycleLengthFileAdd = getNameFile(method, sizeDiag, dimension, 'a');
		
			checkFileCorrect(cycleLengthFile, cycleLengthFileAdd, foutLogs);
    			break;
		}
		
		case 't': {	// создание файла длин циклов и их отношений для конкретной диаграммы
			int numbStr;
			string calc1File;
			string colsDiag;
			
			numbStr = atoi(argv[5]);
			calc1File = getNameFile(method, sizeDiag, dimension, '1');
			colsDiag = getNameFile(method, sizeDiag, dimension, 'c', numbStr);
			
			cutCols(numbStr, colsDiag, calc1File, foutLogs);
			break;
		}
		
		case 'o': {	// для ограничения размерности по готовым файлам 
				// if(dim > (int)(alpha * maxDim))
			oint128 maxDim;
			string cycleLengthsLimit;
			string fileAddLimit;
			float alpha;	
			
			maxDim = atoi(argv[5]);
			alpha = stod(argv[6]);
			
			cycleLengthFile = getNameFile(method, sizeDiag, dimension, 'g');
			cycleLengthsLimit = getNameFile(method, sizeDiag, dimension, 'o');
			fileAddLimit = getNameFile(method, sizeDiag, dimension, '0');
	
			cutBlocks(cycleLengthsLimit, cycleLengthFile, fileAddLimit, maxDim, alpha, foutLogs);
			break;
		}
		
		
		case 'd': {	// вывод размерностей диаграмм, заданного размера (2D, 3D)
		/*Чтобы сделать для 4D, нужна функция getDim() Diagram4D!!*/
			string dimensionsFile;
			dimensionsFile = getNameFile(method, sizeDiag, dimension, 'd');
			
			writeFile(method, sizeDiag, countDiag, dimension, dimensionsFile, foutLogs);	
			break;	
		}
	
		case 'm': { 				// вывод информации о потреблении памяти
			printMemory();
			break;
		}
		
		default: 
			break;
	}
	
	foutLogs.close();
	return 0;
}
