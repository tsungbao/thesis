#ifndef AF_CAL
#define AF_CAL
#include <cstddef>
#include <iostream>
namespace tb{
    class Af_cal{
    private:
        // Neither accu_valid nor accu_invalid is able to exceed the 
        // limitation of c++ ULLONG_MAX
        size_t accu_valid = 0;
        size_t accu_invalid = 0 ;
    public:
        Af_cal(/* args */) = default;
        void valid_incre() ;
        void invalid_incre() ;
        double output_af() ;
        
        size_t get_invalid(){
            return this->accu_invalid ; 
        }

        size_t get_valid(){
            return this->accu_valid ; 
        }

        void reset() ; 
        #if defined(TEST) || defined(DEBUG)
            void print_af(){
                std::cout << "valid: " << this->accu_valid << std::endl ;
                std::cout << "invalid: " << this->accu_invalid << std::endl ;
            }
        #endif 
        ~Af_cal() = default ;
    };
}
#endif