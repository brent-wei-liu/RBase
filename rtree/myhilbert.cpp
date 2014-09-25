#include "../common/pch.h"

/*****************************************************************
this function computes the hilbert value of a multi-dimensional point
para:
num_bit: resolution -- recommeneded 15
dim: dimensionality
coord: coordinates of the point
umax: axis length
return value is an int64 type
coded by yufei tao feb 2004
*****************************************************************/

bitmask_t cal_hilbert(int _num_bit, int _dim, float *_coord, float _umax){
    bitmask_t factor = 1 << _num_bit;
    bitmask_t *coord=new bitmask_t[_dim];
    for (int i=0; i<_dim; i++){
//        coord[i]=bitmask_t(_coord[i]/_umax*( (signed __int64) factor));
        coord[i]=bitmask_t(_coord[i]/_umax*( (long long) factor));
    }
    bitmask_t hil=hilbert_c2i(_dim, _num_bit, coord);
    delete []coord;
    return hil;
}

/*****************************************************************
this function does the same thing as the previous one, except that
it takes a rectangle as the input. the hilbert value is for the centroid
of the rectangle
para:
num_bit: resolution -- recommeneded 15
dim: dimensionality
mbr:
umax: axis length
return value is an int64 type
coded by yufei tao may 2005
*****************************************************************/

bitmask_t cal_hilbert_mbr(int _num_bit, int _dim, float *_mbr, float _umax){
    float *pt = new float[_dim];
    for (int i = 0; i < _dim; i ++)
        pt[i] = (_mbr[2 * i] + _mbr[2 * i + 1]) / 2;
    bitmask_t hil = cal_hilbert(_num_bit, _dim, pt, _umax);
    delete []pt;
    return hil;
}
