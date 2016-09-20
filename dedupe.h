#ifndef _DEDUPE_H
#define _DEDUPE_H

//#include "f2fs.h"


typedef u32 block_t;

struct dedupe
{
	block_t addr;
	int ref;
	u8 hash[16];
};

/*struct dedupe
{
	u8 hash[32];
	struct list_head list;
	block_t addr;
	int ref;
};*/

struct dedupe_info
{
	int queue_len;
	int digest_len;
	struct dedupe* dedupe_md;
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

#endif

