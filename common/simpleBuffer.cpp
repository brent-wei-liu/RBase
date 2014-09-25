#include "pch.h"

using namespace std;


Page::Page(){
}

void Page::setPage(long pageID, long priority){
    this->pageID = pageID;
    this->key = -priority; // lower key is better
}

bool Page::operator==(const Page &p){
    return (key==p.key);
}

bool Page::operator<(const Page &p){
    return (key<p.key);
}

bool Page::operator>(const Page &p){
    return (key>p.key);
}

bool Page::operator<=(const Page &p){
    return (key<=p.key);
}

bool Page::operator>=(const Page &p){
    return (key>=p.key);
}



int left(int i) {
        //left child is at 2*i(index of self) + 1
        return 2*i + 1;
}


int right(int i) {
                //right child is at 2*i(index of self) + 2
        return  2* i + 2;
}


template <class T> void MaxHeapify(T *input, int index, int size) {
        //let the index of the largest element be the one that was
        //passed in for now
        int largestIndex = index;
        int leftIndex = left(index);
        int rightIndex = right(index);

        //ensure that the leftIndex will not go out of bounds
        if (leftIndex < size) {
                //if the value at the left child is greater than
                //the current largest index, then make the
                //largest index be the left child's index
                if(input[leftIndex] > input[largestIndex]) {
                        largestIndex = leftIndex;
                }
        }

        //ensure that the rightIndex will not go out of bounds
        if (rightIndex < size) {
                //if the value at the right child is greater than
                //the current largest index, then make the
                //largest index be the right child's index
                if (input[rightIndex] > input[largestIndex]) {
                        largestIndex = rightIndex;
                }
        }
        //if the index of the largest element was changed
        //swap the value of the child with the value of the root
        //to maintain the max heap property
        if (largestIndex != index) {
                Page temp = input[index];
                input[index] = input[largestIndex];
                input[largestIndex] = temp;

                //since the child was swapped with it's parent
                //now we have to max heapify the child to maintain the max
                //heap property. since the child was the largest index, we pass
                //the largest index to the MaxHeapify method
                MaxHeapify(input, largestIndex, size);

        }
        return;
}




simpleBuffer::simpleBuffer(int bufSize) {
    globalTime = 0;
    currSize = 0;
    pHits = bHits = pMisses = bMisses = 0;
    this->bufSize = bufSize;
    bf = new Page[bufSize];
}

simpleBuffer::~simpleBuffer(){
    delete[] bf;
}

int simpleBuffer::find(long pageID){
    int ndx=-1;
    for (int i=0; i<currSize; i++){
        if (bf[i].pageID==pageID){
            ndx=i;
            break;
        }
    }
    return ndx;
}


void simpleBuffer::reset(int bufSize){
    this->bufSize = bufSize;
    globalTime = 0;
    currSize = 0;
    pHits = bHits = pMisses = bMisses = 0;
    delete[] bf;
    bf = new Page[bufSize];
}


ostream& operator <<(ostream &os, const simpleBuffer &buf){
    os << "********* simpleBuffer *********" << endl;
    os << "currSize = " << buf.currSize << endl;
//        for (int i=0; i<currSize; i++){
//            os << buf.bf[i].pageID << ": " << buf.bf[i].key << endl;
//        }
    os << "pHits = " << buf.pHits << endl;
    os << "pMisses = " << buf.pMisses << endl;
    os << "bHits = " << buf.bHits << endl;
    os << "bMisses = " << buf.bMisses << endl;
    os << "********************************" << endl;
    return os;
}


void simpleBuffer::expand(int newSize) {
    assert(newSize > currSize);
    Page *newbf = new Page[newSize];
    memcpy(newbf, bf, currSize*sizeof(Page));
    delete[] bf;
    bf = newbf;
    bufSize = newSize;
}


void simpleBuffer::shrink(int newSize) {
//    assert(newSize < bufSize);
    Page *newbf = new Page[newSize];

    while (currSize > newSize) {
        bf[0] = bf[currSize-1];
        currSize--;
        MaxHeapify<Page>(bf, 0, currSize);
    }

    memcpy(newbf, bf, currSize*sizeof(Page));
    delete[] bf;
    bf = newbf;
    bufSize = newSize;
}


bool simpleBuffer::isHit(long realPageID, int type, long priority){
    long pageID = type*MAXPAGE + realPageID;
    bool hit = false;

    if (priority < 0) {
        priority = ++globalTime;
    }


    int ndx = find(pageID);

    if (ndx == -1){
        if (currSize < bufSize){
            bf[currSize].setPage(pageID, priority);
            currSize++;
        }
        else {
            bf[0].setPage(pageID, priority);
        }
    }
    else {
        bf[ndx].setPage(pageID, priority);
        hit = true;
    }
    MaxHeapify<Page>(bf, 0, currSize);
    if (type == PPAGE) {
        if (hit) {
            pHits++;
        }
        else{
            pMisses++;
        }
    }
    else if (type == BPAGE) {
        if (hit) {
            bHits++;
        }
        else{
            bMisses++;
        }
    }

    return hit;
}




