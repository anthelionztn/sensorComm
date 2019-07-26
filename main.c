#include "sensorComm.h"

//全局变量声明区：
unsigned char sendData[] = {0xFE, 0x2A, 0x00, 0x01, 0x7E, 0xD0, 0x41,
                            0x13, 0x07, 0x17, 0x10, 0x1D, 0x39,
                            0x7E, 0x09,
                            0x38,
                            0x06, 0x00,
                            0xC8, 0x32,
                            0x10, 0x00,
                            0x27, 0x00,
                            0x2D, 0x00,
                            0x66, 0x09,
                            0x22, 0x03,
                            0x57, 0x01,
                            0x22, 0x00,
                            0x00, 0x00,
                            0x00, 0x00,
                            0x00, 0x00,
                            0x82, 0xBE};  //伪造一帧气象数据用于测试
SDFrame recvFrame;

//--------------------------------------------------------------------------
//主函数区：
int main() {
    SDFrame *test = malloc(sizeof(SDFrame));
    memset(test,0, sizeof(SDFrame));
    test = sensorFrameDeal(sendData, sizeof(sendData));
    if (test==NULL) {
        printf("This is not a complete frame...");
    } else{
        printf("设备时间:20%d年%d月%d日 %d时%d分%d秒\n",
               test->dataType.weatherData.equipmentTime.year,
               test->dataType.weatherData.equipmentTime.month,
               test->dataType.weatherData.equipmentTime.day,
               test->dataType.weatherData.equipmentTime.hour,
               test->dataType.weatherData.equipmentTime.minute,
               test->dataType.weatherData.equipmentTime.second);
        printf("温度:%.2f\n", (float)(test->dataType.weatherData.temperature)/100);
        printf("湿度:%d%%\n",test->dataType.weatherData.humidity);
        printf("光照度:%dlux\n", test->dataType.weatherData.illuminance);
        printf("能见度:%dm\n", test->dataType.weatherData.visibility);
        printf("PM1.0:%d\n", test->dataType.weatherData.pm_1p0);
        printf("PM2.5:%d\n", test->dataType.weatherData.pm_2p5);
        printf("PM10:%d\n", test->dataType.weatherData.pm_10);
        printf(">0.3um颗粒数:%d\n", test->dataType.weatherData.particleNum_0p3);
        printf(">0.5um颗粒数:%d\n", test->dataType.weatherData.particleNum_0p5);
        printf(">1.0um颗粒数:%d\n", test->dataType.weatherData.particleNum_1);
        printf(">2.5um颗粒数:%d\n", test->dataType.weatherData.particleNum_2p5);
        printf(">5.0um颗粒数:%d\n", test->dataType.weatherData.particleNum_5);
        printf(">10.0um颗粒数:%d\n", test->dataType.weatherData.particleNum_10);
    }

    return 0;
}
//
//SDFrame *sensorFrameDeal(unsigned char *recvBuff, int recvBuffLen){
//    if (recvBuff[0]!=0xFE||recvBuff[recvBuffLen-1]!=0xBE||(recvBuff[2]<<8|recvBuff[1])!= recvBuffLen){  //判断首位帧及帧长
//        return NULL;
//    } else{
//        recvFrame.header = 0xFE;
//        recvFrame.frameLength = recvBuffLen;
//        recvFrame.funcCode = recvBuff[3];
//        recvFrame.targetID = recvBuff[5]<<8|recvBuff[4];
//        recvFrame.targetChannel = recvBuff[6];
//        recvFrame.checkSum = recvBuff[recvBuffLen-2];
////待插入各类型数据解析函数
//        switch (recvFrame.funcCode){
//            case 0x01: recvFrame.dataType.weatherData = weatherDataDeal(recvBuff); //解析气象类型数据
//
//        }
//
//        recvFrame.tail = 0xBE;
//        return &recvFrame;
//    }
//}
//
//wthData weatherDataDeal(unsigned char *recvBuff){
//    wthData weatherData;
//    weatherData.equipmentTime.year = recvBuff[7];
//    weatherData.equipmentTime.month = recvBuff[8];
//    weatherData.equipmentTime.day = recvBuff[9];
//    weatherData.equipmentTime.hour = recvBuff[10];
//    weatherData.equipmentTime.minute = recvBuff[11];
//    weatherData.equipmentTime.second = recvBuff[12];
//    weatherData.temperature = recvBuff[14]<<8|recvBuff[13];
//    weatherData.humidity = recvBuff[15];
//    weatherData.illuminance = recvBuff[17]<<8|recvBuff[16];
//    weatherData.visibility = recvBuff[19]<<8|recvBuff[18];
//    weatherData.pm_1p0 = recvBuff[21]<<8|recvBuff[20];
//    weatherData.pm_2p5 = recvBuff[23]<<8|recvBuff[22];
//    weatherData.pm_10 = recvBuff[25]<<8|recvBuff[24];
//    weatherData.particleNum_0p3 = recvBuff[27]<<8|recvBuff[26];
//    weatherData.particleNum_0p5 = recvBuff[29]<<8|recvBuff[28];
//    weatherData.particleNum_1 = recvBuff[31]<<8|recvBuff[30];
//    weatherData.particleNum_2p5 = recvBuff[33]<<8|recvBuff[32];
//    weatherData.particleNum_5 = recvBuff[34]<<8|recvBuff[35];
//    weatherData.particleNum_10 = recvBuff[37]<<8|recvBuff[36];
//    weatherData.freeze = recvBuff[39]<<8|recvBuff[38];
//    return weatherData;
//}
//
////pVData passingVehicleDataDeal(unsigned char *recvBuff){
////    pVData passing
////}