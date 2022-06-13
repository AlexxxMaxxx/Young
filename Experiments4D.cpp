#include "Experiments4D.h"

Diagram4D Experiments4D::createDiag4D(int sizeDiag, char method, ofstream &logging) {

	Diagram4D diag;
	double prob;
	double sum;
	
	for(size_t i = 0; i < sizeDiag; i++) { 
		switch(method) {
			case 'r': {
				diag.add_rich();
				break;
			}
			
			case 'p': {
				diag.add_pp_rand(&prob, &sum);
				break;
			}
			
			default: {
				logging << "<< Ошибка создания диаграммы >>" << endl;
				break;	
			}
		}
	}
	return diag;	
}

int Experiments4D::processTabs4D(Diagram4D diag, int sizeDiag, float memoryLimit, ofstream &logging, bool isLogs, ofstream &output) {

	map <lqc, oint128> arrayTabs; 
	Schutzenberger schutz; 
	
	lqc startTab; 
	int sizeLen;	// количество таблиц, соотв. данной диаграмме

    	
    	startTab.push_back(QC());
    	schutz.findOcorners4D(startTab, arrayTabs, diag, sizeDiag, logging, isLogs, memoryLimit); 
    	
    	output << arrayTabs.size() << endl; // 3 строка
    	sizeLen = schutz.setCucleLengths4D(arrayTabs, output); 
    	
    	arrayTabs.clear();
	startTab.clear();
	return sizeLen;
}

void Experiments4D::calculated4D(int countDiag, int sizeDiag, string saveAddFile, char method, int setTime, float memoryLimit, ofstream &output, ofstream &logging, bool isLogs) {
	
	ResourceControl cntrl;

	time_t startTime;			// для ограничения по времени
	time_t finishTime;
	
	oint128 maxCountLen; 			// максимальная длина списка длин циклов
    	oint128 maxCountAdjLen;
    	//oint128 dim;
    	
    	map <Diagram4D, oint128> diags; 	// все диаграммы 
    	Diagram4D curDiag;
    	int checkSize;			 	// для проверки от повторов при генерации диаграмм
    	int sizeLen;
    	int i;				 	// счетчик диаграмм
    	
    	maxCountLen = 0; 
    	maxCountAdjLen = 0;
    	checkSize = 0;
    	i = 0;
    	//dim = 0;
    	
    	startTime = time(NULL);
    	
    	while(i < countDiag) {
    		checkSize = diags.size(); 				// запоминаем количество диаграмм в карте
//std::cout << "iter = " << i + 1 << endl;
		curDiag = createDiag4D(sizeDiag, method, logging);
		diags.insert(make_pair(curDiag, i));
		//dim = curDiag.getDim();
//std::cout << "checkSize = " << checkSize << endl;
//std::cout << "diagsSize = " << diags.size() << endl;

		if (checkSize == diags.size()) { 	
    			i = i - 1; 
    			if(isLogs) logging << "Повтор диаграммы" << endl;
    		}
    		//else if(dim > 134000000) { // убрать потом и заменить на отсекание по готовым файлам
    		//	i = i - 1; 
    		//	countDiag = countDiag - 1;
    	//		if(isLogs) fout_logs << "Слишком большая размерность" << endl;
    	//	}
    		else {
    			//if (i >= ) {
    			curDiag.print_cols(output); 	// 1 строка
    			output << endl; 
    			output << std::flush;
    			output << sizeDiag << endl;  	// 2 строка
    			// доп переменная для задания размерности
    			
    			sizeLen = processTabs4D(curDiag, sizeDiag, memoryLimit, logging, isLogs, output);
    			
    			ofstream foutAdd;
        	        foutAdd.open(saveAddFile);
     
        	        if(sizeLen > maxCountLen) maxCountLen = sizeLen;
        	        
        	        foutAdd << maxCountLen << endl; //<< maxCountAdjLen;
        	        foutAdd << i + 1 << endl;
        	        
			foutAdd.close();
			//}
    		}
	
		cntrl.checkMemory(memoryLimit, logging, isLogs);
		finishTime = time(NULL);
    		if(isLogs) logging << "Пройденное время = " << (int)(finishTime - startTime) << endl;
		cntrl.checkTimer(startTime, finishTime, setTime); 
		
    		i = i + 1;    		
    	}
    	
    	if(isLogs) {
    		logging << endl << "Генерация закончена: количество сгенерированных диаграмм = " << diags.size() << endl;
		logging << "Затраченное время на вычисления = " << (int)(finishTime - startTime) << endl;
		logging << endl;
	}
	
    	diags.clear();
}
