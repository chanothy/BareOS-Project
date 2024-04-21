#include <barelib.h>
#include <fs.h>

/* fs_write - Takes a file descriptor index into the 'oft', a  pointer to a  *
 *            buffer  that the  function reads data  from and the number of  *
 *            bytes to copy from the buffer to the file.                     *
 *                                                                           *
 *            'fs_write' reads data from the 'buff' and copies it into the   *
 *            file  'blocks' starting  at the 'head'.  The  function  will   *
 *            allocate new blocks from the block device as needed to write   *
 *            data to the file and assign them to the file's inode.          *
 *                                                                           *
 *  returns - 'fs_write' should return the number of bytes written to the    *
 *            file.                                                          */
uint32 fs_write(uint32 fd, char *buff, uint32 len)
{

  filetable_t *file = &oft[fd];
  uint32 blockSpace = MDEV_BLOCK_SIZE - (file->head % MDEV_BLOCK_SIZE);
  uint32 bytes = 0;

  while (bytes < len)  
  {
    if (blockSpace == 0)
    {
      uint32 new_block = fs_getmaskbit(0);
      if (new_block == EMPTY)
        return -1; 

      fs_setmaskbit(new_block);

      // Update the inode with the new block
      if (file->inode.blocks[file->inode.size / MDEV_BLOCK_SIZE] == EMPTY)
        file->inode.blocks[file->inode.size / MDEV_BLOCK_SIZE] = new_block;
    }

    uint32 write_size = blockSpace < len - bytes ? blockSpace : len - bytes;

    uint32 offset = file->head % MDEV_BLOCK_SIZE;
    uint32 blockIndex = file->head / MDEV_BLOCK_SIZE;

    bs_write(file->inode.blocks[blockIndex], offset, buff + bytes, write_size);

    bytes += write_size;
    file->head += write_size;
    blockSpace = MDEV_BLOCK_SIZE - (file->head % MDEV_BLOCK_SIZE);
    // file->inode.id; // test

    if (file->head > file->inode.size)
      file->inode.size = file->head;
  }

  bs_write(file->inode.id, 0, (void *)&file->inode, sizeof(inode_t));

  return bytes;
}
