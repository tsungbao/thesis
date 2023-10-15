#include "Blocks.h"
#include <iostream>
namespace tb{
    Blocks::Blocks(Blocks&& blks){
        this->blks_cnt = blks.blks_cnt ; 
        this->blks_vec = std::move(blks.blks_vec) ; 
    }
    Blocks::Blocks(const Blocks& blks){
        this->blks_cnt = blks.blks_cnt ; 
        this->blks_vec = blks.blks_vec ; 
    }
    Blocks& Blocks::operator=(const Blocks& blks){
        this->blks_cnt = blks.blks_cnt ; 
        this->blks_vec = blks.blks_vec ; 
        return *this ; 
    }
    Blocks& Blocks::operator=(Blocks&& blks){
        this->blks_cnt = blks.blks_cnt ; 
        this->blks_vec = std::move(blks.blks_vec) ;
        return *this ; 
    }

    Blocks::Blocks(int blks_cnt , int page_cnt){
        // initialize blocks
        for(int i = 0 ; i < blks_cnt ; i++){
            this->blks_vec.push_back(tb::Block(i,page_cnt));
        }
        this->blks_cnt = blks_cnt ; 
    }

    tb::Block& Blocks::get_blk(int blk_id){
        return this->blks_vec[blk_id] ; 
    }


}