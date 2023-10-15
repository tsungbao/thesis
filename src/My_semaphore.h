#ifndef MY_SEMAPHORE_H
#define MY_SEMAPHORE_H
#include <thread>
#include <mutex>
#include <condition_variable>
namespace tb{
    class My_semaphore{
    private:
        int value = 0 ;
        int init_value = 0 ;
        std::mutex m_for_value ; 
        std::condition_variable_any cv ; 
    public:
        My_semaphore(int value){
            this->value = value ; 
            this->init_value = value ; 
        }
        
        ~My_semaphore() = default ; 

        void wait();
        void signal();
        void wait_till_value_restored_init_value();
    };
}
#endif
