#include <SDL/SDL.h>
#include <math.h>
#include <vector>
#include "vector.h"
#include "util.h"
#include "sdl.h"
#include "color.h"
#include "camera.h"
#include "geometry.h"
#include "shading.h"
#include "light.h"
#include "background.h"
using std::vector;


Color vfb[VFB_MAX_SIZE][VFB_MAX_SIZE];

Camera camera;
vector<Node> nodes;
Plane plane;
CheckerShader checker;
CheckerShader ceiling;
Plane plane2;
vector<light> sceneLights;
background sceneBackground;

void setupScene()
{
    //Light 1
    pointLight light1 = pointLight();
    light1.lightIntensity = 25000.0;
    light1.position = Vector(-215, 80, 100);
    light1.lightColor = Color(0.5, 0.3, 0.6);
    sceneLights.push_back(light1);

    //Light 2
    pointLight light2 = pointLight();
    light2.lightIntensity = 25000.0;
    light2.position = Vector(285, 80, 100);
    light2.lightColor = Color(0.1, 0.5, 0.7);
    sceneLights.push_back(light2);

    //Light 3
    pointLight light3 = pointLight();
    light3.lightIntensity = 25000.0;
    light3.position = Vector(35, 40, 100);
    light3.lightColor = Color(0.9, 0.7, 0.5);
    sceneLights.push_back(light3);

    //Light 4- behind the camera
    pointLight light4 = pointLight();
    light4.lightIntensity = 25000.0;
    light4.position = Vector(35, 40, -200);
    light4.lightColor = Color(0.8, 0.7, 0.9);
    sceneLights.push_back(light4);

    //Background color
    sceneBackground = solidColorBackground(Color(0, 0, 0.2));

	camera.position = Vector(35, 60, -200);
	camera.yaw = 0;
	camera.pitch = 0;
	camera.roll = 0;
	camera.fov = 90;
	camera.aspectRatio = float(frameWidth()) / float(frameHeight());
	plane.y = 1;
	plane2.y = 200;
	checker.color1 = Color(0, 0, 0.5);
	checker.color2 = Color(1, 0.5, 0);
	ceiling.color1 = Color(1, 1, 1);
	ceiling.color2 = Color(0.5, 0.5, 0.5);
	nodes.push_back({ &plane, &checker });
	nodes.push_back({ &plane2, &ceiling });
	camera.frameBegin();
}

Color raytrace(Ray ray)
{
	Node* closestNode = NULL;
	double closestDist = INF;
	IntersectionInfo closestInfo;
	for (auto& node: nodes) {
		IntersectionInfo info;
		if (!node.geom->intersect(ray, info)) continue;

		if (info.distance < closestDist) {
			closestDist = info.distance;
			closestNode = &node;
			closestInfo = info;
		}
	}
	// check if we hit the sky:
	if (closestNode == NULL)
		return sceneBackground.color;
	else
		return closestNode->shader->shade(ray, closestInfo);
}

void render()
{
	for (int y = 0; y < frameHeight(); y++)
		for (int x = 0; x < frameWidth(); x++) {
			Ray ray = camera.getScreenRay(x, y);
			vfb[y][x] = raytrace(ray);
		}
}

int main ( int argc, char** argv )
{
	initGraphics(RESX, RESY);
	setupScene();
	render();
	displayVFB(vfb);
	waitForUserExit();
	closeGraphics();
	printf("Exited cleanly\n");
	return 0;
}
