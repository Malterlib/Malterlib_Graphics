// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Mib/Graphics/ColorSpace>
#include <Mib/Container/Vector>

using namespace NMib;
using namespace NMib::NGraphics;
using namespace NMib::NStr;
using namespace NMib::NContainer;

namespace
{
	// What the standards round their values to
	constexpr pfp64 gc_PublishedTolerance = 1e-4;

	// What is lost to pfp64 on the way
	constexpr pfp64 gc_ExactTolerance = 1e-12;

	bool fg_Near(pfp64 _Left, pfp64 _Right, pfp64 _Tolerance)
	{
		return __builtin_fabs(_Left - _Right) <= _Tolerance;
	}

	umint fg_CountMismatches(CColorMatrix const &_Matrix, pfp64 const (&_Expected)[3][3], pfp64 _Tolerance)
	{
		umint nMismatches = 0;
		for (umint iRow = 0; iRow < 3; ++iRow)
		{
			for (umint iColumn = 0; iColumn < 3; ++iColumn)
				nMismatches += !fg_Near(_Matrix.m_Rows[iRow][iColumn], _Expected[iRow][iColumn], _Tolerance);
		}

		return nMismatches;
	}

	umint fg_CountMismatches(pfp64 const *_pValues, pfp64 _Expected0, pfp64 _Expected1, pfp64 _Expected2, pfp64 _Tolerance)
	{
		return umint(!fg_Near(_pValues[0], _Expected0, _Tolerance))
			+ umint(!fg_Near(_pValues[1], _Expected1, _Tolerance))
			+ umint(!fg_Near(_pValues[2], _Expected2, _Tolerance))
		;
	}

	constexpr uint32 fg_Signature(ch8 const *_pName)
	{
		return (uint32(uint8(_pName[0])) << 24) | (uint32(uint8(_pName[1])) << 16) | (uint32(uint8(_pName[2])) << 8) | uint32(uint8(_pName[3]));
	}

	void fg_AddUInt32(TCVector<uint8> &o_Bytes, uint32 _Value)
	{
		o_Bytes.f_Insert(uint8(_Value >> 24));
		o_Bytes.f_Insert(uint8(_Value >> 16));
		o_Bytes.f_Insert(uint8(_Value >> 8));
		o_Bytes.f_Insert(uint8(_Value));
	}

	// A profile for RGB with a matrix and curves. _pPrimaries has red, green and blue, and _pToD50 is null for a
	// profile without the matrix to D50.
	TCVector<uint8> fg_CreateProfile(uint32 const *_pPrimaries, uint32 const *_pWhite, uint32 const *_pToD50, TCVector<uint8> const &_Curve)
	{
		struct CTag
		{
			uint32 m_Signature;
			TCVector<uint8> m_Data;
		};

		auto fGetXYZ = [](uint32 const *_pValues)
			{
				TCVector<uint8> Data;
				fg_AddUInt32(Data, fg_Signature("XYZ "));
				fg_AddUInt32(Data, 0);
				for (umint i = 0; i < 3; ++i)
					fg_AddUInt32(Data, _pValues[i]);

				return Data;
			}
		;

		TCVector<CTag> Tags;
		Tags.f_Insert(CTag{fg_Signature("rXYZ"), fGetXYZ(_pPrimaries)});
		Tags.f_Insert(CTag{fg_Signature("gXYZ"), fGetXYZ(_pPrimaries + 3)});
		Tags.f_Insert(CTag{fg_Signature("bXYZ"), fGetXYZ(_pPrimaries + 6)});
		Tags.f_Insert(CTag{fg_Signature("wtpt"), fGetXYZ(_pWhite)});
		Tags.f_Insert(CTag{fg_Signature("rTRC"), _Curve});
		Tags.f_Insert(CTag{fg_Signature("gTRC"), _Curve});
		Tags.f_Insert(CTag{fg_Signature("bTRC"), _Curve});

		if (_pToD50)
		{
			TCVector<uint8> Data;
			fg_AddUInt32(Data, fg_Signature("sf32"));
			fg_AddUInt32(Data, 0);
			for (umint i = 0; i < 9; ++i)
				fg_AddUInt32(Data, _pToD50[i]);

			Tags.f_Insert(CTag{fg_Signature("chad"), fg_Move(Data)});
		}

		TCVector<uint8> Profile;
		Profile.f_SetLen(128);
		for (auto &Byte : Profile)
			Byte = 0;

		auto fSetUInt32 = [&](umint _Offset, uint32 _Value)
			{
				Profile[_Offset] = uint8(_Value >> 24);
				Profile[_Offset + 1] = uint8(_Value >> 16);
				Profile[_Offset + 2] = uint8(_Value >> 8);
				Profile[_Offset + 3] = uint8(_Value);
			}
		;

		fSetUInt32(16, fg_Signature("RGB "));
		fSetUInt32(20, fg_Signature("XYZ "));
		fSetUInt32(36, fg_Signature("acsp"));

		fg_AddUInt32(Profile, uint32(Tags.f_GetLen()));

		uint32 Offset = uint32(132 + Tags.f_GetLen() * 12);
		for (auto &Tag : Tags)
		{
			fg_AddUInt32(Profile, Tag.m_Signature);
			fg_AddUInt32(Profile, Offset);
			fg_AddUInt32(Profile, uint32(Tag.m_Data.f_GetLen()));

			Offset += uint32(Tag.m_Data.f_GetLen());
		}

		for (auto &Tag : Tags)
			Profile.f_Insert(Tag.m_Data.f_GetArray(), Tag.m_Data.f_GetLen());

		fSetUInt32(0, uint32(Profile.f_GetLen()));

		return Profile;
	}

	struct CColor_Tests : public NMib::NTest::CTest
	{
		void f_DoTests()
		{
			DMibTestSuite("Primaries")
			{
				{
					// IEC 61966-2-1
					DMibTestPath("Rec709ToXYZ");
					constexpr pfp64 c_Expected[3][3] =
						{
							{0.4124, 0.3576, 0.1805}
							, {0.2126, 0.7152, 0.0722}
							, {0.0193, 0.1192, 0.9505}
						}
					;

					DMibExpect(fg_CountMismatches(CColorPrimaries::fs_Rec709().f_GetMatrixToXYZ(), c_Expected, gc_PublishedTolerance), ==, 0);
				}

				{
					// How bright the primaries are, from Rec. 2020
					DMibTestPath("Rec2020Brightness");
					auto Matrix = CColorPrimaries::fs_Rec2020().f_GetMatrixToXYZ();

					DMibExpect(fg_CountMismatches(Matrix.m_Rows[1], 0.2627, 0.6780, 0.0593, gc_PublishedTolerance), ==, 0);
				}

				{
					// Rec. 2087
					DMibTestPath("Rec709ToRec2020");
					constexpr pfp64 c_Expected[3][3] =
						{
							{0.6274, 0.3293, 0.0433}
							, {0.0691, 0.9195, 0.0114}
							, {0.0164, 0.0880, 0.8956}
						}
					;

					DMibExpect(fg_CountMismatches(fg_GetColorMatrix(CColorPrimaries::fs_Rec709(), CColorPrimaries::fs_Rec2020()), c_Expected, gc_PublishedTolerance), ==, 0);
				}

				{
					DMibTestPath("SamePrimaries");
					pfp64 const c_Identity[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};

					DMibExpect(fg_CountMismatches(fg_GetColorMatrix(CColorPrimaries::fs_DisplayP3(), CColorPrimaries::fs_DisplayP3()), c_Identity, 0.0), ==, 0);
				}

				{
					DMibTestPath("ThereAndBack");
					pfp64 const c_Identity[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};

					CColorPrimaries const c_Primaries[] =
						{
							CColorPrimaries::fs_Rec709()
							, CColorPrimaries::fs_DisplayP3()
							, CColorPrimaries::fs_Rec2020()
							, CColorPrimaries::fs_AdobeRGB()
							, CColorPrimaries::fs_AcesAP1()
						}
					;

					umint nMismatches = 0;
					umint nWhiteMismatches = 0;
					for (auto &From : c_Primaries)
					{
						for (auto &To : c_Primaries)
						{
							auto There = fg_GetColorMatrix(From, To);
							auto Back = fg_GetColorMatrix(To, From);

							nMismatches += fg_CountMismatches(Back * There, c_Identity, gc_ExactTolerance);

							// White is white with every primaries, also where the white points differ
							pfp64 White[3] = {1.0, 1.0, 1.0};
							There.f_Transform(White, White);
							nWhiteMismatches += fg_CountMismatches(White, 1.0, 1.0, 1.0, gc_ExactTolerance);
						}
					}

					DMibExpect(nMismatches, ==, 0);
					DMibExpect(nWhiteMismatches, ==, 0);
				}

				{
					DMibTestPath("Inverse");
					pfp64 const c_Identity[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
					auto Matrix = CColorPrimaries::fs_AdobeRGB().f_GetMatrixToXYZ();

					DMibExpect(fg_CountMismatches(Matrix * Matrix.f_Inverse(), c_Identity, gc_ExactTolerance), ==, 0);
				}

				{
					DMibTestPath("Contains");
					auto Rec709 = CColorPrimaries::fs_Rec709();
					auto DisplayP3 = CColorPrimaries::fs_DisplayP3();
					auto Rec2020 = CColorPrimaries::fs_Rec2020();
					auto AdobeRGB = CColorPrimaries::fs_AdobeRGB();

					DMibExpectTrue(Rec709.f_Contains(Rec709));
					DMibExpectTrue(DisplayP3.f_Contains(Rec709));
					DMibExpectTrue(Rec2020.f_Contains(Rec709));

					// The red of Display P3 is a little outside Rec. 2020
					DMibExpectFalse(Rec2020.f_Contains(DisplayP3));
					DMibExpectTrue(Rec2020.f_Contains(DisplayP3, 0.002));
					DMibExpectFalse(Rec709.f_Contains(DisplayP3, 0.002));

					DMibExpectTrue(Rec2020.f_Contains(AdobeRGB));
					DMibExpectTrue(AdobeRGB.f_Contains(Rec709));

					DMibExpectFalse(Rec709.f_Contains(DisplayP3));
					DMibExpectFalse(Rec709.f_Contains(Rec2020));
					DMibExpectFalse(DisplayP3.f_Contains(Rec2020));
					DMibExpectFalse(DisplayP3.f_Contains(AdobeRGB));
					DMibExpectFalse(AdobeRGB.f_Contains(DisplayP3));
				}

				{
					DMibTestPath("StoreRows");
					pfp32 Rows[9];
					fg_GetColorMatrix(CColorPrimaries::fs_Rec709(), CColorPrimaries::fs_Rec2020()).f_StoreRows(Rows);

					DMibExpectTrue(fg_Near(Rows[0], 0.6274, gc_PublishedTolerance));
					DMibExpectTrue(fg_Near(Rows[1], 0.3293, gc_PublishedTolerance));
					DMibExpectTrue(fg_Near(Rows[3], 0.0691, gc_PublishedTolerance));
					DMibExpectTrue(fg_Near(Rows[8], 0.8956, gc_PublishedTolerance));
				}
			};

			DMibTestSuite("Curves")
			{
				{
					DMibTestPath("sRGB");

					DMibExpectTrue(fg_Near(fg_ColorCurveToLinear(EColorCurve::mc_sRGB, 0.5), 0.21404114048223255, gc_ExactTolerance));
					DMibExpectTrue(fg_Near(fg_ColorCurveFromLinear(EColorCurve::mc_sRGB, 0.18), 0.46135612950044164, gc_ExactTolerance));
					DMibExpectTrue(fg_Near(fg_ColorCurveToLinear(EColorCurve::mc_sRGB, 0.04045), 0.04045 / 12.92, gc_ExactTolerance));
					DMibExpect(fg_ColorCurveToLinear(EColorCurve::mc_sRGB, 0.0), ==, 0.0);
					DMibExpectTrue(fg_Near(fg_ColorCurveToLinear(EColorCurve::mc_sRGB, 1.0), 1.0, gc_ExactTolerance));
				}

				{
					// Values below 0 and above 1 are what the extended color spaces have
					DMibTestPath("Extended");

					DMibExpectTrue(fg_Near(fg_ColorCurveToLinear(EColorCurve::mc_sRGB, -0.5), -0.21404114048223255, gc_ExactTolerance));
					DMibExpectTrue(fg_Near(fg_ColorCurveFromLinear(EColorCurve::mc_sRGB, -0.18), -0.46135612950044164, gc_ExactTolerance));
					DMibExpectTrue(fg_ColorCurveFromLinear(EColorCurve::mc_sRGB, 4.0) > 1.0);
					DMibExpect(fg_ColorCurveFromLinear(EColorCurve::mc_PQ, -1.0), ==, 0.0);
					DMibExpect(fg_ColorCurveFromLinear(EColorCurve::mc_HLG, -1.0), ==, 0.0);
				}

				{
					// The curve of Rec. 709 is not the one of sRGB
					DMibTestPath("Rec709");

					DMibExpectTrue(fg_Near(fg_ColorCurveFromLinear(EColorCurve::mc_Rec709, 0.18), 0.409, 1e-3));
					DMibExpectTrue(fg_Near(fg_ColorCurveFromLinear(EColorCurve::mc_Rec709, 0.018), 0.081, 1e-3));
					DMibExpectTrue(fg_Near(fg_ColorCurveFromLinear(EColorCurve::mc_Rec709, 1.0), 1.0, gc_ExactTolerance));
				}

				{
					DMibTestPath("Gamma");

					DMibExpectTrue(fg_Near(fg_ColorCurveToLinear(EColorCurve::mc_Gamma22, 0.5), 0.217637640824031, gc_ExactTolerance));
					DMibExpectTrue(fg_Near(fg_ColorCurveToLinear(EColorCurve::mc_Gamma24, 0.5), 0.189464570814653, gc_ExactTolerance));
				}

				{
					// Rec. 2100. The linear values are parts of 10000 nits
					DMibTestPath("PQ");

					DMibExpectTrue(fg_Near(fg_ColorCurveFromLinear(EColorCurve::mc_PQ, 0.01), 0.5080784, 1e-6));
					DMibExpectTrue(fg_Near(fg_ColorCurveFromLinear(EColorCurve::mc_PQ, 0.1), 0.7518271, 1e-6));
					DMibExpectTrue(fg_Near(fg_ColorCurveFromLinear(EColorCurve::mc_PQ, 1.0), 1.0, gc_ExactTolerance));
					DMibExpect(fg_ColorCurveToLinear(EColorCurve::mc_PQ, 0.0), ==, 0.0);
				}

				{
					// Rec. 2100
					DMibTestPath("HLG");

					DMibExpectTrue(fg_Near(fg_ColorCurveFromLinear(EColorCurve::mc_HLG, 1.0 / 12.0), 0.5, gc_ExactTolerance));
					DMibExpectTrue(fg_Near(fg_ColorCurveFromLinear(EColorCurve::mc_HLG, 1.0), 1.0, 1e-7));
					DMibExpectTrue(fg_Near(fg_ColorCurveToLinear(EColorCurve::mc_HLG, 0.5), 1.0 / 12.0, gc_ExactTolerance));
				}

				{
					DMibTestPath("ThereAndBack");
					constexpr EColorCurve c_Curves[] =
						{
							EColorCurve::mc_Linear
							, EColorCurve::mc_sRGB
							, EColorCurve::mc_Gamma22
							, EColorCurve::mc_Gamma24
							, EColorCurve::mc_Rec709
							, EColorCurve::mc_PQ
							, EColorCurve::mc_HLG
						}
					;

					umint nMismatches = 0;
					umint nNotGrowing = 0;
					for (auto Curve : c_Curves)
					{
						pfp64 Last = -1.0;
						for (umint iValue = 0; iValue <= 1000; ++iValue)
						{
							pfp64 Value = pfp64(iValue) / 1000.0;
							pfp64 Linear = fg_ColorCurveToLinear(Curve, Value);

							nMismatches += !fg_Near(fg_ColorCurveFromLinear(Curve, Linear), Value, 1e-9);
							nNotGrowing += !(Linear > Last);

							Last = Linear;
						}
					}

					DMibExpect(nMismatches, ==, 0);
					DMibExpect(nNotGrowing, ==, 0);
				}
			};

			DMibTestSuite("Files")
			{
				// The numbers are the ones of profiles that are in use, with 16 bits after the point
				constexpr uint32 c_ToD50[9] = {0x10c42, 0x5de, 0xfffff326, 0x793, 0xfd90, 0xfffffba2, 0xfffffda3, 0x3dc, 0xc06e};
				constexpr uint32 c_WhiteD50[3] = {0xf6d5, 0x10000, 0xd32c};
				constexpr uint32 c_WhiteD65[3] = {0xf351, 0x10000, 0x116cc};

				constexpr uint32 c_DisplayP3[9] = {0x83df, 0x3dbf, 0xffffffbb, 0x4abf, 0xb137, 0xab9, 0x2838, 0x110b, 0xc8b9};
				constexpr uint32 c_Rec2020[9] = {0xac69, 0x476f, 0xffffff81, 0x2a69, 0xace3, 0x7ad, 0x2003, 0xbad, 0xcbfe};
				constexpr uint32 c_AdobeRGB[9] = {0x9c18, 0x4fa5, 0x4fc, 0x348d, 0xa02c, 0xf95, 0x2631, 0x102f, 0xbe9c};
				constexpr uint32 c_sRGB[9] = {0x6fa2, 0x38f5, 0x390, 0x6299, 0xb785, 0x18da, 0x24a0, 0xf84, 0xb6cf};

				// The curve with parameters of type 3 for sRGB and for Rec. 709
				constexpr uint32 c_CurveOfSRGB[5] = {0x26666, 0xf2a7, 0xd59, 0x13d0, 0xa5b};
				constexpr uint32 c_CurveOfRec709[5] = {0x238e4, 0xe8e0, 0x1720, 0x38e4, 0x14bc};

				auto fGetCurveWithParameters = [](uint32 const *_pParameters) -> TCVector<uint8>
					{
						TCVector<uint8> Curve;
						fg_AddUInt32(Curve, fg_Signature("para"));
						fg_AddUInt32(Curve, 0);
						fg_AddUInt32(Curve, 0x30000);
						for (umint i = 0; i < 5; ++i)
							fg_AddUInt32(Curve, _pParameters[i]);

						return Curve;
					}
				;

				auto fGetCurveWithGamma = [](uint16 _Gamma) -> TCVector<uint8>
					{
						TCVector<uint8> Curve;
						fg_AddUInt32(Curve, fg_Signature("curv"));
						fg_AddUInt32(Curve, 0);
						fg_AddUInt32(Curve, 1);
						Curve.f_Insert(uint8(_Gamma >> 8));
						Curve.f_Insert(uint8(_Gamma));

						return Curve;
					}
				;

				auto fGetCurveWithTable = [](EColorCurve _Curve, pfp64 _Power) -> TCVector<uint8>
					{
						constexpr uint32 c_nValues = 1024;

						TCVector<uint8> Curve;
						fg_AddUInt32(Curve, fg_Signature("curv"));
						fg_AddUInt32(Curve, 0);
						fg_AddUInt32(Curve, c_nValues);
						for (uint32 i = 0; i < c_nValues; ++i)
						{
							pfp64 Linear = __builtin_pow(fg_ColorCurveToLinear(_Curve, pfp64(i) / pfp64(c_nValues - 1)), _Power);
							uint16 Value = uint16(Linear * 65535.0 + 0.5);
							Curve.f_Insert(uint8(Value >> 8));
							Curve.f_Insert(uint8(Value));
						}

						return Curve;
					}
				;

				{
					DMibTestPath("DisplayP3");
					auto Profile = fg_CreateProfile(c_DisplayP3, c_WhiteD50, c_ToD50, fGetCurveWithParameters(c_CurveOfSRGB));

					CColorSpace ColorSpace = CColorSpace::fs_Rec2020();
					DMibExpectTrue(fg_GetColorSpaceFromProfile(Profile.f_GetArray(), Profile.f_GetLen(), ColorSpace));
					DMibExpectTrue(ColorSpace == CColorSpace::fs_DisplayP3());
				}

				{
					DMibTestPath("Rec2020");
					auto Profile = fg_CreateProfile(c_Rec2020, c_WhiteD50, c_ToD50, fGetCurveWithParameters(c_CurveOfRec709));

					CColorSpace ColorSpace;
					DMibExpectTrue(fg_GetColorSpaceFromProfile(Profile.f_GetArray(), Profile.f_GetLen(), ColorSpace));
					DMibExpectTrue(ColorSpace == CColorSpace::fs_Rec2020());
				}

				{
					// A profile that has the white of the color space, and no matrix to D50
					DMibTestPath("AdobeRGB");
					auto Profile = fg_CreateProfile(c_AdobeRGB, c_WhiteD65, nullptr, fGetCurveWithGamma(0x233));

					CColorSpace ColorSpace;
					DMibExpectTrue(fg_GetColorSpaceFromProfile(Profile.f_GetArray(), Profile.f_GetLen(), ColorSpace));
					DMibExpectTrue(ColorSpace.m_Primaries == CColorPrimaries::fs_AdobeRGB());
					DMibExpectTrue(ColorSpace.m_Curve == EColorCurve::mc_Gamma);
					DMibExpect(ColorSpace.m_Gamma, ==, 563.0 / 256.0);
				}

				{
					DMibTestPath("CurveWithATable");
					auto Profile = fg_CreateProfile(c_sRGB, c_WhiteD65, nullptr, fGetCurveWithTable(EColorCurve::mc_sRGB, 1.0));

					CColorSpace ColorSpace = CColorSpace::fs_Rec2020();
					DMibExpectTrue(fg_GetColorSpaceFromProfile(Profile.f_GetArray(), Profile.f_GetLen(), ColorSpace));
					DMibExpectTrue(ColorSpace == CColorSpace::fs_sRGB());
				}

				{
					DMibTestPath("CurveThatIsNotKnown");
					auto Profile = fg_CreateProfile(c_sRGB, c_WhiteD65, nullptr, fGetCurveWithTable(EColorCurve::mc_sRGB, 1.5));

					CColorSpace ColorSpace = CColorSpace::fs_Rec2020();
					DMibExpectFalse(fg_GetColorSpaceFromProfile(Profile.f_GetArray(), Profile.f_GetLen(), ColorSpace));
					DMibExpectTrue(ColorSpace == CColorSpace::fs_Rec2020());
				}

				{
					DMibTestPath("NotWhole");
					auto Profile = fg_CreateProfile(c_DisplayP3, c_WhiteD50, c_ToD50, fGetCurveWithParameters(c_CurveOfSRGB));

					CColorSpace ColorSpace;
					umint nAccepted = 0;
					for (umint nBytes = 0; nBytes < Profile.f_GetLen(); ++nBytes)
						nAccepted += fg_GetColorSpaceFromProfile(Profile.f_GetArray(), nBytes, ColorSpace);

					DMibExpect(nAccepted, ==, 0);
					DMibExpectFalse(fg_GetColorSpaceFromProfile(nullptr, 0, ColorSpace));
				}

				{
					DMibTestPath("NotRGB");
					auto Profile = fg_CreateProfile(c_DisplayP3, c_WhiteD50, c_ToD50, fGetCurveWithParameters(c_CurveOfSRGB));
					Profile[16] = 'C';
					Profile[17] = 'M';
					Profile[18] = 'Y';
					Profile[19] = 'K';

					CColorSpace ColorSpace;
					DMibExpectFalse(fg_GetColorSpaceFromProfile(Profile.f_GetArray(), Profile.f_GetLen(), ColorSpace));
				}

				{
					// What every byte of a profile can be does not read outside the profile
					DMibTestPath("Bytes");
					auto Profile = fg_CreateProfile(c_DisplayP3, c_WhiteD50, c_ToD50, fGetCurveWithParameters(c_CurveOfSRGB));

					CColorSpace ColorSpace;
					umint nAccepted = 0;
					for (umint iByte = 128; iByte < Profile.f_GetLen(); ++iByte)
					{
						for (uint8 Value : {uint8(0), uint8(0x7f), uint8(0xff)})
						{
							auto Changed = Profile;
							Changed[iByte] = Value;
							nAccepted += fg_GetColorSpaceFromProfile(Changed.f_GetArray(), Changed.f_GetLen(), ColorSpace);
						}
					}

					DMibExpect(nAccepted, >, 0);
				}

				{
					DMibTestPath("CodingIndependent");
					CColorSpace ColorSpace;

					DMibExpectTrue(fg_GetColorSpaceFromCodingIndependent(1, 13, ColorSpace));
					DMibExpectTrue(ColorSpace == CColorSpace::fs_sRGB());
					DMibExpectTrue(fg_GetColorSpaceFromCodingIndependent(12, 13, ColorSpace));
					DMibExpectTrue(ColorSpace == CColorSpace::fs_DisplayP3());
					DMibExpectTrue(fg_GetColorSpaceFromCodingIndependent(9, 16, ColorSpace));
					DMibExpectTrue(ColorSpace == CColorSpace::fs_Rec2100PQ());
					DMibExpectTrue(fg_GetColorSpaceFromCodingIndependent(9, 18, ColorSpace));
					DMibExpectTrue(ColorSpace == CColorSpace::fs_Rec2100HLG());
					DMibExpectTrue(fg_GetColorSpaceFromCodingIndependent(9, 14, ColorSpace));
					DMibExpectTrue(ColorSpace == CColorSpace::fs_Rec2020());
					DMibExpectTrue(fg_GetColorSpaceFromCodingIndependent(1, 8, ColorSpace));
					DMibExpectTrue(ColorSpace == CColorSpace::fs_Linear(CColorPrimaries::fs_Rec709()));

					DMibExpectFalse(fg_GetColorSpaceFromCodingIndependent(2, 13, ColorSpace));
					DMibExpectFalse(fg_GetColorSpaceFromCodingIndependent(1, 2, ColorSpace));

					{
						DMibTestPath("NotChanged");
						DMibExpectTrue(ColorSpace == CColorSpace::fs_Linear(CColorPrimaries::fs_Rec709()));
					}
				}

				{
					DMibTestPath("Chromaticities");

					// What PNG has for Display P3, which is rounded to five digits
					pfp64 const c_Chromaticities[8] = {0.3127, 0.329, 0.68, 0.32, 0.265, 0.69, 0.15, 0.06};

					auto DisplayP3 = fg_GetColorSpaceFromChromaticities(c_Chromaticities, 0.45455);
					DMibExpectTrue(DisplayP3 == CColorSpace::fs_DisplayP3());

					auto Gamma = fg_GetColorSpaceFromChromaticities(nullptr, 0.5);
					DMibExpectTrue(Gamma.m_Primaries == CColorPrimaries::fs_Rec709());
					DMibExpectTrue(Gamma.m_Curve == EColorCurve::mc_Gamma);
					DMibExpect(Gamma.m_Gamma, ==, 2.0);

					auto Linear = fg_GetColorSpaceFromChromaticities(nullptr, 1.0);
					DMibExpectTrue(Linear.m_Curve == EColorCurve::mc_Linear);

					pfp64 const c_Other[8] = {0.3127, 0.329, 0.7, 0.3, 0.2, 0.7, 0.1, 0.1};
					auto Other = fg_GetColorSpaceFromChromaticities(c_Other, 0.0);
					DMibExpect(Other.m_Primaries.m_Red.m_X, ==, 0.7);
					DMibExpectTrue(Other.m_Curve == EColorCurve::mc_sRGB);
				}

				{
					DMibTestPath("Gamma");

					DMibExpectTrue(fg_Near(fg_ColorCurveToLinear(EColorCurve::mc_Gamma, 0.5, 2.0), 0.25, gc_ExactTolerance));
					DMibExpectTrue(fg_Near(fg_ColorCurveFromLinear(EColorCurve::mc_Gamma, 0.25, 2.0), 0.5, gc_ExactTolerance));
					DMibExpectTrue(fg_Near(fg_ColorCurveToLinear(EColorCurve::mc_Gamma, -0.5, 2.0), -0.25, gc_ExactTolerance));

					CColorSpace ColorSpace;
					ColorSpace.m_Curve = EColorCurve::mc_Gamma;
					ColorSpace.m_Gamma = 2.0;

					pfp64 Values[3] = {0.5, 0.5, 0.5};
					fg_ConvertColor(ColorSpace, CColorSpace::fs_Linear(CColorPrimaries::fs_Rec709()), Values, Values);
					DMibExpect(fg_CountMismatches(Values, 0.25, 0.25, 0.25, gc_ExactTolerance), ==, 0);
				}
			};

			DMibTestSuite("Colors")
			{
				{
					// The red of sRGB in Display P3, which has the curve of sRGB
					DMibTestPath("RedToDisplayP3");
					CColor Red(1.0f, 0.0f, 0.0f, 0.5f, CColorSpace::fs_sRGB());
					CColor Converted = Red.f_ConvertTo(CColorSpace::fs_DisplayP3());

					DMibExpectTrue(fg_Near(Converted.m_Red, 0.9175, 1e-3));
					DMibExpectTrue(fg_Near(Converted.m_Green, 0.2003, 1e-3));
					DMibExpectTrue(fg_Near(Converted.m_Blue, 0.1386, 1e-3));
					DMibExpect(Converted.m_Alpha, ==, 0.5f);
					DMibExpectTrue(Converted.m_Space == CColorSpace::fs_DisplayP3());
				}

				{
					// A color that the primaries do not have has values below 0
					DMibTestPath("OutsideThePrimaries");
					CColor Green(0.0f, 1.0f, 0.0f, 1.0f, CColorSpace::fs_DisplayP3());
					CColor Converted = Green.f_ConvertTo(CColorSpace::fs_Linear(CColorPrimaries::fs_Rec709()));

					DMibExpectTrue(Converted.m_Red < 0.0f);
					DMibExpectTrue(Converted.m_Green > 1.0f);
					DMibExpectTrue(Converted.m_Blue < 0.0f);
				}

				{
					DMibTestPath("Gray");
					pfp64 Gray[3] = {0.5, 0.5, 0.5};
					fg_ConvertColor(CColorSpace::fs_sRGB(), CColorSpace::fs_Rec2020(), Gray, Gray);

					// Gray stays gray, with the curve of Rec. 2020
					pfp64 Expected = fg_ColorCurveFromLinear(EColorCurve::mc_Rec709, 0.21404114048223255);
					DMibExpect(fg_CountMismatches(Gray, Expected, Expected, Expected, gc_ExactTolerance), ==, 0);
				}

				{
					DMibTestPath("ThereAndBack");
					CColorSpace const c_Spaces[] =
						{
							CColorSpace::fs_sRGB()
							, CColorSpace::fs_DisplayP3()
							, CColorSpace::fs_Rec2020()
							, CColorSpace::fs_Linear(CColorPrimaries::fs_Rec709())
							, CColorSpace::fs_Linear(CColorPrimaries::fs_AcesAP1())
						}
					;

					umint nMismatches = 0;
					for (auto &From : c_Spaces)
					{
						for (auto &To : c_Spaces)
						{
							pfp64 Values[3] = {0.25, 0.5, 0.75};
							fg_ConvertColor(From, To, Values, Values);
							fg_ConvertColor(To, From, Values, Values);

							nMismatches += fg_CountMismatches(Values, 0.25, 0.5, 0.75, 1e-9);
						}
					}

					DMibExpect(nMismatches, ==, 0);
				}
			};
		}
	};

	DMibTestRegister(CColor_Tests, Malterlib::Graphics::ColorSpace);
}
