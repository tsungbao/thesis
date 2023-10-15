#ifndef WINDOW_H
#define WINDOW_H
#include "Block.h"
#include <memory>
#include <queue>       // std::priority_queue
namespace tb{
    class Window{
    private:
        int window_max_size ;       
        class Window_comparator{
            public: 
            bool operator()(const std::pair<int,int>& p1
                            , const std::pair<int,int>& p2){
                    // return "true" if "p1" is ordered
                    // before "p2", for example:
                    return p1.second > p2.second ;
            }
        };                      // <blk_id , valid_cnt>
        std::priority_queue<std::pair<int,int>, 
                            std::vector<std::pair<int,int>>,
                            Window_comparator> pq;
        #ifdef DEBUG
             std::vector<std::pair<int,int>> elements_in_window ; 
        #endif
       
    public:
        Window() = default ;
        
        Window(int window_max_size);

        Window(Window& window){
            this->window_max_size = window.window_max_size ; 
        }
        Window(Window&& window){
            this->window_max_size = window.window_max_size ; 
        }
        Window& operator=(Window& window){
            this->window_max_size = window.window_max_size ; 
            return *this ; 
        }
        Window& operator=(Window&& window){
            this->window_max_size = window.window_max_size ; 
            return *this ; 
        }

        void add_blk(Block& block) ;
        int pick_min() ; // Output the block id who has the least valid
                        // page count among all other blocks in the 
                        // window. 

        int get_cur_cnt(){
            return pq.size() ; 
        }  
        
        #ifdef DEBUG
            void print_window(){
                std::cout << "Window: " ;
                for(int i = 0; i < (int) elements_in_window.size() ; i++){
                    std::cout << "< " << elements_in_window[i].first 
                            << " , " 
                            << elements_in_window[i].second << " >" 
                            << std::endl ;
                }       
            }
            void clear_elements(){
                elements_in_window.clear();
            }
        #endif

        ~Window() = default ;
    };
}
#endif
