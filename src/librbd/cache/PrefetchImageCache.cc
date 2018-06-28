// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
//testing this is Tommy writing a simple comment
#include "PrefetchImageCache.h"
#include "include/buffer.h"
#include "common/dout.h"
#include "librbd/ImageCtx.h"

#define dout_subsys ceph_subsys_rbd
#undef dout_prefix
#define dout_prefix *_dout << "librbd::PrefetchImageCache: " << this << " " \
                           <<  __func__ << ": "

namespace librbd {
namespace cache {

template <typename I>
PrefetchImageCache<I>::PrefetchImageCache(ImageCtx &image_ctx)
  : m_image_ctx(image_ctx), m_image_writeback(image_ctx) {
  CephContext *cct = m_image_ctx.cct;
  ldout(cct, 10) << "lru_queue=" << lru_queue << ", cache_entries=" << cache_entries << dendl;
}


/*  what PrefetchImageCache<I>::aio_read should do!! :
 *  get called (by ImageReadRequest::send_image_cache_request())
 *  get the image exents, and loop through, for each extent: split?? into 
 *    either each extent that comes in is split into its own sub list of offsets of CACHE_CHUNK_SIZE chunks, then these are all checked from the cache, or
 *    maybe, the whole thing gets split into a new image_extents that's just all the incoming extents split into chunks in one big list, then that gets checked?
 *    kinda doesn't matter
 *  then we dispatch the async eviction list updater
 *  but in the end, we have a lot of chunks, and for each one we check the cache - first lock it, then
 *  we use the count(key) method and see if that offset is in the cache, 
 *    if so we get that value which is a bufferlist*
 *    then we copy that back into the read bufferlist
 *  if it's not in the cache, 
 *    do that read from m_image_writeback, that should put it into the read bufferlist (???) 
 *    then can we also get it and put it in the cache??? somehow this needs to happen
 *    
 *  
 *  
 *  
 *  
*/  

template <typename I>
void PrefetchImageCache<I>::aio_read(Extents &&image_extents, bufferlist *bl,
                                        int fadvise_flags, Context *on_finish) {
  CephContext *cct = m_image_ctx.cct;
  ldout(cct, 20) << "image_extents=" << image_extents << ", "
                 << "on_finish=" << on_finish << dendl;

	//get the extents, then call the splitting/chunking function from @Leo's code

	//begin read from cache
	std::unordered_map<uint64_t, ceph::bufferlist *>::iterator it = imageCacheEntry->begin();

	ImageCacheEntries temp; 
	//checks to see if cache is empty
	//if it is, read chunks,	//else read from cluster
	if(!(imageCacheEntry->empty()){
	//iterate over the hash table
	for(auto it: *imageCacheEntry){

		//right now, I have it set so that whatever chunk data we get, it is stored in a temp hash table with the same type as the cache type.
		//i don't know how to make this into a "reassembly" buffer...
		/****************** PROBABLY USING the @param bufferlist *bl? **********************/
				temp.insert(std::make_pair(it.first, it.second));
			 }
		}
	//else read from cluster
	else{
  // writeback's aio_read method used for reading from cluster
		m_image_writeback.aio_read(std::move(image_extents), bl, fadvise_flags,            //do we assume that it's already in the (read) bufferlist 
	
	//call chunking/splitting function again from @Leo's code
	
	}


}

template <typename I>
void PrefetchImageCache<I>::aio_write(Extents &&image_extents,
                                         bufferlist&& bl,
                                         int fadvise_flags,
                                         Context *on_finish) {
  CephContext *cct = m_image_ctx.cct;
  ldout(cct, 20) << "image_extents=" << image_extents << ", "
                 << "on_finish=" << on_finish << dendl;

  m_image_writeback.aio_write(std::move(image_extents), std::move(bl),
                              fadvise_flags, on_finish);
}

template <typename I>
void PrefetchImageCache<I>::aio_discard(uint64_t offset, uint64_t length,
                                           bool skip_partial_discard, Context *on_finish) {
  CephContext *cct = m_image_ctx.cct;
  ldout(cct, 20) << "offset=" << offset << ", "
                 << "length=" << length << ", "
                 << "on_finish=" << on_finish << dendl;

  m_image_writeback.aio_discard(offset, length, skip_partial_discard, on_finish);
}

template <typename I>
void PrefetchImageCache<I>::aio_flush(Context *on_finish) {
  CephContext *cct = m_image_ctx.cct;
  ldout(cct, 20) << "on_finish=" << on_finish << dendl;

  m_image_writeback.aio_flush(on_finish);
}

template <typename I>
void PrefetchImageCache<I>::aio_writesame(uint64_t offset, uint64_t length,
                                             bufferlist&& bl, int fadvise_flags,
                                             Context *on_finish) {
  CephContext *cct = m_image_ctx.cct;
  ldout(cct, 20) << "offset=" << offset << ", "
                 << "length=" << length << ", "
                 << "data_len=" << bl.length() << ", "
                 << "on_finish=" << on_finish << dendl;

  m_image_writeback.aio_writesame(offset, length, std::move(bl), fadvise_flags,
                                  on_finish);
}

template <typename I>
void PrefetchImageCache<I>::aio_compare_and_write(Extents &&image_extents,
                                                     bufferlist&& cmp_bl,
                                                     bufferlist&& bl,
                                                     uint64_t *mismatch_offset,
                                                     int fadvise_flags,
                                                     Context *on_finish) {
  CephContext *cct = m_image_ctx.cct;
  ldout(cct, 20) << "image_extents=" << image_extents << ", "
                 << "on_finish=" << on_finish << dendl;

  m_image_writeback.aio_compare_and_write(
    std::move(image_extents), std::move(cmp_bl), std::move(bl), mismatch_offset,
    fadvise_flags, on_finish);
}

template <typename I>
void PrefetchImageCache<I>::init(Context *on_finish) {
  CephContext *cct = m_image_ctx.cct;    //for logging purposes
  ldout(cct, 20) << dendl;

  //iterates through the bufferlist -- though it might not be needed
  ceph::bufferlist::const_iterator me = begin();
      while (!me.end()) {
	++me;
      }

      //just wants to see the result of the bufferlist
  for (auto const &pair: ImageCacheEntries)
	  std::cout << "{" << pair.first << " -> " << pair.second << "}\n";

  ceph::bufferlist * bl;
  uint64_t be;

  //begin initializing LRU and hash table.
  LRUQueue *lru_q = new LRUQueue();
  ImageCacheEntries *imageCacheEntry = new ImageCacheEntries();
  
  

  //ImageCacheEntry.insert(std::make_pair<uint64_t, ceph::bufferlist *>(be, bl));

  // //arbitrary size 26, could be any size
  // deque<char> deque1(26, '0');

  // deque<char>::iterator i;

  // //populates the deque with 26 elements. 
  //   for (i = deque1.begin(); i != deque1.end(); ++i)
  //   cout << *i << endl;

  //don't know how to implement on_finish
  on_finish->complete(0);

	// init() called where? which context to use for on_finish?
	// in other implementations, init() is called in OpenRequest, totally unrelated
	// to constructor, which makes sense - image context is 'owned' by program
	// using librbd, so lifecycle is not quite the same as the actual open image connection

	// see librbd/Utils.h line 132 for create_context_callback which seems to be the kind of context that's needed for on_finish
}
	
	
template <typename I>
void PrefetchImageCache<I>::shut_down(Context *on_finish) {
  CephContext *cct = m_image_ctx.cct;
  ldout(cct, 20) << dendl;

	//erases the content of the LRU queue
	//since the content are ints, there's no need for deallocation
	//by using the erase-remove idiom
	lru_q -> erase(std::remove_if(lru_q->begin(), lru_q->end(), true), lru_q->end());

	//calls the destructor which therefore destroys the object, not just only the reference to the object. 
	lru_q -> clear();

	
	/* erases the content of the hash table */

	//pointer to a hash table
	ImageCacheEntries *cache_entries;

 	// the hash table container, along with the objects in it
	delete cache_entries;
						
	}
	
	
template <typename I>
void PrefetchImageCache<I>::invalidate(Context *on_finish) {
  CephContext *cct = m_image_ctx.cct;
  ldout(cct, 20) << dendl;

  // dump cache contents (don't have anything)
  on_finish->complete(0);
}

template <typename I>
void PrefetchImageCache<I>::flush(Context *on_finish) {
  CephContext *cct = m_image_ctx.cct;
  ldout(cct, 20) << dendl;

  // internal flush -- nothing to writeback but make sure
  // in-flight IO is flushed
  aio_flush(on_finish);
}

} // namespace cache
} // namespace librbd

template class librbd::cache::PrefetchImageCache<librbd::ImageCtx>;
