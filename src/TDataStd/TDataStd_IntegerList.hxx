// Created on: 2007-05-29
// Created by: Vlad Romashko
// Copyright (c) 2007-2014 OPEN CASCADE SAS
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

#ifndef _TDataStd_IntegerList_HeaderFile
#define _TDataStd_IntegerList_HeaderFile

#include <Standard.hxx>
#include <Standard_Type.hxx>

#include <TColStd_ListOfInteger.hxx>
#include <TDF_Attribute.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Integer.hxx>
#include <Standard_OStream.hxx>
class Standard_GUID;
class TDF_Label;
class TDF_Attribute;
class TDF_RelocationTable;


class TDataStd_IntegerList;
DEFINE_STANDARD_HANDLE(TDataStd_IntegerList, TDF_Attribute)

//! Contains a list of integers.
class TDataStd_IntegerList : public TDF_Attribute
{

public:

  
  //! Static methods
  //! ==============
  //! Returns the ID of the list of integer attribute.
  Standard_EXPORT static const Standard_GUID& GetID();
  
  //! Finds or creates a list of integer values attribute.
  Standard_EXPORT static Handle(TDataStd_IntegerList) Set (const TDF_Label& label);
  
  Standard_EXPORT TDataStd_IntegerList();
  
  Standard_EXPORT Standard_Boolean IsEmpty() const;
  
  Standard_EXPORT Standard_Integer Extent() const;
  
  Standard_EXPORT void Prepend (const Standard_Integer value);
  
  Standard_EXPORT void Append (const Standard_Integer value);
  
  //! Inserts the <value> before the first meet of <before_value>.
  Standard_EXPORT Standard_Boolean InsertBefore (const Standard_Integer value, const Standard_Integer before_value);
  
  //! Inserts the <value> after the first meet of <after_value>.
  Standard_EXPORT Standard_Boolean InsertAfter (const Standard_Integer value, const Standard_Integer after_value);
  
  //! Removes the first meet of the <value>.
  Standard_EXPORT Standard_Boolean Remove (const Standard_Integer value);
  
  Standard_EXPORT void Clear();
  
  Standard_EXPORT Standard_Integer First() const;
  
  Standard_EXPORT Standard_Integer Last() const;
  
  Standard_EXPORT const TColStd_ListOfInteger& List() const;
  
  Standard_EXPORT const Standard_GUID& ID() const Standard_OVERRIDE;
  
  Standard_EXPORT void Restore (const Handle(TDF_Attribute)& With) Standard_OVERRIDE;
  
  Standard_EXPORT Handle(TDF_Attribute) NewEmpty() const Standard_OVERRIDE;
  
  Standard_EXPORT void Paste (const Handle(TDF_Attribute)& Into, const Handle(TDF_RelocationTable)& RT) const Standard_OVERRIDE;
  
  Standard_EXPORT virtual Standard_OStream& Dump (Standard_OStream& anOS) const Standard_OVERRIDE;




  DEFINE_STANDARD_RTTIEXT(TDataStd_IntegerList,TDF_Attribute)

protected:




private:


  TColStd_ListOfInteger myList;


};







#endif // _TDataStd_IntegerList_HeaderFile
