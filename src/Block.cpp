#include "Block.h"
#include <utility>
namespace tb{

    Block::Block(int block_id , int page_cnt){
        // valid = [false , false , ... , false]
        // pages = [-1 , -1 , ... , -1]
        this->block_id = block_id ; 
        this->free = true ; 
        this->page_cnt = page_cnt ; 
        this->pages = std::vector<int>(page_cnt,-1);
        this->valids = std::vector<bool>(page_cnt,false) ; 
        this->valid_cnt = 0 ; 
        this->cur_incoming_page = 0 ;
    }

    void Block::become_data_placement_blk(){
        this->cur_incoming_page = 0 ;
        this->free = false ; 
    }

    void Block::become_blk_in_free_pool(){
        this->free = true ; // Just make sure that it's free
        // purge pages & valid variables
        this->pages = std::vector<int>(page_cnt,-1) ;   
        this->valids = std::vector<bool>(page_cnt,false) ; 
        this->valid_cnt = 0 ; 
        this->cur_incoming_page = 0 ;
    }

    Block::Block(Block&& blk){
        this->block_id = blk.block_id ;  
        this->free = blk.free ;
        this->pages = std::move(blk.pages) ;
        this->page_cnt = blk.page_cnt ; 
        this->valids = std::move(blk.valids) ; 
        this->valid_cnt = blk.valid_cnt ; 
        this->cur_incoming_page = blk.cur_incoming_page ; 
    }
    Block::Block(const Block& blk){
        this->block_id = blk.block_id ;  
        this->free = blk.free ;
        this->pages = blk.pages ;
        this->page_cnt = blk.page_cnt ; 
        this->valids = blk.valids ; 
        this->valid_cnt = blk.valid_cnt ; 
        this->cur_incoming_page = blk.cur_incoming_page ;
    }

    Block& Block::operator=(const Block& blk){
        this->block_id = blk.block_id ;  
        this->free = blk.free ;
        this->pages = blk.pages ;
        this->page_cnt = blk.page_cnt ; 
        this->valids = blk.valids ; 
        this->valid_cnt = blk.valid_cnt ; 
        this->cur_incoming_page = blk.cur_incoming_page ;
        return *this ; 
    }

    Block& Block::operator=(Block&& blk) {
        this->block_id = blk.block_id ;  
        this->free = blk.free ;
        this->pages = std::move(blk.pages) ;
        this->page_cnt = blk.page_cnt ; 
        this->valids = std::move(blk.valids) ; 
        this->valid_cnt = blk.valid_cnt ; 
        this->cur_incoming_page = blk.cur_incoming_page ; 
        return *this ; 
    }





    


}
