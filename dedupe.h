#ifndef _DEDUPE_H
#define _DEDUPE_H

#define DEDUPE_PER_BLOCK (PAGE_CACHE_SIZE/sizeof(struct dedupe)

typedef u32 block_t;

struct dedupe
{
	block_t addr;
	int ref;
	u8 hash[16];
};

struct dedupe_info
{
	int queue_len;
	int digest_len;
	struct dedupe* dedupe_md;
	unsigned long dedupe_md_dirty_bmp[8];
	struct dedupe* cur;
	struct list_head queue;
	spinlock_t lock;
};


extern int f2fs_dedupe_calc_hash(struct page *p, u8 hash[]);
extern struct dedupe *f2fs_dedupe_search(u8 hash[], struct dedupe_info *dedupe_info);
extern block_t f2fs_dedupe_search_addr(block_t addr, struct dedupe_info *dedupe_info);
extern int f2fs_dedupe_add(u8 hash[], struct dedupe_info *dedupe_info, block_t addr);
extern int init_dedupe_info(struct dedupe_info *dedupe_info);
extern int f2fs_dedupe_delete_addr(block_t addr, struct dedupe_info *dedupe_info);
extern void set_dedupe_dirty(struct dedupe_info *dedupe_info, struct dedupe *dedupe);

#endif

