// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <Mib/Core/Core>
#include <Mib/Container/Vector>
#include <Mib/Storage/SharedPointer>

#include "Malterlib_Graphics_Font.h"

namespace NMib::NGraphics
{
	// A font that is drawn on a grid of pixels, and is only pixel perfect at its size and at whole multiples of it
	struct CPixelFontDesign
	{
		NStr::CStr m_Name;
		uint32 m_PixelsPerEm = 0; // The size that the grid of the font is pixels at
		uint32 m_CellWidth = 0; // Of a character, in pixels at m_PixelsPerEm
		uint32 m_CellHeight = 0; // Of a line, in pixels at m_PixelsPerEm
		uint32 m_MaxScale = 1; // The largest multiple of the size that is used
		NStorage::TCSharedPointer<CFont const> m_pFonts[gc_nFontStyles]; // By EFontStyle. Null for a style that the design does not have
	};

	struct CPixelFontSize
	{
		umint m_iDesign = 0; // In CPixelFontSet::m_Designs
		uint32 m_Scale = 1; // What the size of the design is multiplied by
		uint32 m_PixelsPerEm = 0;
		uint32 m_CellWidth = 0; // In pixels
		uint32 m_CellHeight = 0;
		pfp32 m_Size = 0.0f; // m_CellHeight in points, on the display that it was chosen for
	};

	// Designs of a font at different sizes, of which the one that is pixel perfect closest to a size in points is chosen
	struct CPixelFontSet
	{
		NContainer::TCVector<CPixelFontSize> f_GetSizes(pfp32 _PixelsPerPoint, EFontStyle _Style) const;
		CPixelFontSize f_Choose(pfp32 _Size, pfp32 _PixelsPerPoint, EFontStyle _Style) const;
		NStorage::TCSharedPointer<CFont const> const &f_GetFont(CPixelFontSize const &_Size, EFontStyle _Style) const;

		NContainer::TCVector<CPixelFontDesign> m_Designs; // Where two give the same cell, the one at the smaller multiple of its size is used, and then the earlier
	};
}

#ifndef DMibPNoShortCuts
	using namespace NMib::NGraphics;
#endif
