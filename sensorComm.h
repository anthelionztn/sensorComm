//
// Created by ZhangTunan on 2019-07-26.
//

#ifndef SENSORCOMM_SENSORCOMM_H
#define SENSORCOMM_SENSORCOMM_H

#endif //SENSORCOMM_SENSORCOMM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned char byte;

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


