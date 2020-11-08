#include "camera.h"
#include "map.h"
#include "player.h"
#include "ui.h"
#include <math.h>

void Camera::setOffsets(const Player& player, const Map& map, const UI& ui)
{
    // TODO: Camera will have to be adjusted once we add UI, it'll be another offset from the UI
    // TODO: This is bugged when viewport is larger than 128, player/camera skips around mid map

    auto [x, y]             = player.getXY();
    auto [width, height]    = map.getXY();

    int halfCamX = cols  / 2;
    int halfCamY = lines / 2;

    if(cols < width)
    {
        // Left edge case
        if(x > halfCamX)
            offsetX = x - halfCamX - 1;

        // Right edge case
        if(x > width - halfCamX)
            offsetX = width - cols;
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