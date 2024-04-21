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
  filetable_t *file = &oft[fd];
  uint32 head = file->head;
  inode_t *inode = &file->inode;
  uint32 bytesRead = 0;

  uint32 blockIndex = head / MDEV_BLOCK_SIZE;
  uint32 offset = head % MDEV_BLOCK_SIZE;

  while (len > 0)
  {
    char block[MDEV_BLOCK_SIZE];
    bs_read(inode->blocks[blockIndex], offset, block, MDEV_BLOCK_SIZE - offset);

    uint32 bytes = (len < MDEV_BLOCK_SIZE - offset) ? len : (MDEV_BLOCK_SIZE - offset);

    for (uint32 i = 0; i < bytes; i++)
    {
      *buff++ = block[i];
      bytesRead++;
    }

    len -= bytes;
    head += bytes;
    offset = 0;
    blockIndex++;
  }

  file->head = head;

  return bytesRead;
}
