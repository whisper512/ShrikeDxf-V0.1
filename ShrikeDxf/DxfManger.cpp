#include "DxfManger.h"
#include "CommonDataManger.h"
#include <QMessageBox>



CDxfManger::CDxfManger(QWidget* pMainWnd) 
    : m_pMainWnd(pMainWnd)
    , m_DxfReader(nullptr)
    , m_DxfData(nullptr)
{
    m_DxfData = std::make_unique<CDxfData>();
    m_DxfReader = std::make_unique<CDxfReader>(m_DxfData.get());
}

CDxfManger::~CDxfManger()
{
    
}

bool CDxfManger::LoadDxfFile(const QString& strPath)
{
    // 确保 Reader 持有 Data 指针
    m_DxfReader->SetDataTarget(m_DxfData.get());
    // 执行读取
    bool ok = m_DxfReader->ReadFile(strPath);
    if (!ok) {
        QMessageBox::warning(m_pMainWnd, QStringLiteral("打开失败"),
            QStringLiteral("无法打开或解析文件:\n%1").arg(strPath));
        return false;
    }

    // 更新treeview
    m_DxfTreeviewModel.UpdateLayoutItemModel(m_DxfData->GetLayers());
    emit signalRefreshTreeview(&m_DxfTreeviewModel);
    emit signalRefreshTreeviewAfterRead();
    // 更新graphicsview

    return true;
}

bool CDxfManger::SaveDxfFile(const QString& strPath)
{


    return true;
}

bool CDxfManger::NewDxfFile()
{

    return true;
}

bool CDxfManger::CloseDxfFile()
{
    m_DxfTreeviewModel.clear();
    m_DxfGraphicsScene.clear();
    m_DxfLayerTableviewModel.clear();
    return false;
}
