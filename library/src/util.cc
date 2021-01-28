#include "util.hh"
#include "iface/Vector.hh"

// Absolutely perfect, dw
bool sw::util::WorldToScreen(sw::iface::Vector& point, sw::iface::Vector& screen)
{
    auto matrix = interfaces::IVEngineClient->WorldToScreenMatrix();
    bool inScreen = true;

    screen.x = matrix._11 * point.x + matrix._12 * point.y + matrix._13 * point.z + matrix._14;
    screen.y = matrix._21 * point.x + matrix._22 * point.y + matrix._23 * point.z + matrix._24;
    screen.z = 0.f;

    float w = matrix._41 * point.x + matrix._42 * point.y + matrix._43 * point.z + matrix._44;

    if (w < 0.001f)
    {
        inScreen = false;
    }
    else
    {
        float invertedw = 1.f / w;
        screen.x *= invertedw;
        screen.y *= invertedw;

        int width, height;
        interfaces::IVEngineClient->GetScreenSize(width, height);
        float x = width / 2;
        float y = height / 2;
        x += .5 * screen.x * width + .5;
        y -= .5 * screen.y * height + .5;
        screen.x = x;
        screen.y = y;
    }

    return inScreen;
}

void sw::util::VectorAngles(iface::Vector& forward, iface::Vector& angles)
{
    if (forward.y == 0.f && forward.x == 0.f)
    {
        angles.x = (forward.z > 0.f) ? 270.f : 90.f;
        angles.y = 0.f;
    }
    else
    {
        angles.x = atan2(-forward.z, forward.Length2D()) * -180 / M_PI;
        angles.y = atan2(forward.y, forward.x) * 180 / M_PI;

        if (angles.y > 90) angles.y -= 180;
        else if (angles.y < 90) angles.y += 180;
        else if(angles.y == 90) angles.y = 0;
    }

    angles.z = 0;
}

sw::iface::Vector sw::util::CalcAngle(iface::Vector source, iface::Vector destination)
{
    iface::Vector angle(0, 0, 0);
    iface::Vector delta(source.x - destination.x, source.y - destination.y, source.z - destination.z);

    VectorAngles(delta, angle);

    return angle;
}

sw::iface::Vector sw::util::ClampAngle(sw::iface::Vector angle)
{
    if (angle.x > 89.f) angle.x = 89.f;
    if (angle.x < -89.f) angle.x = -89.f;

    if (angle.y > 180.f) angle.y = 180.f;
    if (angle.y < -180.f) angle.y = -180.f;

    angle.z = 0.f;

    return angle;
}

sw::iface::Color sw::util::MixColors(iface::Color a, iface::Color b, float mix)
{
    iface::Color result;

    result.r = a.r + mix * (b.r - a.r);
    result.g = a.g + mix * (b.g - a.g);
    result.b = a.b + mix * (b.b - a.b);
    result.a = a.a + mix * (b.a - a.a);

    return result;
}