#include "My_semaphore.h"
namespace tb{
    void My_semaphore::wait(){
        this->m_for_value.lock() ; 
        while(this->value <= 0){
            this->cv.wait(this->m_for_value);
        }
        this->value-- ; 
        this->m_for_value.unlock();
    }   

    void My_semaphore::signal(){
        this->m_for_value.lock() ; 
        this->value++ ; 
        if(this->value > 0){
            cv.notify_all();
        }
        this->m_for_value.unlock();
    } 

    void My_semaphore::wait_till_value_restored_init_value(){
        this->m_for_value.lock() ; 
        while(this->value < this->init_value){
            this->cv.wait(this->m_for_value);
        }
        this->m_for_value.unlock();
    }
}