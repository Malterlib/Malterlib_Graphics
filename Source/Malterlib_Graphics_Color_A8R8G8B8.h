// Copyright © 2024 Unbroken AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

#include <Mib/Graphics/Blend>

namespace NMib::NGraphics
{
	struct CColorR8G8B8;

	struct CColorA8R8G8B8
	{
		operator CColorR8G8B8();

		constexpr auto operator <=> (CColorA8R8G8B8 const &_Right) const = default;

		constexpr CColorA8R8G8B8 operator - (CColorA8R8G8B8 _Other) const;
		constexpr CColorA8R8G8B8 operator + (CColorA8R8G8B8 _Other) const;
		constexpr CColorA8R8G8B8 operator * (CColorA8R8G8B8 _Other) const;

		constexpr CColorA8R8G8B8 f_Invert() const;
		constexpr CColorA8R8G8B8 f_Min(CColorA8R8G8B8 _Other) const;
		constexpr CColorA8R8G8B8 f_Max(CColorA8R8G8B8 _Other) const;

		template <typename tf_CStr>
		void f_Format(tf_CStr &o_Str) const;
		
		constexpr CColorA8R8G8B8 f_Blend(CColorA8R8G8B8 _DestinationColor, CColorBlend const &_Blend);

		constexpr inline_always uint8 f_Alpha() const;
		constexpr inline_always uint8 f_Red() const;
		constexpr inline_always uint8 f_Green() const;
		constexpr inline_always uint8 f_Blue() const;

		static constexpr inline_always CColorA8R8G8B8 fs_FromRGBA(uint8 _Red, uint8 _Green, uint8 _Blue, uint8 _Alpha = 255);

		uint32 m_Color = 0;
	};
}

#ifndef DMibPNoShortCuts
	using namespace NMib::NGraphics;
#endif

#include "Malterlib_Graphics_Color_A8R8G8B8.hpp"
