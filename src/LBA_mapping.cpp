#include "LBA_mapping.h"
namespace tb{
    LBA_mapping::LBA_mapping(const LBA_mapping& lba_mapping){
        this->size = lba_mapping.size ; 
        this->page_nums = lba_mapping.page_nums ; 
        this->block_ids = lba_mapping.block_ids ; 
    }
    LBA_mapping::LBA_mapping(LBA_mapping&& lba_mapping){
        this->size = lba_mapping.size ; 
        this->page_nums = std::move(lba_mapping.page_nums) ; 
        this->block_ids = std::move(lba_mapping.block_ids) ;
    }
    LBA_mapping& LBA_mapping::operator=(const LBA_mapping& lba_mapping){
        this->size = lba_mapping.size ; 
        this->page_nums = lba_mapping.page_nums ; 
        this->block_ids = lba_mapping.block_ids ;  
        return *this ; 
    }
    LBA_mapping& LBA_mapping::operator=(LBA_mapping&& lba_mapping){
        this->size = lba_mapping.size ; 
        this->page_nums = std::move(lba_mapping.page_nums) ; 
        this->block_ids = std::move(lba_mapping.block_ids) ;
        return *this ; 
    }


    LBA_mapping::LBA_mapping(int size){
        this->size = size ; 
        this->block_ids = std::vector<int>(size,-1) ; 
        this->page_nums = std::vector<int>(size,-1) ;
    }

    // block_id may equal to -1, which means lba_num not yet exists
    // in the disk
    void LBA_mapping::in_which_blk(int lba_num , int* block_id_ptr , 
                                        int* page_num_ptr){
        *block_id_ptr = this->block_ids[lba_num] ;
        *page_num_ptr = this->page_nums[lba_num] ;                   
    }

    void LBA_mapping::update_lba_info(int lba_num , int block_id,
                                        int page_num){
        this->block_ids[lba_num] = block_id ; 
        this->page_nums[lba_num] = page_num ; 
    }
}