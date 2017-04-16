#include "PlotterSetting.h"


PlotterSetting::PlotterSetting(void)
{
    minX = -10.0;
    minY = -10.0;
    maxX = 10.0;
    maxY = 10.0;
    numXTicks = 20;
    numYTicks = 20;
    numXGrid = 40;
    numYGrid = 40;
}


PlotterSetting::~PlotterSetting(void)
{
}

void PlotterSetting::scroll(int dxTicks, int dyTicks)
{
    double stepX = spanX() / numXTicks;
    minX += dxTicks * stepX;
    maxX += dxTicks * stepX;

    double stepY = spanY() / numYTicks;
    minY += dyTicks * stepY;
    maxY += dyTicks * stepY;
}
