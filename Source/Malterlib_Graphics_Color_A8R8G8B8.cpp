// Copyright © 2024 Unbroken AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#include "Malterlib_Graphics_Color_A8R8G8B8.h"

#include <Mib/Graphics/ColorR8G8B8>

namespace NMib::NGraphics
{
	CColorA8R8G8B8::operator CColorR8G8B8()
	{
		return {.m_Color = m_Color};
	}
}
