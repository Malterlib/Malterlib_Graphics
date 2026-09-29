// Copyright © Unbroken AB
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "../../External/libpng/png.h"
#include "../../External/zlib/zlib.h"
#include <Mib/Graphics/Image>
#include <Mib/Core/RuntimeType>

#ifdef DCompiler_MSVC
	#pragma warning(disable:4611)
#endif

namespace NMib::NGraphics
{
	struct CImageIO_Png : public NMib::NGraphics::CImageIO
	{
		bool f_DetectFormatFromFilename(const NStr::CStr &_FileName) override
		{
			if (_FileName.f_FindNoCase(".png") >= 0)
				return true;
			return false;
		}

		enum
		{
			EPng_SigSize = 4,
		};

		static void calling_convention_c fs_PngRead(png_struct *_pPng, png_byte *_pBytes, png_size_t _nBytes)
		{
			NStream::CBinaryStream *pStream = (NStream::CBinaryStream *)png_get_io_ptr(_pPng);
			pStream->f_ConsumeBytes(_pBytes, _nBytes);
		}

		static void calling_convention_c fs_PngWrite(png_struct *_pPng, png_byte *_pBytes, png_size_t _nBytes)
		{
			NStream::CBinaryStream *pStream = (NStream::CBinaryStream *)png_get_io_ptr(_pPng);
			pStream->f_FeedBytes(_pBytes, _nBytes);
		}

		static void calling_convention_c fs_PngFlush(png_struct *_pPng)
		{
		}

		bool f_DetectFormatFromStream(NStream::CBinaryStream &_Stream) override
		{
			uint8 Buffer[EPng_SigSize];
			try
			{
				_Stream.f_ConsumeBytes(Buffer, EPng_SigSize);
				return png_sig_cmp(Buffer, 0, EPng_SigSize) == 0;
			}
			catch (NFile::CExceptionFile)
			{
				return false;
			}
		}

		static uint64 fs_PngFromatToCImageFormat(png_struct *_pPng, png_info *_pPngInfo)
		{
			switch( png_get_color_type(_pPng, _pPngInfo) )
			{
			case PNG_COLOR_TYPE_GRAY:
				{
					switch (png_get_bit_depth(_pPng, _pPngInfo))
					{
					case 1:	return DMibGraphicsImageFormat(R1_UNorm);
					case 2:	return DMibGraphicsImageFormat(R2_UNorm);
					case 4:	return DMibGraphicsImageFormat(R4_UNorm);
					case 8:	return DMibGraphicsImageFormat(R8_UNorm);
					case 16:return DMibGraphicsImageFormat(R16_UNorm);
					}
				}

			case PNG_COLOR_TYPE_RGB:
				{
					switch (png_get_bit_depth(_pPng, _pPngInfo))
					{
					case 8:	return DMibGraphicsImageFormat(B8G8R8_UNorm);
					case 16:return DMibGraphicsImageFormat(B16G16R16_UNorm);
					}
				}

			case PNG_COLOR_TYPE_RGBA:
				{
					switch (png_get_bit_depth(_pPng, _pPngInfo))
					{
					case 8:	return DMibGraphicsImageFormat(B8G8R8A8_UNorm);
					case 16:return DMibGraphicsImageFormat(B16G16R16A16_UNorm);
					}
				}

			case PNG_COLOR_TYPE_GRAY_ALPHA:
				{
					switch (png_get_bit_depth(_pPng, _pPngInfo))
					{
					case 8:	return DMibGraphicsImageFormat(R8A8_UNorm);
					case 16:return DMibGraphicsImageFormat(R16A16_UNorm);
					}
				}
			}
			return 0;
		}

		bool fs_CImageFormatToPngFormat( NGraphics::CImage* _pImg, uint32& _nBitDepth, uint32& _nColorType, bool &_bBgr)
		{
			switch(_pImg->m_ImageFormat)
			{
			case DMibGraphicsImageFormat(R1_UNorm):
				{
					_nBitDepth = 1;
					_nColorType	= PNG_COLOR_TYPE_GRAY;
					return true;
				}
			case DMibGraphicsImageFormat(R2_UNorm):
				{
					_nBitDepth = 2;
					_nColorType	= PNG_COLOR_TYPE_GRAY;
					return true;
				}
			case DMibGraphicsImageFormat(R4_UNorm):
				{
					_nBitDepth = 4;
					_nColorType	= PNG_COLOR_TYPE_GRAY;
					return true;
				}
			case DMibGraphicsImageFormat(R8_UNorm):
				{
					_nBitDepth = 8;
					_nColorType	= PNG_COLOR_TYPE_GRAY;
					return true;
				}
			case DMibGraphicsImageFormat(R16_UNorm):
				{
					_nBitDepth = 16;
					_nColorType	= PNG_COLOR_TYPE_GRAY;
					return true;
				}

			case DMibGraphicsImageFormat(R8G8B8_UNorm):
				{
					_nBitDepth = 8;
					_nColorType	= PNG_COLOR_TYPE_RGB;
					return true;
				}
			case DMibGraphicsImageFormat(R16G16B16_UNorm):
				{
					_nBitDepth = 16;
					_nColorType	= PNG_COLOR_TYPE_RGB;
					return true;
				}
			case DMibGraphicsImageFormat(R16G16B16A16_UNorm):
				{
					_nBitDepth = 16;
					_nColorType	= PNG_COLOR_TYPE_RGBA;
					return true;
				}
			case DMibGraphicsImageFormat(R8G8B8A8_UNorm):
				{
					_nBitDepth = 8;
					_nColorType	= PNG_COLOR_TYPE_RGBA;
					return true;
				}

			case DMibGraphicsImageFormat(B8G8R8_UNorm):
				{
					_nBitDepth = 8;
					_nColorType	= PNG_COLOR_TYPE_RGB;
					_bBgr = true;
					return true;
				}
			case DMibGraphicsImageFormat(B16G16R16_UNorm):
				{
					_nBitDepth = 16;
					_nColorType	= PNG_COLOR_TYPE_RGB;
					_bBgr = true;
					return true;
				}
			case DMibGraphicsImageFormat(B16G16R16A16_UNorm):
				{
					_nBitDepth = 16;
					_nColorType	= PNG_COLOR_TYPE_RGBA;
					_bBgr = true;
					return true;
				}
			case DMibGraphicsImageFormat(B8G8R8A8_UNorm):
				{
					_nBitDepth = 8;
					_nColorType	= PNG_COLOR_TYPE_RGBA;
					_bBgr = true;
					return true;
				}

			case DMibGraphicsImageFormat(R8A8_UNorm):
				{
					_nBitDepth = 8;
					_nColorType	= PNG_COLOR_TYPE_GRAY_ALPHA;
					return true;
				}
			case DMibGraphicsImageFormat(R16A16_UNorm):
				{
					_nBitDepth = 16;
					_nColorType	= PNG_COLOR_TYPE_GRAY_ALPHA;
					return true;
				}
			}
			return false;
		}


		bool f_ReadImage(CImageMemory &_Destination, NStream::CBinaryStream &_Stream) override
		{
			png_struct *pPng = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
			if (!pPng)
				return false;

			png_info *pPngInfo = png_create_info_struct(pPng);
			if (!pPngInfo)
			{
				png_destroy_read_struct(&pPng, nullptr, nullptr);
				return false;
			}

			if (setjmp(png_jmpbuf(pPng)))
			{
				png_destroy_read_struct(&pPng, &pPngInfo, nullptr);
				return false;
			}

			bool bRet = false;
			try
			{
				png_set_read_fn(pPng, &_Stream, fs_PngRead);
				png_set_sig_bytes(pPng, 0);
				png_set_bgr(pPng);
				png_read_png(pPng, pPngInfo, PNG_TRANSFORM_IDENTITY, nullptr);
				uint64 Format = fs_PngFromatToCImageFormat(pPng, pPngInfo);
				if (Format == 0)
				{
					png_destroy_read_struct(&pPng, &pPngInfo, nullptr);
					return false;
				}

				_Destination.f_Create(Format, CImageDimensions(png_get_image_width(pPng, pPngInfo), png_get_image_height(pPng, pPngInfo)));
				fs_ReadColorInformation(pPng, pPngInfo, _Destination.m_ColorInformation);
				CImage::CLockedData Data;
				if (_Destination.f_LockWrite(Data))
				{
					uint32 height = png_get_image_height(pPng, pPngInfo);
					uint8 *pImagePtr = (uint8 *)Data.m_pData;
					umint DstStride = Data.m_Stride.f_GetWidth();
					umint RowStride = fg_Min(DstStride, png_get_rowbytes(pPng, pPngInfo));
					auto pRowPointers = png_get_rows(pPng, pPngInfo);
					for( uint32 i = 0; i < height; i++ )
						memcpy( pImagePtr + DstStride * i, pRowPointers[i], RowStride);

					_Destination.f_UnLock(Data);
					bRet = true;
				}
			}
			catch (NFile::CExceptionFile)
			{
			}

			png_destroy_read_struct(&pPng, &pPngInfo, nullptr);

			return bRet;
		}

		static void fs_ReadColorInformation(png_struct *_pPng, png_info *_pPngInfo, NGraphics::CImageColorInformation &o_Information)
		{
			o_Information = NGraphics::CImageColorInformation();

#ifdef PNG_cICP_SUPPORTED
			{
				png_byte Primaries = 0;
				png_byte Transfer = 0;
				png_byte Matrix = 0;
				png_byte Range = 0;
				if (png_get_cICP(_pPng, _pPngInfo, &Primaries, &Transfer, &Matrix, &Range))
				{
					o_Information.m_bCodingIndependent = true;
					o_Information.m_CodingIndependent[0] = Primaries;
					o_Information.m_CodingIndependent[1] = Transfer;
					o_Information.m_CodingIndependent[2] = Matrix;
					o_Information.m_CodingIndependent[3] = Range;
				}
			}
#endif

			{
				png_charp pName = nullptr;
				int Compression = 0;
				png_bytep pProfile = nullptr;
				png_uint_32 nProfile = 0;
				if (png_get_iCCP(_pPng, _pPngInfo, &pName, &Compression, &pProfile, &nProfile) && pProfile && nProfile)
					o_Information.m_Profile.f_Insert(pProfile, nProfile);
			}

			{
				int Intent = 0;
				if (png_get_sRGB(_pPng, _pPngInfo, &Intent))
					o_Information.m_bSRGB = true;
			}

			{
				double Gamma = 0.0;
				if (png_get_gAMA(_pPng, _pPngInfo, &Gamma) && Gamma > 0.0)
					o_Information.m_Gamma = Gamma;
			}

			{
				double Chromaticities[8] = {};
				if
				(
					png_get_cHRM
					(
						_pPng
						, _pPngInfo
						, &Chromaticities[0]
						, &Chromaticities[1]
						, &Chromaticities[2]
						, &Chromaticities[3]
						, &Chromaticities[4]
						, &Chromaticities[5]
						, &Chromaticities[6]
						, &Chromaticities[7]
					)
				)
				{
					o_Information.m_bChromaticities = true;
					for (umint i = 0; i < 8; ++i)
						o_Information.m_Chromaticities[i] = Chromaticities[i];
				}
			}
		}

		bool f_WriteImage(NGraphics::CImage &_Source, NStream::CBinaryStream &_Stream) override
		{
			png_struct *pPng = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
			if (!pPng)
				return false;

			png_info *pPngInfo = png_create_info_struct(pPng);
			if (!pPngInfo)
			{
				png_destroy_write_struct(&pPng, nullptr);
				return false;
			}

			if (setjmp(png_jmpbuf(pPng)))
			{
				png_destroy_write_struct(&pPng, &pPngInfo);
				return false;
			}

			bool bRet = false;
			try
			{
				png_set_write_fn( pPng, &_Stream, fs_PngWrite, fs_PngFlush);

				uint32 nBitDepth = 8;
				uint32 nColorType = 0;
				bool bBgr = false;
				if (!fs_CImageFormatToPngFormat( &_Source, nBitDepth, nColorType, bBgr))
				{
					png_destroy_write_struct(&pPng, &pPngInfo);
					return false;
				}
				png_set_IHDR(pPng, pPngInfo, _Source.m_Dimensions.f_GetWidth(), _Source.m_Dimensions.f_GetHeight(), nBitDepth, nColorType, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE );
				png_set_compression_level(pPng, Z_BEST_SPEED);
				if (bBgr)
					png_set_bgr(pPng);

				// Should do palette thingy here

				png_write_info(pPng, pPngInfo);

				CImage::CLockedData Data;
				if (_Source.f_LockRead(Data))
				{
					png_bytep pImagePtr = (png_bytep)Data.m_pData;
					uint32 Stride = Data.m_Stride[0];
					uint32 Height = _Source.m_Dimensions.f_GetHeight();
					NContainer::TCVector<png_bytep> lRowPointers;
					lRowPointers.f_SetLen(Height);
					for( uint32 i = 0; i < Height; i++ )
						lRowPointers[i]	= pImagePtr + i * Stride;

					png_write_image(pPng, lRowPointers.f_GetArray());
					_Source.f_UnLock(Data);
					png_write_end(pPng, pPngInfo);
					bRet = true;
				}
			}
			catch (NFile::CExceptionFile)
			{
			}

			png_destroy_write_struct(&pPng, &pPngInfo);

			return bRet;
		}
	};
}

using NMib_NGraphics_CImageIO_Png = NMib::NGraphics::CImageIO_Png;
DMibRuntimeClassNamedCasted(NMib::NGraphics::CImageIO, NMib_NGraphics_CImageIO_Png, NMib::NGraphics::CImageIO_Png, NMib::NGraphics::CImageIO);

void fg_Malterlib_Graphics_MakeActive_Png()
{
	DMibRuntimeClassMakeActive(NMib_NGraphics_CImageIO_Png);
}
