// Copyright © 2024 Favro Holding AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib::NGraphics
{
	struct CColorR8G8B8
	{
		constexpr inline_always uint8 f_Red();
		constexpr inline_always uint8 f_Green();
		constexpr inline_always uint8 f_Blue();

		static constexpr inline_always CColorR8G8B8 fs_FromRGB(uint8 _Red, uint8 _Green, uint8 _Blue);

		uint32 m_Color;
	};
}

#ifndef DMibPNoShortCuts
	using namespace NMib::NGraphics;
#endif

#include "Malterlib_Graphics_Utilities.hpp"
