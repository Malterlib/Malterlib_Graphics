// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "Malterlib_Graphics_UnbrokenFont.h"

namespace NMib::NGraphics
{
	namespace
	{
		// The outlines are drawn on the grid of pixels, and the instructions that ttfautohint added move details off it, as the dot of 0
		constexpr EFontGridFitting gc_UnbrokenGridFitting = EFontGridFitting::mc_None;

		auto fg_UnbrokenDesign
			(
				FReadUnbrokenFontFile const &_fReadFile
				, NStr::CStr const &_Name
				, NStr::CStr const &_FilePrefix
				, uint32 _PixelsPerEm
				, uint32 _CellWidth
				, uint32 _CellHeight
				, bool _bBold
			)
			-> CPixelFontDesign
		{
			CPixelFontDesign Design;
			Design.m_Name = _Name;
			Design.m_PixelsPerEm = _PixelsPerEm;
			Design.m_CellWidth = _CellWidth;
			Design.m_CellHeight = _CellHeight;
			Design.m_MaxScale = 5;
			Design.m_pFonts[umint(EFontStyle::mc_Regular)] = fg_Construct(_fReadFile(_FilePrefix + "-Regular.ttf"), gc_UnbrokenGridFitting);
			if (_bBold)
				Design.m_pFonts[umint(EFontStyle::mc_Bold)] = fg_Construct(_fReadFile(_FilePrefix + "-Bold.ttf"), gc_UnbrokenGridFitting);

			return Design;
		}
	}

	// The styles that are pixel perfect, as the onboarding of Unbroken Code offers them: the strokes of bold and italic
	// are not on whole pixels in the design of 10 pixels, and italics are slanted in all of them
	CPixelFontSet fg_CreateUnbrokenFontSet(FReadUnbrokenFontFile const &_fReadFile)
	{
		CPixelFontSet Set;
		Set.m_Designs.f_Insert(fg_UnbrokenDesign(_fReadFile, "Unbroken", "Unbroken", 10, 6, 10, false));
		Set.m_Designs.f_Insert(fg_UnbrokenDesign(_fReadFile, "Unbroken Retina", "Unbroken-Retina", 20, 12, 20, true));
		Set.m_Designs.f_Insert(fg_UnbrokenDesign(_fReadFile, "Unbroken 12 Retina", "Unbroken12-Retina", 24, 14, 24, true));
		return Set;
	}
}
