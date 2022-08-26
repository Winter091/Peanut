#pragma once

#include "2D/Render/Renderer2D.hpp"
#include "2D/Shapes/Rectangle.hpp"
#include "2D/Shapes/Shape.hpp"

#include "Application/Application.hpp"
#include "Application/CommandLineArgs.hpp"

#include "Core/Utils/FileUtils.hpp"
#include "Core/Assert.hpp"
#include "Core/KeyCodes.hpp"
#include "Core/Log.hpp"
#include "Core/MouseCodes.hpp"
#include "Core/StoragePath.hpp"
#include "Core/TimeProfiler.hpp"

#include "Events/Event.hpp"
#include "Events/EventQueue.hpp"
#include "Events/EventType.hpp"
#include "Events/KeyEvents.hpp"
#include "Events/MouseEvents.hpp"
#include "Events/WindowEvents.hpp"

#include "Render/Buffers/Buffer.hpp"
#include "Render/Buffers/BufferLayout.hpp"
#include "Render/Buffers/BufferMapAccess.hpp"
#include "Render/Buffers/ConstantBuffer.hpp"
#include "Render/Buffers/IndexBuffer.hpp"
#include "Render/Buffers/VertexArray.hpp"
#include "Render/Buffers/VertexBuffer.hpp"
#include "Render/Commands/RenderCommand.hpp"
#include "Render/Shaders/Shader.hpp"
#include "Render/Shaders/ShaderInputLayout.hpp"
#include "Render/Textures/Texture.hpp"
#include "Render/Textures/Texture2D.hpp"
#include "Render/Textures/TextureLoad.hpp"
#include "Render/Textures/TextureSampler.hpp"
#include "Render/RenderAPI.hpp"

#include "Scene/Camera/Camera.hpp"
#include "Scene/Camera/OrthoCamera.hpp"

#include "Window/RenderContext.hpp"
#include "Window/Window.hpp"
#include "Window/WindowProvider.hpp"
#include "Window/WindowSettings.hpp"
