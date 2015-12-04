// Created on: 1996-12-20
// Created by: Robert COUBLANC
// Copyright (c) 1996-1999 Matra Datavision
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

#ifndef _AIS_Shape_HeaderFile
#define _AIS_Shape_HeaderFile

#include <Standard.hxx>
#include <Standard_Type.hxx>

#include <TopoDS_Shape.hxx>
#include <Bnd_Box.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Real.hxx>
#include <AIS_InteractiveObject.hxx>
#include <Standard_Integer.hxx>
#include <AIS_KindOfInteractive.hxx>
#include <Prs3d_TypeOfHLR.hxx>
#include <PrsMgr_PresentationManager3d.hxx>
#include <SelectMgr_Selection.hxx>
#include <Quantity_NameOfColor.hxx>
#include <Graphic3d_NameOfMaterial.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <Prs3d_Drawer.hxx>
class TopoDS_Shape;
class Prs3d_Presentation;
class Prs3d_Projector;
class Geom_Transformation;
class Quantity_Color;
class Graphic3d_MaterialAspect;
class Bnd_Box;


class AIS_Shape;
DEFINE_STANDARD_HANDLE(AIS_Shape, AIS_InteractiveObject)

//! A framework to manage presentation and selection of shapes.
//! AIS_Shape is the interactive object which is used the
//! most by   applications. There are standard functions
//! available which allow you to prepare selection
//! operations on the constituent elements of shapes -
//! vertices, edges, faces etc - in an open local context.
//! The selection modes specific to "Shape" type objects
//! are referred to as Standard Activation Mode. These
//! modes are only taken into account in open local
//! context and only act on Interactive Objects which
//! have redefined the virtual method
//! AcceptShapeDecomposition so that it returns true.
//! Several advanced functions are also available. These
//! include functions to manage deviation angle and
//! deviation coefficient - both HLR and non-HLR - of
//! an inheriting shape class. These services allow you to
//! select one type of shape interactive object for higher
//! precision drawing. When you do this, the
//! Prs3d_Drawer::IsOwn... functions corresponding to the
//! above deviation angle and coefficient functions return
//! true indicating that there is a local setting available
//! for the specific object.
class AIS_Shape : public AIS_InteractiveObject
{

public:

  
  //! Initializes construction of the shape shap from wires,
  //! edges and vertices.
  Standard_EXPORT AIS_Shape(const TopoDS_Shape& shap);
  
  //! Returns index 0. This value refers to SHAPE from TopAbs_ShapeEnum
  Standard_EXPORT virtual Standard_Integer Signature() const Standard_OVERRIDE;
  
  //! Returns Object as the type of Interactive Object.
  Standard_EXPORT virtual AIS_KindOfInteractive Type() const Standard_OVERRIDE;
  
  //! Returns true if the Interactive Object accepts shape decomposition.
  Standard_EXPORT virtual Standard_Boolean AcceptShapeDecomposition() const Standard_OVERRIDE;
  
  //! Constructs an instance of the shape object ashape.
    void Set (const TopoDS_Shape& ashap);
  
  //! Returns this shape object.
    const TopoDS_Shape& Shape() const;
  
  //! Sets a local value for deviation coefficient for this specific shape.
  Standard_EXPORT Standard_Boolean SetOwnDeviationCoefficient();
  
  //! Sets a local value for HLR deviation coefficient for this specific shape.
  Standard_EXPORT Standard_Boolean SetOwnHLRDeviationCoefficient();
  
  //! Sets a local value for deviation angle for this specific shape.
  Standard_EXPORT Standard_Boolean SetOwnDeviationAngle();
  
  //! Sets a local value for HLR deviation angle for this specific shape.
  Standard_EXPORT Standard_Boolean SetOwnHLRDeviationAngle();
  
  //! Sets a local value for deviation coefficient for this specific shape.
  Standard_EXPORT void SetOwnDeviationCoefficient (const Standard_Real aCoefficient);
  
  //! sets myOwnHLRDeviationCoefficient field in Prs3d_Drawer &
  //! recomputes presentation
  Standard_EXPORT void SetOwnHLRDeviationCoefficient (const Standard_Real aCoefficient);
  
  //! this compute a new angle and Deviation from the value anAngle
  //! and set the values stored in myDrawer with these that become local to the shape
  Standard_EXPORT void SetAngleAndDeviation (const Standard_Real anAngle);
  
  //! gives back the angle initial value put by the User.
  Standard_EXPORT Standard_Real UserAngle() const;
  
  //! sets myOwnDeviationAngle field in Prs3d_Drawer & recomputes presentation
  Standard_EXPORT void SetOwnDeviationAngle (const Standard_Real anAngle);
  
  //! this compute a new Angle and Deviation from the value anAngle for HLR
  //! and set the values stored in myDrawer for with these that become local to the shape
  Standard_EXPORT void SetHLRAngleAndDeviation (const Standard_Real anAngle);
  
  //! sets myOwnHLRDeviationAngle field in Prs3d_Drawer & recomputes presentation
  Standard_EXPORT void SetOwnHLRDeviationAngle (const Standard_Real anAngle);
  
  //! Returns true and the values of the deviation
  //! coefficient aCoefficient and the previous deviation
  //! coefficient aPreviousCoefficient. If these values are
  //! not already set, false is returned.
  Standard_EXPORT Standard_Boolean OwnDeviationCoefficient (Standard_Real& aCoefficient, Standard_Real& aPreviousCoefficient) const;
  
  //! Returns   true and the values of the HLR deviation
  //! coefficient aCoefficient and the previous HLR
  //! deviation coefficient aPreviousCoefficient. If these
  //! values are not already set, false is returned.
  Standard_EXPORT Standard_Boolean OwnHLRDeviationCoefficient (Standard_Real& aCoefficient, Standard_Real& aPreviousCoefficient) const;
  
  //! Returns true and the values of the deviation angle
  //! anAngle and the previous deviation angle aPreviousAngle.
  //! If these values are not already set, false is returned.
  Standard_EXPORT Standard_Boolean OwnDeviationAngle (Standard_Real& anAngle, Standard_Real& aPreviousAngle) const;
  
  //! Returns true and the values   of the HLR deviation
  //! angle anAngle and of the previous HLR deviation
  //! angle aPreviousAngle. If these values are not
  //! already set, false is returned.
  Standard_EXPORT Standard_Boolean OwnHLRDeviationAngle (Standard_Real& anAngle, Standard_Real& aPreviousAngle) const;
  
  //! Sets the type of HLR algorithm used by the shape
    void SetTypeOfHLR (const Prs3d_TypeOfHLR theTypeOfHLR);
  
  //! Gets the type of HLR algorithm
    Prs3d_TypeOfHLR TypeOfHLR() const;
  
  //! Sets the color aColor in the reconstructed
  //! compound shape. Acts via the Drawer methods below on the appearance of:
  //! -   free boundaries:
  //! Prs3d_Drawer_FreeBoundaryAspect,
  //! -   isos: Prs3d_Drawer_UIsoAspect,
  //! Prs3dDrawer_VIsoAspect,
  //! -   shared boundaries:
  //! Prs3d_Drawer_UnFreeBoundaryAspect,
  //! -   shading: Prs3d_Drawer_ShadingAspect,
  //! -   visible line color in hidden line mode:
  //! Prs3d_Drawer_SeenLineAspect
  //! -   hidden line color in hidden line mode:
  //! Prs3d_Drawer_HiddenLineAspect.
  Standard_EXPORT virtual void SetColor (const Quantity_NameOfColor aColor) Standard_OVERRIDE;
  
  Standard_EXPORT virtual void SetColor (const Quantity_Color& aColor) Standard_OVERRIDE;
  
  //! Removes settings for color in the reconstructed compound shape.
  Standard_EXPORT virtual void UnsetColor() Standard_OVERRIDE;
  
  //! Sets the value aValue for line width in the reconstructed compound shape.
  //! Changes line aspects for lines-only presentation modes like Wireframe and Bounding Box.
  //! Doesn't change face boundary line aspect.
  Standard_EXPORT virtual void SetWidth (const Standard_Real aValue) Standard_OVERRIDE;
  
  //! Removes the setting for line width in the reconstructed compound shape.
  Standard_EXPORT virtual void UnsetWidth() Standard_OVERRIDE;
  
  Standard_EXPORT virtual void SetMaterial (const Graphic3d_NameOfMaterial aName) Standard_OVERRIDE;
  
  //! Allows you to provide settings for the material aName
  //! in the reconstructed compound shape.
  Standard_EXPORT virtual void SetMaterial (const Graphic3d_MaterialAspect& aName) Standard_OVERRIDE;
  
  //! Removes settings for material in the reconstructed compound shape.
  Standard_EXPORT virtual void UnsetMaterial() Standard_OVERRIDE;
  
  //! Sets the value aValue for transparency in the reconstructed compound shape.
  Standard_EXPORT virtual void SetTransparency (const Standard_Real aValue = 0.6) Standard_OVERRIDE;
  
  //! Removes the setting for transparency in the reconstructed compound shape.
  Standard_EXPORT virtual void UnsetTransparency() Standard_OVERRIDE;
  
  //! Constructs a bounding box with which to reconstruct
  //! compound topological shapes for presentation.
  Standard_EXPORT virtual const Bnd_Box& BoundingBox();
  
  //! Returns the NameOfColor attributes of the shape accordingly to
  //! the current facing model;
  Standard_EXPORT virtual Quantity_NameOfColor Color() const Standard_OVERRIDE;
  
  //! Returns the Color attributes of the shape accordingly to
  //! the current facing model;
  Standard_EXPORT virtual void Color (Quantity_Color& aColor) const Standard_OVERRIDE;
  
  //! Returns the NameOfMaterial attributes of the shape accordingly to
  //! the current facing model;
  Standard_EXPORT virtual Graphic3d_NameOfMaterial Material() const Standard_OVERRIDE;
  
  //! Returns the transparency attributes of the shape accordingly to
  //! the current facing model;
  Standard_EXPORT virtual Standard_Real Transparency() const Standard_OVERRIDE;
  
  //! Activates the same TopAbs shape enumerations as
  //! those used by SelectionMode assigning a type to the mode aDecompositionMode.
  Standard_EXPORT static TopAbs_ShapeEnum SelectionType (const Standard_Integer aDecompositionMode);
  
  //! Establishes an equivalence between a mode and the
  //! type, aShapeType,   of selection. The correspondences are as follows:
  //! -   mode 0 - Shape
  //! -   mode 1 - Vertex
  //! -   mode 2 - Edge
  //! -   mode 3 - Wire
  //! -   mode 4 - Face
  //! -   mode 5 - Shell
  //! -   mode 6 - Solid
  //! -   mode 7 - Compsolid
  //! -   mode 8 - Compound
  Standard_EXPORT static Standard_Integer SelectionMode (const TopAbs_ShapeEnum aShapeType);




  DEFINE_STANDARD_RTTIEXT(AIS_Shape,AIS_InteractiveObject)

protected:

  
  Standard_EXPORT virtual void Compute (const Handle(PrsMgr_PresentationManager3d)& aPresentationManager, const Handle(Prs3d_Presentation)& aPresentation, const Standard_Integer aMode = 0) Standard_OVERRIDE;
  
  Standard_EXPORT virtual void Compute (const Handle(Prs3d_Projector)& aProjector, const Handle(Prs3d_Presentation)& aPresentation) Standard_OVERRIDE;
  
  Standard_EXPORT virtual void Compute (const Handle(Prs3d_Projector)& aProjector, const Handle(Geom_Transformation)& aTrsf, const Handle(Prs3d_Presentation)& aPresentation) Standard_OVERRIDE;
  
  Standard_EXPORT virtual void ComputeSelection (const Handle(SelectMgr_Selection)& aSelection, const Standard_Integer aMode) Standard_OVERRIDE;
  
  Standard_EXPORT void LoadRecomputable (const Standard_Integer TheMode);
  
  Standard_EXPORT void setColor (const Handle(Prs3d_Drawer)& theDrawer, const Quantity_Color& theColor) const;
  
  Standard_EXPORT void setWidth (const Handle(Prs3d_Drawer)& theDrawer, const Standard_Real theWidth) const;
  
  Standard_EXPORT void setTransparency (const Handle(Prs3d_Drawer)& theDrawer, const Standard_Real theValue) const;
  
  Standard_EXPORT void setMaterial (const Handle(Prs3d_Drawer)& theDrawer, const Graphic3d_MaterialAspect& theMaterial, const Standard_Boolean theToKeepColor, const Standard_Boolean theToKeepTransp) const;

  TopoDS_Shape myshape;
  Bnd_Box myBB;
  Standard_Boolean myCompBB;


private:

  
  Standard_EXPORT void Compute (const Handle(Prs3d_Projector)& aProjector, const Handle(Prs3d_Presentation)& aPresentation, const TopoDS_Shape& ashape);

  Standard_Real myInitAng;


};


#include <AIS_Shape.lxx>





#endif // _AIS_Shape_HeaderFile
