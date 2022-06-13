#include "Schutzenberger.h"

QC Schutzenberger::schutzenberger4D(lqc& tab, dqc* sch_path, const bool addLast) {
        uchar x;
        uchar y;
        uchar z;
        uchar l;
        
        uchar actX;
        uchar actY;
        uchar actZ;
        uchar actL;
        
        lqc::iterator iter;

        actX = tab.front().getX();	
        actY = tab.front().getY();
        actZ = tab.front().getZ();
        actL = tab.front().getL();
        tab.pop_front();

       if (sch_path != NULL)
		sch_path->clear();

	iter = tab.begin();
        while (iter != tab.end()) {
                x = iter->getX();
                y = iter->getY();
                z = iter->getZ();
                l = iter->getL();

                if ((z == actZ && l == actL) && 
                ((x == actX + 1 && y == actY) || (x == actX && y == actY + 1))
                || (x == actX && y == actY) && 
                ((z == actZ + 1 && l == actL) || (z == actZ && l == actL + 1))) {

                        iter = tab.erase(iter);
                        tab.insert(iter, QC(actX, actY, actZ, actL));
                        if (sch_path != NULL)
                        	sch_path->push_back(QC(actX, actY, actZ, actL));

                	actX = x;
                	actY = y;
                	actZ = z;
                	actL = l;
                }
                else ++iter;
        }

        if (addLast) tab.push_back(QC(actX, actY, actZ, actL));

        if (sch_path != NULL)
        	sch_path->push_back(QC(actX, actY, actZ, actL));
        return QC(actX, actY, actZ, actL); 
}

Diagram Schutzenberger::buildDiag2D(lpc tab) {
	Diagram tempDiag;
	size_t size;
	size_t i;
	
	size = tab.size();
	i = 0;
	while(i < size) {
	        pc temp = tab.front();
		if(i != 0) tempDiag.add(temp.second);	//по у
	        tab.pop_front();
	        i = i + 1;
	}   
	return tempDiag;
}

Diagram3D Schutzenberger::buildDiag3D(ltc tab) { // таблица -> диаграмма
	Diagram3D tempDiag;
	size_t size;
	size_t i;
	
	size = tab.size();
	i = 0;
	while(i < size) {
        	tc temp = tab.front();
        	if(i != 0) tempDiag.add(temp.z, temp.y); 
        	tab.pop_front();
        	i = i + 1;
    	} 
    	
 	return tempDiag;   	
}

Diagram4D Schutzenberger::buildDiag4D(lqc tab) {
	Diagram4D tempDiag;
	size_t size;
	size_t i;
	
	size = tab.size();
	i = 0;
	
	while(i < size) {
		QC temp = tab.front();
		tempDiag.add(temp.getL(), temp.getZ(), temp.getY());
		tab.pop_front();
        	i = i + 1;
	}
	
 	return tempDiag;   
}

void Schutzenberger::findOcorners2D(lpc curTabs, map <lpc, oint128> &arrayTabs, Diagram form, int sizeForm, ofstream &fout_logs, bool isLogs, float memoryLimit) {	
	mi *ocorners; 
	mi::iterator it; 
	
	Diagram tempDiag; 
	uchar x;
	uchar y;

	if(curTabs.size() > 1){
		tempDiag = buildDiag2D(curTabs);	
	}			
        ocorners = tempDiag.get_points();		
	it = ocorners->begin();
	
	while(it != ocorners->end()) {
     		y = it->first;
        	x = tempDiag.get_width(y);

            	if(x < form.get_width(y) && y < form.get_height(x)) {
    			pc newCell = make_pair(x, y);
            		createTabs2D(newCell, curTabs, arrayTabs, form, sizeForm, fout_logs, isLogs, memoryLimit);	
            	}
        it++;
        }
}

void Schutzenberger::findOcorners3D(ltc curTabs, map <ltc, oint128> &arrayTabs, Diagram3D form, int sizeForm, ofstream &fout_logs, bool isLogs, float memoryLimit) {
	mpu *ocorners; 	// места, куда можно добавить клетку в текущей табл
	mpu::iterator it;
	
	Diagram3D tempDiag;    // таблица -> диаграмму (для работы с окорнерами)(изначально размер=1!!!!)
	
	uchar x;
	uchar y;
	uchar z;
	
	if(curTabs.size() > 1){ 
		tempDiag = buildDiag3D(curTabs);
	}	
	ocorners = tempDiag.get_points();
	it = ocorners->begin();
	
	while(it != ocorners->end()) {
		z = (it->first).first;
		y = (it->first).second;
		x = tempDiag.getpointX(z, y);
		if (x < form.getpointX(z, y) && y < form.getpointY(z, x)  && z < form.getpointZ(x, y)) {
			tc newCell = tc(x, y, z);
			createTabs3D(newCell, curTabs, arrayTabs, form, sizeForm, fout_logs, isLogs, memoryLimit);
		}
		it++;
	}
}

void Schutzenberger::findOcorners4D(lqc curTabs, map <lqc, oint128> &arrayTabs, Diagram4D form, int sizeForm, ofstream &logging, bool isLogs, float memoryLimit) {

	mtid ocorners;			// места, куда можно добавить клетку в текущей табл.
	mtid::iterator it;
	
	Diagram4D tempDiag;		 // таблица -> диаграмму (для работы с окорнерами)(изначально размер=0)
	
	uchar x;
	uchar y;
	uchar z;
	uchar l;
	
	tempDiag = buildDiag4D(curTabs);
	
	ocorners = tempDiag.getOcorners();	
	it = ocorners.begin();
	
	while(it != ocorners.end()) {
		x = tempDiag.get_oX(get<0>(it->first), get<1>(it->first), get<2>(it->first));
            	y = get<2>(it->first);
            	z = get<1>(it->first);
            	l = get<0>(it->first);

		if(x < form.get_oX(l, z, y) && y < form.get_oY(l, z, x) &&
            	z < form.get_oZ(l, x, y) && l < form.get_oD(x, y, z)) { 
            		QC newCell = QC(x, y, z, l);
            		createTabs4D(newCell, curTabs, arrayTabs, form, sizeForm, logging, isLogs, memoryLimit);
            	}
            	it++;	
	}
}

void Schutzenberger::createTabs2D(pc newCell, lpc curTabs, map <lpc, oint128> &arrayTabs, Diagram form, int sizeForm, ofstream &fout_logs, bool isLogs, float memoryLimit) {
	curTabs.push_back(newCell);
    			
    	if (curTabs.size() != sizeForm) {
    		checkMemory(memoryLimit, fout_logs, isLogs);
    		findOcorners2D(curTabs, arrayTabs, form, sizeForm, fout_logs, isLogs, memoryLimit);
    	}
    	else {
    		oint128 index = arrayTabs.size();
    		if(isLogs) fout_logs << "arrayTabs.size() = " << arrayTabs.size() / 1000000 << " mln " << endl;
    		arrayTabs.insert(make_pair(curTabs, index));
    		curTabs.clear();
    	}
}

void Schutzenberger::createTabs3D(tc newCell, ltc curTabs, map <ltc, oint128> &arrayTabs, Diagram3D form, int sizeForm, ofstream &fout_logs, bool isLogs, float memoryLimit) {
	curTabs.push_back(newCell);
    			
    	if (curTabs.size() != sizeForm) {
    		checkMemory(memoryLimit, fout_logs, isLogs); 
    		findOcorners3D(curTabs, arrayTabs, form, sizeForm, fout_logs, isLogs, memoryLimit);
    	}
    	else {
    		oint128 index = arrayTabs.size();
    		if(isLogs) fout_logs << "arrayTabs.size() = " << arrayTabs.size() / 1000000 << " mln " << endl;
    		arrayTabs.insert(make_pair(curTabs, index));
    		curTabs.clear();
    	}
}

void Schutzenberger::createTabs4D(QC newCell, lqc curTabs, map <lqc, oint128> &arrayTabs, Diagram4D form, int sizeForm, ofstream &logging, bool isLogs, float memoryLimit) {
	curTabs.push_back(newCell);
	
	if (curTabs.size() != sizeForm) { 
		checkMemory(memoryLimit, logging, isLogs); 
		findOcorners4D(curTabs, arrayTabs, form, sizeForm, logging, isLogs, memoryLimit);
	}
	else { 
		oint128 index = arrayTabs.size();
		if(isLogs) logging << "arrayTabs.size() = " << arrayTabs.size() / 1000000 << " mln " << endl;
    		arrayTabs.insert(make_pair(curTabs, index));
    		curTabs.clear();
	}
 }

void Schutzenberger::CucleLengths2D(map <lpc, oint128> &allTabs, lpc &tab, ofstream &output) {
        lpc startTab;
        oint128 length;
        
        startTab = tab;
        length = 0;      
        do {              
		schutzenberger2D(tab);
		map <lpc, oint128>::iterator iterForRem = allTabs.find(tab);
  		if(iterForRem != allTabs.end()) allTabs.erase(iterForRem);
                length = length + 1;
        } while(tab!=startTab);
        
        output << length << " ";
}

void Schutzenberger::CucleLengths3D(map <ltc, oint128> &allTabs, ltc &tab, ofstream &output) {
        ltc startTab;
        oint128 length;
        
        startTab = tab;
        length = 0;
        do {       
		schutzenberger3D(tab);
		map <ltc, oint128>::iterator iterForRem = allTabs.find(tab);
  		if(iterForRem != allTabs.end()) allTabs.erase(iterForRem);
                length = length + 1;
        } while(tab!=startTab);
 	
 	output << length << " ";
}

void Schutzenberger::CucleLengths4D(map <lqc, oint128> &allTabs, lqc &tab, ofstream &output) {
        lqc startTab;
        oint128 length;
        
        startTab = tab;
        length = 0;   
        
        do {
        	schutzenberger4D(tab);
        	map <lqc, oint128>::iterator iterForRem = allTabs.find(tab);
  		if(iterForRem != allTabs.end()) allTabs.erase(iterForRem);
        	length = length + 1;
        } while(tab!=startTab); 
        
 	output << length << " ";         
}

int Schutzenberger::setCucleLengths2D(map <lpc, oint128> &allTabs, ofstream &output) { 
	int sizeLen = 0;
	while(!allTabs.empty()) {
        	lpc tab = allTabs.begin()->first; 
        	CucleLengths2D(allTabs, tab, output);  
        	sizeLen = sizeLen + 1;    	
    	}
    	output << endl;
    	return sizeLen;
}
// перенесети в абстрактный класс
int Schutzenberger::setCucleLengths3D(map <ltc, oint128> &allTabs, ofstream &output) {

	int sizeLen = 0;
	while(!allTabs.empty()) {
        	ltc tab = allTabs.begin()->first;  
        	CucleLengths3D(allTabs, tab, output);  
        	sizeLen = sizeLen + 1; 
    	}
    	output << endl;
    	return sizeLen;
}

int Schutzenberger::setCucleLengths4D(map <lqc, oint128> &allTabs, ofstream &output) {

	int sizeLen = 0;
	while(!allTabs.empty()) { 
		lqc tab = allTabs.begin()->first;
		CucleLengths4D(allTabs, tab, output);
		sizeLen = sizeLen + 1;
	}
	output << endl;
    	return sizeLen;
}

// почему дважды?
void Schutzenberger::checkMemory(float memoryLimit, ofstream &fout_logs, bool isLogs) {
	struct sysinfo si;
	sysinfo(&si);

	long double totalRAM;
	long double freeRAM;
	
	totalRAM = si.totalram;
	freeRAM = si.freeram;

	const double percent = totalRAM / 100.0;
	double freePercent;
	freePercent = freeRAM / percent;

	if(freePercent <= (double)memoryLimit) {
		if(isLogs) fout_logs << "<< Превышение ограничения по памяти >>" << endl;
		exit(0);
	}
}

void Schutzenberger::printUchar(uchar smb) {
	printf("%d", smb);
}

void Schutzenberger::printQU(QU active) {
        std::cout << "(" << active.getX() << "; " << active.getY() << "; ";
        std::cout << active.getZ() << "; " << active.getL() << ")";
        std::cout << "\n";
}

void Schutzenberger::printPU(pu active, ofstream &fout) {
        fout << "(" << active.first << "; " << active.second << ") ";
}

void Schutzenberger::printPC(pc active, ostream &output) {
        output << "(" ;
        printf("%d", active.first);
        output << "; ";
        printf("%d", active.second);
        output << ") " ;
        
}

void Schutzenberger::printTC(tc active) {
	std::cout << "(";
	printUchar(active.x);
	std::cout << "; ";
	printUchar(active.y);
	std::cout << "; ";
	printUchar(active.z);
	std::cout << ")" << endl;
}

void Schutzenberger::printQC(QC active, ostream &output) {	

	output << "(";
	printUchar(active.getX());
	output << "; ";
	printUchar(active.getY());
	output << "; ";
	printUchar(active.getZ());
	output << "; ";
	printUchar(active.getL());
	output << ")" << endl;
}

void Schutzenberger::printNerve(dqu sch_path){
        size_t i; 
        i = 0; 
        
        while(i < sch_path.size()) {
                printQU(sch_path[i]);
                std::cout << "\n";  
                i = i + 1;    
        } 
        
        std::cout << "\n";
}


void Schutzenberger::printLQU(lqu tab) {
        size_t size;
        size_t i;
        
        size = tab.size();
        i = 0;
        
        while(i < size) {
                printQU(tab.front());
                tab.pop_front();
                std::cout << "\n";
                ++i;
        } 
        std::cout << "\n";
}

void Schutzenberger::printLTC(ltc tab) {
	size_t size;
        size_t i;
        
        size = tab.size();
        i = 0;
        
         while(i < size) {
                printTC(tab.front());
                tab.pop_front();
                std::cout << "\n";
                ++i;
        } 
        std::cout << "\n";  
}

void Schutzenberger::printLQC(lqc tab, ostream &output) {
	size_t size;
        size_t i;
        
        size = tab.size();
        i = 0;
        
        while(i < size) {
                printQC(tab.front());
                tab.pop_front();
                output << "\n";
                ++i;
        } 
        output << "\n";  
}

void Schutzenberger::printLPU(lpu tab, ofstream &fout) {
	size_t size;
	size_t i;
	
	size = tab.size();
	i = 0;
	
	while(i < size) {
                printPU(tab.front(), fout);
                tab.pop_front();
                ++i;
        } 
        fout << "\n";
}

void Schutzenberger::printLPC(lpc tab, ostream &output) {
	size_t size;
	size_t i;
	
	size = tab.size();
	i = 0;
	
	while(i < size) {
                printPC(tab.front(), output);
                tab.pop_front();
                ++i;
        } 
        output << "\n";
}

/*lqu Schutzenberger::involution(lqu tab) {
        dqu sch_path;
        QU new_active;
        oint128 size;
        lqu tab2;
	size_t i;
	
	size = tab.size();
	i = 1;
	
	while(i <= size) {
                new_active = schutzenberger4D(tab, sch_path, false);
                tab2.push_front(new_active);

                std::cout << "Iteration is " << i << "\n";
                std::cout << "Active : (" << new_active.getX() << "; " << new_active.getY() << "; ";
                std::cout << new_active.getZ() << "; " << new_active.getL() << ")\n";
                std::cout << "Nerve: ";
                size_t j;
                j = 0;
                
                while(j < sch_path.size()) {
                       std::cout << "(" << sch_path[j].getX() << "; " << sch_path[j].getY() << "; ";
                       std::cout << sch_path[j].getZ() << "; " << sch_path[j].getL() << ")   ";
                	j = j + 1;
                } 
                std::cout << "\nTab size: " << tab.size() << "\n\n";
        i = i + 1;
        }
        return tab2;
}*/
