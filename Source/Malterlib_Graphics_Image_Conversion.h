// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

template <uint32 t_Check>
struct TCStaticCheck
{
};

template <>
struct TCStaticCheck<1>
{
	static constexpr aint mc_Check = 1;
};

template <uint64 const t_FromFormat, uint64 const t_ToFormat>
struct TCImageConversion
{
	template <uint32 t_PhysicalFormat, uint32 t_nBits0, uint32 t_nBits1, uint32 t_nBits2, uint32 t_nBits3, uint32 t_MakePartial>
	struct TCPhysicalFormat
	{
	};

	template <uint32 t_nBits0, uint32 t_nBits1, uint32 t_nBits2, uint32 t_nBits3, uint32 t_MakePartial>
	struct TCPhysicalFormat<EImageFormatConstant_Format_Int, t_nBits0, t_nBits1, t_nBits2, t_nBits3, t_MakePartial>
	{
		static constexpr aint mc_NumBits = t_nBits0 + t_nBits1 + t_nBits2 + t_nBits3;
		static constexpr aint mc_Check = TCStaticCheck<mc_NumBits == 32>::mc_Check;
	};

#define DTemp_ImplementCheck(d_Format, d_nBits0, d_nBits1, d_nBits2, d_nBits3) \
	template <uint32 t_MakePartial>\
	struct TCPhysicalFormat<d_Format, d_nBits0, d_nBits1, d_nBits2, d_nBits3, t_MakePartial>\
	{\
	public:\
		static constexpr aint mc_Check = 1;\
	}

	DTemp_ImplementCheck(EImageFormatConstant_Format_Float, 32, 32, 32, 32);

#undef DTemp_ImplementCheck

	static void fs_ConvertImage(void *_pFrom, void *_pTo, CImageDimensions const &_Dim, CImageStride const &_StrideFrom, CImageStride const &_StrideTo);

	static constexpr aint mc_From_PhysicalFormat = DMibGraphicsImageFormat_Get_PhysicalFormat(t_FromFormat);
	static constexpr aint mc_From_PhysicalEdian = DMibGraphicsImageFormat_Get_PhysicalEndian(t_FromFormat);
	static constexpr aint mc_From_Component0 = DMibGraphicsImageFormat_Get_Component(t_FromFormat, 0);
	static constexpr aint mc_From_Component1 = DMibGraphicsImageFormat_Get_Component(t_FromFormat, 1);
	static constexpr aint mc_From_Component2 = DMibGraphicsImageFormat_Get_Component(t_FromFormat, 2);
	static constexpr aint mc_From_Component3 = DMibGraphicsImageFormat_Get_Component(t_FromFormat, 3);
	static constexpr aint mc_From_Component0_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(mc_From_Component0);
	static constexpr aint mc_From_Component0_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(mc_From_Component0);
	static constexpr aint mc_From_Component0_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(mc_From_Component0);
	static constexpr aint mc_From_Component0_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(mc_From_Component0);
	static constexpr aint mc_From_Component1_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(mc_From_Component1);
	static constexpr aint mc_From_Component1_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(mc_From_Component1);
	static constexpr aint mc_From_Component1_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(mc_From_Component1);
	static constexpr aint mc_From_Component1_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(mc_From_Component1);
	static constexpr aint mc_From_Component2_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(mc_From_Component2);
	static constexpr aint mc_From_Component2_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(mc_From_Component2);
	static constexpr aint mc_From_Component2_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(mc_From_Component2);
	static constexpr aint mc_From_Component2_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(mc_From_Component2);
	static constexpr aint mc_From_Component3_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(mc_From_Component3);
	static constexpr aint mc_From_Component3_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(mc_From_Component3);
	static constexpr aint mc_From_Component3_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(mc_From_Component3);
	static constexpr aint mc_From_Component3_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(mc_From_Component3);
	static constexpr aint mc_To_PhysicalFormat = DMibGraphicsImageFormat_Get_PhysicalFormat(t_ToFormat);
	static constexpr aint mc_To_PhysicalEdian = DMibGraphicsImageFormat_Get_PhysicalEndian(t_ToFormat);
	static constexpr aint mc_To_Component0 = DMibGraphicsImageFormat_Get_Component(t_ToFormat, 0);
	static constexpr aint mc_To_Component1 = DMibGraphicsImageFormat_Get_Component(t_ToFormat, 1);
	static constexpr aint mc_To_Component2 = DMibGraphicsImageFormat_Get_Component(t_ToFormat, 2);
	static constexpr aint mc_To_Component3 = DMibGraphicsImageFormat_Get_Component(t_ToFormat, 3);
	static constexpr aint mc_To_Component0_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(mc_To_Component0);
	static constexpr aint mc_To_Component0_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(mc_To_Component0);
	static constexpr aint mc_To_Component0_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(mc_To_Component0);
	static constexpr aint mc_To_Component0_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(mc_To_Component0);
	static constexpr aint mc_To_Component1_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(mc_To_Component1);
	static constexpr aint mc_To_Component1_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(mc_To_Component1);
	static constexpr aint mc_To_Component1_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(mc_To_Component1);
	static constexpr aint mc_To_Component1_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(mc_To_Component1);
	static constexpr aint mc_To_Component2_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(mc_To_Component2);
	static constexpr aint mc_To_Component2_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(mc_To_Component2);
	static constexpr aint mc_To_Component2_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(mc_To_Component2);
	static constexpr aint mc_To_Component2_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(mc_To_Component2);
	static constexpr aint mc_To_Component3_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(mc_To_Component3);
	static constexpr aint mc_To_Component3_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(mc_To_Component3);
	static constexpr aint mc_To_Component3_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(mc_To_Component3);
	static constexpr aint mc_To_Component3_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(mc_To_Component3);
};

#include "Malterlib_Graphics_Image_Conversion.hpp"
