#include "sensorComm.h"

//全局变量声明区：
char sendData[] = {0xFE, 0x2A, 0x00, 0x01, 0x7E, 0xD0, 0x41,
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
uint8_t rebuildWeatherDataFrame[43];
SDFrame recvFrame;

//--------------------------------------------------------------------------
//主函数区：
int main() {
    int temp = sizeof(sendData);
    int *sendDataLen = &temp;
    sensorFrameDeal(sendData, sendDataLen);    //帧数据解析回调函数
    rebuildDataFrame(&recvFrame,1);
    for(int i = 0; i < 43; i++){
        printf("%02X ", rebuildWeatherDataFrame[i]);
    }
    printf("\n");
    printf("设备时间:20%d年%d月%d日 %d时%d分%d秒\n",
            recvFrame.dataType.weatherData.equipmentTime.year,
            recvFrame.dataType.weatherData.equipmentTime.month,
            recvFrame.dataType.weatherData.equipmentTime.day,
            recvFrame.dataType.weatherData.equipmentTime.hour,
            recvFrame.dataType.weatherData.equipmentTime.minute,
            recvFrame.dataType.weatherData.equipmentTime.second);
    printf("温度:%.2f\n", (float)(recvFrame.dataType.weatherData.temperature)/100);
    printf("湿度:%d%%\n",recvFrame.dataType.weatherData.humidity);
    printf("光照度:%dlux\n", recvFrame.dataType.weatherData.illuminance);
    printf("能见度:%dm\n", recvFrame.dataType.weatherData.visibility);
    printf("PM1.0:%d\n", recvFrame.dataType.weatherData.pm_1p0);
    printf("PM2.5:%d\n", recvFrame.dataType.weatherData.pm_2p5);
    printf("PM10:%d\n", recvFrame.dataType.weatherData.pm_10);
    printf(">0.3um颗粒数:%d\n", recvFrame.dataType.weatherData.particleNum_0p3);
    printf(">0.5um颗粒数:%d\n", recvFrame.dataType.weatherData.particleNum_0p5);
    printf(">1.0um颗粒数:%d\n", recvFrame.dataType.weatherData.particleNum_1);
    printf(">2.5um颗粒数:%d\n", recvFrame.dataType.weatherData.particleNum_2p5);
    printf(">5.0um颗粒数:%d\n", recvFrame.dataType.weatherData.particleNum_5);
    printf(">10.0um颗粒数:%d\n", recvFrame.dataType.weatherData.particleNum_10);

    return 0;
}
