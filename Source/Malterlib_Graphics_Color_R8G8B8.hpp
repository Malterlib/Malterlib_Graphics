// Copyright © 2024 Favro Holding AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

namespace NMib::NGraphics
{
	constexpr inline_always uint8 CColorR8G8B8::f_Red() const
	{
		return (m_Color >> 16) & uint32(0xff);
	}

	constexpr inline_always uint8 CColorR8G8B8::f_Green() const
	{
		return (m_Color >> 8) & uint32(0xff);
	}

	constexpr inline_always uint8 CColorR8G8B8::f_Blue() const
	{
		return m_Color & uint32(0xff);
	}

	constexpr inline_always CColorR8G8B8 CColorR8G8B8::fs_FromRGB(uint8 _Red, uint8 _Green, uint8 _Blue)
	{
		return {.m_Color = uint32(_Red) << 16 | uint32(_Green) << 8 | uint32(_Blue)};
	}
}

#ifndef DMibPNoShortCuts
	using namespace NMib::NGraphics;
#endif
