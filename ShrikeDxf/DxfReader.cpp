#include <QtMath>
#include "DxfReader.h"

CDxfReader::CDxfReader()
{
}

CDxfReader::~CDxfReader()
{
	
}

void CDxfReader::addHeader(const DRW_Header* data)
{
}

void CDxfReader::addLType(const DRW_LType& data)
{
}

void CDxfReader::addLayer(const DRW_Layer& data)
{
	
}

void CDxfReader::addDimStyle(const DRW_Dimstyle& data)
{
}

void CDxfReader::addVport(const DRW_Vport& data)
{
}

void CDxfReader::addTextStyle(const DRW_Textstyle& data)
{
}

void CDxfReader::addAppId(const DRW_AppId& data)
{
}

void CDxfReader::addBlock(const DRW_Block& data)
{
}

void CDxfReader::setBlock(const int handle)
{
}

void CDxfReader::endBlock()
{
}

void CDxfReader::addPoint(const DRW_Point& data)
{
	
}

void CDxfReader::addLine(const DRW_Line& data)
{
	
}

void CDxfReader::addRay(const DRW_Ray& data)
{
}

void CDxfReader::addXline(const DRW_Xline& data)
{
}

void CDxfReader::addCircle(const DRW_Circle& data)
{
	
}

void CDxfReader::addEllipse(const DRW_Ellipse& data)
{
}

void CDxfReader::addArc(const DRW_Arc& data)
{
	
}

void CDxfReader::addMText(const DRW_MText& data)
{
}

void CDxfReader::addText(const DRW_Text& data)
{
	
}

void CDxfReader::addDimAlign(const DRW_DimAligned* data)
{
}

void CDxfReader::addDimLinear(const DRW_DimLinear* data)
{
}

void CDxfReader::addDimRadial(const DRW_DimRadial* data)
{
}

void CDxfReader::addDimDiametric(const DRW_DimDiametric* data)
{
}

void CDxfReader::addDimAngular(const DRW_DimAngular* data)
{
}

void CDxfReader::addDimAngular3P(const DRW_DimAngular3p* data)
{
}

void CDxfReader::addDimOrdinate(const DRW_DimOrdinate* data)
{
}

void CDxfReader::addLeader(const DRW_Leader* data)
{
}

void CDxfReader::addHatch(const DRW_Hatch* data)
{
}

void CDxfReader::addViewport(const DRW_Viewport& data)
{
}

void CDxfReader::addImage(const DRW_Image* data)
{
}

void CDxfReader::linkImage(const DRW_ImageDef* data)
{
}

void CDxfReader::addComment(const char* comment)
{
}

void CDxfReader::addPlotSettings(const DRW_PlotSettings* data)
{
}

void CDxfReader::writeHeader(DRW_Header& data)
{
}

void CDxfReader::writeBlocks()
{
}

void CDxfReader::writeBlockRecords()
{
}

void CDxfReader::writeEntities()
{
}

void CDxfReader::writeLTypes()
{
}

void CDxfReader::writeLayers()
{
}

void CDxfReader::writeTextstyles()
{
}

void CDxfReader::writeVports()
{
}

void CDxfReader::writeDimstyles()
{
}

void CDxfReader::writeObjects()
{
}

void CDxfReader::writeAppId()
{
}

void CDxfReader::addLWPolyline(const DRW_LWPolyline& data)
{
	
}

void CDxfReader::addPolyline(const DRW_Polyline& data)
{
	
}

void CDxfReader::addSpline(const DRW_Spline* data)
{
}

void CDxfReader::addKnot(const DRW_Entity& data)
{
}

void CDxfReader::addInsert(const DRW_Insert& data)
{
}

void CDxfReader::addTrace(const DRW_Trace& data)
{
}

void CDxfReader::add3dFace(const DRW_3Dface& data)
{
}

void CDxfReader::addSolid(const DRW_Solid& data)
{
}

