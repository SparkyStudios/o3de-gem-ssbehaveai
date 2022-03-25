// Copyright (c) 2021-present Sparky Studios. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <DetourDebugDraw.h>

#include <Navigation/Components/DynamicNavigationMeshEditorComponent.h>

namespace SparkyStudios::AI::Behave::Navigation
{
    void DynamicNavigationMeshEditorComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto* sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            DynamicNavigationMeshComponent::Reflect(rc);

            sc->Class<DynamicNavigationMeshEditorComponent, EditorComponentBase>()
                ->Field("debugDraw", &DynamicNavigationMeshEditorComponent::_enableDebug)
                ->Field("debugDepthTest", &DynamicNavigationMeshEditorComponent::_depthTest)
                ->Field("settings", &DynamicNavigationMeshEditorComponent::_settings);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                ec->Class<DynamicNavigationMeshComponent>(
                      "Dynamic Navigation Mesh", "Setup a navigation mesh to be dynamically built and updated at runtime.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &DynamicNavigationMeshComponent::_settings, "Settings", "Navigation Mesh Settings.")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);

                ec->Class<NavigationMeshSettings>("Dynamic Navigation Mesh Settings", "Settings to use when building the navigation mesh.")
                    ->ClassElement(AZ::Edit::ClassElements::Group, "Rasterization")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Slider, &NavigationMeshSettings::m_cellSize, "Cell Size",
                        "The xy-plane cell size to use for fields.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.1f)
                    ->Attribute(AZ::Edit::Attributes::Max, 1.0f)
                    ->Attribute(AZ::Edit::Attributes::Decimals, 2)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Slider, &NavigationMeshSettings::m_cellHeight, "Cell Height",
                        "The z-axis cell size to use for fields.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.1f)
                    ->Attribute(AZ::Edit::Attributes::Max, 1.0f)
                    ->Attribute(AZ::Edit::Attributes::Decimals, 2)

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Region")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &NavigationMeshSettings::m_regionMinSize, "Min Region Size",
                        "The minimum number of cells allowed to form isolated island areas.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &NavigationMeshSettings::m_regionMergedSize, "Merged Region Height",
                        "Any regions with a span count smaller than this value will, if possible, be merged with larger regions.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Partitioning")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::ComboBox, &NavigationMeshSettings::m_partitionType, "Partinion Type",
                        "Define how the regions should be partitioned.")
                    ->EnumAttribute(NavigationMeshPartitionType::Watershed, "Watershed")
                    ->EnumAttribute(NavigationMeshPartitionType::Monotone, "Monotone")
                    ->EnumAttribute(NavigationMeshPartitionType::Layers, "Layers")

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Filtering")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::CheckBox, &NavigationMeshSettings::m_filterLowHangingObstacles, "Low Hanging Obstacles",
                        "Allows the formation of walkable regions that will flow over low lying objects such as curbs, and up structures "
                        "such as stairways.")
                    ->DataElement(
                        AZ::Edit::UIHandlers::CheckBox, &NavigationMeshSettings::m_filterLedgeSpans, "Ledge Spans",
                        "A ledge is a span with one or more neighbors whose maximum is further away than the walkable climb value from the "
                        "current span's maximum. This filter removes the impact of the overestimation of conservative voxelization so the "
                        "resulting mesh will not have regions hanging in the air over ledges.")
                    ->DataElement(
                        AZ::Edit::UIHandlers::CheckBox, &NavigationMeshSettings::m_filterWalkableLowHeightSpans,
                        "Walkable Low Height Spans",
                        "For this filter, the clearance above the span is the distance from the span's maximum to the next higher span's "
                        "minimum. (Same grid column.)")

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Polygonization")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &NavigationMeshSettings::m_edgeMaxLength, "Max Edge Lenght",
                        "The maximum allowed length for contour edges along the border of the mesh.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &NavigationMeshSettings::m_edgeMaxError, "Max Edge Error",
                        "The maximum distance a simplfied contour's border edges should deviate the original raw contour.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &NavigationMeshSettings::m_maxVerticesPerPoly, "Mar Vertices per Polygon",
                        "The maximum number of vertices allowed for polygons generated during the contour to polygon conversion process.")
                    ->Attribute(AZ::Edit::Attributes::Min, 3)
                    ->Attribute(AZ::Edit::Attributes::Max, 12)

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Detail Mesh")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Slider, &NavigationMeshSettings::m_detailSampleDist, "Detail Sample Distance",
                        "Sets the sampling distance to use when generating the detail mesh.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->Attribute(AZ::Edit::Attributes::Max, 16)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Slider, &NavigationMeshSettings::m_detailSampleMaxError, "Max Sample Error",
                        "The maximum distance the detail mesh surface should deviate from heightfield data.")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->Attribute(AZ::Edit::Attributes::Max, 16)

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Tiling")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(
                        AZ::Edit::UIHandlers::CheckBox, &NavigationMeshSettings::m_enableTiling, "Enable Tiling",
                        "If enabled, the navigation mesh wiil be built separated in tiles.")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Slider, &NavigationMeshSettings::m_tileSize, "Tile Size",
                        "The width/height size of tile's on the xy-plane.")
                    ->Attribute(AZ::Edit::Attributes::Min, 16)
                    ->Attribute(AZ::Edit::Attributes::Max, 1024)
                    ->Attribute(AZ::Edit::Attributes::Step, 16)

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Stats")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, false)
                    ->UIElement(AZ::Edit::UIHandlers::Default, "Voxels", "Voxel Size")
                    ->Attribute(AZ::Edit::Attributes::ValueText, &NavigationMeshSettings::StatGetVoxelSize);

                ec->Class<DynamicNavigationMeshEditorComponent>(
                      "Dynamic Navigation Mesh", "Setup a navigation mesh to be dynamically built and updated at runtime.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "Behave AI")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c))
                    ->Attribute(AZ::Edit::Attributes::FixedComponentListIndex, 0)
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::NameLabelOverride, "Behave AI - Dynamic Navigation Mesh")

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Debug")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &DynamicNavigationMeshEditorComponent::_enableDebug, "Enable",
                        "Draw the navigation mesh.")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &DynamicNavigationMeshEditorComponent::_depthTest, "Depth Test",
                        "Enable the depth test while drawing the navigation mesh.")

                    ->ClassElement(AZ::Edit::ClassElements::Group, "")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default, &DynamicNavigationMeshEditorComponent::_settings, "Settings",
                        "Settings to use when building the navigation mesh.")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)

                    ->UIElement(AZ::Edit::UIHandlers::Button, "", "Build the navigation mesh with the current settings.")
                    ->Attribute(AZ::Edit::Attributes::ButtonText, "Build")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, &DynamicNavigationMeshEditorComponent::OnBuildNavigationMesh);
            }
        }

        if (auto* bc = azrtti_cast<AZ::BehaviorContext*>(rc))
        {
            bc->Enum<static_cast<int>(NavigationMeshPartitionType::Watershed)>("eNMPT_Watershed")
                ->Enum<static_cast<int>(NavigationMeshPartitionType::Monotone)>("eNMPT_Monotone")
                ->Enum<static_cast<int>(NavigationMeshPartitionType::Layers)>("eNMPT_Layers");
        }
    }

    void DynamicNavigationMeshEditorComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("DynamicNavigationMeshService"));
    }

    void DynamicNavigationMeshEditorComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("DynamicNavigationMeshService"));
    }

    void DynamicNavigationMeshEditorComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("AxisAlignedBoxShapeService"));
    }

    void DynamicNavigationMeshEditorComponent::GetDependentServices(
        [[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    DynamicNavigationMeshEditorComponent::~DynamicNavigationMeshEditorComponent()
    {
    }

    void DynamicNavigationMeshEditorComponent::Init()
    {
    }

    void DynamicNavigationMeshEditorComponent::Activate()
    {
        _currentEntityTransform = AZ::Transform::CreateIdentity();
        _navigationMesh = new RecastNavigationMesh(GetEntityId(), true);

        BehaveNavigationMeshNotificationBus::Handler::BusConnect(GetEntityId());
        LmbrCentral::ShapeComponentNotificationsBus::Handler::BusConnect(GetEntityId());
        AZ::TransformNotificationBus::Handler::BusConnect(GetEntityId());
        AzFramework::EntityDebugDisplayEventBus::Handler::BusConnect(GetEntityId());

        _navMeshComponent.Init();
        SyncSettings();
    }

    void DynamicNavigationMeshEditorComponent::Deactivate()
    {
        AzFramework::EntityDebugDisplayEventBus::Handler::BusDisconnect();
        AZ::TransformNotificationBus::Handler::BusDisconnect();
        LmbrCentral::ShapeComponentNotificationsBus::Handler::BusDisconnect();
        BehaveNavigationMeshNotificationBus::Handler::BusDisconnect();

        delete _navigationMesh;
    }

    void DynamicNavigationMeshEditorComponent::BuildGameEntity(AZ::Entity* entity)
    {
        AZ::SerializeContext* context = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(context, &AZ::ComponentApplicationBus::Events::GetSerializeContext);

        if (!context)
        {
            AZ_Error("BehaveAI", false, "Unable to get a serialize context from component application.");
            return;
        }

        SyncSettings();
        entity->AddComponent(context->CloneObject(&_navMeshComponent));
    }

    void DynamicNavigationMeshEditorComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

    int DynamicNavigationMeshEditorComponent::GetTickOrder()
    {
        return AZ::TICK_DEFAULT;
    }

    void DynamicNavigationMeshEditorComponent::OnNavigationMeshUpdated()
    {
        _waitingOnNavMeshBuild = false;
    }

    void DynamicNavigationMeshEditorComponent::OnShapeChanged(ShapeChangeReasons changeReason)
    {
        if (changeReason == ShapeChangeReasons::ShapeChanged)
            UpdateNavMeshAABB();
    }

    void DynamicNavigationMeshEditorComponent::OnTransformChanged(const AZ::Transform& /*local*/, const AZ::Transform& world)
    {
        _currentEntityTransform = world;
    }

    void DynamicNavigationMeshEditorComponent::DisplayEntityViewport(
        [[maybe_unused]] const AzFramework::ViewportInfo& viewportInfo, AzFramework::DebugDisplayRequests& debugDisplay)
    {
        if (_enableDebug && _navigationMesh->IsNavigationMeshReady() && _navigationMesh->GetNavigationMesh())
        {
            // only uniform scale is supported in physics so the debug visuals reflect this fact
            AZ::Transform worldFromLocalWithUniformScale = _currentEntityTransform;
            worldFromLocalWithUniformScale.SetUniformScale(worldFromLocalWithUniformScale.GetUniformScale());

            debugDisplay.PushMatrix(AZ::Transform::Identity());

            _debugDraw.SetEnableDepthTest(_depthTest);
            _debugDraw.SetDebugDisplayRequestsHandler(&debugDisplay);
            duDebugDrawNavMesh(&_debugDraw, *_navigationMesh->_navMesh, DU_DRAWNAVMESH_OFFMESHCONS);
            // duDebugDrawPolyMeshDetail(&_debugDraw, *_navigationMesh->_detailMesh);
            // duDebugDrawPolyMesh(&_debugDraw, *_navigationMesh->_polyMesh);
            // duDebugDrawCompactHeightfieldSolid(&_debugDraw, *_navigationMesh->_compactHeightField);

            debugDisplay.PopMatrix();
        }
    }

    void DynamicNavigationMeshEditorComponent::UpdateNavMeshAABB()
    {
        AZ::Transform transform;
        AZ::Aabb aabb;

        LmbrCentral::ShapeComponentRequestsBus::Event(
            GetEntityId(), &LmbrCentral::ShapeComponentRequestsBus::Events::GetTransformAndLocalBounds, transform, aabb);

        _settings.m_aabb = AZStd::move(aabb);
        SyncSettings();
    }

    void DynamicNavigationMeshEditorComponent::SyncSettings()
    {
        _navMeshComponent._settings = _settings;
    }

    AZ::Crc32 DynamicNavigationMeshEditorComponent::OnBuildNavigationMesh()
    {
        if (_waitingOnNavMeshBuild)
            return AZ::Edit::PropertyRefreshLevels::None;

        _waitingOnNavMeshBuild = true;
        _navigationMesh->BuildNavigationMesh(_settings);

        return AZ::Edit::PropertyRefreshLevels::EntireTree;
    }

    AZ::Crc32 DynamicNavigationMeshEditorComponent::GetBuildButtonState()
    {
        return _waitingOnNavMeshBuild ? AZ::Edit::PropertyVisibility::Hide : AZ::Edit::PropertyVisibility::Show;
    }

} // namespace SparkyStudios::AI::Behave::Navigation
