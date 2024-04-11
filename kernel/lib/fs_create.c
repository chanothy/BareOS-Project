#include <barelib.h>
#include <fs.h>
#include <bareio.h>

extern fsystem_t* fsd;
void* memcpy(void*, const void*, int);


/*  Search for 'filename' in the root directory.  If the  *
 *  file exists,  returns an error.  Otherwise, create a  *
 *  new file  entry in the  root directory, a llocate an  *
 *  unused  block in the block  device and  assign it to  *
 *  the new file.                                         */
int32 fs_create(char* filename) {
  int entry = -1;
  // go thru all the stuff to find unused entry
  for (uint32 i = 0; i < DIR_SIZE; i++) {
    if (fsd->root_dir.entry[i].name[0] == '\0' && fsd->root_dir.entry[i].inode_block == EMPTY) {
      entry = i;
      break;
    }
  }

  if (entry == -1) {
    return -1;
  }

  // find if filename there
  for (int i = 0; i < DIR_SIZE; i++) {
    int j = 0;
    while (fsd->root_dir.entry[i].name[j] != '\0' || filename[j] != '\0') {
        if (fsd->root_dir.entry[i].name[j] != filename[j]) {
            break; 
        }
        j++;
    }

    if (fsd->root_dir.entry[i].name[j] == '\0' && filename[j] == '\0') {
        return -1; 
    }
  }

  // // gets a free block index for inode
  // int freeBlock = -1;
  // for (int i = 0; i < fsd->device.nblocks; i++) {
  //   if (fs_getmaskbit(i) == 0) {
  //     freeBlock = i;
  //     break;
  //   }
  // }

  // // set file name to 
  // fsd->root_dir.entry[entry].inode_block = freeBlock;
  // for (int32 i = 0; i < FILENAME_LEN; i++) {
  //   fsd->root_dir.entry[entry].name[i] = filename[i];
  //   if (filename[i] == '\0') {
  //       break;
  //   }
  // }
  // // memcpy(fsd->root_dir.entry[entry].name, filename, FILENAME_LEN);

  // fsd->root_dir.numentries++;

  // // create inode pointing to the freeblock
  // inode_t inode;
  // inode.id = freeBlock;
  // inode.size = 0;
  // for (int i = 0; i < INODE_BLOCKS; i++) {
  //   inode.blocks[i] = EMPTY;
  // }

  // bs_write(freeBlock, 0, &inode, sizeof(inode_t));

  // fs_setmaskbit(freeBlock);

  // bs_write(1, 0, fsd->freemask, fsd->freemasksz);
  
  return 0;
}
