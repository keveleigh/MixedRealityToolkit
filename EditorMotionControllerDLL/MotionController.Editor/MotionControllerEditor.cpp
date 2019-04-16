// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"
#include "MotionController.h"
#include "MotionControllerEditor.h"

SpatialInteractionManager TryGetSpatialInteractionManager()
{
	winrt::com_ptr<ISpatialInteractionManagerInterop> spatialInteractionManagerInterop =
		winrt::get_activation_factory<SpatialInteractionManager, ISpatialInteractionManagerInterop>();

	winrt::com_ptr<ABI::Windows::UI::Input::Spatial::ISpatialInteractionManager> spSpatialInteractionManager;
	winrt::check_hresult(spatialInteractionManagerInterop->GetForWindow(FindWindow(L"UnityHoloInEditorWndClass", NULL), __uuidof(ABI::Windows::UI::Input::Spatial::ISpatialInteractionManager), winrt::put_abi(spSpatialInteractionManager)));

	return spSpatialInteractionManager.as<SpatialInteractionManager>();
}
