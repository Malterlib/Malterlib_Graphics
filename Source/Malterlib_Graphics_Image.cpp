// Copyright © 2015 Hansoft AB 
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#include "Malterlib_Graphics_Image.h"

#include <Mib/Core/RuntimeType>

namespace NMib::NGraphics
{
	// Used for creating unknown types such as FourCC
	bool CImageMemory::f_Create(const CImage &_Src)
	{
		return f_Create(_Src.m_ImageFormat, _Src.m_Dimensions);
	}
	bool CImageMemory::f_Create(const CImageMemory &_Src)
	{
		return f_Create(_Src.m_ImageFormat, _Src.m_Dimensions, _Src.m_Stride);
	}

	bool CImageMemory::f_CreateRaw(uint64 _ImageFormat, CImageDimensions _Dimensions, CImageStride _Stride, mint _DataSize)
	{
		DMibFastCheck(_Dimensions.m_Dimensions[0] > 0);
		DMibFastCheck(_Dimensions.m_Dimensions[1] > 0);
		DMibFastCheck(_Dimensions.m_Dimensions[2] > 0);
		DMibFastCheck(_Dimensions.m_Dimensions[3] > 0);
		m_ImageFormat = _ImageFormat;
		m_Dimensions = _Dimensions;
		m_Stride = _Stride;
		m_Data.f_SetLen(_DataSize);
		return true;
	}

	bool CImageMemory::f_Create(uint64 _ImageFormat, CImageDimensions _Dimensions, CImageStride _Stride)
	{
		DMibFastCheck(_Dimensions.m_Dimensions[0] > 0);
		DMibFastCheck(_Dimensions.m_Dimensions[1] > 0);
		DMibFastCheck(_Dimensions.m_Dimensions[2] > 0);
		DMibFastCheck(_Dimensions.m_Dimensions[3] > 0);
		uint32 PhysicalFormat = DMibGraphicsImageFormat_Get_PhysicalFormat(_ImageFormat);

		uint32 ComponentR = DMibGraphicsImageFormat_Get_Component(_ImageFormat, 0);
		uint32 ComponentG = DMibGraphicsImageFormat_Get_Component(_ImageFormat, 1);
		uint32 ComponentB = DMibGraphicsImageFormat_Get_Component(_ImageFormat, 2);
		uint32 ComponentA = DMibGraphicsImageFormat_Get_Component(_ImageFormat, 3);

		uint32 BitsR = DMibGraphicsImageFormat_Get_ComponentNumBits(ComponentR);
		uint32 BitsG = DMibGraphicsImageFormat_Get_ComponentNumBits(ComponentG);
		uint32 BitsB = DMibGraphicsImageFormat_Get_ComponentNumBits(ComponentB);
		uint32 BitsA = DMibGraphicsImageFormat_Get_ComponentNumBits(ComponentA);

		uint32 TotalBits = BitsR + BitsG + BitsB + BitsA;

		m_Dimensions = _Dimensions;
		m_ImageFormat = _ImageFormat;

		uint32 PhysicalWidth = _Dimensions[0];
		uint32 PhysicalHeight = _Dimensions[1];
		uint32 PhysicalDepth = _Dimensions[2];
		uint32 PhysicalArrays = _Dimensions[3];

		uint32 StrideX = _Stride.m_Stride[0];
		uint32 StrideY = _Stride.m_Stride[1];
		uint32 StrideZ = _Stride.m_Stride[2];

		uint32 StrideAlign = 16;

		switch (PhysicalFormat)
		{
		case EImageFormatConstant_Format_Int:
		case EImageFormatConstant_Format_Float:
			{
				if (!StrideX)
					StrideX = fg_AlignUp((TotalBits * PhysicalWidth) >> 3, StrideAlign);
				if (!StrideY)
					StrideY = fg_AlignUp(PhysicalHeight * StrideX, StrideAlign);
				if (!StrideZ)
					StrideZ = fg_AlignUp(PhysicalDepth * StrideY, StrideAlign);
			}
			break;
		case EImageFormatConstant_Format_FourcCC:
			{
				// We don't support creating a FourCC image
				return false;
			}
			break;
		case EImageFormatConstant_Format_CB4x4_Interpolate_8:
		case EImageFormatConstant_Format_CB4x4_Interpolate_8_8:
		case EImageFormatConstant_Format_CB4x4_Interpolate_5_6_5:
			{
				PhysicalWidth = fg_AlignUp(PhysicalWidth, 4);
				PhysicalHeight = fg_AlignUp(PhysicalHeight, 4);

				if (!StrideX)
					StrideX = fg_AlignUp(((PhysicalWidth * 4) >> 4) * 8, StrideAlign);
				if (!StrideY)
					StrideY = fg_AlignUp((PhysicalHeight * StrideX) >> 2,StrideAlign);
				if (!StrideZ)
					StrideZ = fg_AlignUp(PhysicalDepth * StrideY, StrideAlign);
			}
			break;
		case EImageFormatConstant_Format_CB4x4_Interpolate_5_6_5_Interpolate_8:
		case EImageFormatConstant_Format_CB4x4_Interpolate_5_6_5_Raw_4:
		case EImageFormatConstant_Format_CB4x4_Interpolate_8_Interpolate_8:
			{
				PhysicalWidth = fg_AlignUp(PhysicalWidth, 4);
				PhysicalHeight = fg_AlignUp(PhysicalHeight, 4);

				if (!StrideX)
					StrideX = fg_AlignUp(((PhysicalWidth * 4) >> 4) * 16, StrideAlign);
				if (!StrideY)
					StrideY = fg_AlignUp((PhysicalHeight * StrideX) >> 2, StrideAlign);
				if (!StrideZ)
					StrideZ = fg_AlignUp(PhysicalDepth * StrideY, StrideAlign);
			}
			break;
			break;
		}


		m_Stride[0] = StrideX;
		m_Stride[1] = StrideY;
		m_Stride[2] = StrideZ;
		uint32 TotalSize = StrideZ * PhysicalArrays;

		m_Data.f_SetLen(TotalSize);
		return true;
	}


	// This is a quickly written, crappy, slightly generic image conversion system.
	// Used to provide RGB(A) -> BGR(A) conversions.

	// Decode to RGBA

	template <uint64 _InputPixelType, typename t_ConversionComponentType>
	inline_always void fs_DecodePixel(uint8 const* _pIn, t_ConversionComponentType _Dest[4])
	{
		_Dest[0] = 0;
		_Dest[1] = 0;
		_Dest[2] = 0;
		_Dest[3] = 0;
	}

	template <>
	inline_always void fs_DecodePixel<DMibGraphicsImageFormat(R8G8B8_UNorm), uint8>(uint8 const* _pIn, uint8 _Dest[4])
	{
		_Dest[0] = _pIn[0];
		_Dest[1] = _pIn[1];
		_Dest[2] = _pIn[2];
		_Dest[3] = 255;
	}

	template <>
	inline_always void fs_DecodePixel<DMibGraphicsImageFormat(B8G8R8_UNorm), uint8>(uint8 const* _pIn, uint8 _Dest[4])
	{
		_Dest[0] = _pIn[2];
		_Dest[1] = _pIn[1];
		_Dest[2] = _pIn[0];
		_Dest[3] = 255;
	}

	template <>
	inline_always void fs_DecodePixel<DMibGraphicsImageFormat(R8G8B8A8_UNorm), uint8>(uint8 const* _pIn, uint8 _Dest[4])
	{
		_Dest[0] = _pIn[0];
		_Dest[1] = _pIn[1];
		_Dest[2] = _pIn[2];
		_Dest[3] = _pIn[3];
	}

	template <>
	inline_always void fs_DecodePixel<DMibGraphicsImageFormat(B8G8R8A8_UNorm), uint8>(uint8 const* _pIn, uint8 _Dest[4])
	{
		_Dest[0] = _pIn[2];
		_Dest[1] = _pIn[1];
		_Dest[2] = _pIn[0];
		_Dest[3] = _pIn[3];
	}

	// Encode from RGBA

	template <typename t_ConversionComponentType, uint64 _OutputPixelType>
	inline_always void fs_EncodePixel(t_ConversionComponentType _Dest[4], uint8 * _pOut)
	{
	}

	template <>
	inline_always void fs_EncodePixel<uint8, DMibGraphicsImageFormat(R8G8B8_UNorm)>(uint8 _Src[4], uint8 * _pOut)
	{
		_pOut[0] = _Src[0];
		_pOut[1] = _Src[1];
		_pOut[2] = _Src[2];
	}

	template <>
	inline_always void fs_EncodePixel<uint8, DMibGraphicsImageFormat(B8G8R8_UNorm)>(uint8 _Src[4], uint8 * _pOut)
	{
		_pOut[0] = _Src[2];
		_pOut[1] = _Src[1];
		_pOut[2] = _Src[0];
	}

	template <>
	inline_always void fs_EncodePixel<uint8, DMibGraphicsImageFormat(R8G8B8A8_UNorm)>(uint8 _Src[4], uint8 * _pOut)
	{
		_pOut[0] = _Src[0];
		_pOut[1] = _Src[1];
		_pOut[2] = _Src[2];
		_pOut[3] = _Src[3];
	}

	template <>
	inline_always void fs_EncodePixel<uint8, DMibGraphicsImageFormat(B8G8R8A8_UNorm)>(uint8 _Src[4], uint8 * _pOut)
	{
		_pOut[0] = _Src[2];
		_pOut[1] = _Src[1];
		_pOut[2] = _Src[0];
		_pOut[3] = _Src[3];
	}

	template <uint64 _InputPixelType, uint64 _OutputPixelType, typename t_ConversionComponentType>
	inline_always bool fs_Convert(CImage &_From, CImage &_To)
	{
		CImage::CLockedData LockSrc;
		CImage::CLockedData LockDest;
		bool bRet = false;
		if (_From.f_LockRead(LockSrc))
		{
			if (_To.f_LockWrite(LockDest))
			{

				uint32 Width = _From.m_Dimensions.f_GetWidth();
				uint32 Height = _From.m_Dimensions.f_GetHeight();
				uint32 Depth = _From.m_Dimensions.f_GetDepth();
				uint32 Arrays = _From.m_Dimensions.f_GetNumArrays();

				uint32 SrcStride[3] = {uint32(LockSrc.m_Stride[0]), uint32(LockSrc.m_Stride[1]), uint32(LockSrc.m_Stride[2])};
				uint32 DestStride[3] = {uint32(LockDest.m_Stride[0]), uint32(LockDest.m_Stride[1]), uint32(LockDest.m_Stride[2])};

				uint32 SrcPixelStride =(DMibGraphicsImageFormat_Get_ComponentNumBits(DMibGraphicsImageFormat_Get_Component(_InputPixelType, 0)) +
										DMibGraphicsImageFormat_Get_ComponentNumBits(DMibGraphicsImageFormat_Get_Component(_InputPixelType, 1)) +
										DMibGraphicsImageFormat_Get_ComponentNumBits(DMibGraphicsImageFormat_Get_Component(_InputPixelType, 2)) +
										DMibGraphicsImageFormat_Get_ComponentNumBits(DMibGraphicsImageFormat_Get_Component(_InputPixelType, 3)) ) >> 3;
				uint32 DestPixelStride =(DMibGraphicsImageFormat_Get_ComponentNumBits(DMibGraphicsImageFormat_Get_Component(_OutputPixelType, 0)) +
										 DMibGraphicsImageFormat_Get_ComponentNumBits(DMibGraphicsImageFormat_Get_Component(_OutputPixelType, 1)) +
										 DMibGraphicsImageFormat_Get_ComponentNumBits(DMibGraphicsImageFormat_Get_Component(_OutputPixelType, 2)) +
										 DMibGraphicsImageFormat_Get_ComponentNumBits(DMibGraphicsImageFormat_Get_Component(_OutputPixelType, 3)) ) >> 3;

				t_ConversionComponentType Intermediate[4];

				for (mint a = 0; a < Arrays; ++a)
				{
					for (mint z = 0; z < Depth; ++z)
					{
						for (mint y = 0; y < Height; ++y)
						{
							for (mint x = 0; x < Width; ++x)
							{
								const uint8 *pSrc = ((const uint8 *)LockSrc.m_pData) + x*SrcPixelStride + y*SrcStride[0] + z*SrcStride[1] + a*SrcStride[2];
								uint8 *pDst = (uint8 *)LockDest.m_pData +  x*DestPixelStride + y*DestStride[0] + z*DestStride[1] + a*DestStride[2];

								fs_DecodePixel<_InputPixelType, t_ConversionComponentType>(pSrc, Intermediate);
								fs_EncodePixel<t_ConversionComponentType, _OutputPixelType>(Intermediate, pDst);
							}
						}
					}
				}

				bRet = true;
				_To.f_UnLock(LockDest);
			}
			_From.f_UnLock(LockSrc);
		}

		return bRet;
	}


	bool CImage::f_ConvertInto(CImage &_To)
	{
		// Only same dimensions
		if (_To.m_Dimensions != m_Dimensions)
			return false;

		uint64 SrcFormat = m_ImageFormat;
		uint64 DstFormat = _To.m_ImageFormat;

		if (SrcFormat == DstFormat)
		{
			// Here we just need to copy the data over
			CLockedData LockSrc;
			CLockedData LockDst;
			if (f_LockRead(LockSrc))
			{
				if (_To.f_LockWrite(LockDst))
				{
					uint32 Height = m_Dimensions.f_GetHeight();
					uint32 Depth = m_Dimensions.f_GetDepth();
					uint32 Arrays = m_Dimensions.f_GetNumArrays();

					uint32 SrcStride[3] = {uint32(LockSrc.m_Stride[0]), uint32(LockSrc.m_Stride[1]), uint32(LockSrc.m_Stride[2])};
					uint32 DstStride[3] = {uint32(LockDst.m_Stride[0]), uint32(LockDst.m_Stride[1]), uint32(LockDst.m_Stride[2])};
					uint32 CpyStride = fg_Min(SrcStride[0], DstStride[0]);


					for (mint a = 0; a < Arrays; ++a)
					{
						for (mint z = 0; z < Depth; ++z)
						{
							const uint8 *pSrc = ((const uint8 *)LockSrc.m_pData) + z*SrcStride[1] + a*SrcStride[2];
							uint8 *pDst = (uint8 *)LockDst.m_pData + z*DstStride[1] + a*DstStride[2];

							for (mint y = 0; y < Height; ++y)
							{
								NMemory::fg_MemCopy(pDst, pSrc, CpyStride);
								pDst += DstStride[0];
								pSrc += SrcStride[0];
							}
						}
					}

					_To.f_UnLock(LockDst);
					f_UnLock(LockSrc);
					return true;
				}
				f_UnLock(LockSrc);
			}
			return false;
		}


#define MAKE_CONVERSION(_FromFormat, _ToFormat, _CompType)					\
		else if (SrcFormat == _FromFormat && DstFormat == _ToFormat)	\
		{																\
			return fs_Convert<_FromFormat, _ToFormat, _CompType>(*this, _To);	\
		}

	if (0) {}
	MAKE_CONVERSION(DMibGraphicsImageFormat(B8G8R8A8_UNorm), DMibGraphicsImageFormat(R8G8B8A8_UNorm), uint8)
	MAKE_CONVERSION(DMibGraphicsImageFormat(B8G8R8A8_UNorm), DMibGraphicsImageFormat(R8G8B8_UNorm), uint8)
	MAKE_CONVERSION(DMibGraphicsImageFormat(B8G8R8A8_UNorm), DMibGraphicsImageFormat(B8G8R8_UNorm), uint8)

	MAKE_CONVERSION(DMibGraphicsImageFormat(B8G8R8_UNorm), DMibGraphicsImageFormat(B8G8R8A8_UNorm), uint8)
	MAKE_CONVERSION(DMibGraphicsImageFormat(B8G8R8_UNorm), DMibGraphicsImageFormat(R8G8B8A8_UNorm), uint8)
	MAKE_CONVERSION(DMibGraphicsImageFormat(B8G8R8_UNorm), DMibGraphicsImageFormat(R8G8B8_UNorm), uint8)

	MAKE_CONVERSION(DMibGraphicsImageFormat(R8G8B8A8_UNorm), DMibGraphicsImageFormat(B8G8R8A8_UNorm), uint8)
	MAKE_CONVERSION(DMibGraphicsImageFormat(R8G8B8A8_UNorm), DMibGraphicsImageFormat(R8G8B8_UNorm), uint8)
	MAKE_CONVERSION(DMibGraphicsImageFormat(R8G8B8A8_UNorm), DMibGraphicsImageFormat(B8G8R8_UNorm), uint8)

	MAKE_CONVERSION(DMibGraphicsImageFormat(R8G8B8_UNorm), DMibGraphicsImageFormat(B8G8R8A8_UNorm), uint8)
	MAKE_CONVERSION(DMibGraphicsImageFormat(R8G8B8_UNorm), DMibGraphicsImageFormat(R8G8B8A8_UNorm), uint8)
	MAKE_CONVERSION(DMibGraphicsImageFormat(R8G8B8_UNorm), DMibGraphicsImageFormat(B8G8R8_UNorm), uint8)
	else
	{

		uint32 SrcPhysical = DMibGraphicsImageFormat_Get_PhysicalFormat(SrcFormat);
		uint32 DstPhysical = DMibGraphicsImageFormat_Get_PhysicalFormat(DstFormat);
		bool bIntOnlyPath = false;
		if (SrcPhysical == EImageFormatConstant_Format_Int && DstPhysical == EImageFormatConstant_Format_Int)
			bIntOnlyPath = true;

		if (bIntOnlyPath)
		{
			CLockedData LockSrc;
			CLockedData LockDst;
			if (f_LockRead(LockSrc))
			{
				if (_To.f_LockWrite(LockDst))
				{
					uint32 Height = m_Dimensions.f_GetHeight();
					uint32 Depth = m_Dimensions.f_GetDepth();
					uint32 Arrays = m_Dimensions.f_GetNumArrays();

					uint32 SrcStride[3] = {uint32(LockSrc.m_Stride[0]), uint32(LockSrc.m_Stride[1]), uint32(LockSrc.m_Stride[2])};
					uint32 DstStride[3] = {uint32(LockDst.m_Stride[0]), uint32(LockDst.m_Stride[1]), uint32(LockDst.m_Stride[2])};
					uint32 CpyStride = fg_Min(SrcStride[0], DstStride[0]);

					for (mint a = 0; a < Arrays; ++a)
					{
						for (mint z = 0; z < Depth; ++z)
						{
							const uint8 *pSrc = ((const uint8 *)LockSrc.m_pData) + z*SrcStride[1] + a*SrcStride[2];
							uint8 *pDst = (uint8 *)LockDst.m_pData + z*DstStride[1] + a*DstStride[2];

							for (mint y = 0; y < Height; ++y)
							{
								NMemory::fg_MemCopy(pDst, pSrc, CpyStride);
								pDst += DstStride[0];
								pSrc += SrcStride[0];
							}
						}
					}

					_To.f_UnLock(LockDst);
					f_UnLock(LockSrc);
					return true;
				}
				f_UnLock(LockSrc);
			}
		}
		else
		{
			CImageMemory TempImage;

			CImage *pDstImage = &_To;

			if (DstFormat != DMibGraphicsImageFormat(R32G32B32A32_Float))
			{
				TempImage.f_Create(DMibGraphicsImageFormat(R32G32B32A32_Float), _To.m_Dimensions);
				pDstImage = &TempImage;
			}

			switch (SrcFormat)
			{
			case DMibGraphicsImageFormat(R32G32B32A32_Float):
				break;
			case DMibGraphicsImageFormat(R32G32B32A32_UInt):
				fs_ConvertImage<DMibGraphicsImageFormat(R32G32B32A32_Float), DMibGraphicsImageFormat(R32G32B32A32_UInt)>(*this, *pDstImage);
				break;
			case 0:
				break;
			}
		}
		return false;
	}

#undef MAKECONVERSION

	return false;
	}


	bool CImageMemory::fs_ReadImage(CImageMemory &_Destination, NStr::CStr _FileName)
	{
		NMib::CRunTimeObjectInfo *pTests = fg_GetRuntimeTypeInfo("NMib::NGraphics::CImageIO");
		NFile::TCBinaryStreamFile<> Stream;
		Stream.f_Open(_FileName, NFile::EFileOpen_Read|NFile::EFileOpen_ShareAll);

		if (pTests)
		{
			auto Iter = pTests->m_Children.f_GetIter();

			while (Iter)
			{
				NMib::NStorage::TCUniquePointer<CImageIO> pObject((CImageIO *)Iter->f_CreateObject());
				if (pObject)
				{
					if (pObject->f_DetectFormatFromFilename(_FileName))
					{
						Stream.f_SetPosition(0);
						if (pObject->f_DetectFormatFromStream(Stream))
						{
							Stream.f_SetPosition(0);
							bool bRet = pObject->f_ReadImage(_Destination, Stream);
							return bRet;
						}
					}
				}

				++Iter;
			}
		}
		return false;
	}

	bool CImageMemory::fs_ReadImage(CImageMemory &_Destination, NStream::CBinaryStream &_Stream)
	{
		NMib::CRunTimeObjectInfo *pTests = fg_GetRuntimeTypeInfo("NMib::NGraphics::CImageIO");

		if (pTests)
		{
			auto Iter = pTests->m_Children.f_GetIter();

			while (Iter)
			{
				NMib::NStorage::TCUniquePointer<CImageIO> pObject((CImageIO *)Iter->f_CreateObject());
				if (pObject)
				{
					_Stream.f_SetPosition(0);
					if (pObject->f_DetectFormatFromStream(_Stream))
					{
						_Stream.f_SetPosition(0);
						bool bRet = pObject->f_ReadImage(_Destination, _Stream);
						return bRet;
					}
				}

				++Iter;
			}
		}
		return false;
	}

	bool CImage::fs_WriteImage(CImage &_Source, NStr::CStr _FileName)
	{
		NMib::CRunTimeObjectInfo *pTests = fg_GetRuntimeTypeInfo("NMib::NGraphics::CImageIO");
		bool bRet = false;

		if (pTests)
		{
			auto Iter = pTests->m_Children.f_GetIter();

			while (Iter && !bRet)
			{
				NMib::NStorage::TCUniquePointer<CImageIO> pObject((CImageIO *)Iter->f_CreateObject());
				if (pObject)
				{
					if (pObject->f_DetectFormatFromFilename(_FileName))
					{
						NFile::TCBinaryStreamFile<> Stream;
						Stream.f_Open(_FileName, NFile::EFileOpen_Write | NFile::EFileOpen_ShareAll);
						bRet = pObject->f_WriteImage(_Source, Stream);
					}
				}
				++Iter;
			}
		}
		return bRet;
	}

	bool CImage::fs_WriteImage(CImage &_Source, NStream::CBinaryStream &_Stream, NStr::CStr _Type)
	{
		NMib::CRunTimeObjectInfo *pTests = fg_GetRuntimeTypeInfo("NMib::NGraphics::CImageIO");

		bool bRet = false;
		if (pTests)
		{
			auto Iter = pTests->m_Children.f_GetIter();

			while (Iter && !bRet)
			{
				NMib::NStorage::TCUniquePointer<CImageIO> pObject((CImageIO *)Iter->f_CreateObject());
				if (pObject)
				{
					if (pObject->f_DetectFormatFromFilename(_Type))
					{
						bRet = pObject->f_WriteImage(_Source, _Stream);
					}
				}
				++Iter;
			}
		}

		return bRet;
	}

	bool CImage::f_FlipHorizontal(CImage &_Dst)
	{
		if (_Dst.m_Dimensions != m_Dimensions)
			return false;

		if (_Dst.m_ImageFormat != m_ImageFormat)
			return false;

		CLockedData LockSrc;
		CLockedData LockDst;
		if (f_LockRead(LockSrc))
		{
			if (_Dst.f_LockWrite(LockDst))
			{
				uint32 Height = m_Dimensions.f_GetHeight();
				uint32 Depth = m_Dimensions.f_GetDepth();
				uint32 Arrays = m_Dimensions.f_GetNumArrays();

				uint32 SrcStride[3] = {uint32(LockSrc.m_Stride[0]), uint32(LockSrc.m_Stride[1]), uint32(LockSrc.m_Stride[2])};
				uint32 DstStride[3] = {uint32(LockDst.m_Stride[0]), uint32(LockDst.m_Stride[1]), uint32(LockDst.m_Stride[2])};
				uint32 CpyStride = fg_Min(SrcStride[0], DstStride[0]);

				for (mint a = 0; a < Arrays; ++a)
				{
					for (mint z = 0; z < Depth; ++z)
					{
						const uint8 *pSrc = ((const uint8 *)LockSrc.m_pData) + z*SrcStride[1] + a*SrcStride[2];
						uint8 *pDst = (uint8 *)LockDst.m_pData + z*DstStride[1] + a*DstStride[2] + (Height - 1) * DstStride[0];

						for (mint y = 0; y < Height; ++y)
						{
							NMemory::fg_MemCopy(pDst, pSrc, CpyStride);
							pDst -= DstStride[0];
							pSrc += SrcStride[0];
						}
					}
				}

				_Dst.f_UnLock(LockDst);
				f_UnLock(LockSrc);
				return true;
			}
			f_UnLock(LockSrc);
		}
		return false;
	}

	template <typename t_Func>
	bool CImage::fp_StretchHalfXY_Core(CImage &_Dst, t_Func _Func)
	{
		CLockedData LockSrc;
		CLockedData LockDst;
		if (f_LockRead(LockSrc))
		{
			if (_Dst.f_LockWrite(LockDst))
			{
				uint32 Width = _Dst.m_Dimensions.f_GetWidth();
				uint32 Height = _Dst.m_Dimensions.f_GetHeight();
				uint32 Depth = _Dst.m_Dimensions.f_GetDepth();
				uint32 Arrays = _Dst.m_Dimensions.f_GetNumArrays();

				uint32 SrcStride[3] = {uint32(LockSrc.m_Stride[0]), uint32(LockSrc.m_Stride[1]), uint32(LockSrc.m_Stride[2])};
				uint32 DstStride[3] = {uint32(LockDst.m_Stride[0]), uint32(LockDst.m_Stride[1]), uint32(LockDst.m_Stride[2])};

				for (mint a = 0; a < Arrays; ++a)
				{
					for (mint z = 0; z < Depth; ++z)
					{
						const uint8 *pSrc = ((const uint8 *)LockSrc.m_pData) + z*SrcStride[1] + a*SrcStride[2];
						uint8 *pDst = (uint8 *)LockDst.m_pData + z*DstStride[1] + a*DstStride[2];

						for (mint y = 0; y < Height; ++y)
						{

							_Func(pSrc, pDst, Width, SrcStride[0]);

							pSrc += SrcStride[0]*2;
							pDst += DstStride[0];
						}

						DMibSafeCheck((pDst <= (uint8 *)LockDst.m_pData + DstStride[0] * Height), "");
					}
				}

				_Dst.f_UnLock(LockDst);
				f_UnLock(LockSrc);
				return true;
			}
			f_UnLock(LockSrc);
		}
		return false;
	}

	bool CImage::f_StretchHalfXY(CImage &_Dst)
	{
		CImageDimensions Dims(fg_Max(m_Dimensions[0]/2, 1), fg_Max(m_Dimensions[1]/2, 1), m_Dimensions[2], m_Dimensions[3]);
		if (_Dst.m_Dimensions != Dims)
			return false;

		if (_Dst.m_ImageFormat != m_ImageFormat)
			return false;

		switch (m_ImageFormat)
		{
		case DMibGraphicsImageFormat(R8G8B8_UNorm):
		case DMibGraphicsImageFormat(B8G8R8_UNorm):
			return fp_StretchHalfXY_Core(_Dst, [](uint8 const* pSrc, uint8* pDst, mint Width, uint32 SrcStride0){
				const uint8 *pSrc0 = (const uint8 *)pSrc;
				const uint8 *pSrc1 = (const uint8 *)(pSrc + SrcStride0);
				uint8 *pDst0 = (uint8 *)pDst;
				for (mint x = 0; x < Width; ++x)
				{
					uint32 Pix0R = pSrc0[x*6 + 0];
					uint32 Pix0G = pSrc0[x*6 + 1];
					uint32 Pix0B = pSrc0[x*6 + 2];
					uint32 Pix1R = pSrc0[x*6 + 3];
					uint32 Pix1G = pSrc0[x*6 + 4];
					uint32 Pix1B = pSrc0[x*6 + 5];
					uint32 Pix2R = pSrc1[x*6 + 0];
					uint32 Pix2G = pSrc1[x*6 + 1];
					uint32 Pix2B = pSrc1[x*6 + 2];
					uint32 Pix3R = pSrc1[x*6 + 3];
					uint32 Pix3G = pSrc1[x*6 + 4];
					uint32 Pix3B = pSrc1[x*6 + 5];

					uint32 Comp0 = (Pix0R + Pix1R + Pix2R + Pix3R) >> 2;
					uint32 Comp1 = (Pix0G + Pix1G + Pix2G + Pix3G) >> 2;
					uint32 Comp2 = (Pix0B + Pix1B + Pix2B + Pix3B) >> 2;

					pDst0[x*3+0] = Comp0;
					pDst0[x*3+1] = Comp1;
					pDst0[x*3+2] = Comp2;
				}
			});
			break;
		case DMibGraphicsImageFormat(R8G8B8A8_UNorm):
		case DMibGraphicsImageFormat(B8G8R8A8_UNorm):
			return fp_StretchHalfXY_Core(_Dst, [](uint8 const* pSrc, uint8* pDst, mint Width, uint32 SrcStride0){
				const uint32 *pSrc0 = (const uint32 *)pSrc;
				const uint32 *pSrc1 = (const uint32 *)(pSrc + SrcStride0);
				uint32 *pDst0 = (uint32 *)pDst;
				for (mint x = 0; x < Width; ++x)
				{
					uint32 Pix0 = pSrc0[x*2 + 0];
					uint32 Pix1 = pSrc0[x*2 + 1];
					uint32 Pix2 = pSrc1[x*2 + 0];
					uint32 Pix3 = pSrc1[x*2 + 1];

					uint32 Comp0 = (((Pix0>>0) & 0xff) + ((Pix1>>0) & 0xff) + ((Pix2>>0) & 0xff) + ((Pix3>>0) & 0xff)) >> 2;
					uint32 Comp1 = (((Pix0>>8) & 0xff) + ((Pix1>>8) & 0xff) + ((Pix2>>8) & 0xff) + ((Pix3>>8) & 0xff)) >> 2;
					uint32 Comp2 = (((Pix0>>16) & 0xff) + ((Pix1>>16) & 0xff) + ((Pix2>>16) & 0xff) + ((Pix3>>16) & 0xff)) >> 2;
					uint32 Comp3 = (((Pix0>>24) & 0xff) + ((Pix1>>24) & 0xff) + ((Pix2>>24) & 0xff) + ((Pix3>>24) & 0xff)) >> 2;

					pDst0[x] = Comp0 | (Comp1 << 8) | (Comp2 << 16) | (Comp3 << 24);
				}
			});
			break;
		default:
			return false;
		}
	}

	bool CImage::f_Blend(CImage &_Dst, fp64 _SrcBlend, fp64 _DstBlend)
	{
		if (_Dst.m_Dimensions != m_Dimensions)
			return false;

		if (_Dst.m_ImageFormat != m_ImageFormat)
			return false;

		switch (m_ImageFormat)
		{
		case DMibGraphicsImageFormat(R8G8B8_UNorm):
		case DMibGraphicsImageFormat(B8G8R8_UNorm):
		case DMibGraphicsImageFormat(R8G8B8A8_UNorm):
		case DMibGraphicsImageFormat(B8G8R8A8_UNorm):
			break;
		default:
			return false;
		}
		enum
		{
			EPrecissionBits = 16,
			EPrecission = 1 << EPrecissionBits
		};

		uint32 SrcFraction = (_SrcBlend * fp64(EPrecission)).f_ToIntRound();
		uint32 DstFraction = (_DstBlend * fp64(EPrecission)).f_ToIntRound();;

		CLockedData LockSrc;
		CLockedData LockDst;
		if (f_LockRead(LockSrc))
		{
			if (_Dst.f_LockWrite(LockDst))
			{
				uint32 Width = _Dst.m_Dimensions.f_GetWidth();
				uint32 Height = _Dst.m_Dimensions.f_GetHeight();
				uint32 Depth = _Dst.m_Dimensions.f_GetDepth();
				uint32 Arrays = _Dst.m_Dimensions.f_GetNumArrays();

				uint32 SrcStride[3] = {uint32(LockSrc.m_Stride[0]), uint32(LockSrc.m_Stride[1]), uint32(LockSrc.m_Stride[2])};
				uint32 DstStride[3] = {uint32(LockDst.m_Stride[0]), uint32(LockDst.m_Stride[1]), uint32(LockDst.m_Stride[2])};

				for (mint a = 0; a < Arrays; ++a)
				{
					for (mint z = 0; z < Depth; ++z)
					{
						const uint8 *pSrc = ((const uint8 *)LockSrc.m_pData) + z*SrcStride[1] + a*SrcStride[2];
						uint8 *pDst = (uint8 *)LockDst.m_pData + z*DstStride[1] + a*DstStride[2];

						for (mint y = 0; y < Height; ++y)
						{
							switch (m_ImageFormat)
							{
							case DMibGraphicsImageFormat(R8G8B8_UNorm):
							case DMibGraphicsImageFormat(B8G8R8_UNorm):
								{
									const uint8 *pSrc0 = (const uint8 *)pSrc;
									uint8 *pDst0 = (uint8 *)pDst;
									for (mint x = 0; x < Width; ++x)
									{
										uint32 Pix0R = pSrc0[x*3 + 0];
										uint32 Pix0G = pSrc0[x*3 + 1];
										uint32 Pix0B = pSrc0[x*3 + 2];

										uint32 Pix1R = pDst0[x*3 + 0];
										uint32 Pix1G = pDst0[x*3 + 1];
										uint32 Pix1B = pDst0[x*3 + 2];

										uint64 ResultR = (Pix0R * SrcFraction + Pix1R * DstFraction) >> EPrecissionBits;
										uint64 ResultG = (Pix0G * SrcFraction + Pix1G * DstFraction) >> EPrecissionBits;
										uint64 ResultB = (Pix0B * SrcFraction + Pix1B * DstFraction) >> EPrecissionBits;

										pDst0[x*3+0] = ResultR;
										pDst0[x*3+1] = ResultG;
										pDst0[x*3+2] = ResultB;
									}
								}
								break;
							case DMibGraphicsImageFormat(R8G8B8A8_UNorm):
							case DMibGraphicsImageFormat(B8G8R8A8_UNorm):
								{
									const uint32 *pSrc0 = (const uint32 *)pSrc;
									uint32 *pDst0 = (uint32 *)pDst;
									for (mint x = 0; x < Width; ++x)
									{
										uint32 Pix0 = pSrc0[x];
										uint32 Pix1 = pDst0[x];

										uint32 Pix0R = ((Pix0 >> 0) & 0xff);
										uint32 Pix0G = ((Pix0 >> 8) & 0xff);
										uint32 Pix0B = ((Pix0 >> 16) & 0xff);
										uint32 Pix0A = ((Pix0 >> 24) & 0xff);
										uint32 Pix1R = ((Pix1 >> 0) & 0xff);
										uint32 Pix1G = ((Pix1 >> 8) & 0xff);
										uint32 Pix1B = ((Pix1 >> 16) & 0xff);
										uint32 Pix1A = ((Pix1 >> 24) & 0xff);

										uint64 ResultR = (Pix0R * SrcFraction + Pix1R * DstFraction) >> EPrecissionBits;
										uint64 ResultG = (Pix0G * SrcFraction + Pix1G * DstFraction) >> EPrecissionBits;
										uint64 ResultB = (Pix0B * SrcFraction + Pix1B * DstFraction) >> EPrecissionBits;
										uint64 ResultA = (Pix0A * SrcFraction + Pix1A * DstFraction) >> EPrecissionBits;

										pDst0[x] = ResultR | (ResultG << 8) | (ResultB << 16) | (ResultA << 24);
									}
								}
								break;
							}
							pSrc += SrcStride[0];
							pDst += DstStride[0];
						}

						DMibSafeCheck((pDst <= (uint8 *)LockDst.m_pData + DstStride[0] * Height), "");
					}
				}

				_Dst.f_UnLock(LockDst);
				f_UnLock(LockSrc);
				return true;
			}
			f_UnLock(LockSrc);
		}
		return false;
	}

	bool CImage::f_StretchBilinear(CImage &_Dst, fp64 _OffsetX, fp64 _OffsetY)
	{
		CImageDimensions Dims = _Dst.m_Dimensions;
		if (_Dst.m_ImageFormat != m_ImageFormat)
			return false;

		if (m_Dimensions.f_GetWidth() > 1 << 16 || m_Dimensions.f_GetHeight() > 1 << 16)
			return false;

		switch (m_ImageFormat)
		{
		case DMibGraphicsImageFormat(R8G8B8_UNorm):
		case DMibGraphicsImageFormat(B8G8R8_UNorm):
		case DMibGraphicsImageFormat(R8G8B8A8_UNorm):
		case DMibGraphicsImageFormat(B8G8R8A8_UNorm):
			break;
		default:
			return false;
		}

		CLockedData LockSrc;
		CLockedData LockDst;
		if (f_LockRead(LockSrc))
		{
			if (_Dst.f_LockWrite(LockDst))
			{
				int32 SrcWidth = m_Dimensions.f_GetWidth();
				int32 SrcHeight = m_Dimensions.f_GetHeight();

				uint32 Width = _Dst.m_Dimensions.f_GetWidth();
				uint32 Height = _Dst.m_Dimensions.f_GetHeight();
				uint32 Depth = _Dst.m_Dimensions.f_GetDepth();
				uint32 Arrays = _Dst.m_Dimensions.f_GetNumArrays();

				uint32 SrcStride[3] = {uint32(LockSrc.m_Stride[0]), uint32(LockSrc.m_Stride[1]), uint32(LockSrc.m_Stride[2])};
				uint32 DstStride[3] = {uint32(LockDst.m_Stride[0]), uint32(LockDst.m_Stride[1]), uint32(LockDst.m_Stride[2])};

				typedef int32 CLargeType;
				typedef uint32 CLargeTypeU;
				enum
				{
					EPrecissionBits = 8,
					EPrecission = 1 << EPrecissionBits
				};

				int32 SourceXOffset = (_OffsetX * EPrecission).f_ToIntRound();
				int32 SourceYOffset = (_OffsetY * EPrecission).f_ToIntRound();

				uint32 SourceYAdd = (SrcHeight*EPrecission) / Height;
				uint32 SourceXAdd = (SrcWidth*EPrecission) / Width;

				for (mint a = 0; a < Arrays; ++a)
				{
					for (mint z = 0; z < Depth; ++z)
					{
						const uint8 *pSrc = ((const uint8 *)LockSrc.m_pData) + z*SrcStride[1] + a*SrcStride[2];
						uint8 *pDst = (uint8 *)LockDst.m_pData + z*DstStride[1] + a*DstStride[2];

						CLargeType SourceY = SourceYOffset;
						for (mint y = 0; y < Height; ++y)
						{
							int32 SrcY0 = fg_Clamp(SourceY >> EPrecissionBits, 0, SrcHeight-1);
							int32 SrcY1 = fg_Clamp(SrcY0 + 1, 0, SrcHeight-1);
							uint32 SrcYFractionInv = SourceY - (SrcY0 << EPrecissionBits);
							uint32 SrcYFraction = EPrecission - SrcYFractionInv;
							switch (m_ImageFormat)
							{
							case DMibGraphicsImageFormat(R8G8B8_UNorm):
							case DMibGraphicsImageFormat(B8G8R8_UNorm):
								{
									const uint8 *pSrc0 = (const uint8 *)(pSrc + SrcStride[0] * SrcY0);
									const uint8 *pSrc1 = (const uint8 *)(pSrc + SrcStride[0] * SrcY1);
									uint8 *pDst0 = (uint8 *)pDst;

									CLargeType SourceX = SourceXOffset;

									mint ByteWidth = Width * 3;
									for (mint x = 0; x < ByteWidth; x+=3)
									{
										int32 SrcX0 = fg_Clamp((SourceX >> EPrecissionBits), 0, SrcWidth-1);
										int32 SrcX1 = fg_Clamp(SrcX0 + 1, 0, SrcWidth-1);
										uint32 SrcXFractionInv = SourceX - (SrcX0 << EPrecissionBits);
										uint32 SrcXFraction = EPrecission - SrcXFractionInv;
										SourceX += SourceXAdd;

										SrcX0 *= 3;
										SrcX1 *= 3;

										uint32 Pix0R = pSrc0[SrcX0 + 0];
										uint32 Pix0G = pSrc0[SrcX0 + 1];
										uint32 Pix0B = pSrc0[SrcX0 + 2];
										uint32 Pix1R = pSrc0[SrcX1 + 0];
										uint32 Pix1G = pSrc0[SrcX1 + 1];
										uint32 Pix1B = pSrc0[SrcX1 + 2];
										uint32 Pix2R = pSrc1[SrcX0 + 0];
										uint32 Pix2G = pSrc1[SrcX0 + 1];
										uint32 Pix2B = pSrc1[SrcX0 + 2];
										uint32 Pix3R = pSrc1[SrcX1 + 0];
										uint32 Pix3G = pSrc1[SrcX1 + 1];
										uint32 Pix3B = pSrc1[SrcX1 + 2];

										CLargeTypeU Result0R = (Pix0R * SrcXFraction + Pix1R * SrcXFractionInv);
										CLargeTypeU Result1R = (Pix2R * SrcXFraction + Pix3R * SrcXFractionInv);
										uint32 ResultR = (Result0R * SrcYFraction + Result1R * SrcYFractionInv) >> (EPrecissionBits * 2);

										CLargeTypeU Result0G = (Pix0G * SrcXFraction + Pix1G * SrcXFractionInv);
										CLargeTypeU Result1G = (Pix2G * SrcXFraction + Pix3G * SrcXFractionInv);
										uint32 ResultG = (Result0G * SrcYFraction + Result1G * SrcYFractionInv) >> (EPrecissionBits * 2);

										CLargeTypeU Result0B = (Pix0B * SrcXFraction + Pix1B * SrcXFractionInv);
										CLargeTypeU Result1B = (Pix2B * SrcXFraction + Pix3B * SrcXFractionInv);
										uint32 ResultB = (Result0B * SrcYFraction + Result1B * SrcYFractionInv) >> (EPrecissionBits * 2);

										pDst0[x+0] = ResultR;
										pDst0[x+1] = ResultG;
										pDst0[x+2] = ResultB;
									}
								}
								break;
							case DMibGraphicsImageFormat(R8G8B8A8_UNorm):
							case DMibGraphicsImageFormat(B8G8R8A8_UNorm):
								{
									const uint32 *pSrc0 = (const uint32 *)(pSrc + SrcStride[0] * SrcY0);
									const uint32 *pSrc1 = (const uint32 *)(pSrc + SrcStride[0] * SrcY1);
									uint32 *pDst0 = (uint32 *)pDst;
									CLargeType SourceX = SourceXOffset;
									for (mint x = 0; x < Width; ++x)
									{
										int32 SrcX0 = fg_Clamp((SourceX >> EPrecissionBits), 0, SrcWidth-1);
										int32 SrcX1 = fg_Clamp(SrcX0 + 1, 0, SrcWidth-1);
										uint32 SrcXFractionInv = SourceX - (SrcX0 << EPrecissionBits);
										uint32 SrcXFraction = EPrecission - SrcXFractionInv;
										SourceX += SourceXAdd;

										uint32 Pix0 = pSrc0[SrcX0];
										uint32 Pix1 = pSrc0[SrcX1];
										uint32 Pix2 = pSrc1[SrcX0];
										uint32 Pix3 = pSrc1[SrcX1];

										uint32 Pix0R = ((Pix0 >> 0) & 0xff);
										uint32 Pix0G = ((Pix0 >> 8) & 0xff);
										uint32 Pix0B = ((Pix0 >> 16) & 0xff);
										uint32 Pix0A = ((Pix0 >> 24) & 0xff);
										uint32 Pix1R = ((Pix1 >> 0) & 0xff);
										uint32 Pix1G = ((Pix1 >> 8) & 0xff);
										uint32 Pix1B = ((Pix1 >> 16) & 0xff);
										uint32 Pix1A = ((Pix1 >> 24) & 0xff);
										uint32 Pix2R = ((Pix2 >> 0) & 0xff);
										uint32 Pix2G = ((Pix2 >> 8) & 0xff);
										uint32 Pix2B = ((Pix2 >> 16) & 0xff);
										uint32 Pix2A = ((Pix2 >> 24) & 0xff);
										uint32 Pix3R = ((Pix3 >> 0) & 0xff);
										uint32 Pix3G = ((Pix3 >> 8) & 0xff);
										uint32 Pix3B = ((Pix3 >> 16) & 0xff);
										uint32 Pix3A = ((Pix3 >> 24) & 0xff);

										CLargeTypeU Result0R = (Pix0R * SrcXFraction + Pix1R * SrcXFractionInv);
										CLargeTypeU Result1R = (Pix2R * SrcXFraction + Pix3R * SrcXFractionInv);
										uint32 ResultR = (Result0R * SrcYFraction + Result1R * SrcYFractionInv) >> (EPrecissionBits * 2);

										CLargeTypeU Result0G = (Pix0G * SrcXFraction + Pix1G * SrcXFractionInv);
										CLargeTypeU Result1G = (Pix2G * SrcXFraction + Pix3G * SrcXFractionInv);
										uint32 ResultG = (Result0G * SrcYFraction + Result1G * SrcYFractionInv) >> (EPrecissionBits * 2);

										CLargeTypeU Result0B = (Pix0B * SrcXFraction + Pix1B * SrcXFractionInv);
										CLargeTypeU Result1B = (Pix2B * SrcXFraction + Pix3B * SrcXFractionInv);
										uint32 ResultB = (Result0B * SrcYFraction + Result1B * SrcYFractionInv) >> (EPrecissionBits * 2);

										CLargeTypeU Result0A = (Pix0A * SrcXFraction + Pix1A * SrcXFractionInv);
										CLargeTypeU Result1A = (Pix2A * SrcXFraction + Pix3A * SrcXFractionInv);
										uint32 ResultA = (Result0A * SrcYFraction + Result1A * SrcYFractionInv) >> (EPrecissionBits * 2);

										pDst0[x] = ResultR | (ResultG << 8) | (ResultB << 16) | (ResultA << 24);
									}
								}
								break;
							}
							pDst += DstStride[0];
							SourceY += SourceYAdd;
						}

						DMibSafeCheck((pDst <= (uint8 *)LockDst.m_pData + DstStride[0] * Height), "");
					}
				}

				_Dst.f_UnLock(LockDst);
				f_UnLock(LockSrc);
				return true;
			}
			f_UnLock(LockSrc);
		}
		return false;
	}

	uint32 CImage::f_PixelSize()
	{
		uint32 ComponentR = DMibGraphicsImageFormat_Get_Component(m_ImageFormat, 0);
		uint32 ComponentG = DMibGraphicsImageFormat_Get_Component(m_ImageFormat, 1);
		uint32 ComponentB = DMibGraphicsImageFormat_Get_Component(m_ImageFormat, 2);
		uint32 ComponentA = DMibGraphicsImageFormat_Get_Component(m_ImageFormat, 3);

		uint32 BitsR = DMibGraphicsImageFormat_Get_ComponentNumBits(ComponentR);
		uint32 BitsG = DMibGraphicsImageFormat_Get_ComponentNumBits(ComponentG);
		uint32 BitsB = DMibGraphicsImageFormat_Get_ComponentNumBits(ComponentB);
		uint32 BitsA = DMibGraphicsImageFormat_Get_ComponentNumBits(ComponentA);

		uint32 TotalBits = BitsR + BitsG + BitsB + BitsA;
		return TotalBits / 8;
	}


	bool CImage::f_RemoveAlpha(CImage &_Dst)
	{
		if (_Dst.m_Dimensions != m_Dimensions)
			return false;

		if (_Dst.m_ImageFormat != m_ImageFormat)
			return false;

		uint32 PhysicalFormat = DMibGraphicsImageFormat_Get_PhysicalFormat(m_ImageFormat);

		if (PhysicalFormat != EImageFormatConstant_Format_Int)
			return false;

		uint32 ComponentR = DMibGraphicsImageFormat_Get_Component(m_ImageFormat, 0);
		uint32 ComponentG = DMibGraphicsImageFormat_Get_Component(m_ImageFormat, 1);
		uint32 ComponentB = DMibGraphicsImageFormat_Get_Component(m_ImageFormat, 2);
		uint32 ComponentA = DMibGraphicsImageFormat_Get_Component(m_ImageFormat, 3);

		uint32 SComponentA = DMibGraphicsImageFormat_Get_ComponentSourceComponent(ComponentA);
		if (SComponentA > EImageFormatConstant_Component_3)
			return false; // No alpha

		uint32 BitsR = DMibGraphicsImageFormat_Get_ComponentNumBits(ComponentR);
		uint32 BitsG = DMibGraphicsImageFormat_Get_ComponentNumBits(ComponentG);
		uint32 BitsB = DMibGraphicsImageFormat_Get_ComponentNumBits(ComponentB);
		uint32 BitsA = DMibGraphicsImageFormat_Get_ComponentNumBits(ComponentA);
		if (BitsA > 32)
			return false; // Not supported

		uint32 TotalBits = BitsR + BitsG + BitsB + BitsA;
		uint32 AlphaBitPos = 0;
		if (SComponentA > EImageFormatConstant_Component_0)
			AlphaBitPos += BitsR;
		if (SComponentA > EImageFormatConstant_Component_1)
			AlphaBitPos += BitsG;
		if (SComponentA > EImageFormatConstant_Component_2)
			AlphaBitPos += BitsB;

		CLockedData LockSrc;
		CLockedData LockDst;
		if (f_LockRead(LockSrc))
		{
			if (_Dst.f_LockWrite(LockDst))
			{
				uint32 Width = m_Dimensions.f_GetWidth();
				uint32 Height = m_Dimensions.f_GetHeight();
				uint32 Depth = m_Dimensions.f_GetDepth();
				uint32 Arrays = m_Dimensions.f_GetNumArrays();

				uint32 SrcStride[3] = {uint32(LockSrc.m_Stride[0]), uint32(LockSrc.m_Stride[1]), uint32(LockSrc.m_Stride[2])};
				uint32 DstStride[3] = {uint32(LockDst.m_Stride[0]), uint32(LockDst.m_Stride[1]), uint32(LockDst.m_Stride[2])};

				for (mint a = 0; a < Arrays; ++a)
				{
					for (mint z = 0; z < Depth; ++z)
					{
						const uint8 *pSrc = ((const uint8 *)LockSrc.m_pData) + z*SrcStride[1] + a*SrcStride[2];
						uint8 *pDst = (uint8 *)LockDst.m_pData + z*DstStride[1] + a*DstStride[2];

						for (mint y = 0; y < Height; ++y)
						{

							if (m_ImageFormat == DMibGraphicsImageFormat(R8G8B8A8_UNorm) ||
								m_ImageFormat == DMibGraphicsImageFormat(B8G8R8A8_UNorm))
							{
								const uint32 *pSrc0 = (const uint32 *)pSrc;
								uint32 *pDst0 = (uint32 *)pDst;
								for (mint x = 0; x < Width; ++x)
								{
									pDst0[x] = pSrc0[x] | (0xff << 24);
								}
							}
							else
							{
								if (BitsA > 16)
								{
								}
								else if (BitsA > 8)
								{
								}
								else
								{
									for (mint x = 0; x < Width; ++x)
									{
										uint32 Pos = x * TotalBits + AlphaBitPos;
										uint32 BytePos = Pos >> 3;
										uint32 BitPos = Pos - (BytePos << 3);
										uint32 nBits0 = fg_Min(8 - BitPos, BitsA);
										uint32 nBits1 = fg_Min(BitPos, BitsA - nBits0);
										pDst[BytePos] = (pSrc[BytePos] & DMibBitRange(BitPos, BitPos + nBits0 - 1)) | ((1 << nBits0) - 1) << BitPos;
										if (nBits1)
											pDst[BytePos+1] = (pSrc[BytePos+1] & DMibBitRange(0, nBits1 - 1)) | ((1 << nBits1) - 1);
									}
								}
							}
							pDst += DstStride[0];
							pSrc += SrcStride[0];
						}
					}
				}

				_Dst.f_UnLock(LockDst);
				f_UnLock(LockSrc);
				return true;
			}
			f_UnLock(LockSrc);
		}
		return false;
	}
}
