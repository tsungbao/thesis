#include "Model.h"
#include <cstdlib> /* 亂數相關函數 */
#include <ctime>   /* 時間相關函數 */
#include <iostream>
namespace tb{
    // Output the new incoming lba according
    // to the provided tl
    // tl = 10 => 90/10 rule
    int Model::cal_lba_number(int tl, int u,bool result_80_per){
        // If u*n_p = 320 and tl = 10
        // 90% of the data reference comes from lba: 0~31
        int unp = u*this->n_p ; 
        int partition_num = unp*tl*0.01 ;
        int x = rand() ;
        if(result_80_per){
            if(x % 10 == 9 || x % 10 == 8){
                // 20% of possibility comes to here
                int min = partition_num + 1 ;
                int max = u*this->n_p-1 ;
                
                return x % (max - min + 1) + min ; 
            }
            // 80% of possibility comes to here
            int min = 0 ;
            int max = partition_num ; 
            return x % (max - min + 1) + min ; 
        }

        // result 90 %

        if(x % 10 != 9){
            // 90% of possibility comes to here
            int min = 0 ;
            int max = partition_num ; 
            return x % (max - min + 1) + min ; 
        }
        // x == 9
        // 10% of possibility comes to here
        int min = partition_num + 1 ;
        int max = u*this->n_p-1 ;
            
        return x % (max - min + 1) + min ; 
    }

    int Model::cal_lba_number_random(int u){
        int x = rand() ;
        return x % (u*this->n_p) ; 
    }

    void Model::supplement_window(int round){
        for(int i = 0 ; i < round ;i++){
            // The block which is going to be supplemented
            // to the window
            int blk_id = this->occupied_blks.front() ; 
            this->occupied_blks.pop_front();

            this->window.add_blk(this->blks.get_blk(blk_id)) ;
            
            #ifdef DEBUG
                std::cout << "blk " << this->occupied_blks.front() 
                          << " be supplemented into the window!!" 
                          << std::endl ;  
            #endif
            
        }
    }
    // may throw exception 
    double Model::start(int u, int s , int t , int pick , int tl ,
                    int r, int n_p, size_t data_coming_times,
                    bool result_80_per) {
        #ifdef DEBUG
            srand(1) ;
        #else
            srand(time(NULL)) ;
        #endif
        occupied_blks.clear() ;
        free_blks_pool.clear() ;
        
        // initialize free blocks pool 
        for(int i = 0 ; i < t ; i++){
            this->free_blks_pool.push_back(i) ; 
        }
        this->s = s ;
        this->r = r ; 
        this->pick = pick ; 
        this->n_p = n_p ; 
        
        // 1. Initialize Blocks with t and ask Blocks to initialize blocks
        //    within it.
        this->blks = Blocks(t,n_p) ; 
        
        // 2. Initialize Window with size s and ask window to initialize
        //    a min heap.
        this->window = Window(s) ; 
        
        // 3. Initialize Lba_mapping with u*n_p.
        this->lba_mapping = LBA_mapping(u*this->n_p) ; 

        this->dp = Data_placement(free_blks_pool.front(),*this) ;
        
        free_blks_pool.pop_front();
        
        for(size_t i = 0 ; i < data_coming_times ; i++){
            int lba ; 
            if(tl==-1){
                // random
                lba = this->cal_lba_number_random(u) ; 
            }else{
                lba = this->cal_lba_number(tl,u,result_80_per) ; 
            }
            
            // may throw exception 
            this->dp.data_placement(false,lba,false,*this) ;  
        }
        #if defined(TEST) || defined(DEBUG)
            this->af_cal.print_af();
        #endif
        return this->af_cal.output_af() ; 
    }

    // may throw exception 
    void Model::gc_collection(){
        #ifdef DEBUG
            std::cout << "Garbage collection starts!!!" << std::endl ; 
        #endif

        // Supplement window from occupied blocks first 
        this->supplement_window(this->s) ;

        #ifdef DEBUG
            this->window.print_window();
        #endif

        int pick_tmp = this->pick ;
        while(pick_tmp > 0){

            int min_blk_id = this->window.pick_min();
            #ifdef DEBUG
                std::cout << "Block " << min_blk_id << " be picked"
                          << std::endl ; 
            #endif

            // may throw exception 
            this->recycle_blk(min_blk_id) ;
              
            pick_tmp-- ;
        }

        // Push blocks in the window back to the front of occupied blocks
        while(this->window.get_cur_cnt() > 0){
            this->occupied_blks.push_front(this->window.pick_min());
        }


        #ifdef DEBUG
            this->window.clear_elements();
            std::cout << "Garbage collection ends!!!" << std::endl ; 
        #endif
    }

    // may throw exception 
    void Model::recycle_blk(int blk_id){
        Block& recycled_blk = this->blks.get_blk(blk_id) ;
        for(int i = 0 ; i < this->n_p ; i++){
            if(recycled_blk.is_valid(i)){
                // This page has live data
                this->dp.data_placement(true,recycled_blk.get_page_in_pages(i)
                                                        ,true,*this) ;
                // update af calculator
                this->af_cal.valid_incre();
                // may throw exception
            }else{
                // This page has dead data
                
                // update af calculator
                this->af_cal.invalid_incre(); 
                // may throw exception        
            }
        }
        recycled_blk.become_blk_in_free_pool();
        this->free_blks_pool.push_back(blk_id) ;
    }



}