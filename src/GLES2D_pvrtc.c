#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <GLES/gl.h>
#include <GLES/egl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GLES2D/GLES2D_platform.h>
#include <GLES2D/GLES2D_texture.h>
#include <GLES2D/GLES2D_drawing.h>
#include "GLES2D_pvrtc.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <GLES/gl.h>
#include <GLES/egl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GLES2D/GLES2D_platform.h>
#include <GLES2D/GLES2D_texture.h>
#include <GLES2D/GLES2D_drawing.h>
#include "GLES2D_pvrtc.h"

unsigned int GLES2D_CreateTexturePVRFromPointer( int *w, int *h, const void * const pointer, GLuint * const texName )
{
	const void * const texPtr = NULL;
	const unsigned int nLoadFromLevel = 0;
	const void *psTextureHeader = NULL;

	PVR_Texture_Header* psPVRHeader = (PVR_Texture_Header*)pointer;

	*w = (int) psPVRHeader->dwWidth;
	*h = (int) psPVRHeader->dwHeight;

	unsigned int u32NumSurfs;

	// perform checks for old PVR psPVRHeader
	if(psPVRHeader->dwHeaderSize!=sizeof(PVR_Texture_Header))
	{	// Header V1
		if(psPVRHeader->dwHeaderSize==PVRTEX_V1_HEADER_SIZE)
		{	// react to old psPVRHeader: i.e. fill in numsurfs as this is missing from old header
			printf("PVRTTexture:PVRTLoadPartialTextureFromPointer warning: this is an old pvr"
				" - you can use PVRTexTool to update its header.\n");
			if(psPVRHeader->dwpfFlags&PVRTEX_CUBEMAP)
				u32NumSurfs = 6;
			else
				u32NumSurfs = 1;
		}
		else
		{	// not a pvr at all
			printf("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: not a valid pvr.\n");
			return 0;
		}
	}
	else
	{	// Header V2
		if(psPVRHeader->dwNumSurfs<1)
		{	// encoded with old version of PVRTexTool before zero numsurfs bug found.
			if(psPVRHeader->dwpfFlags & PVRTEX_CUBEMAP)
				u32NumSurfs = 6;
			else
				u32NumSurfs = 1;
		}
		else
		{
			u32NumSurfs = psPVRHeader->dwNumSurfs;
		}
	}

	GLuint textureName;
	GLenum textureFormat = 0;
	GLenum textureType = GL_RGB;

	int IsPVRTCSupported = 1;
	int IsBGRA8888Supported  = 0;

	*texName = 0;	// install warning value

	int IsCompressedFormatSupported = 0, IsCompressedFormat = 0;

	/* Only accept untwiddled data UNLESS texture format is PVRTC */
	if ( ((psPVRHeader->dwpfFlags & PVRTEX_TWIDDLE) == PVRTEX_TWIDDLE)
		&& ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)!=OGL_PVRTC2)
		&& ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)!=OGL_PVRTC4) )
	{
		// We need to load untwiddled textures -- hw will twiddle for us.
		printf("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: texture should be untwiddled.\n");
		return 0;
	}

	switch(psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)
	{
	case OGL_RGBA_4444:
		textureFormat = GL_UNSIGNED_SHORT_4_4_4_4;
		textureType = GL_RGBA;
		break;

	case OGL_RGBA_5551:
		textureFormat = GL_UNSIGNED_SHORT_5_5_5_1;
		textureType = GL_RGBA;
		break;

	case OGL_RGBA_8888:
		textureFormat = GL_UNSIGNED_BYTE;
		textureType = GL_RGBA;
		break;

	/* New OGL Specific Formats Added */

	case OGL_RGB_565:
		textureFormat = GL_UNSIGNED_SHORT_5_6_5;
		textureType = GL_RGB;
		break;

	case OGL_RGB_555:
		printf("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: pixel type OGL_RGB_555 not supported.\n");
		return 0; // Deal with exceptional case

	case OGL_RGB_888:
		textureFormat = GL_UNSIGNED_BYTE;
		textureType = GL_RGB;
		break;

	case OGL_I_8:
		textureFormat = GL_UNSIGNED_BYTE;
		textureType = GL_LUMINANCE;
		break;

	case OGL_AI_88:
		textureFormat = GL_UNSIGNED_BYTE;
		textureType = GL_LUMINANCE_ALPHA;
		break;

	case OGL_PVRTC2:
		if(IsPVRTCSupported)
		{
			IsCompressedFormatSupported = IsCompressedFormat = 1;
			textureFormat = psPVRHeader->dwAlphaBitMask==0 ? GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG : GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG ;	// PVRTC2
		}
		else
		{
			IsCompressedFormatSupported = 0;
			IsCompressedFormat = 1;
			textureFormat = GL_UNSIGNED_BYTE;
			textureType = GL_RGBA;
			printf("PVRTTexture:PVRTLoadPartialTextureFromPointer warning: PVRTC2 not supported. Converting to RGBA8888 instead.\n");
		}
		break;

	case OGL_PVRTC4:
		if(IsPVRTCSupported)
		{
			IsCompressedFormatSupported = IsCompressedFormat = 1;
			textureFormat = psPVRHeader->dwAlphaBitMask==0 ? GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG ;	// PVRTC4
		}
		else
		{
			IsCompressedFormatSupported = 0;
			IsCompressedFormat = 1;
			textureFormat = GL_UNSIGNED_BYTE;
			textureType = GL_RGBA;
			printf("PVRTTexture:PVRTLoadPartialTextureFromPointer warning: PVRTC4 not supported. Converting to RGBA8888 instead.\n");
		}
		break;

	case OGL_BGRA_8888:
		if(IsBGRA8888Supported)
		{
			textureFormat = GL_UNSIGNED_BYTE;
			textureType   = GL_BGRA;
			break;
		}
		else
		{
			printf("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: Unable to load GL_BGRA texture as extension GL_IMG_texture_format_BGRA8888 is unsupported.\n");
			return 0;
		}
	default:											// NOT SUPPORTED
		printf("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: pixel type not supported.\n");
		return 0;
	}

	// load the texture up
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);				// Never have row-aligned in psPVRHeaders

	glGenTextures(1, &textureName);

	//  check that this data is cube map data or not.
	if(psPVRHeader->dwpfFlags & PVRTEX_CUBEMAP)
	{ // not in OGLES you don't
		printf("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: cube map textures are not available in OGLES1.x.\n");
		return 0;
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, textureName);
	}

	if(glGetError())
	{
		printf("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: glBindTexture() failed.\n");
		return 0;
	}

	int i;
	for(i=0; i<u32NumSurfs; i++)
	{
		char *theTexturePtr = (texPtr? (char*)texPtr :  (char*)psPVRHeader + psPVRHeader->dwHeaderSize) + psPVRHeader->dwTextureDataSize * i;
		char *theTextureToLoad = 0;
		int		nMIPMapLevel;
		int		nTextureLevelsNeeded = (psPVRHeader->dwpfFlags & PVRTEX_MIPMAP)? psPVRHeader->dwMipMapCount : 0;
		unsigned int		nSizeX= psPVRHeader->dwWidth, nSizeY = psPVRHeader->dwHeight;
		unsigned int		CompressedImageSize = 0;

		for(nMIPMapLevel = 0; nMIPMapLevel <= nTextureLevelsNeeded; nSizeX = PVRT_MAX(nSizeX/2, (unsigned int)1), nSizeY = PVRT_MAX(nSizeY/2, (unsigned int)1), nMIPMapLevel++)
		{
			// Do Alpha-swap if needed

			theTextureToLoad = theTexturePtr;

			// Load the Texture

			/* If the texture is PVRTC then use GLCompressedTexImage2D */
			if(IsCompressedFormat)
			{
				/* Calculate how many bytes this MIP level occupies */
				if ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)==OGL_PVRTC2)
				{
					CompressedImageSize = ( PVRT_MAX(nSizeX, PVRTC2_MIN_TEXWIDTH) * PVRT_MAX(nSizeY, PVRTC2_MIN_TEXHEIGHT) * psPVRHeader->dwBitCount + 7) / 8;
				}
				else
				{// PVRTC4 case
					CompressedImageSize = ( PVRT_MAX(nSizeX, PVRTC4_MIN_TEXWIDTH) * PVRT_MAX(nSizeY, PVRTC4_MIN_TEXHEIGHT) * psPVRHeader->dwBitCount + 7) / 8;
				}

				if(((signed int)nMIPMapLevel - (signed int)nLoadFromLevel) >= 0)
				{
					if(IsCompressedFormatSupported)
					{
						//if(psPVRHeader->dwpfFlags&PVRTEX_CUBEMAP)
						//{
						//	/* Load compressed texture data at selected MIP level */
						//	glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, nMIPMapLevel-nLoadFromLevel, textureFormat, nSizeX, nSizeY, 0,
						//					CompressedImageSize, theTextureToLoad);
						//}
						//else
						{
							/* Load compressed texture data at selected MIP level */
							glCompressedTexImage2D(GL_TEXTURE_2D, nMIPMapLevel-nLoadFromLevel, textureFormat, nSizeX, nSizeY, 0,
											CompressedImageSize, theTextureToLoad);

						}
					}
					else
					{
						// Convert PVRTC to 32-bit
						unsigned char *u8TempTexture = (unsigned char*)malloc(nSizeX*nSizeY*4);
						if ((psPVRHeader->dwpfFlags & PVRTEX_PIXELTYPE)==OGL_PVRTC2)
						{
							printf("PVRTCDecompress Unsupported\n");
							//PVRTCDecompress(theTextureToLoad, 1, nSizeX, nSizeY, u8TempTexture);
						}
						else
						{// PVRTC4 case
							printf("PVRTCDecompress Unsupported\n");
							//PVRTCDecompress(theTextureToLoad, 0, nSizeX, nSizeY, u8TempTexture);
						}


						//if(psPVRHeader->dwpfFlags&PVRTEX_CUBEMAP)
						//{// Load compressed cubemap data at selected MIP level
						//	// Upload the texture as 32-bits
						//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,nMIPMapLevel-nLoadFromLevel,GL_RGBA,
						//		nSizeX,nSizeY,0, GL_RGBA,GL_UNSIGNED_BYTE,u8TempTexture);
						//	FREE(u8TempTexture);
						//}
						//else
						{// Load compressed 2D data at selected MIP level
							// Upload the texture as 32-bits
							glTexImage2D(GL_TEXTURE_2D,nMIPMapLevel-nLoadFromLevel,GL_RGBA,
								nSizeX,nSizeY,0, GL_RGBA,GL_UNSIGNED_BYTE,u8TempTexture);
							FREE(u8TempTexture);
						}
					}
				}
			}
			else
			{
				if(((signed int)nMIPMapLevel - (signed int)nLoadFromLevel) >= 0)
				{
					//if(psPVRHeader->dwpfFlags&PVRTEX_CUBEMAP)
					//{
					//	/* Load uncompressed texture data at selected MIP level */
					//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,nMIPMapLevel-nLoadFromLevel,textureType,nSizeX,nSizeY,
					//		0, textureType,textureFormat,theTextureToLoad);
					//}
					//else
					{
						/* Load uncompressed texture data at selected MIP level */
						glTexImage2D(GL_TEXTURE_2D,nMIPMapLevel-nLoadFromLevel,textureType,nSizeX,nSizeY,0, textureType,textureFormat,theTextureToLoad);
					}
				}
			}



			if(glGetError())
			{
				printf("PVRTTexture:PVRTLoadPartialTextureFromPointer failed: glBindTexture() failed.\n");
				return 0;
			}

			// offset the texture pointer by one mip-map level

			/* PVRTC case */
			if ( IsCompressedFormat )
			{
				theTexturePtr += CompressedImageSize;
			}
			else
			{
				/* New formula that takes into account bit counts inferior to 8 (e.g. 1 bpp) */
				theTexturePtr += (nSizeX * nSizeY * psPVRHeader->dwBitCount + 7) / 8;
			}
		}
	}

	*texName = textureName;

	if(psTextureHeader)
	{
		*(PVR_Texture_Header*)psTextureHeader = *psPVRHeader;
		((PVR_Texture_Header*)psTextureHeader)->dwPVR = PVRTEX_IDENTIFIER;
		((PVR_Texture_Header*)psTextureHeader)->dwNumSurfs = u32NumSurfs;
	}

	return psPVRHeader->dwpfFlags|0x80000000;		// PVR psPVRHeader flags with topmost bit set so that it is non-zero
}

