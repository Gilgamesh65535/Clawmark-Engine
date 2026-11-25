#include <cmath>
#include "Globals.hpp"
#include "Sprite.hpp"

void RenderedTarget::SetXY(double x, double y, bool force) {
    if (isStage) return;
    if (dragging && !force) return;
    auto [fx, fy] = GetFencedPosition(x, y);
    this->x = fx;
    this->y = fy;
    if (visible) g_redrawRequested = true;
}

std::pair<double, double> RenderedTarget::GetFencedPosition(double newX, double newY) {
    const Costume& costume = sprite->costumes[currentCostume];

    double scaledW = (costume.width * size / 100.0) / costume.bitmapResolution;
    double scaledH = (costume.height * size / 100.0) / costume.bitmapResolution;
    double rcX = (costume.rotationCenterX * size / 100.0) / costume.bitmapResolution;
    double rcY = (costume.rotationCenterY * size / 100.0) / costume.bitmapResolution;

    double aabbLeft, aabbRight, aabbTop, aabbBottom;

    if (rotationStyle == RotationStyle::ALL_AROUND) {
        double angleRad = (direction - 90.0) * M_PI / 180.0;
        double cosA = std::cos(angleRad);
        double sinA = std::sin(angleRad);

        double corners[4][2] = {
            { -rcX,          -rcY          },
            { scaledW - rcX, -rcY          },
            { scaledW - rcX, scaledH - rcY },
            { -rcX,          scaledH - rcY }
        };

        double minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9;
        for (auto& corner : corners) {
            double rx = corner[0] * cosA - corner[1] * sinA;
            double ry = corner[0] * sinA + corner[1] * cosA;
            minX = std::min(minX, rx);
            maxX = std::max(maxX, rx);
            minY = std::min(minY, ry);
            maxY = std::max(maxY, ry);
        }

        aabbLeft   = newX + minX;
        aabbRight  = newX + maxX;
        aabbBottom = newY + minY;
        aabbTop    = newY + maxY;
    } else {
        aabbLeft   = newX - rcX;
        aabbRight  = newX + (scaledW - rcX);
        aabbBottom = newY - rcY;
        aabbTop    = newY + (scaledH - rcY);
    }

    double aabbW = aabbRight - aabbLeft;
    double aabbH = aabbTop - aabbBottom;

    double inset = std::floor(std::min(aabbW, aabbH) / 2.0);
    double sx = 240.0 - std::min(15.0, inset);
    double sy = 180.0 - std::min(15.0, inset);

    if (aabbRight < -sx) {
        newX = std::ceil(newX - (sx + aabbRight));
    } else if (aabbLeft > sx) {
        newX = std::floor(newX + (sx - aabbLeft));
    }

    if (aabbBottom < -sy) {
        newY = std::ceil(newY - (sy + aabbBottom));
    } else if (aabbTop > sy) {
        newY = std::floor(newY + (sy - aabbTop));
    }

    return {newX, newY};
}