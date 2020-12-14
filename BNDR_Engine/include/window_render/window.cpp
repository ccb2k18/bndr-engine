#include <pch.h>
#include "window.h"

namespace bndr {

	void ErrorCallback(int code, const char* message) {
		
		std::cout << "The Process exited with code " << code << " and message: " << message << "\n";
		throw std::runtime_error("error");
	}

	void ClearGLErrors() {

		while (glGetError()) {

		}
	}
	void CheckGLError() {

		uint errorCode = glGetError();
		if (errorCode != GL_NO_ERROR) {

			std::cout << "OpenGL reported error code " << errorCode << "\n";
			exit(-1);
		}
	}

	std::pair<std::pair<uint, uint>, uchar*> LoadBitMap(const char* filePath) {

		// open the file
		FILE* imgFile = fopen(filePath, "rb");
		// check if it is open
		if (imgFile == NULL) { throw std::runtime_error("failed to open file\n"); }
		// specify some values for validating the file
		uchar header[54];
		uint dataStart;
		uint width, height, imageSize;
		uchar* imgData;

		// ensure this is a valid bitmap file
		if (fread(header, 1, 54, imgFile) != 54) {
			std::cout << "Invalid file type for LoadBitMap\n";
			throw std::runtime_error("error");
		}
		if (header[0] != 'B' || header[1] != 'M') { 
			
			std::cout << "Incorrect Bitmap file for LoadBitMap\n";
			throw std::runtime_error("error");
		}
		// get the data from the header
		dataStart = *(int*)&(header[0x0A]);
		imageSize = *(int*)&(header[0x22]);
		width = *(int*)&(header[0x12]);
		height = *(int*)&(header[0x16]);
		// put in missing information if there is info missing
		if (imageSize == 0) { imageSize = width * height * 3; } // rgb color scheme
		if (dataStart == 0) { dataStart = 54; } // location of first uchar of data

		// allocate memory on the heap
		imgData = new uchar[imageSize];
		// read the contents of the image file into the memory
		fread(imgData, 1, imageSize, imgFile);

		// close the file stream
		fclose(imgFile);

		// return the data pointer
		return { {width, height}, imgData };

	}

	/*uchar* ConvertRGBToRGBA(uchar* imageData, uint width, uint height) {

		uchar* newImageData = new uchar[width * height * 4];
		uint 
	}*/
	Window::Window(int x, int y, int width, int height, const char* title) {

		// initialize glfw
		if (!glfwInit()) { 
			
			std::cout << "Failed to initialize GLFW\n";
			throw std::runtime_error("error");
		
		}
		glfwSetErrorCallback(ErrorCallback);

		// window hints for the window

		// disable double buffering so we have unlimited fps
		//glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);

		// create window
		window = glfwCreateWindow(width, height, title, NULL, NULL);
		// make sure it created successfully
		if (window == NULL) {

			glfwTerminate();
			std::cout << "GLFW window creation failed\n";
			throw std::runtime_error("error");
		}

		// more window rules

		// position of the window
		glfwSetWindowPos(window, x, y);
		// resizing constraints are half the size specified up to the size specified
		glfwSetWindowSizeLimits(window, width / 2, height / 2, width, height);
		// maintain aspect ratio
		glfwSetWindowAspectRatio(window, width, height);
		aspect = (float)width / (float)height;
		// make context for window
		glfwMakeContextCurrent(window);

		// create opengl context
		if (glewInit() != GLEW_OK) {

			glfwDestroyWindow(window);
			glfwTerminate();
			std::cout << "OpenGL context failed to create\n";
			throw std::runtime_error("error");
		}
	}

	void Window::SetIconImage(const char* imageFile) {

		// load the image and get the parameters
		std::pair<std::pair<uint, uint>, uchar*> imageParameters = LoadBitMap(imageFile);
		// define the icon
		GLFWimage icon;
		icon.width = (int)imageParameters.first.first;
		icon.height = (int)imageParameters.first.second;
		icon.pixels = imageParameters.second;

		// hand it over to glfw
		glfwSetWindowIcon(window, 1, &icon);

		// free the image memory
		delete[] imageParameters.second;
	}

	bool Window::Update() {

		PollEvents();

		if (!IsOpen() || (windowFlags & WINDOW_CLOSE)) {
		
			return false;
		}
		return true;
	}

	Window::~Window() {

		// destruct window
		glfwDestroyWindow(window);
		glfwTerminate();
	}

}
