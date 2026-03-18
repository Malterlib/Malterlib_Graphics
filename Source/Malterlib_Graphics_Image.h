// Copyright © 2015 Hansoft AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#pragma once

#include <Mib/Numeric/Vector>

namespace NMib::NGraphics
{
	template <typename t_CPoint>
	class TCImageRect
	{
	public:
		using CType = typename t_CPoint::CType;

		t_CPoint m_Corners[2];

		CType f_GetWidth();
		CType f_GetHeight();
		CType f_GetDepth();
		CType f_GetNumArrays();
	};

	template <typename t_CPoint>
	class TCImageDimensions
	{
	public:
		using CType = typename t_CPoint::CType;

		t_CPoint m_Dimensions;

		TCImageDimensions(CType _Width = 1, CType _Height = 1, CType _Depth = 1, CType _Arrays = 1);

		CType f_GetWidth();
		CType f_GetHeight();
		CType f_GetDepth();
		CType f_GetNumArrays();

		bool operator == (const TCImageDimensions &_Other) const noexcept;
		COrdering_Strong operator <=> (const TCImageDimensions &_Other) const noexcept;
		CType &operator[] (int _Pos);
	};

	template <typename t_CPoint>
	class TCImageStride
	{
	public:
		using CType = typename t_CPoint::CType;

		t_CPoint m_Stride;

		TCImageStride();
		TCImageStride(CType _Width, CType _Height = 0, CType _Depth = 0);

		CType f_GetWidth();
		CType f_GetHeight();
		CType f_GetDepth();
		CType &operator[] (int _Pos);
	};

	using CImageDimensions = TCImageDimensions<NNumeric::CVec4Duint32>;
	using CImageStride = TCImageStride<NNumeric::CVec4Duint32>;
	using CImageRect = TCImageRect<NNumeric::CVec4Duint32>;

	enum EImageFormatConstant
	{
		//////////////////////////////////////////
		// 8 bit format spec (not per channel)

		// 5 bit Physical format
		EImageFormatConstant_Format_Int = 0,
		EImageFormatConstant_Format_Float,
		EImageFormatConstant_Format_FourcCC,								// Format is defined in lower 32 bits as FourCC
		EImageFormatConstant_Format_CB4x4_Interpolate_5_6_5,				// 4x4 Compressed block (4 bits/pixel)
		EImageFormatConstant_Format_CB4x4_Interpolate_5_6_5_Interpolate_8,	// 4x4 Compressed block (8 bits/pixel)
		EImageFormatConstant_Format_CB4x4_Interpolate_5_6_5_Raw_4,			// 4x4 Compressed block (8 bits/pixel)
		EImageFormatConstant_Format_CB4x4_Interpolate_8_Interpolate_8,		// 4x4 Compressed block (8 bits/pixel)
		EImageFormatConstant_Format_CB4x4_Interpolate_8,					// 4x4 Compressed block (4 bits/pixel)
		EImageFormatConstant_Format_CB4x4_Interpolate_8_8,					// 4x4 Compressed block (4 bits/pixel)

		// 3 bit Physical Endian
		EImageFormatConstant_Endian_8_LittleEndian = 0,	// Endian is swapped in chunks of 8 bit and is in little endian
		EImageFormatConstant_Endian_16_LittleEndian,	// Endian is swapped in chunks of 8 bit and is in little endian
		EImageFormatConstant_Endian_32_LittleEndian,	// Endian is swapped in chunks of 8 bit and is in little endian
		EImageFormatConstant_Endian_64_LittleEndian,	// Endian is swapped in chunks of 8 bit and is in little endian
		EImageFormatConstant_Endian_8_BigEndian,		// Endian is swapped in chunks of 8 bit and is in big endian
		EImageFormatConstant_Endian_16_BigEndian,		// Endian is swapped in chunks of 8 bit and is in big endian
		EImageFormatConstant_Endian_32_BigEndian,		// Endian is swapped in chunks of 8 bit and is in big endian
		EImageFormatConstant_Endian_64_BigEndian,		// Endian is swapped in chunks of 8 bit and is in big endian
#ifdef DMibPLittleEndian
		EImageFormatConstant_Endian_8_LocalEndian = EImageFormatConstant_Endian_8_LittleEndian,
		EImageFormatConstant_Endian_16_LocalEndian = EImageFormatConstant_Endian_16_LittleEndian,
		EImageFormatConstant_Endian_32_LocalEndian = EImageFormatConstant_Endian_32_LittleEndian,
		EImageFormatConstant_Endian_64_LocalEndian = EImageFormatConstant_Endian_64_LittleEndian,
#else
		EImageFormatConstant_Endian_8_LocalEndian = EImageFormatConstant_Endian_8_BigEndian,
		EImageFormatConstant_Endian_16_LocalEndian = EImageFormatConstant_Endian_16_BigEndian,
		EImageFormatConstant_Endian_32_LocalEndian = EImageFormatConstant_Endian_32_BigEndian,
		EImageFormatConstant_Endian_64_LocalEndian = EImageFormatConstant_Endian_64_BigEndian,
#endif

		//////////////////////////////////////////
		// 14 bit per channel format spec (54 bits)

		// 6 bit number of physical bits in component. Only the last components can be disabled
		EImageFormatConstant_BitDepth_0 = 0,	// Disable component
		EImageFormatConstant_BitDepth_32 = 32,
		EImageFormatConstant_BitDepth_48,
		EImageFormatConstant_BitDepth_64,
		EImageFormatConstant_BitDepth_80,
		EImageFormatConstant_BitDepth_96,
		EImageFormatConstant_BitDepth_112,
		EImageFormatConstant_BitDepth_128,
		EImageFormatConstant_BitDepth_Reserved0,
		EImageFormatConstant_BitDepth_Reserved1,
		EImageFormatConstant_BitDepth_Reserved2,
		EImageFormatConstant_BitDepth_Reserved3,
		EImageFormatConstant_BitDepth_Reserved4,
		EImageFormatConstant_BitDepth_Reserved5,
		EImageFormatConstant_BitDepth_Reserved6,
		EImageFormatConstant_BitDepth_Reserved7,
		EImageFormatConstant_BitDepth_Reserved8,
		EImageFormatConstant_BitDepth_Reserved9,
		EImageFormatConstant_BitDepth_Reserved10,
		EImageFormatConstant_BitDepth_Reserved11,
		EImageFormatConstant_BitDepth_Reserved12,
		EImageFormatConstant_BitDepth_Reserved13,
		EImageFormatConstant_BitDepth_Reserved14,
		EImageFormatConstant_BitDepth_Reserved15,
		EImageFormatConstant_BitDepth_Reserved16,
		EImageFormatConstant_BitDepth_Reserved17,
		EImageFormatConstant_BitDepth_Reserved18,
		EImageFormatConstant_BitDepth_Reserved19,
		EImageFormatConstant_BitDepth_Reserved20,
		EImageFormatConstant_BitDepth_Reserved21,
		EImageFormatConstant_BitDepth_Reserved22,
		EImageFormatConstant_BitDepth_Reserved23,
		EImageFormatConstant_BitDepth_Reserved24,

		// 3 bit source component
		EImageFormatConstant_Component_0 = 0,	// Red
		EImageFormatConstant_Component_1,		// Green
		EImageFormatConstant_Component_2,		// Blue
		EImageFormatConstant_Component_3,		// Alpha
		EImageFormatConstant_Component_Unused,	// Unused component (to fill out bits)
		EImageFormatConstant_Component_Zero,	// Zero
		EImageFormatConstant_Component_One,		// One
		EImageFormatConstant_Component_Reserved0,

		// 3 bit usage
		EImageFormatConstant_Usage_Unknown = 0,	// Unknown
		EImageFormatConstant_Usage_Default,		// Normal (Color, Position)
		EImageFormatConstant_Usage_D,			// Depth
		EImageFormatConstant_Usage_S,			// Stencil
		EImageFormatConstant_Usage_E,			// Exponent
		EImageFormatConstant_Usage_N,			// Normal (will assume normalization with all other normal components)
		EImageFormatConstant_Usage_YUV,			// YUV component
		EImageFormatConstant_Usage_SRGB,		// SRGB Conversion

		// 2 bit physical to expanded type
		EImageFormatConstant_Type_UFloat = 0,	// Integer format expands to 0 - 1
		EImageFormatConstant_Type_SFloat,		// Integer format expands to about -1 - 1
		EImageFormatConstant_Type_UInt,			// Signed Integer format
		EImageFormatConstant_Type_SInt			// Unsigned Integer format
	};

	// Component0_Component|Component0_Type<<4|Component

#define DMibGraphicsImageFormat_Component(_nBits, _SourceComponent, _Usage, _ExpandFormat) ((uint32(_nBits) << 8) | (uint32(_SourceComponent) << 5) | (uint32(_Usage) << 2) | uint32(_ExpandFormat))
#define DMibGraphicsImageFormat_Build(_Component0, _Component1, _Component2, _Component3) ((uint64(_Component0) << 14*3) | (uint64(_Component1) << 14*2) | (uint64(_Component2) << 14*1)| (uint64(_Component3)))
#define DMibGraphicsImageFormat_Build_Low(_Component0, _Component1, _Component2, _Component3) (DMibGraphicsImageFormat_Build(_Component0, _Component1, _Component2, _Component3) & 0xFFFFffff)
#define DMibGraphicsImageFormat_Build_High(_Component0, _Component1, _Component2, _Component3) (DMibGraphicsImageFormat_Build(_Component0, _Component1, _Component2, _Component3) >> 32)
#define DMibGraphicsImageFormat_DefineEnums(_Name, _PhysicalFormat, _PhysicalEndian, _Component0, _Component1, _Component2, _Component3) \
EImageFormat_##_Name##_Low = DMibGraphicsImageFormat_Build_Low(_Component0, _Component1, _Component2, _Component3),\
EImageFormat_##_Name##_High = (uint32(_PhysicalFormat) << 27 | uint32(_PhysicalEndian) << 24) | DMibGraphicsImageFormat_Build_High(_Component0, _Component1, _Component2, _Component3)



	enum EImageFormat
	{
		// This is the default conversion format that most operations can be executed on

		/////////////////////////////////////////////
		// 512 bit per fragment formats

		DMibGraphicsImageFormat_DefineEnums(R128G128B128A128_Float, EImageFormatConstant_Format_Float, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(R128G128B128A128_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R128G128B128A128_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R128G128B128A128_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R128G128B128A128_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_128, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		/////////////////////////////////////////////
		// 256 bit per fragment formats

		DMibGraphicsImageFormat_DefineEnums(R64G64B64A64_Float, EImageFormatConstant_Format_Float, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(R64G64B64A64_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R64G64B64A64_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R64G64B64A64_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R64G64B64A64_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(EImageFormatConstant_BitDepth_64, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		/////////////////////////////////////////////
		// 128 bit per fragment formats

		DMibGraphicsImageFormat_DefineEnums(R32G32B32A32_Float, EImageFormatConstant_Format_Float, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(R32G32B32A32_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R32G32B32A32_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R32G32B32A32_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R32G32B32A32_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		/////////////////////////////////////////////
		// 64 bit per fragment formats

		DMibGraphicsImageFormat_DefineEnums(R32G32_Float, EImageFormatConstant_Format_Float, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(R32G32_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R32G32_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R32G32_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R32G32_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(R16G16B16A16_Float, EImageFormatConstant_Format_Float, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(R16G16B16A16_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R16G16B16A16_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R16G16B16A16_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R16G16B16A16_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),


		DMibGraphicsImageFormat_DefineEnums(B16G16R16A16_Float, EImageFormatConstant_Format_Float, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(B16G16R16A16_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(B16G16R16A16_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(B16G16R16A16_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(B16G16R16A16_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),


		/////////////////////////////////////////////
		// 48 bit per fragment formats

		DMibGraphicsImageFormat_DefineEnums(R16G16B16_Float, EImageFormatConstant_Format_Float, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(R16G16B16_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R16G16B16_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R16G16B16_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R16G16B16_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),



		DMibGraphicsImageFormat_DefineEnums(B16G16R16_Float, EImageFormatConstant_Format_Float, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(B16G16R16_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(B16G16R16_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(B16G16R16_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(B16G16R16_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),


		/////////////////////////////////////////////
		// 32 bit per fragment formats

		// 1 component formats

		DMibGraphicsImageFormat_DefineEnums(R32_Float, EImageFormatConstant_Format_Float, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(R32_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R32_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R32_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R32_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(32, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		// 2 component formats
		DMibGraphicsImageFormat_DefineEnums(R16G16_Float, EImageFormatConstant_Format_Float, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(R16G16_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R16G16_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R16G16_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R16G16_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		// 2 component formats
		DMibGraphicsImageFormat_DefineEnums(R16A16_Float, EImageFormatConstant_Format_Float, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(R16A16_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R16A16_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R16A16_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R16A16_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		// 11, 11, 10 format
		DMibGraphicsImageFormat_DefineEnums(X11Y11Z10_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(11, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(11, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(X11Y11Z10_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(11, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(11, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(X11Y11Z10_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(11, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(11, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(X11Y11Z10_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(11, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(11, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		// 10-10-10-2 format
		DMibGraphicsImageFormat_DefineEnums(R10G10B10A2_Float, EImageFormatConstant_Format_Float, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(2, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R10G10B10A2_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(2, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R10G10B10A2_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(2, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R10G10B10A2_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(2, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R10G10B10A2_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(10, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(2, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		// 9-9-9-5 format exponent
		DMibGraphicsImageFormat_DefineEnums(R9G9B9E5_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(9, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(9, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(9, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_E      , EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R9G9B9E5_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(9, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(9, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(9, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_E      , EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R9G9B9E5_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(9, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(9, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(9, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_E      , EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R9G9B9E5_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(9, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(9, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(9, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_E      , EImageFormatConstant_Type_SFloat)),

		// 8-8-8-8 format

		DMibGraphicsImageFormat_DefineEnums(R8G8B8A8_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R8G8B8A8_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R8G8B8A8_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R8G8B8A8_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		// 8-8-8-8 format rev

		DMibGraphicsImageFormat_DefineEnums(B8G8R8A8_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(B8G8R8A8_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(B8G8R8A8_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(B8G8R8A8_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		/////////////////////////////////////////////
		// 24 bit per fragment formats

		// 8-8-8 format

		DMibGraphicsImageFormat_DefineEnums(R8G8B8_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R8G8B8_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R8G8B8_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R8G8B8_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		// 8-8-8 format rev

		DMibGraphicsImageFormat_DefineEnums(B8G8R8_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(B8G8R8_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(B8G8R8_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(B8G8R8_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat)),

		/////////////////////////////////////////////
		// 16 bit per fragment formats

		// 1 component formats

		DMibGraphicsImageFormat_DefineEnums(R16_Float, EImageFormatConstant_Format_Float, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(R16_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R16_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R16_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R16_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(16, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		// 2 component formats

		DMibGraphicsImageFormat_DefineEnums(R8G8_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R8G8_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R8G8_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R8G8_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		DMibGraphicsImageFormat_DefineEnums(R8A8_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(1, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R8A8_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R8A8_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R8A8_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero,	  EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		// 5-6-5 format

		DMibGraphicsImageFormat_DefineEnums(R5G6B5_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(6, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R5G6B5_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(6, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R5G6B5_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(6, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R5G6B5_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(6, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),

		// 5-5-5-1 format

		DMibGraphicsImageFormat_DefineEnums(R5G6B5A1_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(1, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R5G6B5A1_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(1, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R5G6B5A1_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(1, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R5G6B5A1_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(5, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(1, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		/////////////////////////////////////////////
		// 8 bit per fragment formats
		// 1 component formats

		DMibGraphicsImageFormat_DefineEnums(R8_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R8_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R8_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R8_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),


		/////////////////////////////////////////////
		// 4 bit per fragment formats
		// 1 component formats

		DMibGraphicsImageFormat_DefineEnums(R4_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(4, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R4_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(4, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R4_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(4, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R4_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(4, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),


		/////////////////////////////////////////////
		// 2 bit per fragment formats
		// 1 component formats

		DMibGraphicsImageFormat_DefineEnums(R2_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(2, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R2_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(2, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R2_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(2, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R2_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(2, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),


		/////////////////////////////////////////////
		// 1 bit per fragment formats
		// 1 component formats

		DMibGraphicsImageFormat_DefineEnums(R1_UInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(1, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UInt)),

		DMibGraphicsImageFormat_DefineEnums(R1_SInt, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(1, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SInt)),

		DMibGraphicsImageFormat_DefineEnums(R1_UNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(1, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(R1_SNorm, EImageFormatConstant_Format_Int, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(1, EImageFormatConstant_Component_0,   EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat),
		DMibGraphicsImageFormat_Component(0, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Unknown, EImageFormatConstant_Type_SFloat)),


		/////////////////////////////////////////////
		// Block compression formats
		// 1 component formats


		DMibGraphicsImageFormat_DefineEnums(DXT1, EImageFormatConstant_Format_CB4x4_Interpolate_5_6_5, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(DXT3, EImageFormatConstant_Format_CB4x4_Interpolate_5_6_5_Raw_4, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(DXT5, EImageFormatConstant_Format_CB4x4_Interpolate_5_6_5_Interpolate_8, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_2, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_3, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat)),

		DMibGraphicsImageFormat_DefineEnums(3DC, EImageFormatConstant_Format_CB4x4_Interpolate_8_Interpolate_8, EImageFormatConstant_Endian_8_LocalEndian,
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_0, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_1, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat),
		DMibGraphicsImageFormat_Component(8, EImageFormatConstant_Component_Zero, EImageFormatConstant_Usage_Default, EImageFormatConstant_Type_UFloat))


	};

#define DMibGraphicsImageFormat(_Name) (uint64(NGraphics::EImageFormat_##_Name##_High) << 32 | uint64(NGraphics::EImageFormat_##_Name##_Low))

#define DMibGraphicsImageFormat_Get_Component(_Format, _Component) ((_Format >> (3-_Component) * 14) & ((1 << 14) - 1))
#define DMibGraphicsImageFormat_Get_ComponentExpandFormat(_Component) (_Component & 3)
#define DMibGraphicsImageFormat_Get_ComponentUsage(_Component) ((_Component >> 2) & 7)
#define DMibGraphicsImageFormat_Get_ComponentSourceComponent(_Component) ((_Component >> 5) & 7)
#define DMibGraphicsImageFormat_Get_ComponentNumBits(_Component) ((_Component >> 8) & 63)
#define DMibGraphicsImageFormat_Get_PhysicalEndian(_Format) ((_Format >> (32+24)) & (7))
#define DMibGraphicsImageFormat_Get_PhysicalFormat(_Format) (_Format >> (32+27))


	#include "Malterlib_Graphics_Image_Conversion.h"

	class CImage
	{
	public:
		uint64 m_ImageFormat;
		CImageDimensions m_Dimensions;
		class CLockedData
		{
		public:
			void *m_pData;
			CImageStride m_Stride;
			mint m_InternalData; // Used by the image implementation to store internal data
		};
		virtual ~CImage();

		template <uint64 _FromFormat, uint64 _ToFormat>
		static void fs_ConvertImage(CImage &_From, CImage &_To);

		bool f_ConvertInto(CImage &_To);

		virtual bool f_LockRead(CLockedData &_Data);
		virtual bool f_LockWrite(CLockedData &_Data);

		virtual bool f_LockReadWrite(CLockedData &_Data) = 0;
		virtual void f_UnLock(CLockedData &_Data) = 0;

		static bool fs_WriteImage(CImage &_Destination, NStr::CStr _FileName);
		static bool fs_WriteImage(CImage &_Destination, NStream::CBinaryStream &_Stream, NStr::CStr _Type);

		bool f_FlipHorizontal(CImage &_Dst);
		bool f_RemoveAlpha(CImage &_Dst);
		bool f_Blend(CImage &_Dst, fp64 _SrcBlend, fp64 _DstBlend);
		bool f_StretchHalfXY(CImage &_Dst);
		bool f_StretchBilinear(CImage &_Dst, fp64 _OffsetX, fp64 _OffsetY);
		uint32 f_PixelSize();

	private:
		template <typename t_Func>
		bool fp_StretchHalfXY_Core(CImage &_Dst, t_Func _Func);
	};


	class CImageMemory : public CImage
	{
	public:
		using CStorageType = uint8; // Align here in the future

		CImageStride m_Stride;

		NContainer::TCVector<CStorageType> m_Data;

		bool f_Create(uint64 _ImageFormat, CImageDimensions _Dimensions, CImageStride _Stride = CImageStride());
		bool f_Create(const CImageMemory &_Src);
		bool f_Create(const CImage &_Src);
		bool f_CreateRaw(uint64 _ImageFormat, CImageDimensions _Dimensions, CImageStride _Stride, mint _DataSize);

		bool f_LockReadWrite(CLockedData &_Data);
		void f_UnLock(CLockedData &_Data);

		static bool fs_ReadImage(CImageMemory &_Destination, NStr::CStr _FileName);
		static bool fs_ReadImage(CImageMemory &_Destination, NStream::CBinaryStream &_Stream);
	};

	class CImageIO
	{
	public:
		virtual ~CImageIO();
		virtual bool f_DetectFormatFromFilename(const NStr::CStr &_FileName) = 0;
		virtual bool f_DetectFormatFromStream(NStream::CBinaryStream &_Stream) = 0;
		virtual bool f_ReadImage(CImageMemory &_Destination, NStream::CBinaryStream &_Stream) = 0;
		virtual bool f_WriteImage(CImage &_Source, NStream::CBinaryStream &_Stream) = 0;
	};
}

#include "Malterlib_Graphics_Image.hpp"

#ifndef DMibPNoShortCuts
	using namespace NMib::NGraphics;
#endif
