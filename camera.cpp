#include "camera.h"
#include "map.h"
#include "player.h"
#include "ui.h"
#include <math.h>

void Camera::setOffsets(const Player& player, const Map& map, const UI& ui)
{
    auto [x, y]             = player.getXY();
    auto [width, height]    = map.getXY();

    int halfCamX = (cols - ui.getSize())  / 2;
    int halfCamY = lines / 2;

    if(cols < width + ui.getSize())
    {
        // Left edge case
        if(x > halfCamX)
            offsetX = x - halfCamX - 1;

        // Right edge case
        if(x > width - halfCamX + 1)
            offsetX -= x - (width - halfCamX + 1);
    }

    // Y axis seems off-center with camera
    if(lines < height)
    {
        // Top case
        if(y > halfCamY)
            offsetY = y - halfCamY - 1;

        // Bottom case
        if(y > height - halfCamY)
            offsetY = height - lines;
    }
}
