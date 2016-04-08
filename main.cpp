//
// Created by ivan on 08.04.16.
//

#include <iostream>
#include <algorithm>
#include <chrono>
#include <set>
#include <fstream>
#include <limits.h>

#define L1CACHE 32768
#define L2CACHE 262144
#define SAMPLES_NUMB 327680
#define multiply(x) x x x x
using namespace std;
struct Test{
    Test* test;
    int useless;
    int r;
};

void consequentAccess(int step){
    register int m;
    size_t size = step;
    ofstream ofs("consequent.csv");
    Test* t = new Test();
    Test* temp = t;
    for(int i = 0;i < SAMPLES_NUMB; ++i){
        t->test = new Test();
        t = t->test;
    }
    t->test = temp;
    t = temp;
    Test *tt = t;
    while(size < SAMPLES_NUMB){
        timespec begin, end;
        clock_gettime(CLOCK_REALTIME,&begin);
        for(m = 0;m < size;m+=step){
            multiply(multiply(multiply(multiply(tt = tt->test;))))
        }
        clock_gettime(CLOCK_REALTIME,&end);
        ofs << size*sizeof(Test) << ',' << int((end.tv_nsec-begin.tv_nsec)/(double)size)<< "\n";
        if(size < L1CACHE/sizeof(Test)){
            size += step;
        }else{
            size += step * 8;
        }
    }
    while(t->test != NULL){
        temp = t->test;
        delete t;
        t = temp;
    }
    ofs.close();
}
void createRandomList(Test* nodesArray,int size){
    for (int i = 0; i < size; ++i) {
        nodesArray[i].r = i;
    }
    set<int> integerSet;
//    int randomSize = int(SAMPLES_NUMB);
    int random[size];
    for (int i = 0; i < size; ++i) {
        random[i] = i;
    }
    random_shuffle(random, random + size);
    int i = 0;
    int k = 0;
    Test *start = &nodesArray[0];
    while (i != size) {
        int randInt = random[k % size];
        k++;
//        randInt = randInt % SAMPLES_NUMB;
        if (integerSet.find(randInt) == integerSet.end()) {
            if (nodesArray[randInt].r != start->r) {
                start->test = &nodesArray[randInt];
                integerSet.insert(start->r);
                start = start->test;
                i++;
            } else {
                if (i == size - 1) {
                    start->test = &nodesArray[0];
                    break;
                }
            }
        }
    }
}
void randomAccess(int step){

    ofstream ofs("random.csv");
    Test* t = new Test[SAMPLES_NUMB];
    createRandomList(t,SAMPLES_NUMB);
//        Test * t = new Test[size];

    register int m,j;
    size_t size = step;
    int repeat = 5;
    while(size < SAMPLES_NUMB) {

        Test *tt;
//        int repeat = 0;
//        if(size > 100000){
//            repeat = 5;
//        }else{
//            repeat = 5;
//        }
//        for(int i = 0;i < repeat; ++i) {
//            tt = &t[0];
//            for (m = 0; m < size; ++m) {
//                tt = tt->test;
//            }
//        }
        timespec begin, end;
        clock_gettime(CLOCK_REALTIME,&begin);
        for(j = 0; j < repeat; ++j) {
            tt = &t[0];
            for (m = 0; m < size; m += step) {
                multiply(multiply(multiply(multiply(tt = tt->test;))))
            }
        }
        clock_gettime(CLOCK_REALTIME,&end);
        ofs << size*sizeof(Test) << ',' << int((end.tv_nsec-begin.tv_nsec)/((double)(repeat*size)))<< "\n";
        if(size < L1CACHE/sizeof(Test)) {
            size += step / 2;
        }else if (size < L2CACHE/sizeof(Test)){
            size += step * 2;
        }else{
            size += step * 16;
        }

    }
    delete[] t;
}
int main(int argc, char* argv[]) {
    srand(15);
    int step = 256;
    consequentAccess(step);
    randomAccess(step);

    return 0;
}



/* each time create new list
void randomAccess(int step){
    register int m;
    size_t size = step;
    ofstream ofs("random.csv");
    while(size < SAMPLES_NUMB) {

        Test t[size];
//        Test * t = new Test[size];
        for (int i = 0; i < size; ++i) {
            t[i].r = i;
        }
        set<int> integerSet;
//        int randomSize = int(size * 1.5);
        int random[size];
        for (int i = 0; i < size; ++i) {
            random[i] = i;
        }
        random_shuffle(random, random + size);
        int i = 0;
        int k = 0;
        Test *start = &t[0];
        while (i != size) {
            int randInt = random[k % size];
            k++;
            randInt = randInt % size;
            if (integerSet.find(randInt) == integerSet.end()) {
                if (t[randInt].r != start->r) {
                    start->test = &t[randInt];
                    integerSet.insert(start->r);
                    start = start->test;
                    i++;
                } else {
                    if (i == size - 1) {
                        start->test = NULL;
                        break;
                    }
                }
            }
        }
        auto begin = std::chrono::high_resolution_clock::now();
        Test *tt = t[0].test;
        for(m = 0;m < size;m+=step){
            tt = tt->test;
            tt = tt->test;
            tt = tt->test;
        }
        auto end = chrono::high_resolution_clock::now();
        ofs << size*sizeof(Test) << ',' << chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()/size << "\n";
        if(size < L1CACHE/sizeof(Test)){
            size += step;
        }else{
            size += step * 8;
        }
//        delete[] t;
//        delete[] random;
    }
}


 void consequentAccess(int step){
    register int m;
    size_t size = step;
    ofstream ofs("consequent.csv");
    while(size < SAMPLES_NUMB){
        Test* t = new Test();
        Test* temp = t;
        for(int i = 0;i < size; ++i){
            t->test = new Test();
            t = t->test;
        }
        t = temp;

        auto begin = std::chrono::high_resolution_clock::now();
        Test *tt = t->test;
        for(m = 0;m < size;m+=step){
            tt = tt->test;
            tt = tt->test;
            tt = tt->test;
        }
        auto end = chrono::high_resolution_clock::now();
        ofs << size*sizeof(Test) << ',' << chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()/size << "\n";
        if(size < L1CACHE/sizeof(Test)){
            size += step;
        }else{
            size += step * 8;
        }
        while(t->test != NULL){
            temp = t->test;
            delete t;
            t = temp;
        }
    }
    ofs.close();
}*/


/* random heap list
   void createRandomList(Test* nodesArray,int size){
    for (int i = 0; i < size; ++i) {
        nodesArray[i].r = i;
    }
    set<int> integerSet;
//    int randomSize = int(SAMPLES_NUMB);
    int random[size];
    for (int i = 0; i < size; ++i) {
        random[i] = i;
    }
    random_shuffle(random, random + size);
    int i = 0;
    int k = 0;
    Test *start = &nodesArray[0];
    while (i != size) {
        int randInt = random[k % size];// k can be > samples
        k++;
//        randInt = randInt % SAMPLES_NUMB;
        if (integerSet.find(randInt) == integerSet.end()) {
            if (nodesArray[randInt].r != start->r) {
                start->test = &nodesArray[randInt];
                integerSet.insert(start->r);
                start = start->test;
                i++;
            } else {
                if (i == size - 1) {
                    start->test = NULL;
                    break;
                }
            }
        }
    }
}
void randomAccess(int step){

    ofstream ofs("random.csv");
    Test t[SAMPLES_NUMB];
    createRandomList(t,SAMPLES_NUMB);
//        Test * t = new Test[size];

    register int m;
    size_t size = step;
    while(size < SAMPLES_NUMB) {


        auto begin = std::chrono::high_resolution_clock::now();
        Test *tt = t[0].test;
        for(m = 0;m < size;m+=step){
            tt = tt->test;
            tt = tt->test;
            tt = tt->test;
        }
        auto end = chrono::high_resolution_clock::now();
        ofs << size*sizeof(Test) << ',' << chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()/size << "\n";
        if(size < L1CACHE/sizeof(Test)){
            size += step;
        }else{
            size += step * 4;
        }
//        delete[] t;
//        delete[] random;
    }
}*/