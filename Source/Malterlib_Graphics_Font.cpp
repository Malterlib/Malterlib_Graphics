// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "Malterlib_Graphics_Font.h"

#include <Mib/Thread/Thread>

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace NMib::NStr;

namespace NMib::NGraphics
{
	namespace
	{
		int32 fg_RoundFontUnits(int64 _Units, uint32 _PixelsPerEm, uint32 _UnitsPerEm)
		{
			int64 Scaled = _Units * int64(_PixelsPerEm);
			int64 Half = int64(_UnitsPerEm) / 2;
			return int32(Scaled >= 0 ? (Scaled + Half) / int64(_UnitsPerEm) : -((-Scaled + Half) / int64(_UnitsPerEm)));
		}
	}

	struct CFont::CInternal
	{
		~CInternal()
		{
			if (m_pFace)
				FT_Done_Face(m_pFace);
			if (m_pLibrary)
				FT_Done_FreeType(m_pLibrary);
		}

		void f_SetSize(uint32 _PixelsPerEm)
		{
			if (m_PixelsPerEm == _PixelsPerEm)
				return;

			if (FT_Error Error = FT_Set_Pixel_Sizes(m_pFace, 0, _PixelsPerEm))
				DMibError("FreeType could not set the size {} of a font: error {}"_f << _PixelsPerEm << Error);

			m_PixelsPerEm = _PixelsPerEm;
		}

		NContainer::CByteVector m_Data; // FreeType reads the file from here while the face exists
		FT_Library m_pLibrary = nullptr;
		FT_Face m_pFace = nullptr;
		uint32 m_PixelsPerEm = 0;
		EFontGridFitting m_GridFitting = EFontGridFitting::mc_Font;
		NThread::CMutual m_Lock; // A face can only be used by one thread at a time
	};

	CFont::CFont(NContainer::CByteVector &&_Data, EFontGridFitting _GridFitting)
		: mp_pInternal(fg_Construct())
	{
		auto &Internal = *mp_pInternal;
		Internal.m_Data = fg_Move(_Data);
		Internal.m_GridFitting = _GridFitting;

		if (FT_Error Error = FT_Init_FreeType(&Internal.m_pLibrary))
			DMibError("FreeType could not be initialized: error {}"_f << Error);

		if
		(
			FT_Error Error = FT_New_Memory_Face
				(
					Internal.m_pLibrary
					, Internal.m_Data.f_GetArray()
					, FT_Long(Internal.m_Data.f_GetLen())
					, 0
					, &Internal.m_pFace
				)
		)
		{
			DMibError("FreeType could not load a font: error {}"_f << Error);
		}

		if (!FT_IS_SCALABLE(Internal.m_pFace))
			DMibError("Only fonts with outlines are supported");
	}

	CFont::~CFont() = default;

	NStr::CStr CFont::f_GetFamilyName() const
	{
		return mp_pInternal->m_pFace->family_name ? NStr::CStr(mp_pInternal->m_pFace->family_name) : NStr::CStr();
	}

	NStr::CStr CFont::f_GetStyleName() const
	{
		return mp_pInternal->m_pFace->style_name ? NStr::CStr(mp_pInternal->m_pFace->style_name) : NStr::CStr();
	}

	bool CFont::f_HasGlyph(uint32 _CodePoint) const
	{
		auto &Internal = *mp_pInternal;
		DMibLock(Internal.m_Lock);
		return FT_Get_Char_Index(Internal.m_pFace, _CodePoint) != 0;
	}

	NContainer::TCVector<uint32> CFont::f_GetCodePoints() const
	{
		auto &Internal = *mp_pInternal;
		DMibLock(Internal.m_Lock);

		NContainer::TCVector<uint32> CodePoints;
		FT_UInt iGlyph = 0;
		for (FT_ULong CodePoint = FT_Get_First_Char(Internal.m_pFace, &iGlyph); iGlyph; CodePoint = FT_Get_Next_Char(Internal.m_pFace, CodePoint, &iGlyph))
			CodePoints.f_Insert(uint32(CodePoint));

		return CodePoints;
	}

	CFontMetrics CFont::f_GetMetrics(uint32 _PixelsPerEm) const
	{
		auto &Face = *mp_pInternal->m_pFace;
		uint32 UnitsPerEm = Face.units_per_EM;

		CFontMetrics Metrics;
		Metrics.m_Ascender = fg_RoundFontUnits(Face.ascender, _PixelsPerEm, UnitsPerEm);
		Metrics.m_Descender = -fg_RoundFontUnits(Face.descender, _PixelsPerEm, UnitsPerEm);
		Metrics.m_LineHeight = fg_RoundFontUnits(Face.height, _PixelsPerEm, UnitsPerEm);

		return Metrics;
	}

	bool CFont::f_RenderGlyph(uint32 _CodePoint, uint32 _PixelsPerEm, CFontGlyph &o_Glyph) const
	{
		auto &Internal = *mp_pInternal;
		DMibLock(Internal.m_Lock);

		FT_UInt iGlyph = FT_Get_Char_Index(Internal.m_pFace, _CodePoint);
		if (!iGlyph)
			return false;

		Internal.f_SetSize(_PixelsPerEm);

		FT_Int32 LoadFlags = FT_LOAD_RENDER | (Internal.m_GridFitting == EFontGridFitting::mc_None ? FT_LOAD_NO_HINTING : FT_LOAD_TARGET_NORMAL);
		if (FT_Error Error = FT_Load_Glyph(Internal.m_pFace, iGlyph, LoadFlags))
			DMibError("FreeType could not render the glyph of U+{nfh}: error {}"_f << _CodePoint << Error);

		FT_GlyphSlot pSlot = Internal.m_pFace->glyph;
		FT_Bitmap const &Bitmap = pSlot->bitmap;
		if (Bitmap.pixel_mode != FT_PIXEL_MODE_GRAY && Bitmap.width && Bitmap.rows)
			DMibError("FreeType rendered the glyph of U+{nfh} in pixel mode {}"_f << _CodePoint << Bitmap.pixel_mode);

		o_Glyph.m_Width = Bitmap.width;
		o_Glyph.m_Height = Bitmap.rows;
		o_Glyph.m_Left = pSlot->bitmap_left;
		o_Glyph.m_Top = pSlot->bitmap_top;
		o_Glyph.m_Advance = int32((pSlot->advance.x + 32) >> 6);
		o_Glyph.m_Coverage.f_SetLen(umint(Bitmap.width) * Bitmap.rows);

		for (uint32 iRow = 0; iRow < Bitmap.rows; ++iRow)
		{
			uint8 const *pSource = Bitmap.buffer + smint(iRow) * Bitmap.pitch;
			NMemory::fg_MemCopy(o_Glyph.m_Coverage.f_GetArray() + umint(iRow) * Bitmap.width, pSource, Bitmap.width);
		}

		return true;
	}
}
