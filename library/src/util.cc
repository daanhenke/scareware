#include "util.hh"

// Untested
sw::iface::Vector2D sw::util::WorldToScreen(sw::iface::Vector worldVector)
{
        iface::VMatrix& matrix = interfaces::IVEngineClient->WorldToScreenMatrix();
        iface::Vector2D out;

        int _w, _h;
        interfaces::IVEngineClient->GetScreenSize(_w, _h);
        out.x = (float) _w / 2;
        out.y = (float) _h / 2;

        const float idk = matrix.m[3][0] * worldVector.x + matrix.m[3][1] * worldVector.y + matrix.m[3][2] * worldVector.z + matrix.m[3][3];

        matrix.m[0,2];
        out.x *= 1.f + (matrix.m[0][0] * worldVector.x + matrix.m[0][1] * worldVector.y + matrix.m[0][2] * worldVector.z + matrix.m[0][3]) / idk;
        out.y *= 1.f - (matrix.m[1][0] * worldVector.x + matrix.m[1][1] * worldVector.y + matrix.m[1][2] * worldVector.z + matrix.m[1][3]) / idk;

        return out;
}