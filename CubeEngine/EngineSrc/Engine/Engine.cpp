#include "Engine.h"
#include "../Scene/SceneMgr.h"
#include "../Texture/TextureMgr.h"
#include "../Rendering/Renderer.h"
#include "../Game/ConsolePanel.h"

#include "../Event/EventMgr.h"
#include "time.h"
#include "EngineSrc/3D/Effect/EffectMgr.h"
#include "WorkerThreadSystem.h"
#include "AudioSystem/AudioSystem.h"
#include <windows.h>
#include "ScriptPy/ScriptPyMgr.h"
#include "2D/GUISystem.h"
#define CLOCKS_TO_MS(c) int((c * 1.0f)/CLOCKS_PER_SEC * 1000 + 0.5f)
#include "Collision/PhysicsMgr.h"
#include "Utility/log/Tlog.h"

namespace tzw {

Engine * Engine::m_instance = nullptr;

Engine *Engine::shared()
{
    if(!m_instance)
    {
        m_instance = new Engine();
    }
    return m_instance;
}

Engine::Engine(): m_winBackEnd(nullptr)
	{
		m_isEnableOutLine = false;
	}

	bool Engine::getIsEnableOutLine() const
{
    return m_isEnableOutLine;
}

void Engine::setIsEnableOutLine(bool isEnableOutLine)
{
	m_isEnableOutLine = isEnableOutLine;
}

void Engine::setClearColor(float r, float g, float b)
{
	RenderBackEnd::shared()->setClearColor(r, g, b);
}

void Engine::setUnlimitedCursor(bool enable)
{
	m_winBackEnd->setUnlimitedCursor(enable);
}

std::string Engine::getWorkingDirectory()
{
	int length = 32;
	char str[32];
	GetCurrentDirectory(length,str);
	return str;
}

int Engine::getMouseButton(int mouseButton)
{
	return m_winBackEnd->getMouseButton(mouseButton);
}

int Engine::getDrawCallCount()
{
    return m_drawCallCount;
}

void Engine::setDrawCallCount(int drawCallCount)
{
    m_drawCallCount = drawCallCount;
}

void Engine::resetDrawCallCount()
{
    m_drawCallCount = 0;
}

void Engine::increaseVerticesIndicesCount(int v, int i)
{
    m_verticesCount += v;
    m_indicesCount += i;
}

void Engine::resetVerticesIndicesCount()
{
    m_verticesCount = 0;
    m_indicesCount = 0;
}

std::string Engine::getFilePath(std::string path)
{
    return "./Res/" + path;
}

int Engine::getApplyRenderTime() const
{
    return m_applyRenderTime;
}

int Engine::getLogicUpdateTime() const
{
    return m_logicUpdateTime;
}

int Engine::getIndicesCount() const
{
    return m_indicesCount;
}

int Engine::getVerticesCount() const
{
    return m_verticesCount;
}


float Engine::deltaTime() const
{
    return m_deltaTime;
}


void Engine::update(float delta)
{
    m_deltaTime = delta;
    int logicBefore = clock();
	EventMgr::shared()->apply(delta);
	PhysicsMgr::shared()->stepSimulation(delta);
    Engine::shared()->delegate()->onUpdate(delta);
	
	
    SceneMgr::shared()->doVisit();
	resetDrawCallCount();
    m_logicUpdateTime = CLOCKS_TO_MS(clock() - logicBefore);
    int applyRenderBefore = clock();
    resetVerticesIndicesCount();
    Renderer::shared()->renderAll();
	AudioSystem::shared()->update();
	ScriptPyMgr::shared()->doScriptUpdate();
    m_applyRenderTime = CLOCKS_TO_MS(clock() - applyRenderBefore);

	
}

static void writeFunction(const char * str)
{
	ConsolePanel::shared()->AddLog(str);
    //g_GetCurrScene()->getConsolePanel()->print(str);
}

void Engine::onStart(int width,int height)
{
    Engine::shared()->setWindowWidth(width);
    Engine::shared()->setWindowHeight(height);
    Engine::shared()->initSingletons();
    TlogSystem::get()->setWriteFunc(writeFunction);
    Engine::shared()->delegate()->onStart();
	ScriptPyMgr::shared()->doScriptInit();
	GUISystem::shared()->initGUI();

	//WorkerThreadSystem::shared()->init();
}

float Engine::FPS()
{
    return 1.0f/deltaTime();
}

AppEntry *Engine::delegate() const
{
    return m_delegate;
}

void Engine::setDelegate(AppEntry *delegate)
{
    m_delegate = delegate;
}

void Engine::initSingletons()
{
    SceneMgr::shared()->init();
    TextureMgr::shared();
    Renderer::shared();
    EffectMgr::shared();
	ScriptPyMgr::shared()->init();
}

vec2 Engine::winSize()
{
    return vec2(m_windowWidth,m_windowHeight);
}

tzw::vec2 Engine::getMousePos()
{
	double x, y;
	m_winBackEnd->getMousePos(&x, &y);
	return vec2(x, y);
}

float Engine::windowHeight() const
{
    return m_windowHeight;
}

void Engine::setWindowHeight(float windowHeight)
{
    m_windowHeight = windowHeight;
}

float Engine::windowWidth() const
{
    return m_windowWidth;
}

void Engine::setWindowWidth(float windowWidth)
{
    m_windowWidth = windowWidth;
}

int Engine::run(int argc, char *argv[], AppEntry * delegate)
{
    Engine::shared()->setDelegate(delegate);
	Engine::shared()->m_winBackEnd = WindowBackEndMgr::shared()->getWindowBackEnd(TZW_WINDOW_GLFW);
	Engine::shared()->m_winBackEnd->prepare();
	Engine::shared()->m_winBackEnd->run();
    return 0;
}
} // namespace tzw

