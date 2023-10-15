#ifndef BLOCKS_H
#define BLOCKS_H
#include "Block.h"
namespace tb{
    class Blocks{
        private:
            std::vector<tb::Block> blks_vec ; 
            int blks_cnt ; 
        public:
            Blocks() = default ; 
            Blocks(int blks_cnt , int page_cnt);

            Blocks(Blocks&& blks) ; 
            Blocks(const Blocks& blks) ;
            Blocks& operator=(const Blocks& blks);
            Blocks& operator=(Blocks&& blks) ; 


            #ifdef DEBUG
            void print_current_state() {
                for(int i = 0 ; i < this->blks_cnt ; i++){
                    Block& blk = this->blks_vec[i] ;
                    blk.print_state() ; 
                }
            }
            #endif
            
            ~Blocks() = default ;
            Block& get_blk(int blk_id) ; 
    };
}
#endif
