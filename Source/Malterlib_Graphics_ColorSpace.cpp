// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "Malterlib_Graphics_ColorSpace.h"

#include <Mib/Container/Vector>

namespace
{
	using namespace NMib;
	using namespace NMib::NGraphics;

	constexpr CColorChromaticity gc_WhiteD65 = {0.3127, 0.3290};

	// The constants of the curve of Rec. 709 with the digits that make its two parts meet, from Rec. 2020
	constexpr pfp64 gc_Rec709Alpha = 1.09929682680944;
	constexpr pfp64 gc_Rec709Beta = 0.018053968510807;

	constexpr pfp64 gc_PQ_M1 = 2610.0 / 16384.0;
	constexpr pfp64 gc_PQ_M2 = 2523.0 / 4096.0 * 128.0;
	constexpr pfp64 gc_PQ_C1 = 3424.0 / 4096.0;
	constexpr pfp64 gc_PQ_C2 = 2413.0 / 4096.0 * 32.0;
	constexpr pfp64 gc_PQ_C3 = 2392.0 / 4096.0 * 32.0;

	constexpr pfp64 gc_HLG_A = 0.17883277;
	constexpr pfp64 gc_HLG_B = 1.0 - 4.0 * gc_HLG_A;

	// 0.5 - gc_HLG_A * ln(4 * gc_HLG_A)
	constexpr pfp64 gc_HLG_C = 0.55991072953;

	// How much the cones of the eye respond to a color of CIE XYZ, of the Bradford adaptation
	constexpr CColorMatrix gc_Bradford =
		{
			{
				{0.8951, 0.2664, -0.1614}
				, {-0.7502, 1.7135, 0.0367}
				, {0.0389, -0.0685, 1.0296}
			}
		}
	;

	void fg_GetXYZ(CColorChromaticity const &_Chromaticity, pfp64 *o_pXYZ)
	{
		o_pXYZ[0] = _Chromaticity.m_X / _Chromaticity.m_Y;
		o_pXYZ[1] = 1.0;
		o_pXYZ[2] = (1.0 - _Chromaticity.m_X - _Chromaticity.m_Y) / _Chromaticity.m_Y;
	}

	// Returns the matrix that makes a color of CIE XYZ that is seen with one white look the same with another.
	CColorMatrix fg_GetAdaptation(CColorChromaticity const &_From, CColorChromaticity const &_To)
	{
		if (_From == _To)
			return CColorMatrix::fs_Identity();

		pfp64 From[3];
		pfp64 To[3];
		fg_GetXYZ(_From, From);
		fg_GetXYZ(_To, To);

		pfp64 FromCones[3];
		pfp64 ToCones[3];
		gc_Bradford.f_Transform(From, FromCones);
		gc_Bradford.f_Transform(To, ToCones);

		CColorMatrix Scale;
		for (umint i = 0; i < 3; ++i)
			Scale.m_Rows[i][i] = ToCones[i] / FromCones[i];

		return gc_Bradford.f_Inverse() * Scale * gc_Bradford;
	}

	// The curves with a power are the same for values below 0 as above, with the sign kept, which is what the
	// extended color spaces have.
	template <typename tf_FCurve>
	pfp64 fg_WithSign(pfp64 _Value, tf_FCurve &&_fCurve)
	{
		if (_Value < 0.0)
			return -_fCurve(-_Value);

		return _fCurve(_Value);
	}
}

namespace NMib::NGraphics
{
	// CColorMatrix

	CColorMatrix CColorMatrix::fs_Identity()
	{
		CColorMatrix Result;
		for (umint i = 0; i < 3; ++i)
			Result.m_Rows[i][i] = 1.0;

		return Result;
	}

	// The result transforms with _Right first.
	CColorMatrix CColorMatrix::operator * (CColorMatrix const &_Right) const
	{
		CColorMatrix Result;
		for (umint iRow = 0; iRow < 3; ++iRow)
		{
			for (umint iColumn = 0; iColumn < 3; ++iColumn)
			{
				pfp64 Sum = 0.0;
				for (umint i = 0; i < 3; ++i)
					Sum += m_Rows[iRow][i] * _Right.m_Rows[i][iColumn];

				Result.m_Rows[iRow][iColumn] = Sum;
			}
		}

		return Result;
	}

	// _pSource and o_pDestination have three values, and can be the same.
	void CColorMatrix::f_Transform(pfp64 const *_pSource, pfp64 *o_pDestination) const
	{
		pfp64 Source[3] = {_pSource[0], _pSource[1], _pSource[2]};

		for (umint iRow = 0; iRow < 3; ++iRow)
			o_pDestination[iRow] = m_Rows[iRow][0] * Source[0] + m_Rows[iRow][1] * Source[1] + m_Rows[iRow][2] * Source[2];
	}

	// The matrix must have an inverse, which the matrices of primaries that are a triangle have.
	CColorMatrix CColorMatrix::f_Inverse() const
	{
		auto &Rows = m_Rows;

		CColorMatrix Cofactors;
		for (umint iRow = 0; iRow < 3; ++iRow)
		{
			umint iRow0 = (iRow + 1) % 3;
			umint iRow1 = (iRow + 2) % 3;

			for (umint iColumn = 0; iColumn < 3; ++iColumn)
			{
				umint iColumn0 = (iColumn + 1) % 3;
				umint iColumn1 = (iColumn + 2) % 3;

				Cofactors.m_Rows[iRow][iColumn] = Rows[iRow0][iColumn0] * Rows[iRow1][iColumn1] - Rows[iRow0][iColumn1] * Rows[iRow1][iColumn0];
			}
		}

		pfp64 Determinant = Rows[0][0] * Cofactors.m_Rows[0][0] + Rows[0][1] * Cofactors.m_Rows[0][1] + Rows[0][2] * Cofactors.m_Rows[0][2];

		CColorMatrix Result;
		for (umint iRow = 0; iRow < 3; ++iRow)
		{
			for (umint iColumn = 0; iColumn < 3; ++iColumn)
				Result.m_Rows[iRow][iColumn] = Cofactors.m_Rows[iColumn][iRow] / Determinant;
		}

		return Result;
	}

	// Writes nine values, a row at a time.
	void CColorMatrix::f_StoreRows(pfp32 *o_pDestination) const
	{
		for (umint iRow = 0; iRow < 3; ++iRow)
		{
			for (umint iColumn = 0; iColumn < 3; ++iColumn)
				o_pDestination[iRow * 3 + iColumn] = pfp32(m_Rows[iRow][iColumn]);
		}
	}

	// CColorPrimaries

	CColorPrimaries CColorPrimaries::fs_Rec709()
	{
		return {{0.64, 0.33}, {0.30, 0.60}, {0.15, 0.06}, gc_WhiteD65};
	}

	CColorPrimaries CColorPrimaries::fs_DisplayP3()
	{
		return {{0.680, 0.320}, {0.265, 0.690}, {0.150, 0.060}, gc_WhiteD65};
	}

	CColorPrimaries CColorPrimaries::fs_Rec2020()
	{
		return {{0.708, 0.292}, {0.170, 0.797}, {0.131, 0.046}, gc_WhiteD65};
	}

	CColorPrimaries CColorPrimaries::fs_AdobeRGB()
	{
		return {{0.64, 0.33}, {0.21, 0.71}, {0.15, 0.06}, gc_WhiteD65};
	}

	CColorPrimaries CColorPrimaries::fs_AcesAP1()
	{
		return {{0.713, 0.293}, {0.165, 0.830}, {0.128, 0.044}, {0.32168, 0.33767}};
	}

	// Returns the matrix from linear values with the primaries to CIE XYZ, where white has the brightness 1.
	CColorMatrix CColorPrimaries::f_GetMatrixToXYZ() const
	{
		pfp64 Primaries[3][3];
		fg_GetXYZ(m_Red, Primaries[0]);
		fg_GetXYZ(m_Green, Primaries[1]);
		fg_GetXYZ(m_Blue, Primaries[2]);

		CColorMatrix Unscaled;
		for (umint iRow = 0; iRow < 3; ++iRow)
		{
			for (umint iColumn = 0; iColumn < 3; ++iColumn)
				Unscaled.m_Rows[iRow][iColumn] = Primaries[iColumn][iRow];
		}

		// How much of each primary there is in white
		pfp64 White[3];
		fg_GetXYZ(m_White, White);

		pfp64 Scale[3];
		Unscaled.f_Inverse().f_Transform(White, Scale);

		CColorMatrix Result;
		for (umint iRow = 0; iRow < 3; ++iRow)
		{
			for (umint iColumn = 0; iColumn < 3; ++iColumn)
				Result.m_Rows[iRow][iColumn] = Unscaled.m_Rows[iRow][iColumn] * Scale[iColumn];
		}

		return Result;
	}

	// Returns if every color that _Other has is a color with values that are not below 0 with these primaries.
	// _Tolerance is how far below 0 a linear value of a primary of _Other can be, where white is 1. The default
	// is what the digits of the chromaticities of the standards give, for primaries that are shared. The red of
	// Display P3 is 0.0012 below 0 with Rec. 2020.
	bool CColorPrimaries::f_Contains(CColorPrimaries const &_Other, pfp64 _Tolerance) const
	{
		if (*this == _Other)
			return true;

		CColorMatrix Matrix = fg_GetColorMatrix(_Other, *this);
		for (umint iPrimary = 0; iPrimary < 3; ++iPrimary)
		{
			for (umint iRow = 0; iRow < 3; ++iRow)
			{
				if (Matrix.m_Rows[iRow][iPrimary] < -_Tolerance)
					return false;
			}
		}

		return true;
	}

	// CColorSpace

	CColorSpace CColorSpace::fs_sRGB()
	{
		return {CColorPrimaries::fs_Rec709(), EColorCurve::mc_sRGB};
	}

	CColorSpace CColorSpace::fs_DisplayP3()
	{
		return {CColorPrimaries::fs_DisplayP3(), EColorCurve::mc_sRGB};
	}

	CColorSpace CColorSpace::fs_Rec2020()
	{
		return {CColorPrimaries::fs_Rec2020(), EColorCurve::mc_Rec709};
	}

	CColorSpace CColorSpace::fs_Rec2100PQ()
	{
		return {CColorPrimaries::fs_Rec2020(), EColorCurve::mc_PQ};
	}

	CColorSpace CColorSpace::fs_Rec2100HLG()
	{
		return {CColorPrimaries::fs_Rec2020(), EColorCurve::mc_HLG};
	}

	CColorSpace CColorSpace::fs_Linear(CColorPrimaries const &_Primaries)
	{
		return {_Primaries, EColorCurve::mc_Linear};
	}

	// CColor

	CColor::CColor(pfp32 _Red, pfp32 _Green, pfp32 _Blue, pfp32 _Alpha, CColorSpace const &_Space)
		: m_Red(_Red)
		, m_Green(_Green)
		, m_Blue(_Blue)
		, m_Alpha(_Alpha)
		, m_Space(_Space)
	{
	}

	CColor CColor::f_ConvertTo(CColorSpace const &_Space) const
	{
		pfp64 Values[3] = {m_Red, m_Green, m_Blue};
		fg_ConvertColor(m_Space, _Space, Values, Values);

		return CColor(pfp32(Values[0]), pfp32(Values[1]), pfp32(Values[2]), m_Alpha, _Space);
	}

	// Returns the matrix from linear values with the primaries of _From to linear values with the primaries of _To.
	// Where the white points differ, a color looks the same with the white of _To as it did with the white of
	// _From.
	CColorMatrix fg_GetColorMatrix(CColorPrimaries const &_From, CColorPrimaries const &_To)
	{
		if (_From == _To)
			return CColorMatrix::fs_Identity();

		return _To.f_GetMatrixToXYZ().f_Inverse() * fg_GetAdaptation(_From.m_White, _To.m_White) * _From.f_GetMatrixToXYZ();
	}

	// Returns the linear value of a value that has the curve. The curves with a power keep the sign of values
	// below 0. PQ and HLG have no values below 0, which give 0. _Gamma is for mc_Gamma.
	pfp64 fg_ColorCurveToLinear(EColorCurve _Curve, pfp64 _Value, pfp64 _Gamma)
	{
		switch (_Curve)
		{
		case EColorCurve::mc_Linear:
			return _Value;
		case EColorCurve::mc_sRGB:
			return fg_WithSign
				(
					_Value
					, [](pfp64 _Value) -> pfp64
					{
						if (_Value <= 0.04045)
							return _Value / 12.92;

						return __builtin_pow((_Value + 0.055) / 1.055, 2.4);
					}
				)
			;
		case EColorCurve::mc_Gamma22:
			return fg_WithSign
				(
					_Value
					, [](pfp64 _Value) -> pfp64
					{
						return __builtin_pow(_Value, 2.2);
					}
				)
			;
		case EColorCurve::mc_Gamma24:
			return fg_WithSign
				(
					_Value
					, [](pfp64 _Value) -> pfp64
					{
						return __builtin_pow(_Value, 2.4);
					}
				)
			;
		case EColorCurve::mc_Gamma:
			return fg_WithSign
				(
					_Value
					, [&](pfp64 _Value) -> pfp64
					{
						return __builtin_pow(_Value, _Gamma);
					}
				)
			;
		case EColorCurve::mc_Rec709:
			return fg_WithSign
				(
					_Value
					, [](pfp64 _Value) -> pfp64
					{
						if (_Value < 4.5 * gc_Rec709Beta)
							return _Value / 4.5;

						return __builtin_pow((_Value + gc_Rec709Alpha - 1.0) / gc_Rec709Alpha, 1.0 / 0.45);
					}
				)
			;
		case EColorCurve::mc_PQ:
			{
				if (!(_Value > 0.0))
					return 0.0;

				pfp64 Power = __builtin_pow(_Value, 1.0 / gc_PQ_M2);
				pfp64 Numerator = Power - gc_PQ_C1;
				if (!(Numerator > 0.0))
					return 0.0;

				return __builtin_pow(Numerator / (gc_PQ_C2 - gc_PQ_C3 * Power), 1.0 / gc_PQ_M1);
			}
		case EColorCurve::mc_HLG:
			{
				if (!(_Value > 0.0))
					return 0.0;

				if (_Value <= 0.5)
					return _Value * _Value / 3.0;

				return (__builtin_exp((_Value - gc_HLG_C) / gc_HLG_A) + gc_HLG_B) / 12.0;
			}
		}

		return _Value;
	}

	// Returns the value with the curve of a linear value. See fg_ColorCurveToLinear.
	pfp64 fg_ColorCurveFromLinear(EColorCurve _Curve, pfp64 _Value, pfp64 _Gamma)
	{
		switch (_Curve)
		{
		case EColorCurve::mc_Linear:
			return _Value;
		case EColorCurve::mc_sRGB:
			return fg_WithSign
				(
					_Value
					, [](pfp64 _Value) -> pfp64
					{
						if (_Value <= 0.0031308)
							return _Value * 12.92;

						return 1.055 * __builtin_pow(_Value, 1.0 / 2.4) - 0.055;
					}
				)
			;
		case EColorCurve::mc_Gamma22:
			return fg_WithSign
				(
					_Value
					, [](pfp64 _Value) -> pfp64
					{
						return __builtin_pow(_Value, 1.0 / 2.2);
					}
				)
			;
		case EColorCurve::mc_Gamma24:
			return fg_WithSign
				(
					_Value
					, [](pfp64 _Value) -> pfp64
					{
						return __builtin_pow(_Value, 1.0 / 2.4);
					}
				)
			;
		case EColorCurve::mc_Gamma:
			return fg_WithSign
				(
					_Value
					, [&](pfp64 _Value) -> pfp64
					{
						return __builtin_pow(_Value, 1.0 / _Gamma);
					}
				)
			;
		case EColorCurve::mc_Rec709:
			return fg_WithSign
				(
					_Value
					, [](pfp64 _Value) -> pfp64
					{
						if (_Value < gc_Rec709Beta)
							return _Value * 4.5;

						return gc_Rec709Alpha * __builtin_pow(_Value, 0.45) - (gc_Rec709Alpha - 1.0);
					}
				)
			;
		case EColorCurve::mc_PQ:
			{
				if (!(_Value > 0.0))
					return 0.0;

				pfp64 Power = __builtin_pow(_Value, gc_PQ_M1);

				return __builtin_pow((gc_PQ_C1 + gc_PQ_C2 * Power) / (1.0 + gc_PQ_C3 * Power), gc_PQ_M2);
			}
		case EColorCurve::mc_HLG:
			{
				if (!(_Value > 0.0))
					return 0.0;

				if (_Value <= 1.0 / 12.0)
					return __builtin_sqrt(3.0 * _Value);

				return gc_HLG_A * __builtin_log(12.0 * _Value - gc_HLG_B) + gc_HLG_C;
			}
		}

		return _Value;
	}

	// Converts three values. _pSource and o_pDestination can be the same. The linear values of the two curves
	// are taken to be the same light, so what PQ and HLG say about how bright a value is, is for the caller.
	void fg_ConvertColor(CColorSpace const &_From, CColorSpace const &_To, pfp64 const *_pSource, pfp64 *o_pDestination)
	{
		pfp64 Linear[3];
		for (umint i = 0; i < 3; ++i)
			Linear[i] = fg_ColorCurveToLinear(_From.m_Curve, _pSource[i], _From.m_Gamma);

		if (_From.m_Primaries != _To.m_Primaries)
			fg_GetColorMatrix(_From.m_Primaries, _To.m_Primaries).f_Transform(Linear, Linear);

		for (umint i = 0; i < 3; ++i)
			o_pDestination[i] = fg_ColorCurveFromLinear(_To.m_Curve, Linear[i], _To.m_Gamma);
	}

	namespace
	{
		constexpr CColorChromaticity gc_WhiteD50OfProfiles = {0.9642 / (0.9642 + 1.0 + 0.8249), 1.0 / (0.9642 + 1.0 + 0.8249)};
		constexpr pfp64 gc_WhiteD50XYZ[3] = {0.9642, 1.0, 0.8249};

		// How far the chromaticities of a file can be from the ones of a standard to be them, which is what files
		// round them to
		constexpr pfp64 gc_KnownPrimariesTolerance = 0.003;

		// How far a curve of a profile can be from a curve of a standard to be it, which is what the tables of
		// profiles round to
		constexpr pfp64 gc_KnownCurveTolerance = 0.002;

		struct CProfileReader
		{
			bool f_Has(umint _Offset, umint _nBytes) const
			{
				return _Offset <= m_nBytes && _nBytes <= m_nBytes - _Offset;
			}

			uint32 f_GetUInt32(umint _Offset) const
			{
				return (uint32(m_pData[_Offset]) << 24) | (uint32(m_pData[_Offset + 1]) << 16) | (uint32(m_pData[_Offset + 2]) << 8) | uint32(m_pData[_Offset + 3]);
			}

			uint16 f_GetUInt16(umint _Offset) const
			{
				return uint16((uint32(m_pData[_Offset]) << 8) | uint32(m_pData[_Offset + 1]));
			}

			// A number with 16 bits after the point
			pfp64 f_GetFixed(umint _Offset) const
			{
				return pfp64(int32(f_GetUInt32(_Offset))) / 65536.0;
			}

			// Returns false when the profile does not have the tag, and when the tag is not inside the profile, which
			// o_pNotWhole is set for.
			bool f_FindTag(uint32 _Signature, umint &o_Offset, umint &o_nBytes, bool *o_pNotWhole = nullptr) const
			{
				if (o_pNotWhole)
					*o_pNotWhole = false;

				uint32 nTags = f_GetUInt32(128);
				if (!f_Has(132, umint(nTags) * 12))
				{
					if (o_pNotWhole)
						*o_pNotWhole = true;

					return false;
				}

				for (uint32 iTag = 0; iTag < nTags; ++iTag)
				{
					umint Entry = 132 + umint(iTag) * 12;
					if (f_GetUInt32(Entry) != _Signature)
						continue;

					o_Offset = f_GetUInt32(Entry + 4);
					o_nBytes = f_GetUInt32(Entry + 8);

					bool bWhole = o_nBytes >= 8 && f_Has(o_Offset, o_nBytes);
					if (o_pNotWhole)
						*o_pNotWhole = !bWhole;

					return bWhole;
				}

				return false;
			}

			bool f_GetXYZ(uint32 _Signature, pfp64 *o_pXYZ) const
			{
				umint Offset;
				umint nBytes;
				if (!f_FindTag(_Signature, Offset, nBytes) || nBytes < 20 || f_GetUInt32(Offset) != fs_Signature("XYZ "))
					return false;

				for (umint i = 0; i < 3; ++i)
					o_pXYZ[i] = f_GetFixed(Offset + 8 + i * 4);

				return true;
			}

			static constexpr uint32 fs_Signature(ch8 const *_pName)
			{
				return (uint32(uint8(_pName[0])) << 24) | (uint32(uint8(_pName[1])) << 16) | (uint32(uint8(_pName[2])) << 8) | uint32(uint8(_pName[3]));
			}

			uint8 const *m_pData;
			umint m_nBytes;
		};

		// The curve of a profile, which gives the linear value of a value that has the curve.
		struct CProfileCurve
		{
			pfp64 f_Get(pfp64 _Value) const
			{
				if (!m_Table.f_IsEmpty())
				{
					pfp64 Position = fg_Clamp(_Value, 0.0, 1.0) * pfp64(m_Table.f_GetLen() - 1);
					umint iFirst = fg_Min(umint(Position), m_Table.f_GetLen() - 2);
					pfp64 Fraction = Position - pfp64(iFirst);

					return m_Table[iFirst] + (m_Table[iFirst + 1] - m_Table[iFirst]) * Fraction;
				}

				// The curves with parameters of the profiles, which the type says which of are used
				pfp64 G = m_Parameters[0];
				pfp64 A = m_Parameters[1];
				pfp64 B = m_Parameters[2];
				pfp64 C = m_Parameters[3];
				pfp64 D = m_Parameters[4];
				pfp64 E = m_Parameters[5];
				pfp64 F = m_Parameters[6];

				auto fPower = [&](pfp64 _Base) -> pfp64
					{
						return _Base > 0.0 ? __builtin_pow(_Base, G) : 0.0;
					}
				;

				switch (m_Type)
				{
				case 0:
					return fPower(_Value);
				case 1:
					return _Value >= -B / A ? fPower(A * _Value + B) : 0.0;
				case 2:
					return _Value >= -B / A ? fPower(A * _Value + B) + C : C;
				case 3:
					return _Value >= D ? fPower(A * _Value + B) : C * _Value;
				default:
					return _Value >= D ? fPower(A * _Value + B) + E : C * _Value + F;
				}
			}

			bool f_IsGamma() const
			{
				return m_Table.f_IsEmpty() && m_Type == 0;
			}

			uint32 m_Type = 0;
			pfp64 m_Parameters[7] = {1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0};
			NContainer::TCVector<pfp64> m_Table;
		};

		bool fg_ReadCurve(CProfileReader const &_Reader, uint32 _Signature, CProfileCurve &o_Curve)
		{
			umint Offset;
			umint nBytes;
			if (!_Reader.f_FindTag(_Signature, Offset, nBytes) || nBytes < 12)
				return false;

			uint32 Type = _Reader.f_GetUInt32(Offset);

			if (Type == CProfileReader::fs_Signature("curv"))
			{
				uint32 nValues = _Reader.f_GetUInt32(Offset + 8);
				if (nValues == 0)
					return true;

				if (!_Reader.f_Has(Offset + 12, umint(nValues) * 2) || umint(nValues) * 2 > nBytes - 12)
					return false;

				if (nValues == 1)
				{
					o_Curve.m_Parameters[0] = pfp64(_Reader.f_GetUInt16(Offset + 12)) / 256.0;
					return true;
				}

				o_Curve.m_Table.f_SetLen(nValues);
				for (uint32 i = 0; i < nValues; ++i)
					o_Curve.m_Table[i] = pfp64(_Reader.f_GetUInt16(Offset + 12 + umint(i) * 2)) / 65535.0;

				return true;
			}

			if (Type != CProfileReader::fs_Signature("para"))
				return false;

			constexpr umint c_nParameters[5] = {1, 3, 4, 5, 7};

			o_Curve.m_Type = _Reader.f_GetUInt16(Offset + 8);
			if (o_Curve.m_Type > 4)
				return false;

			umint nParameters = c_nParameters[o_Curve.m_Type];
			if (nBytes < 12 + nParameters * 4)
				return false;

			for (umint i = 0; i < nParameters; ++i)
				o_Curve.m_Parameters[i] = _Reader.f_GetFixed(Offset + 12 + i * 4);

			// The curves that have it divide by it
			return o_Curve.m_Type == 0 || o_Curve.m_Type >= 3 || o_Curve.m_Parameters[1] != 0.0;
		}

		pfp64 fg_GetDistance(CProfileCurve const &_Left, CProfileCurve const &_Right)
		{
			pfp64 Distance = 0.0;
			for (umint i = 0; i <= 64; ++i)
			{
				pfp64 Value = pfp64(i) / 64.0;
				Distance = fg_Max(Distance, __builtin_fabs(_Left.f_Get(Value) - _Right.f_Get(Value)));
			}

			return Distance;
		}

		// Returns false for a curve that is not one that the module has.
		bool fg_GetCurve(CProfileCurve const &_Curve, EColorCurve &o_Curve, pfp64 &o_Gamma)
		{
			if (_Curve.f_IsGamma())
			{
				pfp64 Gamma = _Curve.m_Parameters[0];
				if (!(Gamma > 0.0))
					return false;

				o_Curve = Gamma == 1.0 ? EColorCurve::mc_Linear : EColorCurve::mc_Gamma;
				o_Gamma = Gamma;

				return true;
			}

			constexpr EColorCurve c_Curves[] = {EColorCurve::mc_sRGB, EColorCurve::mc_Rec709, EColorCurve::mc_Gamma22, EColorCurve::mc_Gamma24, EColorCurve::mc_Linear};

			pfp64 Least = gc_KnownCurveTolerance;
			bool bFound = false;
			for (auto Curve : c_Curves)
			{
				pfp64 Distance = 0.0;
				for (umint i = 0; i <= 64; ++i)
				{
					pfp64 Value = pfp64(i) / 64.0;
					Distance = fg_Max(Distance, __builtin_fabs(_Curve.f_Get(Value) - fg_ColorCurveToLinear(Curve, Value)));
				}

				if (Distance > Least)
					continue;

				Least = Distance;
				o_Curve = Curve;
				bFound = true;
			}

			return bFound;
		}
	}

	// Returns the primaries of a standard that the primaries are, when what differs is what files round to.
	CColorPrimaries fg_GetKnownPrimaries(CColorPrimaries const &_Primaries)
	{
		CColorPrimaries const c_Known[] =
			{
				CColorPrimaries::fs_Rec709()
				, CColorPrimaries::fs_DisplayP3()
				, CColorPrimaries::fs_Rec2020()
				, CColorPrimaries::fs_AdobeRGB()
				, CColorPrimaries::fs_AcesAP1()
			}
		;

		auto fNear = [](CColorChromaticity const &_Left, CColorChromaticity const &_Right)
			{
				return __builtin_fabs(_Left.m_X - _Right.m_X) <= gc_KnownPrimariesTolerance && __builtin_fabs(_Left.m_Y - _Right.m_Y) <= gc_KnownPrimariesTolerance;
			}
		;

		for (auto &Known : c_Known)
		{
			if
			(
				fNear(_Primaries.m_Red, Known.m_Red)
				&& fNear(_Primaries.m_Green, Known.m_Green)
				&& fNear(_Primaries.m_Blue, Known.m_Blue)
				&& fNear(_Primaries.m_White, Known.m_White)
			)
			{
				return Known;
			}
		}

		return _Primaries;
	}

	// Returns false for the values that the module has no color space for. The values are the ones of H.273, which
	// cICP of PNG and of profiles have.
	bool fg_GetColorSpaceFromCodingIndependent(uint8 _Primaries, uint8 _Transfer, CColorSpace &o_ColorSpace)
	{
		CColorSpace ColorSpace;

		switch (_Primaries)
		{
		case 1:
			ColorSpace.m_Primaries = CColorPrimaries::fs_Rec709();
			break;
		case 9:
			ColorSpace.m_Primaries = CColorPrimaries::fs_Rec2020();
			break;
		case 12:
			ColorSpace.m_Primaries = CColorPrimaries::fs_DisplayP3();
			break;
		default:
			return false;
		}

		switch (_Transfer)
		{
		case 1:
		case 6:
		case 14:
		case 15:
			ColorSpace.m_Curve = EColorCurve::mc_Rec709;
			break;
		case 4:
			ColorSpace.m_Curve = EColorCurve::mc_Gamma22;
			break;
		case 8:
			ColorSpace.m_Curve = EColorCurve::mc_Linear;
			break;
		case 13:
			ColorSpace.m_Curve = EColorCurve::mc_sRGB;
			break;
		case 16:
			ColorSpace.m_Curve = EColorCurve::mc_PQ;
			break;
		case 18:
			ColorSpace.m_Curve = EColorCurve::mc_HLG;
			break;
		default:
			return false;
		}

		o_ColorSpace = ColorSpace;

		return true;
	}

	// Returns false for a profile that is not for RGB with a matrix and curves, as the ones with tables for the
	// colors, and for a profile that is not whole. Profiles have their colors as they are seen with the white D50,
	// which is taken back to the white of the color space.
	bool fg_GetColorSpaceFromProfile(uint8 const *_pProfile, umint _nBytes, CColorSpace &o_ColorSpace)
	{
		CProfileReader Reader{_pProfile, _nBytes};

		if
		(
			!_pProfile
			|| _nBytes < 132
			|| Reader.f_GetUInt32(36) != CProfileReader::fs_Signature("acsp")
			|| Reader.f_GetUInt32(16) != CProfileReader::fs_Signature("RGB ")
			|| Reader.f_GetUInt32(20) != CProfileReader::fs_Signature("XYZ ")
		)
		{
			return false;
		}

		{
			umint Offset;
			umint nBytes;
			bool bNotWhole;
			if
			(
				Reader.f_FindTag(CProfileReader::fs_Signature("cicp"), Offset, nBytes, &bNotWhole)
				&& nBytes >= 12
				&& Reader.f_GetUInt32(Offset) == CProfileReader::fs_Signature("cicp")
				&& fg_GetColorSpaceFromCodingIndependent(_pProfile[Offset + 8], _pProfile[Offset + 9], o_ColorSpace)
			)
			{
				return true;
			}

			if (bNotWhole)
				return false;
		}

		pfp64 Primaries[3][3];
		pfp64 White[3];
		if
		(
			!Reader.f_GetXYZ(CProfileReader::fs_Signature("rXYZ"), Primaries[0])
			|| !Reader.f_GetXYZ(CProfileReader::fs_Signature("gXYZ"), Primaries[1])
			|| !Reader.f_GetXYZ(CProfileReader::fs_Signature("bXYZ"), Primaries[2])
			|| !Reader.f_GetXYZ(CProfileReader::fs_Signature("wtpt"), White)
		)
		{
			return false;
		}

		CColorMatrix FromD50 = CColorMatrix::fs_Identity();
		{
			umint Offset;
			umint nBytes;
			bool bNotWhole;
			if (Reader.f_FindTag(CProfileReader::fs_Signature("chad"), Offset, nBytes, &bNotWhole))
			{
				if (nBytes < 44 || Reader.f_GetUInt32(Offset) != CProfileReader::fs_Signature("sf32"))
					return false;

				CColorMatrix ToD50;
				for (umint iRow = 0; iRow < 3; ++iRow)
				{
					for (umint iColumn = 0; iColumn < 3; ++iColumn)
						ToD50.m_Rows[iRow][iColumn] = Reader.f_GetFixed(Offset + 8 + (iRow * 3 + iColumn) * 4);
				}

				FromD50 = ToD50.f_Inverse();
				FromD50.f_Transform(gc_WhiteD50XYZ, White);
			}
			else if (bNotWhole)
				return false;
			else if (White[1] > 0.0)
			{
				// The profiles without the matrix have the white of the color space as the white of what is measured
				CColorChromaticity WhiteOfProfile = {White[0] / (White[0] + White[1] + White[2]), White[1] / (White[0] + White[1] + White[2])};
				FromD50 = fg_GetAdaptation(gc_WhiteD50OfProfiles, WhiteOfProfile);
			}
		}

		auto fGetChromaticity = [](pfp64 const *_pXYZ, CColorChromaticity &o_Chromaticity) -> bool
			{
				pfp64 Sum = _pXYZ[0] + _pXYZ[1] + _pXYZ[2];
				if (!(Sum > 0.0) || !(_pXYZ[1] > 0.0))
					return false;

				o_Chromaticity.m_X = _pXYZ[0] / Sum;
				o_Chromaticity.m_Y = _pXYZ[1] / Sum;

				return true;
			}
		;

		for (auto &Primary : Primaries)
			FromD50.f_Transform(Primary, Primary);

		CColorSpace ColorSpace;
		if
		(
			!fGetChromaticity(Primaries[0], ColorSpace.m_Primaries.m_Red)
			|| !fGetChromaticity(Primaries[1], ColorSpace.m_Primaries.m_Green)
			|| !fGetChromaticity(Primaries[2], ColorSpace.m_Primaries.m_Blue)
			|| !fGetChromaticity(White, ColorSpace.m_Primaries.m_White)
		)
		{
			return false;
		}

		ColorSpace.m_Primaries = fg_GetKnownPrimaries(ColorSpace.m_Primaries);

		CProfileCurve Curves[3];
		if
		(
			!fg_ReadCurve(Reader, CProfileReader::fs_Signature("rTRC"), Curves[0])
			|| !fg_ReadCurve(Reader, CProfileReader::fs_Signature("gTRC"), Curves[1])
			|| !fg_ReadCurve(Reader, CProfileReader::fs_Signature("bTRC"), Curves[2])
		)
		{
			return false;
		}

		// A color space has one curve
		if (fg_GetDistance(Curves[0], Curves[1]) > gc_KnownCurveTolerance || fg_GetDistance(Curves[0], Curves[2]) > gc_KnownCurveTolerance)
			return false;

		if (!fg_GetCurve(Curves[0], ColorSpace.m_Curve, ColorSpace.m_Gamma))
			return false;

		o_ColorSpace = ColorSpace;

		return true;
	}

	// The chromaticities are the ones of white, red, green and blue, as PNG has them. _Gamma is what the values of
	// the file are the linear values to the power of, as PNG has it, or 0 for the curve of sRGB.
	CColorSpace fg_GetColorSpaceFromChromaticities(pfp64 const *_pChromaticities, pfp64 _Gamma)
	{
		CColorSpace ColorSpace;

		if (_pChromaticities)
		{
			CColorPrimaries Primaries;
			Primaries.m_White = {_pChromaticities[0], _pChromaticities[1]};
			Primaries.m_Red = {_pChromaticities[2], _pChromaticities[3]};
			Primaries.m_Green = {_pChromaticities[4], _pChromaticities[5]};
			Primaries.m_Blue = {_pChromaticities[6], _pChromaticities[7]};

			ColorSpace.m_Primaries = fg_GetKnownPrimaries(Primaries);
		}

		if (_Gamma > 0.0)
		{
			pfp64 Gamma = 1.0 / _Gamma;

			// What files have for sRGB, which has a curve that is close to this power
			if (__builtin_fabs(_Gamma - 0.45455) < 0.0001)
				ColorSpace.m_Curve = EColorCurve::mc_sRGB;
			else if (Gamma == 1.0)
				ColorSpace.m_Curve = EColorCurve::mc_Linear;
			else
			{
				ColorSpace.m_Curve = EColorCurve::mc_Gamma;
				ColorSpace.m_Gamma = Gamma;
			}
		}

		return ColorSpace;
	}
}
