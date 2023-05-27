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
		ImGui::SetCurrentContext(ImGui::CreateContext()); // 创建并设置ImGui的上下文
		Application& app = Application::Get();
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)app.GetWindow().GetNativeWindow(), true); // 初始化GLFW的ImGui后端，传递应用程序窗口的原生窗口句柄（不然会报错：bd为空指针）
	}

	ImGuiLayer::~ImGuiLayer()
	{
		OnDetach();
	}

	/* 这个函数在这个层（ImGuiLayer）被添加到层栈时被调用 */
	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext(); // 创建了一个ImGui的上下文对象，这个对象包含了所有ImGui需要的数据和状态
		ImGui::StyleColorsDark(); // 设置了ImGui的风格颜色为暗色（Dark）主题

		// @brief ImGuiIO结构体：包含了所有ImGui需要的输入输出数据，比如鼠标位置，键盘状态，显示大小等。
		ImGuiIO& io = ImGui::GetIO(); // 获取io对象
		// 设置了io.BackendFlags属性的两个位标志为真（这两个标志表示后端支持鼠标光标和设置鼠标位置的功能）
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // 鼠标光标
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos; // 鼠标位置

		ImGui_ImplOpenGL3_Init("#version 410"); // 初始化OpenGL3渲染器（这个字符串参数表示使用OpenGL着色器语言版本4.10）
	}

	/* 这个函数在这个层从层栈中移除时被调用 */
	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown(); // 关闭OpenGL3渲染器
		ImGui_ImplGlfw_Shutdown(); // 关闭GLFW的ImGui后端
		ImGui::DestroyContext(); // 销毁ImGui的上下文对象
	}

	/* 这个函数在每一帧被调用 */
	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO(); // 获取io对象
		Application& app = Application::Get(); // 通过app获取当前窗口对象
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight()); // 设置显示大小

		float time = (float)glfwGetTime(); // 获取当前时间
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : float(1.0f / 60.0f); // 计算出每一帧的时间间隔
		m_Time = time; // 更新m_Time变量为当前时间


		ImGui_ImplOpenGL3_NewFrame(); // 通知OpenGL3渲染器开始新一帧的渲染

		/* 更新ImGui的鼠标数据和鼠标光标（新版本） */
		ImGui_ImplGlfw_UpdateMouseData();
		ImGui_ImplGlfw_UpdateMouseCursor();
		auto mouse_x = io.MousePos.x; // 获取鼠标的x坐标
		auto mouse_y = io.MousePos.y; // 获取鼠标的y坐标
		io.AddMousePosEvent((float)mouse_x, (float)mouse_y); // 添加鼠标位置更新事件

		ImGui::NewFrame(); // 开始新一帧的GUI创建和渲染

		/* 演示窗口 */
		static bool flagDemoWindowShow = true; // 表示是否显示这个窗口，可以通过点击窗口右上角的关闭按钮来改变它
		ImGui::ShowDemoWindow(&flagDemoWindowShow); // 显示一个演示窗口，展示ImGui提供的各种控件和功能

		ImGui::Render(); // 结束当前帧的GUI创建和渲染，并生成绘制数据
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // 这个函数是OpenGL3渲染器提供的辅助库中的一个函数，用来把绘制数据转换OpenGL命令并执行它们
	}

	/* 这个函数在这个层接收到一个事件对象时被调用 */
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

		return false; // return false 的原因是因为我们希望其他层也可以处理这个事件
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (e.GetMouseButton() >= 0 && e.GetMouseButton() < ImGuiMouseButton_COUNT)
			io.AddMouseButtonEvent(e.GetMouseButton(), e.GetEventType() == EventType::MouseButtonReleased);
		// 也可以把e.GetEventType() == EventType::MouseButtonReleased换成false，因为这个函数只在鼠标按钮释放事件中调用。

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

		if (io.KeyMods & ImGuiKey_ModCtrl) // 添加这一段代码，判断是否按下了Ctrl键
			io.AddInputCharacter(ImGuiMod_Ctrl); // 如果是，就添加ImGuiKey_ModCtrl事件
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
