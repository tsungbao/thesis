#ifndef LBA_MAPPING_H
#define LBA_MAPPING_H
#include <vector>
namespace tb{
    class LBA_mapping{
    // If the corresponding LBA num doesn't exist, block_id and page_num will
    // be -1

    /*
        LBA number           0   1   2   3   4   5
        physical_block_id    x   x   x   x   x   x
        physical_page_num    x   x   x   x   x   x
    */
    private:
        int size ; 
        std::vector<int> block_ids ; 
        std::vector<int> page_nums ; 
    public:
        LBA_mapping() = default ; 
        LBA_mapping(int size);
        
        LBA_mapping(const tb::LBA_mapping& lba_mapping) ; 
        LBA_mapping(tb::LBA_mapping&& lba_mapping) ;
        tb::LBA_mapping& operator=(const tb::LBA_mapping& lba_mapping) ;
        tb::LBA_mapping& operator=(tb::LBA_mapping&& lba_mapping) ;

        void in_which_blk(int lba_num , int* block_id_ptr , 
                                        int* page_num_ptr) ; 
        void update_lba_info(int lba_num , int block_id,
                                        int page_num) ; 
        ~LBA_mapping() = default ;
    };
}
#endif