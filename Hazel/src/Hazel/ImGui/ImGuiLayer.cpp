#include "hzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Hazel/Application.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_glfw.cpp>

// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
		ImGui::SetCurrentContext(ImGui::CreateContext()); // ����������ImGui��������
		Application& app = Application::Get();
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)app.GetWindow().GetNativeWindow(), true); // ��ʼ��GLFW��ImGui��ˣ�����Ӧ�ó��򴰿ڵ�ԭ�����ھ������Ȼ�ᱨ��bdΪ��ָ�룩
	}

	ImGuiLayer::~ImGuiLayer()
	{
		OnDetach();
	}

	/* �������������㣨ImGuiLayer������ӵ���ջʱ������ */
	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext(); // ������һ��ImGui�������Ķ�������������������ImGui��Ҫ�����ݺ�״̬
		ImGui::StyleColorsDark(); // ������ImGui�ķ����ɫΪ��ɫ��Dark������

		// @brief ImGuiIO�ṹ�壺����������ImGui��Ҫ������������ݣ��������λ�ã�����״̬����ʾ��С�ȡ�
		ImGuiIO& io = ImGui::GetIO(); // ��ȡio����
		// ������io.BackendFlags���Ե�����λ��־Ϊ�棨��������־��ʾ���֧���������������λ�õĹ��ܣ�
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // �����
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos; // ���λ��

		ImGui_ImplOpenGL3_Init("#version 410"); // ��ʼ��OpenGL3��Ⱦ��������ַ���������ʾʹ��OpenGL��ɫ�����԰汾4.10��
	}

	/* ��������������Ӳ�ջ���Ƴ�ʱ������ */
	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown(); // �ر�OpenGL3��Ⱦ��
		ImGui_ImplGlfw_Shutdown(); // �ر�GLFW��ImGui���
		ImGui::DestroyContext(); // ����ImGui�������Ķ���
	}

	/* ���������ÿһ֡������ */
	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO(); // ��ȡio����
		Application& app = Application::Get(); // ͨ��app��ȡ��ǰ���ڶ���
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight()); // ������ʾ��С

		float time = (float)glfwGetTime(); // ��ȡ��ǰʱ��
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : float(1.0f / 60.0f); // �����ÿһ֡��ʱ����
		m_Time = time; // ����m_Time����Ϊ��ǰʱ��


		ImGui_ImplOpenGL3_NewFrame(); // ֪ͨOpenGL3��Ⱦ����ʼ��һ֡����Ⱦ

		/* ����ImGui��������ݺ�����꣨�°汾�� */
		ImGui_ImplGlfw_UpdateMouseData();
		ImGui_ImplGlfw_UpdateMouseCursor();
		auto mouse_x = io.MousePos.x; // ��ȡ����x����
		auto mouse_y = io.MousePos.y; // ��ȡ����y����
		io.AddMousePosEvent((float)mouse_x, (float)mouse_y); // ������λ�ø����¼�

		ImGui::NewFrame(); // ��ʼ��һ֡��GUI��������Ⱦ

		/* ��ʾ���� */
		static bool flagDemoWindowShow = true; // ��ʾ�Ƿ���ʾ������ڣ�����ͨ������������ϽǵĹرհ�ť���ı���
		ImGui::ShowDemoWindow(&flagDemoWindowShow); // ��ʾһ����ʾ���ڣ�չʾImGui�ṩ�ĸ��ֿؼ��͹���

		ImGui::Render(); // ������ǰ֡��GUI��������Ⱦ�������ɻ�������
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // ���������OpenGL3��Ⱦ���ṩ�ĸ������е�һ�������������ѻ�������ת��OpenGL���ִ������
	}

	/* ����������������յ�һ���¼�����ʱ������ */
	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));

		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));

		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (e.GetMouseButton() >= 0 && e.GetMouseButton() < ImGuiMouseButton_COUNT)
			io.AddMouseButtonEvent(e.GetMouseButton(), e.GetEventType() == EventType::MouseButtonPressed);

		return false; // return false ��ԭ������Ϊ����ϣ��������Ҳ���Դ�������¼�
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (e.GetMouseButton() >= 0 && e.GetMouseButton() < ImGuiMouseButton_COUNT)
			io.AddMouseButtonEvent(e.GetMouseButton(), e.GetEventType() == EventType::MouseButtonReleased);
		// Ҳ���԰�e.GetEventType() == EventType::MouseButtonReleased����false����Ϊ�������ֻ����갴ť�ͷ��¼��е��á�

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent((float)e.GetX(), (float)e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent((float)e.GetXOffset(), (float)e.GetYOffset());

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (e.GetKeyCode() >= 0 && e.GetKeyCode() < IM_ARRAYSIZE(io.KeysDown))
			io.AddKeyEvent((ImGuiKey)e.GetKeyCode(), e.GetEventType() == EventType::KeyPressed);

		if (io.KeyMods & ImGuiKey_ModCtrl) // �����һ�δ��룬�ж��Ƿ�����Ctrl��
			io.AddInputCharacter(ImGuiMod_Ctrl); // ����ǣ������ImGuiKey_ModCtrl�¼�
		if (io.KeyMods & ImGuiKey_ModShift)
			//io.AddInputCharacter(ImGuiMod_Shift);
			io.AddKeyEvent(ImGuiMod_Shift, true);
		if (io.KeyMods & ImGuiMod_Alt)
			io.AddInputCharacter(ImGuiMod_Alt);
		if (io.KeyMods & ImGuiMod_Super)
			io.AddInputCharacter(ImGuiMod_Super);

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (e.GetKeyCode() >= 0 && e.GetKeyCode() < IM_ARRAYSIZE(io.KeysDown))
			io.AddKeyEvent((ImGuiKey)e.GetKeyCode(), e.GetEventType() == EventType::KeyReleased);

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(float(e.GetWidth()), float(e.GetHeight()));
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}

}
