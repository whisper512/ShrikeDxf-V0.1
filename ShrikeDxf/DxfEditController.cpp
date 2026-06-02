#include "DxfEditController.h"
#include "DxfSelectionController.h"


CDxfEditController::CDxfEditController(CDxfData* pData, CDxfGraphicsScene* pScene, QObject* parent)
    : QObject(parent), m_pData(pData), m_pScene(pScene)
{
}

CDxfEditController::~CDxfEditController() {}
