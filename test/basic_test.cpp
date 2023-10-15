#include "../src/Model.h"
#include "../src/My_semaphore.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <cstdio>
#include <cmath>

void initial_setup(int& s , int& t, int& n_p, size_t& data_coming_times
                    , bool& result_80_per){
    s = 500 ;  // 500
    t = 40000 ; // 40000
    n_p = 64 ; // 64
    data_coming_times = 5 * n_p * t ; 
    // 1 round => n_p*t. Here, 5 rounds

    result_80_per = true ; // if == true, tl% of the LBA account 
                           // for 80% data reference
    
}

void thread_op(int u, int s, int t, int pick, int tl, int r
                , int n_p , size_t data_coming_times 
                , std::vector<double>& return_af,int task_num
                , tb::My_semaphore& thread_sem,bool result_80_per){
    double a_f = 0 ;
    try{
        tb::Model model ;  

        // may throw exception 
        // tl = -1 => random
        a_f = model.start(u,s,t,pick,tl,r,n_p,data_coming_times,result_80_per) ; 
        return_af[task_num] = a_f ; 
        thread_sem.signal(); 
    }catch(const char* message){
        // "invalid count overflowwwww!!!!!" or 
        // "valid count overflowwwww!!!!!"
        std::cout << "Exception: " << message << std::endl;
    }
    
}

std::vector<double> distribute_tasks_to_threads(int thread_cnt,
                                 std::vector<double>& picks_ratios,
                                 int smaller_of_r_s,
                                 int u, int s, int t, int tl, int r,
                                 int n_p, size_t data_coming_times,
                                 bool result_80_per){
    
    /*
        picks = {0.1,0.3,0.5,0.7}
        smaller_of_r_s should be >= every element in picks,
        otherwise, we should remove the elements that violate the rule.
    */

    // int tasks_cnt = picks.size() ; 

    int tasks_cnt = picks_ratios.size() ; 
    for(int i = 0 ; i < picks_ratios.size() ; i++){
        int pick = (int)(s * picks_ratios[i]) ; 
        if(smaller_of_r_s <= pick){
            tasks_cnt = i ; 
            break ; 
        }
    }

    std::vector<double> return_afs(tasks_cnt,0);
    /*
        Perhaps there are plenty of picks, which are our tasks.
        The number of elements in picks may surpass the number of
        threads that we have right now.

        picks = [... many many ...]

        Threads will choose tasks from picks array, after they finish it,
        they'll check out picks array to see if there are remaining 
        tasks that haven't been done.
    */
    int task_num = 0 ;
    tb::My_semaphore thread_sem(thread_cnt) ; // "thread_cnt" threads
                                              // Why do I need this
                                              // semaphore? Since I 
                                              // don't want to create
                                              // too many threads
    while(task_num < tasks_cnt){
        int pick = (int)(s * picks_ratios[task_num])  ; 
        // int pick = picks[task_num] ;

        // Although return_afs is shared variable among multiple threads,
        // it actually won't cause race condition. This is because that 
        // each thread will only change the value of return_afs[task_num].
        // The task_num that each thread given is different.
        std::thread worker_thread(thread_op,u,s,t,pick,tl,r,n_p
                            ,data_coming_times,
                            std::ref(return_afs),task_num,
                            std::ref(thread_sem),result_80_per);
        task_num++ ; 
        worker_thread.detach();
        thread_sem.wait() ; // Sleep, if there's no remaining thread.
    }

    // Wait for all the threads that have not yet finished their jobs
    thread_sem.wait_till_value_restored_init_value() ; 

    return return_afs ; 
}


int main(){

    std::ofstream myfile("/thesis/docker_volume/result_average.txt",std::ios::app) ; 
    int s, t, n_p ;
    size_t data_coming_times ;
    bool result_80_per ; 
    initial_setup(s,t,n_p,data_coming_times,result_80_per) ; 
    
    myfile << "s: " << s << " , t: " << t 
           << " , n_p: " << n_p << std::endl;
    
    double a_f = 0 ; 

    int threads_cnt = 4 ;

    // tb::Model model ; 
    // // int tl = 10 ;
    // int pick = 3 ; 
    // int u_to_t = 50 ; 
    // int u = (int) u_to_t * t * 0.01 ; 
    // int r = 2 ; 
    // try{
    //     a_f = model.start(u,s,t,pick,0,r,n_p,data_coming_times) ; 
    // }catch(const char* message){
    //     // "invalid count overflowwwww!!!!!" or 
    //     // "valid count overflowwwww!!!!!"
    //     std::cerr << "Exception: " << message << std::endl;
    //     return 1 ;
    // }

    // myfile << "random" << "%," << u_to_t << "%," << r << "," << pick 
    //     <<"," << a_f << std::endl ; 
    
                    // tl = -1 => random
    std::vector<int> tls = {-1,10,30,50} ; // {-1,10,30,50}
    std::vector<int> rs = {10,500,1000,1500} ; // {10,500,1000,1500}
    std::vector<int> u_to_ts = {10,30,50,70,90} ; 
    std::vector<double> picks_ratios = {0.1,0.3,0.5,0.7} ; //{0.1,0.3,0.5,0.7}

    for(int tl : tls){
        for (int u_to_t : u_to_ts){
            int u = (int) u_to_t * t * 0.01 ;   
            for (int r : rs){
                std::vector<std::vector<double>> a_f_results ; 
                for(int repeat = 0 ; repeat < 100 ; repeat++){
                    // r and s should be larger than "pick" in order to
                    //  accomodate live pages from "pick" victim blocks

                    int smaller_of_r_s = (r < s)?r:s ;

                    a_f_results.push_back(distribute_tasks_to_threads(threads_cnt,picks_ratios
                                                ,smaller_of_r_s,
                                                u, s, t, tl, r, n_p, 
                                                data_coming_times,result_80_per));

                }

                /*                   M/s  0.1 0.3 0.5 0.7
                    a_f_results     = [ [  x   x   x   x  ]
                                        [  x   x   x   x  ]
                                                .               } repeat
                                                .
                                                .
                                        [  x   x   x   x  ] ]

                    average_a_f     =   [  x   x   x   x  ]
                    stand_err_a_f   =   [  x   x   x   x  ]
                */

                std::vector<double> average_a_f ; 
                std::vector<double> stand_err_a_f ;

                double repeat_size = (double)a_f_results.size() ; 

                for(int j = 0 ; j < a_f_results[0].size() ; j++){
                    double sum = 0 ; 
                    for(int i  = 0 ; i < (int)repeat_size ; i++){
                        sum += a_f_results[i][j] ; 
                    }
                    double average = sum / repeat_size ;
                    average_a_f.push_back(average) ; 

                    double sum_for_deviation = 0 ;
                    for(int i  = 0 ; i < (int)repeat_size ; i++){
                        sum_for_deviation += ((a_f_results[i][j]-average)*(a_f_results[i][j]-average)) ; 
                    }

                    double std_dev = sqrt(sum_for_deviation
                                                /repeat_size) ;
                    stand_err_a_f.push_back(std_dev/sqrt(repeat_size)) ;
                }


                for(int j = 0 ; j < average_a_f.size() ; j++){
                    myfile << tl << "%," << u_to_t << "%," << r 
                        << "," 
                        << picks_ratios[j] 
                        //    << 1
                        << "," << average_a_f[j] 
                        << "," << stand_err_a_f[j] << ",    [" ; 
                    for(int i = 0 ; i < (int)repeat_size ; i++){
                        if(i == (int)repeat_size - 1){
                            myfile << a_f_results[i][j] ;
                        }else{
                            myfile << a_f_results[i][j] << ", " ;
                        }
                    } 
                    myfile << "]" << std::endl ; 
                }  
            }
        }
    }

    myfile.close() ;

    return 0 ; 
}