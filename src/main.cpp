#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "ugine.h"
#include <glfw3.h>
#include <iostream>
#include <vector>

using namespace std;

int main() {
	// Inicializamos GLFW
	if (!glfwInit()) {
		cout << "Error: No se ha podido inicializar GLFW" << endl;
		return -1;
	}
	atexit(glfwTerminate);

	// Creamos la ventana
	glfwWindowHint(GLFW_RESIZABLE, true);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Collisions", nullptr, nullptr);
	if (!window) {
		cout << "Error: No se ha podido crear la ventana" << endl;
		return -1;
	}

	// Activamos contexto de OpenGL
	glfwMakeContextCurrent(window);

	// Inicializamos LiteGFX
	lgfx_setup2d(800, 600);

	// Cargamos texturas
	ltex_t* ballTex = loadTexture("data/ball.png");
	ltex_t* beeTex = loadTexture("data/bee.png");
	ltex_t* boxTex = loadTexture("data/box.png");
	ltex_t* circleTex = loadTexture("data/circle.png");
	ltex_t* rectTex = loadTexture("data/rect.png");

	// Creamos sprites
	Sprite ballSprite(ballTex);
	ballSprite.setCollisionType(Sprite::COLLISION_CIRCLE);
	Sprite beeSprite(beeTex);
	beeSprite.setCollisionType(Sprite::COLLISION_PIXELS);
	Sprite boxSprite(boxTex);
	boxSprite.setCollisionType(Sprite::COLLISION_RECT);
	Sprite mouseSprite(circleTex);
	mouseSprite.setCollisionType(Sprite::COLLISION_CIRCLE);

	// Variables de escalado
	float scale = 1.0f;
	float scaleFactor = 0.25f;

	// Bucle principal
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		// Actualizamos delta
		float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();

		// Actualizamos tamaño de ventana
		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		lgfx_setviewport(0, 0, screenWidth, screenHeight);
		Vec2 screenSize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

		// Actualizamos coordenadas de raton
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		Vec2 mousePos = Vec2(static_cast<float>(mouseX), static_cast<float>(mouseY));

		// Posicionamos los sprites
		ballSprite.setPosition(Vec2(screenWidth * 0.25f, screenHeight * 0.5f));
		beeSprite.setPosition(Vec2(screenWidth * 0.50f, screenHeight * 0.5f));
		boxSprite.setPosition(Vec2(screenWidth * 0.75f, screenHeight * 0.5f));
		mouseSprite.setPosition(mousePos);

		// Escalamos los sprites
		scale += scaleFactor * deltaTime;
		if (scale >= 1.1f) { scale = 1.1f; scaleFactor *= -1; }
		if (scale <= 0.9f) { scale = 0.9f; scaleFactor *= -1; }
		ballSprite.setScale(Vec2(scale, scale));
		beeSprite.setScale(Vec2(scale, scale));
		boxSprite.setScale(Vec2(scale, scale));

		// Establecemos tipo de colision
		if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ) {
			mouseSprite.setTexture(circleTex);
			mouseSprite.setCollisionType(Sprite::COLLISION_CIRCLE);
		} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
			mouseSprite.setTexture(rectTex);
			mouseSprite.setCollisionType(Sprite::COLLISION_RECT);
		} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) {
			mouseSprite.setTexture(beeTex);
			mouseSprite.setCollisionType(Sprite::COLLISION_PIXELS);
		}

		// Comprobamos colisiones
		mouseSprite.setColor(1, 1, 1, 1);
		ballSprite.setColor(1, 1, 1, 1);
		beeSprite.setColor(1, 1, 1, 1);
		boxSprite.setColor(1, 1, 1, 1);
		if (mouseSprite.collides(ballSprite)) {
			mouseSprite.setColor(1, 0, 0, 1);
			ballSprite.setColor(1, 0, 0, 1);
		}
		if (mouseSprite.collides(beeSprite)) {
			mouseSprite.setColor(1, 0, 0, 1);
			beeSprite.setColor(1, 0, 0, 1);
		}
		if (mouseSprite.collides(boxSprite)) {
			mouseSprite.setColor(1, 0, 0, 1);
			boxSprite.setColor(1, 0, 0, 1);
		}

		// Pintado
		// -------

		lgfx_clearcolorbuffer(0.15f, 0.15f, 0.15f);
		ballSprite.draw();
		beeSprite.draw();
		boxSprite.draw();
		mouseSprite.draw();

		// Actualizamos ventana y eventos
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// eliminamos texturas
	// ...

    return 0;
}
