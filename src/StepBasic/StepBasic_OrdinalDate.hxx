// Created on: 1995-12-01
// Created by: EXPRESS->CDL V0.2 Translator
// Copyright (c) 1995-1999 Matra Datavision
// Copyright (c) 1999-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#ifndef _StepBasic_OrdinalDate_HeaderFile
#define _StepBasic_OrdinalDate_HeaderFile

#include <Standard.hxx>
#include <Standard_Type.hxx>

#include <Standard_Integer.hxx>
#include <StepBasic_Date.hxx>


class StepBasic_OrdinalDate;
DEFINE_STANDARD_HANDLE(StepBasic_OrdinalDate, StepBasic_Date)


class StepBasic_OrdinalDate : public StepBasic_Date
{

public:

  
  //! Returns a OrdinalDate
  Standard_EXPORT StepBasic_OrdinalDate();
  
  Standard_EXPORT virtual void Init (const Standard_Integer aYearComponent) Standard_OVERRIDE;
  
  Standard_EXPORT virtual void Init (const Standard_Integer aYearComponent, const Standard_Integer aDayComponent);
  
  Standard_EXPORT void SetDayComponent (const Standard_Integer aDayComponent);
  
  Standard_EXPORT Standard_Integer DayComponent() const;




  DEFINE_STANDARD_RTTIEXT(StepBasic_OrdinalDate,StepBasic_Date)

protected:




private:


  Standard_Integer dayComponent;


};







#endif // _StepBasic_OrdinalDate_HeaderFile
