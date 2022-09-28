#include <iostream>
#include <set>
#include <random>
#include <fstream>

using State = int ;

class BaseState {
public:
    virtual bool contains (State s) const = 0;

};


class DiscreteState: public BaseState {
private :
    State const s0 ;
public :
    DiscreteState(State s0): s0(s0) { }
    bool contains (State s) const override{
        return s == s0; }

};

class SegmentState: public BaseState {
private :
    State const begin_s0 , end_s0 ;
public :
    SegmentState ( State begin_s0 , State end_s0 ) : begin_s0 ( begin_s0 ) , end_s0 ( end_s0 ) { }
    bool contains(State s) const override{
        return begin_s0 <= s and end_s0 >= s ;
    } };

class ProbabilityTest {
private :
    State const E_min, E_max;
public :
    ProbabilityTest(State E_min, State E_max) :
            E_min(E_min), E_max(E_max) {}

    float Test (BaseState * system, unsigned test_count, unsigned seed){
        std :: default_random_engine reng ( seed ) ;
        std :: uniform_int_distribution<int> dstr(E_min, E_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt) if (system->contains(dstr(reng))) ++good;
        return
                static_cast<float >(good) /static_cast<float >(test_count );
    }
};


int main() {
    std::ofstream out_d, out_s;
    out_d.open("data_d.txt"); 
    out_s.open("data_s.txt");

    for (int i = 100000; i < 1000000; i = i + 10000) {
        BaseState *d = new DiscreteState(0);
        BaseState *s = new SegmentState(0, 100);
        ProbabilityTest pt(-1000, 1000);
        out_d << pt.Test(d, i, 1) << std::endl;
        out_s << pt.Test(s, i, 1) << std::endl;
    }
}