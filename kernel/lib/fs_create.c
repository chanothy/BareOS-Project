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
  if (fsd->root_dir.numentries >= INODE_BLOCKS) {
    return -1;
  } 

  // find if filename there WRONG SOMEHOW
  for (uint32 i = 0; i < DIR_SIZE; i++) {
    int j = 0;
    while (fsd->root_dir.entry[i].name[j] != '\0' && filename[j] != '\0') {
      if (fsd->root_dir.entry[i].name[j] != filename[j]) {
          break; 
      }
      j++;
    }

    if (fsd->root_dir.entry[i].name[j] == '\0' && filename[j] == '\0') {
      return -1; 
    }
  }


  uint32 freeBlock = EMPTY;
  for (uint32 i = 0; i < fsd->device.nblocks; i++) {
    if (fs_getmaskbit(i) == 0) {
      freeBlock = i;
      break;
    }
  }

  if (freeBlock == EMPTY) {
    return -1;
  }

  uint32 entry = -1;
  // go thru all the stuff to find unused entry
  for (uint32 i = 0; i < DIR_SIZE; i++) {
    if (fsd->root_dir.entry[i].inode_block == 16777215) {
      entry = i;
      break;
    }
  }

  if (entry == -1) {
    return -1;
  }

  fsd->root_dir.numentries++;

  // set file name to 
  fsd->root_dir.entry[entry].inode_block = freeBlock;
  int len = 0;
  while (filename[len] != '\0') {
    len++;
  }

  for (uint32 i = 0; i < len; i++) {
    fsd->root_dir.entry[entry].name[i] = filename[i];
  }
  fsd->root_dir.entry[entry].name[len] = '\0';



  // make inode to put in mem
  inode_t myInode;
  myInode.id = freeBlock;
  myInode.size = 0;
  for (int i = 0; i < INODE_BLOCKS; i++) {
    myInode.blocks[i] = EMPTY;
  }

  bs_write(freeBlock, 0, &myInode, sizeof(inode_t));

  fs_setmaskbit(freeBlock);

  bs_write(BM_BIT, 0, fsd->freemask, fsd->freemasksz);
  
  return 0;
  
}


