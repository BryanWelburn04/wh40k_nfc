#pragma once


void DES (BYTE *Data, BYTE *Key, int bEncrypt);
void DES3 (BYTE *Data, BYTE *Key, int bEncrypt);
void DES2 (BYTE *Data, BYTE *Key, int bDecrypt);
void DES6 (BYTE *Data, BYTE *Key, int bDecrypt);
