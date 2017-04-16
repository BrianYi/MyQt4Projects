#pragma once
class PlotterSetting
{
public:
    PlotterSetting(void);
    ~PlotterSetting(void);
    double spanX() const { return maxX - minX; }
    double spanY() const { return maxY - minY; }
    void scroll(int dxTicks, int dyTicks);
    double minX;
    double minY;
    double maxX;
    double maxY;
    int numXTicks;
    int numYTicks;
    int numXGrid;
    int numYGrid;
};

