/*
 * Sequence.h
 *
 *  Created on: April 9, 2016
 *      Author: vduzhin
 */

#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include "Diagram.h"
#include "StrictDiagram.h"
#include "Common.h"

using namespace std;

class Sequence
{
	private:
		const int PRECISION = 20;
		string io, type, fname;
		fstream f;
		uint64 line;
	public:
		Sequence(const char* inputoutput, const char* t, const char* fn);
		~Sequence();

		int read_int(int* row);

		int read_standard(Diagram* diag = NULL);
		int read_strict(StrictDiagram* diag);

		void write_standard_maxprob(Diagram diag);
		void write_standard_fullinfo(Diagram &diag, int row = -1, bool greedy = false);

		void write_strict_maxprob(StrictDiagram diag);
		void write_strict_lineprob(StrictDiagram diag, uint64 line);
		void write_strict_fullinfo(StrictDiagram &diag, int row = -1, bool greedy = false);

		uint64 get_line();
};


#endif /* SEQUENCE_H_ */
