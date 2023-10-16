# Simulator
This is a simulator producing the write amplification factor for the garbage collection scheme of NAND flash SSDs.
## Math symbols
Before introducing the architecure of the garbage collection, we first go through some symbols.

1. $n_p$: The number of pages in a block.
2. $V,I$: In the simulation, garbage collection will be activated many times; as a result, a large number of victim blocks will be recycled. During simulation, there are $V$ valid (live) pages collected from all the victim blocks, and $I$ invalid (dead) pages collected from all the victim blocks. In this thesis, a valid page and a live page are synonymous, and they will be used interchangeably. Likewise, an invalid page and a dead page are synonymous, and they will be used interchangeably.
3. $A_f$: An indicator to measure how well a garbage collection scheme performs is the write amplification factor $A_f = V/I$. In order to recycle a fixed number of $I$ invalid pages in the victim blocks (by erasing those dead pages and turning them into free pages), $V$ pages have to be consumed elsewhere to preserve the data in them; therefore, $V$ and $I$ can be thought of as the cost and the benefit of garbage collection, respectively. A garbage collection scheme with a low $A_f$ means such garbage collection scheme is more efficient. 
4. $t,u$: The overall physical capacity, which is the raw storage capacity,  provided by the NAND flash SSD is $t$ blocks. The storage capacity that can be used by the users is $u$ blocks, where $u \lt t$. So the logical space can take up at most $u/t$ of physical space. For example, suppose $u=5$ and $t=10$. When a user has already stored $5n_p$ pages of data, the storage system will appear full to him. Nonetheless, in reality, there are yet another $5n_p$ pages which are either unoccupied or store invalid data. Thus the logical space can take up at most 50\% of the physical space.
5. $s,r$: All the occupied blocks are kept in the occupied blocks pool. The $s$ oldest blocks of the occupied blocks pool, which are the earliest $s$ blocks in the occupied blocks pool, form a window. All the free blocks are kept in the free blocks pool.  A round of garbage collection will be triggered when there are fewer than $r$ blocks in the free blocks pool. In other words, a round of garbage collection will be triggered when there are $t-r$ blocks in the occupied blocks pool.
6. $t_\ell$: We now model the hot-cold access pattern. This model assumes the ratio $t_\ell$ ($0 < t_\ell < 100\%$) of all the possible LBAs that may be issued by the write operations from the upper layer are the hot data, while the rest of $(1-t_\ell)$ of them are the cold data. The reason only write operations are considered is that read operations will not change the blocks in the NAND flash memory. For example, a write operation usually causes data in an LBA to become stale; in contrast, no data change after a read operation. This model assumes 80\% (the possibility of other numerical values is discussed in Chapter \ref{chap:conclusion}) of the write operations from the upper layer consist of hot data while the rest of 20\% of them consist of cold data. For example, suppose $t_\ell = 10\%$ and the possible LBAs that may be issued by the upper layer range from LBA 0 to LBA 9. Therefore, one out of ten LBAs is hot data. Assume LBA 0 is the hot data. As a result, 80\% of the write operations from the upper layer consist of LBA 0, while the rest of 20\% of them consist of LBAs 1 $\sim$ 9. Finally, this model assumes each LBA is issued by the upper layer independently.    
7. $M$ blocks are picked as the victim blocks in every round of garbage collection.

## Architecure
There are five steps in total.
![alt text](https://github.com/tsungbao/thesis/blob/master/miscellaneous/Architecture.png?raw=true)
1. The incoming LBAs issued independently by the upper layer are filled into a block retrieved from the free blocks pool.
2. Once the block is full, it will be moved to the end of the occupied blocks pool.
3. During a round of garbage collection, 𝑀 victim blocks from the window will be picked, where 𝑀 ≤ 𝑠. The victim blocks are chosen as those 𝑴 blocks with the fewest numbers of live pages.
4. The first block of the free blocks pool is selected to accommodate the data in live pages of the victim blocks from step 3. If one block is not enough, move the full block to the end of the occupied blocks pool as step 2, then repeat step 4.
5. The victim blocks are erased, become free blocks to be inserted back into the free blocks pool.

## Program
### How to start the program?
```
1. docker-compose up --build -d
2. docker exec -it program-mythesis-1 sh
```
===== NOW, WE'RE IN THE CONTAINER ====

```
3. ./bin/test.o
```

After running `./bin/test.o` program, output will be sent to `result_average.txt` in a ***docker volume (host volume)*** linked to `docker_volume`, which is a ***host directory***, for persistent storage.

### Inputs
There are four parameters as the input. They are
1. $t_\ell$: Introduced above. Note that $t_\ell = -1$% means the random access pattern is being used instead of the hot-cold access pattern introduced above.

2. $r$: Introduced above.

3. $u/t$: The ratio of logical capacity to physical capacity.

4. $M/s$: The ratio of the number of the victim blocks per round of garbage collection to the number of blocks in the window.
   

The inputs are hard-coded in test/basic_test.cpp as shown in the following figure.
![alt text](https://github.com/tsungbao/thesis/blob/master/miscellaneous/inputs.jpg?raw=true)

### Output

### Class diagram

### Special parts
