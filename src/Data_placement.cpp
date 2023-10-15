#include "Model.h"

namespace tb{
    // may throw exception 
    void Model::Data_placement::change_data_placement_blk
                    (bool in_process_of_gc, Model& model) {
        int old_blk_id = this->cur_data_placement_blk_id ;

        // pop the first block in free blocks pool
        int new_blk_id = model.get_free_blocks_pool().front() ;
        model.get_free_blocks_pool().pop_front();

        // set the previously popped block as data_placement_blk
        model.get_blks().get_blk(new_blk_id).become_data_placement_blk();
        this->cur_data_placement_blk_id = new_blk_id ;

            // If we're under the process of gc,
            // we're not going to it again recursively.
        if(!in_process_of_gc && 
            model.get_free_blocks_pool().size() 
                                    <= (size_t) model.get_r()){
            // may throw exception 
            model.gc_collection();
        }

        // throw the filled old block to occupied blks
        model.get_occupied_blks().push_back(old_blk_id) ; 
    }
    
    
    Model::Data_placement::Data_placement(int cur_data_placement_blk_id, Model& model){
        this->cur_data_placement_blk_id = cur_data_placement_blk_id ;
        model.get_blks().get_blk(cur_data_placement_blk_id).become_data_placement_blk();
    }

    // may throw exception 
    // place lba_num to this->cur_page_num of 
    // this->cur_data_placement_blk_id
    void Model::Data_placement::data_placement(bool in_process_of_gc , 
                                int lba_num , bool recycle,Model& model){
       #ifdef DEBUG
            std::cout << "incoming lba: " << lba_num << std::endl; 
        #endif
        LBA_mapping& lba_mapping = model.get_lba_mapping();

        if(!recycle){
            // update valid_cnt & valid[] of the corresponding old block
            int physical_block_id ;
            int physical_page_num ;
            lba_mapping.in_which_blk(lba_num,&physical_block_id
                                            ,&physical_page_num) ;
            if(physical_block_id < 0){
                // this lba_num not yet exists in the disk
            }else{
                tb::Block& old_blk = model.get_blks().get_blk(physical_block_id) ;
                old_blk.set_valid_arr(physical_page_num,false) ; 
                old_blk.set_valid_cnt(old_blk.get_valid_cnt()-1) ; 
            }
        }

        Block& blk = model.get_blks().get_blk(this->cur_data_placement_blk_id) ;
        blk.set_valid_arr(blk.get_cur_incoming_page(),true) ;
        blk.set_valid_cnt(blk.get_valid_cnt()+1) ; 
        blk.set_page(blk.get_cur_incoming_page(),lba_num);

        // update lba_mapping
        lba_mapping.update_lba_info(lba_num,this->cur_data_placement_blk_id
                                        ,blk.get_cur_incoming_page()) ;
        blk.incre_cur_incoming_page();

        if(model.get_np() <= blk.get_cur_incoming_page()){
            // I want to check the current state of blocks
            // whenever the data placement block is needed to be
            // changed
            #ifdef DEBUG
                std::cout << "changing data placement blk..." 
                          << std::endl ;
                std::cout << "Current state of all blks:" 
                          << std::endl ;
                model.get_blks().print_current_state() ; 
            #endif
            // may throw exception 
            this->change_data_placement_blk(in_process_of_gc,model) ;
        }
    }

}  