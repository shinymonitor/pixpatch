#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdbool.h>

typedef struct {int fd;} RYLR_Device;

bool RYLR_init(const char* portname, RYLR_Device* device){
    device->fd=open(portname, O_RDWR|O_NOCTTY|O_SYNC);
    if (device->fd<0) {perror("open"); return false;}
    struct termios tty;
    if (tcgetattr(device->fd, &tty)!=0) {perror("tcgetattr"); close(device->fd); return false;}
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    cfmakeraw(&tty);
    tty.c_cflag|=(CLOCAL|CREAD);
    tty.c_cc[VMIN]=0;
    tty.c_cc[VTIME]=5;
    if (tcsetattr(device->fd, TCSANOW, &tty)!=0) {perror("tcsetattr"); close(device->fd); return false;}
    tcflush(device->fd, TCIOFLUSH);
    return true;
}
bool RYLR_command(RYLR_Device* device, const char* cmd){
    if (write(device->fd, cmd, strlen(cmd))==-1) {perror("write"); return false;}
    usleep(50*1000);
    return true;
}
bool RYLR_broadcast(RYLR_Device* device, char* data){
    char buffer[512];
    int len=snprintf(buffer, sizeof(buffer), "AT+SEND=0,%zu,%s\r\n", strlen(data), data);
    if (write(device->fd, buffer, len)==-1) {perror("write"); return false;}
    usleep(50*1000);
    return true;
}
bool RYLR_receive(RYLR_Device* device, char buffer[512], int* len) {
    char raw[512]={0};
    size_t total=0;
    while (total<511) {
        int n=read(device->fd, raw+total, 511-total);
        if (n>0) {total+=n; if (raw[total-1]=='\n') break;}
        else break;
    }
    if (total==0) return false;
    if (strncmp(raw, "+RCV=", 5)!=0) return false;
    char* p=raw+5;
    p=strchr(p, ',');
    if (!p) return false;
    p++;
    char* len_end=strchr(p, ',');
    if (!len_end) return false;
    int data_len=atoi(p);
    if (data_len<=0) return false;
    p = len_end+1;
    if ((size_t)data_len>=512) data_len=511;
    memcpy(buffer, p, data_len);
    buffer[data_len]='\0';
    *len=data_len;
    return true;
}
bool RYLR_response(RYLR_Device* device, char buffer[512]) {
    size_t total=0;
    while (total<511) {
        int n=read(device->fd, buffer+total, 511-total);
        if (n>0) {total+=n; if (buffer[total-1]=='\n') break;}
        else {break;}
    }
    buffer[total]='\0';
    return total>0;
}
void RYLR_close(RYLR_Device* device){
    if (device->fd>=0) close(device->fd);
}