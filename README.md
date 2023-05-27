# Hazel

Hazel Engine



## Issue

#### 1. 引入GLFW时，vs2022上运行代码（链接）时的报错问题（2023.5.24）

```
1>GLFW.lib(init.obj) : error LNK2019: 无法解析的外部符号 _glfwSelectPlatform，函数 glfwInit 中引用了该符号
1>GLFW.lib(vulkan.obj) : error LNK2019: 无法解析的外部符号 _glfwPlatformLoadModule，函数 _glfwInitVulkan 中引用了该符号
1>GLFW.lib(vulkan.obj) : error LNK2019: 无法解析的外部符号 _glfwPlatformFreeModule，函数 _glfwTerminateVulkan 中引用了该符号
1>GLFW.lib(vulkan.obj) : error LNK2019: 无法解析的外部符号 _glfwPlatformGetModuleSymbol，函数 _glfwInitVulkan 中引用了该符号
1>..\bin\Debug-windows-x86_64\Hazel\Hazel.dll : fatal error LNK1120: 4 个无法解析的外部命令
```

+ 解决方法：若遇到了与 `LNK2019 GLFW.lib (init.obj) or (vulkan.obj)` 相关的构建问题，可以在 Visual Studio 2022 中通过检出 GLFW 存储库的 "latest" 分支来解决该问题。 这是因为可能存在一个与 GLFW 存储库的主分支（master branch）相关的问题，导致构建失败并出现 `LNK2019` 错误。为了解决这个问题，建议切换到 GLFW 存储库的 "latest" 分支。

  1. 打开你的 GLFW 存储库文件夹，并确认你拥有对该存储库的访问权限。

  2. 在存储库文件夹中，执行以下命令以切换到 "latest" 分支：

     ```
   git checkout latest
     ```

     该命令将使你的存储库切换到名为 "latest" 的分支。
  
  3. 在 Visual Studio 2022 中重新构建你的项目，检查是否仍然出现 `LNK2019` 错误。

  请注意，这种解决方法是基于他人的经验，并不保证适用于所有情况。这只是一个尝试，可能会对你的项目产生积极影响。如果问题仍然存在，你可能需要进一步调查和尝试其他解决方案，如更新 GLFW 存储库或寻求相关支持渠道的帮助。最好的做法是参考 GLFW 存储库的文档、问题跟踪器或相关讨论来了解与 Visual Studio 2022 兼容性和构建问题相关的最新信息和解决方案。

#### 2. OnEvent函数中为什么要逆序遍历LayerStack?（2023.5.24）

```c++
// In Hazel/Application.cpp OnEvent函数中为什么要逆序遍历LayerStack?
for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
    (*--it)->OnEvent(e);
    if (e.IsHandled())
        break; 	
}
```

这段代码的目的是在层栈中处理一个事件对象e。事件对象可以表示任何发生在应用程序中的事情，比如键盘按键，鼠标移动，窗口关闭等。不同的层可能对同一个事件有不同的响应，比如一个图形层可能要根据鼠标位置绘制一个光标，而一个声音层可能要根据键盘按键播放一个音效。

为了让每个层都有机会处理事件，这段代码使用了一个循环来遍历层栈中的所有层，并调用它们的OnEvent函数，把事件对象作为参数传递给它们。OnEvent函数是一个虚拟函数，可以被派生类重写，以实现不同的事件处理逻辑。

但是，并不是所有的事件都需要被所有的层处理。有些事件可能只需要被最上层的层处理，比如一个覆盖层显示了一个弹出菜单，那么它就应该拦截鼠标点击事件，而不让它传递给下面的层。为了实现这种效果，事件对象有一个属性叫做IsHandled，表示事件是否已经被处理过了。如果一个层处理了一个事件，它就可以把这个属性设置为true，这样其他的层就不需要再处理这个事件了。

因此，这段代码在每次调用OnEvent函数后，都会检查事件对象的IsHandled属性，如果它是true，就跳出循环，不再向下传递事件。这样就实现了一种优先级机制，让上层的层可以优先处理事件，并阻止下层的层接收到不需要的事件。

为了实现这种优先级机制，这段代码使用了逆序遍历LayerStack的方法。也就是说，它从最后一个元素开始往前遍历，而不是从第一个元素开始往后遍历。这样做的原因是，在LayerStack中，最后添加的层是最上面的层，也就是覆盖层。而最先添加的层是最下面的层，也就是普通层。所以，为了让覆盖层优先处理事件，并有机会拦截事件，必须从后往前遍LayerStack。

具体来说，这段代码使用了一个反向迭代器it来遍历LayerStack。反向迭代器是一种特殊的迭代器，它可以让向量或其他容器按照相反的顺序遍历。它有以下特点：

- 它初始化为m_LayerStack.end()，也就是指向向量尾部之后的位置。
- 它在每次循环前都要自减（--it），也就是向前移动一个位置。
- 它在每次循环中都要解引用（*it），也就是获取当前位置指向的元素。
- 它在循环条件中使用了不等于（!=）运算符来判断是否到达向量开头之前的位置。
- 它在循环体中使用了break语句来跳出循环。

通过这种方式，这段代码可以实现逆序遍历LayerStack，并按照覆盖层优先的原则来处理事件。

#### 3. Hazel做到ImGui（latest imgui）时，出现不能正常点击和拖动窗口等事件问题（2023.5.26）【docking分支是运行正常的代码，但是使用的是旧版的ImGui】

+ 解决方法：使用新版的ImGui函数接口，如AddMouseButtonEvent()和AddKeyEvent()等，可以参考imgui_impl_glfw.cpp中的代码。**【注意：新版API和旧版API不能混合使用！！！】**

+ 如果使用新版的接口时报错：
```
Assertion failed: GImGui != 0 && "No current context. Did you call ImGui::CreateContext() and ImGui::SetCurrentContext() ?", file E:\codes\C++\GameEngine\Hazel\Hazel\vendor\imgui\imgui.cpp, line 4139
```

这个错误表示你没有正确地创建和设置ImGui的上下文，ImGui的上下文是一个存储ImGui的状态和数据的结构体，你需要在使用ImGui之前调用ImGui::CreateContext()函数来创建一个上下文，并且调用ImGui::SetCurrentContext()函数来设置当前的上下文1。你可以在你的ImGuiLayer类的构造函数或者初始化函数中加上这样一行代码：

```c++
ImGui::SetCurrentContext(ImGui::CreateContext()); // 创建并设置ImGui的上下文
```

这样就可以避免这个错误了。

+ 此时，你的ImGuiLayer的构造函数应该长这样：

```C++
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
		ImGui::SetCurrentContext(ImGui::CreateContext()); // 创建并设置ImGui的上下文
		Application& app = Application::Get();
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)app.GetWindow().GetNativeWindow(), true); // 初始化GLFW的ImGui后端（否则会报错：bd为空指针）
	}
// 其中，GetNativeWindow()是我刚刚补充的，目的是为了获取GLFWwindow*
```

+ 同时，你的OnMouseButtonPressedEvent和OnKeyPressedEvent等函数应该长这样：

```C++
	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (e.GetMouseButton() >= 0 && e.GetMouseButton() < ImGuiMouseButton_COUNT)
			io.AddMouseButtonEvent(e.GetMouseButton(), e.GetEventType() == EventType::MouseButtonPressed);
		//io.MouseDown[e.GetMouseButton()] = true; // old version

		return false; // return false 的原因是因为我们希望其他层也可以处理这个事件
	}
// 其中if语句是一个范围检查，确保鼠标按钮的索引在ImGuiMouseButton_COUNT以内，这是一个好习惯，可以避免一些潜在的错误。
// 其实，e.GetEventType() == EventType::MouseButtonPressed可以改成true，并且OnMouseButtonReleasedEvent中的e.GetEventType() == EventType::MouseButtonReleased也可以改成false。
// 因为，true表示按下，而且这个函数只在鼠标按钮按下事件中调用；同理，false表示释放。
```

```C++
bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (e.GetKeyCode() >= 0 && e.GetKeyCode() < IM_ARRAYSIZE(io.KeysDown))
			io.AddKeyEvent((ImGuiKey)e.GetKeyCode(), e.GetEventType() == EventType::KeyPressed);
		// 	与OnMouseButtonPressedEvent类似，e.GetEventType() == EventType::KeyPressed可以改成true。
    
		// （旧版本）
		/*
		io.KeysDown[e.GetKeyCode()] = true;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		*/
		
    	// （新版本）
		if (io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL]) // 判断是否按下了Ctrl键
			io.AddKeyEvent(ImGuiKey_ModCtrl, true); // 如果是，就添加ImGuiKey_ModCtrl事件
		if (io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT])
			io.AddKeyEvent(ImGuiKey_ModShift, true);
		if (io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT])
			io.AddKeyEvent(ImGuiKey_ModAlt, true);
		if (io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER])
			io.AddKeyEvent(ImGuiKey_ModSuper, true);

		return false;
	}
```

+ **==注意：上述代码应该还隐含着一些bug，运行时就能看出来，而且按下按键时会有一个断言错误。因为我不是imgui专家（第一次正式接触imgui），所以目前我还找不到，等日后有空系统性学习这些东西后再回来看看。==**
