#pragma once
#include "EntityBase.h"

// ─── Spline ─────────────────────────────────────────────────
struct EntitySpline
{
    EntityProp prop;
    Vertex3D   normalVec;                          //法向量
    Vertex3D   tgStart, tgEnd;                     //起点切向向量,终点切向向量
    int     flags = 0;
    int     degree = 3;                            //样条阶数
    double  knotTolerance = 0.0000001;             //节点容差
    double  controlTolerance = 0.0000001;          //控制点容差
    double  fitTolerance = 0.0000001;              //拟合点容差

    std::vector<double>      knots;                //节点矢量
    std::vector<double>      weights;              //权重值
    std::vector<Vertex3D>    controlPoints;        //控制点
    std::vector<Vertex3D>    fitPoints;            //拟合点
};
