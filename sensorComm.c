//
// Created by ZhangTunan on 2019-07-26.
//
#include "sensorComm.h"
extern SDFrame recvFrame;
extern unsigned char sendData[];

SDFrame *sensorFrameDeal(unsigned char *recvBuff, int recvBuffLen){
    if (recvBuff[0]!=0xFE||recvBuff[recvBuffLen-1]!=0xBE||(recvBuff[2]<<8|recvBuff[1])!= recvBuffLen){  //判断首位帧及帧长
        return NULL;
    } else{
        recvFrame.header = 0xFE;
        recvFrame.frameLength = recvBuffLen;
        recvFrame.funcCode = recvBuff[3];
        recvFrame.targetID = recvBuff[5]<<8|recvBuff[4];
        recvFrame.targetChannel = recvBuff[6];
        recvFrame.checkSum = recvBuff[recvBuffLen-2];
//待插入各类型数据解析函数
        switch (recvFrame.funcCode){
            case 0x01: recvFrame.dataType.weatherData = weatherDataDeal(recvBuff); //解析气象类型数据

        }

        recvFrame.tail = 0xBE;
        return &recvFrame;
    }
}

wthData weatherDataDeal(unsigned char *recvBuff){
    wthData weatherData;
    weatherData.equipmentTime.year = recvBuff[7];
    weatherData.equipmentTime.month = recvBuff[8];
    weatherData.equipmentTime.day = recvBuff[9];
    weatherData.equipmentTime.hour = recvBuff[10];
    weatherData.equipmentTime.minute = recvBuff[11];
    weatherData.equipmentTime.second = recvBuff[12];
    weatherData.temperature = recvBuff[14]<<8|recvBuff[13];
    weatherData.humidity = recvBuff[15];
    weatherData.illuminance = recvBuff[17]<<8|recvBuff[16];
    weatherData.visibility = recvBuff[19]<<8|recvBuff[18];
    weatherData.pm_1p0 = recvBuff[21]<<8|recvBuff[20];
    weatherData.pm_2p5 = recvBuff[23]<<8|recvBuff[22];
    weatherData.pm_10 = recvBuff[25]<<8|recvBuff[24];
    weatherData.particleNum_0p3 = recvBuff[27]<<8|recvBuff[26];
    weatherData.particleNum_0p5 = recvBuff[29]<<8|recvBuff[28];
    weatherData.particleNum_1 = recvBuff[31]<<8|recvBuff[30];
    weatherData.particleNum_2p5 = recvBuff[33]<<8|recvBuff[32];
    weatherData.particleNum_5 = recvBuff[34]<<8|recvBuff[35];
    weatherData.particleNum_10 = recvBuff[37]<<8|recvBuff[36];
    weatherData.freeze = recvBuff[39]<<8|recvBuff[38];
    return weatherData;
}

//pVData passingVehicleDataDeal(unsigned char *recvBuff){
//    pVData passing
//}