#include <barelib.h>
#include <fs.h>
#include <bareio.h>

extern fsystem_t *fsd;
extern filetable_t oft[NUM_FD];

/*  Search for a filename  in the directory, if the file doesn't exist  *
 *  or it is  already  open, return  an error.   Otherwise find a free  *
 *  slot in the open file table and initialize it to the corresponding  *
 *  inode in the root directory.                                        *
 *  'head' is initialized to the start of the file.                     */
int32 fs_open(char* filename) {
  directory_t dir = fsd->root_dir;
  for (int i = 0; i < NUM_FD; i++) {
    if (oft[i].state == FSTATE_OPEN) {
      int j = 0;
      while (dir.entry[i].name[j] != '\0' && filename[j] != '\0' && dir.entry[i].name[j] == filename[j]) {
        j++;
      }
      if (dir.entry[i].name[j] == filename[j]) {
        return -1;
      }
    }
  }


  int slot = -1;
  for (int i = 0; i < NUM_FD; i++) {
    if (oft[i].state == FSTATE_CLOSED) {
      slot = i;
      break;
    }
  }
  
  if (slot == -1) {
    return -1;
  }

  for (int i = 0; i < dir.numentries; i++) {
    int j = 0;
    while (dir.entry[i].name[j] != '\0' && filename[j] != '\0' && dir.entry[i].name[j] == filename[j]) {
      j++;
    }
    if (dir.entry[i].name[j] == filename[j]) {
      bs_read(dir.entry[i].inode_block, 0, &(oft[slot].inode), sizeof(inode_t));
      oft[slot].state = FSTATE_OPEN;
      oft[slot].head = 0;
      oft[slot].direntry = i;
      return slot;
    }
  }

  
  return -1;
}

