#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <vector>
namespace tb{
    class Block{
    private:
        int block_id ; 
        bool free ;
        std::vector<int> pages ;
        int page_cnt ; 
        std::vector<bool> valids ;  // live data
        int valid_cnt ; 
        int cur_incoming_page ; 
    public: 
       
        Block() = default ; 
        Block(int block_id , int page_cnt);

        Block(Block&& blk) ; 
        Block(const Block& blk) ;
        Block& operator=(const Block& blk) ;
        Block& operator=(Block&& blk) ; 
        
        void become_data_placement_blk();
        void become_blk_in_free_pool();

        void incre_cur_incoming_page(){
            this->cur_incoming_page++ ; 
        }

        #ifdef DEBUG
            void print_state(){
                std::cout << "=====" << std::endl ;
                std::cout << "Block id: " << this->block_id << std::endl ;
                if(this->get_free()){
                    std::cout << "\tThis block is free. " << std::endl ; 
                }else{
                    std::cout << std::endl << "\tpages: " ;
                    for(int i = 0 ; i < this->get_cur_incoming_page() ; i++){
                        std::cout << this->get_page_in_pages(i) << " " ; 
                    }
                    std::cout << std::endl << "\tvalids: " ;
                    for(int i = 0 ; i < this->get_cur_incoming_page() ; i++){
                        std::cout << (int)this->is_valid(i) << " " ; 
                    }
                    std::cout << std::endl << "\tvalid cnt: " 
                                << this->get_valid_cnt() ;
                }
                
                std::cout << std::endl << "=====" << std::endl ;
            }
        #endif

        int get_id() const {
            return this->block_id ; 
        }
        void set_page(int page_num, int lba){
            this->pages[page_num] = lba ; 
        }
        void set_valid_cnt(int valid_cnt){
            this->valid_cnt = valid_cnt ; 
        }
        void set_valid_arr(int page_num , bool valid){
            this->valids[page_num] = valid ; 
        }
        int get_valid_cnt() const {
            return this->valid_cnt ; 
        } 
        bool is_valid(int idx){
            return this->valids[idx] ; 
        }
        int get_page_in_pages(int idx) const {
            return this->pages[idx] ; 
        }
        bool get_free() const {
            return this->free ; 
        }
        int get_page_cnt() const {
            return this->page_cnt ; 
        }
        int get_cur_incoming_page() const {
            return this->cur_incoming_page ; 
        }
        
        ~Block() = default ;
    };
}
#endif



