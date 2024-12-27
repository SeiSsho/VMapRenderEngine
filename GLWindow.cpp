#include "GLWindow.h"

#include <glm/gtc/type_ptr.hpp>

#include "Input.h"
#include "Mesh.h"
#include "Component/Camera.h"
#include "GLObject/Shader.h"
#include <future>
#include <iostream>

#include "OSM/Database.h"
#include "OSM/Way.h"

GLWindow::GLWindow(std::string_view title, const int& width, const int& height, const bool& useVsync)
{
	_windowInfo._width = width;
	_windowInfo._height = height;
	if (!glfwInit()) {
		throw std::exception("Failed to initialize GLFW.");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	_glfwWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
	if (!_glfwWindow) {
		throw std::exception("Failed to initialize GLFWwindow.");
	}

	glfwMakeContextCurrent(_glfwWindow);
	glfwSetWindowUserPointer(_glfwWindow, &_windowInfo);

	glfwSetFramebufferSizeCallback(_glfwWindow, [](GLFWwindow* window, int width, int height) -> void {
		auto windowInfo = static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
		windowInfo->updateSize(width, height);
		});
	glfwSetCursorPosCallback(_glfwWindow, [](GLFWwindow* window, double x, double y) -> void {
		Input::getInstance().getMouse().setPosition(x, y);
		});

	glfwSwapInterval(useVsync);

	if (glewInit() != GLEW_OK) {
		throw std::exception("Failed to initialize GLEW.");
	}

	Transform cameraTransform;
	//cameraTransform._position = { 210, 1055, -3 };
	cameraTransform._position = { 1055, 210, 3 };
	cameraTransform._rotation = glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
	//camera = std::make_shared<Camera>(cameraTransform, 45.0f, _windowInfo.getAspect(), 0.00001f, 1000000.0f);
	camera = std::make_shared<Camera>(cameraTransform, 
		-1 * _windowInfo.getAspect() / 4, 1 * _windowInfo.getAspect() / 4, 1.0 / 4, -1.0 / 4, 0.01f, 1000000000000.0f);

	_gui = std::make_unique<GUI>(_glfwWindow);
	_gui->addRenderBlock([=]()
		{
			ImGui::Begin("Setting");
			if (ImGui::TreeNode("Camera")) {
				ImGui::InputFloat3("Position", glm::value_ptr(camera->_transform._position));
				static glm::vec3 eulerAngle = camera->_transform.getEulerAngle();
				if (ImGui::SliderFloat3("Rotation", &eulerAngle[0], -1080.0f, 1080.0f, "%.3f")) {
					camera->_transform._rotation = glm::quat(eulerAngle);
				}
				ImGui::InputFloat("Near", &camera->_near, 0.001, 1.0, "%.4f");
				ImGui::InputFloat("Far", &camera->_far, 1.0, 10.0, "%.0f");
				if (camera->_cameraType == Camera::Type::Orthographic) {
					ImGui::InputFloat("Zoom", &camera->_zoomScale, 0.1, 10.0f);
				}
				static auto currentProjection = camera->_cameraType;
				const char* projectionItems[] = { "Orthographic", "Perspective" };
				if (ImGui::BeginCombo("Projection", projectionItems[static_cast<int>(currentProjection)])) {
					for (int i = 0; i < IM_ARRAYSIZE(projectionItems); ++i) {
						bool isSelected = (i == static_cast<int>(currentProjection));
						if (ImGui::Selectable(projectionItems[static_cast<int>(i)], isSelected)) {
							currentProjection = static_cast<Camera::Type>(i);
						}
						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Display")) {
				static float pointSize = 1.0f;
				if (ImGui::SliderFloat("Point size", &pointSize, 1.0, 20.0)) {
					glPointSize(pointSize);
					spdlog::info("Change point size {}", pointSize);
				}
				static float lineWidth = 1.0f;
				if (ImGui::SliderFloat("Line width", &lineWidth, 1.0, 20.0)) {
					glLineWidth(lineWidth);
					spdlog::info("Change line width {}", lineWidth);
				}
				if (ImGui::TreeNode("Way")) {
					ImGui::Checkbox("Points", &isDisplayPoint);
					ImGui::Checkbox("Lines", &isDisplayLines);
					ImGui::Checkbox("Đường đi", &isDisplayHighwayLines);
					ImGui::Checkbox("Công trình", &isDisplayBuildingLines);
					ImGui::Checkbox("Thực vật", &isDisplayNaturalLines);
					ImGui::Checkbox("Điện thế", &isDisplayPowerLines);
				}
			}
			ImGui::End();
		});

	glEnable(GL_MULTISAMPLE);
	glfwWindowHint(GLFW_SAMPLES, 8);
}

GLWindow::~GLWindow()
{
	glfwDestroyWindow(_glfwWindow);
	glfwTerminate();
}

void GLWindow::run()
{
	auto vaoPoints = std::make_unique<VertexAttributeObject>();
	auto vaoLines = std::make_unique<VertexAttributeObject>();
	auto vaoHighwayLines = std::make_unique<VertexAttributeObject>();
	auto vaoBuildingLines = std::make_unique<VertexAttributeObject>();
	auto vaoNaturalLines = std::make_unique<VertexAttributeObject>();
	auto vaoPowerLines = std::make_unique<VertexAttributeObject>();
	unsigned long long pointsSize = 0;
	unsigned long long linesSize = 0;
	unsigned long long highwaySize = 0;
	unsigned long long buildingSize = 0;
	unsigned long long naturalSize = 0;
	unsigned long long powerSize = 0;

	{
		std::vector<double> arrPoints;
		for (const auto& node : vmap::osm::Database::instance().nodes()) {
			arrPoints.push_back((node.second->lat() * 10));
			arrPoints.push_back((node.second->lon() * 10));
			arrPoints.push_back(0.01f);
		}

		VertexBufferObject vbo = VertexBufferObject(arrPoints);
		vaoPoints->bind();
		vbo.bind();
		vaoPoints->link(vbo, 0, 3, GLCore::Type::Double, sizeof(double) * 3, 0);
		vbo.unbind();
		vaoPoints->unbind();
		pointsSize = arrPoints.size();
	}
	{
		std::vector<float> arrLines;
		for (const auto& way : vmap::osm::Database::instance().ways()) {
			for (int i = 0; i < way.second->_nodes.size() - 1; i++) {
				auto nodes = way.second->_nodes;
					arrLines.push_back(nodes[i]->lat() * 10);
					arrLines.push_back(nodes[i]->lon() * 10);
					arrLines.push_back(0.0f);
					arrLines.push_back(nodes[i + 1]->lat() * 10);
					arrLines.push_back(nodes[i + 1]->lon() * 10);
					arrLines.push_back(0.0f);
			}
		}
		VertexBufferObject vbo = VertexBufferObject(arrLines);
		vaoLines->bind();
		vbo.bind();
		vaoLines->link(vbo, 0, 3, GLCore::Type::Float, sizeof(float) * 3, 0);
		vbo.unbind();
		vaoLines->unbind();
		linesSize = arrLines.size();
	}
	{
		std::vector<float> lines;
		for (const auto& way : vmap::osm::Database::instance().ways()) {
			for (int i = 0; i < way.second->_nodes.size() - 1; i++) {
				auto nodes = way.second->_nodes;
				if (way.second->tags().find("highway") != way.second->tags().end()) {
					lines.push_back(nodes[i]->lat() * 10);
					lines.push_back(nodes[i]->lon() * 10);
					lines.push_back(0.0f);
					lines.push_back(nodes[i + 1]->lat() * 10);
					lines.push_back(nodes[i + 1]->lon() * 10);
					lines.push_back(0.0f);
				}
			}
		}
		VertexBufferObject vbo = VertexBufferObject(lines);
		vaoHighwayLines->bind();
		vbo.bind();
		vaoHighwayLines->link(vbo, 0, 3, GLCore::Type::Float, sizeof(float) * 3, 0);
		vbo.unbind();
		vaoHighwayLines->unbind();
		highwaySize = lines.size();
	}
	{
		std::vector<float> lines;
		for (const auto& way : vmap::osm::Database::instance().ways()) {
			for (int i = 0; i < way.second->_nodes.size() - 1; i++) {
				auto nodes = way.second->_nodes;
				if (way.second->tags().find("building") != way.second->tags().end()) {
					lines.push_back(nodes[i]->lat() * 10);
					lines.push_back(nodes[i]->lon() * 10);
					lines.push_back(0.0f);
					lines.push_back(nodes[i + 1]->lat() * 10);
					lines.push_back(nodes[i + 1]->lon() * 10);
					lines.push_back(0.0f);
				}
			}
		}
		VertexBufferObject vbo = VertexBufferObject(lines);
		vaoBuildingLines->bind();
		vbo.bind();
		vaoBuildingLines->link(vbo, 0, 3, GLCore::Type::Float, sizeof(float) * 3, 0);
		vbo.unbind();
		vaoBuildingLines->unbind();
		buildingSize = lines.size();
	}
	{
		std::vector<float> lines;
		for (const auto& way : vmap::osm::Database::instance().ways()) {
			for (int i = 0; i < way.second->_nodes.size() - 1; i++) {
				auto nodes = way.second->_nodes;
				if (way.second->tags().find("natural") != way.second->tags().end()) {
					lines.push_back(nodes[i]->lat() * 10);
					lines.push_back(nodes[i]->lon() * 10);
					lines.push_back(0.0f);
					lines.push_back(nodes[i + 1]->lat() * 10);
					lines.push_back(nodes[i + 1]->lon() * 10);
					lines.push_back(0.0f);
				}
			}
		}
		VertexBufferObject vbo = VertexBufferObject(lines);
		vaoNaturalLines->bind();
		vbo.bind();
		vaoNaturalLines->link(vbo, 0, 3, GLCore::Type::Float, sizeof(float) * 3, 0);
		vbo.unbind();
		vaoNaturalLines->unbind();
		naturalSize = lines.size();
	}
	{
		std::vector<float> lines;
		for (const auto& way : vmap::osm::Database::instance().ways()) {
			for (int i = 0; i < way.second->_nodes.size() - 1; i++) {
				auto nodes = way.second->_nodes;
				if (way.second->tags().find("power") != way.second->tags().end()) {
					lines.push_back(nodes[i]->lat() * 10);
					lines.push_back(nodes[i]->lon() * 10);
					lines.push_back(0.0f);
					lines.push_back(nodes[i + 1]->lat() * 10);
					lines.push_back(nodes[i + 1]->lon() * 10);
					lines.push_back(0.0f);
				}
			}
		}
		VertexBufferObject vbo = VertexBufferObject(lines);
		vaoPowerLines->bind();
		vbo.bind();
		vaoPowerLines->link(vbo, 0, 3, GLCore::Type::Float, sizeof(float) * 3, 0);
		vbo.unbind();
		vaoPowerLines->unbind();
		powerSize = lines.size();
	}

	auto shader = ShaderRegistry::getDefaultShader();
	auto shaderId = shader->getId();
	GLint modelLoc = glGetUniformLocation(shaderId, "model");
	GLint viewLoc = glGetUniformLocation(shaderId, "view");
	GLint projLoc = glGetUniformLocation(shaderId, "projection");
	GLint colorLoc = glGetUniformLocation(shaderId, "u_color");

	Transform objectTransform;
	Mesh meshPoint(std::move(vaoPoints), pointsSize, GLCore::Primitive::Points);
	Mesh meshLine(std::move(vaoLines), linesSize, GLCore::Primitive::Lines);
	Mesh meshHighwayLine(std::move(vaoHighwayLines), highwaySize, GLCore::Primitive::Lines);
	Mesh meshBuildingLine(std::move(vaoBuildingLines), buildingSize, GLCore::Lines);
	Mesh meshNaturalLine(std::move(vaoNaturalLines), naturalSize, GLCore::Lines);
	Mesh meshPowerLine(std::move(vaoPowerLines), powerSize, GLCore::Lines);
	//glEnable(GL_DEPTH_TEST);
	glm::vec3 red(1.0, 0.0, 0.0);
	glm::vec3 green(0.0, 1.0, 0.0);
	glClearColor(1, 1, 1, 1);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	while (!glfwWindowShouldClose(_glfwWindow)) {
		glfwPollEvents();
		inputHandle();
		_gui->draw();

		glViewport(0, 0, _windowInfo.getWidth(), _windowInfo.getHeight());
		glClear(GL_COLOR_BUFFER_BIT);

		shader->bind();
		glUniformMatrix4fv(modelLoc, 1, false, glm::value_ptr(objectTransform.matrixTransform()));
		glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(camera->getViewMatrix()));
		glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(camera->getProjectionMatrix()));

		//glEnable(GL_LINE_SMOOTH);
		//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

		if (isDisplayPoint) {
			glUniform3fv(colorLoc, 1, glm::value_ptr(red));
			meshPoint.draw();
		}
		if (isDisplayLines) {
			glUniform3fv(colorLoc, 1, glm::value_ptr(green));
			meshLine.draw();
		}
		if (isDisplayHighwayLines) {
			glm::vec3 highwayColor(237, 109, 12);
			highwayColor = glm::normalize(highwayColor);
			glUniform3fv(colorLoc, 1, glm::value_ptr(highwayColor));
			meshHighwayLine.draw();
		}
		if (isDisplayBuildingLines) {
			glm::vec3 buildingColor((12, 226, 237));
			buildingColor = glm::normalize(buildingColor);
			glUniform3fv(colorLoc, 1, glm::value_ptr(buildingColor));
			meshBuildingLine.draw();
		}
		if (isDisplayNaturalLines) {
			glm::vec3 naturalColor(5, 247, 74);
			naturalColor = glm::normalize(naturalColor);
			glUniform3fv(colorLoc, 1, glm::value_ptr(naturalColor));
			meshNaturalLine.draw();
		}
		if (isDisplayPowerLines) {
			//glPushAttrib(GL_ENABLE_BIT);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(10, 0x00FF);

			glm::vec3 powerColor(247, 5, 94);
			powerColor = glm::normalize(powerColor);
			glUniform3fv(colorLoc, 1, glm::value_ptr(powerColor));
			meshPowerLine.draw();
			glDisable(GL_LINE_STIPPLE);
			//glPopAttrib();
		}

		_gui->renderDrawData();
		glfwSwapBuffers(_glfwWindow);
	}
}

void GLWindow::inputHandle() {
	if (glfwGetKey(_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_glfwWindow, true);
	if (glfwGetKey(_glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		camera->_transform._position += camera->_transform.forward() * 0.005f;
	if (glfwGetKey(_glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		camera->_transform._position += camera->_transform.forward() * -0.005f;
	if (glfwGetKey(_glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		camera->_transform._position += camera->_transform.right() * -0.005f;
	if (glfwGetKey(_glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		camera->_transform._position += camera->_transform.right() * 0.005f;
	if (glfwGetKey(_glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera->_transform._position += camera->_transform.up() * -0.005f;
	if (glfwGetKey(_glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->_transform._position += camera->_transform.up() * +0.005f;
	if (glfwGetKey(_glfwWindow, GLFW_KEY_T) == GLFW_PRESS)
		camera->_zoomScale += 0.025f;
	if (glfwGetKey(_glfwWindow, GLFW_KEY_Y) == GLFW_PRESS)
		camera->_zoomScale -= 0.025f;
}

void GLWindow::WindowInfo::updateSize(const int& width, const int& height)
{
	_width = width;
	_height = height;
	spdlog::trace("Resize window {}x{}", width, height);
}
