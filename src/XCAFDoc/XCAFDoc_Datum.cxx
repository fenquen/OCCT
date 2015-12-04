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


#include <Standard_GUID.hxx>
#include <Standard_Type.hxx>
#include <TCollection_HAsciiString.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_Label.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_RelocationTable.hxx>
#include <XCAFDoc_Datum.hxx>
#include <TDataStd_AsciiString.hxx>
#include <TDataStd_IntegerArray.hxx>
#include <TDataStd_RealArray.hxx>
#include <TDataStd_Integer.hxx>
#include <TDataStd_Real.hxx>
#include <TNaming_NamedShape.hxx>
#include <TNaming_Builder.hxx>


#include <XCAFDimTolObjects_DatumObject.hxx>

IMPLEMENT_STANDARD_RTTIEXT(XCAFDoc_Datum,TDF_Attribute)

enum ChildLab
{
  ChildLab_Name = 1,
  ChildLab_Position,
  ChildLab_Modifiers,
  ChildLab_ModifierWithValue,
  ChildLab_IsDTarget,
  ChildLab_DTargetType,
  ChildLab_AxisLoc,
  ChildLab_AxisN,
  ChildLab_AxisRef,
  ChildLab_DTargetLength,
  ChildLab_DTargetWidth,
  ChildLab_DTargetNumber,
  ChildLab_DatumTarget,
};

//=======================================================================
//function : XCAFDoc_Datum
//purpose  : 
//=======================================================================
XCAFDoc_Datum::XCAFDoc_Datum()
{
}


//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& XCAFDoc_Datum::GetID() 
{
  static Standard_GUID DID("58ed092e-44de-11d8-8776-001083004c77");
  //static Standard_GUID ID("efd212e2-6dfd-11d4-b9c8-0060b0ee281b");
  return DID;
  //return ID;
}


//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(XCAFDoc_Datum) XCAFDoc_Datum::Set(const TDF_Label& theLabel,
                                         const Handle(TCollection_HAsciiString)& theName,
                                         const Handle(TCollection_HAsciiString)& theDescription,
                                         const Handle(TCollection_HAsciiString)& theIdentification) 
{
  Handle(XCAFDoc_Datum) aDatum;
  if (!theLabel.FindAttribute(XCAFDoc_Datum::GetID(), aDatum)) {
    aDatum = new XCAFDoc_Datum();
    theLabel.AddAttribute(aDatum);
  }
  aDatum->Set(theName,theDescription,theIdentification); 
  return aDatum;
}

//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(XCAFDoc_Datum) XCAFDoc_Datum::Set(const TDF_Label& theLabel) 
{
  Handle(XCAFDoc_Datum) aDatum;
  if (!theLabel.FindAttribute(XCAFDoc_Datum::GetID(), aDatum)) {
    aDatum = new XCAFDoc_Datum();
    theLabel.AddAttribute(aDatum);
  }
  return aDatum;
}


//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

void XCAFDoc_Datum::Set(const Handle(TCollection_HAsciiString)& theName,
                        const Handle(TCollection_HAsciiString)& theDescription,
                        const Handle(TCollection_HAsciiString)& theIdentification) 
{
  Backup();
  myName = theName;
  myDescription = theDescription;
  myIdentification = theIdentification;
}


//=======================================================================
//function : GetName
//purpose  : 
//=======================================================================

Handle(TCollection_HAsciiString) XCAFDoc_Datum::GetName() const
{
  if(myName.IsNull())
    return new TCollection_HAsciiString();
  return myName;
}

//=======================================================================
//function : GetDescriptio7n
//purpose  : 
//=======================================================================

Handle(TCollection_HAsciiString) XCAFDoc_Datum::GetDescription() const
{
  return myDescription;
}


//=======================================================================
//function : GetIdentification
//purpose  : 
//=======================================================================

Handle(TCollection_HAsciiString) XCAFDoc_Datum::GetIdentification() const
{
  return myIdentification;
}

//=======================================================================
//function : SetObject
//purpose  : 
//=======================================================================

void XCAFDoc_Datum::SetObject(const Handle(XCAFDimTolObjects_DatumObject)& theObject)
{
  Backup();
  TDF_ChildIterator anIter(Label());
  for(;anIter.More(); anIter.Next())
  {
    anIter.Value().ForgetAllAttributes();
  }
  if (!theObject->GetName().IsNull() && !theObject->GetName()->IsEmpty())
  {
    Handle(TDataStd_AsciiString) anAttName;
    if(!Label().FindChild(ChildLab_Name).FindAttribute(TDataStd_AsciiString::GetID(), anAttName))
    {
      anAttName = new TDataStd_AsciiString();
      Label().FindChild(ChildLab_Name).AddAttribute(anAttName);
    }
    anAttName->Set(theObject->GetName()->String());
  }

  Handle(TDataStd_Integer) aPosition = new TDataStd_Integer();
  aPosition->Set(theObject->GetPosition());
  Label().FindChild(ChildLab_Position).AddAttribute(aPosition);

  if(theObject->GetModifiers().Length() > 0)
  {
    Handle(TDataStd_IntegerArray) aModifiers;
    if(!Label().FindChild(ChildLab_Modifiers).FindAttribute(TDataStd_IntegerArray::GetID(), aModifiers))
    {
      aModifiers = new TDataStd_IntegerArray();
      Label().FindChild(ChildLab_Modifiers).AddAttribute(aModifiers);
    }
    Handle(TColStd_HArray1OfInteger) anArr = new TColStd_HArray1OfInteger(1,theObject->GetModifiers().Length());
    for(Standard_Integer i = 1; i <= theObject->GetModifiers().Length(); i++)
      anArr->SetValue(i,theObject->GetModifiers().Value(i));
    aModifiers->ChangeArray(anArr);
  }

  XCAFDimTolObjects_DatumModifWithValue aM;
  Standard_Real aV;
  theObject->GetModifierWithValue(aM, aV);
  if(aM != XCAFDimTolObjects_DatumModifWithValue_None)
  {
    Handle(TDataStd_Integer) aModifierWithValueM;
    if(!Label().FindChild(ChildLab_ModifierWithValue).FindAttribute(TDataStd_Integer::GetID(), aModifierWithValueM))
    {
      aModifierWithValueM = new TDataStd_Integer();
      Label().FindChild(ChildLab_ModifierWithValue).AddAttribute(aModifierWithValueM);
    }
    Handle(TDataStd_Real) aModifierWithValueV;
    if(!Label().FindChild(ChildLab_ModifierWithValue).FindAttribute(TDataStd_Real::GetID(), aModifierWithValueV))
    {
      aModifierWithValueV = new TDataStd_Real();
      Label().FindChild(ChildLab_ModifierWithValue).AddAttribute(aModifierWithValueV);
    }
    aModifierWithValueM->Set(aM);
    aModifierWithValueV->Set(aV);
  }

  Handle(TDataStd_Integer) aIsTarget = new TDataStd_Integer();
  aIsTarget->Set(theObject->IsDatumTarget());
  Label().FindChild(ChildLab_IsDTarget).AddAttribute(aIsTarget);

  if(theObject->IsDatumTarget())
  {
    Handle(TDataStd_Integer) aType = new TDataStd_Integer();
    aType->Set(theObject->GetDatumTargetType());
    Label().FindChild(ChildLab_DTargetType).AddAttribute(aType);

    if(theObject->GetDatumTargetType() == XCAFDimTolObjects_DatumTargetType_Area)
    {
      if(!theObject->GetDatumTarget().IsNull())
      {
        TNaming_Builder tnBuild(Label().FindChild(ChildLab_DatumTarget));
        tnBuild.Generated(theObject->GetDatumTarget());
      }
    }
    else
    {
      Handle(TDataStd_RealArray) aLoc = new TDataStd_RealArray();
      Handle(TDataStd_RealArray) aN = new TDataStd_RealArray();
      Handle(TDataStd_RealArray) aR = new TDataStd_RealArray();
      gp_Ax2 anAx = theObject->GetDatumTargetAxis();

      Handle(TColStd_HArray1OfReal) aLocArr = new TColStd_HArray1OfReal(1, 3);
      for (Standard_Integer i = 1; i <= 3; i++)
        aLocArr->SetValue(i, anAx.Location().Coord(i));
      aLoc->ChangeArray(aLocArr);

      Handle(TColStd_HArray1OfReal) aNArr = new TColStd_HArray1OfReal(1, 3);
      for (Standard_Integer i = 1; i <= 3; i++)
        aNArr->SetValue(i, anAx.Direction().Coord(i));
      aN->ChangeArray(aNArr);

      Handle(TColStd_HArray1OfReal) aRArr = new TColStd_HArray1OfReal(1, 3);
      for (Standard_Integer i = 1; i <= 3; i++)
        aRArr->SetValue(i, anAx.XDirection().Coord(i));
      aR->ChangeArray(aRArr);

      Label().FindChild(ChildLab_AxisLoc).AddAttribute(aLoc);
      Label().FindChild(ChildLab_AxisN).AddAttribute(aN);
      Label().FindChild(ChildLab_AxisRef).AddAttribute(aR);

      if(theObject->GetDatumTargetType() != XCAFDimTolObjects_DatumTargetType_Point)
      {
        Handle(TDataStd_Real) aLen = new TDataStd_Real();
        aLen->Set(theObject->GetDatumTargetLength());
        Label().FindChild(ChildLab_DTargetLength).AddAttribute(aLen);
        if(theObject->GetDatumTargetType() == XCAFDimTolObjects_DatumTargetType_Rectangle)
        {
          Handle(TDataStd_Real) aWidth = new TDataStd_Real();
          aWidth->Set(theObject->GetDatumTargetWidth());
          Label().FindChild(ChildLab_DTargetWidth).AddAttribute(aWidth);
        }
      }
    }
    Handle(TDataStd_Integer) aNum = new TDataStd_Integer();
    aNum->Set(theObject->GetDatumTargetNumber());
    Label().FindChild(ChildLab_DTargetNumber).AddAttribute(aNum);
  }
}

//=======================================================================
//function : GetObject
//purpose  : 
//=======================================================================

Handle(XCAFDimTolObjects_DatumObject) XCAFDoc_Datum::GetObject() const
{
  Handle(XCAFDimTolObjects_DatumObject) anObj = new XCAFDimTolObjects_DatumObject();

  Handle(TDataStd_AsciiString) anAttName;
  if(Label().FindChild(ChildLab_Name).FindAttribute(TDataStd_AsciiString::GetID(), anAttName))
  {
    Handle(TCollection_HAsciiString) aStr = new TCollection_HAsciiString(anAttName->Get());
    anObj->SetName(aStr);
  }

  Handle(TDataStd_IntegerArray) anArr;
  if(Label().FindChild(ChildLab_Modifiers).FindAttribute(TDataStd_IntegerArray::GetID(), anArr)
     && !anArr->Array().IsNull())
  {
    XCAFDimTolObjects_DatumModifiersSequence aModifiers;
    for(Standard_Integer i = 1; i <= anArr->Length(); i++)
      aModifiers.Append((XCAFDimTolObjects_DatumSingleModif)anArr->Value(i));
    anObj->SetModifiers(aModifiers);
  }

  Handle(TDataStd_Integer) aModifierWithValueM;
  if(Label().FindChild(ChildLab_ModifierWithValue).FindAttribute(TDataStd_Integer::GetID(), aModifierWithValueM))
  {
    Handle(TDataStd_Real) aModifierWithValueV;
    if(Label().FindChild(ChildLab_ModifierWithValue).FindAttribute(TDataStd_Real::GetID(), aModifierWithValueV))
    {
      anObj->SetModifierWithValue((XCAFDimTolObjects_DatumModifWithValue)aModifierWithValueM->Get(),aModifierWithValueV->Get());
    }
  }

  Handle(TDataStd_Integer) aPosition;
  if(Label().FindChild(ChildLab_Position).FindAttribute(TDataStd_Integer::GetID(), aPosition))
  {
    anObj->SetPosition(aPosition->Get());
  }

  Handle(TDataStd_Integer) aIsDTarget;
  if(Label().FindChild(ChildLab_IsDTarget).FindAttribute(TDataStd_Integer::GetID(), aIsDTarget))
  {
    anObj->IsDatumTarget((aIsDTarget->Get() != 0));
  }
  else
  {
    return anObj;
  }

  if (aIsDTarget->Get() != 0)
  {
    Handle(TDataStd_Integer) aDTargetType;
    if(Label().FindChild(ChildLab_DTargetType).FindAttribute(TDataStd_Integer::GetID(), aDTargetType))
    {
      anObj->SetDatumTargetType((XCAFDimTolObjects_DatumTargetType)aDTargetType->Get());
      if(anObj->GetDatumTargetType() == XCAFDimTolObjects_DatumTargetType_Area)
      {
        Handle(TNaming_NamedShape) aDatumTarget;
        if(Label().FindChild(ChildLab_DatumTarget).FindAttribute(TNaming_NamedShape::GetID(), aDatumTarget))
        {
          anObj->SetDatumTarget(aDatumTarget->Get());
        }
      }
      else
      {
        Handle(TDataStd_RealArray) aLoc;
        Handle(TDataStd_RealArray) aN;
        Handle(TDataStd_RealArray) aR;
        if(Label().FindChild(ChildLab_AxisLoc).FindAttribute(TDataStd_RealArray::GetID(), aLoc) && aLoc->Length() == 3 &&
          Label().FindChild(ChildLab_AxisN).FindAttribute(TDataStd_RealArray::GetID(), aN) && aN->Length() == 3 &&
          Label().FindChild(ChildLab_AxisRef).FindAttribute(TDataStd_RealArray::GetID(), aR) && aR->Length() == 3 )
        {
          gp_Pnt aL(aLoc->Value(aLoc->Lower()), aLoc->Value(aLoc->Lower()+1), aLoc->Value(aLoc->Lower()+2));
          gp_Dir aD(aN->Value(aN->Lower()), aN->Value(aN->Lower()+1), aN->Value(aN->Lower()+2));
          gp_Dir aDR(aR->Value(aR->Lower()), aR->Value(aR->Lower()+1), aR->Value(aR->Lower()+2));
          gp_Ax2 anAx(aL, aD, aDR);
          anObj->SetDatumTargetAxis(anAx);
        }

        if(anObj->GetDatumTargetType() != XCAFDimTolObjects_DatumTargetType_Point)
        {
          Handle(TDataStd_Real) aLen;
          if(Label().FindChild(ChildLab_DTargetLength).FindAttribute(TDataStd_Real::GetID(), aLen))
          {
            anObj->SetDatumTargetLength(aLen->Get());
          }
          if(anObj->GetDatumTargetType() == XCAFDimTolObjects_DatumTargetType_Rectangle)
          {
            Handle(TDataStd_Real) aWidth;
            if(Label().FindChild(ChildLab_DTargetWidth).FindAttribute(TDataStd_Real::GetID(), aWidth))
            {
              anObj->SetDatumTargetWidth(aWidth->Get());
            }
          }
        }
      }
    }
    Handle(TDataStd_Integer) aNum;
    if(Label().FindChild(ChildLab_DTargetNumber).FindAttribute(TDataStd_Integer::GetID(), aNum))
    {
      anObj->SetDatumTargetNumber(aNum->Get());
    }
    else 
    {
      anObj->SetDatumTargetNumber(0);
    }
  }

  return anObj;
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& XCAFDoc_Datum::ID() const
{
  return GetID();
}


//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void XCAFDoc_Datum::Restore(const Handle(TDF_Attribute)& theWith) 
{
  myName = Handle(XCAFDoc_Datum)::DownCast(theWith)->GetName();
  myDescription = Handle(XCAFDoc_Datum)::DownCast(theWith)->GetDescription();
  myIdentification = Handle(XCAFDoc_Datum)::DownCast(theWith)->GetIdentification();
}


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) XCAFDoc_Datum::NewEmpty() const
{
  return new XCAFDoc_Datum();
}


//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void XCAFDoc_Datum::Paste(const Handle(TDF_Attribute)& theInto,
                          const Handle(TDF_RelocationTable)& /*RT*/) const
{
  Handle(XCAFDoc_Datum)::DownCast(theInto)->Set(myName,myDescription,myIdentification);
}

