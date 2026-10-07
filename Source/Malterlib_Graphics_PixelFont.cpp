// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "Malterlib_Graphics_PixelFont.h"

#include <Mib/Container/Map>

#include <math.h>

namespace NMib::NGraphics
{
	NContainer::TCVector<CPixelFontSize> CPixelFontSet::f_GetSizes(pfp32 _PixelsPerPoint, EFontStyle _Style) const
	{
		struct CCell
		{
			auto operator <=> (CCell const &_Right) const = default;

			uint32 m_Height;
			uint32 m_Width;
		};

		NContainer::TCMap<CCell, CPixelFontSize> Sizes;
		for (umint iDesign = 0; iDesign < m_Designs.f_GetLen(); ++iDesign)
		{
			auto &Design = m_Designs[iDesign];
			if (!Design.m_pFonts[umint(_Style)])
				continue;

			for (uint32 Scale = 1; Scale <= Design.m_MaxScale; ++Scale)
			{
				CPixelFontSize Size;
				Size.m_iDesign = iDesign;
				Size.m_Scale = Scale;
				Size.m_PixelsPerEm = Design.m_PixelsPerEm * Scale;
				Size.m_CellWidth = Design.m_CellWidth * Scale;
				Size.m_CellHeight = Design.m_CellHeight * Scale;
				Size.m_Size = pfp32(Size.m_CellHeight) / _PixelsPerPoint;

				auto &Existing = Sizes[CCell{Size.m_CellHeight, Size.m_CellWidth}];
				if (!Existing.m_PixelsPerEm || Scale < Existing.m_Scale)
					Existing = Size;
			}
		}

		NContainer::TCVector<CPixelFontSize> Result;
		for (auto &Size : Sizes)
			Result.f_Insert(Size);

		return Result;
	}

	CPixelFontSize CPixelFontSet::f_Choose(pfp32 _Size, pfp32 _PixelsPerPoint, EFontStyle _Style) const
	{
		auto Sizes = f_GetSizes(_PixelsPerPoint, _Style);
		if (Sizes.f_IsEmpty())
			DMibError("No design of the pixel font has the style that was asked for");

		// By the ratio of the sizes, so that a size twice as large is as far as one half as large
		CPixelFontSize const *pBest = nullptr;
		pfp32 BestDistance = 0.0f;
		for (auto &Size : Sizes)
		{
			pfp32 Distance = fabsf(logf(Size.m_Size / _Size));
			if (!pBest || Distance < BestDistance)
			{
				pBest = &Size;
				BestDistance = Distance;
			}
		}

		return *pBest;
	}

	NStorage::TCSharedPointer<CFont const> const &CPixelFontSet::f_GetFont(CPixelFontSize const &_Size, EFontStyle _Style) const
	{
		return m_Designs[_Size.m_iDesign].m_pFonts[umint(_Style)];
	}
}
