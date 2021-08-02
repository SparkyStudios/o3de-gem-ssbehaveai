#include <Editor/Application/precompiled.h>

#include <Editor/Core/EditorFlowView.h>

namespace SparkyStudios::AI::BehaviorTree::Editor::Core
{
    EditorFlowView::EditorFlowView(EditorFlowScene* scene, QWidget* parent)
        : QtNodes::FlowView(scene, parent)
    {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
        setDragMode(QGraphicsView::RubberBandDrag);
        setAlignment(Qt::AlignTop | Qt::AlignLeft);
        setCacheMode(QGraphicsView::CacheBackground);
    }
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Core