#include <Engine.hpp>
#include <Bitmap.hpp>
#include <Texture.hpp>
#include <GL/GLTexture.hpp>
#include <Renderer.hpp>
#include <MeshData.hpp>
#include <ShaderProgram.hpp>
#include "UIManager.hpp"

UIManager::UIManager(const Engine& engine)
	: _engine(engine)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendPlatformName = "custom_micro_engine";

	//io.KeyMap[ImGuiKey_Tab] = static_cast<size_t>();

	io.RenderDrawListsFn = nullptr;

	int width = 0;
	int height = 0;
	unsigned char* dataPtr;
	io.Fonts->GetTexDataAsRGBA32(&dataPtr, &width, &height);
	std::vector<unsigned char> image(dataPtr, dataPtr + (width * height * 4));

	Bitmap bitmap(4, std::move(image), glm::vec2{ width, height });

	//_tex = _engine.renderer().createTexture(bitmap);

	//_vertexBuffer = engine.renderer().createVertexBuffer(std::move(data));
	_program = _engine.renderer().createProgram("draw");

	_textureUniform = _program->createTextureUniform("uTexture");
	_textureUniform->texture = _engine.renderer().createTexture(std::move(bitmap));

	_screenSizeUniform = _program->createVec2Uniform("uScreenSize");
	_transformUniform = _program->createMat3Uniform("uTransform");
}

void UIManager::visit()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(_engine.get_window_width(), _engine.get_window_height());

	ImGui::NewFrame();
	static bool show_demo_window;
	ImGui::ShowDemoWindow(&show_demo_window);
	ImGui::Render();

	auto drawData = ImGui::GetDrawData();

	static_assert(sizeof(MeshData::Vertex) == sizeof(ImDrawVert));
	static_assert(sizeof(MeshData::Vertex::position) == sizeof(ImDrawVert::pos));
	static_assert(sizeof(MeshData::Vertex::texcoord) == sizeof(ImDrawVert::uv));
	static_assert(sizeof(MeshData::Vertex::color) == sizeof(ImDrawVert::col));
	static_assert(offsetof(MeshData::Vertex, position) == offsetof(ImDrawVert, pos));
	static_assert(offsetof(MeshData::Vertex, texcoord) == offsetof(ImDrawVert, uv));
	static_assert(offsetof(MeshData::Vertex, color) == offsetof(ImDrawVert, col));

	for (int n = 0; n < drawData->CmdListsCount; ++n)
	{
		const ImDrawList* cmd_list = drawData->CmdLists[n];

		auto vertex_data = reinterpret_cast<MeshData::Vertex*>(cmd_list->VtxBuffer.Data);
		size_t vert_count = static_cast<size_t>(cmd_list->VtxBuffer.size());

		const uint32_t* indexes = cmd_list->IdxBuffer.Data;
		size_t index_count = static_cast<size_t>(cmd_list->IdxBuffer.size());

		MeshData meshData;
		meshData.vertices = { vertex_data, vertex_data + vert_count };
		meshData.indexes = { indexes, indexes + index_count };

		auto vertexBuffer = _engine.renderer().createVertexBuffer(std::move(meshData));

		//command.vertexBuffer = std::move(vertexBuffer);

	}
	//GLTexture* tex_copy = static_cast<GLTexture*>(_tex.get());
	//tex_copy->active();
}