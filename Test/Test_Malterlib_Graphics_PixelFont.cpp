// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Mib/Graphics/UnbrokenFont>
#include <Mib/File/File>
#include <Mib/Container/Vector>

using namespace NMib;
using namespace NMib::NGraphics;
using namespace NMib::NStr;
using namespace NMib::NContainer;

namespace
{
	CStr fg_FontDirectory()
	{
		return NFile::CFile::fs_AppendPath(NFile::CFile::fs_GetPath(CStr(__FILE__)), "../Fonts/Unbroken");
	}

	CPixelFontSet fg_UnbrokenSet()
	{
		return fg_CreateUnbrokenFontSet
			(
				[](CStr const &_FileName)
				{
					return NFile::CFile::fs_ReadFile(NFile::CFile::fs_AppendPath(fg_FontDirectory(), _FileName));
				}
			)
		;
	}

	CStr fg_DescribeSizes(TCVector<CPixelFontSize> const &_Sizes)
	{
		CStr Description;
		for (auto &Size : _Sizes)
			Description += "{}x{}:{}x{} "_f << Size.m_CellWidth << Size.m_CellHeight << Size.m_iDesign << Size.m_Scale;

		return Description;
	}

	struct CPixelFont_Tests : public NMib::NTest::CTest
	{
		void f_DoTests()
		{
			DMibTestSuite("Sizes")
			{
				auto Set = fg_UnbrokenSet();

				{
					DMibTestPath("Unbroken");

					// The sizes of the onboarding of Unbroken Code: the standard design at odd multiples, as the others are
					// covered by Retina at a smaller multiple
					DMibExpect
						(
							fg_DescribeSizes(Set.f_GetSizes(2.0f, EFontStyle::mc_Regular))
							, ==
							, "6x10:0x1 12x20:1x1 14x24:2x1 18x30:0x3 24x40:1x2 28x48:2x2 30x50:0x5 36x60:1x3 42x72:2x3 48x80:1x4 56x96:2x4 "
							"60x100:1x5 70x120:2x5 "
						)
					;
				}

				{
					DMibTestPath("InPoints");

					auto Sizes = Set.f_GetSizes(2.0f, EFontStyle::mc_Regular);
					DMibExpect(Sizes[1].m_Size, ==, 10.0f);
					DMibExpect(Sizes[2].m_Size, ==, 12.0f);
				}

				{
					DMibTestPath("WithoutStyle");

					CPixelFontSet Partial;
					Partial.m_Designs.f_Insert(Set.m_Designs[0]);
					Partial.m_Designs.f_Insert(Set.m_Designs[1]);
					Partial.m_Designs[0].m_MaxScale = 2;
					Partial.m_Designs[1].m_MaxScale = 1;

					DMibExpect(fg_DescribeSizes(Partial.f_GetSizes(1.0f, EFontStyle::mc_Regular)), ==, "6x10:0x1 12x20:1x1 ");
					DMibExpect(fg_DescribeSizes(Partial.f_GetSizes(1.0f, EFontStyle::mc_Bold)), ==, "12x20:1x1 ");
				}
			};

			DMibTestSuite("Choose")
			{
				auto Set = fg_UnbrokenSet();

				auto fChoose = [&](pfp32 _Size, pfp32 _PixelsPerPoint)
					{
						auto Size = Set.f_Choose(_Size, _PixelsPerPoint, EFontStyle::mc_Regular);
						return CStr("{}x{}"_f << Size.m_iDesign << Size.m_Scale);
					}
				;

				{
					DMibTestPath("Retina");

					DMibExpect(fChoose(10.0f, 2.0f), ==, "1x1");
					DMibExpect(fChoose(12.0f, 2.0f), ==, "2x1");
					DMibExpect(fChoose(20.0f, 2.0f), ==, "1x2");
					DMibExpect(fChoose(5.0f, 2.0f), ==, "0x1");
				}

				{
					DMibTestPath("Standard");

					DMibExpect(fChoose(10.0f, 1.0f), ==, "0x1");
					DMibExpect(fChoose(12.0f, 1.0f), ==, "0x1");
					DMibExpect(fChoose(15.0f, 1.0f), ==, "1x1");
					DMibExpect(fChoose(30.0f, 1.0f), ==, "0x3");
				}

				{
					DMibTestPath("Fractional");

					// At 150 %, 10 points are 15 pixels, which is closer to the 20 of Retina than to the 10 of the standard design
					DMibExpect(fChoose(10.0f, 1.5f), ==, "1x1");
					DMibExpect(fChoose(8.0f, 1.5f), ==, "0x1");
				}
			};

			DMibTestSuite("Glyphs")
			{
				auto Set = fg_UnbrokenSet();

				for (umint iDesign = 0; iDesign < Set.m_Designs.f_GetLen(); ++iDesign)
				{
					auto &Design = Set.m_Designs[iDesign];
					DMibTestPath(Design.m_Name);

					for (umint iStyle = 0; iStyle < gc_nFontStyles; ++iStyle)
					{
						auto &pFont = Design.m_pFonts[iStyle];
						if (!pFont)
							continue;

						DMibTestPath("Style{}"_f << iStyle);

						for (uint32 Scale = 1; Scale <= 3; ++Scale)
						{
							DMibTestPath("x{}"_f << Scale);

							uint32 PixelsPerEm = Design.m_PixelsPerEm * Scale;
							auto Metrics = pFont->f_GetMetrics(PixelsPerEm);
							DMibExpect(uint32(Metrics.m_LineHeight), ==, Design.m_CellHeight * Scale);
							DMibExpect(uint32(Metrics.m_Ascender + Metrics.m_Descender), ==, Design.m_CellHeight * Scale);

							umint nMissing = 0;
							umint nWrongAdvance = 0;
							for (uint32 CodePoint = 0x20; CodePoint < 0x7f; ++CodePoint)
							{
								CFontGlyph Glyph;
								if (!pFont->f_RenderGlyph(CodePoint, PixelsPerEm, Glyph))
									++nMissing;
								else
									nWrongAdvance += uint32(Glyph.m_Advance) != Design.m_CellWidth * Scale;
							}

							DMibExpect(nMissing, ==, 0);
							DMibExpect(nWrongAdvance, ==, 0);

							// Glyphs of only straight lines cover whole pixels. Curves and diagonals are drawn with partly
							// covered pixels by the design. Some edges of the fonts are a unit or two off the grid, which
							// leaves 1 % of a pixel
							umint nPartial = 0;
							for (uint32 CodePoint : {uint32('H'), uint32('L'), uint32('T'), uint32('E'), uint32('|'), uint32('_'), uint32('-')})
							{
								CFontGlyph Glyph;
								pFont->f_RenderGlyph(CodePoint, PixelsPerEm, Glyph);
								for (uint8 Value : Glyph.m_Coverage)
									nPartial += Value > 5 && Value < 250;
							}

							DMibExpect(nPartial, ==, 0);
						}
					}
				}
			};
		}
	};

	DMibTestRegister(CPixelFont_Tests, Malterlib::Graphics::PixelFont);
}
