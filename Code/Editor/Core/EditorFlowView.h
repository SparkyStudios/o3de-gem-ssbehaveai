#pragma once

#if !defined(Q_MOC_RUN)
#include <Editor/Application/precompiled.h>
#include <Editor/Core/EditorBase.h>
#include <Editor/Core/EditorFlowScene.h>
#endif

namespace SparkyStudios::AI::BehaviorTree::Editor::Core
{
    class EditorFlowView : public QtNodes::FlowView
    {
        Q_OBJECT

    public:
        AZ_CLASS_ALLOCATOR(EditorFlowView, AZ::SystemAllocator, 0);
        AZ_RTTI(EditorFlowView, "{a5514cc2-b6a1-4435-83d7-5fb914b763ea}", QtNodes::FlowView);

        EditorFlowView(EditorFlowScene* scene, QWidget* parent = Q_NULLPTR);
        virtual ~EditorFlowView() = default;
    };
} // namespace SparkyStudios::AI::BehaviorTree::Editor::Core