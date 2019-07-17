
#include "dataBuf.h"
#include <string.h>
#include <stdio.h>

int main() {
  char buf[256] = {0};
  char read_buf[256] = {0};

  DataBufHandle handle =  DataBufferCreate(1024);

  printf("------------------------case 1: write------------------------------\n");
  snprintf(buf, sizeof(buf), "hello world");
  DataBufferWrite(handle, buf, strlen(buf) + 1);
  printf("[%s:%d] free=%d, data=%d\n", __FUNCTION__, __LINE__, DataBufferGetFreeSize(handle), DataBufferGetDataSize(handle));
  snprintf(buf, sizeof(buf), "xiaoC hello");
  DataBufferWrite(handle, buf, strlen(buf) + 1);
  printf("[%s:%d] free=%d, data=%d\n", __FUNCTION__, __LINE__, DataBufferGetFreeSize(handle), DataBufferGetDataSize(handle));

  printf("------------------------case 2: peak------------------------------\n");
  DataBufferPeek(read_buf, 14, handle);
  printf("[%s:%d] peak_buf:%s\n", __FUNCTION__, __LINE__, read_buf);
  printf("[%s:%d] free=%d, data=%d\n", __FUNCTION__, __LINE__, DataBufferGetFreeSize(handle), DataBufferGetDataSize(handle));
  
  printf("------------------------case 3: read------------------------------\n");
  DataBufferRead(read_buf, strlen(buf) + 1, handle);
  printf("[%s:%d] read_buf: %s\n", __FUNCTION__, __LINE__, read_buf);
  printf("[%s:%d] free=%d, data=%d\n", __FUNCTION__, __LINE__, DataBufferGetFreeSize(handle), DataBufferGetDataSize(handle));
  DataBufferRead(read_buf, strlen(buf) + 1, handle);
  printf("[%s:%d] free=%d, data=%d\n", __FUNCTION__, __LINE__, DataBufferGetFreeSize(handle), DataBufferGetDataSize(handle));

  DataBufferDestroy(handle);
  return 0;
}
