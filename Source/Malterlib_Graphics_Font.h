// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <Mib/Core/Core>
#include <Mib/Container/Vector>
#include <Mib/Storage/UniquePointer>

namespace NMib::NGraphics
{
	enum class EFontStyle : uint8
	{
		mc_Regular
		, mc_Bold
		, mc_Italic
		, mc_BoldItalic
	};

	inline constexpr umint gc_nFontStyles = 4;

	enum class EFontGridFitting : uint8
	{
		mc_Font // As the instructions of the font say
		, mc_None // The outlines as they are, for fonts that are drawn on the grid of pixels
	};

	// The coverage of a glyph at a size, as FreeType renders it
	struct CFontGlyph
	{
		uint32 m_Width = 0; // In pixels
		uint32 m_Height = 0;
		int32 m_Left = 0; // From the pen to the left of the image
		int32 m_Top = 0; // From the baseline up to the top of the image
		int32 m_Advance = 0; // How far the pen moves, in whole pixels
		NContainer::TCVector<uint8> m_Coverage; // Rows from the top, from 0 to 255
	};

	struct CFontMetrics
	{
		int32 m_Ascender = 0; // Above the baseline, in pixels
		int32 m_Descender = 0; // Below the baseline, as a positive distance
		int32 m_LineHeight = 0; // From one baseline to the next
	};

	// A font file. Threads that use it at the same time take turns
	struct CFont
	{
		CFont(NContainer::CByteVector &&_Data, EFontGridFitting _GridFitting = EFontGridFitting::mc_Font);
		~CFont();

		CFont(CFont const &) = delete;
		CFont &operator = (CFont const &) = delete;

		NStr::CStr f_GetFamilyName() const;
		NStr::CStr f_GetStyleName() const;
		bool f_HasGlyph(uint32 _CodePoint) const;
		NContainer::TCVector<uint32> f_GetCodePoints() const;
		CFontMetrics f_GetMetrics(uint32 _PixelsPerEm) const;
		bool f_RenderGlyph(uint32 _CodePoint, uint32 _PixelsPerEm, CFontGlyph &o_Glyph) const;

	private:
		struct CInternal;
		NStorage::TCUniquePointer<CInternal> mp_pInternal;
	};
}

#ifndef DMibPNoShortCuts
	using namespace NMib::NGraphics;
#endif
