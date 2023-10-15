#include "Af_cal.h"
#include <limits>

namespace tb{
    // throws execption
    void Af_cal::valid_incre(){
        if(this->accu_valid > std::numeric_limits<size_t>::max()-5){
            throw "valid count overflowwwww!!!!!" ;
        }
        this->accu_valid++ ; 
    }

    // throws execption
    void Af_cal::invalid_incre(){
        if(this->accu_invalid > std::numeric_limits<size_t>::max()-5){
            throw "invalid count overflowwwww!!!!!" ;
        }
        this->accu_invalid++ ; 
    }
    double Af_cal::output_af(){
        return (double)this->accu_valid / (double)this->accu_invalid ;
    }

    void Af_cal::reset(){
        this->accu_invalid = 0 ;
        this->accu_valid = 0 ;
    }
}