// Copyright © 2024 Unbroken AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib::NGraphics
{
	struct CColorR8G8B8
	{
		constexpr auto operator <=> (CColorR8G8B8 const &_Right) const = default;

		constexpr inline_always uint8 f_Red() const;
		constexpr inline_always uint8 f_Green() const;
		constexpr inline_always uint8 f_Blue() const;

		static constexpr inline_always CColorR8G8B8 fs_FromRGB(uint8 _Red, uint8 _Green, uint8 _Blue);

		uint32 m_Color = 0;
	};
}

#ifndef DMibPNoShortCuts
	using namespace NMib::NGraphics;
#endif

#include "Malterlib_Graphics_Color_R8G8B8.hpp"
