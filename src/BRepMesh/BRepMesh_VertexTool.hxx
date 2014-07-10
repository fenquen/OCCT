// Copyright (c) 2013 OPEN CASCADE SAS
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

#ifndef _BRepMesh_VertexTool_HeaderFile
#define _BRepMesh_VertexTool_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>
#include <BRepMesh_VertexInspector.hxx>
#include <BRepMesh_Collections.hxx>
#include <Standard_OStream.hxx>
#include <gp_XYZ.hxx>
#include <gp_XY.hxx>

class BRepMesh_Vertex;

//! Describes data structure intended to keep mesh nodes 
//! defined in UV space and implements functionality 
//! providing their uniqueness regarding thir position.
class BRepMesh_VertexTool
{
public:

  DEFINE_STANDARD_ALLOC
  
  //! Constructor.
  //! \param theReservedSize size to be reserved for vector of vertices.
  //! \param theAllocator memory allocator to be used by internal collections.
  Standard_EXPORT BRepMesh_VertexTool(const Standard_Integer        theReservedSize,
                                      const BRepMeshCol::Allocator& theAllocator);

  //! Sets new size of cell for cellfilter equal in both directions.
  Standard_EXPORT void SetCellSize(const Standard_Real theSize)
  {
    myCellFilter.Reset(theSize, myAllocator);
    mySelector.Clear();
  }

  //! Sets new size of cell for cellfilter.
  //! \param theSizeX size for X dimension.
  //! \param theSizeY size for Y dimension.
  Standard_EXPORT void SetCellSize(const Standard_Real theSizeX,
                                   const Standard_Real theSizeY)
  {
    Standard_Real aCellSize[2] = { theSizeX, theSizeY };
    myCellFilter.Reset(aCellSize, myAllocator);
    mySelector.Clear();
  }

  //! Sets the tolerance to be used for identification of 
  //! coincident vertices equal for both dimensions.
  Standard_EXPORT void SetTolerance(const Standard_Real theTolerance)
  {
    mySelector.SetTolerance( theTolerance );
    myTolerance[0] = theTolerance;
    myTolerance[1] = theTolerance;
  }

  //! Sets the tolerance to be used for identification of 
  //! coincident vertices.
  //! \param theToleranceX tolerance for X dimension.
  //! \param theToleranceY tolerance for Y dimension.
  Standard_EXPORT void SetTolerance(const Standard_Real theToleranceX,
                                    const Standard_Real theToleranceY)
  {
    mySelector.SetTolerance( theToleranceX, theToleranceY );
    myTolerance[0] = theToleranceX;
    myTolerance[1] = theToleranceY;
  }

  //! Adds vertex with empty data to the tool.
  Standard_EXPORT Standard_Integer Add(const BRepMesh_Vertex& theVertex);

  //! Adds vertex with associated data to the tool.
  //! \param theVertex vertex to be added.
  //! \param theParams data associated with the vertex.
  Standard_EXPORT Standard_Integer Add(const BRepMesh_Vertex&            theVertex,
                                       const BRepMeshCol::ListOfInteger& theParams);

  //! Deletes vertex with the given index from the tool.
  Standard_EXPORT void Delete(const Standard_Integer theIndex);

  //! Returns data assigned to link with the given index.
  //! \param theIndex index of link which data should be returned.
  //! \return attached data.
  inline BRepMeshCol::ListOfInteger& FindFromIndex(const Standard_Integer theIndex) const
  {
    return (BRepMeshCol::ListOfInteger&)myLinksMap.Find(theIndex);
  }

  //! Alias for FindFromIndex.
  BRepMeshCol::ListOfInteger& operator()(const Standard_Integer theIndex) const
  {
    return FindFromIndex(theIndex);
  }

  //! Returns vertex by the given index.
  inline const BRepMesh_Vertex& FindKey(const Standard_Integer theIndex)
  {
    return mySelector.GetVertex(theIndex);
  }

  //! Returns index of the given vertex.
  Standard_EXPORT Standard_Integer FindIndex(const BRepMesh_Vertex& theVertex)
  {
    mySelector.SetPoint(theVertex.Coord());
    myCellFilter.Inspect (theVertex.Coord(), mySelector);
    return mySelector.GetCoincidentPoint();
  }

  //! Returns a number of vertices.
  inline Standard_Integer Extent() const
  {
    return mySelector.NbVertices();
  }

  //! Returns True when the map contains no keys. <br>
  inline Standard_Boolean IsEmpty() const
  {
    return (Extent() == 0);
  }

  //! Substitutes vertex with the given by the given vertex with attributes.
  //! \param theIndex index of vertex to be substituted.
  //! \param theVertex replacement vertex.
  //! \param theData data associated to the vertex.
  Standard_EXPORT void Substitute(const Standard_Integer            theIndex,
                                  const BRepMesh_Vertex&            theVertex,
                                  const BRepMeshCol::ListOfInteger& theData);

  //! Remove last node from the structure.
  inline void RemoveLast()
  {
    Delete(Extent());
  }

  //! Returns the list with indexes of vertices that have movability attribute
  //! equal to BRepMesh_Deleted and can be replaced with another node.
  inline const BRepMeshCol::ListOfInteger& GetListOfDelNodes() const
  {
    return mySelector.GetListOfDelPoints();
  }

  //! Prints statistics.
  Standard_EXPORT void Statistics(Standard_OStream& theStream) const;

private:
  
  //! Expands the given point according to specified tolerance.
  //! \param thePoint point to be expanded.
  //! \param[out] theMinPoint bottom left corner of area defined by expanded point.
  //! \param[out] theMaxPoint top right corner of area defined by expanded point.
  inline void expandPoint(const gp_XY& thePoint,
                          gp_XY&       theMinPoint,
                          gp_XY&       theMaxPoint)
  {
    theMinPoint.SetX(thePoint.X() - myTolerance[0]);
    theMinPoint.SetY(thePoint.Y() - myTolerance[1]);
    theMaxPoint.SetX(thePoint.X() + myTolerance[0]);
    theMaxPoint.SetY(thePoint.Y() + myTolerance[1]);
  }

private:

  BRepMeshCol::Allocator                     myAllocator;
  BRepMeshCol::VertexCellFilter              myCellFilter;
  BRepMesh_VertexInspector                   mySelector;
  BRepMeshCol::DMapOfIntegerListOfInteger    myLinksMap;
  Standard_Real                              myTolerance[2];
};

#endif
