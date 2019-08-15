#ifndef _UTF8TOGBK_H_
#define _UTF8TOGBK_H_

int switch_to_gbk(const unsigned char* pszBufIn, int nBufInLen, 
                  unsigned char* pszBufOut, int* pnBufOutLen);
int is_ascii_encode_wpa_ssid_txt(const unsigned char *ssid, int ssid_len);
int is_str_utf8(const unsigned char* str);

#endif


