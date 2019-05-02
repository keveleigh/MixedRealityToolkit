// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <winrt/Windows.UI.Input.Spatial.h>

using namespace winrt::Windows::UI::Input::Spatial;

SpatialInteractionManager TryGetSpatialInteractionManager();
SpatialInteractionSource GetSpatialInteractionSource(UINT32 controllerId);
