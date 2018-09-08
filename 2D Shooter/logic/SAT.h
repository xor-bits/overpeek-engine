#pragma once


#include "../graphics/layers/objectLayer.h"

using namespace graphics;
namespace logic {

	glm::vec2 closestPointOnLine(glm::vec2 A, glm::vec2 B, glm::vec2 P) {
		glm::vec2 AB = B - A;
		glm::vec2 AP = P - A;
		float lengthSqrAB = AB.x * AB.x + AB.y * AB.y;
		float t = (AP.x * AB.x + AP.y * AB.y) / lengthSqrAB;
		
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f; 
		
		return A + t * AB;
	}

	glm::vec2 rotatePoint(float cx, float cy, float angle, glm::vec2 p)
	{
		float s = sin(angle);
		float c = cos(angle);

		// translate point back to origin:
		p.x -= cx;
		p.y -= cy;

		// rotate point
		float xnew = p.x * c - p.y * s;
		float ynew = p.x * s + p.y * c;

		// translate point back:
		p.x = xnew + cx;
		p.y = ynew + cy;
		return p;
	}

	void minXYmaxXY(float x, float y, float &maxX, float &maxY, float &minX, float &minY, float rotation, float rotationAxis, float dstX, float dstY, std::vector<glm::vec2> points) {
		for (int i = 0; i < points.size(); i++)
		{
			glm::vec2 p = points[i];

			p = rotatePoint(0.0f, 0.0f, rotation, p);
			p.x += x; p.y += y;
			p.x -= dstX; p.y -= dstY;
			p = rotatePoint(x, y, -rotationAxis, p);

			if (p.x > maxX) maxX = p.x;
			if (p.x < minX) minX = p.x;
			if (p.y > maxY) maxY = p.y;
			if (p.y < minY) minY = p.y;
		}
	}

	//AXIS ROTATION IS FROM FIRST LAYER
	bool SAT(ObjectLayer *layerA, ObjectLayer *layerB, float axisRotation) {
		float axelX = (layerA->getX() + layerB->getX())/2;
		float axelY = (layerA->getY() + layerB->getY())/2;

		float xAtoB = layerA->getX() - layerB->getX();
		float yAtoB = layerA->getY() - layerB->getY();
		float maxX1 = -1000.0f, minX1 = 1000.0f;
		float maxY1 = -1000.0f, minY1 = 1000.0f;
		float maxX2 = -1000.0f, minX2 = 1000.0f;
		float maxY2 = -1000.0f, minY2 = 1000.0f;

		minXYmaxXY(axelX, axelY, maxX1, maxY1, minX1, minY1, layerA->getRotation(), axisRotation, 0.0f, 0.0f, layerA->getPoints());
		minXYmaxXY(axelX, axelY, maxX2, maxY2, minX2, minY2, layerB->getRotation(), axisRotation, xAtoB, yAtoB, layerB->getPoints());
		
		if (maxX1 < minX2) return false;
		if (minX1 > maxX2) return false;
		
		if (maxY1 < minY2) return false;
		if (minY1 > maxY2) return false;

		return true;
	}

	bool boxSAT(ObjectLayer *objA, ObjectLayer *objB) {
		if (!SAT(objA, objB, objA->getRotation())) return false;
		if (!SAT(objA, objB, objB->getRotation())) return false;
		return true;
	}

	bool circleSAT(ObjectLayer *objA, ObjectLayer *objB) {

	}

}