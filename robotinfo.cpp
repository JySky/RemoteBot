#include "robotinfo.h"

RobotInfo::RobotInfo()
{

}

void RobotInfo::setSpeedFront(int SpeedFront)
{
    this->SpeedFront=SpeedFront;
}

void RobotInfo::setBatLevel(char BatLevel)
{
    this->BatLevel=BatLevel;
}

void RobotInfo::setIR(char IR)
{
    this->IR=IR;
}

void RobotInfo::setIR2(char IR2)
{
    this->IR2=IR2;
}

void RobotInfo::setodometry(long odometry)
{
    this->odometry=odometry;
}

void RobotInfo::setCurrent(char Current)
{
    this->Current=Current;
}

void RobotInfo::setVersion(char Version)
{
    this->Version=Version;
}
