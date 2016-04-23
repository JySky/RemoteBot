#ifndef ROBOTINFO_H
#define ROBOTINFO_H


class RobotInfo
{
    private:
        int SpeedFront;
        char BatLevel;
        char IR;
        char IR2;
        long odometry;
        char Current;
        char Version;
    public:
        RobotInfo();
        int getSpeedFront(){return SpeedFront;}
        char getBatLevel(){return BatLevel;}
        char getIR(){return IR;}
        char getIR2(){return IR2;}
        long getodometry(){return odometry;}
        char getCurrent(){return Current;}
        char getVersion(){return Version;}
        void setSpeedFront(int SpeedFront);
        void setBatLevel(char BatLevel);
        void setIR(char IR);
        void setIR2(char IR2);
        void setodometry(long odometry);
        void setCurrent(char Current);
        void setVersion(char Version);
};

#endif // ROBOTINFO_H
