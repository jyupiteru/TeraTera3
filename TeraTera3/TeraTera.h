/**
 * @file Template.h
 * @author jupiter ()
 * @brief Templateのルート、これらをいちいち呼び出す（探す）のは大変なので
 * @n 読み込み遅くなるならどうするか考える
 */

#pragma once

#include "ComSystem/ComSystem.h"

#include "System/CDirectInput.h"

#include "DebugLog/CDebugLog.h"

#include "Managers/ImGuiSystem/ImGuiHeaders.h"

#include "Managers/ResourceContainer/CContainer.h"

#include "Managers/SceneSystem/CSceneManager/CSceneManager.h"

#include "Managers/ShaderManager/CShaderManager.h"

#include "Managers/ShadowManager/CShadowManager.h"

#include "Managers/TextureManager/CTextureManager.h"

#include "Timer/CTimer.h"
