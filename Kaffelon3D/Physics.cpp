#include "Physics.h"


Physics::Physics(Timer* mainTimer)
{
	lastTimeStep = mainTimer->GetTime();
}
// Finds the intersection between a line segment and a plane.
// @param[in] p0 Starting point of line segment.
// @param[in] p1 End point of line segment.
// @param[in] normal Normal vector of the plane.
// @param[in] planePoint any point on the plane.
std::tuple<bool, glm::vec3> Physics::LineSegIntersectPlane(glm::vec3 p0, glm::vec3 p1, glm::vec3 normal, glm::vec3 planePoint)
{
	glm::vec3 lineDir = glm::normalize(p1-p0);
	float dotLineNorm = glm::dot(normal, lineDir);

	// Only care about planes facing the ray (Determined by negative dot product).
	if (dotLineNorm < -1e-6)
	{
		// We solve for the collision point by putting the ray in parametric form (StartingPoint + Direction * t = intersection)
		// A line segment on the plane can formed from two points on the plane. The dot product of this line and the plane normal is zero since they are parallel.
		// One point on the plane will be the intersection, which we can substitute with our parametric form ray.
		// We solve for t then plug t back into the ray equation.
		
		// After algebra
		float t = glm::dot(planePoint - p0, normal) / glm::dot(lineDir, normal);

		if(t > 1e-6)
			return std::tuple<bool,glm::vec3>(true, p0 + (t * lineDir));
		else
			return std::tuple<bool, glm::vec3>(false, glm::vec3(0));
	}
	else
		return std::tuple<bool, glm::vec3>(false, glm::vec3(0));
}

void Physics::Update(std::vector<GameObject*>* gameObjects, Player* player, bool lastOnGround, Timer* mainTimer)
{
	
	mainTimer->GetTime();
	std::chrono::duration<double> delta = mainTimer->GetTime() - lastTimeStep;
	if (delta.count() > physicsStepDelta) {
		double remainder = delta.count() - physicsStepDelta;
		lastTimeStep = mainTimer->GetTime() + std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>(remainder));
		player->SetDesiredPos(player->GetPosition() + player->GetVelocity() * (float)delta.count());
		PlayerConvexCollision(gameObjects, player, lastOnGround);
		player->MoveToDesired();
	}
}

float CalculateAngle(glm::vec3 line, glm::vec3 line2)
{

	float upNormDot = glm::dot(line, line2);

	float length1 = glm::length(line);
	float length2 = glm::length(line2);

	float vectorProduct = length1 * length2;

	float cosTheta = upNormDot / (vectorProduct);

	return acos(cosTheta);
}

void CalculateOverlap(float point, float boundMin, float boundMax, float* overlap, glm::vec3* smallestAxis, glm::vec3 testAxis)
{
	float newOverlap = boundMax - point;

	if ((point - boundMin) < newOverlap)
		newOverlap = point - boundMin;

	if (abs(newOverlap) < abs(*overlap))
	{
		*overlap = newOverlap;
		*smallestAxis = testAxis;
	}
}

float CalculateNewOverlap(float point, float boundMin, float boundMax)
{
	float newOverlap = boundMax - point;

	if ((point - boundMin) < newOverlap)
		newOverlap = point - boundMin;

	return newOverlap;
}

glm::vec3 NegateOverlap(float overlap, float pPos, float oPos, glm::vec3* normal)
{
	if ((pPos - oPos) < 0.0f)
	{
		*normal = *normal * -1.0f;
		//return overlap * -1.0f;
	}
	return *normal;
}

bool isOverlap(std::vector<glm::vec3> separatingAxes, glm::vec3 playerPoints[8], glm::vec3 objectPoints[8], glm::vec3 adjust, glm::vec3 oldDelta)
{
	int numIntersections = 0;
	for (int i = 0; i < separatingAxes.size(); i++)
	{
		if (separatingAxes[i] == glm::vec3(0))
		{
			numIntersections++;
			continue;
		}
		// Calculate min and max on the axis for each object.
		float pMin = glm::dot(playerPoints[0] + adjust - oldDelta, separatingAxes[i]);
		float pMax = glm::dot(playerPoints[0] + adjust - oldDelta, separatingAxes[i]);

		float oMin = glm::dot(objectPoints[0], separatingAxes[i]);
		float oMax = glm::dot(objectPoints[0], separatingAxes[i]);

		for (int x = 0; x < 8; x++)
		{
			float newPlayerProjection = glm::dot(playerPoints[x] + adjust - oldDelta, separatingAxes[i]);
			if (newPlayerProjection < pMin)
				pMin = newPlayerProjection;
			if (newPlayerProjection > pMax)
				pMax = newPlayerProjection;

			float newObjectProjection = glm::dot(objectPoints[x], separatingAxes[i]);
			if (newObjectProjection < oMin)
				oMin = newObjectProjection;
			if (newObjectProjection > oMax)
				oMax = newObjectProjection;
		}

		if (pMin >= oMin && pMin <= oMax)
		{
			numIntersections++;
		}
		else if (pMax >= oMin && pMax <= oMax)
		{
			numIntersections++;
		}
		else if (oMin >= pMin && oMin <= pMax)
		{
			numIntersections++;
		}
		else if (oMax >= pMin && oMax <= pMax)
		{
			numIntersections++;
		}
	}

	if (numIntersections == separatingAxes.size())
		return true;	
	else
		return false;
}

glm::vec3 SeparationAxes(GameObject* gameObject, glm::vec3 playerPoints[8], glm::vec3 playerDelta, glm::vec3 playerPos, Player* player)
{
	HitBox* hitbox = gameObject->GetHitBox();

	glm::vec3 objectPoints[8] = { glm::vec3(0) };
	for (int i = 0; i < 8; i++)
	{
		objectPoints[i] = hitbox->points[i] + hitbox->offset + gameObject->GetPosition();
	}

	// The 15 axes to test.
	std::vector<glm::vec3> separatingAxes;
	// Add Players axes.
	separatingAxes.push_back(glm::normalize(player->GetAxes()[0]));
	separatingAxes.push_back(glm::normalize(player->GetAxes()[1]));
	separatingAxes.push_back(glm::normalize(player->GetAxes()[2]));

	// Add Objects Axes (indices have been predetermined 0=X, 1=Z, 4=Y)

	separatingAxes.push_back(glm::normalize(hitbox->GetNormal(0)));
	separatingAxes.push_back(glm::normalize(hitbox->GetNormal(1)));
	separatingAxes.push_back(glm::normalize(hitbox->GetNormal(4)));

	// Add edge cross products.
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			glm::vec3 axis = glm::cross(player->GetEdges()[x], hitbox->edges[y]);
			if (axis != glm::vec3(0))
			{
				glm::vec3 axis = glm::cross(glm::normalize(player->GetEdges()[x]), glm::normalize(hitbox->edges[y]));
				separatingAxes.push_back(glm::normalize(axis));
			}
			else
				separatingAxes.push_back(axis);
		}
	}

	// Check for separating axes.
	// We will want to find the shortest distance to a separating axes.
	// We will save the vector and distance.

	glm::vec3 shortestAxes = separatingAxes[0];
	float distance = 999999999999.0f;

	float overlap = 99999999999.0f;
	float insideOverlap = 99999999999.0f;
	glm::vec3 insideAxis = separatingAxes[0];
	glm::vec3 smallestAxes = separatingAxes[0];

	int numIntersections = 0;

	bool inside = false;

	std::map<float, glm::vec3> smallestAxesArray;

	for (int i = 0; i < separatingAxes.size(); i++)
	{
		if (separatingAxes[i] == glm::vec3(0))
		{
			numIntersections++;
			continue;
		}

		// Calculate min and max on the axis for each object.
		float pMin = glm::dot(playerPoints[0], separatingAxes[i]);
		float pMax = glm::dot(playerPoints[0], separatingAxes[i]);

		float oMin = glm::dot(objectPoints[0], separatingAxes[i]);
		float oMax = glm::dot(objectPoints[0], separatingAxes[i]);

		for (int x = 0; x < 8; x++)
		{
			float newPlayerProjection = glm::dot(playerPoints[x], separatingAxes[i]);
			if (newPlayerProjection < pMin)
				pMin = newPlayerProjection;
			if (newPlayerProjection > pMax)
				pMax = newPlayerProjection;

			float newObjectProjection = glm::dot(objectPoints[x], separatingAxes[i]);
			if (newObjectProjection < oMin)
				oMin = newObjectProjection;
			if (newObjectProjection > oMax)
				oMax = newObjectProjection;
		}

		bool intersect = false;

		if (pMin > oMin && pMin < oMax)
		{
			intersect = true;
			CalculateOverlap(pMin, oMin, oMax, &overlap, &smallestAxes, separatingAxes[i]);
			smallestAxesArray[abs(CalculateNewOverlap(pMin, oMin, oMax))] = separatingAxes[i];

		}
		if (pMax > oMin && pMax < oMax)
		{
			intersect = true;
			CalculateOverlap(pMax, oMin, oMax, &overlap, &smallestAxes, separatingAxes[i]);
			smallestAxesArray[abs(CalculateNewOverlap(pMax, oMin, oMax))] = separatingAxes[i];
		}
		if (oMin > pMin && oMin < pMax)
		{
			intersect = true;
			CalculateOverlap(oMin, pMin, pMax, &overlap, &smallestAxes, separatingAxes[i]);
			smallestAxesArray[abs(CalculateNewOverlap(oMin, pMin, pMax))] = separatingAxes[i];
		}
		if (oMax > pMin && oMax < pMax)
		{
			intersect = true;
			CalculateOverlap(oMax, pMin, pMax, &overlap, &smallestAxes, separatingAxes[i]);
			smallestAxesArray[abs(CalculateNewOverlap(oMax, pMin, pMax))] = separatingAxes[i];
		}

		if (intersect)
		{
			numIntersections++;
		}
	}

	// If we have an intersection
	if (numIntersections == separatingAxes.size())
	{
		smallestAxes = NegateOverlap(overlap, glm::dot(player->GetPosition(), smallestAxes), glm::dot(gameObject->GetPosition(), smallestAxes), &smallestAxes);

		float partAlongNormal = glm::dot(-smallestAxes, playerDelta);
		glm::vec3 delta = playerDelta + (partAlongNormal * smallestAxes);

		while (isOverlap(separatingAxes, playerPoints, objectPoints, delta, playerDelta) && smallestAxesArray.size() > 1)
		{
			smallestAxes = smallestAxesArray.begin()->second;
			overlap = smallestAxesArray.begin()->first;
			smallestAxes = NegateOverlap(overlap, glm::dot(player->GetPosition(), smallestAxes), glm::dot(gameObject->GetPosition(), smallestAxes), &smallestAxes);
			smallestAxesArray.erase(smallestAxesArray.begin());

			float partAlongNormal = glm::dot(-smallestAxes, playerDelta);
			delta = playerDelta + (partAlongNormal * smallestAxes);
		}
		if (smallestAxesArray.size() == 1)
		{
			smallestAxes = smallestAxesArray.begin()->second;
			overlap = smallestAxesArray.begin()->first;
			smallestAxes = NegateOverlap(overlap, glm::dot(player->GetPosition(), smallestAxes), glm::dot(gameObject->GetPosition(), smallestAxes), &smallestAxes);
			//smallestAxesArray.erase(smallestAxesArray.begin());

			float partAlongNormal = glm::dot(-smallestAxes, playerDelta);
			delta = playerDelta + (partAlongNormal * smallestAxes);
		}
		return smallestAxes;
	}
	return glm::vec3(0);
}

bool PointInBounds(glm::vec3 point, glm::vec3 min, glm::vec3 max)
{
	if ((point.x >= min.x && point.x <= max.x) &&
		(point.y >= min.y && point.y < max.y) &&
		(point.z >= min.z && point.z < max.z))
	{
		return true;
	}
	return false;
}

glm::vec3 CheckCollisions(std::vector<GameObject*>* objects, Player* player, glm::vec3 deltaPos)
{
	glm::vec3 pPoints[8] = { glm::vec3(0) };

	for (int i = 0; i < 8; i++)
	{
		pPoints[i] = player->GetPoint(i) + player->GetPosition() + deltaPos;
	}

	for (GameObject* it : *objects)
	{
		if (it->IsCollider() == false)
		{
			continue;
		}
		glm::vec3 adjust = SeparationAxes(it, pPoints, deltaPos, player->GetPosition(), player);

		if (adjust != glm::vec3(0))
		{
			return adjust;;
		}

	}
	return glm::vec3(0);

}

glm::vec3 Physics::PlayerConvexCollision(std::vector<GameObject*>* objects, Player* player, bool last)
{
	glm::vec3 deltaPos = player->GetDesiredPosition() - player->GetPosition();
	bool yUp = false;
	player->PlayerInAir();
	player->SetPlayerCeiling(false);
	std::set<GameObject*> objectsCollided;
	int collisionCount = 0;
	bool onGround = false;

	glm::vec3 lastDelta = deltaPos;
	glm::vec3 yNormal = glm::vec3(0);
	glm::vec3 up = glm::vec3(0, 1, 0);

	while (collisionCount < MaxCollisions)
	{
		glm::vec3 normal = glm::vec3(0);
		normal = CheckCollisions(objects, player, deltaPos);
		if (normal != glm::vec3(0))
		{
			collisionCount++;
			float partAlongNormal = glm::dot(-normal, deltaPos);
			deltaPos = deltaPos + (partAlongNormal * normal);

			if (normal.y > 0)
			{
				float dot = glm::dot(normal, up);
				float cosineAngle = dot / (glm::length(normal) * glm::length(up));

				if (glm::degrees(acos(cosineAngle)) < MaxWalkAngle)
				{
					onGround = true;
				}
			}
		}
		else
		{
			break;
		}
	}
	
	if (onGround)
	{
		player->SetOnGround();
	}
	if (collisionCount >= MaxCollisions)
	{
		if (deltaPos.y < 0)
		{
			player->SetOnGround();
		}
		player->SetDesiredPos(player->GetPosition());
	}
	else
	{
		player->SetDesiredPos(player->GetPosition() + deltaPos);
	}
	return glm::vec3(0);
}



/// <summary>
/// 
/// </summary>
/// <param name="face">0=LOWX, 1=HIGHX, 2 = LOWZ, 3 = HIGHZ, 4 = LOWY, 5 = HIGHY</param>
/// <param name="object"></param>
/// <param name="lineStart"></param>
/// <param name="lineEnd"></param>
/// <returns></returns>
std::tuple<bool, glm::vec3> Physics::IntersectObjectFace(char face, GameObject* object, glm::vec3 lineStart, glm::vec3 lineEnd)
{
	std::tuple<bool, glm::vec3> result = std::tuple<bool, glm::vec3>(false, glm::vec3(0));
	glm::vec3 min = object->GetHitBoxMin();
	glm::vec3 max = object->GetHitBoxMax();
	if (face == '0') {
		glm::vec3 planePoint = glm::vec3(object->GetHitBoxMin().x, object->GetPosition().y, object->GetPosition().z);
		std::tuple<bool, glm::vec3> intersection = Physics::LineSegIntersectPlane(lineStart, lineEnd, object->GetHitBox()->xMinNormal, planePoint);
		glm::vec3 intersect = std::get<1>(intersection);
		

		if (std::get<0>(intersection) &&
			intersect.y > min.y && intersect.y < max.y && intersect.z > min.z && intersect.z < max.z)
		{
			result = std::tuple<bool, glm::vec3>(true, intersect);
			return result;
		}
	}
	else if (face == '1'){
		glm::vec3 planePoint = glm::vec3(object->GetHitBoxMax().x, object->GetPosition().y, object->GetPosition().z);
		std::tuple<bool, glm::vec3> intersection = Physics::LineSegIntersectPlane(lineStart, lineEnd, object->GetHitBox()->xMaxNormal, planePoint);
		glm::vec3 intersect = std::get<1>(intersection);

		if (std::get<0>(intersection) &&
			intersect.y > min.y && intersect.y < max.y && intersect.z > min.z && intersect.z < max.z)
		{
			result = std::tuple<bool, glm::vec3>(true, intersect);
			return result;
		}
	}
	else if (face == '2'){
		glm::vec3 planePoint = glm::vec3(object->GetPosition().x, object->GetPosition().y, object->GetHitBoxMin().z);
		std::tuple<bool, glm::vec3> intersection = Physics::LineSegIntersectPlane(lineStart, lineEnd, object->GetHitBox()->zMinNormal, planePoint);
		glm::vec3 intersect = std::get<1>(intersection);

		if (std::get<0>(intersection) &&
			intersect.x > min.x && intersect.x < max.x && intersect.y > min.y && intersect.y < max.y)
		{
			result = std::tuple<bool, glm::vec3>(true, intersect);
			return result;
		}
	}
	else if (face == '3') {
		glm::vec3 planePoint = glm::vec3(object->GetPosition().x, object->GetPosition().y, object->GetHitBoxMax().z);
		std::tuple<bool, glm::vec3> intersection = Physics::LineSegIntersectPlane(lineStart, lineEnd, object->GetHitBox()->zMaxNormal, planePoint);
		glm::vec3 intersect = std::get<1>(intersection);

		if (std::get<0>(intersection) &&
			intersect.x > min.x && intersect.x < max.x && intersect.y > min.y && intersect.y < max.y)
		{
			result = std::tuple<bool, glm::vec3>(true, intersect);
			return result;
		}
	}
	else if (face == '4'){
		glm::vec3 planePoint = glm::vec3(object->GetPosition().x, object->GetHitBoxMin().y, object->GetPosition().z);
		std::tuple<bool, glm::vec3> intersection = Physics::LineSegIntersectPlane(lineStart, lineEnd, object->GetHitBox()->yMinNormal, planePoint);
		glm::vec3 intersect = std::get<1>(intersection);

		if (std::get<0>(intersection) &&
			intersect.x > min.x && intersect.x < max.x && intersect.z > min.z && intersect.z < max.z)
		{
			result = std::tuple<bool, glm::vec3>(true, intersect);
			return result;
		}
	}
	else if (face == '5') {
		glm::vec3 planePoint = glm::vec3(object->GetPosition().x, object->GetHitBoxMax().y, object->GetPosition().z);
		std::tuple<bool, glm::vec3> intersection = Physics::LineSegIntersectPlane(lineStart, lineEnd, object->GetHitBox()->yMaxNormal, planePoint);
		glm::vec3 intersect = std::get<1>(intersection);

		if (std::get<0>(intersection) &&
			intersect.x > min.x && intersect.x < max.x && intersect.z > min.z && intersect.z < max.z)
		{
			result = std::tuple<bool, glm::vec3>(true, intersect);
			return result;
		}
	}
	return result;
}

GameObject* Physics::MouseOverObjects(std::vector<GameObject*>* objects, Player* player)
{
	float distance = 100;
	glm::vec3 lineStart = player->GetPosition();
	glm::vec3 lineEnd = player->GetControls()->GetLookDirection();
	lineEnd = glm::normalize(lineEnd) * distance + lineStart;
	glm::vec3 lineMin = glm::vec3(0);
	glm::vec3 lineMax = glm::vec3(0);
	lineEnd = glm::normalize(lineEnd) * distance;

	std::tuple<GameObject*, float> Closest = std::tuple<GameObject*, float>(nullptr, 0);

	if (lineStart.x < lineEnd.x) {
		lineMin.x = lineStart.x;
		lineMax.x = lineEnd.x;
	}
	else {
		lineMin.x = lineEnd.x;
		lineMax.x = lineStart.x;
	}
	if (lineStart.y < lineEnd.y) {
		lineMin.y = lineStart.y;
		lineMax.y = lineEnd.y;
	}
	else {
		lineMin.y = lineEnd.y;
		lineMax.y = lineStart.y;
	}

	if (lineStart.z < lineEnd.z) {
		lineMin.z = lineStart.z;
		lineMax.z = lineEnd.z;
	}
	else {
		lineMin.z = lineEnd.z;
		lineMax.z = lineStart.z;
	}

	for (auto it : *objects)
	{
		glm::vec3 min = it->GetHitBoxMin();
		glm::vec3 max = it->GetHitBoxMax();
		if ((min.x < lineMax.x || max.x > lineMin.x) &&
			(min.y < lineMax.y || max.y > lineMin.y) &&
			(min.z < lineMax.z || max.z > lineMin.z))
		{
			std::tuple<bool, glm::vec3> result = IntersectObjectFace('0', it, lineStart, lineEnd);
			if (std::get<0>(result)) {
				AssignNewClosest(&Closest, lineStart, std::get<1>(result), it);
			}

			result = IntersectObjectFace('1', it, lineStart, lineEnd);
			if (std::get<0>(result)) {
				AssignNewClosest(&Closest, lineStart, std::get<1>(result), it);
			}

			result = IntersectObjectFace('2', it, lineStart, lineEnd);
			if (std::get<0>(result)) {
				AssignNewClosest(&Closest, lineStart, std::get<1>(result), it);
			}

			result = IntersectObjectFace('3', it, lineStart, lineEnd);
			if (std::get<0>(result)) {
				AssignNewClosest(&Closest, lineStart, std::get<1>(result), it);
			}

			result = IntersectObjectFace('4', it, lineStart, lineEnd);
			if (std::get<0>(result)) {
				AssignNewClosest(&Closest, lineStart, std::get<1>(result), it);
			}

			result = IntersectObjectFace('5', it, lineStart, lineEnd);
			if (std::get<0>(result)) {
				AssignNewClosest(&Closest, lineStart, std::get<1>(result), it);
			}
		}
	}

	return std::get<0>(Closest);
}

void Physics::AssignNewClosest(std::tuple<GameObject*, float>* Closest, glm::vec3 lineStart, glm::vec3 intersect, GameObject* object)
{
	float distX = intersect.x - lineStart.x;
	float distY = intersect.y - lineStart.y;
	float distZ = intersect.z - lineStart.z;

	float distance = sqrt((distX * distX) + (distY * distY) + (distZ * distZ));

	if (std::get<0>(*Closest) == nullptr || std::get<1>(*Closest) > distance)
	{
		*Closest = std::tuple<GameObject*, float>(object, distance);
	}
}
float Physics::GetGravity()
{
	return gravity;
}