#include "OApplication.h"
#include "../Objects/Materials/Dielectric.h"
#include "../Objects/Sphere.h"
#include "../Objects/Materials/Lambertian.h"
#include "../Objects/Materials/Metal.h"
#include "../Types/Output/PPMSerializer.h"
#include "../Types/Profiler/Profiler.h"
#include "../Utils/Materials.h"
#include "Renderers/AsyncRenderer.h"
#include "Renderers/Renderer.h"
#include "Renderers/SyncRenderer.h"
#include "Scencens\Scenes.h"


#define INIT_SCENE(Name) \
	Scenes::OScene* scene = new Name(); \
	scene->InitScene(*this); \
	delete scene;

OApplication* OApplication::Instance = nullptr;

OApplication::OApplication()
{
	Init();
	Render();
}

void OApplication::AddSphere(const SVec3& Position, float Radius, std::shared_ptr<IMaterial> Material)
{
	ENSURE(Material != nullptr);
	World.Add(make_shared<OSphere>(Position, Radius, Material));
}

void OApplication::Init()
{
	Camera = std::make_shared<OCamera>();
	INIT_SCENE(Scenes::OLotsRandomSpheresScene);
	Camera->Init();

	Serializer = std::make_shared<OPPMSerializer>(Camera->SamplesPerPixel);
	Renderer = std::make_shared<OAsyncRenderer>(Camera->ImageWidth, Camera->ImageHeight);
}

void OApplication::Render()
{
	PROFILE();
	const auto& buffer = Renderer->Draw(Camera, World, Camera->SamplesPerPixel);
	Serializer->Serialize(buffer);
}
