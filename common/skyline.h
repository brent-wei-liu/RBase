/*
 * skyline.h
 *
 *  Created on: Jan 16, 2010
 *      Author: dsachar
 */

#ifndef SKYLINE_H_
#define SKYLINE_H_



#define L 2*i
#define R 2*i+1

#define DYN 0
#define ORTH 1



string pointToString(float *point, int dimension);
string mbbToString(float *point, int dimension);

bool isPoint(float* mbb, int dimension);
float* mindisPoint(float *p, float *mbb, int dimension);

float* pointEps(float *point, float *query, int dimension);

bool dominate(float *pt1, float *spt2, int dimension);


bool PdyndomP(float *pt1, float *pt2, float *q, int dimension);
bool PdyndomM(float *pt1, float *mbb, float *q, int dimension);

bool PorthdomP(float *pt1, float *pt2, float *q, int dimension);
bool PorthdomM(float *pt1, float *mbb, float *q, int dimension);
bool MorthdomP(float *mbb, float *point, float *query, int dimension);
bool MorthdomM(float *mbb1, float *mbb2, float *query, int dimension);

bool PworthdomP(float *pt1, float *pt2, float *q, int dimension);

bool SMorthdomSM(float *mbb1, float *mbb2, float *query, int dimension);
bool SMorthdomM(float* mbb1, float* mbb2, float* query, int dimension);
bool PorthdomSM(float *pt1, float *mbb2, float *query, int dimension);
bool MorthdomSM(float *mbb1, float *mbb2, float *query, int dimension);




/////// dom w.r.t. mbb

// the one to rule them all
bool MorthdomMwrtM(float *mbb1, float *mbb2, float *qm, int dimension);


bool PorthdomPwrtM(float *pt1, float *pt2, float *qm, int dimension);
bool PorthdomMwrtM(float *pt1, float *pt2, float *qm, int dimension);
bool MorthdomPwrtM(float *mbb1, float *pt2, float *qm, int dimension);


bool SMorthdomSMwrtM(float *mbb1, float *mbb2, float *qm, int dimension);
bool SMorthdomMwrtM(float *mbb1, float *mbb2, float *qm, int dimension);
bool MorthdomSMwrtM(float *mbb1, float *mbb2, float *qm, int dimension);
bool SMorthdomSMwrtSM(float *mbb1, float *mbb2, float *qm, int dimension);


#endif /* SKYLINE_H_ */
