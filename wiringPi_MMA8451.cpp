// gcc -Wall -o wpm source_file -lwiringPi
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include "wiringPi_MMA8451.h"

int fd;
uint8_t reg8;


// join bits of XYZ_MSB and XYZ_LSB, using read8(reg)/getRange(void)
float join(uint8_t msb_reg, uint8_t lsb_reg) {
    int16_t xyz;
    uint8_t range = getRange();
    uint16_t divider = 1;

    if(range == MMA8451_RANGE_8_G) divider = 1024;
    if(range == MMA8451_RANGE_4_G) divider = 2048;
    if(range == MMA8451_RANGE_2_G) divider = 4096; 
    
    xyz = read8(msb_reg); xyz <<= 8; xyz |= read8(lsb_reg); xyz >>= 2;
    return (float)xyz/divider;
}

// get updated g from sensors using join(reg, reg)
void update_3_g(float *x, float *y, float *z) {
    *x = join(MMA8451_REG_OUT_X_MSB,MMA8451_REG_OUT_X_LSB);
    *y = join(MMA8451_REG_OUT_Y_MSB,MMA8451_REG_OUT_Y_LSB);
    *z = join(MMA8451_REG_OUT_Z_MSB,MMA8451_REG_OUT_Z_LSB);
    return;
}

// setting up using read8/write8 
bool setup(int32_t devId) {
    fd = wiringPiI2CSetup(devId);
    
    // reset
    write8(MMA8451_REG_CTRL_REG2, 0x40); 
    usleep(50);
    // enable 4G range
    write8(MMA8451_REG_XYZ_DATA_CFG, MMA8451_RANGE_4_G);
    usleep(50);
    // High res
    write8(MMA8451_REG_CTRL_REG2, 0x12);
    usleep(50);
    // Low noise @100Hz output rate, where 800Hz is the max
    write8(MMA8451_REG_CTRL_REG1, (MMA8451_DATARATE_100_HZ | MMA8451_L_NS));
    usleep(50);
    // Activate! 
    write8(MMA8451_REG_CTRL_REG1, (read8(MMA8451_REG_CTRL_REG1) | 0x01));
    usleep(50);

    uint8_t deviceid = read8(MMA8451_REG_WHOAMI);
    if(deviceid != MMA8451_DEVICE_ID) {
        return false;
    }
    return true;
}


int main() {
    // declare the gs 
    float x_g, y_g, z_g;
    // setting up
    if(setup(MMA8451_DEFAULT_ADDRESS)) {
        printf("MMA8451 activated! \n");
    }
    else {
        printf("Error Exit. \n");
        return -1;
    }
    // debugging 
    // printf("fd: %6hd \n", fd);
    // foread(); 

    // update the gs
    while(true) {
        update_3_g(&x_g, &y_g, &z_g);
        printf("g of x: %6.4lf y: %6.4lf z: %6.4lf\n", x_g, y_g, z_g);
        usleep(50000);
    }
    return 0;
    
}


// basic i2c read/write
uint8_t read8(uint8_t reg) {
    return wiringPiI2CReadReg8(fd,reg);
}

void write8(uint8_t reg, uint8_t sig) {
    wiringPiI2CWriteReg8(fd,reg,sig);
}

// find out the range as the last three bits of MMA8451_REG_XYZ_DATA_CFG
mma8451_range_t getRange(void) {
    return (mma8451_range_t)(read8(MMA8451_REG_XYZ_DATA_CFG) & 0x03);
}

// debugging function
const char *byte_to_binary(int x) {
    static char b[9];
    b[0] = '\0';
    int z;
    for (z = 128; z > 0; z >>= 1) {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
    return b;
}

// debugging function 
void foread() {
    for(uint8_t i=0x00; i < 0x2E; i++) {
        reg8=read8(i);
        printf("%04x list: %6hd 0x%.2X %s\n", i, reg8,reg8,byte_to_binary(reg8));
    }
}

