#include <barelib.h>
#include <fs.h>

extern fsystem_t *fsd;
extern filetable_t oft[NUM_FD];

/*  Search for a filename  in the directory, if the file doesn't exist  *
 *  or it is  already  open, return  an error.   Otherwise find a free  *
 *  slot in the open file table and initialize it to the corresponding  *
 *  inode in the root directory.                                        *
 *  'head' is initialized to the start of the file.                     */
int32 fs_open(char *filename) {
  int inode_index = -1;
  for (int i = 0; i < DIR_SIZE; i++) {
    int j = 0;
    while (fsd->root_dir.entry[i].name[j] != '\0' || filename[j] != '\0') {
      if (fsd->root_dir.entry[i].name[j] != filename[j]) {
        break;
      }
      j++;
    }
    if (fsd->root_dir.entry[i].name[j] == '\0' && filename[j] == '\0') {
      inode_index = fsd->root_dir.entry[i].inode_block;
      break;
    }
  }

  if (inode_index == -1) {
    return -1;
  }

  for (int i = 0; i < NUM_FD; i++) {  
    if (oft[i].state != FSTATE_CLOSED && oft[i].direntry == inode_index) {
      return -1; 
    }
  }

  
  int slot = -1;
  for (int i = 0; i < NUM_FD; i++) {
    if (oft[i].state == FSTATE_CLOSED) {
      slot = i;
      // oft[i].state = FSTATE_OPEN;   
      break; 
    }
  }

  if (slot == -1) {
      return -1;
  }
  // inode_t file_inode;
  // bs_read(inode_index, 0, &file_inode, sizeof(inode_t));
  inode_t inode;
  bs_read(inode_index, 0, &inode, sizeof(inode_t));
  oft[slot].state = FSTATE_OPEN;
  oft[slot].direntry = inode_index;
  oft[slot].head = 0;
  oft[slot].inode = inode;
  oft[slot].inode.id = inode_index;
  
  return slot;
}
