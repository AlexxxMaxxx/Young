#include "Experiments2D.h"

Diagram Experiments2D::createDiag2D(int sizeDiag, char method, ofstream &fout_logs){
	Diagram diag;
	switch(method) {
		case 'r':	
			Process::rich(sizeDiag, &diag);
			break;
		case 'p':
			Process::plansh(sizeDiag - 1, &diag);		
			break;
		default:
			fout_logs << "<< Ошибка создания диаграммы >>" << endl;
			break;	
	}
	return diag;	
}

int Experiments2D::processTabs2D(Diagram diag, int sizeDiag, float memoryLimit, ofstream &fout_logs, ofstream &output, bool isLogs) { 
	map <lpc, oint128> arrayTabs; 
	Schutzenberger schutz; 
	lpc startTab; 				// таблица - есть список пар координат x, y 
	int sizeLen;
	
	startTab.push_back(make_pair(0, 0));
	schutz.findOcorners2D(startTab, arrayTabs, diag, sizeDiag, fout_logs, isLogs, memoryLimit); 
	if(isLogs) fout_logs << "diag.getDim() = " << diag.getDim() << endl; 
	if(isLogs) fout_logs << "arrayTabs.size() = " << arrayTabs.size() << endl; 
	 	
	if (arrayTabs.size() != diag.getDim()) fout_logs << "<< НЕСОВПАДЕНИЕ РАЗМЕРНОСТЕЙ ДИАГРАММЫ >>" << endl;
	sizeLen = schutz.setCucleLengths2D(arrayTabs, output); //делать список не очень удобно, но так как мы его копируем - можно зарезервировать вектор конкретной длины и удалить копию
	
	arrayTabs.clear(); 
	startTab.clear();
	return sizeLen; 
}

void Experiments2D::calculated2D(char method, int sizeDiag, oint128 countDiag, int dimension, ofstream &fout) {

	map <Diagram, oint128> diags; 
	oint128 min_dim;
	oint128 max_dim;
	oint128 dim;
	
	double ave_dim;
	int checkSize;
	Diagram temp;
	size_t i;
	
	min_dim = 0;
	max_dim = 0;
	ave_dim = 0.0;
	checkSize = 0;
	i = 0;
	
	while (i < countDiag) {
		checkSize = diags.size();
		temp = createDiag2D(sizeDiag, method, fout);
		if (i == 0) {
			min_dim = temp.getDim();
			max_dim = temp.getDim();
		}
		diags.insert(make_pair(temp, i));
		
		if (checkSize == diags.size()) i = i - 1;
    		else {
    			fout << "Диаграмма № " << i <<":" << endl;
			temp.print_cols(fout);
			
			dim = temp.getDim();
    			fout << "Ее размерность: " << dim << endl << endl;
    			if (i != 0) {
    				if (min_dim > dim) min_dim = dim;
    				if (max_dim < dim) max_dim = dim;
    			}
		}
		i = i + 1;
	}
	ave_dim = ( min_dim + max_dim ) / 2;
	fout << "Min dim = " << min_dim << endl;
	fout << "Max dim = " << max_dim << endl;
	fout << "Ave dim = " << ave_dim << endl;
	
	diags.clear();
}

void Experiments2D::calculated2D(int countDiag, int sizeDiag, ofstream &output, string saveAdd_file, char method, int set_time, float memoryLimit, ofstream &logging, bool isLogs) {
	
	ResourceControl cntrl;
		
	time_t start_time;
	time_t finish_time;
	
    	oint128 maxCountLen;
    	oint128 maxCountAdjLen;
    	oint128 dim;
    	
    	map <Diagram, oint128> diags;
    	Diagram cur_diag;
    	int checkSize;
    	int sizeLen;
    	int i;
    	
    	maxCountLen = 0; 
    	maxCountAdjLen = 0;
    	dim = 0;
    	checkSize = 0;
    	i = 0;
    	
    	start_time = time(NULL);
    	while(i < countDiag) {
		cntrl.checkMemory(memoryLimit, logging, isLogs);
    		checkSize = diags.size();
    		cur_diag = createDiag2D(sizeDiag, method, logging);
    		dim = cur_diag.getDim(); 
    		diags.insert(make_pair(cur_diag, i));
    		
    		if (checkSize == diags.size()) {
    			i = i - 1; 
    			if(isLogs) logging << "Повтор диаграммы" << endl;
    		}
    		else if(dim > 134000000) { //size 20 2D
    			i = i - 1; 
    			countDiag = countDiag - 1;
    			if(isLogs) logging << "Слишком большая размерность" << endl;
    		}
    		else { //if(dim > (int)(alpha * maxDim)) {
    		//if (i >= 4) {//
    			cur_diag.print_cols(output); 	// 1 строка 
    			output << std::flush;
        		output << sizeDiag << endl;  	// 2 строка
        		output << dim << endl;		// 3 строка 							
    			sizeLen = processTabs2D(cur_diag, sizeDiag, memoryLimit, logging, output, isLogs);	// 4 строка
    			
    			ofstream fout_add;
        	        fout_add.open(saveAdd_file);		   
    			if(sizeLen > maxCountLen) maxCountLen = sizeLen;
        	        fout_add << maxCountLen << endl;
        	        fout_add << i + 1 << endl;
			fout_add.close();
			//} 
    		}
    		//else if(isLogs) logging << "Слишком маленькая размерность" << endl;
    			
    		cntrl.checkMemory(memoryLimit, logging, isLogs);
    		finish_time = time(NULL);
		
		if(isLogs) logging << "Пройденное время = " << (int)(finish_time - start_time) << endl;
    		cntrl.checkTimer(start_time, finish_time, set_time); 
		i = i + 1;
	}
	if(isLogs) {
		logging << "\nГенерация закончена: количество сгенерированных диаграмм = " << diags.size() << endl;
		logging << "Затраченное время на вычисления = " << (int)finish_time - start_time << endl;
		logging << endl;
	}
	
	diags.clear();
}

