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

	/* �������������㣨ImGuiLayer������ӵ���ջʱ������ */
	void ImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext(); // ������һ��ImGui�������Ķ�������������������ImGui��Ҫ�����ݺ�״̬
		// @brief ImGuiIO�ṹ�壺����������ImGui��Ҫ������������ݣ��������λ�ã�����״̬����ʾ��С�ȡ�
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark(); // ������ImGui�ķ����ɫΪ��ɫ��Dark������
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
		ImGui_ImplOpenGL3_Init("#version 410"); // ��ʼ��OpenGL3��Ⱦ��������ַ���������ʾʹ��OpenGL��ɫ�����԰汾4.10��
	}

	/* ��������������Ӳ�ջ���Ƴ�ʱ������ */
	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown(); // �ر�OpenGL3��Ⱦ��
		ImGui_ImplGlfw_Shutdown(); // �ر�GLFW��ImGui���
		ImGui::DestroyContext(); // ����ImGui�������Ķ���
	}
	
	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame(); // ��ʼ�µ�һ֡
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
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // ��Ⱦ����

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) // ���ӿ���Ⱦ
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext(); // ���ݵ�ǰ��������
			ImGui::UpdatePlatformWindows(); // ����ƽ̨����
			ImGui::RenderPlatformWindowsDefault(); // ��Ⱦƽ̨����
			glfwMakeContextCurrent(backup_current_context); // �ָ���ǰ��������
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show); // ��ʾImGui��Demo����
	}

}