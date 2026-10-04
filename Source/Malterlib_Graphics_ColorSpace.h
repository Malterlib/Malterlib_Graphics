// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <Mib/Core/Core>

// A color space is primaries with a white point, and a curve. The two do not depend on each other. Conversions are
// calculated in pfp64 when something is set up, and are not for what is done for each pixel.

namespace NMib::NGraphics
{
	// A point in the chromaticity diagram of CIE 1931.
	struct CColorChromaticity
	{
		bool operator == (CColorChromaticity const &_Right) const = default;

		pfp64 m_X = 0.0;
		pfp64 m_Y = 0.0;
	};

	struct CColorMatrix
	{
		static CColorMatrix fs_Identity();

		CColorMatrix operator * (CColorMatrix const &_Right) const;

		void f_Transform(pfp64 const *_pSource, pfp64 *o_pDestination) const;
		CColorMatrix f_Inverse() const;
		void f_StoreRows(pfp32 *o_pDestination) const;

		pfp64 m_Rows[3][3] = {};
	};

	struct CColorPrimaries
	{
		bool operator == (CColorPrimaries const &_Right) const = default;

		static CColorPrimaries fs_Rec709();
		static CColorPrimaries fs_DisplayP3();
		static CColorPrimaries fs_Rec2020();
		static CColorPrimaries fs_AdobeRGB();
		static CColorPrimaries fs_AcesAP1();

		CColorMatrix f_GetMatrixToXYZ() const;
		bool f_Contains(CColorPrimaries const &_Other, pfp64 _Tolerance = 1e-6) const;

		CColorChromaticity m_Red;
		CColorChromaticity m_Green;
		CColorChromaticity m_Blue;
		CColorChromaticity m_White;
	};

	enum class EColorCurve : uint8
	{
		mc_Linear
		, mc_sRGB
		, mc_Gamma22
		, mc_Gamma24
		, mc_Gamma // The power that the color space says
		, mc_Rec709
		, mc_PQ // Linear values are parts of 10000 nits
		, mc_HLG // Linear values are the light of the scene, where 1 is the most that is encoded
	};

	struct CColorSpace
	{
		bool operator == (CColorSpace const &_Right) const = default;

		static CColorSpace fs_sRGB();
		static CColorSpace fs_DisplayP3();
		static CColorSpace fs_Rec2020();
		static CColorSpace fs_Rec2100PQ();
		static CColorSpace fs_Rec2100HLG();
		static CColorSpace fs_Linear(CColorPrimaries const &_Primaries);

		CColorPrimaries m_Primaries = CColorPrimaries::fs_Rec709();
		EColorCurve m_Curve = EColorCurve::mc_sRGB;
		pfp64 m_Gamma = 2.2; // The power of mc_Gamma, which the linear values are the values of the curve to
	};

	struct CColor
	{
		CColor() = default;
		CColor(pfp32 _Red, pfp32 _Green, pfp32 _Blue, pfp32 _Alpha, CColorSpace const &_Space);

		CColor f_ConvertTo(CColorSpace const &_Space) const;

		pfp32 m_Red = 0.0f;
		pfp32 m_Green = 0.0f;
		pfp32 m_Blue = 0.0f;
		pfp32 m_Alpha = 1.0f; // Is not changed by conversions
		CColorSpace m_Space;
	};

	CColorMatrix fg_GetColorMatrix(CColorPrimaries const &_From, CColorPrimaries const &_To);
	pfp64 fg_ColorCurveToLinear(EColorCurve _Curve, pfp64 _Value, pfp64 _Gamma = 2.2);
	pfp64 fg_ColorCurveFromLinear(EColorCurve _Curve, pfp64 _Value, pfp64 _Gamma = 2.2);
	void fg_ConvertColor(CColorSpace const &_From, CColorSpace const &_To, pfp64 const *_pSource, pfp64 *o_pDestination);

	CColorPrimaries fg_GetKnownPrimaries(CColorPrimaries const &_Primaries);
	bool fg_GetColorSpaceFromCodingIndependent(uint8 _Primaries, uint8 _Transfer, CColorSpace &o_ColorSpace);
	bool fg_GetColorSpaceFromProfile(uint8 const *_pProfile, umint _nBytes, CColorSpace &o_ColorSpace);
	CColorSpace fg_GetColorSpaceFromChromaticities(pfp64 const *_pChromaticities, pfp64 _Gamma);
}
