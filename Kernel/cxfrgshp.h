// $Id: cxfrgshp.h 751 2006-03-31 15:43:49Z alex $
/* @@tag:xara-cn@@ DO NOT MODIFY THIS LINE
================================XARAHEADERSTART===========================
 
               Xara LX, a vector drawing and manipulation program.
                    Copyright (C) 1993-2006 Xara Group Ltd.
       Copyright on certain contributions may be held in joint with their
              respective authors. See AUTHORS file for details.

LICENSE TO USE AND MODIFY SOFTWARE
----------------------------------

This file is part of Xara LX.

Xara LX is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as published
by the Free Software Foundation.

Xara LX and its component source files are distributed in the hope
that it will be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with Xara LX (see the file GPL in the root directory of the
distribution); if not, write to the Free Software Foundation, Inc., 51
Franklin St, Fifth Floor, Boston, MA  02110-1301 USA


ADDITIONAL RIGHTS
-----------------

Conditional upon your continuing compliance with the GNU General Public
License described above, Xara Group Ltd grants to you certain additional
rights. 

The additional rights are to use, modify, and distribute the software
together with the wxWidgets library, the wxXtra library, and the "CDraw"
library and any other such library that any version of Xara LX relased
by Xara Group Ltd requires in order to compile and execute, including
the static linking of that library to XaraLX. In the case of the
"CDraw" library, you may satisfy obligation under the GNU General Public
License to provide source code by providing a binary copy of the library
concerned and a copy of the license accompanying it.

Nothing in this section restricts any of the rights you have under
the GNU General Public License.


SCOPE OF LICENSE
----------------

This license applies to this program (XaraLX) and its constituent source
files only, and does not necessarily apply to other Xara products which may
in part share the same code base, and are subject to their own licensing
terms.

This license does not apply to files in the wxXtra directory, which
are built into a separate library, and are subject to the wxWindows
license contained within that directory in the file "WXXTRA-LICENSE".

This license does not apply to the binary libraries (if any) within
the "libs" directory, which are subject to a separate license contained
within that directory in the file "LIBS-LICENSE".


ARRANGEMENTS FOR CONTRIBUTION OF MODIFICATIONS
----------------------------------------------

Subject to the terms of the GNU Public License (see above), you are
free to do whatever you like with your modifications. However, you may
(at your option) wish contribute them to Xara's source tree. You can
find details of how to do this at:
  http://www.xaraxtreme.org/developers/

Prior to contributing your modifications, you will need to complete our
contributor agreement. This can be found at:
  http://www.xaraxtreme.org/developers/contribute/

Please note that Xara will not accept modifications which modify any of
the text between the start and end of this header (marked
XARAHEADERSTART and XARAHEADEREND).


MARKS
-----

Xara, Xara LX, Xara X, Xara X/Xtreme, Xara Xtreme, the Xtreme and Xara
designs are registered or unregistered trademarks, design-marks, and/or
service marks of Xara Group Ltd. All rights in these marks are reserved.


      Xara Group Ltd, Gaddesden Place, Hemel Hempstead, HP2 6EX, UK.
                        http://www.xara.com/

=================================XARAHEADEREND============================
 */
/********************************************************************************************

>	cxfrgshp.h

	Author:		Andy_Hayward (Xara Group Ltd) <camelotdev@xara.com>
	Created:	04/06/96
	Purpose:	Abstract Base class for exporting regular shapes

********************************************************************************************/

#ifndef INC_CXF_REGULAR_SHAPE
#define INC_CXF_REGULAR_SHAPE

/********************************************************************************************

>	class CXaraFileRegularShape

	Author:		Andy_Hayward (Xara Group Ltd) <camelotdev@xara.com>
	Created:	04/06/96
	Base Class:	-
	Purpose:	Abstract class holding importing and exporting code for regular
				shapes to and from the new file format
	SeeAlso:	class NodeRegularShape
				(friend class of NodeRegularShape)

********************************************************************************************/

class CXaraFileRegularShape
{
	// this is an abstract class, so no object of this class will (should) be created
public:
	static BOOL WritePreChildrenNative(BaseCamelotFilter * pFilter, NodeRegularShape *pShape);
	static BOOL WritePreChildrenWeb(BaseCamelotFilter * pFilter, NodeRegularShape *pShape);

protected:
	static BOOL WriteShapeNative(BaseCamelotFilter * pFilter, NodeRegularShape *pShape);
	static BOOL WriteShapeWeb(BaseCamelotFilter *pFilter, NodeRegularShape *pShape);

	static BOOL FixOldShape(NodeRegularShape *pShape);

	static BOOL WriteShapeGeneral(BaseCamelotFilter * pFilter, NodeRegularShape *pShape);
	static BOOL WriteShapeInvalid(BaseCamelotFilter * pFilter, NodeRegularShape *pShape);


// other functions
public:
	static BOOL IsEllipse(NodeRegularShape *pShape);
	static BOOL IsRectangle(NodeRegularShape *pShape);
	static BOOL IsSimple(NodeRegularShape *pShape);
	static BOOL IsComplex(NodeRegularShape *pShape);
	static BOOL IsRounded(NodeRegularShape *pShape);
	static BOOL IsStellated(NodeRegularShape *pShape);
	static BOOL IsReformed(NodeRegularShape *pShape);

protected:
	static BYTE GetFlags(NodeRegularShape *pShape);

	static DocCoord GetCentrePoint(NodeRegularShape *pShape);
	static DocCoord GetMajorAxis(NodeRegularShape *pShape);
	static DocCoord GetMinorAxis(NodeRegularShape *pShape);
	static DocCoord GetUTCentrePoint(NodeRegularShape *pShape);
	static DocCoord GetUTMajorAxis(NodeRegularShape *pShape);
	static DocCoord GetUTMinorAxis(NodeRegularShape *pShape);
	
	static INT32 GetHeight(NodeRegularShape *pShape);
	static INT32 GetWidth(NodeRegularShape *pShape);

	static UINT16 GetNumberOfSides(NodeRegularShape *pShape);

	static double GetCurvature(NodeRegularShape *pShape);
	static double GetPrimaryCurvature(NodeRegularShape *pShape);
	static double GetSecondaryCurvature(NodeRegularShape *pShape);
	
	static double GetStellationRadius(NodeRegularShape *pShape);
	static double GetStellationOffset(NodeRegularShape *pShape);

	static Matrix GetTransformMatrix(NodeRegularShape *pShape);

	static Path * GetEdgePath(NodeRegularShape *pShape);
	static Path * GetPrimaryEdgePath(NodeRegularShape *pShape);
	static Path * GetSecondaryEdgePath(NodeRegularShape *pShape);

// member variables
protected:
	const static double mEpsilon;
};

#endif // include CXF_REGULAR_SHAPE
