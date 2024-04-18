#include <barelib.h>
#include <fs.h>

/* fs_read - Takes a file descriptor index into the 'oft', a  pointer to a  *
 *           buffer that the function writes data to and a number of bytes  *
 *           to read.                                                       *
 *                                                                          *
 *           'fs_read' reads data starting at the open file's 'head' until  *
 *           it has  copied either 'len' bytes  from the file's  blocks or  *
 *           the 'head' reaches the end of the file.                        *
 *                                                                          *
 * returns - 'fs_read' should return the number of bytes read (either 'len' *
 *           or the  number of bytes  remaining in the file,  whichever is  *
 *           smaller).                                                      */

uint32 fs_read(uint32 fd, char *buff, uint32 len)
{
  filetable_t *file_entry = &oft[fd];
  uint32 head = file_entry->head;
  inode_t *inode = &file_entry->inode;
  uint32 total_bytes_read = 0;

  uint32 block_index = head / MDEV_BLOCK_SIZE;
  uint32 block_offset = head % MDEV_BLOCK_SIZE;

  while (len > 0 && head < inode->size)
  {
    char block[MDEV_BLOCK_SIZE];
    bs_read(inode->blocks[block_index], block_offset, block, MDEV_BLOCK_SIZE - block_offset);

    uint32 bytes_to_read = (len < MDEV_BLOCK_SIZE - block_offset) ? len : (MDEV_BLOCK_SIZE - block_offset);

    for (uint32 i = 0; i < bytes_to_read; i++)
    {
      *buff++ = block[i];
      total_bytes_read++;
    }

    len -= bytes_to_read;
    head += bytes_to_read;
    block_offset = 0;
    block_index++;
  }

  file_entry->head = head;

  return total_bytes_read;
}
