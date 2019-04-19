// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"
#include "MotionController.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Perception;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Input::Spatial;

EXTERN_C __declspec(dllexport) uint32_t TryGetMotionControllerModel()
{
	SpatialInteractionManager spatialInteractionManager = TryGetSpatialInteractionManager();

	if (spatialInteractionManager != nullptr)
	{
		// Get the current PerceptionTimestamp. This will be used to get the currently detected spatial sources.
		PerceptionTimestamp perceptionTimestamp = PerceptionTimestampHelper::FromHistoricalTargetTime(clock::now());

		// Get the currently detected spatial sources.
		auto sources = spatialInteractionManager.GetDetectedSourcesAtTimestamp(perceptionTimestamp);

		return sources.Size();
	}

	return 1000;
}

EXTERN_C __declspec(dllexport) BOOL TryGetMotionControllerModel(_In_ UINT32 controllerId, _Out_ UINT32& outputSize, _Outptr_result_bytebuffer_(outputSize) BYTE& outputBuffer)
{
    SpatialInteractionSource source = GetSpatialInteractionSource(controllerId);

    if (source != nullptr)
    {
        // Start getting the renderable model stream asynchronously.
        IAsyncOperation<IRandomAccessStreamWithContentType> modelOperation;
        try
        {
            modelOperation = source.Controller.TryGetRenderableModelAsync();
        }
        catch (...)
        {
            return FALSE;
        }

        IRandomAccessStreamWithContentType stream = modelOperation.get();

        // If the model call failed or the resulting stream is empty, return.
        if (stream == nullptr || stream.Size == 0)
        {
            return FALSE;
        }

        // Create a buffer from the stream, to read the contents into a byte array to pass back to the app.
        unsigned int streamSize = (unsigned int)stream.Size;
		IAsyncOperationWithProgress<IBuffer, uint32_t> readOperation = stream.ReadAsync(Buffer(streamSize), streamSize, InputStreamOptions::None);

        IBuffer buffer = readOperation.get();

		// If the model call failed or the resulting stream is empty, return.
		if (buffer == nullptr || buffer.Length == 0)
		{
			return FALSE;
		}

        //if (status != task_status::completed)
        //{
        //    return FALSE;
        //}

        outputSize = buffer.Length;

        // Now, create a DataReader from the buffer, which can then transfer the bytes into a byte array.
        DataReader reader = DataReader::FromBuffer(buffer);
		std::array<BYTE, buffer.Length> test;
        outputBuffer = test;
		reader.ReadBytes({ outputBuffer });

        return TRUE;
    }

    return FALSE;
}

SpatialInteractionSource GetSpatialInteractionSource(UINT32 controllerId)
{
	SpatialInteractionManager spatialInteractionManager = TryGetSpatialInteractionManager();

    if (spatialInteractionManager != nullptr)
    {
        // Get the current PerceptionTimestamp. This will be used to get the currently detected spatial sources.
        PerceptionTimestamp perceptionTimestamp = PerceptionTimestampHelper::FromHistoricalTargetTime(clock::now());

        // Get the currently detected spatial sources.
        auto sources = spatialInteractionManager.GetDetectedSourcesAtTimestamp(perceptionTimestamp);

        // Iterate through the detected sources and check against the controller ID that was passed in.
        for (unsigned int i = 0; i < sources.Size(); i++)
        {
            if (sources.GetAt(i).Source().Id() == controllerId)
            {
                return sources.GetAt(i).Source();
            }
        }
    }

    return nullptr;
}