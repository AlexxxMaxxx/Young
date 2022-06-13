#include "Experiments3D.h"

Diagram3D Experiments3D::createDiag3D(int sizeDiag, char method, ofstream &fout_logs) {
	Diagram3D diag;
	switch(method) {
		case 'r':	
			Process::rich3D(sizeDiag, &diag);
			break;
		case 'p':
			Process::plansh3D(sizeDiag - 1, &diag);	
			break;
		default:
			fout_logs << "<< Ошибка создания диаграммы >>" << endl;
			break;	
	}
	return diag;	
}

int Experiments3D::processTabs3D(Diagram3D diag, int sizeDiag, float memoryLimit, ofstream &fout_logs, bool isLogs, ofstream &output) { 
	map <ltc, oint128> arrayTabs;
	Schutzenberger schutz; 
	ltc startTab; 			// таблица - есть структура координат x, y, z
	int sizeLen;
	
	startTab.push_back(tc());
	schutz.findOcorners3D(startTab, arrayTabs, diag, sizeDiag, fout_logs, isLogs, memoryLimit); //
	
	if (arrayTabs.size() != diag.getDim()) fout_logs << "<< НЕСОВПАДЕНИЕ РАЗМЕРНОСТЕЙ ДИАГРАММЫ >>" << endl; 
	sizeLen = schutz.setCucleLengths3D(arrayTabs, output); 
	
	arrayTabs.clear();
	startTab.clear();
	return sizeLen;
}

void Experiments3D::calculated3D(char method, int sizeDiag, oint128 countDiag, int dimension, ofstream &fout) {
	map <Diagram3D, oint128> diags;
	
	oint128 min_dim;
	oint128 max_dim;
	oint128 dim;
	
	double ave_dim;
	int checkSize;
	Diagram3D temp;
	size_t i;
	
	min_dim = 0;
	max_dim = 0;
	ave_dim = 0.0;
	checkSize = 0;
	i = 0;
	
	while (i < countDiag) {
		checkSize = diags.size();
		temp = createDiag3D(sizeDiag, method, fout);
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

void Experiments3D::calculated3D(int countDiag, int sizeDiag, ofstream &output, string saveAdd_file, char method, int set_time, float memoryLimit, ofstream &fout_logs, bool isLogs) {

	ResourceControl cntrl;
	
	time_t start_time;			// для ограничения по времени
	time_t finish_time;
	
	oint128 maxCountLen; 			// максимальная длина списка длин циклов
    	oint128 maxCountAdjLen;
    	oint128 dim;				// количество таблиц, соотв данной диаграмме
    	
    	map <Diagram3D, oint128> diags; 	// все диаграммы 
    	Diagram3D cur_diag;
    	int checkSize;			 	// для проверки от повторов при генерации диаграмм
    	int sizeLen;
    	int i;				 	// счетчик диаграмм
    	
    	maxCountLen = 0; 
    	maxCountAdjLen = 0;
    	dim = 0;
    	checkSize = 0;
    	i = 0;
    	
    	start_time = time(NULL);
    
    	while(i < countDiag) {
    	
    		checkSize = diags.size(); 				// запоминаем количество диаграмм в карте 
    		cur_diag = createDiag3D(sizeDiag, method, fout_logs); // создание диаграммы !!!
  		dim = cur_diag.getDim();
    		diags.insert(make_pair(cur_diag, i));
    		
    		if (checkSize == diags.size()) { 	
    			i = i - 1; 
    			if(isLogs) fout_logs << "Повтор диаграммы" << endl;
    		}
    		/*else if(dim > limitDim) { // убрать потом и заменить на отсекание по готовым файлам
    			i = i - 1; 
    			countDiag = countDiag - 1;
    			if(isLogs) fout_logs << "Слишком большая размерность" << endl;
    		}*/
    		else { //if(dim > (int)(alpha * maxDim)) { // рассматривать диаграммы с макс. размер-ми
    			cur_diag.print_cols(output); 	// 1 строка
    			output << std::flush;
        		output << sizeDiag << endl;  	// 2 строка
        		output << dim << endl;		// 3 строка
        		
        		sizeLen = processTabs3D(cur_diag, sizeDiag, memoryLimit, fout_logs, isLogs, output);						// 4 строка 

        		ofstream fout_add;
        	        fout_add.open(saveAdd_file);	
        	        	   
    			if(sizeLen > maxCountLen) maxCountLen = sizeLen;
    			
    			fout_add << maxCountLen << endl; //<< maxCountAdjLen;
        	        fout_add << i + 1 << endl;
        	        
			fout_add.close();
    		}
    		//else fout_logs << "Слишком маленькая размерность" << endl;

    		cntrl.checkMemory(memoryLimit, fout_logs, isLogs);
    		
    		finish_time = time(NULL);
    		if(isLogs) fout_logs << "Пройденное время = " << (int)(finish_time - start_time) << endl;
		cntrl.checkTimer(start_time, finish_time, set_time); 
		
    		i = i + 1; 
    	} 

	if(isLogs) {
    		fout_logs << "\nГенерация закончена: количество сгенерированных диаграмм = " << diags.size() << endl;
		fout_logs << "Затраченное время на вычисления = " << (int)finish_time - start_time << endl;
		fout_logs << endl;
	}
	
	diags.clear();
}
