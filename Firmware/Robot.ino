//
// Robot.ino
//
//
//   Wheel 3               Wheel 0
//     +--------------------+
//     |                    |
//     |               Arm  | -->
//     |                    |
//     +--------------------+
//   Wheel 2               Wheel 1
//

#include <CAN.h>
#include <SPI.h>

#define DEBUGMODE 0
#define CAN_BUS_SPEED 1000  // 1Mbaud

void robot_begin()
{
    robot_initCAN(CAN_BUS_SPEED);

    for (int wheel = 0; wheel < 4; wheel++)
    {
        robot_initWheel(wheel);
    }

    #if DEBUGMODE
    Serial.println("@E01: Robot movement is ready.");
    #endif
}

void robot_initCAN(uint16_t baudrate)
{
    CAN.begin();
    CAN.baudConfig(baudrate);
    CAN.setMode(NORMAL);
    delay(100);
}

void robot_initWheel(uint8_t wheel)
{
    robot_velocityMode(wheel);
    robot_motorPowerOn(wheel);
    robot_motorStart(wheel);
}

byte robot_nodeAdress(uint8_t wheel)
{
    byte nodeAdress[4] = {0x01, 0x02, 0x03, 0x04};
    return nodeAdress[wheel];
}

void robot_velocityMode(uint8_t wheel)
{
    #if DEBUGMODE
    Serial.println("# Motor velocity mode");
    #endif

    byte cmd[3][8] = {
        {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:0 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:1 ENV:1 SWO:1
        {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00},  // STATUSWORD
        {0x2F, 0x60, 0x60, 0x00, 0x02, 0x00, 0x00, 0x00}   // MODE OF OPERATION
    };
    for (int i = 0; i < 3; i++)
    {
        robot_command(wheel, cmd[i]);
    }
}

void robot_motorPowerOn(uint8_t wheel)
{
    #if DEBUGMODE
    Serial.println("# Motor power on");
    #endif

    byte cmd[6][8] = {
        {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00},  // STATUSWORD
        {0x2B, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:0 FTR:0 MS3:0 MS2:0 MS1:0 ENO:0 QUS:1 ENV:1 SWO:0
        {0x2B, 0x40, 0x60, 0x00, 0x07, 0x00, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:0 FTR:0 MS3:0 MS2:0 MS1:0 ENO:0 QUS:1 ENV:1 SWO:1
        {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00},  // STATUSWORD
        {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:0 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:1 ENV:1 SWO:1
        {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00}   // STATUSWORD
    };
    for (int i = 0; i < 6; i++)
    {
        robot_command(wheel, cmd[i]);
    }
}

void robot_motorStart(uint8_t wheel)
{
    #if DEBUGMODE
    Serial.println("# Motor start");
    #endif

    byte cmd[13][8] = {
        {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00},  // STATUSWORD
        {0x40, 0x40, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:0 FTR:0 MS3:0 MS2:0 MS1:0 ENO:0 QUS:0 ENV:0 SWO:0
        {0x23, 0x46, 0x60, 0x01, 0x00, 0x00, 0x00, 0x00},  // VL VELOCITY MIN AMOUNT
        {0x23, 0x46, 0x60, 0x02, 0xA8, 0x61, 0x00, 0x00},  // VL VELOCITY MAX AMOUNT
        {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x01, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:1 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:1 ENV:1 SWO:1
        {0x2B, 0x42, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00},  // VL TARGET VELOCITY
        {0x23, 0x48, 0x60, 0x01, 0x20, 0x4E, 0x00, 0x00},  // VL ACCEL DELTA SPEED
        {0x2B, 0x48, 0x60, 0x02, 0x01, 0x00, 0x00, 0x00},  // VL ACCEL DELTA TIME
        {0x23, 0x49, 0x60, 0x01, 0x20, 0x4E, 0x00, 0x00},  // VL DECEL DELTA SPEED
        {0x2B, 0x49, 0x60, 0x02, 0x01, 0x00, 0x00, 0x00},  // VL DECEL DELTA TIME
        {0x23, 0x4A, 0x60, 0x01, 0xA0, 0x86, 0x01, 0x00},  // VL QUICK STOP DELTA SPEED
        {0x2B, 0x4A, 0x60, 0x02, 0x01, 0x00, 0x00, 0x00},  // VL QUICK STOP DELTA TIME
        {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00}   // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:0 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:1 ENV:1 SWO:1
    };
    for (int i = 0; i < 13; i++)
    {
        robot_command(wheel, cmd[i]);
    }
}

void robot_setMotorSpeed(uint8_t wheel, int16_t speed)
{
    #if DEBUGMODE
    Serial.print("# Motor speed: "); Serial.println(speed);
    #endif

    int8_t physicalDirection[4] = {-1, 1, 1, -1};
    speed *= physicalDirection[wheel];

    byte lowNibble = speed;
    byte highNibble = speed >> 8;
    byte cmd[8] = {0x2B, 0x42, 0x60, 0x00, lowNibble, highNibble, 0x00, 0x00};
    robot_command(wheel, cmd);
}

void robot_quickStop(uint8_t wheel)
{
    #if DEBUGMODE
    Serial.println("# Motor quickstop");
    #endif

    byte cmd[5][8] = {
        {0x40, 0x40, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:0 FTR:0 MS3:0 MS2:0 MS1:0 ENO:0 QUS:0 ENV:0 SWO:0
        {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x01, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:1 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:1 ENV:1 SWO:1
        {0x40, 0x40, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:0 FTR:0 MS3:0 MS2:0 MS1:0 ENO:0 QUS:0 ENV:0 SWO:0
        {0x2B, 0x40, 0x60, 0x00, 0x0B, 0x01, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:1 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:0 ENV:1 SWO:1
        {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x01, 0x00, 0x00}   // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:1 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:1 ENV:1 SWO:1
    };
    for (int i = 0; i < 5; i++)
    {
        robot_command(wheel, cmd[i]);
    }
}

void robot_command(uint8_t wheel, byte data[8])
{
    uint16_t COB = 0x600;
    uint16_t frameId = COB + robot_nodeAdress(wheel);
    CAN.load_ff_0(8, frameId, data);
    CAN.send_0();
    delay(10);

    #if DEBUGMODE
    for (int i = 0; i < 8; i++)
    {
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    #endif
}
