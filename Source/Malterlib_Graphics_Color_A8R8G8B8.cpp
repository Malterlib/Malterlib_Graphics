// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "Malterlib_Graphics_Color_A8R8G8B8.h"

#include <Mib/Graphics/ColorR8G8B8>

namespace NMib::NGraphics
{
	CColorA8R8G8B8::operator CColorR8G8B8()
	{
		return {.m_Color = m_Color};
	}
}
