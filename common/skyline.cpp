/*
 * skyline.cpp
 *
 *  Created on: Jan 16, 2010
 *      Author: dsachar
 */

#include "pch.h"


string pointToString(float *point, int dimension) {
    ostringstream os;
    for (int i=0; i<dimension; i++) {
        if (i!=0) os << ",";
        os << point[i];
    }
    return os.str();
}

string mbbToString(float *mbb, int dimension) {
    ostringstream os;
    for (int i=0; i<dimension; i++) {
        if (i!=0) os << ",";
        os << mbb[L] << ":" << mbb[R];
    }
    return os.str();
}


bool isPoint(float *mbb, int dimension){
    for (int i=0; i<dimension; i++){
        if (mbb[R] > mbb[L]){
            return false;
        }
    }
    return true;
}


float* mindisPoint(float *p, float *mbb, int dim) {
    float *r = new float[dim];

    for (int i=0; i<dim; i++) {
        if (p[i] < mbb[L]) {
            r[i] = mbb[L];
        } else {
            if (p[i] > mbb[R]) {
                r[i] = mbb[R];
            } else {
                r[i] = p[i];
            }
        }
    }
    return r;
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
////////////////////////////// SKYLINE METHODS //////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////



// dominate which removes duplicates
//bool dominate(int _dim, float *_pt1, float *_pt2){
//    bool ret = true;
//    for (int i = 0; i < _dim; i ++){
//        if (_pt1[i] > _pt2[i] + FLOATZERO){
//            ret = false;
//            break;
//        }
//    }
//    return ret;
//}

// dominance of points; returns true if pt1 dominates pt2
bool dominate(float *pt1, float *pt2, int dimension){

    int cnt = 0;

    for (int i = 0; i < dimension; i ++) {

        if (pt1[i] == pt2[i]) {
            cnt++;
        }
        else if (pt1[i] > pt2[i] + FLOATZERO){
            return false;
        }
    }

    if (cnt == dimension) {
        return false;
    } else {
        return true;
    }
}

// checks if pt1 dynamically dominates pt2 w.r.t. q
bool PdyndomP(float *pt1, float *pt2, float *query, int dimension){

    int cnt = 0;

    for (int i = 0; i < dimension; i ++) {

        if ( fabs(pt1[i] - query[i]) == fabs(pt2[i] - query[i]) ) {
            cnt++;
        }
        else if (fabs(pt1[i] - query[i]) > fabs(pt2[i] - query[i]) + FLOATZERO){
            return false;
        }
    }

    if (cnt == dimension) {
        return false;
    } else {
        return true;
    }
}


// checks if pt1 dynamically dominates mbb w.r.t. q
bool PdyndomM(float *pt1, float *mbb, float *query, int dimension){

    float *pt2 = mindis_point(query, mbb, dimension);

    bool isit = PdyndomP(pt1, pt2, query, dimension);

    delete[] pt2;
    return isit;
}


// checks if pt1 orthant dominates pt2 w.r.t. q
bool PorthdomP(float *pt1, float *pt2, float *query, int dimension){
    int cnt = 0;

    for (int i = 0; i < dimension; i ++) {
        if ( (pt1[i] - query[i]) * (pt2[i] - query[i]) < 0 ) {
            return false;
        }
        else if ( fabs(pt1[i] - query[i]) == fabs(pt2[i] - query[i]) ) {
            cnt++;
        }
        else if (fabs(pt1[i] - query[i]) > fabs(pt2[i] - query[i]) + FLOATZERO){
            return false;
        }
    }


    float *mbb1 = new float[2*dimension];
    float *mbb2 = new float[2*dimension];
    float *qm = new float[2*dimension];
    for (int i=0; i<dimension; i++) {
        mbb1[L] = mbb1[R] = pt1[i];
        mbb2[L] = mbb2[R] = pt2[i];
        qm[L] = qm[R] = query[i];
    }
//    bool mmm = MorthdomMwrtM(mbb1, mbb2, qm, dimension);
    delete[] mbb1;
    delete[] mbb2;
    delete[] qm;



    if (cnt == dimension) {
//        if (mmm != false){
//            cerr << "oops! PorthdomP :: " << mmm << " for " << pointToString(pt1, dimension) << "   " << pointToString(pt2, dimension) << "   " << pointToString(query, dimension) << endl;
//        }
        return false;
    } else {
//        if (mmm != true){
//            cerr << "oops! PorthdomP :: " << mmm << " for " << pointToString(pt1, dimension) << "   " << pointToString(pt2, dimension) << "   " << pointToString(query, dimension) << endl;
//        }
        return true;
    }
}



// checks if pt1 weakly orthant dominates pt2 w.r.t. q (returns true if pt1 and pt2 coincide)
bool PworthdomP(float *pt1, float *pt2, float *query, int dimension){

    for (int i = 0; i < dimension; i ++) {

        if ( (pt1[i] - query[i]) * (pt2[i] - query[i]) < 0 ) {
            return false;
        }
        else if (fabs(pt1[i] - query[i]) > fabs(pt2[i] - query[i]) + FLOATZERO){
            return false;
        }
    }

    return true;
}


// checks if pt1 orthant dominates mbb w.r.t. q
bool PorthdomM(float *pt1, float *mbb, float *query, int dimension){

    float *pt2 = mindisPoint(query, mbb, dimension);

    bool isit = PorthdomP(pt1, pt2, query, dimension);


//    float *mbb1 = new float[2*dimension];
//    float *qm = new float[2*dimension];
//    for (int i=0; i<dimension; i++) {
//        mbb1[L] = mbb1[R] = pt1[i];
//        qm[L] = qm[R] = query[i];
//    }
//    bool mmm = MorthdomMwrtM(mbb1, mbb, qm, dimension);
//    delete[] mbb1;
//    delete[] qm;
//
//    if (mmm != isit){
//        cerr << "oops! PorthdomM :: " << mmm << " for " << pointToString(pt1, dimension) << "   " << mbbToString(mbb, dimension) << "   " << pointToString(query, dimension) << endl;
//    }

    delete[] pt2;
    return isit;
}



// add eps to point in each dimension
float* pointEps(float *point, float *query, int dimension) {
    const float eps = 0.001;
    float *pointeps = new float[dimension];
    for (int i=0; i<dimension; i++) {
        if (point[i] > query[i]) {
            pointeps[i] = point[i] + eps;
        }
        else if (point[i] < query[i]) {
            pointeps[i] = point[i] - eps;
        }
        else {
            pointeps[i] = point[i];
        }
    }
    return pointeps;
}


// check if mbb orthant dominates point w.r.t. query
bool MorthdomP(float *mbb, float *point, float *query, int dimension){

    // if mbb is a point apply PorthdomP
    if (isPoint(mbb, dimension)){
        float *mbbPoint = new float[dimension];
        for (int i=0; i<dimension; i++){
            mbbPoint[i] = mbb[L];
        }
        bool isit = PorthdomP(mbbPoint, point, query, dimension);
        delete[] mbbPoint;
        return isit;
    }

    bool strict = false;
    bool side = false;

    for (int i=0; i<dimension; i++) {

        // mbb right of query
        if (mbb[L] >= query[i]) {
            if (point[i] > mbb[R]) {
                strict = true;
            }
            else if (point[i] < mbb[R]) {
                if (point[i] < mbb[L]) {
                    return false;
                }
                else {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                    if (point[i] > mbb[L]) {
                        strict = true;
                    }
                }
            }
        }

        // mbb left of query
        if (mbb[R] <= query[i]) {
            if (point[i] < mbb[L]) {
                strict = true;
            }
            else if (point[i] > mbb[L]) {
                if (point[i] > mbb[R]) {
                    return false;
                }
                else {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                    if (point[i] < mbb[R]) {
                        strict = true;
                    }
                }
            }
        }

        // query is inside mbb
        if (mbb[L] < query[i] && mbb[R] > query[i]) {
            if (side == true){
                return false;
            }
            else {
                side = true;
            }
            if (point[i] < mbb[R] && point[i] > mbb[L]) {
                return false;
            }
        }

    }


//    float *mbb2 = new float[2*dimension];
//    float *qm = new float[2*dimension];
//    for (int i=0; i<dimension; i++) {
//        mbb2[L] = mbb2[R] = point[i];
//        qm[L] = qm[R] = query[i];
//    }
//    bool mmm = MorthdomMwrtM(mbb, mbb2, qm, dimension);
//    delete[] mbb2;
//    delete[] qm;


    if (side == true && strict == false) {
//        if (mmm != false) {
//            cerr << "oops! MorthdomP :: " << mmm << " for " << mbbToString(mbb, dimension) << "   " << pointToString(point, dimension) << "   " << pointToString(query, dimension) << endl;
//        }
        return false;
    }
    else {
//        if (mmm != true){
//            cerr << "oops! MorthdomP :: " << mmm << " for " << mbbToString(mbb, dimension) << "   " << pointToString(point, dimension) << "   " << pointToString(query, dimension) << endl;
//        }
        return true;
    }
}

/*
// check if mbb1 orthant dominates mbb2 w.r.t. query
bool MorthdomM(float* mbb1, float* mbb2, float* query, int dimension){
    float *pt2 = mindis_point(query, mbb2, dimension);

    bool isit = MorthdomP(mbb1, pt2, query, dimension);

    delete[] pt2;
    return isit;
}
*/

///*
// check if mbb1 orthant dominates mbb2 w.r.t. query
bool MorthdomM(float* mbb1, float* mbb2, float* query, int dimension){

    // if mbb1 is a point apply PorthdomM
    if (isPoint(mbb1, dimension)){
        float* mbbPoint = new float[dimension];
        for (int i=0; i<dimension; i++){
            mbbPoint[i] = mbb1[L];
        }
        bool isit = PorthdomM(mbbPoint, mbb2, query, dimension);
        delete[] mbbPoint;
        return isit;
    }

    bool strict = false;
    bool side = false;

    for (int i=0; i<dimension; i++) {

        // mbb1 strictly right of query
        if (mbb1[L] > query[i]) {
            if (mbb2[L] > mbb1[R]) {
                strict = true;
            }
            else if (mbb2[L] < mbb1[R]) {
                if (mbb2[L] < mbb1[L]) {
                    return false;
                }
                else {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                    if (mbb2[L] > mbb1[L]) {
                        strict = true;
                    }
                }
            }
        }

        // mbb1[L] coincides with query[i]
        if (mbb1[L] == query[i]) {
            if (mbb2[L] > mbb1[R]) {
                strict = true;
            }
            else if (mbb2[L] < mbb1[R]) {
                if (side == true){
                    return false;
                }
                else {
                    side = true;
                }
                if (mbb2[L] > mbb1[L] || mbb2[R] < mbb1[L]) {
                    strict = true;
                }
            }
        }


        // mbb1 strictly left of query
        if (mbb1[R] < query[i]) {
            if (mbb2[R] < mbb1[L]) {
                strict = true;
            }
            else if (mbb2[R] > mbb1[L]) {
                if (mbb2[R] > mbb1[R]) {
                    return false;
                }
                else {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                    if (mbb2[R] < mbb1[R]) {
                        strict = true;
                    }
                }
            }
        }

        // mbb1[R] coincides with query[i]
        if (mbb1[R] == query[i]) {
            if (mbb2[R] < mbb1[L]) {
                strict = true;
            }
            else if (mbb2[R] > mbb1[L]) {
                if (side == true){
                    return false;
                }
                else {
                    side = true;
                }
                if (mbb2[R] < mbb1[R] || mbb2[L] > mbb1[R]) {
                    strict = true;
                }
            }
        }


        // query is inside mbb1
        if (mbb1[L] < query[i] && mbb1[R] > query[i]) {
            if (side == true){
                return false;
            }
            else {
                side = true;
            }
            if ( !(mbb2[R] <= mbb1[L] && mbb2[L] >= mbb1[R]) ) {
                return false;
            }
            else if (mbb2[R] < mbb1[L] || mbb2[L] > mbb1[R]) {
                strict = true;
            }
        }

    }

//    float *q = new float[2*dimension];
//    for (int i=0; i<dimension; i++) {
//        q[L] = q[R] = query[i];
//    }
//    bool mmm = MorthdomMwrtM(mbb1, mbb2, q, dimension);
//    delete[] q;


    if (side == true && strict == false) {
//        if (mmm != false) {
//            cerr << "oops! MorthdomM :: " << mmm << " for " << mbbToString(mbb1, dimension) << "   " << mbbToString(mbb2, dimension) << "   " << pointToString(query, dimension) << endl;
//        }
        return false;
    }
    else {
//        if (mmm != true){
//            cerr << "oops! MorthdomM :: " << mmm << " for " << mbbToString(mbb1, dimension) << "   " << mbbToString(mbb2, dimension) << "   " << pointToString(query, dimension) << endl;
//        }
        return true;
    }

}
//*/



// check if Some point in mbb1 orthant dominates Some point in mbb2 w.r.t. query
bool SMorthdomSM(float *mbb1, float *mbb2, float *query, int dimension){
    if (point_inside_mbr(query, mbb1, dimension)) return true;

    float *pt1 = mindis_point(query, mbb1, dimension); // w.r.t. query
    float *pt1b = pointEps(pt1, query, dimension); // add eps to all dims
    float *pt2 = mindis_point(pt1b, mbb2, dimension); // w.r.t. pt1b

    bool isit = PorthdomP(pt1, pt2, query, dimension);

    delete[] pt1;
    delete[] pt1b;
    delete[] pt2;


//    float *qm = new float[2*dimension];
//    for (int i=0; i<dimension; i++) {
//        qm[L] = qm[R] = query[i];
//    }
//    bool mmm = SMorthdomSMwrtM(mbb1, mbb2, qm, dimension);
//    delete[] qm;
//
//    if (mmm != isit){
//        cerr << "oops! SMorthdomSM :: " << mmm << " for " << mbbToString(mbb1, dimension) << "   " << mbbToString(mbb2, dimension) << "   " << pointToString(query, dimension) << endl;
//    }

    return isit;
}


// check if Some point in mbb1 orthant dominates mbb2 w.r.t. query
bool SMorthdomM(float* mbb1, float* mbb2, float* query, int dimension){
    float *pt1 = mindis_point(query, mbb1, dimension); // w.r.t. query
    float *pt2 = mindis_point(query, mbb2, dimension); // w.r.t. query

    bool isit = PorthdomP(pt1, pt2, query, dimension);

    delete[] pt1;
    delete[] pt2;


//    float *qm = new float[2*dimension];
//    for (int i=0; i<dimension; i++) {
//        qm[L] = qm[R] = query[i];
//    }
//    bool mmm = SMorthdomMwrtM(mbb1, mbb2, qm, dimension);
//    delete[] qm;
//
//    if (mmm != isit){
//        cerr << "oops! SMorthdomM :: " << mmm << " for " << mbbToString(mbb1, dimension) << "   " << mbbToString(mbb2, dimension) << "   " << pointToString(query, dimension) << endl;
//    }

    return isit;
}



// check if pt1 orthant dominates Some point in mbb2 w.r.t. query
bool PorthdomSM(float *pt1, float *mbb2, float *query, int dimension){
    bool strict = false;

    for (int i=0; i<dimension; i++) {

        // pt1 strictly right of query
        if (pt1[i] > query[i]) {
            if (mbb2[R] > pt1[i]) {
                strict = true;
            }
            else if (mbb2[R] < pt1[i]) {
                return false;
            }
        }

        // pt1 strictly left of query
        if (pt1[i] < query[i]) {
            if (mbb2[L] < pt1[i]) {
                strict = true;
            }
            else if (mbb2[L] > pt1[i]) {
                return false;
            }
        }

        // pt1[i] coincides with query[i]
        if (pt1[i] == query[i]) {
            if (mbb2[L] != pt1[i] || mbb2[R] != pt1[i]) {
                strict = true;
            }
        }

    }


//    float *qm = new float[2*dimension];
//    float *mbb1 = new float[2*dimension];
//    for (int i=0; i<dimension; i++) {
//        mbb1[L] = mbb1[R] = pt1[i];
//        qm[L] = qm[R] = query[i];
//    }
//    bool mmm = MorthdomSMwrtM(mbb1, mbb2, qm, dimension);
//    delete[] qm;
//    delete[] mbb1;
//
//    if (mmm != strict){
//        cerr << "oops! PorthdomSM :: " << mmm << " for " << pointToString(pt1, dimension) << "   " << mbbToString(mbb2, dimension) << "   " << pointToString(query, dimension) << endl;
//    }


    return strict;
}



// check if mbb1 orthant dominates Some point in mbb2 w.r.t. query
bool MorthdomSM(float* mbb1, float* mbb2, float* query, int dimension){


    // if mbb1 is a point apply PorthdomSM
    if (isPoint(mbb1, dimension)){
        float* mbbPoint = new float[dimension];
        for (int i=0; i<dimension; i++){
            mbbPoint[i] = mbb1[L];
        }
        bool isit = PorthdomSM(mbbPoint, mbb2, query, dimension);
        delete[] mbbPoint;
        return isit;
    }

    bool strict = false;
    bool side = false;

    for (int i=0; i<dimension; i++) {

        // mbb1 strictly right of query
        if (mbb1[L] > query[i]) {
            if (mbb2[R] > mbb1[R]) {
                strict = true;
            }
            else if (mbb2[R] < mbb1[R]) {
                if (mbb2[R] < mbb1[L]) {
                    return false;
                }
                else {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                    if (mbb2[L] > mbb1[R]) {
                        strict = true;
                    }
                }
            }
        }

        // mbb1[L] coincides with query[i]
        if (mbb1[L] == query[i]) {
            if (mbb2[R] > mbb1[R]) {
                strict = true;
            }
            else if (mbb2[R] < mbb1[R]) {
                if (side == true){
                    return false;
                }
                else {
                    side = true;
                }
                if (mbb2[L] != mbb1[L] || mbb2[R] != mbb1[L]) {
                    strict = true;
                }
            }
        }


        // mbb1 strictly left of query
        if (mbb1[R] < query[i]) {
            if (mbb2[L] < mbb1[L]) {
                strict = true;
            }
            else if (mbb2[L] > mbb1[L]) {
                if (mbb2[L] > mbb1[R]) {
                    return false;
                }
                else {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                    if (mbb2[L] < mbb1[R]) {
                        strict = true;
                    }
                }
            }
        }

        // mbb1[R] coincides with query[i]
        if (mbb1[R] == query[i]) {
            if (mbb2[L] < mbb1[L]) {
                strict = true;
            }
            else if (mbb2[L] > mbb1[L]) {
                if (side == true){
                    return false;
                }
                else {
                    side = true;
                }
                if (mbb2[L] != mbb1[R] || mbb2[R] != mbb1[R]) {
                    strict = true;
                }
            }
        }


        // query is inside mbb1
        // query is inside mbb1
        if (mbb1[L] < query[i] && mbb1[R] > query[i]) {
            if (side == true){
                return false;
            }
            else {
                side = true;
            }
            if ( !(mbb2[L] <= mbb1[L] && mbb2[R] >= mbb1[R]) ) {
                return false;
            }
            else if (mbb2[L] < mbb1[L] || mbb2[R] > mbb1[R]) {
                strict = true;
            }
        }

    }


    bool isit;
    if (side == true && strict == false) {
        isit = false;
    }
    else {
        isit = true;
    }


//    float *qm = new float[2*dimension];
//    for (int i=0; i<dimension; i++) {
//        qm[L] = qm[R] = query[i];
//    }
//    bool mmm = MorthdomSMwrtM(mbb1, mbb2, qm, dimension);
//    delete[] qm;
//
//    if (mmm != isit){
//        cerr << "oops! MorthdomSM :: " << mmm << " for " << mbbToString(mbb1, dimension) << "   " << mbbToString(mbb2, dimension) << "   " << pointToString(query, dimension) << endl;
//    }

    return isit;
}



// check if mbb1 orthant dominates mbb2 w.r.t. mbb query
bool MorthdomMwrtM(float *mbb1, float *mbb2, float *qm, int dimension){

    bool strict = false;
    bool side = false;
    bool isPoint = true; // is mbb1 a point?
    bool corner = true; // true if a corner of mmb1 touches qm

    for (int i=0; i<dimension; i++) {

        if (mbb1[L] < mbb1[R]) {
            isPoint = false;
        }

        // mbb1 strictly right of qm
        if (mbb1[L] > qm[R]) {
            corner = false;
            if (mbb2[L] > mbb1[R]) {
                strict = true;
            }
            else if (mbb2[L] < mbb1[R]) {
                if (mbb2[L] < mbb1[L]) {
                    return false;
                }
                else {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                    if (mbb2[L] > mbb1[L]) {
                        strict = true;
                    }
                }
            }
        }

        // mbb1[L] coincides with qm[R]
        else if (mbb1[L] == qm[R]) {
            if (mbb2[L] > mbb1[R]) {
                strict = true;
            }
            else if (mbb2[L] < mbb1[R]) {
                if (qm[L] != qm[R] && mbb2[L] < qm[R]) {
                    return false;
                }
                if (mbb1[L] != mbb1[R]) {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                }
                if (mbb2[L] > mbb1[L] || (qm[L] == qm[R] && mbb2[R] < mbb1[L]) ) {
                    strict = true;
                }
            }
        }

        // mbb1 strictly left of qm
        else if (mbb1[R] < qm[L]) {
            corner = false;
            if (mbb2[R] < mbb1[L]) {
                strict = true;
            }
            else if (mbb2[R] > mbb1[L]) {
                if (mbb2[R] > mbb1[R]) {
                    return false;
                }
                else {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                    if (mbb2[R] < mbb1[R]) {
                        strict = true;
                    }
                }
            }
        }

        // mbb1[R] coincides with qm[L]
        else if (mbb1[R] == qm[L]) {
            if (mbb2[R] < mbb1[L]) {
                strict = true;
            }
            else if (mbb2[R] > mbb1[L]) {
                if (qm[L] != qm[R] && mbb2[R] > qm[L]) {
                    return false;
                }
                if (mbb1[L] != mbb1[R]) {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                }
                if (mbb2[R] < mbb1[R] || (qm[L] == qm[R] && mbb2[L] > mbb1[R]) ) {
                    strict = true;
                }
            }
        }

        // mbb1[R] right of qm, and mbb1[L] inside qm
        else if (mbb1[R] >= qm[R] && mbb1[L] < qm[R] && mbb1[L] > qm[L]) {
            if (side == true){
                return false;
            }
            else {
                side = true;
            }
            if (mbb2[L] > mbb1[R]) {
                strict = true;
            }
            else if (mbb2[L] < mbb1[R]) {
                return false;
            }
        }

        // mbb1[L] left of qm, and mbb1[R] inside qm
        else if (mbb1[L] <= qm[L] && mbb1[R] < qm[R] && mbb1[R] > qm[L]) {
            if (side == true){
                return false;
            }
            else {
                side = true;
            }
            if (mbb2[R] < mbb1[L]) {
                strict = true;
            }
            else if (mbb2[R] > mbb1[L]) {
                return false;
            }
        }

        // mbb1[L] left of qm, and mbb1[R] right of qm
        else if ( (qm[L] != qm[R] && mbb1[L] <= qm[L] && mbb1[R] >= qm[R]) || (mbb1[L] < qm[L] && mbb1[R] > qm[R]) ) {
            if (mbb1[L] < qm[L] && mbb1[R] > qm[R]) {
                corner = false;
            }
            if (side == true){
                return false;
            }
            else {
                side = true;
            }
            if (mbb2[L] >= mbb1[R] || mbb2[R] <= mbb1[L]) {
                if (mbb2[L] > mbb1[R] || mbb2[R] < mbb1[L]) {
                    strict = true;
                }
            }
            else {
                return false;
            }
        }

        // mbb1 is inside qm
        else if (mbb1[L] > qm[L] && mbb1[R] < qm[R]) {
            return false;
        }
    }

    if (corner && (isPoint || side)) {
        return false;
    }

    if (isPoint || side == true) {
        return strict;
    }
    else {
        return true;
    }
}


// checks if pt1 orthant dominates pt2 w.r.t. mbb qm
bool PorthdomPwrtM(float *pt1, float *pt2, float *qm, int dimension){
    float *mbb1 = new float[2*dimension];
    float *mbb2 = new float[2*dimension];
    for (int i=0; i<dimension; i++) {
        mbb1[L] = mbb1[R] = pt1[i];
        mbb2[L] = mbb2[R] = pt2[i];
    }
    bool mmm = MorthdomMwrtM(mbb1, mbb2, qm, dimension);
    delete[] mbb1;
    delete[] mbb2;
    return mmm;
}


// checks if pt1 orthant dominates mbb w.r.t. mbb qm
bool PorthdomMwrtM(float *pt1, float *mbb2, float *qm, int dimension){
    float *mbb1 = new float[2*dimension];
    for (int i=0; i<dimension; i++) {
        mbb1[L] = mbb1[R] = pt1[i];
    }
    bool mmm = MorthdomMwrtM(mbb1, mbb2, qm, dimension);
    delete[] mbb1;
    return mmm;
}


// checks if mbb1 orthant dominates pt2 w.r.t. mbb qm
bool MorthdomPwrtM(float *mbb1, float *pt2, float *qm, int dimension){
    float *mbb2 = new float[2*dimension];
    for (int i=0; i<dimension; i++) {
        mbb2[L] = mbb2[R] = pt2[i];
    }
    bool mmm = MorthdomMwrtM(mbb1, mbb2, qm, dimension);
    delete[] mbb2;
    return mmm;
}


// check if Some point in mbb1 orthant dominates Some point in mbb2 w.r.t. mbb qm
bool SMorthdomSMwrtM(float *mbb1, float *mbb2, float *qm, int dimension){
    bool strict = false;

    for (int i=0; i<dimension; i++) {

        // mbb1 strictly right of qm
        if (mbb1[L] > qm[R]) {
            if (mbb2[R] > mbb1[L]) {
                strict = true;
            }
            else if (mbb2[R] < mbb1[L]) {
                return false;
            }
        }

        // mbb1[L] coincides with qm[R]
        else if (mbb1[L] == qm[R]) {
            if (mbb2[R] > mbb1[L]) {
                strict = true;
            }
            else if (mbb2[R] < mbb1[L]) {
                if (qm[L] != qm[R]) {
                    return false;
                }
                else { // qm[L] and qm[R] coincide
                    strict = true;
                }
            }
        }

        // mbb1 strictly left of qm
        else if (mbb1[R] < qm[L]) {
            if (mbb2[L] < mbb1[R]) {
                strict = true;
            }
            else if (mbb2[L] > mbb1[R]) {
                return false;
            }
        }

        // mbb1[R] coincides with qm[L]
        else if (mbb1[R] == qm[L]) {
            if (mbb2[L] < mbb1[R]) {
                strict = true;
            }
            else if (mbb2[L] > mbb1[R]) {
                if (qm[L] != qm[R]) {
                    return false;
                }
                else { // qm[L] and qm[R] coincide
                    strict = true;
                }
            }
        }

        // mbb1[R] right of qm, and mbb1[L] inside qm
        else if (mbb1[R] >= qm[R] && mbb1[L] < qm[R] && mbb1[L] > qm[L]) {
            if (mbb2[R] > qm[R]) {
                strict = true;
            }
            else if (mbb2[R] < qm[R]) {
                return false;
            }
        }

        // mbb1[L] left of qm, and mbb1[R] inside qm
        else if (mbb1[L] <= qm[L] && mbb1[R] < qm[R] && mbb1[R] > qm[L]) {
            if (mbb2[L] < qm[L]) {
                strict = true;
            }
            else if (mbb2[L] > qm[L]) {
                return false;
            }
        }

        // mbb1[L] left of qm, and mbb1[R] right of qm
        else if ( (qm[L] != qm[R] && mbb1[L] <= qm[L] && mbb1[R] >= qm[R]) || (mbb1[L] < qm[L] && mbb1[R] > qm[R]) ) {
            if (mbb2[R] >= qm[R] || mbb2[L] <= qm[L]) {
                if (mbb2[R] > qm[R] || mbb2[L] < qm[L]) {
                    strict = true;
                }
            }
            else {
                return false;
            }
        }

        // mbb1 is inside qm
        else if (mbb1[L] > qm[L] && mbb1[R] < qm[R]) {
            return false;
        }

    }

    return strict;
}


// check if Some point in mbb1 orthant dominates mbb2 w.r.t. mbb qm
bool SMorthdomMwrtM(float *mbb1, float *mbb2, float *qm, int dimension){
    bool strict = false;

    for (int i=0; i<dimension; i++) {

        // mbb1 strictly right of qm
        if (mbb1[L] > qm[R]) {
            if (mbb2[L] > mbb1[L]) {
                strict = true;
            }
            else if (mbb2[L] < mbb1[L]) {
                return false;
            }
        }

        // mbb1[L] coincides with qm[R]
        else if (mbb1[L] == qm[R]) {
            if (mbb2[L] > mbb1[L]) {
                strict = true;
            }
            else if (mbb2[L] < mbb1[L]) {
                if (qm[L] != qm[R]) {
                    return false;
                }
                else { // qm is a point
                    if (mbb2[R] < mbb1[L]) {
                        strict = true;
                    }
                }
            }
        }

        // mbb1 strictly left of qm
        else if (mbb1[R] < qm[L]) {
            if (mbb2[R] < mbb1[R]) {
                strict = true;
            }
            else if (mbb2[R] > mbb1[R]) {
                return false;
            }
        }

        // mbb1[R] coincides with qm[L]
        else if (mbb1[R] == qm[L]) {
            if (mbb2[R] < mbb1[R]) {
                strict = true;
            }
            else if (mbb2[R] > mbb1[R]) {
                if (qm[L] != qm[R]) {
                    return false;
                }
                else { // qm is a point
                    if (mbb2[L] > mbb1[R]) {
                        strict = true;
                    }
                }
            }
        }

        // mbb1[R] right of qm, and mbb1[L] inside qm
        else if (mbb1[R] >= qm[R] && mbb1[L] < qm[R] && mbb1[L] > qm[L]) {
            if (mbb2[L] > qm[R]) {
                strict = true;
            }
            else if (mbb2[L] < qm[R]) {
                return false;
            }
        }

        // mbb1[L] left of qm, and mbb1[R] inside qm
        else if (mbb1[L] <= qm[L] && mbb1[R] < qm[R] && mbb1[R] > qm[L]) {
            if (mbb2[R] < qm[L]) {
                strict = true;
            }
            else if (mbb2[R] > qm[L]) {
                return false;
            }
        }

        // mbb1[L] left of qm, and mbb1[R] right of qm
        else if ( (qm[L] != qm[R] && mbb1[L] <= qm[L] && mbb1[R] >= qm[R]) || (mbb1[L] < qm[L] && mbb1[R] > qm[R]) ) {
            if (mbb2[L] >= qm[R] || mbb2[R] <= qm[L] || qm[L] == qm[R]) {
                if (mbb2[L] > qm[R] || mbb2[R] < qm[L]) {
                    strict = true;
                }
            }
            else {
                return false;
            }
        }

        // mbb1 is inside qm
        else if (mbb1[L] > qm[L] && mbb1[R] < qm[R]) {
            return false;
        }
    }

    return strict;
}


// check if mbb1 orthant dominates Some point in mbb2 w.r.t. mbb qm
bool MorthdomSMwrtM(float *mbb1, float *mbb2, float *qm, int dimension){

    bool strict = false;
    bool side = false;
    bool isPoint = true; // is mbb1 a point?

    for (int i=0; i<dimension; i++) {

        if (mbb1[L] < mbb1[R]) {
            isPoint = false;
        }

        // mbb1 strictly right of qm
        if (mbb1[L] > qm[R]) {
            if (mbb2[R] > mbb1[R]) {
                strict = true;
            }
            else if (mbb2[R] < mbb1[R]) {
                if (mbb2[R] < mbb1[L]) {
                    return false;
                }
                else {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                    if (mbb2[R] > mbb1[L]) {
                        strict = true;
                    }
                }
            }
        }

        // mbb1[L] coincides with qm[R]
        else if (mbb1[L] == qm[R]) {
            if (mbb2[R] > mbb1[R]) {
                strict = true;
            }
            else if (mbb2[R] < mbb1[R]) {
                if (qm[L] != qm[R] && mbb2[R] < qm[R]) {
                    return false;
                }
                if (mbb1[L] != mbb1[R]) {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                }
                if (mbb2[R] > mbb1[L] || (qm[L] == qm[R] && mbb2[L] < mbb1[L]) ) {
                    strict = true;
                }
            }
        }

        // mbb1 strictly left of qm
        else if (mbb1[R] < qm[L]) {
            if (mbb2[L] < mbb1[L]) {
                strict = true;
            }
            else if (mbb2[L] > mbb1[L]) {
                if (mbb2[L] > mbb1[R]) {
                    return false;
                }
                else {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                    if (mbb2[L] < mbb1[R]) {
                        strict = true;
                    }
                }
            }
        }

        // mbb1[R] coincides with qm[L]
        else if (mbb1[R] == qm[L]) {
            if (mbb2[L] < mbb1[L]) {
                strict = true;
            }
            else if (mbb2[L] > mbb1[L]) {
                if (qm[L] != qm[R] && mbb2[L] > qm[L]) {
                    return false;
                }
                if (mbb1[L] != mbb1[R]) {
                    if (side == true){
                        return false;
                    }
                    else {
                        side = true;
                    }
                }
                if (mbb2[L] < mbb1[R] || (qm[L] == qm[R] && mbb2[R] > mbb1[R]) ) {
                    strict = true;
                }
            }
        }

        // mbb1[R] right of qm, and mbb1[L] inside qm
        else if (mbb1[R] >= qm[R] && mbb1[L] < qm[R] && mbb1[L] > qm[L]) {
            if (side == true){
                return false;
            }
            else {
                side = true;
            }
            if (mbb2[R] > mbb1[R]) {
                strict = true;
            }
            else if (mbb2[R] < mbb1[R]) {
                return false;
            }
        }

        // mbb1[L] left of qm, and mbb1[R] inside qm
        else if (mbb1[L] <= qm[L] && mbb1[R] < qm[R] && mbb1[R] > qm[L]) {
            if (side == true){
                return false;
            }
            else {
                side = true;
            }
            if (mbb2[L] < mbb1[L]) {
                strict = true;
            }
            else if (mbb2[L] > mbb1[L]) {
                return false;
            }
        }

        // mbb1[L] left of qm, and mbb1[R] right of qm
        else if ( (qm[L] != qm[R] && mbb1[L] <= qm[L] && mbb1[R] >= qm[R]) || (mbb1[L] < qm[L] && mbb1[R] > qm[R]) ) {
            if (side == true){
                return false;
            }
            else {
                side = true;
            }
            if (mbb2[R] >= mbb1[R] || mbb2[L] <= mbb1[L]) {
                if (mbb2[R] > mbb1[R] || mbb2[L] < mbb1[L]) {
                    strict = true;
                }
            }
            else {
                return false;
            }
        }

        // mbb1 is inside qm
        else if (mbb1[L] > qm[L] && mbb1[R] < qm[R]) {
            return false;
        }
    }

    if (isPoint || side == true) {
        return strict;
    }
    else {
        return true;
    }
}


// check if Some point in mbb1 orthant dominates Some point in mbb2 w.r.t. Some point in qm
bool SMorthdomSMwrtSM(float *mbb1, float *mbb2, float *qm, int dimension){
    bool strict = false;
    bool corner = true; // true if a corner of mmb1 touches qm

    for (int i=0; i<dimension; i++) {

        // mbb1 strictly right of qm
        if (mbb1[L] > qm[R]) {
            corner = false;
            if (mbb2[R] > mbb1[L]) {
                strict = true;
            }
            else if (mbb2[R] < mbb1[L]) {
                return false;
            }
        }

        // mbb1[L] coincides with qm[R]
        else if (mbb1[L] == qm[R]) {
            if (mbb2[R] > mbb1[L] || mbb2[L] < mbb1[L]) {
                strict = true;
            }
            if (mbb1[L] != mbb1[R] && mbb2[R] > mbb1[L]) {
                corner = false;
            }
        }

        // mbb1 strictly left of qm
        else if (mbb1[R] < qm[L]) {
            corner = false;
            if (mbb2[L] < mbb1[R]) {
                strict = true;
            }
            else if (mbb2[L] > mbb1[R]) {
                return false;
            }
        }

        // mbb1[R] coincides with qm[L]
        else if (mbb1[R] == qm[L]) {
            if (mbb2[L] < mbb1[R] || mbb2[R] > mbb1[R]) {
                strict = true;
            }
            if (mbb1[L] != mbb1[R] && mbb2[L] < mbb1[R]) {
                corner = false;
            }
        }

        // mbb1[R] right of qm, and mbb1[L] inside qm
        else if (mbb1[R] >= qm[R] && mbb1[L] < qm[R] && mbb1[L] > qm[L]) {
            strict = true;
            corner = false;
        }

        // mbb1[L] left of qm, and mbb1[R] inside qm
        else if (mbb1[L] <= qm[L] && mbb1[R] < qm[R] && mbb1[R] > qm[L]) {
            strict = true;
            corner = false;
        }

        // mbb1[L] left of qm, and mbb1[R] right of qm
        else if ( (qm[L] != qm[R] && mbb1[L] <= qm[L] && mbb1[R] >= qm[R]) || (mbb1[L] < qm[L] && mbb1[R] > qm[R]) ) {
            if ( !(qm[L] == qm[R] && mbb2[L] == mbb2[R] && mbb2[L] == qm[L]) ) {
                strict = true;
            }
            if (qm[L] != qm[R] || mbb2[R] > qm[R] || mbb2[L] < qm[L]) {
                corner = false;
            }
            if ( !(qm[L] == qm[R] && mbb2[R] == qm[R] && mbb2[L] == qm[L]) ) { // this should be equivalent to above
                corner = false;
            }
        }

        // mbb1 is inside qm
        else if (mbb1[L] > qm[L] && mbb1[R] < qm[R]) {
            if ( !(qm[L] == qm[R] && mbb2[L] == mbb2[R] && mbb2[L] == qm[L]) ) {
                strict = true;
            }
            corner = false;
        }

    }

    if (corner) return false;

    return strict;
}
