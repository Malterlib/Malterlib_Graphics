// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "Malterlib_Graphics_Color_R8G8B8.h"

#include <Mib/Graphics/ColorA8R8G8B8>

namespace NMib::NGraphics
{
	CColorR8G8B8::operator CColorA8R8G8B8() const
	{
		return CColorA8R8G8B8::fs_FromRGBA(f_Red(), f_Green(), f_Blue(), 255);
	}
}
