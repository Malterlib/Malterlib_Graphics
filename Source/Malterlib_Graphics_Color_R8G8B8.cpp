// Copyright © 2024 Unbroken AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#include "Malterlib_Graphics_Color_R8G8B8.h"

#include <Mib/Graphics/ColorA8R8G8B8>

namespace NMib::NGraphics
{
	CColorR8G8B8::operator CColorA8R8G8B8()
	{
		return {.m_Color = m_Color};
	}
}
