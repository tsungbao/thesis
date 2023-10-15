#include "Window.h"
#ifdef DEBUG
    #include <iostream>
#endif
namespace tb{
    Window::Window(int window_max_size){
        // initialize My_min_heap 
        this->window_max_size = window_max_size ; 
    }

    // Output block id of the block whose valid_cnt is the lowest
    int Window::pick_min(){ 
        std::pair<int,int> blk_pair = this->pq.top() ;
        this->pq.pop() ;
        return blk_pair.first ; 
    }

    void Window::add_blk(Block& block){
        std::pair<int,int> blk_pair(block.get_id(),block.get_valid_cnt());
        this->pq.push(blk_pair) ;        
        #ifdef DEBUG
           elements_in_window.push_back(blk_pair) ; 
        #endif
    }

}