#ifndef MODEL_H
#define MODEL_H
#include "Blocks.h"
#include "Window.h"
#include "LBA_mapping.h"
#include "Af_cal.h"
#include <deque>
namespace tb{
    class Model{
    private:
        std::deque<int> free_blks_pool ; 
        std::deque<int> occupied_blks ; 
        Blocks blks ;
        Window window ; 
        LBA_mapping lba_mapping ; 
        int n_p ;
        int pick ; 
        int r ;
        int s ;
        Af_cal af_cal ; 
        void supplement_window(int round) ; 
        int cal_lba_number(int tl, int u,bool result_80_per) ;
                                     // Output the new incoming lba according
                                    // to the provided tl
        int cal_lba_number_random(int u) ; 
        
        void recycle_blk(int blk_id) ; 
            
    public:
        class Data_placement{
        private:
            int cur_data_placement_blk_id ; 
            void change_data_placement_blk(bool in_process_of_gc
                                            ,Model& model) ; 
        public:
            Data_placement() = default ; 
            Data_placement(int cur_data_placement_blk_id, Model& model);
            void data_placement(bool in_process_of_gc , 
                            int lba_num , bool recycle,Model& model) ;  
            ~Data_placement() = default ;
        };
        
        Data_placement dp ; 
        
        Model() = default ;

        double start(int u, int s , int t , int pick , int tl ,
                    int r, int n_p, size_t data_coming_times
                    ,bool result_80_per) ; 
    
        void gc_collection() ;
        
        
        LBA_mapping& get_lba_mapping(){
            return this->lba_mapping ; 
        }
        int get_np(){
            return this->n_p ; 
        }
        int get_s(){
            return this->s ; 
        }
        Blocks& get_blks(){
            return this->blks ; 
        }
        Window& get_window(){
            return this->window ; 
        }
        int get_r(){
            return this->r ; 
        }
        std::deque<int>& get_free_blocks_pool(){
            return this->free_blks_pool ; 
        }
        std::deque<int>& get_occupied_blks(){
            return this->occupied_blks ; 
        }
        tb::Af_cal& get_af(){
            return this->af_cal ; 
        }
        ~Model() = default ;
    };
}

#endif