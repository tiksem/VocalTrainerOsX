
#include "VTEManager.h"
#include "GLSceneDrawer.h"
#include "VxFile.h"

namespace VTE
{
	VTEManager::VTEManager()
		: _glSceneDrawer(nullptr)
		, _vxPlayer(nullptr)
	{

	}

	VTEManager::~VTEManager()
	{
		GLSceneDrawer* sceneDrawerPtr = static_cast<GLSceneDrawer*>(_glSceneDrawer);
		if (sceneDrawerPtr)
			delete sceneDrawerPtr;

		if (_vxPlayer)
			delete _vxPlayer;
	}

	void VTEManager::initScene()
	{
		_glSceneDrawer = new GLSceneDrawer();
	}

	void VTEManager::drawScene(int iWidth, int iHeight)
	{
		if (_glSceneDrawer)
			((GLSceneDrawer*)_glSceneDrawer)->draw(iWidth, iHeight);
	}

	void VTEManager::setMoveOctaves(bool bValue)
	{
		if (_glSceneDrawer)
			((GLSceneDrawer*)_glSceneDrawer)->setMoveBetweenOctaves(bValue);
	}

	void VTEManager::PlayInVxFile(const char* fileName)
	{
		if (!_vxPlayer)
			_vxPlayer = new VxFile();

		((VxFile*)_vxPlayer)->load(fileName);
		((VxFile*)_vxPlayer)->play();
	}
}