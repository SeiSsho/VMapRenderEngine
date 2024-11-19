#pragma once
#include "GLWindow.h"
#include "Asset/Scene.h"
#include <memory>

class Renderer
{
	friend class GLWindow;
private:
	std::shared_ptr<Scene> _scene = nullptr;

private:
	void onPreCullPhase();
	void onBecomeInvisiblePhase();
	void onWillRenderObject();
	void onPreRender();
	void onRenderObject();
	void onPostRender();
	void onRenderImage();

public:
	void render();
	void setScene(std::shared_ptr<Scene> _scene);
	std::shared_ptr<Scene> getScene() const;
};

