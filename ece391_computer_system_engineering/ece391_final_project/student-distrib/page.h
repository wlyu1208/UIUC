/* page.h - header file for initializing structure to implement paging mechanism, also turn on paging by setting controller register 
 * set up a PDE, PTE
 * PDE = 1kb, PTE = 1kb, kernel is a single 4MB page, first 4 MB is 1000 4kB pages.
 */
#ifndef PAGE_H
#define PAGE_H
#define TABLE_SIZE 1024
#define PAGE_CAPACITY 4096
#define shiftbits 12
#define read_write 0x02
#define present_mask 0x1
#define kernel_address 0x400000
#define kernel_mask 0x83
#define video_address 0xB8
#define video_page_sizes 8
#define mem_4mb 0x400000
#define user_bit 0x04
#define large_page_size 0x80
#define uservideo1 (0xB8000+0x1000)
#define uservideo2 (0xB8000+(0x1000)*2)
#define uservideo3 (0xB8000+(0x1000)*3)
/* initialize paging mechanism */   
void init_paging();

/* initialize one page */
void init_single_page(unsigned int* entry,unsigned int address,unsigned int mask);

/* map from virtual address to physical address */
void map_from_vir_to_phy(unsigned int vir_address,unsigned int phy_address);

/* map from virtual address to physical address */
void map_from_vir_to_phy_with_4kb_size_page(unsigned int vir_address,unsigned int phy_address);

void map_from_vir_to_phy_with_page_index(unsigned int vir_address,unsigned int phy_address,unsigned int index);
#endif
