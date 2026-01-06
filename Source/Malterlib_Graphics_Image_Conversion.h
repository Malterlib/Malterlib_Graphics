// Copyright © 2015 Hansoft AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

template <uint32 _Check>
class TCStaticCheck
{
};

template <>
class TCStaticCheck<1>
{
public:

	enum
	{
		ECheck = 1
	};
};


template <const uint64 _FromFormat, const uint64 _ToFormat>
class TCImageConversion
{
public:
	template <uint32 _PhysicalFormat, uint32 _nBits0, uint32 _nBits1, uint32 _nBits2, uint32 _nBits3, uint32 _MakePartial>
	class TCPhysicalFormat
	{
	public:
	};

	template <uint32 _nBits0, uint32 _nBits1, uint32 _nBits2, uint32 _nBits3, uint32 _MakePartial>
	class TCPhysicalFormat<EImageFormatConstant_Format_Int, _nBits0, _nBits1, _nBits2, _nBits3, _MakePartial>
	{
	public:
		enum
		{
			ENumBits = _nBits0 + _nBits1 +_nBits2 + _nBits3,
			ECheck = TCStaticCheck<ENumBits == 32>::ECheck
		};
	};

#define ETemp_ImplementCheck(_Format, _nBits0, _nBits1, _nBits2, _nBits3) \
	template <uint32 _MakePartial>\
	class TCPhysicalFormat<_Format, _nBits0, _nBits1, _nBits2, _nBits3, _MakePartial>\
	{\
	public:\
		enum\
		{\
			ECheck = 1\
		};\
	}

	ETemp_ImplementCheck(EImageFormatConstant_Format_Float, 32, 32, 32, 32);

#undef ETemp_ImplementCheck


	enum
	{
		EFrom_PhysicalFormat = DMibGraphicsImageFormat_Get_PhysicalFormat(_FromFormat),
		EFrom_PhysicalEdian = DMibGraphicsImageFormat_Get_PhysicalEndian(_FromFormat),
		EFrom_Component0 = DMibGraphicsImageFormat_Get_Component(_FromFormat, 0),
		EFrom_Component1 = DMibGraphicsImageFormat_Get_Component(_FromFormat, 1),
		EFrom_Component2 = DMibGraphicsImageFormat_Get_Component(_FromFormat, 2),
		EFrom_Component3 = DMibGraphicsImageFormat_Get_Component(_FromFormat, 3),

		EFrom_Component0_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(EFrom_Component0),
		EFrom_Component0_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(EFrom_Component0),
		EFrom_Component0_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(EFrom_Component0),
		EFrom_Component0_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(EFrom_Component0),

		EFrom_Component1_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(EFrom_Component1),
		EFrom_Component1_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(EFrom_Component1),
		EFrom_Component1_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(EFrom_Component1),
		EFrom_Component1_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(EFrom_Component1),

		EFrom_Component2_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(EFrom_Component2),
		EFrom_Component2_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(EFrom_Component2),
		EFrom_Component2_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(EFrom_Component2),
		EFrom_Component2_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(EFrom_Component2),

		EFrom_Component3_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(EFrom_Component3),
		EFrom_Component3_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(EFrom_Component3),
		EFrom_Component3_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(EFrom_Component3),
		EFrom_Component3_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(EFrom_Component3),

//		EFrom_Check0 = TCPhysicalFormat<EFrom_PhysicalFormat, EFrom_Component0_NumBits, EFrom_Component1_NumBits, EFrom_Component2_NumBits, EFrom_Component3_NumBits, 0>::ECheck,

		ETo_PhysicalFormat = DMibGraphicsImageFormat_Get_PhysicalFormat(_ToFormat),
		ETo_PhysicalEdian = DMibGraphicsImageFormat_Get_PhysicalEndian(_ToFormat),
		ETo_Component0 = DMibGraphicsImageFormat_Get_Component(_ToFormat, 0),
		ETo_Component1 = DMibGraphicsImageFormat_Get_Component(_ToFormat, 1),
		ETo_Component2 = DMibGraphicsImageFormat_Get_Component(_ToFormat, 2),
		ETo_Component3 = DMibGraphicsImageFormat_Get_Component(_ToFormat, 3),

		ETo_Component0_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(ETo_Component0),
		ETo_Component0_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(ETo_Component0),
		ETo_Component0_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(ETo_Component0),
		ETo_Component0_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(ETo_Component0),

		ETo_Component1_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(ETo_Component1),
		ETo_Component1_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(ETo_Component1),
		ETo_Component1_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(ETo_Component1),
		ETo_Component1_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(ETo_Component1),

		ETo_Component2_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(ETo_Component2),
		ETo_Component2_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(ETo_Component2),
		ETo_Component2_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(ETo_Component2),
		ETo_Component2_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(ETo_Component2),

		ETo_Component3_NumBits = DMibGraphicsImageFormat_Get_ComponentNumBits(ETo_Component3),
		ETo_Component3_SourceComponent = DMibGraphicsImageFormat_Get_ComponentSourceComponent(ETo_Component3),
		ETo_Component3_Usage = DMibGraphicsImageFormat_Get_ComponentUsage(ETo_Component3),
		ETo_Component3_ExpandFormat = DMibGraphicsImageFormat_Get_ComponentExpandFormat(ETo_Component3),

//		ETo_Check0 = TCPhysicalFormat<EFrom_PhysicalFormat, EFrom_Component0_NumBits, EFrom_Component1_NumBits, EFrom_Component2_NumBits, EFrom_Component3_NumBits, 0>::ECheck
	};

	static void fs_ConvertImage(void * _pFrom, void * _pTo, const CImageDimensions &_Dim, const CImageStride &_StrideFrom, const CImageStride &_StrideTo)
	{

	}
};
