#include "hzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Hazel/Application.h"

// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	/* 这个函数在这个层（ImGuiLayer）被添加到层栈时被调用 */
	void ImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext(); // 创建了一个ImGui的上下文对象，这个对象包含了所有ImGui需要的数据和状态
		// @brief ImGuiIO结构体：包含了所有ImGui需要的输入输出数据，比如鼠标位置，键盘状态，显示大小等。
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark(); // 设置了ImGui的风格颜色为暗色（Dark）主题
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410"); // 初始化OpenGL3渲染器（这个字符串参数表示使用OpenGL着色器语言版本4.10）
	}

	/* 这个函数在这个层从层栈中移除时被调用 */
	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown(); // 关闭OpenGL3渲染器
		ImGui_ImplGlfw_Shutdown(); // 关闭GLFW的ImGui后端
		ImGui::DestroyContext(); // 销毁ImGui的上下文对象
	}
	
	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame(); // 开始新的一帧
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // 渲染数据

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) // 多视口渲染
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext(); // 备份当前的上下文
			ImGui::UpdatePlatformWindows(); // 更新平台窗口
			ImGui::RenderPlatformWindowsDefault(); // 渲染平台窗口
			glfwMakeContextCurrent(backup_current_context); // 恢复当前的上下文
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show); // 显示ImGui的Demo窗口
	}

}