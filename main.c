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
