//===========================================================================//
// Purpose : Declaration and inline definitions for the universal 
//           TFH_FabricBlockHandler singleton class. This class supports
//           grid block overrides for the VPR placement tool
//
//===========================================================================//

//---------------------------------------------------------------------------//
// Copyright (C) 2013 Jeff Rudolph, Texas Instruments (jrudolph@ti.com)      //
//                                                                           //
// This program is free software; you can redistribute it and/or modify it   //
// under the terms of the GNU General Public License as published by the     //
// Free Software Foundation; version 3 of the License, or any later version. //
//                                                                           //
// This program is distributed in the hope that it will be useful, but       //
// WITHOUT ANY WARRANTY; without even an implied warranty of MERCHANTABILITY //
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License   //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU General Public License along   //
// with this program; if not, see <http://www.gnu.org/licenses>.             //
//---------------------------------------------------------------------------//

#ifndef TFH_FABRIC_BLOCK_HANDLER_H
#define TFH_FABRIC_BLOCK_HANDLER_H

#include <cstdio>
using namespace std;

#include "TFH_Typedefs.h"
#include "TFH_GridBlock.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
class TFH_FabricBlockHandler_c
{
public:

   static void NewInstance( void );
   static void DeleteInstance( void );
   static TFH_FabricBlockHandler_c& GetInstance( bool newInstance = true );
   static bool HasInstance( void );

   size_t GetLength( void ) const;
   const TFH_GridBlockList_t& GetGridBlockList( void ) const;

   void Clear( void );

   void Add( const TGO_Point_c& vpr_gridPoint,
             int vpr_typeIndex,
             TFH_BlockType_t blockType,
             const string& srBlockName,
             const string& srMasterName );
   void Add( const TGO_Point_c& vpr_gridPoint,
             int vpr_typeIndex,
             TFH_BlockType_t blockType,
             const char* pszBlockName,
             const char* pszMasterName );
   void Add( const TFH_GridBlock_c& gridBlock );

   bool IsMember( const TGO_Point_c& vpr_gridPoint ) const;
   bool IsMember( const TFH_GridBlock_c& gridBlock ) const;

   bool IsValid( void ) const;

protected:

   TFH_FabricBlockHandler_c( void );
   ~TFH_FabricBlockHandler_c( void );

private:

   TFH_GridBlockList_t gridBlockList_;

   static TFH_FabricBlockHandler_c* pinstance_; // Define ptr for singleton instance

private:

   enum TFH_DefCapacity_e 
   { 
      TFH_GRID_BLOCK_LIST_DEF_CAPACITY = 64
   };
};

#endif