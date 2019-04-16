// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"
#include "MotionController.h"

SpatialInteractionManager TryGetSpatialInteractionManager()
{
	return SpatialInteractionManager::GetForCurrentView();
}
