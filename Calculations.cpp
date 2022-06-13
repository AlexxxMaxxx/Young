#include "Calculations.h"
void Calculations::calculateVectorNormaliz(vector <double> &vect, oint128 dim) {

	int i;
	i = 0;
	
	while(i < vect.size()) {
		vect[i] = vect[i] / (double) dim;
		i = i + 1;
	} 
} 

void Calculations::calculateAdjCyclesLength(vector <double> &vect, ofstream &fout) {

	int i;
	i = 0;
	
	while(i < vect.size() - 1) { 
		fout << (double) vect[i + 1] / (double) vect[i] << " ";
		i = i + 1;
	} fout << endl;
}


double Calculations::calculateMedian(vector <double> &vect) {

	int size;
	size = vect.size();
	
	switch(size % 2) {
		case 0: return (double) (vect[size / 2] + vect[(size / 2) - 1]) / 2; break;
		case 1: return (double) vect[size / 2]; break;
	}
}

double Calculations::calculateDisp(vector <double> & vect, double MO, int n) {

	int i;
	int size;

	i = 0;
	size = vect.size();
	
	vector <double> differences(size);
	while(i < size) {
		differences[i] = pow(vect[i] - MO, 2);
		i = i + 1;
	}
	
	return calculateSum (differences) / (double) n;
}

double Calculations::calculateMO(vector <double> & vect, int n){

	return calculateSum (vect) / (double) n;
}

double Calculations::calculateSum(vector <double> & vect) {

	return std::accumulate(vect.begin(), vect.end(), 0.0);
}
