#include <linux/pagemap.h>
#include <crypto/hash.h>
#include <crypto/md5.h>
#include <crypto/sha.h>
#include <crypto/algapi.h>
#include "dedupe.h"
#include <linux/f2fs_fs.h>

#define DEDUPE_MD_SIZE 12*1024*1024
#define DEDUPE_MD_ENTRY_COUNT DEDUPE_MD_SIZE/sizeof(struct dedupe)

int f2fs_dedupe_calc_hash(struct page *p, u8 hash[])
{
	//int i;
	int ret;
	
	struct crypto_shash *tfm = crypto_alloc_shash("md5", 0, 0);
	struct {
		struct shash_desc desc;
		char ctx[crypto_shash_descsize(tfm)];
	} sdesc;
	char *d;

	sdesc.desc.tfm = tfm;
	sdesc.desc.flags = 0;
	ret = crypto_shash_init(&sdesc.desc);
	if (ret)
		return ret;

	d = kmap(p);
	ret = crypto_shash_digest(&sdesc.desc, d, PAGE_SIZE, hash);
	kunmap(p);

	/*for(i=0;i<4;i++)
	{
		printk("%llx",be64_to_cpu(*(long long*)&hash[i*8]));
	}
	printk("\n");*/

	crypto_free_shash(tfm);
	return ret;
}

struct dedupe *f2fs_dedupe_search(u8 hash[], struct dedupe_info *dedupe_info)
{
	struct dedupe *cur;

	for(cur = dedupe_info->cur;cur != dedupe_info->dedupe_md;cur--)
	{
		if(unlikely(cur->ref&&!memcmp(hash, cur->hash, dedupe_info->digest_len)))
		{
			return cur;
		}
	}
	for(cur = dedupe_info->dedupe_md + DEDUPE_MD_ENTRY_COUNT - 1;cur != dedupe_info->cur;cur--)
	{
		if(unlikely(cur->ref&&!memcmp(hash, cur->hash, dedupe_info->digest_len)))
		{
			return cur;
		}
	}

	return NULL;
}

block_t f2fs_dedupe_search_addr(block_t addr, struct dedupe_info *dedupe_info)
{
	struct dedupe *cur;
	int search_count = 0;

	cur = (struct dedupe *)dedupe_info->cur;
	while(cur->addr)
	{
		if(unlikely(cur->ref && addr == cur->addr))
		{
			return addr;
		}
		if(unlikely(dedupe_info->cur == dedupe_info->dedupe_md))
		{
			cur = dedupe_info->dedupe_md + DEDUPE_MD_ENTRY_COUNT - 1;
		}
		else
		{
			cur--;
		}
		search_count++;
		if(search_count>DEDUPE_MD_ENTRY_COUNT)
		{
			printk("can not add f2fs dedupe md.\n");
			return 0;
		}
	}
	return 0;
}

int f2fs_dedupe_delete_addr(block_t addr, struct dedupe_info *dedupe_info)
{
	struct dedupe *cur;

	spin_lock(&dedupe_info->lock);
	if(NEW_ADDR == addr) return -1;
	for(cur = dedupe_info->cur;cur != dedupe_info->dedupe_md;cur--)
	{
		if(unlikely(cur->ref && addr == cur->addr))
		{
			cur->ref--;
			if(0 == cur->ref)
			{
				cur->addr = 0;
				return 0;
			}
			else
			{
				return cur->ref;
			}
		}
	}
	for(cur = dedupe_info->dedupe_md + DEDUPE_MD_ENTRY_COUNT - 1;cur != dedupe_info->cur;cur--)
	{
		if(unlikely(cur->ref && addr == cur->addr))
		{
			cur->ref--;
			if(0 == cur->ref)
			{
				cur->addr = 0;
				return 0;
			}
			else
			{
				return cur->ref;
			}
		}
	}
	return -1;
}


int f2fs_dedupe_add(u8 hash[], struct dedupe_info *dedupe_info, block_t addr)
{
	int ret = 0;
	int search_count = 0;
	dedupe_info->queue_len++;	
	while(dedupe_info->cur->addr)
	{
		if(likely(dedupe_info->cur != dedupe_info->dedupe_md + DEDUPE_MD_ENTRY_COUNT - 1))
		{
			dedupe_info->cur++;
		}
		else
		{
			dedupe_info->cur = dedupe_info->dedupe_md;
		}
		search_count++;
		if(search_count>DEDUPE_MD_ENTRY_COUNT)
		{
			printk("can not add f2fs dedupe md.\n");
			ret = -1;
			break;
		}
	}
	if(0 == ret)
	{	
		struct dedupe *dedupe = dedupe_info->cur;
		dedupe->addr = (u32)addr;
		dedupe->ref = 1;
		memcpy(dedupe->hash, hash, dedupe_info->digest_len);
	}
	return ret;
}

int init_dedupe_info(struct dedupe_info *dedupe_info)
{
	int ret = 0;
	dedupe_info->queue_len = 0;
	dedupe_info->digest_len = 16;
	spin_lock_init(&dedupe_info->lock);
	INIT_LIST_HEAD(&dedupe_info->queue);
	dedupe_info->dedupe_md = vmalloc(12*1024*1024);
	memset(dedupe_info->dedupe_md, 0, 12*1024*1024);
	dedupe_info->cur = dedupe_info->dedupe_md;

	return ret;
}


