/*==============================================================================
Copyright(c) 2017 Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files(the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and / or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
============================================================================*/

#include "GmmResourceULT.h"

/////////////////////////////////////////////////////////////////////////////////////
/// CTestResource Constructor
/////////////////////////////////////////////////////////////////////////////////////
CTestResource::CTestResource()
{

}


/////////////////////////////////////////////////////////////////////////////////////
/// CTestResource Destructor
///
/////////////////////////////////////////////////////////////////////////////////////
CTestResource::~CTestResource()
{

}

/////////////////////////////////////////////////////////////////////////////////////
/// Sets up common environment for Resource fixture tests. this is called once per
/// test case before executing all tests under resource fixture test case.
///  It also calls SetupTestCase from CommonULT to initialize global context and others.
///
/// @see    CTestResource::SetUpTestCase()
///
/////////////////////////////////////////////////////////////////////////////////////
void CTestResource::SetUpTestCase()
{
    printf("%s\n", __FUNCTION__);

    GfxPlatform.eProductFamily = IGFX_BROADWELL;
    GfxPlatform.eRenderCoreFamily = IGFX_GEN8_CORE;

    CommonULT::SetUpTestCase();
}

/////////////////////////////////////////////////////////////////////////////////////
/// Cleans up once all the tests finish execution.  It also calls TearDownTestCase
/// from CommonULT to destroy global context and others.
///
/// @see    CTestResource::TearDownTestCase()
/////////////////////////////////////////////////////////////////////////////////////
void CTestResource::TearDownTestCase()
{
    printf("%s\n", __FUNCTION__);

    CommonULT::TearDownTestCase();
}

/// @brief ULT for 1D Linear Resource
TEST_F(CTestResource, Test1DLinearResource)
{
    // Horizontal/Vertical pixel alignment
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;
    const UINT  MinPitch = 32;
    const UINT  PitchAlignment = 64;

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_1D;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.Linear = 1;
    gmmParams.Flags.Gpu.Texture = 1;

    // Allocate 1x1x1 surface
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x1;
        gmmParams.BaseHeight = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT AlignedHeight = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        UINT PitchInBytes = AlignedWidth * GetBppValue(bpp);
        PitchInBytes = GFX_MAX(PitchInBytes, MinPitch);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        UINT AlignedSize = GMM_ULT_ALIGN(PitchInBytes * AlignedHeight, PAGE_SIZE);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourceSize<true>(ResourceInfo, AlignedSize); 
        VerifyResourceQPitch<false>(ResourceInfo, 0); // Not valid for 1D
    }


    // Allocate more than 1 page
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x1001;
        gmmParams.BaseHeight = 1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT AlignedHeight = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        UINT PitchInBytes = AlignedWidth * GetBppValue(bpp);
        PitchInBytes = GFX_MAX(PitchInBytes, MinPitch);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        UINT AlignedSize = GMM_ULT_ALIGN(PitchInBytes * AlignedHeight, PAGE_SIZE);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourceSize<true>(ResourceInfo, AlignedSize);
        VerifyResourceQPitch<false>(ResourceInfo, 0); 
    }
}

/// @brief ULT for 1D Linear Resource Arrays
TEST_F(CTestResource, Test1DLinearResourceArrays)
{
    // Horizontal/Vertical pixel alignment
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;
    const UINT  MinPitch = 32;
    const UINT  PitchAlignment = 64;

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_1D;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.Linear = 1;
    gmmParams.Flags.Gpu.Texture = 1;
    gmmParams.ArraySize = 4;

    // Allocate 1x1x1 surface
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x100;
        gmmParams.BaseHeight = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT AlignedHeight = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        UINT PitchInBytes = AlignedWidth * GetBppValue(bpp);
        PitchInBytes = GFX_MAX(PitchInBytes, MinPitch);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        UINT AlignedSize = GMM_ULT_ALIGN(PitchInBytes * AlignedHeight * gmmParams.ArraySize, PAGE_SIZE);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourceSize<true>(ResourceInfo, AlignedSize);
        VerifyResourceQPitch<true>(ResourceInfo, AlignedHeight); 
    }
}

/// @brief ULT for 1D Mipped Linear Resource
TEST_F(CTestResource, Test1DLinearResourceMips)
{
    // Horizontal/Vertical pixel alignment
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;
    const UINT  MinPitch = 32;
    const UINT  PitchAlignment = 64;

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_1D;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.Linear = 1;
    gmmParams.Flags.Gpu.Texture = 1;
    gmmParams.MaxLod = 5;

    // Allocate 256x1 surface
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x100;
        gmmParams.BaseHeight = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT AlignedHeight = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        // Height of 1D surface will just be VAlign. So height of the whole block will be
        // Height of Mip0 + Mip2 + Mip3 + Mip4 + Mip5...
        AlignedHeight = gmmParams.MaxLod * AlignedHeight;
        UINT PitchInBytes = AlignedWidth * GetBppValue(bpp);
        PitchInBytes = GFX_MAX(PitchInBytes, MinPitch);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        UINT AlignedSize = GMM_ULT_ALIGN(PitchInBytes * AlignedHeight, PAGE_SIZE);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourceSize<true>(ResourceInfo, AlignedSize);
        VerifyResourceQPitch<false>(ResourceInfo, 0); // N/A for non-arrayed surface

        // Mip0 should be at offset 0. X/Y/Z Offset should be 0 for linear.
        GMM_REQ_OFFSET_INFO OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 0; //Mip 0
        ResourceInfo.GetOffset(OffsetInfo);
        EXPECT_EQ(0, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip1 should be after Mip0.  X/Y/Z Offset should be 0 for linear.
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 1; //Mip 1
        ResourceInfo.GetOffset(OffsetInfo);
        ULONG SizeOfMip0 = PitchInBytes * GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        EXPECT_EQ(SizeOfMip0, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip2 should to the right of Mip1.  X/Y/Z Offset should be 0 for linear.
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 2; //Mip 2
        ResourceInfo.GetOffset(OffsetInfo);
        ULONG StartOfMip = SizeOfMip0 + (GMM_ULT_ALIGN(gmmParams.BaseWidth64 >> 1, HAlign) * GetBppValue(bpp));
        EXPECT_EQ(StartOfMip, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Rest of the mips should be below mip2, each with height of HAlign. X/Y/Z Offset should be 0 for linear.
        for (int mip = 3; mip <= gmmParams.MaxLod; mip++)
        {
            OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.MipLevel = mip;
            ResourceInfo.GetOffset(OffsetInfo);
            StartOfMip += PitchInBytes * VAlign;
            EXPECT_EQ(StartOfMip, OffsetInfo.Render.Offset64);
            EXPECT_EQ(0, OffsetInfo.Render.XOffset);
            EXPECT_EQ(0, OffsetInfo.Render.YOffset);
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);
        }
    }
}


// ********************************************************************************//

/// @brief ULT for 2D Linear Resource Arrays
TEST_F(CTestResource, Test2DLinearResourceArrays)
{
    // Horizontal/Vertical pixel alignment
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;
    const UINT  MinPitch = 32;
    const UINT  PitchAlignment = 64;

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_2D;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.Linear = 1;
    gmmParams.Flags.Gpu.Texture = 1;
    gmmParams.ArraySize = 4;

    // Allocate 256x256 surface
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x100;
        gmmParams.BaseHeight = 0x100;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT AlignedHeight = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        UINT PitchInBytes = AlignedWidth * GetBppValue(bpp);
        PitchInBytes = GFX_MAX(PitchInBytes, MinPitch);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        UINT AlignedSize = GMM_ULT_ALIGN(PitchInBytes * AlignedHeight * gmmParams.ArraySize, PAGE_SIZE);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourceSize<true>(ResourceInfo, AlignedSize);
        VerifyResourceQPitch<true>(ResourceInfo, AlignedHeight);
    }
}

/// @brief ULT for 2D Mipped Linear Resource
TEST_F(CTestResource, Test2DLinearResourceMips)
{
    // Horizontal/Vertical pixel alignment
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;
    const UINT  MinPitch = 32;
    const UINT  PitchAlignment = 64;

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_2D;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.Linear = 1;
    gmmParams.Flags.Gpu.Texture = 1;
    gmmParams.MaxLod = 8;

    // Allocate 256x256 surface
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x100;
        gmmParams.BaseHeight = 0x100;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT AlignedHeight;
        UINT PitchInBytes = AlignedWidth * GetBppValue(bpp);
        PitchInBytes = GFX_MAX(PitchInBytes, MinPitch);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        UINT HeightOfPrevMip = AlignedHeight = gmmParams.BaseHeight;;

        // Mip0 should be at offset 0. X/Y/Z Offset should be 0 for linear.
        GMM_REQ_OFFSET_INFO OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 0; //Mip 0
        ResourceInfo.GetOffset(OffsetInfo);
        EXPECT_EQ(0, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip1 should be after Mip0.  X/Y/Z Offset should be 0 for linear.
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 1; //Mip 1
        ResourceInfo.GetOffset(OffsetInfo);
        ULONG SizeOfMip0 = PitchInBytes * GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        EXPECT_EQ(SizeOfMip0, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip2 should to the right of Mip1.  X/Y/Z Offset should be 0 for linear.
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 2; //Mip 2
        ResourceInfo.GetOffset(OffsetInfo);
        ULONG StartOfMip = SizeOfMip0 + (GMM_ULT_ALIGN(gmmParams.BaseWidth64 >> 1, HAlign) * GetBppValue(bpp));
        EXPECT_EQ(StartOfMip, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Rest of the mips should be below mip2, each with height of HAlign. X/Y/Z Offset should be 0 for linear.
        for (int mip = 3; mip <= 8; mip++)
        {
            OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.MipLevel = mip;
            ResourceInfo.GetOffset(OffsetInfo);
            HeightOfPrevMip = GMM_ULT_ALIGN((gmmParams.BaseHeight >> (mip - 1)), VAlign);
            AlignedHeight += HeightOfPrevMip;
            StartOfMip += PitchInBytes * HeightOfPrevMip;
            EXPECT_EQ(StartOfMip, OffsetInfo.Render.Offset64);
            EXPECT_EQ(0, OffsetInfo.Render.XOffset);
            EXPECT_EQ(0, OffsetInfo.Render.YOffset);
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);
        }

        //Mip8 will go till Mip0Height + Mip1Height + VALIGN
        AlignedHeight += VAlign;
        UINT AlignedSize = GMM_ULT_ALIGN(PitchInBytes * AlignedHeight, PAGE_SIZE);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourceSize<true>(ResourceInfo, AlignedSize);
        VerifyResourceQPitch<false>(ResourceInfo, 0); // N/A for non-arrayed surface
    }
}

/// @brief ULT for Arrayed 2D TileX Resource
TEST_F(CTestResource, Test2DTileXResourceArrays)
{
    // Horizontal/Vertical pixel alignment
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;

    const ULONG TileWidth = 512;
    const ULONG TileHeight = 8;

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_2D;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.TiledX = 1;
    gmmParams.Flags.Gpu.Texture = 1;
    gmmParams.ArraySize = 4;

    // Allocate 2 tiles in X/Y dimension
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = (TileWidth / GetBppValue(bpp)) + 1; // 1 pixel larger than 1 tile width
        gmmParams.BaseHeight = TileHeight + 1; // 1 row larger than 1 tile height

        const UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT PitchInBytes = AlignedWidth * GetBppValue(bpp);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, TileWidth);

        ULONG AlignedHeight, BlockHeight;
        BlockHeight = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        AlignedHeight = GMM_ULT_ALIGN_NP2(BlockHeight*gmmParams.ArraySize, TileHeight);
        
        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes); // As wide as 2 tile
        VerifyResourcePitchInTiles<true>(ResourceInfo, PitchInBytes/TileWidth); // 2 tile wide
        VerifyResourceSize<true>(ResourceInfo, PitchInBytes * AlignedHeight); // 4 tile big x 4 array size

        VerifyResourceQPitch<true>(ResourceInfo, BlockHeight);
    }
}

/// @brief ULT for Mipped 2D TileX Resource
TEST_F(CTestResource, Test2DTileXResourceMips)
{
    const ULONG TileSize[2] = { 512, 8 };
    enum  Coords { X = 0, Y = 1 };

    // Test normal mip case
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        const ULONG ResourceWidth = 0x400;
        const ULONG ResourceHeight = 0x400;
        const ULONG MaxLod = 10;


        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        GMM_RESCREATE_PARAMS gmmParams = {};
        gmmParams.Type = RESOURCE_2D;
        gmmParams.Flags.Info.TiledX = 1;
        gmmParams.NoGfxMemory = 1;
        gmmParams.Flags.Gpu.Texture = 1;
        gmmParams.BaseWidth64 = ResourceWidth;
        gmmParams.BaseHeight = ResourceHeight;
        gmmParams.MaxLod = MaxLod;
        gmmParams.Format = SetResourceFormat(bpp);

        // Create resource
        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        // Mip0 is the widest mip. So it will dictate the pitch of the whole surface
        const ULONG Pitch = ResourceWidth * GetBppValue(bpp);
        ULONG AllocationHeight = 0;

        // Mip0 should be at offset 0 and tile aligned
        GMM_REQ_OFFSET_INFO OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 0; //Mip 0
        ResourceInfo.GetOffset(OffsetInfo);
        EXPECT_EQ(0, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip1 should be after whole Mip0. Should still be tile aligned.
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 1; // Mip 1
        ResourceInfo.GetOffset(OffsetInfo);
        ULONG SizeOfMip0 = Pitch * ResourceHeight;
        ULONG HeightOfPrevMip = AllocationHeight = ResourceHeight;
        EXPECT_EQ(SizeOfMip0, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip2 should be on the right of Mip1. Should still be tile aligned.
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 2; // Mip 2
        ResourceInfo.GetOffset(OffsetInfo);
        // MipOffset = Mip1Offset + Mip1Width in tiles
        ULONG MipOffset = SizeOfMip0 + ((Pitch >> 1) / TileSize[X]) * PAGE_SIZE;
        EXPECT_EQ(MipOffset, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mips 3-8 should be on tile boundary
        for (int i = 3; i < 9; i++)
        {
            OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.MipLevel = i;
            ResourceInfo.GetOffset(OffsetInfo);

            HeightOfPrevMip = (ResourceHeight >> (i - 1));
            AllocationHeight += HeightOfPrevMip;
            MipOffset += Pitch * HeightOfPrevMip;
            EXPECT_EQ(MipOffset, OffsetInfo.Render.Offset64);
            // No X/Y/Z offset since mip is at tile boundary
            EXPECT_EQ(0, OffsetInfo.Render.XOffset);
            EXPECT_EQ(0, OffsetInfo.Render.YOffset);
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);
        }

        // Mips 9 will share 1 tile with Mip 8
        AllocationHeight += TileSize[Y];
        ULONG HeightOfPrevMipsInTile = 0;
        {
            OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.MipLevel = 9;
            ResourceInfo.GetOffset(OffsetInfo);

            EXPECT_EQ(MipOffset, OffsetInfo.Render.Offset64); // Same as previous tile aligned mip offset

                                                              // X is 0, but Y offset will change
            EXPECT_EQ(0, OffsetInfo.Render.XOffset);
            HeightOfPrevMipsInTile += (ResourceHeight >> (9 - 1));
            EXPECT_EQ(HeightOfPrevMipsInTile, OffsetInfo.Render.YOffset);
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);
        }

        // Mip 10 is back on tile boundary
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 10; // Mip 10
        ResourceInfo.GetOffset(OffsetInfo);

        MipOffset += Pitch * TileSize[Y];
        AllocationHeight += TileSize[Y];
        EXPECT_EQ(MipOffset, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Verify ResourceSize and Pitch
        VerifyResourceSize<true>(ResourceInfo, AllocationHeight * Pitch);
        VerifyResourcePitch<true>(ResourceInfo, Pitch);
        VerifyResourcePitchInTiles<true>(ResourceInfo, Pitch / TileSize[X]);
        // These are verified elsewhere
        VerifyResourceHAlign<false>(ResourceInfo, 0);
        VerifyResourceVAlign<false>(ResourceInfo, 0);
        VerifyResourceQPitch<false>(ResourceInfo, 0);
    }


    // Test where Mip1 + Mip2 width > Mip0 width, and where everything fits in 1 tile
    for (UINT i = 0; i < TEST_BPP_64; i++)
    {
        const ULONG ResourceWidth = 0x4;
        const ULONG ResourceHeight = 0x2;
        const ULONG MaxLod = 0x2;

        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        GMM_RESCREATE_PARAMS gmmParams = {};
        gmmParams.Type = RESOURCE_2D;
        gmmParams.Flags.Info.TiledX = 1;
        gmmParams.NoGfxMemory = 1;
        gmmParams.Flags.Gpu.Texture = 1;
        gmmParams.BaseWidth64 = ResourceWidth;
        gmmParams.BaseHeight = ResourceHeight;
        gmmParams.MaxLod = MaxLod;
        gmmParams.Format = SetResourceFormat(bpp);

        // Create resource
        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        // Get Alignment that GmmLib is using
        const ULONG HAlign = ResourceInfo.GetHAlign();
        const ULONG VAlign = ResourceInfo.GetVAlign();

        // Mip1 + Mip2 is the widest width. So it will dictate the pitch of the whole surface
        ULONG Pitch = GetBppValue(bpp) *
            (GMM_ULT_ALIGN(ResourceWidth >> 1, HAlign) + GMM_ULT_ALIGN(ResourceWidth >> 2, HAlign));
        Pitch = GMM_ULT_ALIGN(Pitch, TileSize[X]);

        // Mip0 should be at offset 0 and tile aligned
        GMM_REQ_OFFSET_INFO OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 0; //Mip 0
        ResourceInfo.GetOffset(OffsetInfo);
        EXPECT_EQ(0, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip1 should be after whole Mip0. Not tile aligned
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 1; // Mip 1
        ResourceInfo.GetOffset(OffsetInfo);
        EXPECT_EQ(0, OffsetInfo.Render.Offset64); // Same tile as Mip0
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(GMM_ULT_ALIGN(ResourceHeight, VAlign), OffsetInfo.Render.YOffset); // After Mip0
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip2 should be on the right of Mip1. Not tile aligned
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 2; // Mip 2
        ResourceInfo.GetOffset(OffsetInfo);
        EXPECT_EQ(0, OffsetInfo.Render.Offset64); // Same Tile as Mip0
        ULONG Mip1Width = GMM_ULT_ALIGN(ResourceWidth >> 1, HAlign) * GetBppValue(bpp);
        EXPECT_EQ(Mip1Width, OffsetInfo.Render.XOffset);    // On right of Mip1
        EXPECT_EQ(GMM_ULT_ALIGN(ResourceHeight, VAlign), OffsetInfo.Render.YOffset); // After Mip0
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Verify ResourceSize and Pitch
        VerifyResourceSize<true>(ResourceInfo, PAGE_SIZE); // everything should fit in 1 tile
        VerifyResourcePitch<true>(ResourceInfo, Pitch);
        VerifyResourcePitchInTiles<true>(ResourceInfo, Pitch / TileSize[X]);
        // These are verified elsewhere
        VerifyResourceHAlign<false>(ResourceInfo, 0);
        VerifyResourceVAlign<false>(ResourceInfo, 0);
        VerifyResourceQPitch<false>(ResourceInfo, 0);
    }
}

/// @brief ULT for 2D Linear Resource
TEST_F(CTestResource, Test2DLinearResource)
{
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_2D;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.Linear = 1;
    gmmParams.Flags.Gpu.Texture = 1;

    //Allocate 1x1 surface
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x1;
        gmmParams.BaseHeight = 0x1;

        const UINT MinPitch = 32;
        const UINT PitchAlignment = 64;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        const UINT AlignedHeight = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        UINT PitchInBytes = AlignedWidth * GetBppValue(bpp);
        PitchInBytes = GMM_ULT_MAX(PitchInBytes, MinPitch);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        const UINT AlignedSize = GMM_ULT_ALIGN(PitchInBytes * AlignedHeight, PAGE_SIZE);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourceSize<true>(ResourceInfo, AlignedSize);
    }

    // Allocate 256 x 256
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 256;
        gmmParams.BaseHeight = 256;

        const UINT MinPitch = 32;
        const UINT PitchAlignment = 64;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        const UINT AlignedHeight = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        UINT PitchInBytes = AlignedWidth * GetBppValue(bpp);
        PitchInBytes = GMM_ULT_MAX(PitchInBytes, MinPitch);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        const UINT AlignedSize = GMM_ULT_ALIGN(PitchInBytes * AlignedHeight, PAGE_SIZE);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourceSize<true>(ResourceInfo, AlignedSize);
    }

}

/// @brief ULT for 2D TileX Resource
TEST_F(CTestResource, Test2DTileXResource)
{
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;

    const ULONG TileWidth = 512;
    const ULONG TileHeight = 8;

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_2D;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.TiledX = 1;
    gmmParams.Flags.Gpu.Texture = 1;

    //Allocate 1x1 surface
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x1;
        gmmParams.BaseHeight = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, TileWidth);    // As wide as 1 Tile
        VerifyResourcePitchInTiles<true>(ResourceInfo, 1);  // 1 Tile wide
        VerifyResourceSize<true>(ResourceInfo, GMM_KBYTE(4)); // 1 Tile Big
        VerifyResourceQPitch<false>(ResourceInfo, 0);   // Not Tested
    }

    // Allocate surface that requires multi tiles in two dimension
    // Allocate 2 tiles in X dimension
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = (TileWidth / GetBppValue(bpp)) + 1; // 1 pixel larger than 1 tile width
        gmmParams.BaseHeight = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, TileWidth * 2); // As wide as 2 tile
        VerifyResourcePitchInTiles<true>(ResourceInfo, 2); // 2 tile wide
        VerifyResourceSize<true>(ResourceInfo, GMM_KBYTE(4) * 2); // 2 tile big

        VerifyResourceQPitch<false>(ResourceInfo, 0); // Not tested
    }

    // Allocate 2 tiles in X/Y dimension
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = (TileWidth / GetBppValue(bpp)) + 1; // 1 pixel larger than 1 tile width
        gmmParams.BaseHeight = TileHeight + 1; // 1 row larger than 1 tile height

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, TileWidth * 2); // As wide as 2 tile
        VerifyResourcePitchInTiles<true>(ResourceInfo, 2); // 2 tile wide
        VerifyResourceSize<true>(ResourceInfo, GMM_KBYTE(4) * 4); // 4 tile big

        VerifyResourceQPitch<false>(ResourceInfo, 0); // Not tested
    }

}

/// @brief ULT for 2D TileY Resource
TEST_F(CTestResource, Test2DTileYResource)
{
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;

    const ULONG TileWidth = 128;
    const ULONG TileHeight = 32;

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_2D;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.TiledY = 1;
    gmmParams.Flags.Gpu.Texture = 1;

    //Allocate 1x1 surface
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x1;
        gmmParams.BaseHeight = 0x1;

        const UINT MinPitch = 32;
        const UINT PitchAlignment = 32;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT PitchInBytes = AlignedWidth * GetBppValue(bpp);
        PitchInBytes = GMM_ULT_MAX(PitchInBytes, MinPitch);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, TileWidth);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);    
        VerifyResourcePitchInTiles<true>(ResourceInfo, PitchInBytes / TileWidth);  
        VerifyResourceSize<true>(ResourceInfo, PitchInBytes / TileWidth * GMM_KBYTE(4)); 
        VerifyResourceQPitch<false>(ResourceInfo, 0);   // Not Tested
    }

    // Allocate surface that requires multi tiles in two dimension
    // Allocate 2 tiles in X dimension
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = (TileWidth / GetBppValue(bpp)) + 1; // 1 pixel larger than 1 tile width
        gmmParams.BaseHeight = 0x1;

        const UINT MinPitch = 32;
        const UINT PitchAlignment = 32;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT PitchInBytes = AlignedWidth * GetBppValue(bpp);
        PitchInBytes = GMM_ULT_MAX(PitchInBytes, MinPitch);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, TileWidth);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourcePitchInTiles<true>(ResourceInfo, PitchInBytes / TileWidth);
        VerifyResourceSize<true>(ResourceInfo, PitchInBytes / TileWidth * GMM_KBYTE(4));

        VerifyResourceQPitch<false>(ResourceInfo, 0); // Not tested
    }

    // Allocate 2 tiles in X/Y dimension
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = (TileWidth / GetBppValue(bpp)) + 1; // 1 pixel larger than 1 tile width
        gmmParams.BaseHeight = TileHeight + 1; // 1 row larger than 1 tile height

        const UINT MinPitch = 32;
        const UINT PitchAlignment = 32;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT PitchInBytes = AlignedWidth * GetBppValue(bpp);
        PitchInBytes = GMM_ULT_MAX(PitchInBytes, MinPitch);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, TileWidth);
        
        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourcePitchInTiles<true>(ResourceInfo, PitchInBytes / TileWidth);
        VerifyResourceSize<true>(ResourceInfo, PitchInBytes / TileWidth * 2 * GMM_KBYTE(4));

        VerifyResourceQPitch<false>(ResourceInfo, 0); // Not tested
    }

}

/// @brief ULT for Arrayed 2D TileY Resource
TEST_F(CTestResource, Test2DTileYResourceArrays)
{
    // Horizontal/Vertical pixel alignment
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;

    const ULONG TileWidth = 128;
    const ULONG TileHeight = 32;

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_2D;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.TiledY = 1;
    gmmParams.Flags.Gpu.Texture = 1;
    gmmParams.ArraySize = 4;

    // Allocate 2 tiles in X/Y dimension
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = (TileWidth / GetBppValue(bpp)) + 1; // 1 pixel larger than 1 tile width
        gmmParams.BaseHeight = TileHeight + 1; // 1 row larger than 1 tile height
        ULONG AlignedHeight, BlockHeight;
        
        BlockHeight = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        AlignedHeight = GMM_ULT_ALIGN_NP2(BlockHeight*gmmParams.ArraySize, TileHeight);

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT PitchInBytes = AlignedWidth * GetBppValue(bpp);
        PitchInBytes = GMM_ULT_ALIGN(PitchInBytes, TileWidth);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourcePitchInTiles<true>(ResourceInfo, PitchInBytes / TileWidth);
        VerifyResourceSize<true>(ResourceInfo, PitchInBytes * AlignedHeight); // 4 tile big x 4 array size
        VerifyResourceQPitch<true>(ResourceInfo, BlockHeight); 
    }
}

/// @brief ULT for Mipped 2D TileY Resource
TEST_F(CTestResource, Test2DTileYResourceMips)
{
    const ULONG TileSize[2]     = {128, 32};
    enum  Coords {X = 0, Y = 1};

    // Test normal mip case
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        const ULONG ResourceWidth   = 0x200;
        const ULONG ResourceHeight  = 0x200;
        const ULONG MaxLod          = 0x9;


        TEST_BPP bpp            = static_cast<TEST_BPP>(i);
        GMM_RESCREATE_PARAMS gmmParams  = {};
        gmmParams.Type                  = RESOURCE_2D;
        gmmParams.Flags.Info.TiledY     = 1;
        gmmParams.NoGfxMemory           = 1;
        gmmParams.Flags.Gpu.Texture     = 1;
        gmmParams.BaseWidth64           = ResourceWidth;
        gmmParams.BaseHeight            = ResourceHeight;
        gmmParams.MaxLod                = MaxLod;
        gmmParams.Format                = SetResourceFormat(bpp);

        // Create resource
        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        // Mip0 is the widest mip. So it will dictate the pitch of the whole surface
        const ULONG Pitch = ResourceWidth * GetBppValue(bpp);
        ULONG AllocationHeight = 0;

        // Mip0 should be at offset 0 and tile aligned
        GMM_REQ_OFFSET_INFO OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel  = 0; //Mip 0
        ResourceInfo.GetOffset(OffsetInfo);
        EXPECT_EQ(0, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip1 should be after whole Mip0. Should still be tile aligned.
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel  = 1; // Mip 1
        ResourceInfo.GetOffset(OffsetInfo);
        ULONG SizeOfMip0 = Pitch * ResourceHeight;
        ULONG HeightOfPrevMip = AllocationHeight = ResourceHeight;
        EXPECT_EQ(SizeOfMip0, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip2 should be on the right of Mip1. Should still be tile aligned.
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel  = 2; // Mip 2
        ResourceInfo.GetOffset(OffsetInfo);
        // MipOffset = Mip1Offset + Mip1Width in tiles
        ULONG MipOffset = SizeOfMip0 + ((Pitch >> 1) / TileSize[X]) * PAGE_SIZE;
        EXPECT_EQ(MipOffset, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mips 3-5 should be on tile boundary
        for(int i = 3; i < 6; i++)
        {
            OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.MipLevel  = i;
            ResourceInfo.GetOffset(OffsetInfo);

            HeightOfPrevMip = (ResourceHeight >> (i-1));
            AllocationHeight += HeightOfPrevMip;
            MipOffset += Pitch * HeightOfPrevMip;
            EXPECT_EQ(MipOffset, OffsetInfo.Render.Offset64);
            // No X/Y/Z offset since mip is at tile boundary
            EXPECT_EQ(0, OffsetInfo.Render.XOffset);
            EXPECT_EQ(0, OffsetInfo.Render.YOffset);
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);
        }

        // Mips 6-8 will share 1 tile
        AllocationHeight += TileSize[Y];
        ULONG HeightOfPrevMipsInTile = 0;
        for(int i = 6; i < 9; i++)
        {
            OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.MipLevel  = i;
            ResourceInfo.GetOffset(OffsetInfo);

            EXPECT_EQ(MipOffset, OffsetInfo.Render.Offset64); // Same as previous tile aligned mip offset

            // X is 0, but Y offset will change
            EXPECT_EQ(0, OffsetInfo.Render.XOffset);
            HeightOfPrevMipsInTile += (ResourceHeight >> (i-1));
            EXPECT_EQ(HeightOfPrevMipsInTile, OffsetInfo.Render.YOffset);
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);
        }

        // Mip 9 is back on tile boundary
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 9; // Mip 9
        ResourceInfo.GetOffset(OffsetInfo);

        MipOffset += Pitch * TileSize[Y];
        AllocationHeight += TileSize[Y];
        EXPECT_EQ(MipOffset, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Verify ResourceSize and Pitch
        VerifyResourceSize<true>(ResourceInfo, AllocationHeight * Pitch);
        VerifyResourcePitch<true>(ResourceInfo, Pitch);
        VerifyResourcePitchInTiles<true>(ResourceInfo, Pitch/TileSize[X]);
        // These are verified elsewhere
        VerifyResourceHAlign<false>(ResourceInfo, 0);
        VerifyResourceVAlign<false>(ResourceInfo, 0);
        VerifyResourceQPitch<false>(ResourceInfo, 0);
    }


    // Test where Mip1 + Mip2 width > Mip0 width, and where everything fits in 1 tile
    for (UINT i = 0; i < TEST_BPP_64; i++)
    {
        const ULONG ResourceWidth   = 0x4;
        const ULONG ResourceHeight  = 0x2;
        const ULONG MaxLod          = 0x2;

        TEST_BPP bpp            = static_cast<TEST_BPP>(i);
        GMM_RESCREATE_PARAMS gmmParams  = {};
        gmmParams.Type                  = RESOURCE_2D;
        gmmParams.Flags.Info.TiledY     = 1;
        gmmParams.NoGfxMemory           = 1;
        gmmParams.Flags.Gpu.Texture     = 1;
        gmmParams.BaseWidth64           = ResourceWidth;
        gmmParams.BaseHeight            = ResourceHeight;
        gmmParams.MaxLod                = MaxLod;
        gmmParams.Format                = SetResourceFormat(bpp);

        // Create resource
        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        // Get Alignment that GmmLib is using
        const ULONG HAlign = ResourceInfo.GetHAlign();
        const ULONG VAlign = ResourceInfo.GetVAlign();

        // Mip1 + Mip2 is the widest width. So it will dictate the pitch of the whole surface
        ULONG Pitch = GetBppValue(bpp) *
                      (GMM_ULT_ALIGN(ResourceWidth >> 1, HAlign) + GMM_ULT_ALIGN(ResourceWidth >> 2, HAlign));
        Pitch = GMM_ULT_ALIGN(Pitch, TileSize[X]);

        // Mip0 should be at offset 0 and tile aligned
        GMM_REQ_OFFSET_INFO OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel  = 0; //Mip 0
        ResourceInfo.GetOffset(OffsetInfo);
        EXPECT_EQ(0, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip1 should be after whole Mip0. Not tile aligned
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel  = 1; // Mip 1
        ResourceInfo.GetOffset(OffsetInfo);
        EXPECT_EQ(0, OffsetInfo.Render.Offset64); // Same tile as Mip0
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(GMM_ULT_ALIGN(ResourceHeight, VAlign), OffsetInfo.Render.YOffset); // After Mip0
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip2 should be on the right of Mip1. Not tile aligned
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel  = 2; // Mip 2
        ResourceInfo.GetOffset(OffsetInfo);
        EXPECT_EQ(0, OffsetInfo.Render.Offset64); // Same Tile as Mip0
        ULONG Mip1Width = GMM_ULT_ALIGN(ResourceWidth >> 1, HAlign) * GetBppValue(bpp);
        EXPECT_EQ(Mip1Width, OffsetInfo.Render.XOffset);    // On right of Mip1
        EXPECT_EQ(GMM_ULT_ALIGN(ResourceHeight, VAlign), OffsetInfo.Render.YOffset); // After Mip0
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Verify ResourceSize and Pitch
        VerifyResourceSize<true>(ResourceInfo, PAGE_SIZE); // everything should fit in 1 tile
        VerifyResourcePitch<true>(ResourceInfo, Pitch);
        VerifyResourcePitchInTiles<true>(ResourceInfo, Pitch/TileSize[X]);
        // These are verified elsewhere
        VerifyResourceHAlign<false>(ResourceInfo, 0);
        VerifyResourceVAlign<false>(ResourceInfo, 0);
        VerifyResourceQPitch<false>(ResourceInfo, 0);
    }
}

// ********************************************************************************//

/// @brief ULT for 3D Linear Resource
TEST_F(CTestResource, Test3DLinearResource)
{
    // Horizontal/Vertical pixel alignment
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;

    GMM_RESCREATE_PARAMS gmmParams  = {};
    gmmParams.Type                  = RESOURCE_3D;
    gmmParams.NoGfxMemory           = 1;
    gmmParams.Flags.Info.Linear     = 1;
    gmmParams.Flags.Gpu.Texture     = 1;

    // Allocate 1x1x1 surface
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp                = static_cast<TEST_BPP>(i);
        gmmParams.Format            = SetResourceFormat(bpp);
        gmmParams.BaseWidth64       = 0x1;
        gmmParams.BaseHeight        = 0x1;
        gmmParams.Depth             = 0x1;

        const UINT MinPitch         = 32;
        const UINT PitchAlignment   = 32;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const UINT AlignedWidth     = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        const UINT AlignedHeight    = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        UINT PitchInBytes           = AlignedWidth * GetBppValue(bpp);
        PitchInBytes                = GFX_MAX(PitchInBytes, MinPitch);
        PitchInBytes                = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        const UINT AlignedSize      = GMM_ULT_ALIGN(PitchInBytes * AlignedHeight, PAGE_SIZE);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourceSize<true>(ResourceInfo, AlignedSize);
    }

    // Allocate 256 x 256 x 256
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp                = static_cast<TEST_BPP>(i);
        gmmParams.Format            = SetResourceFormat(bpp);
        gmmParams.BaseWidth64       = 256;
        gmmParams.BaseHeight        = 256;
        gmmParams.Depth             = 256;

        const UINT MinPitch         = 32;
        const UINT PitchAlignment   = 32;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const UINT AlignedWidth     = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        const UINT AlignedHeight    = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        UINT PitchInBytes           = AlignedWidth * GetBppValue(bpp);
        PitchInBytes                = GFX_MAX(PitchInBytes, MinPitch);
        PitchInBytes                = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        const UINT AlignedSize      = GMM_ULT_ALIGN(PitchInBytes * AlignedHeight * gmmParams.Depth, PAGE_SIZE);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourceSize<true>(ResourceInfo, AlignedSize);
    }
}

/// @brief ULT for 3D TileX Resource
TEST_F(CTestResource, Test3DTileXResource)
{
    const ULONG HAlign                    = 16;
    const ULONG VAlign                    = 4;
    const ULONG TileSize[TEST_BPP_MAX][3] = { {512, 8, 1},
                                              {512, 8, 1},
                                              {512, 8, 1},
                                              {512, 8, 1},
                                              {512, 8, 1} };

    GMM_RESCREATE_PARAMS gmmParams        = {};
    gmmParams.Type                        = RESOURCE_3D;
    gmmParams.NoGfxMemory                 = 1;
    gmmParams.Flags.Info.TiledX           = 1;
    gmmParams.Flags.Gpu.Texture           = 1;

    // Allocate 1x1x1 surface
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp          = static_cast<TEST_BPP>(i);
        gmmParams.Format      = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x1;
        gmmParams.BaseHeight  = 0x1;
        gmmParams.Depth       = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, TileSize[i][0]);
        VerifyResourcePitchInTiles<true>(ResourceInfo, 1);
        VerifyResourceSize<true>(ResourceInfo, GMM_KBYTE(4));
    }

    // Allocate 2 tiles in X dimension
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp          = static_cast<TEST_BPP>(i);
        gmmParams.Format      = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = (TileSize[i][0] / GetBppValue(bpp)) + 1;
        gmmParams.BaseHeight  = 0x1;
        gmmParams.Depth       = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, 2 * TileSize[i][0]);
        VerifyResourcePitchInTiles<true>(ResourceInfo, 2);
        VerifyResourceSize<true>(ResourceInfo, 2 * GMM_KBYTE(4));
    }

    // Allocate 2 tiles in X/Y dimension
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp          = static_cast<TEST_BPP>(i);
        gmmParams.Format      = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = (TileSize[i][0] / GetBppValue(bpp)) + 1;
        gmmParams.BaseHeight  = TileSize[i][1] + 1;
        gmmParams.Depth       = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, 2 * TileSize[i][0]);
        VerifyResourcePitchInTiles<true>(ResourceInfo, 2);
        VerifyResourceSize<true>(ResourceInfo, 2 * 2 * GMM_KBYTE(4));
    }

    // Allocate 2 tiles in X/Y/Z dimension
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp              = static_cast<TEST_BPP>(i);
        gmmParams.Format          = SetResourceFormat(bpp);
        gmmParams.BaseWidth64     = (TileSize[i][0] / GetBppValue(bpp)) + 1;
        gmmParams.BaseHeight      = TileSize[i][1] + 1;
        gmmParams.Depth           = TileSize[i][2] + 1;

        const UINT MinPitch       = 32;
        const UINT PitchAlignment = 32;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const UINT AlignedWidth   = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT PitchInBytes         = AlignedWidth * GetBppValue(bpp);
        PitchInBytes              = GFX_MAX(PitchInBytes, MinPitch);
        PitchInBytes              = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        PitchInBytes              = GMM_ULT_ALIGN(PitchInBytes, TileSize[i][0]);
        const UINT AlignedHeight  = 24; // See details in GetTotal3DHeight();

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourcePitchInTiles<true>(ResourceInfo, PitchInBytes / TileSize[i][0]);
        VerifyResourceSize<true>(ResourceInfo, AlignedHeight / TileSize[i][1] * PitchInBytes / TileSize[i][0] * GMM_KBYTE(4));
    }
}

/// @brief ULT for 3D TileY Resource
TEST_F(CTestResource, Test3DTileYResource)
{
    // Horizontal/Vertical pixel alignment
    const ULONG HAlign                    = 16;
    const ULONG VAlign                    = 4;
    const ULONG TileSize[TEST_BPP_MAX][3] = { {128, 32, 1},
                                              {128, 32, 1},
                                              {128, 32, 1},
                                              {128, 32, 1},
                                              {128, 32, 1} };

    GMM_RESCREATE_PARAMS gmmParams        = {};
    gmmParams.Type                        = RESOURCE_3D;
    gmmParams.NoGfxMemory                 = 1;
    gmmParams.Flags.Info.TiledY           = 1;
    gmmParams.Flags.Gpu.Texture           = 1;

    // Allocate 1x1x1 surface
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp                = static_cast<TEST_BPP>(i);
        gmmParams.Format            = SetResourceFormat(bpp);
        gmmParams.BaseWidth64       = 0x1;
        gmmParams.BaseHeight        = 0x1;
        gmmParams.Depth             = 0x1;

        const UINT MinPitch         = 32;
        const UINT PitchAlignment   = 32;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const UINT AlignedWidth = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT PitchInBytes       = AlignedWidth * GetBppValue(bpp);
        PitchInBytes            = GFX_MAX(PitchInBytes, MinPitch);
        PitchInBytes            = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        PitchInBytes            = GMM_ULT_ALIGN(PitchInBytes, TileSize[i][0]);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourcePitchInTiles<true>(ResourceInfo, PitchInBytes / TileSize[i][0]);
        VerifyResourceSize<true>(ResourceInfo, PitchInBytes / TileSize[i][0] * GMM_KBYTE(4));
    }

    // Allocate 2 tiles in X dimension
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp              = static_cast<TEST_BPP>(i);
        gmmParams.Format          = SetResourceFormat(bpp);
        gmmParams.BaseWidth64     = (TileSize[i][0] / GetBppValue(bpp)) + 1;
        gmmParams.BaseHeight      = 0x1;
        gmmParams.Depth           = 0x1;

        const UINT MinPitch       = 32;
        const UINT PitchAlignment = 32;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const UINT AlignedWidth   = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT PitchInBytes         = AlignedWidth * GetBppValue(bpp);
        PitchInBytes              = GFX_MAX(PitchInBytes, MinPitch);
        PitchInBytes              = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        PitchInBytes              = GMM_ULT_ALIGN(PitchInBytes, TileSize[i][0]);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourcePitchInTiles<true>(ResourceInfo, PitchInBytes / TileSize[i][0]);
        VerifyResourceSize<true>(ResourceInfo, PitchInBytes / TileSize[i][0] * GMM_KBYTE(4));
    }

    // Allocate 2 tiles in X/Y dimension
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp              = static_cast<TEST_BPP>(i);
        gmmParams.Format          = SetResourceFormat(bpp);
        gmmParams.BaseWidth64     = (TileSize[i][0] / GetBppValue(bpp)) + 1;
        gmmParams.BaseHeight      = TileSize[i][1] + 1;
        gmmParams.Depth           = 0x1;

        const UINT MinPitch       = 32;
        const UINT PitchAlignment = 32;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const UINT AlignedWidth   = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT PitchInBytes         = AlignedWidth * GetBppValue(bpp);
        PitchInBytes              = GFX_MAX(PitchInBytes, MinPitch);
        PitchInBytes              = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        PitchInBytes              = GMM_ULT_ALIGN(PitchInBytes, TileSize[i][0]);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourcePitchInTiles<true>(ResourceInfo, PitchInBytes / TileSize[i][0]);
        VerifyResourceSize<true>(ResourceInfo, PitchInBytes / TileSize[i][0] * 2 * GMM_KBYTE(4));
    }

    // Allocate 2 tiles in X/Y/Z dimension
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp              = static_cast<TEST_BPP>(i);
        gmmParams.Format          = SetResourceFormat(bpp);
        gmmParams.BaseWidth64     = (TileSize[i][0] / GetBppValue(bpp)) + 1;
        gmmParams.BaseHeight      = TileSize[i][1] + 1;
        gmmParams.Depth           = TileSize[i][2] + 1;

        const UINT MinPitch       = 32;
        const UINT PitchAlignment = 32;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const UINT AlignedWidth   = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign);
        UINT PitchInBytes         = AlignedWidth * GetBppValue(bpp);
        PitchInBytes              = GFX_MAX(PitchInBytes, MinPitch);
        PitchInBytes              = GMM_ULT_ALIGN(PitchInBytes, PitchAlignment);
        PitchInBytes              = GMM_ULT_ALIGN(PitchInBytes, TileSize[i][0]);
        const UINT AlignedHeight  = 96; // See details in GetTotal3DHeight();

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, PitchInBytes);
        VerifyResourcePitchInTiles<true>(ResourceInfo, PitchInBytes / TileSize[i][0]);
        VerifyResourceSize<true>(ResourceInfo, AlignedHeight / TileSize[i][1] * PitchInBytes / TileSize[i][0] * GMM_KBYTE(4));
    }
}

/// @brief ULT for Cube Linear Resource
TEST_F(CTestResource, Test3DTileYMippedResource)
{
    // Horizontal/Vertical pixel alignment
    const ULONG HAlign                    = 16;
    const ULONG VAlign                    = 4;
    const ULONG TileSize[TEST_BPP_MAX][3] = { {128, 32, 1},
                                              {128, 32, 1},
                                              {128, 32, 1},
                                              {128, 32, 1},
                                              {128, 32, 1} };

    const ULONG ResourceWidth             = 0x100;
    const ULONG ResourceHeight            = 0x100;
    const ULONG ResourceDepth             = 0x100;
    const ULONG MaxLod                    = 0x9;

    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp                   = static_cast<TEST_BPP>(i);
        GMM_RESCREATE_PARAMS gmmParams = {};
        gmmParams.Type                 = RESOURCE_3D;
        gmmParams.Flags.Info.TiledY    = 1;
        gmmParams.NoGfxMemory          = 1;
        gmmParams.Flags.Gpu.Texture    = 1;
        gmmParams.BaseWidth64          = ResourceWidth;
        gmmParams.BaseHeight           = ResourceHeight;
        gmmParams.Depth                = ResourceDepth;
        gmmParams.MaxLod               = MaxLod;
        gmmParams.Format               = SetResourceFormat(bpp);

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        const ULONG Pitch = ResourceWidth * GetBppValue(bpp);
        const ULONG Mip0Height = ResourceHeight;
        const ULONG Mip0Depth = ResourceDepth;

        // Mip0
        GMM_REQ_OFFSET_INFO OffsetInfo = {};
        OffsetInfo.ReqRender   = 1;
        OffsetInfo.MipLevel    = 0;
        ResourceInfo.GetOffset(OffsetInfo);

        EXPECT_EQ(0, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip1
        OffsetInfo             = {};
        OffsetInfo.ReqRender   = 1;
        OffsetInfo.MipLevel    = 1;
        ResourceInfo.GetOffset(OffsetInfo);

        const ULONG SizeOfMip0 = Pitch * Mip0Height * Mip0Depth;
        const ULONG Mip1Offset = SizeOfMip0;
        const ULONG Mip1Height = Mip0Height >> 1;
        const ULONG Mip1Depth  = Mip0Depth >> 1;

        EXPECT_EQ(Mip1Offset, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip2
        OffsetInfo             = {};
        OffsetInfo.ReqRender   = 1;
        OffsetInfo.MipLevel    = 2;
        ResourceInfo.GetOffset(OffsetInfo);

        const ULONG SizeOfMip1 = (Pitch >> 1) * Mip1Height * Mip1Depth;
        const ULONG Mip2Offset = Mip1Offset + SizeOfMip1;
        const ULONG Mip2Height = Mip1Height >> 1;
        const ULONG Mip2Depth  = Mip1Depth >> 1;

        EXPECT_EQ(Mip2Offset, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip3
        OffsetInfo             = {};
        OffsetInfo.ReqRender   = 1;
        OffsetInfo.MipLevel    = 3;
        ResourceInfo.GetOffset(OffsetInfo);

        const ULONG SizeOfMip2 = (Pitch >> 2) * Mip2Height * Mip2Depth;
        const ULONG Mip3Offset = Mip2Offset + SizeOfMip2;
        const ULONG Mip3Height = Mip2Height >> 1;
        const ULONG Mip3Depth  = Mip2Depth >> 1;

        EXPECT_EQ(Mip3Offset, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip4 (Packed Mip)
        // For those mip width/height smaller than H/VAlign, they are upscaled to align with H/VAlign
        OffsetInfo             = {};
        OffsetInfo.ReqRender   = 1;
        OffsetInfo.MipLevel    = 4;
        ResourceInfo.GetOffset(OffsetInfo);

        const ULONG SizeOfMip3 = (Pitch >> 3) * Mip3Height * Mip3Depth;
        const ULONG Mip4Offset = Mip3Offset + SizeOfMip3;

        EXPECT_EQ(Mip4Offset, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip 5
        OffsetInfo             = {};
        OffsetInfo.ReqRender   = 1;
        OffsetInfo.MipLevel    = 5;
        ResourceInfo.GetOffset(OffsetInfo);

        EXPECT_EQ(Mip4Offset, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(16, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip 6
        OffsetInfo             = {};
        OffsetInfo.ReqRender   = 1;
        OffsetInfo.MipLevel    = 6;
        ResourceInfo.GetOffset(OffsetInfo);

        EXPECT_EQ(Mip4Offset, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(24, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip 7
        OffsetInfo             = {};
        OffsetInfo.ReqRender   = 1;
        OffsetInfo.MipLevel    = 7;
        ResourceInfo.GetOffset(OffsetInfo);

        EXPECT_EQ(Mip4Offset, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(28, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip8 (Start of another packed Mip)
        OffsetInfo             = {};
        OffsetInfo.ReqRender   = 1;
        OffsetInfo.MipLevel    = 8;
        ResourceInfo.GetOffset(OffsetInfo);

        const ULONG Mip8Offset = Mip4Offset + Pitch * TileSize[i][1];

        EXPECT_EQ(Mip8Offset, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        // Mip9
        OffsetInfo             = {};
        OffsetInfo.ReqRender   = 1;
        OffsetInfo.MipLevel    = 9;
        ResourceInfo.GetOffset(OffsetInfo);

        EXPECT_EQ(Mip8Offset, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(4, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);
    }
}

// ********************************************************************************//

/// @brief ULT for Cube Linear Resource
TEST_F(CTestResource, TestCubeLinearResource)
{
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_CUBE;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.Linear = 1;
    gmmParams.Flags.Gpu.Texture = 1;

    // Allocate 1x1
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x1;
        gmmParams.BaseHeight = 0x1;
        gmmParams.Depth = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);

        ULONG ExpectedPitch = GMM_ULT_MAX(GMM_BYTES(32), HAlign * GetBppValue(bpp));   // Min Pitch = 32 bytes
        VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch);                     // As wide as 1 tile
        VerifyResourcePitchInTiles<false>(ResourceInfo, 1);                         // not applicable

        ULONG ExpectedQPitch = VAlign;
        VerifyResourceQPitch<true>(ResourceInfo, ExpectedQPitch);       // Each face should be VAlign rows apart within a tile

        VerifyResourceSize<true>(ResourceInfo,                          // PitchInBytes * Rows where Rows = __GMM_MAX_CUBE_FACE x QPitch, then aligned PAGE_SIZE
            GMM_ULT_ALIGN(ExpectedPitch *
                __GMM_MAX_CUBE_FACE * ExpectedQPitch,
                PAGE_SIZE));

        for (UINT CubeFaceIndex = 0; CubeFaceIndex < __GMM_MAX_CUBE_FACE; CubeFaceIndex++)
        {
            GMM_REQ_OFFSET_INFO OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.CubeFace = static_cast<GMM_CUBE_FACE_ENUM>(CubeFaceIndex);
            ResourceInfo.GetOffset(OffsetInfo);

            EXPECT_EQ(CubeFaceIndex * ExpectedQPitch * ExpectedPitch,
                OffsetInfo.Render.Offset64);                  // Render offset is tile's base address on which cube face begins.
            EXPECT_EQ(0, OffsetInfo.Render.XOffset);                // X Offset should be 0
            EXPECT_EQ(0, OffsetInfo.Render.YOffset);                // Y Offset should be 0
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);                // Z offset N/A should be 0
        }
    }

    // Allocate arbitrary size (X/Y dimension not applicable as linear surface)
    // Width and Height must be equal
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x201;                 // 512 + 1, help ult HAlign/VAlign/Pitch alignment logic as well.
        gmmParams.BaseHeight = gmmParams.BaseWidth64;  // Heigth must be equal to width.
        gmmParams.Depth = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);

        ULONG ExpectedPitch = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign) * GetBppValue(bpp);    // HAligned-width in bytes.
        ExpectedPitch = GMM_ULT_ALIGN(ExpectedPitch, GMM_BYTES(32));
        VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch);
        VerifyResourcePitchInTiles<false>(ResourceInfo, 2);             // not applicable

        ULONG ExpectedQPitch = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        VerifyResourceQPitch<true>(ResourceInfo, ExpectedQPitch);       // Each face should be Valigned-BaseHeight rows apart

        VerifyResourceSize<true>(ResourceInfo,                          // PitchInBytes * Rows where Rows = __GMM_MAX_CUBE_FACE x QPitch, then aligned PAGE_SIZE
            GMM_ULT_ALIGN(ExpectedPitch *
                __GMM_MAX_CUBE_FACE * ExpectedQPitch,
                PAGE_SIZE));

        for (UINT CubeFaceIndex = 0; CubeFaceIndex < __GMM_MAX_CUBE_FACE; CubeFaceIndex++)
        {
            GMM_REQ_OFFSET_INFO OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.CubeFace = static_cast<GMM_CUBE_FACE_ENUM>(CubeFaceIndex);
            ResourceInfo.GetOffset(OffsetInfo);

            EXPECT_EQ(CubeFaceIndex * ExpectedQPitch * ExpectedPitch,
                OffsetInfo.Render.Offset64);                  // Render offset is tile's base address on which cube face begins.
            EXPECT_EQ(0, OffsetInfo.Render.XOffset);                // X Offset should be 0
            EXPECT_EQ(0, OffsetInfo.Render.YOffset);                // Y Offset should be 0
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);                // Z offset N/A should be 0
        }
    }
}

/// @brief ULT for Cube Linear Mipped Resource
TEST_F(CTestResource, TestCubeLinearMippedResourceArray)
{
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;
    const ULONG MaxLod = 9;
    const ULONG ResWidth = 0x201;
    const ULONG MaxArraySize = 0x10;


    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_CUBE;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.Linear = 1;
    gmmParams.Flags.Gpu.Texture = 1;
    gmmParams.MaxLod = MaxLod;
    gmmParams.ArraySize = MaxArraySize;

    // Allocate arbitrary size (X/Y dimension not applicable as linear surface)
    // Width and Height must be equal
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        struct  //Cache the value for verifying array elements/Cube face offset/Mip Offset
        {
            ULONGLONG Offset;  // Note : absolute mip offset
        } RenderOffset[GMM_ULT_MAX_MIPMAP] = {};

        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = ResWidth;              // 1024 + 1, help ult HAlign/VAlign/Pitch alignment logic as well.                 
        gmmParams.BaseHeight = gmmParams.BaseWidth64;  // Heigth must be equal to width.
        gmmParams.Depth = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);

        //------------------------------|
        //                              |
        //          LOD0                |
        //                              |
        //                              |
        //------------------------------|
        //    LOD1     |    LOD2  |
        //             |----------|
        //             | LOD3 |
        //-------------| LOD4 .. so on

        // Mip 0
        // Mip 0 decides the pitch of the entire surface
        const ULONG  AlignedWidthMip0 = GMM_ULT_ALIGN(ResWidth, HAlign);                           // HAlign width in pixel
        const ULONG  AlignedHeightMip0 = GMM_ULT_ALIGN(ResWidth, VAlign);
        ULONG ExpectedPitch = GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign) * GetBppValue(bpp);    // HAligned-width in bytes.
        ExpectedPitch = GMM_ULT_ALIGN(ExpectedPitch, GMM_BYTES(32));
        VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch);
        VerifyResourcePitchInTiles<false>(ResourceInfo, 2);                                     // Not applicable

                                                                                                // Mip0 should be at offset 0 and tile aligned
        GMM_REQ_OFFSET_INFO OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 0; //Mip 0
        ResourceInfo.GetOffset(OffsetInfo);
        EXPECT_EQ(0, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        //cache Mip 0 offset
        RenderOffset[0].Offset = 0;

        // Mip 1 should be under mip 0
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 1;    //Mip 1
        ResourceInfo.GetOffset(OffsetInfo);

        EXPECT_EQ(AlignedHeightMip0 * ExpectedPitch,    // Render offset is the absolute address at which the mip begins
            OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);        // Not applicable for linear surface
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);        // Not applicable for linear surface
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);        // n/a

                                                        //cache Mip 1 offset
        RenderOffset[1].Offset = AlignedHeightMip0 * ExpectedPitch; //Absolute base

        const ULONG  AlignedWidthMip1 = GMM_ULT_ALIGN(ResWidth >> 1, HAlign);              // Align width in pixel to HAlign
        const ULONG  AlignedHeightMip1 = GMM_ULT_ALIGN(ResWidth >> 1, VAlign);

        ULONG HeightOfMip;
        ULONG HeightLinesLevel2 = 0;

        // Mips 2-9 should be stacked on the right of Mip1 as shown in figure above.
        for (int i = 2; i <= MaxLod; i++)
        {
            OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.MipLevel = i;
            ResourceInfo.GetOffset(OffsetInfo);

            HeightOfMip = GMM_ULT_ALIGN(ResWidth >> i, VAlign);

            EXPECT_EQ((AlignedHeightMip0 + HeightLinesLevel2) * ExpectedPitch +   // Render offset is tile's base address on which mip begins
                (AlignedWidthMip1 * GetBppValue(bpp)),
                OffsetInfo.Render.Offset64);

            EXPECT_EQ(0, OffsetInfo.Render.XOffset);        // Not applicable for linear surface
            EXPECT_EQ(0, OffsetInfo.Render.YOffset);        // Not applicable for linear surface
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

            //cache Mip i'th offset
            RenderOffset[i].Offset = (AlignedHeightMip0 + HeightLinesLevel2) * ExpectedPitch +
                (AlignedWidthMip1 * GetBppValue(bpp));

            HeightLinesLevel2 += HeightOfMip;
        }
        
        ULONG ExpectedQPitch = AlignedHeightMip0 + AlignedHeightMip1 + 12 * VAlign;
        VerifyResourceQPitch<true>(ResourceInfo, ExpectedQPitch);       // Each face should be Valigned-BaseHeight rows apart 

        VerifyResourceSize<true>(ResourceInfo,                          // PitchInBytes * Rows where Rows = __GMM_MAX_CUBE_FACE x QPitch, then aligned to tile boundary
            GMM_ULT_ALIGN(ExpectedPitch *
                MaxArraySize * __GMM_MAX_CUBE_FACE * ExpectedQPitch, PAGE_SIZE));

        // Verify each array element's  Mip offset, Cube face offset etc.
        for (UINT ArrayIndex = 0; ArrayIndex < __GMM_MAX_CUBE_FACE; ArrayIndex++)
        {
            for (UINT CubeFaceIndex = 0; CubeFaceIndex < __GMM_MAX_CUBE_FACE; CubeFaceIndex++)
            {
                GMM_REQ_OFFSET_INFO OffsetInfo = {};
                OffsetInfo.ReqRender = 1;
                OffsetInfo.ArrayIndex = ArrayIndex;
                OffsetInfo.CubeFace = static_cast<GMM_CUBE_FACE_ENUM>(CubeFaceIndex);
                ResourceInfo.GetOffset(OffsetInfo);

                //Verify cube face offsets
                EXPECT_EQ(((6 * ArrayIndex) + CubeFaceIndex) * ExpectedQPitch * ExpectedPitch,
                    OffsetInfo.Render.Offset64);                  // Render offset is tile's base address on which cube face begins.
                EXPECT_EQ(0, OffsetInfo.Render.XOffset);                // X Offset should be 0 as linear surf
                EXPECT_EQ(0, OffsetInfo.Render.YOffset);                // Y Offset should be 0 as linear surf
                EXPECT_EQ(0, OffsetInfo.Render.ZOffset);                // Z offset N/A should be 0

                ULONG CubeFaceBaseOffset = ((6 * ArrayIndex) + CubeFaceIndex) * (ExpectedQPitch * ExpectedPitch);

                //Verify mip offsets in each cube face
                for (UINT Lod = 0; Lod <= MaxLod; Lod++)
                {
                    OffsetInfo.MipLevel = Lod;
                    ResourceInfo.GetOffset(OffsetInfo);

                    ULONG MipOffset = CubeFaceBaseOffset + RenderOffset[Lod].Offset;

                    ULONG OffsetX = MipOffset % ExpectedPitch;
                    ULONG OffsetY = MipOffset / ExpectedPitch;

                    ULONG RenderAlignOffset = OffsetY * ExpectedPitch + OffsetX;

                    EXPECT_EQ(RenderAlignOffset, OffsetInfo.Render.Offset64);   // Render offset absolute address on which cube face begins.
                    EXPECT_EQ(0, OffsetInfo.Render.XOffset);                    // X Offset should be 0 as linear surf
                    EXPECT_EQ(0, OffsetInfo.Render.YOffset);                    // Y Offset should be 0 as linear surf
                    EXPECT_EQ(0, OffsetInfo.Render.ZOffset);                    // Z offset N/A should be 0

                }
            }
        }
    }
}

/// @brief ULT for Cube TileX Resource
TEST_F(CTestResource, TestCubeTileXResource)
{
    // Cube is allocated as an array of 6 2D surface representing each cube face below
    //===============================
    //  q  coordinate  |    face    |
    //      0          |    + x     |
    //      1          |    - x     |
    //      2          |    + y     |
    //      3          |    - y     |
    //      4          |    + z     |
    //      5          |    - z     |
    //===============================

    const ULONG HAlign = 16;
    const ULONG VAlign = 4;

    const ULONG TileSize[1][2] = { 512, 8 };

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_CUBE;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.TiledX = 1;
    gmmParams.Flags.Gpu.Texture = 1;

    // Allocate 1x1 surface so that it occupies 1 Tile in X dimension
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x1;
        gmmParams.BaseHeight = 0x1;
        gmmParams.Depth = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        ULONG ExpectedPitch = TileSize[0][0];
        VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch); // As wide as 1 tile
        VerifyResourcePitchInTiles<true>(ResourceInfo, 1);      // 1 tile wide

        ULONG ExpectedQPitch = VAlign;
        VerifyResourceQPitch<true>(ResourceInfo, ExpectedQPitch);        // Each face should be VAlign rows apart within a tile

        VerifyResourceSize<true>(ResourceInfo,                          // PitchInBytes * Rows where Rows = __GMM_MAX_CUBE_FACE x QPitch, then aligned to tile boundary
            ExpectedPitch *
            GMM_ULT_ALIGN(__GMM_MAX_CUBE_FACE * ExpectedQPitch,
                TileSize[0][1]));

        for (UINT CubeFaceIndex = 0; CubeFaceIndex < __GMM_MAX_CUBE_FACE; CubeFaceIndex++)
        {
            GMM_REQ_OFFSET_INFO OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.CubeFace = static_cast<GMM_CUBE_FACE_ENUM>(CubeFaceIndex);
            ResourceInfo.GetOffset(OffsetInfo);

            EXPECT_EQ(GMM_ULT_ALIGN_FLOOR(CubeFaceIndex * ExpectedQPitch, TileSize[0][1]) * ExpectedPitch,
                OffsetInfo.Render.Offset64);                  // Render offset is tile's base address on which cube face begins.
            EXPECT_EQ(0, OffsetInfo.Render.XOffset);                // X Offset should be 0
            EXPECT_EQ((CubeFaceIndex * ExpectedQPitch) % TileSize[0][1],
                OffsetInfo.Render.YOffset);                  // Y Offset should be (CubeFaceIndex * QPitch) % TileHeight
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);                // Z offset N/A should be 0

        }
    }

    // Allocate 2 tiles in X dimension.
    // Width and Height must be equal
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = (TileSize[0][0] / GetBppValue(bpp)) + 1; // 1 pixel larger than 1 tile width
        gmmParams.BaseHeight = gmmParams.BaseWidth64;  // Heigth must be equal to width.
        gmmParams.Depth = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);

        ULONG  ExpectedPitch = TileSize[0][0] * 2;                      // As wide as 2 tile
        VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch);
        VerifyResourcePitchInTiles<true>(ResourceInfo, 2);             // 2 tile wide

        ULONG ExpectedQPitch = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        VerifyResourceQPitch<true>(ResourceInfo, ExpectedQPitch);       // Each face should be Valigned-BaseHeight rows apart

        VerifyResourceSize<true>(ResourceInfo,                          // PitchInBytes * Rows where Rows = __GMM_MAX_CUBE_FACE x QPitch, then aligned to tile boundary
            ExpectedPitch *
            GMM_ULT_ALIGN(__GMM_MAX_CUBE_FACE * ExpectedQPitch,
                TileSize[0][1]));

        for (UINT CubeFaceIndex = 0; CubeFaceIndex < __GMM_MAX_CUBE_FACE; CubeFaceIndex++)
        {
            GMM_REQ_OFFSET_INFO OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.CubeFace = static_cast<GMM_CUBE_FACE_ENUM>(CubeFaceIndex);
            ResourceInfo.GetOffset(OffsetInfo);
            EXPECT_EQ(GMM_ULT_ALIGN_FLOOR(CubeFaceIndex * ExpectedQPitch, TileSize[0][1]) * ExpectedPitch,
                OffsetInfo.Render.Offset64);              // Render offset is tile's base address on which cube face begins.
            EXPECT_EQ(0, OffsetInfo.Render.XOffset);            // X Offset should be 0
            EXPECT_EQ((CubeFaceIndex * ExpectedQPitch) % TileSize[0][1],
                OffsetInfo.Render.YOffset);               // Y Offset should be (CubeFaceIndex * QPitch) % TileHeight
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);            // Z offset N/A should be 0
        }
    }
}


/// @brief ULT for Cube TileY Resource
TEST_F(CTestResource, TestCubeTileYResource)
{
    // Cube is allocated as an array of 6 2D surface representing each cube face below
    //===============================
    //   q coordinate  |    face    |
    //      0          |    + x     |
    //      1          |    - x     |
    //      2          |    + y     |
    //      3          |    - y     |
    //      4          |    + z     |
    //      5          |    - z     |
    //===============================

    const ULONG HAlign = 16;
    const ULONG VAlign = 4;

    const ULONG TileSize[1][2] = { 128, 32 };

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_CUBE;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.TiledY = 1;
    gmmParams.Flags.Gpu.Texture = 1;

    // Allocate 1x1 surface within a tile.
    for (UINT i = 0; i < TEST_BPP_128; i++)  //TEST_BPP_128 cannot fit in a tile as HAlign = 16
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = 0x1;
        gmmParams.BaseHeight = 0x1;
        gmmParams.Depth = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        VerifyResourcePitch<true>(ResourceInfo, TileSize[0][0]); // As wide as 1 tile
        VerifyResourcePitchInTiles<true>(ResourceInfo, 1);       // 1 tile wide
        VerifyResourceSize<true>(ResourceInfo, GMM_KBYTE(4));    // All 6 faces should be accomated in a tile.
        VerifyResourceQPitch<true>(ResourceInfo, VAlign);        // Each face should be VAlign rows apart within a tile

        for (UINT CubeFaceIndex = 0; CubeFaceIndex < __GMM_MAX_CUBE_FACE; CubeFaceIndex++)
        {
            GMM_REQ_OFFSET_INFO OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.CubeFace = static_cast<GMM_CUBE_FACE_ENUM>(CubeFaceIndex);
            ResourceInfo.GetOffset(OffsetInfo);
            EXPECT_EQ(0, OffsetInfo.Render.Offset64);           // Render offset should be 0 as its on single tile.
            EXPECT_EQ(0, OffsetInfo.Render.XOffset);            // X Offset should be 0
            EXPECT_EQ(CubeFaceIndex * VAlign, OffsetInfo.Render.YOffset);   // Y Offset should be VALIGN * CubeFace Index
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);
        }
    }

    // Allocate 2 tiles in X dimension.
    // Width and Height of Cube must be equal
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = (TileSize[0][0] / GetBppValue(bpp)) + 1; // 1 pixel larger than 1 tile width
        gmmParams.BaseHeight = gmmParams.BaseWidth64;                    // Heigth must be equal to width.
        gmmParams.Depth = 0x1;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);

        ULONG  ExpectedPitch = TileSize[0][0] * 2;                      // As wide as 2 tile
        VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch);
        VerifyResourcePitchInTiles<true>(ResourceInfo, 2);             // 2 tile wide

        ULONG ExpectedQPitch = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
        VerifyResourceQPitch<true>(ResourceInfo, ExpectedQPitch);       // Each face should be Valigned-BaseHeight rows apart

        VerifyResourceSize<true>(ResourceInfo,                          // PitchInBytes * Rows where Rows = __GMM_MAX_CUBE_FACE x QPitch, then aligned to tile boundary
            ExpectedPitch *
            GMM_ULT_ALIGN(__GMM_MAX_CUBE_FACE * ExpectedQPitch,
                TileSize[0][1]));


        for (UINT CubeFaceIndex = 0; CubeFaceIndex < __GMM_MAX_CUBE_FACE; CubeFaceIndex++)
        {
            GMM_REQ_OFFSET_INFO OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.CubeFace = static_cast<GMM_CUBE_FACE_ENUM>(CubeFaceIndex);
            ResourceInfo.GetOffset(OffsetInfo);
            EXPECT_EQ(GMM_ULT_ALIGN_FLOOR(CubeFaceIndex * ExpectedQPitch, TileSize[0][1]) * ExpectedPitch,
                OffsetInfo.Render.Offset64);                  // Render offset is tile's base address on which cube face begins.
            EXPECT_EQ(0, OffsetInfo.Render.XOffset);                // X Offset should be 0
            EXPECT_EQ((CubeFaceIndex * ExpectedQPitch) % TileSize[0][1],
                OffsetInfo.Render.YOffset);                  // Y Offset should be (CubeFaceIndex * QPitch) % TileHeight
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);                // Z offset N/A should be 0
        }
    }
}

/// @brief ULT for Cube TileY Mipped Resource Array
TEST_F(CTestResource, TestCubeTileYMippedResourceArray)
{
    const ULONG HAlign = 16;
    const ULONG VAlign = 4;

    const ULONG TileSize[2] = { 128, 32 };
    enum  Coords { X = 0, Y = 1 };

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_CUBE;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.TiledY = 1;
    gmmParams.Flags.Gpu.Texture = 1;

    // Allocate CUBE surface
    for (UINT i = 0; i < TEST_BPP_MAX; i++)
    {
        const ULONG ResWidth = 0x201;
        const ULONG MaxLod = 0x9;
        const ULONG MaxArraySize = 0x10;

        struct  //Cache the value for verifying array elements/Cube face offset/Mip Offset
        {
            ULONGLONG Offset;  // Note : absolute mip offset
        } RenderOffset[GMM_ULT_MAX_MIPMAP];

        TEST_BPP bpp = static_cast<TEST_BPP>(i);
        gmmParams.Format = SetResourceFormat(bpp);
        gmmParams.BaseWidth64 = ResWidth;                                  // 1 pixel larger than 1 tile width
        gmmParams.BaseHeight = gmmParams.BaseWidth64;                     // Heigth must be equal to width.
        gmmParams.Depth = 0x1;
        gmmParams.MaxLod = MaxLod;
        gmmParams.ArraySize = MaxArraySize;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);

        //------------------------------|
        //                              |
        //          LOD0                |
        //                              |
        //                              |
        //------------------------------|
        //    LOD1     |    LOD2  |
        //             |----------|
        //             | LOD3 |
        //-------------| LOD4 .. so on

        //Mip 0
        //Mip 0 decides the pitch of the entire resource.
        const ULONG  AlignedWidthMip0 = GMM_ULT_ALIGN(ResWidth, HAlign);                           // HAlign width in pixel
        const ULONG  AlignedHeightMip0 = GMM_ULT_ALIGN(ResWidth, VAlign);
        ULONG  ExpectedPitch = GMM_ULT_ALIGN(AlignedWidthMip0 * GetBppValue(bpp), TileSize[X]);    // Align AlignedWidthMip0 to 128 bytes
        VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch);
        VerifyResourcePitchInTiles<true>(ResourceInfo, ExpectedPitch / TileSize[X]);            // Pitch/TileY-Width

                                                                                                // Mip0 should be at offset 0 and tile aligned
        GMM_REQ_OFFSET_INFO OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 0; //Mip 0
        ResourceInfo.GetOffset(OffsetInfo);
        EXPECT_EQ(0, OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);
        EXPECT_EQ(0, OffsetInfo.Render.YOffset);
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        //cache Mip 0 offset
        RenderOffset[0].Offset = 0;

        // Mip 1 should be under mip 0
        OffsetInfo = {};
        OffsetInfo.ReqRender = 1;
        OffsetInfo.MipLevel = 1;    //Mip 1
        ResourceInfo.GetOffset(OffsetInfo);

        EXPECT_EQ(GMM_ULT_ALIGN_FLOOR(AlignedHeightMip0, TileSize[Y]) * ExpectedPitch,     // Render offset is tile's base address on which mip begins
            OffsetInfo.Render.Offset64);
        EXPECT_EQ(0, OffsetInfo.Render.XOffset);                                        // Aligns with Mip0 at X = 0
        EXPECT_EQ((AlignedHeightMip0) % TileSize[Y], OffsetInfo.Render.YOffset);        // AlignedHeightMip0 % TileY-Height
        EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

        //cache Mip 1 offset
        RenderOffset[1].Offset = AlignedHeightMip0 * ExpectedPitch; //Absolute base

        const ULONG  AlignedWidthMip1 = GMM_ULT_ALIGN(ResWidth >> 1, HAlign);              // Align width in pixel to HAlign
        const ULONG  AlignedHeightMip1 = GMM_ULT_ALIGN(ResWidth >> 1, VAlign);

        ULONG HeightOfMip;
        ULONG HeightLinesLevel2 = 0;

        // Mips 2-9 should be stacked on the right of Mip1 as shown in figure above.
        for (int i = 2; i <= MaxLod; i++)
        {
            OffsetInfo = {};
            OffsetInfo.ReqRender = 1;
            OffsetInfo.MipLevel = i;
            ResourceInfo.GetOffset(OffsetInfo);

            HeightOfMip = GMM_ULT_ALIGN(ResWidth >> i, VAlign);

            EXPECT_EQ(GMM_ULT_ALIGN_FLOOR(AlignedHeightMip0 + HeightLinesLevel2, TileSize[Y]) * ExpectedPitch +   // Render offset is tile's base address on which mip begins
                (AlignedWidthMip1 * GetBppValue(bpp) / TileSize[X]) * PAGE_SIZE,
                OffsetInfo.Render.Offset64);

            EXPECT_EQ((AlignedWidthMip1 * GetBppValue(bpp)) % TileSize[X], OffsetInfo.Render.XOffset);          // Aligns with Mip0 at X = 0
            EXPECT_EQ((AlignedHeightMip0 + HeightLinesLevel2) % TileSize[Y], OffsetInfo.Render.YOffset);        // AlignedHeightMip0 % TileY-Height
            EXPECT_EQ(0, OffsetInfo.Render.ZOffset);

            //cache Mip i'th offset
            RenderOffset[i].Offset = (AlignedHeightMip0 + HeightLinesLevel2) * ExpectedPitch +
                (AlignedWidthMip1 * GetBppValue(bpp));

            HeightLinesLevel2 += HeightOfMip;
        }

        ULONG ExpectedQPitch = AlignedHeightMip0 + AlignedHeightMip1 + 12 * VAlign;
        VerifyResourceQPitch<true>(ResourceInfo, ExpectedQPitch);       // Each face should be Valigned-BaseHeight rows apart 

        VerifyResourceSize<true>(ResourceInfo,                          // PitchInBytes * Rows where Rows = __GMM_MAX_CUBE_FACE x QPitch, then aligned to tile boundary
            ExpectedPitch *
            GMM_ULT_ALIGN(MaxArraySize * __GMM_MAX_CUBE_FACE * ExpectedQPitch, TileSize[Y]));

        // Verify each array element's  Mip offset, Cube face offset etc.
        for (UINT ArrayIndex = 0; ArrayIndex < __GMM_MAX_CUBE_FACE; ArrayIndex++)
        {
            for (UINT CubeFaceIndex = 0; CubeFaceIndex < __GMM_MAX_CUBE_FACE; CubeFaceIndex++)
            {
                GMM_REQ_OFFSET_INFO OffsetInfo = {};
                OffsetInfo.ReqRender = 1;
                OffsetInfo.ArrayIndex = ArrayIndex;
                OffsetInfo.CubeFace = static_cast<GMM_CUBE_FACE_ENUM>(CubeFaceIndex);
                ResourceInfo.GetOffset(OffsetInfo);

                //Verify cube face offsets
                EXPECT_EQ(GMM_ULT_ALIGN_FLOOR(((6 * ArrayIndex) + CubeFaceIndex) * ExpectedQPitch, TileSize[Y]) * ExpectedPitch,
                    OffsetInfo.Render.Offset64);                  // Render offset is tile's base address on which cube face begins.
                EXPECT_EQ(0, OffsetInfo.Render.XOffset);                // X Offset should be 0 
                EXPECT_EQ((((6 * ArrayIndex) + CubeFaceIndex) * ExpectedQPitch) % TileSize[Y],
                    OffsetInfo.Render.YOffset);                         // Y Offset should be (CubeFaceIndex * QPitch) % TileHeight
                EXPECT_EQ(0, OffsetInfo.Render.ZOffset);                // Z offset N/A should be 0

                ULONG CubeFaceBaseOffset = ((6 * ArrayIndex) + CubeFaceIndex) * (ExpectedQPitch * ExpectedPitch);

                //Verify mip offsets in each cube face
                for (UINT Lod = 0; Lod <= MaxLod; Lod++)
                {
                    OffsetInfo.MipLevel = Lod;
                    ResourceInfo.GetOffset(OffsetInfo);

                    ULONG MipOffset = CubeFaceBaseOffset + RenderOffset[Lod].Offset;

                    ULONG OffsetX = MipOffset % ExpectedPitch;
                    ULONG TileAlignedOffsetX = GMM_ULT_ALIGN_FLOOR(OffsetX, TileSize[X]);
                    OffsetX -= TileAlignedOffsetX;

                    ULONG OffsetY = MipOffset / ExpectedPitch;
                    ULONG TileAlignedOffsetY = GMM_ULT_ALIGN_FLOOR(OffsetY, TileSize[Y]);
                    OffsetY -= TileAlignedOffsetY;

                    ULONG RenderAlignOffset =
                        TileAlignedOffsetY * ExpectedPitch +
                        (TileAlignedOffsetX / TileSize[X]) * PAGE_SIZE;


                    EXPECT_EQ(RenderAlignOffset, OffsetInfo.Render.Offset64);   // Render offset is tile's base address on which cube face begins.
                    EXPECT_EQ(OffsetX, OffsetInfo.Render.XOffset);
                    EXPECT_EQ(OffsetY, OffsetInfo.Render.YOffset);              // Y Offset should be (CubeFaceIndex * QPitch) % TileHeight
                    EXPECT_EQ(0, OffsetInfo.Render.ZOffset);                    // Z offset N/A should be 0

                }
            }
        }
    }
}
// ********************************************************************************//

/// @brief ULT for Buffer Resource
TEST_F(CTestResource, TestBufferLinearResource)
{

}

// ********************************************************************************//

/// @brief ULT for Separate Stencil Resource
TEST_F(CTestResource, TestSeparateStencil)
{
    const ULONG HAlign = 8;   //Separate Stencil alignment (8x8)
    const ULONG VAlign = 8;

    const ULONG StencilTileSize[1][2] = { 64, 64 };  //Stencil is TileW, swizzled (2*w x h/2) onto TileY
    const ULONG AllocTileSize[1][2] = {128, 32 };   //Allocation-aligned to TileY since some HW units support TileW by WA'ing TileY

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.TiledX = 1;         //Any input Tiling - changed to TileW? YES, but expected one is TileW
    gmmParams.Flags.Gpu.SeparateStencil = 1; 
    gmmParams.Format = SetResourceFormat(TEST_BPP_8); //Only R8_UNIT supported, driver assumes so, but creates resource even for other bpps requests (as bpp=8)

    // Allocate 1x1 surface so that it occupies 1 Tile in X dimension 
    for (UINT i = RESOURCE_1D; i <= RESOURCE_CUBE; i++) //SeparateStencil with 2D, cube, 1D, 3D with mip-maps 
    {
        gmmParams.Type = static_cast<GMM_RESOURCE_TYPE>(i);          
        gmmParams.BaseWidth64 = 0x1;
        gmmParams.BaseHeight = 0x1;
        gmmParams.Depth = 0x1;                     //Depth !=1 supported for stencil? Supported on all Gens. Gen8 gives only Lod0 for mip-mapped Stencil too

        GMM_RESOURCE_INFO ResourceInfo;
        EXPECT_EQ(GMM_SUCCESS, ResourceInfo.Create(*pGmmGlobalContext, gmmParams));

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        ULONG ExpectedPitch = StencilTileSize[0][0] * 2;        // 2 TileW tiles interleaved on same row
        VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch); 
        VerifyResourcePitchInTiles<true>(ResourceInfo, 1);      // 1 tileY wide

        if (gmmParams.ArraySize > 1 || gmmParams.Type == RESOURCE_CUBE)
        {
            ULONG ExpectedQPitch = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign); //Interleaved rows for TielW-arrangement. No Qpitch for 3d, only for 2d-array and cube on Gen8
            //Bspec wants it to be in VALign multiple, for Stencil buffer needs it as multiple of 8
            VerifyResourceQPitch<true>(ResourceInfo, ExpectedQPitch);        // Each face should be VAlign rows apart within a tile
        }

        VerifyResourceSize<true>(ResourceInfo, GMM_KBYTE(4));    //1 Tile should be enough

    }

    // Allocate 2 tiles in X dimension.
    for (UINT i = RESOURCE_1D; i <= RESOURCE_CUBE; i++)
    {
        gmmParams.Type = static_cast<GMM_RESOURCE_TYPE>(i);          //Could we loop over Res_types for speacil allocation types
        gmmParams.BaseWidth64 = StencilTileSize[0][0] + 0x1;
        gmmParams.BaseHeight = (gmmParams.Type == RESOURCE_1D) ? 0x1 :
                               (gmmParams.Type == RESOURCE_CUBE) ? gmmParams.BaseWidth64 :
                               StencilTileSize[0][1];
        gmmParams.Depth = 0x1;                     //HW doesn't support Depth/STC for 3D res_type, but driver has no such restriction

        GMM_RESOURCE_INFO ResourceInfo;
        EXPECT_EQ(GMM_SUCCESS, ResourceInfo.Create(*pGmmGlobalContext, gmmParams));

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        ULONG ExpectedPitch = StencilTileSize[0][0] * 2 * 2;     // Requires 2 StencilTiles, further doubled for interleaved rows 2*w = Pitch
        VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch);
        VerifyResourcePitchInTiles<true>(ResourceInfo, 2);      // 2 tiles wide

        ULONG ExpectedQPitch;
        if (gmmParams.ArraySize > 1 || gmmParams.Type == RESOURCE_CUBE)
        {
            ExpectedQPitch = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign); //Interleaved rows for TileW-arrangement - but Qpitch calculated w/o interleaving in mind. No Qpitch for 3d, only for 2d-array and cube on Gen8
            //GMM_ULT_ALIGN(GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign)/2, VAlign); //Doesn't HW expect distance in rows between 2 cube-faces (array slices) : It does so, but in logical view not physical view, so not interleaved rows.
            //Bspec wants it to be in VALign multiple, for Stencil buffer needs it as multiple of 8
            VerifyResourceQPitch<true>(ResourceInfo, ExpectedQPitch);        // Each face should be VAlign rows apart within a tile
        }

        VerifyResourceSize<true>(ResourceInfo,                          // PitchInBytes * Rows where Rows = (__GMM_MAX_CUBE_FACE x QPitch) /2 (Stencil height = halved due to interleaving), then aligned to tile boundary
            ((gmmParams.Type == RESOURCE_CUBE) ?
            ExpectedPitch * GMM_ULT_ALIGN(ExpectedQPitch * __GMM_MAX_CUBE_FACE / 2, AllocTileSize[0][1]) : //cube
            2 * GMM_KBYTE(4)));
    }

    // Allocate 2 tiles in X/Y dimension.
    for (UINT i = RESOURCE_2D; i <= RESOURCE_3D; i++)
    {
        gmmParams.Type = static_cast<GMM_RESOURCE_TYPE>(i);          
        gmmParams.BaseWidth64 = StencilTileSize[0][0] + 0x1;
        gmmParams.BaseHeight =  StencilTileSize[0][1] + 0x1;
        gmmParams.Depth = 0x1;                     

        GMM_RESOURCE_INFO ResourceInfo;
        EXPECT_EQ(GMM_SUCCESS, ResourceInfo.Create(*pGmmGlobalContext, gmmParams));

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        ULONG ExpectedPitch = StencilTileSize[0][0] * 2 * 2;     // Requires 2 StencilTiles, double again for interleaved rows 2*w = Pitch,
        VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch);
        VerifyResourcePitchInTiles<true>(ResourceInfo, 2);      // 2 tile wide

        VerifyResourceSize<true>(ResourceInfo, 2 * 2 * GMM_KBYTE(4));
    }

    // Allocate multi-tiles in X/Y/Z dimension.
    for (UINT i = RESOURCE_3D; i <= RESOURCE_3D; i++)
    {
        gmmParams.Type = static_cast<GMM_RESOURCE_TYPE>(i);          
        gmmParams.BaseWidth64 = StencilTileSize[0][0] + 0x1;
        gmmParams.BaseHeight = StencilTileSize[0][1] + 0x1;
        gmmParams.Depth = StencilTileSize[0][1] + 0x1;              

        GMM_RESOURCE_INFO ResourceInfo;
        EXPECT_EQ(GMM_SUCCESS, ResourceInfo.Create(*pGmmGlobalContext, gmmParams));

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        ULONG ExpectedPitch = StencilTileSize[0][0] * 2 * 2;     // Requires 2 StencilTiles, doubled again for interleaved rows 2*w = Pitch, width < 1.5 Tiles
        VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch);
        VerifyResourcePitchInTiles<true>(ResourceInfo, 2);      // 2 tile wide

        ULONG TwoDQPitch, ExpectedQPitch;
        {
            TwoDQPitch = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign); //Interleaved rows for TileW-arrangement - but Qpitch calculated w/o interleaving in mind. No Qpitch for 3d, only for 2d-array and cube on Gen8
            //GMM_ULT_ALIGN(GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign)/2, VAlign); //Doesn't HW expect distance in rows between 2 cube-faces (array slices) : It does so, but in logical view not physical view, so not interleaved rows.
            //Bspec wants it to be in VALign multiple, for Stencil buffer needs it as multiple of 8

            //VerifyResourceQPitch<false>(ResourceInfo, TwoDQPitch);       //Gen8 doesn't support QPitch for RES_3D

            ExpectedQPitch = gmmParams.Depth * TwoDQPitch;                //Depth slices arranged as 2D-arrayed slices. 
        }
        VerifyResourceSize<true>(ResourceInfo, 
                  ExpectedPitch * GMM_ULT_ALIGN(ExpectedQPitch/2, AllocTileSize[0][1]));
    }

    //Mip-mapped array case: <SNB>Stencil QPitch = h0 ie Stencil only has Lod0
                           //<BDW>Above limit not there, should have mip-map block height for Qpitch
}

/// @brief ULT for Hiz Depth buffer Resource
TEST_F(CTestResource, TestHiZ)
{
    const ULONG HAlign = 16;   //HiZ alignment (16x8) [Depth 16bit: 8x4; ow 4x4]
    const ULONG VAlign = 4;    // 8; Need to debug why driver uses VAlign/2

    //const ULONG DepthTileSize[1][2] = { 64, 64 };  //Depth/Stencil buffer should be TileY/Ys/Yf only (16,24,32 bpp only) no 3D or MSAA
    const ULONG AllocTileSize[1][2] = { 128, 32 };   //HiZ is TileY 

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Info.TiledX = 1;         //Not supported for Depth buffer, but HiZ output is TileY 
    gmmParams.Flags.Gpu.Depth = 1;           //GPU Flags= Depth/SeparateStencil + HiZ
    gmmParams.Flags.Gpu.HiZ = 1;

    // Allocate 1x1 surface so that it occupies 1 Tile in X dimension 
    for (UINT j = TEST_BPP_8; j <= TEST_BPP_128; j++) //Depth bpp doesn't matter, Depth px dimensions decide HiZ size in HW
    {
        gmmParams.Format = SetResourceFormat(static_cast<TEST_BPP>(j)); //Only 16,24,32 supported; But driver creates the resource even for other bpps without failing
        for (UINT i = RESOURCE_1D; i <= RESOURCE_CUBE; i++) //3D doesn't support HiZ - test driver returns proper?
        {
            gmmParams.Type = static_cast<GMM_RESOURCE_TYPE>(i); 
            gmmParams.BaseWidth64 = 0x1;
            gmmParams.BaseHeight = 0x1;
            gmmParams.Depth = 0x1;                     

            GMM_RESOURCE_INFO ResourceInfo;
            EXPECT_EQ(GMM_SUCCESS, ResourceInfo.Create(*pGmmGlobalContext, gmmParams));

            VerifyResourceHAlign<true>(ResourceInfo, HAlign);
            VerifyResourceVAlign<true>(ResourceInfo, VAlign);
            ULONG ExpectedPitch = AllocTileSize[0][0];        
            VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch);
            VerifyResourcePitchInTiles<true>(ResourceInfo, 1);      // 1 tileY wide

            if (gmmParams.ArraySize > 1 || gmmParams.Type == RESOURCE_CUBE)
            {
                ULONG ExpectedQPitch = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);  //Apply formula on Bspec
                ExpectedQPitch = GMM_ULT_ALIGN(ExpectedQPitch / 2, VAlign);

                VerifyResourceQPitch<false>(ResourceInfo, ExpectedQPitch);        // Each face should be VAlign rows apart within a tile, Turn on verification after clarity
            }

            VerifyResourceSize<true>(ResourceInfo, 
                                     GMM_KBYTE(4));    //1 Tile should be enough
        }
    }
    // Allocate 2 tiles in X dimension. (muti-tiles Tiles in Y dimension for cube/array)
    for (UINT i = RESOURCE_1D; i <= RESOURCE_CUBE; i++)
    {
        gmmParams.Type = static_cast<GMM_RESOURCE_TYPE>(i);          
        gmmParams.BaseWidth64 = AllocTileSize[0][0] + 0x1;           
        gmmParams.BaseHeight = (gmmParams.Type == RESOURCE_1D) ? 0x1 :
                               (gmmParams.Type == RESOURCE_CUBE) ? gmmParams.BaseWidth64 :
                                VAlign/2;
        gmmParams.ArraySize = (gmmParams.Type != RESOURCE_3D) ? VAlign : 1;            // Gen8 doesn't support 3D-arrays (so HiZ not supported) [test 3d arrays once -- HiZ would fail but ResCreate doesn't?]
        gmmParams.Depth = 0x1;                     

        GMM_RESOURCE_INFO ResourceInfo;
        EXPECT_EQ(GMM_SUCCESS, ResourceInfo.Create(*pGmmGlobalContext, gmmParams));

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        ULONG ExpectedPitch = AllocTileSize[0][0] * 2;     
        VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch);
        VerifyResourcePitchInTiles<true>(ResourceInfo, 2);      // 2 tileY wide

        ULONG ExpectedQPitch;
        if (gmmParams.ArraySize > 1 || gmmParams.Type == RESOURCE_CUBE)
        {
            ExpectedQPitch = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign); 
            ExpectedQPitch = GMM_ULT_ALIGN(ExpectedQPitch / 2, VAlign);

            VerifyResourceQPitch<false>(ResourceInfo, ExpectedQPitch);        // Each face should be VAlign rows apart within a tile. Turn on verification after clarity
        }

        VerifyResourceSize<true>(ResourceInfo,                          // PitchInBytes * Rows where Rows = (__GMM_MAX_CUBE_FACE x QPitch) /2 (Stencil height = halved due to interleaving), then aligned to tile boundary
            ((gmmParams.Type == RESOURCE_CUBE) ?
            ExpectedPitch * GMM_ULT_ALIGN(ExpectedQPitch * gmmParams.ArraySize *__GMM_MAX_CUBE_FACE, AllocTileSize[0][1]) : //cube
            ((gmmParams.ArraySize > 1) ?
            ExpectedPitch * GMM_ULT_ALIGN(ExpectedQPitch * gmmParams.ArraySize , AllocTileSize[0][1]) : //array
            2 * GMM_KBYTE(4))));
    }

    // Allocate 2 tiles in X/Y dimension (non-arrayed) Multi-tiles for 3D
    for (UINT i = RESOURCE_2D; i <= RESOURCE_3D; i++)
    {
        gmmParams.Type = static_cast<GMM_RESOURCE_TYPE>(i);          
        gmmParams.BaseWidth64 = AllocTileSize[0][0] + 0x1;
        gmmParams.BaseHeight = 2*AllocTileSize[0][1] + 0x1;          //Half-Depth Height or QPitch (lod!=0), aligned to 8 required by HW
        gmmParams.Depth = (gmmParams.Type == RESOURCE_2D) ? 0x1 :
                           VAlign + 1;                     
        gmmParams.ArraySize = 1;

        GMM_RESOURCE_INFO ResourceInfo;
        EXPECT_EQ(GMM_SUCCESS, ResourceInfo.Create(*pGmmGlobalContext, gmmParams));

        VerifyResourceHAlign<true>(ResourceInfo, HAlign);
        VerifyResourceVAlign<true>(ResourceInfo, VAlign);
        ULONG ExpectedPitch = AllocTileSize[0][0] * 2;     
        VerifyResourcePitch<true>(ResourceInfo, ExpectedPitch);
        VerifyResourcePitchInTiles<true>(ResourceInfo, 2);      // 2 tile wide

        ULONG TwoDQPitch, ExpectedQPitch;
        if (gmmParams.Type == RESOURCE_3D)
        {
            TwoDQPitch = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign); 
            ExpectedQPitch = gmmParams.Depth * GMM_ULT_ALIGN(TwoDQPitch / 2, VAlign);                //Depth slices arranged as 2D-arrayed slices. 
        }
        else 
        { 
        //HiZ for 3D not supported. Driver still allocates like IVB/HSW. (should Qpitch or only overall buffer height be Valigned ?)
        VerifyResourceSize<true>(ResourceInfo,
            ((gmmParams.Type == RESOURCE_3D) ?
            ExpectedPitch * GMM_ULT_ALIGN(ExpectedQPitch , AllocTileSize[0][1]) :          
            2 * 2 * GMM_KBYTE(4)));
        }
    }

    //Mip-mapped case: 
}

/// @brief ULT for MSAA Resource
TEST_F(CTestResource, TestMSAA)
{
    enum MSAA_Samples { MSAA_None, MSAA_2x, MSAA_4x, MSAA_8x, MSAA_16x};

    const ULONG MCSTileSize[1][2] = { 128, 32 };   //MCS is TileY 

    //Gen8:No mip-map for MSAA (MSS and MCS), No 16x
    //No MSAA for YUV/compressed formats
    //Interleaved MSS (IMS) for Depth/Stencil. Arrayed MSS (CMS) for Color RT
    //QPitch exists for arrayed MSS - gives distance between slices
    //MSS (Arrayed): px_wL, px_hL = pixel width/height of single sample at Lod L
    //                 MSS width = px_wL, MSS height = NumSamples*px_hL
    //MSS (Interleaved): px_wL, px_hL = pixel width/height of single sample at Lod L
    // Samples         MSS width                MSS Height
    //   2x            4*ceil(px_wL/2)             px_hL
    //   4x            4*ceil(px_wL/2)           4*ceil(px_hL/2)
    //   8x            8*ceil(px_wL/2)           4*ceil(px_hL/2)
    //  16x            8*ceil(px_wL/2)           8*ceil(px_hL/2)
    //MCS (bpp): 2x/4x - bpp_8, 8x - bpp_32, 16x - bpp_64

    const UINT TestDimensions[3][2] = {
        { 0, 0 },                 //1x1x1
        { 1, 0 },                 //2 Tilesx1
        { 1, 1 },                 //2 Tilesx 2
    };

    UINT TestArraySize[2] = { 1, 7 };

    ULONG HAlign, VAlign, TileDimX, TileDimY, MCSHAlign, MCSVAlign, TileSize;
    UINT ExpectedMCSBpp;

    std::vector<std::tuple<int, int, int, bool, int, int>> List; //TEST_TILE_TYPE, TEST_BPP, TEST_RESOURCE_TYPE, Depth or RT, TestDimension index, TestArraySize index
    auto Size = BuildInputIterator(List, 3, 2); // Size of arrays TestDimensions, TestArraySize

    for (auto element : List)
    {
        GMM_RESCREATE_PARAMS gmmParams = {};
        gmmParams.Flags.Info = { 0 };

        TEST_TILE_TYPE Tiling = (TEST_TILE_TYPE)std::get<0>(element);
        TEST_BPP Bpp = (TEST_BPP)std::get<1>(element);
        TEST_RESOURCE_TYPE ResType = (TEST_RESOURCE_TYPE)std::get<2>(element);
        bool IsRT = std::get<3>(element);       // True for RT, False for Depth
        int TestDimIdx = std::get<4>(element);  //index into TestDimensions array
        int ArrayIdx = std::get<5>(element);   //index into TestArraySize 
        TileSize =  GMM_KBYTE(4);

        //Discard un-supported Tiling/Res_type/bpp for this test
        if (ResType != TEST_RESOURCE_2D || Tiling > TEST_TILEY           //No 1D/3D/Cube. Supported 2D mip-maps/array 
            || (!IsRT && (Tiling == TEST_TILEX ||                        //Bspec doesn't support TileX for Depth
                !(Bpp == TEST_BPP_16 || Bpp == TEST_BPP_32)))) //depth supported on 16bit, 32bit formats only
            continue;

        SetTileFlag(gmmParams, Tiling);
        SetResType(gmmParams, ResType);
        SetResGpuFlags(gmmParams, IsRT);
        SetResArraySize(gmmParams, TestArraySize[ArrayIdx]);

        gmmParams.NoGfxMemory = 1;
        gmmParams.Format = SetResourceFormat(Bpp);
        for (UINT k = MSAA_2x; k < MSAA_16x; k++)  //No 16x MSAA on Gen8
        {
            GetAlignmentAndTileDimensionsForMSAA(Bpp, IsRT, Tiling, (TEST_MSAA)k, 
                                    TileDimX, TileDimY, HAlign, VAlign,
                                    ExpectedMCSBpp, MCSHAlign, MCSVAlign);

            gmmParams.BaseWidth64 = TestDimensions[TestDimIdx][0] * TileDimX + 0x1;
            gmmParams.BaseHeight = TestDimensions[TestDimIdx][1] * TileDimY + 0x1;

            gmmParams.Depth = 0x1;                                  //2D Res_type with Depth > 1 doesn't fail, although Tex_cacl doesn't use it
            gmmParams.MSAA.NumSamples = static_cast<ULONG>(pow((double)2, k));    
            gmmParams.Flags.Gpu.MCS = 0;
            gmmParams.ArraySize = 1;

            //MSS surface
            GMM_RESOURCE_INFO MSSResourceInfo;
            GMM_STATUS Ret = MSSResourceInfo.Create(*pGmmGlobalContext, gmmParams);
            EXPECT_EQ(GMM_SUCCESS, Ret);

            if (Ret == GMM_SUCCESS)
            {
                VerifyResourceHAlign<true>(MSSResourceInfo, HAlign);
                VerifyResourceVAlign<true>(MSSResourceInfo, VAlign);
                if (IsRT) //Arrayed MSS
                {
                    ULONG ExpectedPitch = 0, ExpectedQPitch = 0;
                    ExpectedPitch = GMM_ULT_ALIGN(GMM_ULT_ALIGN(gmmParams.BaseWidth64, HAlign) * (UINT)pow(2.0, Bpp), TileDimX);        // Aligned width * bpp, aligned to TileWidth
                    VerifyResourcePitch<true>(MSSResourceInfo, ExpectedPitch);
                    if (Tiling != TEST_LINEAR)
                        VerifyResourcePitchInTiles<true>(MSSResourceInfo, ExpectedPitch / TileDimX);

                    //if (gmmParams.ArraySize > 1) - Arrayed MSS has QPitch
                    {
                        ExpectedQPitch = GMM_ULT_ALIGN(gmmParams.BaseHeight, VAlign);
                        VerifyResourceQPitch<true>(MSSResourceInfo, ExpectedQPitch);
                    }

                    ULONG ExpectedHeight = GMM_ULT_ALIGN(ExpectedQPitch*gmmParams.MSAA.NumSamples*gmmParams.ArraySize, TileDimY); //Align Height =ExpectedPitch * NumSamples * ExpectedQPitch, to Tile-Height 
                    VerifyResourceSize<true>(MSSResourceInfo, GMM_ULT_ALIGN(ExpectedPitch*ExpectedHeight, TileSize));
                }
                else // Interleaved MSS
                {
                    UINT WidthMultiplier, HeightMultiplier;
                    GetInterleaveMSSPattern((TEST_MSAA)k, WidthMultiplier, HeightMultiplier);
                    gmmParams.BaseWidth64 = WidthMultiplier > 1 ? GMM_ULT_ALIGN(gmmParams.BaseWidth64, 2) : gmmParams.BaseWidth64;
                    gmmParams.BaseHeight = HeightMultiplier > 1 ? GMM_ULT_ALIGN(gmmParams.BaseHeight, 2) : gmmParams.BaseHeight;

                    ULONG ExpectedPitch = GMM_ULT_ALIGN(GMM_ULT_ALIGN(gmmParams.BaseWidth64* WidthMultiplier, HAlign) * (UINT)pow(2.0, Bpp), TileDimX);        //AlignedWidth*bpp, then align to Tile
                    VerifyResourcePitch<true>(MSSResourceInfo, ExpectedPitch);
                    if (Tiling != TEST_LINEAR)
                    {
                        VerifyResourcePitchInTiles<true>(MSSResourceInfo, ExpectedPitch / TileDimX);      // 1 tileY wide
                    }

                    ULONG ExpectedQPitch = GMM_ULT_ALIGN(gmmParams.BaseHeight * HeightMultiplier, VAlign); 
                    if (gmmParams.ArraySize > 1)
                    {
                        //Bspec wants it to be in VALign multiple
                        VerifyResourceQPitch<true>(MSSResourceInfo, ExpectedQPitch);        
                    }
                    ULONG ExpectedHeight = GMM_ULT_ALIGN(ExpectedQPitch*gmmParams.ArraySize, TileDimY); //Align Height = ExpectedQPitch*ArraySize, to Tile-Height 
                    VerifyResourceSize<true>(MSSResourceInfo, GMM_ULT_ALIGN(ExpectedPitch*ExpectedHeight, TileSize));    //ExpectedPitch *ExpectedHeight
                }
            }

            //No MCS surface if MSS creation failed
            if (Ret == GMM_SUCCESS)
            {
                gmmParams.Flags.Gpu.MCS = 1;
                GMM_RESOURCE_INFO MCSResourceInfo;
                EXPECT_EQ(GMM_SUCCESS, MCSResourceInfo.Create(*pGmmGlobalContext, gmmParams));

                VerifyResourceHAlign<true>(MCSResourceInfo, MCSHAlign); // MCS alignment same as basic RT alignment
                VerifyResourceVAlign<true>(MCSResourceInfo, MCSVAlign); 

                ULONG ExpectedPitch = GMM_ULT_ALIGN(GMM_ULT_ALIGN(gmmParams.BaseWidth64, MCSHAlign) * ExpectedMCSBpp, MCSTileSize[0][0]);        //AlignedWidth*bpp, then tile-alignment
                VerifyResourcePitch<true>(MCSResourceInfo, ExpectedPitch);
                VerifyResourcePitchInTiles<true>(MCSResourceInfo, ExpectedPitch / MCSTileSize[0][0]);      

                ULONG ExpectedQPitch = gmmParams.BaseHeight;
                if (gmmParams.ArraySize > 1)
                {
                    ExpectedQPitch = GMM_ULT_ALIGN(gmmParams.BaseHeight, MCSVAlign); //QPitch only for array
                    VerifyResourceQPitch<true>(MCSResourceInfo, ExpectedQPitch);
                }

                ULONG ExpectedHeight = GMM_ULT_ALIGN(ExpectedQPitch*gmmParams.ArraySize, MCSTileSize[0][1]);
                VerifyResourceSize<true>(MCSResourceInfo, GMM_ULT_ALIGN(ExpectedPitch*ExpectedHeight, TileSize));    
            }//MCS
        }//NumSamples = k
    }//Iterate through all input tuples
}

/// @brief ULT for Plannar 2D Resource - RGBP
TEST_F(CTestResource, TestPlanar2D_RGBP)
{
    /* Test planar surfaces where all planes are full-sized */
    // YYYYYYYY
    // YYYYYYYY
    // YYYYYYYY
    // YYYYYYYY
    // UUUUUUUU
    // UUUUUUUU
    // UUUUUUUU
    // UUUUUUUU
    // VVVVVVVV
    // VVVVVVVV
    // VVVVVVVV
    // VVVVVVVV
    const TEST_TILE_TYPE TileTypes[] = { TEST_LINEAR, TEST_TILEX, TEST_TILEY };

    const ULONG TileSize[3][2] = { { 1, 1 }, //Linear
                                   { 512, 8 },  // TileX
                                   { 128, 32 } }; // TileY
    for (ULONG TileIndex = 0; TileIndex < sizeof(TileTypes) / sizeof(TileTypes[0]); TileIndex++)
    {
        TEST_TILE_TYPE Tile = TileTypes[TileIndex];

        GMM_RESCREATE_PARAMS gmmParams = {};
        gmmParams.Type = RESOURCE_2D;
        gmmParams.NoGfxMemory = 1;
        gmmParams.Flags.Gpu.Texture = 1;
        gmmParams.BaseWidth64 = 0x100;
        gmmParams.BaseHeight = 0x100;
        gmmParams.Depth = 0x1;
        SetTileFlag(gmmParams, static_cast<TEST_TILE_TYPE>(Tile));
        gmmParams.Flags.Info.Linear = 1; // GmmLib needs linear to be set as fallback for all planar surfaces
        gmmParams.Format = GMM_FORMAT_RGBP;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        ULONG Pitch = GMM_ULT_ALIGN(gmmParams.BaseWidth64, TileSize[TileIndex][0]);
        ULONG Height = GMM_ULT_ALIGN(gmmParams.BaseHeight * 3 /*Y, U, V*/, TileSize[TileIndex][1]);
        ULONG Size = GMM_ULT_ALIGN(Pitch * Height, GMM_KBYTE(4));

        VerifyResourcePitch<true>(ResourceInfo, Pitch);
        if (Tile != TEST_LINEAR)
        {
            VerifyResourcePitchInTiles<true>(ResourceInfo, Pitch / TileSize[TileIndex][0]);
        }
        VerifyResourceSize<true>(ResourceInfo, Size);
        VerifyResourceHAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere
        VerifyResourceVAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere 
        VerifyResourceQPitch<false>(ResourceInfo, 0); // N/A for planar

        // Y plane should be at 0,0
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_Y));
        EXPECT_EQ(0, ResourceInfo.GetPlanarYOffset(GMM_PLANE_Y));

        // U plane should be at end of Y plane
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_U));
        EXPECT_EQ(gmmParams.BaseHeight, ResourceInfo.GetPlanarYOffset(GMM_PLANE_U));

        // V plane should be at end of U plane
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_V));
        EXPECT_EQ(gmmParams.BaseHeight*2, ResourceInfo.GetPlanarYOffset(GMM_PLANE_V));
    }
}

/// @brief ULT for Plannar 2D Resource - MFX_JPEG_YUV422V
TEST_F(CTestResource, TestPlanar2D_MFX_JPEG_YUV422V)
{
    /* Test planar surfaces where both U and V are half the size of Y */
    // YYYYYYYY
    // YYYYYYYY
    // YYYYYYYY
    // YYYYYYYY
    // UUUUUUUU
    // UUUUUUUU
    // VVVVVVVV
    // VVVVVVVV
    const TEST_TILE_TYPE TileTypes[] = { TEST_LINEAR, TEST_TILEX, TEST_TILEY };

    const ULONG TileSize[3][2] = { { 1, 1 }, //Linear
                                   { 512, 8 },  // TileX
                                   { 128, 32 } }; // TileY
    for (ULONG TileIndex = 0; TileIndex < sizeof(TileTypes) / sizeof(TileTypes[0]); TileIndex++)
    {
        TEST_TILE_TYPE Tile = TileTypes[TileIndex];

        GMM_RESCREATE_PARAMS gmmParams = {};
        gmmParams.Type = RESOURCE_2D;
        gmmParams.NoGfxMemory = 1;
        gmmParams.Flags.Gpu.Texture = 1;
        gmmParams.BaseWidth64 = 0x100;
        gmmParams.BaseHeight = 0x100;
        gmmParams.Depth = 0x1;
        SetTileFlag(gmmParams, static_cast<TEST_TILE_TYPE>(Tile));
        gmmParams.Flags.Info.Linear = 1; // GmmLib needs linear to be set as fallback for all planar surfaces
        gmmParams.Format = GMM_FORMAT_MFX_JPEG_YUV422V;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        ULONG Pitch = GMM_ULT_ALIGN(gmmParams.BaseWidth64, TileSize[TileIndex][0]);
        ULONG Height = GMM_ULT_ALIGN(gmmParams.BaseHeight /*Y */ + gmmParams.BaseHeight /*U, V*/, TileSize[TileIndex][1]);
        ULONG Size = GMM_ULT_ALIGN(Pitch * Height, GMM_KBYTE(4));

        VerifyResourcePitch<true>(ResourceInfo, Pitch);
        if (Tile != TEST_LINEAR)
        {
            VerifyResourcePitchInTiles<true>(ResourceInfo, Pitch / TileSize[TileIndex][0]);
        }
        VerifyResourceSize<true>(ResourceInfo, Size);
        VerifyResourceHAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere
        VerifyResourceVAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere 
        VerifyResourceQPitch<false>(ResourceInfo, 0); // N/A for planar

        // Y plane should be at 0,0
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_Y));
        EXPECT_EQ(0, ResourceInfo.GetPlanarYOffset(GMM_PLANE_Y));

        // U plane should be at end of Y plane
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_U));
        EXPECT_EQ(gmmParams.BaseHeight, ResourceInfo.GetPlanarYOffset(GMM_PLANE_U));

        // V plane should be at end of U plane
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_V));
        EXPECT_EQ(gmmParams.BaseHeight + gmmParams.BaseHeight/2, ResourceInfo.GetPlanarYOffset(GMM_PLANE_V));
    }
}

/// @brief ULT for Plannar 2D Resource - MFX_JPEG_YUV411R
TEST_F(CTestResource, TestPlanar2D_MFX_JPEG_YUV411R)
{
    /* Test planar surfaces where both U and V are quarter the size of Y */
    //YYYYYYYY
    //YYYYYYYY
    //YYYYYYYY
    //YYYYYYYY
    //UUUUUUUU
    //VVVVVVVV
    const TEST_TILE_TYPE TileTypes[] = { TEST_LINEAR, TEST_TILEX, TEST_TILEY };

    const ULONG TileSize[3][2] = { { 1, 1 }, //Linear
                                   { 512, 8 },  // TileX
                                   { 128, 32 } }; // TileY
    for (ULONG TileIndex = 0; TileIndex < sizeof(TileTypes) / sizeof(TileTypes[0]); TileIndex++)
    {
        TEST_TILE_TYPE Tile = TileTypes[TileIndex];

        GMM_RESCREATE_PARAMS gmmParams = {};
        gmmParams.Type = RESOURCE_2D;
        gmmParams.NoGfxMemory = 1;
        gmmParams.Flags.Gpu.Texture = 1;
        gmmParams.BaseWidth64 = 0x100;
        gmmParams.BaseHeight = 0x100;
        gmmParams.Depth = 0x1;
        SetTileFlag(gmmParams, static_cast<TEST_TILE_TYPE>(Tile));
        gmmParams.Flags.Info.Linear = 1; // GmmLib needs linear to be set as fallback for all planar surfaces
        gmmParams.Format = GMM_FORMAT_MFX_JPEG_YUV411R_TYPE;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        ULONG Pitch = GMM_ULT_ALIGN(gmmParams.BaseWidth64, TileSize[TileIndex][0]);
        ULONG Height = GMM_ULT_ALIGN(gmmParams.BaseHeight /*Y */ + gmmParams.BaseHeight/2 /*U, V*/, TileSize[TileIndex][1]);
        ULONG Size = GMM_ULT_ALIGN(Pitch * Height, GMM_KBYTE(4));

        VerifyResourcePitch<true>(ResourceInfo, Pitch);
        if (Tile != TEST_LINEAR)
        {
            VerifyResourcePitchInTiles<true>(ResourceInfo, Pitch / TileSize[TileIndex][0]);
        }
        VerifyResourceSize<true>(ResourceInfo, Size);
        VerifyResourceHAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere
        VerifyResourceVAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere 
        VerifyResourceQPitch<false>(ResourceInfo, 0); // N/A for planar

        // Y plane should be at 0,0
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_Y));
        EXPECT_EQ(0, ResourceInfo.GetPlanarYOffset(GMM_PLANE_Y));

        // U plane should be at end of Y plane
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_U));
        EXPECT_EQ(gmmParams.BaseHeight, ResourceInfo.GetPlanarYOffset(GMM_PLANE_U));

        // V plane should be at end of U plane
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_V));
        EXPECT_EQ(gmmParams.BaseHeight + gmmParams.BaseHeight / 4, ResourceInfo.GetPlanarYOffset(GMM_PLANE_V));
    }
}

/// @brief ULT for Plannar 2D Resource - NV12
TEST_F(CTestResource, TestPlanar2D_NV12)
{
    /* Test planar surface with hybrid UV planes where UV plane is half the size 
       of Y and U/V data is packed together */
    // YYYYYYYY
    // YYYYYYYY
    // YYYYYYYY
    // YYYYYYYY
    // [UV-Packing]
    const TEST_TILE_TYPE TileTypes[] = { TEST_LINEAR, TEST_TILEX, TEST_TILEY };

    const ULONG TileSize[3][2] = { {1, 1}, //Linear
                                   { 512, 8 },  // TileX
                                   { 128, 32 } }; // TileY
    for (ULONG TileIndex = 0; TileIndex < sizeof(TileTypes)/sizeof(TileTypes[0]); TileIndex++)
    {
        TEST_TILE_TYPE Tile = TileTypes[TileIndex];

        GMM_RESCREATE_PARAMS gmmParams = {};
        gmmParams.Type = RESOURCE_2D;
        gmmParams.NoGfxMemory = 1;
        gmmParams.Flags.Gpu.Texture = 1;
        gmmParams.BaseWidth64 = 0x100;
        gmmParams.BaseHeight = 0x100;
        gmmParams.Depth = 0x1;
        SetTileFlag(gmmParams, static_cast<TEST_TILE_TYPE>(Tile));
        gmmParams.Flags.Info.Linear = 1; // GmmLib needs linear to be set as fallback for all tiled planar surfaces
        gmmParams.Format = GMM_FORMAT_NV12;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        ULONG Pitch = GMM_ULT_ALIGN(gmmParams.BaseWidth64, TileSize[TileIndex][0]);
        ULONG Height = GMM_ULT_ALIGN(gmmParams.BaseHeight /*Y*/ + gmmParams.BaseHeight / 2 /*UV*/, TileSize[TileIndex][1]);
        ULONG Size = GMM_ULT_ALIGN(Pitch * Height, GMM_KBYTE(4));

        VerifyResourcePitch<true>(ResourceInfo, Pitch);
        if (Tile != TEST_LINEAR)
        {
            VerifyResourcePitchInTiles<true>(ResourceInfo, Pitch / TileSize[TileIndex][0]);
        }
        VerifyResourceSize<true>(ResourceInfo, Size);
        VerifyResourceHAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere
        VerifyResourceVAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere 
        VerifyResourceQPitch<false>(ResourceInfo, 0); // N/A for planar

        // Y plane should be at 0,0
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_Y));
        EXPECT_EQ(0, ResourceInfo.GetPlanarYOffset(GMM_PLANE_Y));

        // U/V plane should be at end of Y plane
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_U));
        EXPECT_EQ(gmmParams.BaseHeight, ResourceInfo.GetPlanarYOffset(GMM_PLANE_U));
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_V));
        EXPECT_EQ(gmmParams.BaseHeight, ResourceInfo.GetPlanarYOffset(GMM_PLANE_V));
    }
}

/// @brief ULT for Planar 2D Resource - IMC4
TEST_F(CTestResource, TestPlanar2D_IMC4)
{
    /* Test planar surface V surface is on the right of U */
    // YYYYYYYY
    // YYYYYYYY
    // YYYYYYYY
    // YYYYYYYY
    // UUUUVVVV
    // UUUUVVVV
    const TEST_TILE_TYPE TileTypes[] = { TEST_LINEAR, TEST_TILEX, TEST_TILEY };

    const ULONG TileSize[3][2] = { { 1, 1 }, //Linear
                                   { 512, 8 },  // TileX
                                   { 128, 32 } }; // TileY
    for (ULONG TileIndex = 0; TileIndex < sizeof(TileTypes) / sizeof(TileTypes[0]); TileIndex++)
    {
        TEST_TILE_TYPE Tile = TileTypes[TileIndex];

        GMM_RESCREATE_PARAMS gmmParams = {};
        gmmParams.Type = RESOURCE_2D;
        gmmParams.NoGfxMemory = 1;
        gmmParams.Flags.Gpu.Texture = 1;
        gmmParams.BaseWidth64 = 0x100;
        gmmParams.BaseHeight = 0x100;
        gmmParams.Depth = 0x1;
        SetTileFlag(gmmParams, static_cast<TEST_TILE_TYPE>(Tile));
        gmmParams.Flags.Info.Linear = 1; // GmmLib needs linear to be set as fallback for all planar surfaces
        gmmParams.Format = GMM_FORMAT_IMC4;

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        ULONG Pitch = GMM_ULT_ALIGN(gmmParams.BaseWidth64, TileSize[TileIndex][0]);
        ULONG Height = GMM_ULT_ALIGN(gmmParams.BaseHeight /*Y*/ + gmmParams.BaseHeight / 2 /*UV*/, TileSize[TileIndex][1]);
        ULONG Size = GMM_ULT_ALIGN(Pitch * Height, GMM_KBYTE(4));

        VerifyResourcePitch<true>(ResourceInfo, Pitch);
        if (Tile != TEST_LINEAR)
        {
            VerifyResourcePitchInTiles<true>(ResourceInfo, Pitch / TileSize[TileIndex][0]);
        }
        VerifyResourceSize<true>(ResourceInfo, Size);
        VerifyResourceHAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere
        VerifyResourceVAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere 
        VerifyResourceQPitch<false>(ResourceInfo, 0); // N/A for planar

        // Y plane should be at 0,0
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_Y));
        EXPECT_EQ(0, ResourceInfo.GetPlanarYOffset(GMM_PLANE_Y));

        // U plane should be at end of Y plane
        EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_U));
        EXPECT_EQ(gmmParams.BaseHeight, ResourceInfo.GetPlanarYOffset(GMM_PLANE_U));

        // V plane should be at end of U plane
        EXPECT_EQ(Pitch / 2, ResourceInfo.GetPlanarXOffset(GMM_PLANE_V));
        EXPECT_EQ(gmmParams.BaseHeight, ResourceInfo.GetPlanarYOffset(GMM_PLANE_V));
    }
}

/// @brief ULT for Planar 2D Resource - YV12
TEST_F(CTestResource, TestPlanar2D_YV12)
{
    /* Test planar surface V surface follows U surface linearly */
    // YYYYYYYY
    // YYYYYYYY
    // YYYYYYYY
    // YYYYYYYY
    // VVVVVV..  <-- V and U planes follow the Y plane, as linear 
    // ..UUUUUU      arrays--without respect to pitch.
    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_2D;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Gpu.Texture = 1;
    gmmParams.BaseWidth64 = 0x100;
    gmmParams.BaseHeight = 0x100;
    gmmParams.Depth = 0x1;
    gmmParams.Flags.Info.Linear = 1; // Linear only since UV plane doesn't have a pitch
    gmmParams.Format = GMM_FORMAT_YV12;

    GMM_RESOURCE_INFO ResourceInfo;
    ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

    ULONG Pitch = gmmParams.BaseWidth64;
    ULONG SizeOfY = Pitch * gmmParams.BaseHeight;
    ULONG Height = (SizeOfY/*Y*/ + SizeOfY/4 /*V*/ + SizeOfY/4 /*U*/) / Pitch;
    ULONG Size = GMM_ULT_ALIGN(Pitch * Height, GMM_KBYTE(4));

    VerifyResourcePitch<true>(ResourceInfo, Pitch);
    VerifyResourcePitchInTiles<false>(ResourceInfo, 0); // N/A for linear
    VerifyResourceSize<true>(ResourceInfo, Size);
    VerifyResourceHAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere
    VerifyResourceVAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere 
    VerifyResourceQPitch<false>(ResourceInfo, 0); // N/A for planar

    // Y plane should be at 0,0
    EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_Y));
    EXPECT_EQ(0, ResourceInfo.GetPlanarYOffset(GMM_PLANE_Y));

    // V plane follows Y
    EXPECT_EQ(0, ResourceInfo.GetPlanarXOffset(GMM_PLANE_V));
    EXPECT_EQ(gmmParams.BaseHeight, ResourceInfo.GetPlanarYOffset(GMM_PLANE_V));

    // U plane should be at end of V plane
    ULONG UByteOffset = SizeOfY /* Y */ +
                        SizeOfY / 4; /* Size of V = 1/4 of Y */
    EXPECT_EQ(UByteOffset % Pitch, ResourceInfo.GetPlanarXOffset(GMM_PLANE_U));
    EXPECT_EQ(UByteOffset / Pitch, ResourceInfo.GetPlanarYOffset(GMM_PLANE_U));
}

/// @brief ULT for Unified aux surface
TEST_F(CTestResource, TestUnifiedAuxSurface)
{
    ULONG TileSize[] = { 128, 32 };

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_2D;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Gpu.Texture = 1;
    gmmParams.Flags.Gpu.CCS = 1;
    gmmParams.Flags.Gpu.UnifiedAuxSurface = 1;
    gmmParams.BaseWidth64 = 0x100;
    gmmParams.BaseHeight = 0x100;
    gmmParams.Depth = 0x1;
    SetTileFlag(gmmParams, TEST_TILEY); 

    for (UINT bpp = TEST_BPP_32; bpp <= TEST_BPP_128; bpp++) // 32/64/128 only
    {
        gmmParams.Format = SetResourceFormat(static_cast<TEST_BPP>(bpp));

        GMM_RESOURCE_INFO ResourceInfo;
        ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

        ULONG64 Pitch = GMM_ULT_ALIGN(gmmParams.BaseWidth64 * GetBppValue(static_cast<TEST_BPP>(bpp)), TileSize[0]);
        ULONG64 Size = Pitch * GMM_ULT_ALIGN(gmmParams.BaseHeight, TileSize[1]);

        Size = GMM_ULT_ALIGN(Size, PAGE_SIZE);

        VerifyResourcePitch<true>(ResourceInfo, Pitch);
        VerifyResourcePitchInTiles<false>(ResourceInfo, Pitch/TileSize[0]);
        VerifyResourceSize<true>(ResourceInfo, Size);
        VerifyResourceHAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere
        VerifyResourceVAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere 
        VerifyResourceQPitch<false>(ResourceInfo, 0); // N/A for non-arrayed

        ULONG AuxPitch = Pitch;
        switch (bpp)
        {
            case TEST_BPP_32:  AuxPitch /= 8; break;
            case TEST_BPP_64:  AuxPitch /= 4; break;
            case TEST_BPP_128: AuxPitch /= 2; break;
        }
        ULONG64 AuxSize = AuxPitch * GMM_ULT_ALIGN(gmmParams.BaseHeight, TileSize[1]);
        AuxSize = GMM_ULT_ALIGN(AuxSize, PAGE_SIZE);

        // Verify unified aux info
        EXPECT_EQ(AuxSize, ResourceInfo.GetSizeAuxSurface(GMM_AUX_CCS));

        if (ResourceInfo.Is64KBPageSuitable())
        {
            EXPECT_EQ(GMM_ULT_ALIGN(Size, 64 * PAGE_SIZE) + AuxSize, ResourceInfo.GetSizeSurface());
        }
        else
        {
            EXPECT_EQ(Size + AuxSize, ResourceInfo.GetSizeSurface());
        }
        
        EXPECT_EQ(256, ResourceInfo.GetAuxHAlign());
        EXPECT_EQ(128, ResourceInfo.GetAuxVAlign());
        EXPECT_EQ(0, ResourceInfo.GetAuxQPitch());
        EXPECT_EQ(AuxPitch, ResourceInfo.GetUnifiedAuxPitch());
        EXPECT_EQ(AuxPitch / TileSize[0], ResourceInfo.GetRenderAuxPitchTiles());
        EXPECT_EQ(Size, ResourceInfo.GetUnifiedAuxSurfaceOffset(GMM_AUX_CCS));

    }

}

/// @brief ULT for Compressed Resource
TEST_F(CTestResource, TestCompressedSurface)
{
    ULONG TileSize[] = { 128, 32 };
    ULONG CompBlock[] = { 4, 4 };

    GMM_RESCREATE_PARAMS gmmParams = {};
    gmmParams.Type = RESOURCE_2D;
    gmmParams.NoGfxMemory = 1;
    gmmParams.Flags.Gpu.Texture = 1;
    gmmParams.BaseWidth64 = 0x100;
    gmmParams.BaseHeight = 0x100;
    gmmParams.Depth = 0x1;
    SetTileFlag(gmmParams, TEST_TILEY);
    //TODO: Programatically test all compression formats. 
    gmmParams.Format = GMM_FORMAT_BC1_UNORM; // BC1 is 4x4 compression block, 64bpe

    GMM_RESOURCE_INFO ResourceInfo;
    ResourceInfo.Create(*pGmmGlobalContext, gmmParams);

    ULONG64 Pitch = GMM_ULT_ALIGN(gmmParams.BaseWidth64 / CompBlock[0] * GetBppValue(TEST_BPP_64), TileSize[0]);
    ULONG64 Size = Pitch * GMM_ULT_ALIGN(gmmParams.BaseHeight / CompBlock[1], TileSize[1]);
    Size = GMM_ULT_ALIGN(Size, PAGE_SIZE);

    VerifyResourcePitch<true>(ResourceInfo, Pitch);
    VerifyResourcePitchInTiles<false>(ResourceInfo, Pitch / TileSize[0]);
    VerifyResourceSize<true>(ResourceInfo, Size);
    VerifyResourceHAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere
    VerifyResourceVAlign<false>(ResourceInfo, 0); // Same as any other 2D surface -- tested elsewhere 
    VerifyResourceQPitch<false>(ResourceInfo, 0); // N/A for non-arrayed
}


