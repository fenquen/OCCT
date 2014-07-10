// Created on: 1993-05-11
// Created by: Didier PIFFAULT
// Copyright (c) 1993-1999 Matra Datavision
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

#include <BRepMesh_DataStructureOfDelaun.hxx>
#include <BRepMesh_PairOfIndex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>

#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <Standard_ErrorHandler.hxx>

IMPLEMENT_STANDARD_HANDLE (BRepMesh_DataStructureOfDelaun, Standard_Transient)
IMPLEMENT_STANDARD_RTTIEXT(BRepMesh_DataStructureOfDelaun, Standard_Transient)

//=======================================================================
//function : BRepMesh_DataStructureOfDelaun
//purpose  : 
//=======================================================================
BRepMesh_DataStructureOfDelaun::BRepMesh_DataStructureOfDelaun(
  const BRepMeshCol::Allocator& theAllocator,
  const Standard_Integer        theReservedNodeSize)
  : myNodes           (theReservedNodeSize, theAllocator),
    myLinks           (theReservedNodeSize * 3),
    myDelLinks        (theAllocator),
    myElements        (theReservedNodeSize * 2),
    myElementsOfDomain(theReservedNodeSize * 2, theAllocator),
    myLinksOfDomain   (theReservedNodeSize * 2, theAllocator),
    myAllocator       (theAllocator)
{
}

//=======================================================================
//function : SubstituteNode
//purpose  : 
//=======================================================================
Standard_Boolean BRepMesh_DataStructureOfDelaun::SubstituteNode(
  const Standard_Integer theIndex, 
  const BRepMesh_Vertex& theNewNode)
{
  if (myNodes.FindIndex(theNewNode) != 0)
    return Standard_False;

  const BRepMeshCol::ListOfInteger& aLinks = myNodes(theIndex);
  myNodes.Substitute(theIndex, theNewNode, aLinks);
  return Standard_True;
}

//=======================================================================
//function : AddLink
//purpose  : 
//=======================================================================
Standard_Integer BRepMesh_DataStructureOfDelaun::AddLink(
  const BRepMesh_Edge& theLink)
{
  Standard_Integer aLinkIndex = IndexOf(theLink);
  if (aLinkIndex > 0)
  {
    return theLink.IsSameOrientation(GetLink(aLinkIndex)) ?
       aLinkIndex : -aLinkIndex;
  }

  BRepMesh_PairOfIndex aPair;
  if (!myDelLinks.IsEmpty())
  {
    aLinkIndex = myDelLinks.First();
    myLinks.Substitute(aLinkIndex, theLink, aPair);
    myDelLinks.RemoveFirst();
  }
  else
    aLinkIndex = myLinks.Add(theLink, aPair);

  const Standard_Integer aLinkId = Abs(aLinkIndex);
  myNodes(theLink.FirstNode()).Append(aLinkId);
  myNodes(theLink.LastNode() ).Append(aLinkId);
  myLinksOfDomain.Add(aLinkIndex);

  return aLinkIndex;
}

//=======================================================================
//function : SubstituteLink
//purpose  : 
//=======================================================================
Standard_Boolean BRepMesh_DataStructureOfDelaun::SubstituteLink(
  const Standard_Integer theIndex,
  const BRepMesh_Edge&   theNewLink)
{
  BRepMesh_PairOfIndex aPair;
  BRepMesh_Edge aLink = GetLink(theIndex);
  if (aLink.Movability() == BRepMesh_Deleted)
  {
    myLinks.Substitute(theIndex, theNewLink, aPair);
    return Standard_True;
  }

  if (IndexOf(theNewLink) != 0) 
    return Standard_False;

  aLink.SetMovability(BRepMesh_Deleted);
  myLinks.Substitute(theIndex, aLink, aPair);
  cleanLink(theIndex, aLink);

  const Standard_Integer aLinkId = Abs(theIndex);
  myNodes(theNewLink.FirstNode()).Append(aLinkId);
  myNodes(theNewLink.LastNode() ).Append(aLinkId);
  myLinks.Substitute(theIndex, theNewLink, aPair);

  return Standard_True;
}

//=======================================================================
//function : ForceRemoveLink
//purpose  : 
//=======================================================================
void BRepMesh_DataStructureOfDelaun::RemoveLink(
  const Standard_Integer theIndex,
  const Standard_Boolean isForce)
{
  BRepMesh_Edge& aLink = (BRepMesh_Edge&)GetLink(theIndex);
  if (aLink.Movability() == BRepMesh_Deleted            ||
      (!isForce && aLink.Movability() != BRepMesh_Free) ||
      ElementsConnectedTo(theIndex).Extent() != 0)
  {
    return;
  }

  cleanLink(theIndex, aLink);
  aLink.SetMovability(BRepMesh_Deleted);

  myLinksOfDomain.Remove(theIndex);
  myDelLinks.Append     (theIndex);
}

//=======================================================================
//function : cleanLink
//purpose  : 
//=======================================================================
void BRepMesh_DataStructureOfDelaun::cleanLink(
  const Standard_Integer theIndex,
  const BRepMesh_Edge&   theLink)
{
  for (Standard_Integer i = 0; i < 2; ++i)
  {
    const Standard_Integer aNodeId = (i == 0) ?
      theLink.FirstNode() : theLink.LastNode();

    BRepMeshCol::ListOfInteger& aLinkList = myNodes(aNodeId);
    BRepMeshCol::ListOfInteger::Iterator aLinkIt(aLinkList);
    for(; aLinkIt.More(); aLinkIt.Next())
    {
      if (aLinkIt.Value() == theIndex)
      {
        aLinkList.Remove(aLinkIt);
        break;
      }
    }
  }
}

//=======================================================================
//function : AddElement
//purpose  : 
//=======================================================================
Standard_Integer BRepMesh_DataStructureOfDelaun::AddElement(
  const BRepMesh_Triangle& theElement)
{
  Standard_Integer aElementIndex = IndexOf(theElement);
  if (aElementIndex > 0)
    return aElementIndex;

  aElementIndex = myElements.Add(theElement);
  myElementsOfDomain.Add(aElementIndex);

  Standard_Integer e[3];
  Standard_Boolean o[3];
  theElement.Edges(e, o);
  for (Standard_Integer i = 0; i < 3; ++i)
    myLinks(e[i]).Append(aElementIndex);

  return aElementIndex;
}

//=======================================================================
//function : RemoveElement
//purpose  : 
//=======================================================================
void BRepMesh_DataStructureOfDelaun::RemoveElement(
  const Standard_Integer theIndex)
{
  BRepMesh_Triangle& aElement = (BRepMesh_Triangle&)GetElement(theIndex);
  if (aElement.Movability() == BRepMesh_Deleted)
    return;

  cleanElement(theIndex, aElement);
  aElement.SetMovability(BRepMesh_Deleted);
  myElementsOfDomain.Remove(theIndex);
}

//=======================================================================
//function : cleanElement
//purpose  : 
//=======================================================================
void BRepMesh_DataStructureOfDelaun::cleanElement(
  const Standard_Integer   theIndex,
  const BRepMesh_Triangle& theElement)
{
  if (theElement.Movability() != BRepMesh_Free)
    return;

  Standard_Integer e[3];
  Standard_Boolean o[3];
  theElement.Edges(e, o);

  for (Standard_Integer i = 0; i < 3; ++i)
    removeElementIndex(theIndex, myLinks(e[i]));
}

//=======================================================================
//function : removeElementIndex
//purpose  : 
//=======================================================================
void BRepMesh_DataStructureOfDelaun::removeElementIndex(
  const Standard_Integer theIndex,
  BRepMesh_PairOfIndex&  thePair)
{
  for (Standard_Integer i = 1, n = thePair.Extent(); i <= n; ++i)
  {
    if (thePair.Index(i) == theIndex)
    {
      thePair.RemoveIndex(i);
      return;
    }
  }
}

//=======================================================================
//function : SubstituteElement
//purpose  : 
//=======================================================================
Standard_Boolean BRepMesh_DataStructureOfDelaun::SubstituteElement(
  const Standard_Integer   theIndex,
  const BRepMesh_Triangle& theNewElement)
{
  const BRepMesh_Triangle& aElement = GetElement(theIndex);
  if (aElement.Movability() == BRepMesh_Deleted) 
  {
    myElements.Substitute(theIndex, theNewElement);
    return Standard_True;
  }

  if (IndexOf(theNewElement) != 0)
    return Standard_False;

  cleanElement(theIndex, aElement);
  // Warning: here new element and old element should have different Hash code
  myElements.Substitute(theIndex, theNewElement);

  Standard_Integer e[3];
  Standard_Boolean o[3];
  theNewElement.Edges(e, o);
  for (Standard_Integer i = 0; i < 3; ++i)
    myLinks(e[i]).Append(theIndex);

  return Standard_True;
}

//=======================================================================
//function : ElementNodes
//purpose  :
//=======================================================================
void BRepMesh_DataStructureOfDelaun::ElementNodes(
    const BRepMesh_Triangle& theElement,
    Standard_Integer         (&theNodes)[3])
{
  Standard_Integer e[3];
  Standard_Boolean o[3];
  theElement.Edges(e, o);

  const BRepMesh_Edge& aLink1 = GetLink(e[0]);
  if (o[0])
  {
    theNodes[0] = aLink1.FirstNode();
    theNodes[1] = aLink1.LastNode();
  }
  else
  {
    theNodes[1] = aLink1.FirstNode();
    theNodes[0] = aLink1.LastNode();
  }
  
  const BRepMesh_Edge& aLink2 = GetLink(e[2]);
  if (o[2])
    theNodes[2] = aLink2.FirstNode();
  else
    theNodes[2] = aLink2.LastNode();
}

//=======================================================================
//function : ClearDomain
//purpose  : 
//=======================================================================
void BRepMesh_DataStructureOfDelaun::ClearDomain()
{
  BRepMeshCol::MapOfInteger aFreeEdges;
  BRepMeshCol::MapOfInteger::Iterator aElementIt(myElementsOfDomain);
  for (; aElementIt.More(); aElementIt.Next())
  {
    const Standard_Integer aElementId = aElementIt.Key();
    BRepMesh_Triangle& aElement = (BRepMesh_Triangle&)GetElement(aElementId);

    Standard_Integer e[3];
    Standard_Boolean o[3];
    aElement.Edges(e, o);

    for (Standard_Integer i = 0; i < 3; ++i)
      aFreeEdges.Add(e[i]);

    cleanElement(aElementId, aElement);
    aElement.SetMovability(BRepMesh_Deleted);
  }
  myElementsOfDomain.Clear();

  BRepMeshCol::MapOfInteger::Iterator aEdgeIt(aFreeEdges);
  for (; aEdgeIt.More(); aEdgeIt.Next())
    RemoveLink(aEdgeIt.Key());
}

//=======================================================================
//function : clearDeletedLinks
//purpose  : 
//=======================================================================
void BRepMesh_DataStructureOfDelaun::clearDeletedLinks()
{
  Standard_Integer aLastLiveItem = NbLinks();
  while (!myDelLinks.IsEmpty())
  {
    while (aLastLiveItem > 0)
    {
      if (GetLink(aLastLiveItem).Movability() != BRepMesh_Deleted)
        break;

      myLinks.RemoveLast();
      --aLastLiveItem;
    }

    Standard_Integer aDelItem = myDelLinks.First();
    myDelLinks.RemoveFirst();

    if (aDelItem > aLastLiveItem)
      continue;

    BRepMesh_Edge aLink = GetLink(aLastLiveItem);
    BRepMesh_PairOfIndex& aPair = myLinks(aLastLiveItem);

    myLinks.RemoveLast();
    myLinks.Substitute(aDelItem, aLink, aPair);

    myLinksOfDomain.Remove(aLastLiveItem);
    myLinksOfDomain.Add(aDelItem);
    --aLastLiveItem;

    const Standard_Integer aLastLiveItemId = aLastLiveItem + 1;
    BRepMeshCol::ListOfInteger::Iterator aLinkIt;
    // update link references
    for (Standard_Integer i = 0; i < 2; ++i)
    {
      const Standard_Integer aCurNodeId = (i == 0) ?
        aLink.FirstNode() : aLink.LastNode();

      for (aLinkIt.Init(myNodes(aCurNodeId)); aLinkIt.More(); aLinkIt.Next())
      {
        Standard_Integer& aLinkId = aLinkIt.ChangeValue();
        if (aLinkId == aLastLiveItemId)
        {
          aLinkId = aDelItem;
          break;
        }
      }
    }

    // update elements references
    for(Standard_Integer j = 1, jn = aPair.Extent(); j <= jn; ++j)
    {
      const BRepMesh_Triangle& aElement = GetElement(aPair.Index(j));

      Standard_Integer e[3];
      Standard_Boolean o[3];
      aElement.Edges(e, o);
      for (Standard_Integer i = 0; i < 3; ++i)
      {
        if (e[i] == aLastLiveItemId)
        {
          e[i] = aDelItem;
          break;
        }
      }

      myElements.Substitute(aLinkIt.Value(), 
        BRepMesh_Triangle(e, o, aElement.Movability()));
    }
  }
}

//=======================================================================
//function : clearDeletedNodes
//purpose  : 
//=======================================================================
void BRepMesh_DataStructureOfDelaun::clearDeletedNodes()
{
  BRepMeshCol::ListOfInteger& aDelNodes = 
    (BRepMeshCol::ListOfInteger&)myNodes.GetListOfDelNodes();

  Standard_Integer aLastLiveItem = NbNodes();
  while (!aDelNodes.IsEmpty())
  {
    while (aLastLiveItem > 0)
    {
      if (GetNode(aLastLiveItem).Movability() != BRepMesh_Deleted)
        break;

      myNodes.RemoveLast();
      --aLastLiveItem;
    }

    Standard_Integer aDelItem = aDelNodes.First();
    aDelNodes.RemoveFirst();

    if (aDelItem > aLastLiveItem)
      continue;

    BRepMesh_Vertex aNode = GetNode(aLastLiveItem);
    BRepMeshCol::ListOfInteger& aLinkList = myNodes(aLastLiveItem);

    myNodes.RemoveLast();
    --aLastLiveItem;

    myNodes.Substitute(aDelItem, aNode, aLinkList);

    const Standard_Integer aLastLiveItemId = aLastLiveItem + 1;
    BRepMeshCol::ListOfInteger::Iterator aLinkIt(aLinkList);
    for (; aLinkIt.More(); aLinkIt.Next())
    {
      const Standard_Integer aLinkId = aLinkIt.Value();
      const BRepMesh_Edge& aLink = GetLink(aLinkId);
      BRepMesh_PairOfIndex& aPair = myLinks(aLinkId);

      Standard_Integer v[2] = { aLink.FirstNode(), aLink.LastNode() };
      if (v[0] == aLastLiveItemId)
        v[0] = aDelItem;
      else if (v[1] == aLastLiveItemId)
        v[1] = aDelItem;

      myLinks.Substitute(aLinkId,
        BRepMesh_Edge(v[0], v[1], aLink.Movability()), aPair);
    }
  }
}

//=======================================================================
//function : Statistics
//purpose  : 
//=======================================================================
void BRepMesh_DataStructureOfDelaun::Statistics(Standard_OStream& theStream) const
{
  theStream << " Map of nodes : \n";
  myNodes.Statistics(theStream);
  theStream << "\n Deleted nodes : " << myNodes.GetListOfDelNodes().Extent() << endl;

  theStream << "\n\n Map of Links : \n";
  myLinks.Statistics(theStream);
  theStream << "\n Deleted links : " << myDelLinks.Extent() << endl;

  theStream << "\n\n Map of elements : \n";
  myElements.Statistics(theStream);
}

//=======================================================================
//function : BRepMesh_Write
//purpose  : 
//  Global function not declared in any public header, intended for use 
//  from debugger prompt (Command Window in Visual Studio).
//
//  Stores the mesh data structure to BRep file with the given name.
//=======================================================================
Standard_CString BRepMesh_Dump(void*            theMeshHandlePtr,
                               Standard_CString theFileNameStr)
{
  if (theMeshHandlePtr == 0 || theFileNameStr == 0)
  {
    return "Error: file name or mesh data is null";
  }

  Handle(BRepMesh_DataStructureOfDelaun) aMeshData = 
    *(Handle(BRepMesh_DataStructureOfDelaun)*)theMeshHandlePtr;

  if (aMeshData.IsNull())
    return "Error: mesh data is empty";

  TopoDS_Compound aMesh;
  BRep_Builder aBuilder;
  aBuilder.MakeCompound(aMesh);

  try
  {
    OCC_CATCH_SIGNALS

    BRepMeshCol::MapOfInteger::Iterator aLinksIt(aMeshData->LinksOfDomain());
    for (; aLinksIt.More(); aLinksIt.Next())
    {
      const BRepMesh_Edge& aLink = aMeshData->GetLink(aLinksIt.Value());
      gp_Pnt aPnt[2];
      for (Standard_Integer i = 0; i < 2; ++i)
      {
        const Standard_Integer aNodeId = 
          (i == 0) ? aLink.FirstNode() : aLink.LastNode();

        const gp_XY& aNode = aMeshData->GetNode(aNodeId).Coord();
        aPnt[i] = gp_Pnt(aNode.X(), aNode.Y(), 0.);
      }

      if (aPnt[0].SquareDistance(aPnt[1]) < Precision::SquareConfusion())
        continue;

      aBuilder.Add(aMesh, BRepBuilderAPI_MakeEdge(aPnt[0], aPnt[1]));
    }

    if (!BRepTools::Write(aMesh, theFileNameStr))
      return "Error: write failed";
  }
  catch (Standard_Failure)
  {
    return Standard_Failure::Caught()->GetMessageString();
  }

  return theFileNameStr;
}
