#include <iostream>
#include <cstdlib> /* 亂數相關函數 */
#include <ctime>   /* 時間相關函數 */

int cal_lba_number(int tl, int unp){
        // If u*n_p = 320 and tl = 10
        // 90% of the data reference comes from lba: 0~31
        int partition_num = unp*tl*0.01 ;
        int x = rand() ;
        if(x % 10 != 9){
            // 90% of possibility comes to here
            int min = 0 ;
            int max = partition_num ; 
            return x % (max - min + 1) + min ; 
        }
        // x == 9
        // 10% of possibility comes to here
        int min = partition_num + 1 ;
        int max = unp-1 ;
            
        return x % (max - min + 1) + min ; 
}
int main() {
    srand(time(NULL)) ;
    int unp = 5000 ;
    int tl = 60 ;  // 90% of the LBA are 0 ~ unp*0.tl
    size_t hot_data = 0 ;
    size_t cold_data = 0 ;
    for(int i = 0 ; i < 10000 ; i++){
        int lba = cal_lba_number(tl,unp);
        if(lba < unp*tl*0.01){
            hot_data++ ; 
        }else{
            cold_data++ ; 
        }
    }
    std::cout << (double)hot_data/10000 << std::endl ; 
    return 0;
}
