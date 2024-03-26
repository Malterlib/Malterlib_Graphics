// Copyright © 2015 Hansoft AB
// Distributed under the MIT license, see license text in LICENSE.Malterlib

#include "wchar.h"

#include <setjmp.h>
#include "../../External/libjpeg-turbo/jpeglib.h"
#include <Mib/Graphics/Image>
#include <Mib/Core/RuntimeType>

#ifdef DCompiler_MSVC
	#pragma warning(disable:4611)
#endif

namespace NMib::NGraphics
{
	class CImageIO_Jpeg : public NMib::NGraphics::CImageIO
	{
	private:
		struct CIOContext
		{
			NStream::CBinaryStream* m_pStream;
			uint8* m_pBuffer;
		};

		enum
		{
			EJpeg_BufferSize = 1024,
		};

		// Jpeg Source Methods
		static void fs_JpegInitSource(j_decompress_ptr _pInfo)
		{
			CIOContext* pContext = (CIOContext*)_pInfo->client_data;
			pContext->m_pBuffer = DMibNew JOCTET[EJpeg_BufferSize];
		}

		static boolean fs_JpegFillInputBuffer(j_decompress_ptr _pInfo)
		{
			jpeg_source_mgr* pSource = _pInfo->src;
			CIOContext* pContext = (CIOContext*)_pInfo->client_data;

			NStream::CFilePos ReadAmount = fg_Min(pContext->m_pStream->f_GetLength() - pContext->m_pStream->f_GetPosition(), EJpeg_BufferSize);

			try
			{
				pContext->m_pStream->f_ConsumeBytes((void*)pContext->m_pBuffer, ReadAmount);
			}
			catch (NFile::CExceptionFile)
			{
				pSource->next_input_byte = nullptr;
				pSource->bytes_in_buffer = 0;
				return 0;
			}

			pSource->next_input_byte = (JOCTET*)pContext->m_pBuffer;
			pSource->bytes_in_buffer = (size_t)ReadAmount;
			return 1;
		}

		static void fs_JpegSkipInputData(j_decompress_ptr _pInfo, long _nBytes)
		{
			CIOContext* pContext = (CIOContext*)_pInfo->client_data;
			jpeg_source_mgr* pSource = _pInfo->src;

			if (size_t(_nBytes) > pSource->bytes_in_buffer)
			{
				try
				{
					pContext->m_pStream->f_SetPosition(pContext->m_pStream->f_GetPosition() + (_nBytes - pSource->bytes_in_buffer));
				}
				catch (NFile::CExceptionFile)
				{
					return;
				}
				pSource->bytes_in_buffer = 0;
			}
			else
			{
				pSource->next_input_byte += _nBytes;
				pSource->bytes_in_buffer -= _nBytes;
			}
		}

		static boolean fs_JpegResyncToRestart(j_decompress_ptr _pInfo, int _Desired)
		{
			return jpeg_resync_to_restart(_pInfo, _Desired);
		}

		static void fs_JpegTermSource(j_decompress_ptr _pInfo)
		{
			CIOContext* pContext = (CIOContext*)_pInfo->client_data;
			delete [] pContext->m_pBuffer;
			pContext->m_pBuffer = nullptr;
		}

		static void fs_JpegInitDestination(j_compress_ptr _pInfo)
		{
			CIOContext* pContext = (CIOContext*)_pInfo->client_data;
			jpeg_destination_mgr* pDest = _pInfo->dest;

			pContext->m_pBuffer = DMibNew uint8 [EJpeg_BufferSize];
			pDest->free_in_buffer = EJpeg_BufferSize;
			pDest->next_output_byte = pContext->m_pBuffer;
		}

		static boolean fs_JpegEmptyOutputBuffer(j_compress_ptr _pInfo)
		{
			CIOContext* pContext = (CIOContext*)_pInfo->client_data;
			jpeg_destination_mgr* pDest = _pInfo->dest;

			try
			{
				pContext->m_pStream->f_FeedBytes(pContext->m_pBuffer, EJpeg_BufferSize);
			}
			catch (NFile::CExceptionFile)
			{
				return 0;
			}

			pDest->free_in_buffer = EJpeg_BufferSize;
			pDest->next_output_byte = pContext->m_pBuffer;

			return 1;
		}

		static void fs_JpegTermDestination(j_compress_ptr _pInfo)
		{
			CIOContext* pContext = (CIOContext*)_pInfo->client_data;
			jpeg_destination_mgr* pDest = _pInfo->dest;

			try
			{
				pContext->m_pStream->f_FeedBytes(pContext->m_pBuffer, EJpeg_BufferSize - pDest->free_in_buffer);
			}
			catch (NFile::CExceptionFile)
			{
			}

			delete pContext->m_pBuffer;
		}


		// Jpeg Error Methods

		struct CJpegErrorManager
		{
		  struct jpeg_error_mgr m_Public;
		  jmp_buf m_SetJmpBuffer;
		};

		static void fs_JpegErrorExit(j_common_ptr _pInfo)
		{
		  CJpegErrorManager* pErrorManager = (CJpegErrorManager*) _pInfo->err;

		  longjmp(pErrorManager->m_SetJmpBuffer, 1);
		}

		static void fs_JpegEmitMessage(j_common_ptr _pInfo, int _MsgLevel)
		{

		}

		static void fs_JpegOutputMessage(j_common_ptr _pInfo)
		{
			char Buffer[JMSG_LENGTH_MAX];

			(*_pInfo->err->format_message) (_pInfo, Buffer);
			DMibTrace("{}" DMibNewLine, Buffer);
		}

		static void fs_JpegFormatMessage(j_common_ptr _pInfo, char * buffer)
		{

		}

		static void fs_JpegResetErrorManager(j_common_ptr _pInfo)
		{

		}

		// Jpeg Util.

		void fp_JpegFillSourceManager(jpeg_source_mgr* _pSrc)
		{
			_pSrc->next_input_byte = nullptr;
			_pSrc->bytes_in_buffer = 0;
			_pSrc->init_source = fs_JpegInitSource;
			_pSrc->fill_input_buffer = fs_JpegFillInputBuffer;
			_pSrc->skip_input_data = fs_JpegSkipInputData;
			_pSrc->resync_to_restart = fs_JpegResyncToRestart;
			_pSrc->term_source = fs_JpegTermSource;
		}

		void fp_JpegFillDestinationManager(jpeg_destination_mgr* _pDest)
		{
			_pDest->free_in_buffer = 0;
			_pDest->next_output_byte = nullptr;
			_pDest->init_destination = fs_JpegInitDestination;
			_pDest->empty_output_buffer = fs_JpegEmptyOutputBuffer;
			_pDest->term_destination = fs_JpegTermDestination;
		}

		void fp_JpegFillErrorManager(CJpegErrorManager *_pManager)
		{
			jpeg_std_error(&_pManager->m_Public);
			_pManager->m_Public.error_exit = fs_JpegErrorExit;
			_pManager->m_Public.output_message = fs_JpegOutputMessage;
		}


	public:
		bool f_DetectFormatFromFilename(const NStr::CStr &_FileName) override
		{
			if (_FileName.f_FindNoCase(".jpg") >= 0)
				return true;
			else if (_FileName.f_FindNoCase(".jpeg") >= 0)
				return true;
			else
				return false;
		}


		bool f_DetectFormatFromStream(NStream::CBinaryStream &_Stream) override
		{
			jpeg_source_mgr Source;
			fp_JpegFillSourceManager(&Source);

			jpeg_decompress_struct DecompressInfo;

			CJpegErrorManager ErrorManager;
			fp_JpegFillErrorManager(&ErrorManager);
			DecompressInfo.err = &ErrorManager.m_Public;

			CIOContext Context;
			Context.m_pStream = &_Stream;
			Context.m_pBuffer = nullptr;

			if (setjmp(ErrorManager.m_SetJmpBuffer))
			{
				jpeg_destroy_decompress(&DecompressInfo);
				delete []Context.m_pBuffer;
				return false;
			}

			jpeg_create_decompress(&DecompressInfo);

			DecompressInfo.src = &Source;
			DecompressInfo.client_data = &Context;

			jpeg_read_header(&DecompressInfo, TRUE);

			bool bRet = false;

			if (DecompressInfo.image_width > 0 &&
				DecompressInfo.image_height >0)
			{
				bRet = true;
			}

			jpeg_destroy_decompress(&DecompressInfo);

			// Strangely this isn't cleaned up by the lib.
			delete [] Context.m_pBuffer;
			Context.m_pBuffer = nullptr;

			return bRet;
		}

		static uint64 fs_JpegFromatToCImageFormat(jpeg_decompress_struct *_pInfo)
		{
			switch( _pInfo->jpeg_color_space)
			{
			case JCS_GRAYSCALE:
				{
					return DMibGraphicsImageFormat(R1_UNorm);
				}

			case JCS_RGB:
			case JCS_YCbCr:
			case JCS_CMYK:
			case JCS_YCCK:
				{
					return DMibGraphicsImageFormat(B8G8R8_UNorm);
				}
			default:
				break;
			}
			return 0;
		}

		bool f_ReadImage(CImageMemory &_DestinationImage, NStream::CBinaryStream &_Stream) override
		{
			jpeg_source_mgr Source;
			fp_JpegFillSourceManager(&Source);

			jpeg_decompress_struct DecompressInfo;

			CJpegErrorManager ErrorManager;
			fp_JpegFillErrorManager(&ErrorManager);
			DecompressInfo.err = &ErrorManager.m_Public;

			CIOContext Context;
			Context.m_pStream = &_Stream;
			Context.m_pBuffer = nullptr;

			if (setjmp(ErrorManager.m_SetJmpBuffer))
			{
				jpeg_destroy_decompress(&DecompressInfo);
				delete []Context.m_pBuffer;
				return false;
			}

			jpeg_create_decompress(&DecompressInfo);

			DecompressInfo.src = &Source;
			DecompressInfo.client_data = &Context;

			jpeg_read_header(&DecompressInfo, TRUE);

			/* We don't need this as we ask jpeglib to convert to RGB for us.
			{
				uint64 InternalFormat = fs_JpegFromatToCImageFormat(&DecompressInfo);

				if (InternalFormat == 0)
				{
					jpeg_destroy_decompress(&DecompressInfo);
					delete []Context.m_pBuffer;
					return false;
				}
			}
			*/
			uint64 InternalFormat = DMibGraphicsImageFormat(B8G8R8_UNorm);

			DecompressInfo.out_color_space = JCS_RGB;

			jpeg_start_decompress(&DecompressInfo);

			_DestinationImage.f_Create(InternalFormat, CImageDimensions(DecompressInfo.output_width, DecompressInfo.output_height));


			CImage::CLockedData Data;
			if (_DestinationImage.f_LockWrite(Data))
			{
				uint8 *pImagePtr = (uint8 *)Data.m_pData;
				uint32 DstStride = Data.m_Stride.f_GetWidth();
				JSAMPROW pRow;

				while (DecompressInfo.output_scanline < DecompressInfo.output_height)
				{
					pRow = (JSAMPROW)pImagePtr;
					(void) jpeg_read_scanlines(&DecompressInfo, &pRow, 1);
					pImagePtr += DstStride;
				}

				// Convert RGB -> BGR (Slow as hell :-( )
				pImagePtr = (uint8 *)Data.m_pData;
				uint8* pEndImagePtr = pImagePtr + DstStride * _DestinationImage.m_Dimensions.f_GetHeight();
				uint32 Width = _DestinationImage.m_Dimensions.f_GetWidth();

				while (pImagePtr != pEndImagePtr)
				{
					uint8* pRowPtr = pImagePtr;

					for (uint32 iX = 0; iX < Width; ++iX)
					{
						uint8 R = pRowPtr[0];
						uint8 G = pRowPtr[1];
						uint8 B = pRowPtr[2];

						pRowPtr[0] = B;
						pRowPtr[1] = G;
						pRowPtr[2] = R;

						pRowPtr += 3;
					}

					pImagePtr += DstStride;
				}

				_DestinationImage.f_UnLock(Data);
			}

			jpeg_finish_decompress(&DecompressInfo);
			jpeg_destroy_decompress(&DecompressInfo);

			return true;
		}

		bool f_WriteImage(NGraphics::CImage &_SourceImage, NStream::CBinaryStream &_Stream) override
		{
			NGraphics::CImageMemory FormattedImage;
			NGraphics::CImage *pSourceImage = &FormattedImage;

			FormattedImage.f_Create(DMibGraphicsImageFormat(R8G8B8_UNorm), _SourceImage.m_Dimensions, NGraphics::CImageStride());
			_SourceImage.f_ConvertInto(FormattedImage);

			struct jpeg_destination_mgr Destination;
			fp_JpegFillDestinationManager(&Destination);

			struct jpeg_compress_struct CompressInfo;

			CJpegErrorManager ErrorManager;
			fp_JpegFillErrorManager(&ErrorManager);
			CompressInfo.err = &ErrorManager.m_Public;

			jpeg_create_compress(&CompressInfo);

			CIOContext Context;
			Context.m_pStream = &_Stream;
			Context.m_pBuffer = nullptr;

			if (setjmp(ErrorManager.m_SetJmpBuffer))
			{
				jpeg_destroy_compress(&CompressInfo);
				delete []Context.m_pBuffer;
				return false;
			}

			CompressInfo.dest = &Destination;
			CompressInfo.client_data = &Context;
			CompressInfo.image_width = pSourceImage->m_Dimensions.f_GetWidth();
			CompressInfo.image_height = pSourceImage->m_Dimensions.f_GetHeight();
			CompressInfo.input_components = 3;
			CompressInfo.in_color_space	= JCS_RGB;

			jpeg_set_defaults(&CompressInfo);

			jpeg_set_colorspace (&CompressInfo, JCS_RGB);
			jpeg_set_quality(&CompressInfo, 85, TRUE); // Quality ranges from 0 to 100.

			jpeg_start_compress(&CompressInfo, TRUE);

			CImage::CLockedData Data;
			if (pSourceImage->f_LockRead(Data))
			{
				JSAMPROW pRow;
				uint8 const* pImagePtr = (uint8 const*)Data.m_pData;
				uint32 SrcStride = Data.m_Stride.f_GetWidth();

				while (CompressInfo.next_scanline < CompressInfo.image_height)
				{
					pRow = (JSAMPROW)pImagePtr;
					jpeg_write_scanlines(&CompressInfo, &pRow, 1);
					pImagePtr += SrcStride;
				}

				pSourceImage->f_UnLock(Data);
			}

			jpeg_finish_compress(&CompressInfo);

			jpeg_destroy_compress(&CompressInfo);

			return true;
		}
	};
}

using NMib_NGraphics_CImageIO_Jpeg = NMib::NGraphics::CImageIO_Jpeg;
DMibRuntimeClassNamedCasted(NMib::NGraphics::CImageIO, NMib_NGraphics_CImageIO_Jpeg, NMib::NGraphics::CImageIO_Jpeg, NMib::NGraphics::CImageIO);

void fg_Malterlib_Graphics_MakeActive_Jpeg()
{
	DMibRuntimeClassMakeActive(NMib_NGraphics_CImageIO_Jpeg);
}
