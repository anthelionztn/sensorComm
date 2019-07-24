#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned char byte;

//数据结构声明区：
//enum DateTypeLengh{ //枚举不同类型消息帧的数据字节数
//    weather = 33,
//    passingVehicle = 12,
//    equipmentStatus = 44,
//    warning04 = 9,
//    warning05 = 7,
//    warning06 = 8,
//    warning07 = 9,
//    warning08 = 7,
//    paraCtrlA0 = 9,
//    paraCtrlA1 = 7
//};
typedef struct equipmentTime{   //设备时间（年月日时分秒）
    byte year;
    byte month;
    byte day;
    byte hour;
    byte minute;
    byte second;
}eqTime;
typedef struct weatherData{ //对应功能码：0x01
    eqTime equipmentTime;
    unsigned short temperature;    //温度（0.01摄氏度）
    byte humidity;  //湿度（%）
    unsigned short illuminance;    //光照度（Lux）
    unsigned short visibility; //能见度（m）
    unsigned short pm_1p0; //PM1.0（ug/m3）
    unsigned short pm_2p5; //PM2.5（ug/m3）
    unsigned short pm_10;  //PM10（ug/m3）
    unsigned short particleNum_0p3;    //颗粒数>0.3um
    unsigned short particleNum_0p5;    //颗粒数>0.5um
    unsigned short particleNum_1;  //颗粒数>1um
    unsigned short particleNum_2p5;    //颗粒数>2.5um
    unsigned short particleNum_5;  //颗粒数>5um
    unsigned short particleNum_10; //颗粒数>10um
    unsigned short freeze; //路面结冰，保留未用
}wthData;

typedef struct passingVehicleData{ //对应功能码：0x02
    eqTime equipmentTime;
    byte lane;  //车道
    unsigned short vehicleSpeed;  //车速
    unsigned short vehicleLength;  //车长
    byte type;  //保留未用
}pVData;

typedef struct equipmentStatusData{ //对应功能码：0x03
    byte equipmentTime[6];  //设备时间（年月日时分秒）
    byte programList;   //节目单
    byte illuminationStatus;    //照明状态（0：关闭，1：开启）
    byte latitude[8];  //纬度
    byte longitude[8];  //经度
    byte chargeType;    //充电类型（1：市电，2：太阳能）
    byte chargeStatus;  //充电状态（0：未充电，1：充电中）
    byte chargeVoltage[2];  //充电电压（mV）
    byte battaryVoltage[2]; //电池电压（mV）
    byte chargeCurrent[2];  //充电电流（mA）
    byte workingTemperature[2]; //工作温度（0.01摄氏度）
    byte radarHardwareVersion;   //雷达硬件版本
    byte radarSoftwareVersion;  //雷达软件版本
    byte equipmentDipAngle[2];  //设备倾角（有符号）
    byte runningTime[4];    //运行时间（秒）
    byte cBoardHardwareVersion; //主控板硬件版本
    byte cBoardSoftwareVersion; //主控板软件版本
}eqpmSData;

typedef struct warning04Data{   //对应功能码：0x04 应急车道占用预警
    byte incidentType;  //事件类型（1：告警，0：解除）
    byte time[6];  //时间（年月日时分秒）
    byte distance[2];   //距离（米）
}w04Data;

typedef struct warning05Data{   //对应功能码：0x05 现场报警
    byte incidentType;  //事件类型（1：告警，0：解除）
    byte time[6];  //时间（年月日时分秒）
}w05Data;

typedef struct warning06Data{   //对应功能码：0x06 设备状态
    byte incidentType;  //事件类型（1：告警，0：解除）
    byte time[6];  //时间（年月日时分秒）
    byte warningType;   //告警类型（1：被撞，2：移动，3：倾倒）
}w06Data;

typedef struct warning07Data{   //对应功能码：0x07 低电压缺点告警
    byte incidentType;  //事件类型（1：告警，0：解除）
    byte time[6];  //时间（年月日时分秒）
    byte voltage[2];    //电压
}w07Data;

typedef struct warning08Data{   //对应功能码：0x08 市电掉电告警
    byte incidentType;  //事件类型（1：告警，0：解除）
    byte time[6];  //时间（年月日时分秒）
}w08Data;

typedef struct paraCtrlA0Data{//对应功能码：0xA0 节目单控制
    byte programListID; //节目单编号
    byte lightLuminance[2]; //灯板亮度（1-65535）
    byte blinkDuty[2];  //闪烁占空比（0/1/2）
    byte blinkFrequency[2]; //闪烁频率（0/1/2/3）
    byte wakeDelay[2];  //尾迹延时（100~65535毫秒）
}pCA0Data;

typedef struct paraCtrlA1Data{//对应功能码：0xA1 路灯照明控制
    byte ctrlType;  //控制方式（1：打开，2：关闭，3：自动）
    byte lightLuminance[2]; //路灯亮度（1-100）
    byte turnOnThreshold[2];    //自动控制时低于此值开路灯（1-65535）
    byte duringTime[2]; //持续时间（自动控制点亮延时，单位秒）
}pCA1Data;


typedef struct SensorDataFrame{
    byte header;    //帧头固定为0xFE
    unsigned short frameLength;  //帧长,2byte,低位在前，高位在后
    byte funcCode;  //功能码
    unsigned short targetID; //目标ID,2byte,低位在前，高位在后
    byte targetChannel; //目标信道
    union data{ //不同类型消息拥有不同的字节长度
        wthData weatherData;    //气象数据
        pVData passingVehicleData;  //过车数据
        eqpmSData equipmentStatusData;  //设备状态数据
        w04Data warning04Data;  //告警事件——应急车道占用告警
        w05Data warning05Data;  //告警事件——现场报警
        w06Data warning06Data;  //告警事件——设备状态
        w07Data warning07Data;  //告警事件——低电压缺电告警
        w08Data warning08Data;  //告警事件——市电掉电告警
        pCA0Data paraCtrlA0Data;    //参数控制——节目单控制
        pCA1Data paraCtrlA1Data;    //参数控制——路灯照明控制
    }dataType;
    byte checkSum;  //校验和
    byte tail;  //帧尾固定为0xBE
}SDFrame;

//--------------------------------------------------------------------------
//函数声明区：
SDFrame *sensorFrameDeal(unsigned char *recvBuff, int recvBuffLen);   //传感器数据解析函数
wthData weatherDataDeal(unsigned char *recvBuff);   //气象类数据解析函数
//unsigned char* byteReverse(unsigned char* c, int strLen);


//--------------------------------------------------------------------------
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
//函数区：
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

pVData passingVehicleDataDeal(unsigned char *recvBuff){
    pVData passing
}