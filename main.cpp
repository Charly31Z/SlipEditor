#include <iostream>

#include "SlipWindow.h"
#include "SlipInput.h"
#include "SlipID.h"
#include "SlipGlobals.h"
#include "SlipFrameBuffer.h"
#include "SlipActor.h"
#include "FileDialog.h"
#include "SlipUI.h"
#include "SlipEditor.h"
#include "SlipDebug.h"

#include "SlipShadows.h"

#include "SlipModelExtract.h"
#include <SlipPhysics.h>

#include "SlipNavMesh.h"

int main1()
{
	SlipWindow* window = new SlipWindow("SlipEditor", 800, 600);

	SlipModelExtract akim{};
	akim.extract(IO::FileDialog::OpenFile("Obj Model (*.obj)\0*.obj"));
	//akim.extractTexture(IO::FileDialog::OpenFile("Texture (*.png)\0*.png"));
	//akim.extractCol(IO::FileDialog::OpenFile("Obj Model (*.obj)\0*.obj"));
	std::cout << "Compilado!\n";

	return 1;
}

int main()
{
	std::cout << "This works\n";

	SlipWindow* window = new SlipWindow("SlipEditor", 1280, 800);
	SlipInput* input = new SlipInput();
	SlipGlobals* globals = new SlipGlobals();
	SlipDebug* debug = new SlipDebug();
	SlipID* ids = new SlipID();
	SlipUI* ui = new SlipUI();

	SlipEditor* editor = new SlipEditor();
	editor->init();

	SlipFrameBuffer* framebuffer = new SlipFrameBuffer();
	framebuffer->init();

	SlipShadows* shadow = new SlipShadows();
	shadow->init();

	SlipPhysics* physics = new SlipPhysics();

	globals->setCamera(&editor->getViewportCamera());

	glEnable(GL_DEPTH_TEST);

	SlipLevel* level = new SlipLevel();
	level->SetCamera(editor->getViewportCamera());

	SlipNavMesh* navmesh = new SlipNavMesh();

	SlipActor* model = new SlipActor("characters/santi/santi");
	model->init();

	level->entities.push_back(model);

	/*level->terrain = new SlipTerrain("assets/textures/Mainland.png");
	level->terrain->init();*/

	ui->image("shadowmap", shadow->getDepthMap(), glm::vec2(0.75f, -0.7f)).scale = glm::vec2(1.f, 15.f);

	auto texto = ui->text("test", "Dssadasdasfasfsa", glm::vec2(28.f, 0.f));

	SlipCamera* cam = new SlipCamera();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int frames = 0;
	double t, t0, fps;

	t0 = glfwGetTime();

	//physics->setGravity(glm::vec3(0.f, -9.21f, 0.f));

	editor->loadIcons();

	while (!glfwWindowShouldClose(&window->getWindow()))
	{
		glfwPollEvents();
		glfwSwapBuffers(&window->getWindow());

		editor->startRender();
		editor->renderHierarchy();
		editor->renderGuizmo();
		editor->renderProperties();
		//editor->renderViewport(framebuffer);  
		editor->renderScene();
		editor->renderDebug();
		editor->processInput();

		cam->drawDebug();

		globals->update(glfwGetTime());

		editor->getViewportCamera().Update();

		globals->setProjection(editor->getViewportCamera().GetProjectionMatrix());
		globals->setView(editor->getViewportCamera().GetViewMatrix());

		level->scripts();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		shadow->calculateLight();

		shadow->configure();

		shadow->bind();

		level->draw();

		shadow->unbind();

		globals->setProjection(editor->getViewportCamera().GetProjectionMatrix());
		globals->setView(editor->getViewportCamera().GetViewMatrix());

		framebuffer->bind();

		glViewport(0, 0, window->getWidth(), window->getHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (editor->wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			level->draw();

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			level->draw();
		}

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		ui->draw();

		glDisable(GL_BLEND);

		framebuffer->unbind();

		glClearColor(.0f, .0f, .0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		framebuffer->draw();

		editor->endRender();

		t = glfwGetTime();

		if ((t - t0) > 1.0 || frames == 0)
		{
			fps = (double)frames / (t - t0);
			texto->text = "Fps: " + std::to_string((int)fps);
			//std::printf("Fps: %.1f\r", fps);
			t0 = t;
			frames = 0;
		}
		frames++;

		//editor->print("Fps: " + std::to_string(fps));
	}
	framebuffer->clean();
	level->clean();

	glfwTerminate();

	return 1;
}