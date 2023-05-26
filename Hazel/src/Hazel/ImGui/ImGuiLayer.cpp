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
		// ���°汾��
		ImGui::SetCurrentContext(ImGui::CreateContext()); // ����������ImGui��������
		Application& app = Application::Get();
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)app.GetWindow().GetNativeWindow(), true); // ��ʼ��GLFW��ImGui��ˣ���Ȼ�ᱨ��bdΪ��ָ�룩
	}

	ImGuiLayer::~ImGuiLayer()
	{
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

		// ��ʱ�������루�ɰ汾��
		/*io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;*/

		ImGui_ImplOpenGL3_Init("#version 410"); // ��ʼ��OpenGL3��Ⱦ��������ַ���������ʾʹ��OpenGL��ɫ�����԰汾4.10��
	}

	/* ��������������Ӳ�ջ���Ƴ�ʱ������ */
	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
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


		/* ����ImGui��������ݺ�����꣨�°汾�� */
		ImGui_ImplGlfw_UpdateMouseData();
		ImGui_ImplGlfw_UpdateMouseCursor();
		auto mouse_x = io.MousePos.x; // ��ȡ����x����
		auto mouse_y = io.MousePos.y; // ��ȡ����y����
		io.AddMousePosEvent((float)mouse_x, (float)mouse_y); // ������λ�ø����¼�


		ImGui_ImplOpenGL3_NewFrame(); // ֪ͨOpenGL3��Ⱦ����ʼ��һ֡����Ⱦ
		ImGui::NewFrame(); // ��ʼ��һ֡��GUI��������Ⱦ

		/* ��ʾ���� */
		static bool flagDemoWindowShow = true; // ��ʾ�Ƿ���ʾ������ڣ�����ͨ������������ϽǵĹرհ�ť���ı���
		ImGui::ShowDemoWindow(&flagDemoWindowShow); // ��ʾһ����ʾ���ڣ�չʾImGui�ṩ�ĸ��ֿؼ��͹���

		ImGui::Render(); // ������ǰ֡��GUI��������Ⱦ�������ɻ�������
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // ���������OpenGL3��Ⱦ���ṩ�ĸ������е�һ�������������ѻ�������ת��ΪOpenGL���ִ������
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
		io.MouseDown[e.GetMouseButton()] = true;

		return false; // return false ��ԭ������Ϊ����ϣ��������Ҳ���Դ�������¼�
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (e.GetMouseButton() >= 0 && e.GetMouseButton() < ImGuiMouseButton_COUNT)
			io.AddMouseButtonEvent(e.GetMouseButton(), e.GetEventType() == EventType::MouseButtonReleased);
		// Ҳ���԰�e.GetEventType() == EventType::MouseButtonReleased����false����Ϊ�������ֻ����갴ť�ͷ��¼��е��á�
		//io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent((float)e.GetX(), (float)e.GetY());
		//io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent((float)e.GetXOffset(), (float)e.GetYOffset());
		//io.MouseWheelH += e.GetXOffset();
		//io.MouseWheel += e.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (e.GetKeyCode() >= 0 && e.GetKeyCode() < IM_ARRAYSIZE(io.KeysDown))
			io.AddKeyEvent((ImGuiKey)e.GetKeyCode(), e.GetEventType() == EventType::KeyPressed);

		// ���ɰ汾��
		/*io.KeysDown[e.GetKeyCode()] = true;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];*/

		if (io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL]) // �����һ�δ��룬�ж��Ƿ�����Ctrl��
			io.AddKeyEvent(ImGuiKey_ModCtrl, true); // ����ǣ������ImGuiKey_ModCtrl�¼�
		if (io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT])
			io.AddKeyEvent(ImGuiKey_ModShift, true);
		if (io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT])
			io.AddKeyEvent(ImGuiKey_ModAlt, true);
		if (io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER])
			io.AddKeyEvent(ImGuiKey_ModSuper, true);

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (e.GetKeyCode() >= 0 && e.GetKeyCode() < IM_ARRAYSIZE(io.KeysDown))
			io.AddKeyEvent((ImGuiKey)e.GetKeyCode(), e.GetEventType() == EventType::KeyReleased);
		//io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		//if (e.GetKeyCode() >= 0 && e.GetKeyCode() < IM_ARRAYSIZE(io.KeysDown))
			//io.AddKeyEvent((ImGuiKey)e.GetKeyCode(), e.GetEventType() == EventType::KeyTyped);
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
