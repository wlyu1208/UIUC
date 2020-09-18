/* page.c - initializing structure to implement paging mechanism, also turn on paging by setting controller register 
 * set up a PDE, PTE
 * PDE = 1kb, PTE = 1kb, kernel is a single 4MB page, first 4 MB is 1000 4kB pages.
 */
#ifndef PAGE_C
#define PAGE_C
#include "page.h"
#include "types.h"

/* some reference for future reader:
 * https://wiki.osdev.org/Setting_Up_Paging
 * https://courses.grainger.illinois.edu/ece391/fa2019/secure/references/descriptors.pdf  
 *          search keyword: 1. page for page descriptor 2.0xB8000 for video memory block
 * https://en.wikipedia.org/wiki/Control_register and https://wiki.osdev.org/CR0#CR0 for control register
 * extended asm can be founded on https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
 *                                                                                
 */

/* 
 *  for i in size:
 *      PDE = 0;
 *      PTE = address | video memory mask
 *  PDE[0] = & PTE <<12
 *  PDE[0] |= 4kb video memory mask
 *  PDE[1] = kernel address
 *  PDE[1] |= 4mb kernel memory mask
 *  also initiate the video memory
 *  enable paging in control register
 */

// 1k pt, each pt has 4kB
uint32_t page_directory_entry[TABLE_SIZE] __attribute__((aligned(PAGE_CAPACITY)));

//1k pages, each page has 4kb
uint32_t page_table_entry[TABLE_SIZE] __attribute__((aligned(PAGE_CAPACITY)));




/* init_paging()
 * description: initialize the structure of paging, enable paging
 * inputs: none
 * outputs: none
 * side effect: cr changed
 */
void init_paging()
{
    int i;
    for(i=0;i<TABLE_SIZE;i++) // init first 0-4mb
    {
        init_single_page((&page_directory_entry[i]),0,read_write); // init each pde
        init_single_page(&page_table_entry[i],i*PAGE_CAPACITY,read_write); // init each pte
    }

    init_single_page(&page_directory_entry[0],(unsigned int)page_table_entry,read_write | present_mask); // init first PDE, set address
    init_single_page(&page_directory_entry[1],kernel_address,kernel_mask); // init kernel page
    init_single_page(&page_table_entry[video_address],video_address<<shiftbits,present_mask|read_write); // init video page
    
    /* store address of PDE array into CR3,
     * change bit 31 of cr0 to enable paging
     * change bit 4 of cr4 to enable 4mb page size
     */
    asm volatile(
                 "xorl %%eax,%%eax;"
                 "movl %0, %%eax;"
                 "movl %%eax, %%cr3;"   // move PDE address to cr3

                 "xorl %%eax,%%eax;"
                 "movl %%cr4, %%eax;"
                 "orl $0x00000010, %%eax;"
                 "movl %%eax, %%cr4;"   // set bit 4 of cr4 to 1

                 "xorl %%eax,%%eax;"
                 "movl %%cr0, %%eax;"
                 "orl $0x80000000, %%eax;"
                 "movl %%eax, %%cr0;"   // set bit 31 of cr0 to 1

                :
                : "r"(page_directory_entry) 
                : "eax"
                );
}

/* initialize_single_page(unsigned int entry,unsigned int address, unsigned int mask)
 * inputs: entry = PDE or PTE memory
 *          address = physical memory of page store in PTE
 *          mask = lower 9 bits to set
 * 
 * outputs: none
 * side effect: change physical memory
 */
void init_single_page(unsigned int* entry,unsigned int address,unsigned int mask)
{
    *entry = address | mask;
}

/* map_from_vir_to_phy(unsigned int vir_address,unsigned int phy_address)
 * inputs: vir_address = virtual address
 *          phy_address = physical address
 * 
 * outputs: none
 * side effect: initialize single page for specific virtual and physical addresses
 */
void map_from_vir_to_phy(unsigned int vir_address,unsigned int phy_address)
{
    init_single_page(&page_directory_entry[vir_address/mem_4mb],phy_address,read_write|present_mask|large_page_size|user_bit);
    //flush tlb
    asm volatile(   "movl %%cr3,%%eax;"
                    "movl %%eax,%%cr3;"
                    :
                    :
                    :"%eax"
                );
}


/* map_from_vir_to_phy_with_4kb_size_page(unsigned int vir_address,unsigned int phy_address)
 * inputs: vir_address = virtual address
 *          phy_address = physical address
 * 
 * outputs: none
 * side effect: initialize single page for specific virtual and physical addresses with 4kb size page
 */
void map_from_vir_to_phy_with_4kb_size_page(unsigned int vir_address, unsigned int phy_address)
{
    // map pde to address of pte
    init_single_page(&page_directory_entry[vir_address/mem_4mb],(unsigned int)page_table_entry,read_write|present_mask|user_bit);
    // map pte to the physical address.
    init_single_page(&page_table_entry[0],phy_address,read_write|present_mask|user_bit);
    //flush tlb
    asm volatile(   "movl %%cr3,%%eax;"
                    "movl %%eax,%%cr3;"
                    :
                    :
                    :"%eax"
                );
}

void map_from_vir_to_phy_with_page_index(unsigned int vir_address, unsigned int phy_address,unsigned int index)
{
    // map pde to address of pte
    init_single_page(&page_directory_entry[vir_address/mem_4mb],(unsigned int)(page_table_entry),read_write|present_mask|user_bit);
    // map pte to the physical address.
    init_single_page(&page_table_entry[index],phy_address,read_write|present_mask|user_bit);
    //flush tlb
    asm volatile(   "movl %%cr3,%%eax;"
                    "movl %%eax,%%cr3;"
                    :
                    :
                    :"%eax"
                );
}
#endif
