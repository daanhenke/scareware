#pragma once

#include "iface/Vector2D.hh"
#include "iface/Vector.hh"
#include "iface/VMatrix.hh"
#include "interfaces.hh"

namespace sw::util
{
    iface::Vector2D WorldToScreen(iface::Vector worldVector);
}