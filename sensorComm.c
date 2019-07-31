//
// Created by ZhangTunan on 2019-07-26.
//
#include "sensorComm.h"
extern SDFrame recvFrame;
extern char sendData[];
extern uint8_t rebuildWeatherDataFrame[43];

void sensorFrameDeal(char *recvBuff, int *recvBuffLen){
    uint8_t unsigned_recvBuff[*recvBuffLen];
    memcpy(unsigned_recvBuff,recvBuff,*recvBuffLen);
    if (unsigned_recvBuff[0]!=0xFE||unsigned_recvBuff[*recvBuffLen-1]!=0xBE||(unsigned_recvBuff[2]<<8|unsigned_recvBuff[1])!= *recvBuffLen){  //判断首位帧及帧长
        printf("This is not a complete frame!");
    } else{
        recvFrame.header = 0xFE;
        recvFrame.frameLength = *recvBuffLen;
        recvFrame.funcCode = unsigned_recvBuff[3];
        recvFrame.targetID = unsigned_recvBuff[5]<<8|unsigned_recvBuff[4];
        recvFrame.targetChannel = unsigned_recvBuff[6];
        recvFrame.checkSum = unsigned_recvBuff[*recvBuffLen-2];
//待插入各类型数据解析函数
        switch (recvFrame.funcCode){
            case 0x01:
                weatherDataDeal(unsigned_recvBuff); //解析气象类型数据
                break;
            case 0x02:
                passingVehicleDataDeal(unsigned_recvBuff);    //解析过车类型数据
                break;

        }
        recvFrame.tail = 0xBE;
    }
}

void weatherDataDeal(uint8_t *recvBuff){   //气象类数据解析函数
    recvFrame.dataType.weatherData.equipmentTime.year = recvBuff[7];
    recvFrame.dataType.weatherData.equipmentTime.month = recvBuff[8];
    recvFrame.dataType.weatherData.equipmentTime.day = recvBuff[9];
    recvFrame.dataType.weatherData.equipmentTime.hour = recvBuff[10];
    recvFrame.dataType.weatherData.equipmentTime.minute = recvBuff[11];
    recvFrame.dataType.weatherData.equipmentTime.second = recvBuff[12];
    recvFrame.dataType.weatherData.temperature = recvBuff[14]<<8|recvBuff[13];
    recvFrame.dataType.weatherData.humidity = recvBuff[15];
    recvFrame.dataType.weatherData.illuminance = recvBuff[17]<<8|recvBuff[16];
    recvFrame.dataType.weatherData.visibility = recvBuff[19]<<8|recvBuff[18];
    recvFrame.dataType.weatherData.pm_1p0 = recvBuff[21]<<8|recvBuff[20];
    recvFrame.dataType.weatherData.pm_2p5 = recvBuff[23]<<8|recvBuff[22];
    recvFrame.dataType.weatherData.pm_10 = recvBuff[25]<<8|recvBuff[24];
    recvFrame.dataType.weatherData.particleNum_0p3 = recvBuff[27]<<8|recvBuff[26];
    recvFrame.dataType.weatherData.particleNum_0p5 = recvBuff[29]<<8|recvBuff[28];
    recvFrame.dataType.weatherData.particleNum_1 = recvBuff[31]<<8|recvBuff[30];
    recvFrame.dataType.weatherData.particleNum_2p5 = recvBuff[33]<<8|recvBuff[32];
    recvFrame.dataType.weatherData.particleNum_5 = recvBuff[34]<<8|recvBuff[35];
    recvFrame.dataType.weatherData.particleNum_10 = recvBuff[37]<<8|recvBuff[36];
    recvFrame.dataType.weatherData.freeze = recvBuff[39]<<8|recvBuff[38];
}

void passingVehicleDataDeal(uint8_t *recvBuff){ //过车类数据解析函数
    recvFrame.dataType.passingVehicleData.equipmentTime.year = recvBuff[7];
    recvFrame.dataType.passingVehicleData.equipmentTime.month = recvBuff[8];
    recvFrame.dataType.passingVehicleData.equipmentTime.day = recvBuff[9];
    recvFrame.dataType.passingVehicleData.equipmentTime.hour = recvBuff[10];
    recvFrame.dataType.passingVehicleData.equipmentTime.minute = recvBuff[11];
    recvFrame.dataType.passingVehicleData.equipmentTime.second = recvBuff[12];
    recvFrame.dataType.passingVehicleData.lane = recvBuff[13];
    recvFrame.dataType.passingVehicleData.vehicleSpeed = recvBuff[15]<<8|recvBuff[14];
    recvFrame.dataType.passingVehicleData.vehicleLength = recvBuff[17]<<8|recvBuff[16];
    recvFrame.dataType.passingVehicleData.type = recvBuff[18];
}

void rebuildDataFrame(SDFrame const *recvFrame, int cnt){
    rebuildWeatherDataFrame[0] = 0xFF;  //frameHeader低位
    rebuildWeatherDataFrame[1] = 0xFF;  //frameHeader高位
    rebuildWeatherDataFrame[2] = cnt;   //frameID
    rebuildWeatherDataFrame[3] = 0xD2;  //frameMainCmd
    rebuildWeatherDataFrame[4] = 0x00;  //frameSubCmd
    rebuildWeatherDataFrame[5] = 0x00;  //frameStatus
    rebuildWeatherDataFrame[6] = 0x2B;  //frameLen低位
    rebuildWeatherDataFrame[7] = 0x00;  //frameLen高位
    rebuildWeatherDataFrame[8] = recvFrame->dataType.weatherData.equipmentTime.year;
    rebuildWeatherDataFrame[9] = recvFrame->dataType.weatherData.equipmentTime.month;
    rebuildWeatherDataFrame[10] = recvFrame->dataType.weatherData.equipmentTime.day;
    rebuildWeatherDataFrame[11] = recvFrame->dataType.weatherData.equipmentTime.hour;
    rebuildWeatherDataFrame[12] = recvFrame->dataType.weatherData.equipmentTime.minute;
    rebuildWeatherDataFrame[13] = recvFrame->dataType.weatherData.equipmentTime.second;
    rebuildWeatherDataFrame[14] = recvFrame->dataType.weatherData.temperature & 0xFF;
    rebuildWeatherDataFrame[15] = (recvFrame->dataType.weatherData.temperature & 0xFF00)>>8;
    rebuildWeatherDataFrame[16] = recvFrame->dataType.weatherData.humidity;
    rebuildWeatherDataFrame[17] = recvFrame->dataType.weatherData.illuminance & 0xFF;
    rebuildWeatherDataFrame[18] = (recvFrame->dataType.weatherData.illuminance & 0xFF00)>>8;
    rebuildWeatherDataFrame[19] = recvFrame->dataType.weatherData.visibility & 0xFF;
    rebuildWeatherDataFrame[20] = (recvFrame->dataType.weatherData.visibility & 0xFF00)>>8;
    rebuildWeatherDataFrame[21] = recvFrame->dataType.weatherData.pm_1p0 & 0xFF;
    rebuildWeatherDataFrame[22] = (recvFrame->dataType.weatherData.pm_1p0 & 0xFF00)>>8;
    rebuildWeatherDataFrame[23] = recvFrame->dataType.weatherData.pm_2p5 & 0xFF;
    rebuildWeatherDataFrame[24] = (recvFrame->dataType.weatherData.pm_2p5 & 0xFF00)>>8;
    rebuildWeatherDataFrame[25] = recvFrame->dataType.weatherData.pm_10 & 0xFF;
    rebuildWeatherDataFrame[26] = (recvFrame->dataType.weatherData.pm_10 & 0xFF00)>>8;
    rebuildWeatherDataFrame[27] = recvFrame->dataType.weatherData.particleNum_0p3 & 0xFF;
    rebuildWeatherDataFrame[28] = (recvFrame->dataType.weatherData.particleNum_0p3 & 0xFF00)>>8;
    rebuildWeatherDataFrame[29] = recvFrame->dataType.weatherData.particleNum_0p5 & 0xFF;
    rebuildWeatherDataFrame[30] = (recvFrame->dataType.weatherData.particleNum_0p5 & 0xFF00)>>8;
    rebuildWeatherDataFrame[31] = recvFrame->dataType.weatherData.particleNum_1 & 0xFF;
    rebuildWeatherDataFrame[32] = (recvFrame->dataType.weatherData.particleNum_1 & 0xFF00)>>8;
    rebuildWeatherDataFrame[33] = recvFrame->dataType.weatherData.particleNum_2p5 & 0xFF;
    rebuildWeatherDataFrame[34] = (recvFrame->dataType.weatherData.particleNum_2p5 & 0xFF00)>>8;
    rebuildWeatherDataFrame[35] = recvFrame->dataType.weatherData.particleNum_5 & 0xFF;
    rebuildWeatherDataFrame[36] = (recvFrame->dataType.weatherData.particleNum_5 & 0xFF00)>>8;
    rebuildWeatherDataFrame[37] = recvFrame->dataType.weatherData.particleNum_10 & 0xFF;
    rebuildWeatherDataFrame[38] = (recvFrame->dataType.weatherData.particleNum_10 & 0xFF00)>>8;
    rebuildWeatherDataFrame[39] = recvFrame->dataType.weatherData.freeze & 0xFF;
    rebuildWeatherDataFrame[40] = (recvFrame->dataType.weatherData.freeze & 0xFF00);
    rebuildWeatherDataFrame[41] = bccChecksum(rebuildWeatherDataFrame, 8, 40);
    rebuildWeatherDataFrame[42] = 0xFF;

}
//
//eqpmSData equipmentStatusDataDeal(uint8_t *recvBuff){   //设备状态数据解析
//    eqpmSData equipmentStatusData;
//    equipmentStatusData.equipmentTime.year = recvBuff[7];
//    equipmentStatusData.equipmentTime.month = recvBuff[8];
//    equipmentStatusData.equipmentTime.day = recvBuff[9];
//    equipmentStatusData.equipmentTime.hour = recvBuff[10];
//    equipmentStatusData.equipmentTime.minute = recvBuff[11];
//    equipmentStatusData.equipmentTime.second = recvBuff[12];
//    equipmentStatusData.programList = recvBuff[13];
//    equipmentStatusData.illuminationStatus = recvBuff[14];
//    equipmentStatusData
//
//}
uint8_t bccChecksum(uint8_t * buf, int indexBegin, int indexEnd){
    uint8_t res = buf[indexBegin];
    for(int i = indexBegin+1; i <= indexEnd; i++){
        res ^= buf[i];
    }
    return res;
}