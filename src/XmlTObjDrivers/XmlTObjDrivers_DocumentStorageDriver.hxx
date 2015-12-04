// Created on: 2004-11-24
// Created by: Michael SAZONOV
// Copyright (c) 2004-2014 OPEN CASCADE SAS
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

// The original implementation Copyright: (C) RINA S.p.A

#ifndef XmlTObjDrivers_DocumentStorageDriver_HeaderFile
#define XmlTObjDrivers_DocumentStorageDriver_HeaderFile

#include <TObj_Common.hxx>
#include <XmlLDrivers_DocumentStorageDriver.hxx>
#include <XmlMDF_ADriverTable.hxx>


//  Block of comments describing class XmlTObjDrivers_DocumentStorageDriver
//

class XmlTObjDrivers_DocumentStorageDriver :
  public XmlLDrivers_DocumentStorageDriver
{
 public:
  // ---------- PUBLIC METHODS ----------

  Standard_EXPORT XmlTObjDrivers_DocumentStorageDriver
                        (const TCollection_ExtendedString& theCopyright);
  // Constructor

  Standard_EXPORT virtual Handle(XmlMDF_ADriverTable) AttributeDrivers
                        (const Handle(CDM_MessageDriver)& theMsgDriver) Standard_OVERRIDE;

 public:
  // Declaration of CASCADE RTTI
  DEFINE_STANDARD_RTTIEXT(XmlTObjDrivers_DocumentStorageDriver,XmlLDrivers_DocumentStorageDriver)
};

// Definition of HANDLE object using Standard_DefineHandle.hxx
DEFINE_STANDARD_HANDLE (XmlTObjDrivers_DocumentStorageDriver,
                        XmlLDrivers_DocumentStorageDriver)

#endif

#ifdef _MSC_VER
#pragma once
#endif
