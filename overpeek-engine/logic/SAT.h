#pragma once
//#include "../engine.h"
#include <glm/glm.hpp>

//using namespace graphics;
namespace logic {

	bool AABB(glm::fvec2 aPos, glm::fvec2 aSize, glm::fvec2 bPos, glm::fvec2 bSize) {
		return	bPos.x < aPos.x + aSize.x && aPos.x < bPos.x + bSize.x
			&&  bPos.y < aPos.y + aSize.y && aPos.y < bPos.y + bSize.y;
	}

	glm::vec2 closestPointOnLine(glm::vec2 A, glm::vec2 B, glm::vec2 P) {
		glm::vec2 AB = B - A;
		glm::vec2 AP = P - A;
		float lengthSqrAB = AB.x * AB.x + AB.y * AB.y;
		float t = (AP.x * AB.x + AP.y * AB.y) / lengthSqrAB;
		
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f; 
		
		return A + t * AB;
	}

	//void minXYmaxXY(float x, float y, float &maxX, float &maxY, float &minX, float &minY, float rotation, float rotationAxis, float dstX, float dstY, std::vector<glm::vec2> points) {
	//	for (int i = 0; i < points.size(); i++)
	//	{
	//		glm::vec2 p = points[i];
	//
	//		p = rotatePoint(0.0f, 0.0f, rotation, p);
	//		p.x += x; p.y += y;
	//		p.x -= dstX; p.y -= dstY;
	//		p = rotatePoint(x, y, -rotationAxis, p);
	//
	//		if (p.x > maxX) maxX = p.x;
	//		if (p.x < minX) minX = p.x;
	//		if (p.y > maxY) maxY = p.y;
	//		if (p.y < minY) minY = p.y;
	//	}
	//}
	//
	////AXIS ROTATION IS FROM FIRST LAYER
	//bool SAT(ObjectLayer *layerA, ObjectLayer *layerB, float axisRotation) {
	//	float axelX = (layerA->getX() + layerB->getX())/2;
	//	float axelY = (layerA->getY() + layerB->getY())/2;
	//
	//	float xAtoB = layerA->getX() - layerB->getX();
	//	float yAtoB = layerA->getY() - layerB->getY();
	//	float maxX1 = -1000.0f, minX1 = 1000.0f;
	//	float maxY1 = -1000.0f, minY1 = 1000.0f;
	//	float maxX2 = -1000.0f, minX2 = 1000.0f;
	//	float maxY2 = -1000.0f, minY2 = 1000.0f;
	//
	//	minXYmaxXY(axelX, axelY, maxX1, maxY1, minX1, minY1, layerA->getRotation(), axisRotation, 0.0f, 0.0f, layerA->getPoints());
	//	minXYmaxXY(axelX, axelY, maxX2, maxY2, minX2, minY2, layerB->getRotation(), axisRotation, xAtoB, yAtoB, layerB->getPoints());
	//	
	//	if (maxX1 < minX2) return false;
	//	if (minX1 > maxX2) return false;
	//	
	//	if (maxY1 < minY2) return false;
	//	if (minY1 > maxY2) return false;
	//
	//	return true;
	//}
	//
	//bool boxSAT(ObjectLayer *objA, ObjectLayer *objB) {
	//	if (!SAT(objA, objB, objA->getRotation())) return false;
	//	if (!SAT(objA, objB, objB->getRotation())) return false;
	//	return true;
	//}
	//
	//bool circleSAT(ObjectLayer *objA, ObjectLayer *objB) {
	//	return true;
	//}

}