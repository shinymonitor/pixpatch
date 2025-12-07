#include "../lib/libefpix.h"
#include "../lib/base85.h"
#include "rylr.c"
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
//=============================================================================================
#define MAX_CONTACTS 10
#define MAX_HASHES 100
//=============================================================================================
Identity node;
uint8_t stored_hashes[MAX_HASHES][HASH_SIZE];
int hash_count = 0;
Contact contacts[MAX_CONTACTS];
int contact_count = 0;
int ignore;
//=============================================================================================
bool hash_check_and_relay(uint8_t hash[HASH_SIZE], uint8_t packet[PACKET_SIZE]) {
    for (int i = 0; i < hash_count; i++) {if (memcmp(stored_hashes[i], hash, HASH_SIZE) == 0) return false;}
    if (hash_count < MAX_HASHES) {memcpy(stored_hashes[hash_count], hash, HASH_SIZE); hash_count=hash_count+1;}
    ignore=packet[0];
    return true;
}
bool get_contact_from_alias(uint8_t alias[ALIAS_SIZE], Contact* contact) {
    for (int i = 0; i < contact_count; i++) {if (memcmp(contacts[i].their_alias, alias, ALIAS_SIZE) == 0) {memcpy(contact, &contacts[i], sizeof(Contact)); return true;}}
    return false;
}
void get_timestamp(uint8_t timestamp[TIMESTAMP_SIZE]) {
    uint32_t now = (uint32_t)(time(NULL)-1577836800);
    memcpy(timestamp, &now, TIMESTAMP_SIZE);
}
uint32_t get_age(uint8_t recv_time[TIMESTAMP_SIZE], uint8_t send_time[TIMESTAMP_SIZE]) {
    uint64_t recv_ts, send_ts;
    memcpy(&recv_ts, recv_time, TIMESTAMP_SIZE);
    memcpy(&send_ts, send_time, TIMESTAMP_SIZE);
    if (recv_ts >= send_ts) {return (uint32_t)(recv_ts - send_ts);}
    else {return UINT32_MAX;}
}
void add_contact(Contact contacts[MAX_CONTACTS], int* contact_count, const char* their_alias_str, uint8_t kx_public_key[32], uint8_t sign_public_key[32], const char* my_alias_str) {
    if (*contact_count < 10) {
        memset(contacts[*contact_count].their_alias, 0, ALIAS_SIZE);
        memset(contacts[*contact_count].my_alias, 0, ALIAS_SIZE);
        strncpy((char*)contacts[*contact_count].their_alias, their_alias_str, ALIAS_SIZE - 1);
        strncpy((char*)contacts[*contact_count].my_alias, my_alias_str, ALIAS_SIZE - 1);
        memcpy(contacts[*contact_count].kx_public_key, kx_public_key, 32);
        memcpy(contacts[*contact_count].sign_public_key, sign_public_key, 32);
        *contact_count=*contact_count+1;
    }
}
void alias_copy(uint8_t dest[ALIAS_SIZE], const char* src) {
    memset(dest, 0, ALIAS_SIZE);
    strncpy((char*)dest, src, ALIAS_SIZE - 1);
    dest[ALIAS_SIZE - 1] = '\0';
}