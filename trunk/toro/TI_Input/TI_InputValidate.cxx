//===========================================================================//
// Purpose : Supporting methods for the TI_Input pre-processor class.  
//           These methods support validating input options and data files.
//
//           Private methods include:
//           - ValidateOptionsStore_
//           - ValidateArchitectureSpec_
//           - ValidateFabricModel_
//           - ValidateCircuitDesign_
//
//===========================================================================//

//---------------------------------------------------------------------------//
// Copyright (C) 2012-2013 Jeff Rudolph, Texas Instruments (jrudolph@ti.com) //
//                                                                           //
// Permission is hereby granted, free of charge, to any person obtaining a   //
// copy of this software and associated documentation files (the "Software"),//
// to deal in the Software without restriction, including without limitation //
// the rights to use, copy, modify, merge, publish, distribute, sublicense,  //
// and/or sell copies of the Software, and to permit persons to whom the     //
// Software is furnished to do so, subject to the following conditions:      //
//                                                                           //
// The above copyright notice and this permission notice shall be included   //
// in all copies or substantial portions of the Software.                    //
//                                                                           //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS   //
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF                //
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN // 
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  //
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR     //
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE //
// USE OR OTHER DEALINGS IN THE SOFTWARE.                                    //
//---------------------------------------------------------------------------//

#include "TI_Input.h"

//===========================================================================//
// Method         : ValidateOptionsStore_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
bool TI_Input_c::ValidateOptionsStore_( 
      void )
{
   bool ok = true;

   // This is only a stub... (currently looking for intereseting work to do)

   return( ok );
}

//===========================================================================//
// Method         : ValidateArchitectureSpec_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
bool TI_Input_c::ValidateArchitectureSpec_( 
      void )
{
   bool ok = true;

   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );
   printHandler.Info( "Validating architecture file ...\n" );

   ok = this->parchitectureSpec_->InitValidate( );

   return( ok );
}

//===========================================================================//
// Method         : ValidateFabricModel_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
bool TI_Input_c::ValidateFabricModel_( 
      void )
{
   bool ok = true;

   // This is only a stub... (currently looking for intereseting work to do)

   return( ok );
}

//===========================================================================//
// Method         : ValidateCircuitDesign_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
bool TI_Input_c::ValidateCircuitDesign_( 
      void )
{
   bool ok = true;

   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );
   printHandler.Info( "Validating circuit file '%s'...\n",
                      TIO_SR_STR( this->pcircuitDesign_->srName ));

   ok = this->pcircuitDesign_->InitValidate( );

   return( ok );
}
