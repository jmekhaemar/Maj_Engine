#include "Test.h"

#include <Vec2.h>

#define GLM_FORCE_LEFT_HANDED
#include <glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS

#include <gtx/vector_angle.hpp>
#include <gtx/compatibility.hpp>

TEST_CLASS("Vec2")
{
	TEST_METHOD("constructor")
	{
		maj::math::Vec2<float> vector;
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));

		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		IsTrue(maj::math::Helper<float>::areSame(vector1.mX, 10.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector1.mY, 10.0f));

		maj::math::Vec2<float> vector2(maj::math::Vec2<float>::skRight);
		IsTrue(maj::math::Helper<float>::areSame(vector2.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector2.mY, 0.0f));

		maj::math::Vec2<float> vector3(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		IsTrue(maj::math::Helper<float>::areSame(vector3.mX, std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(vector3.mY, std::numeric_limits<float>::max()));
	}

	TEST_METHOD("norm")
	{
		maj::math::Vec2<float> vector(10.0f, 10.0f);
		glm::vec2 vec(10.0f, 10.0f);
		IsTrue(maj::math::Helper<float>::areSame(vector.norm(), glm::length(vec)));

		vector = maj::math::Vec2<float>(10.5f, 10.25f);
		vec = glm::vec2(10.5f, 10.25f);
		IsTrue(maj::math::Helper<float>::areSame(vector.norm(), glm::length(vec)));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		IsTrue(isinf(vector.norm()));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		IsTrue(isinf(vector.norm()));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		IsTrue(isinf(vector.norm()));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		IsTrue(isinf(vector.norm()));

	}

	TEST_METHOD("sqr norm")
	{
		maj::math::Vec2<float> vector(10.0f, 10.0f);
		glm::vec2 vec(10.0f, 10.0f);
		AreEqual(vector.sqrNorm(), glm::length2(vec), FLT_EPSILON);

		vector = maj::math::Vec2<float>(10.5f, 10.25f);
		vec = glm::vec2(10.5f, 10.25f);
		AreEqual(vector.sqrNorm(), glm::length2(vec), FLT_EPSILON);

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		IsTrue(isinf(vector.sqrNorm()));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		IsTrue(isinf(vector.sqrNorm()));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		IsTrue(isinf(vector.sqrNorm()));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		IsTrue(isinf(vector.sqrNorm()));

	}

	TEST_METHOD("normalized")
	{
		maj::math::Vec2<float> vector;
		glm::vec2 ve(0.0f, 0.0f);
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mY, 0.0f));

		vector = maj::math::Vec2<float>(10.0f, 10.0f);
		ve = glm::vec2(10.0f, 10.0f);
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mX, glm::normalize(ve).x));
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mY, glm::normalize(ve).y));

		vector = maj::math::Vec2<float>(10.5f, 10.25f);
		ve = glm::vec2(10.5f, 10.25f);
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mX, glm::normalize(ve).x));
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mY, glm::normalize(ve).y));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mY, 0.f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mY, 0.f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mY, 0.f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mY, 0.f));

	}

	TEST_METHOD("angle")
	{

		maj::math::Vec2<float> vector;
		glm::vec2 vec(0.0f, 0.0f);
		AreEqual(vector.angle(vector), 0.0f, FLT_EPSILON);

		maj::math::Vec2<float> vector0(10.f, 10.f);
		glm::vec2 vec0(10.f, 10.f);
		AreEqual(vector0.angle(vector), 0.0f, FLT_EPSILON);
		AreEqual(vector.angle(vector0), 0.0f, FLT_EPSILON);

		maj::math::Vec2<float> vector1(52.f, 1.f);
		glm::vec2 vec1(52.f, 1.f);
		AreEqual(vector0.angle(vector), glm::angle(vec0, vec1), FLT_EPSILON);

		AreEqual(vector1.angle(vector), 0.0f, FLT_EPSILON);
		AreEqual(vector.angle(vector1), 0.0f, FLT_EPSILON);

		maj::math::Vec2<float> vector2(10.5f, 10.25f);
		maj::math::Vec2<float> vector3(52.7f, 1.69f);

		glm::vec2 vec2(10.5f, 10.25f);
		glm::vec2 vec3(52.7f, 1.69f);
		vec2 = glm::normalize(vec2);
		vec3 = glm::normalize(vec3);
		float angleMe = vector2.angle(vector3);
		float angleGLM = glm::degrees(glm::angle(vec2, vec3));
		IsTrue(maj::math::Helper<float>::areSame(angleMe, angleGLM));
	}

	TEST_METHOD("inversed")
	{
		maj::math::Vec2<float> vector;
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mY, 0.0f));

		vector = maj::math::Vec2<float>(10.0f, 10.0f);
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mX, 0.1f));
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mY, 0.1f));

		vector = maj::math::Vec2<float>(10.5f, 10.25f);
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mX, 0.09523809523809523f));
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mY, 0.0975609756097561f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mY, 0.0f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mY, 0.0f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mY, 0.0f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mY, 0.0f));
	}

	TEST_METHOD("normalize")
	{
		maj::math::Vec2<float> vector;
		vector.normalize();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));

		vector = maj::math::Vec2<float>(10.0f, 10.0f);
		vector.normalize();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.7071067811865475f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.7071067811865475f));

		vector = maj::math::Vec2<float>(10.5f, 10.25f);
		vector.normalize();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.7155742058807848f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.6985367247883851f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		vector.normalize();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		vector.normalize();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		vector.normalize();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		vector.normalize();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.f));
	}

	TEST_METHOD("dot")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.007f, 1.69f);
		maj::math::Vec2<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector6(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec2 vec0;
		glm::vec2 vec1(10.0f, 10.0f);
		glm::vec2 vec2(52.0f, 1.0f);
		glm::vec2 vec3(10.5f, 10.25f);
		glm::vec2 vec4(52.007f, 1.69f);

		AreEqual(vector0.dot(vector1), glm::dot(vec0, vec1), 1E-5);
		AreEqual(vector1.dot(vector2), glm::dot(vec1, vec2), 1E-5);

		AreEqual(vector3.dot(vector4), glm::dot(vec3, vec4), 1E-5);

		AreEqual(glm::dot(vec3, vec3), vector3.dot(vector3), FLT_EPSILON);
		AreEqual(glm::dot(vec3, -vec3), vector3.dot(-vector3), FLT_EPSILON);

		IsTrue(isinf(vector5.dot(vector6)));
		IsTrue(isinf(vector7.dot(vector8)));

	}

	TEST_METHOD("inverse")
	{
		maj::math::Vec2<float> vector;
		vector.inverse();
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.inversed().mY, 0.0f));

		vector = maj::math::Vec2<float>(10.0f, 10.0f);
		vector.inverse();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.1f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.1f));

		vector = maj::math::Vec2<float>(10.5f, 10.25f);
		vector.inverse();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.09523809523809523f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0975609756097561f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		vector.inverse();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		vector.inverse();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		vector.inverse();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		vector.inverse();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));

	}

	TEST_METHOD("clamp")
	{
		maj::math::Vec2<float> vector;
		IsTrue(maj::math::Helper<float>::areSame(vector.clamp(maj::math::Vec2<float>(1.f, 2.f), maj::math::Vec2<float>(10.f, 11.f)).mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.clamp(maj::math::Vec2<float>(1.f, 2.f), maj::math::Vec2<float>(10.f, 11.f)).mY, 2.0f));

		vector = maj::math::Vec2<float>(10.0f, 10.0f);
		IsTrue(maj::math::Helper<float>::areSame(vector.clamp(maj::math::Vec2<float>(1.f, 2.f), maj::math::Vec2<float>(9.f, 9.f)).mX, 9.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.clamp(maj::math::Vec2<float>(1.f, 2.f), maj::math::Vec2<float>(9.f, 9.f)).mY, 9.0f));

		vector = maj::math::Vec2<float>(10.5f, 10.25f);
		IsTrue(maj::math::Helper<float>::areSame(vector.clamp(maj::math::Vec2<float>(1.f, 2.f), maj::math::Vec2<float>(10.25f, 10.24f)).mX, 10.25f));
		IsTrue(maj::math::Helper<float>::areSame(vector.clamp(maj::math::Vec2<float>(1.f, 2.f), maj::math::Vec2<float>(10.25f, 10.24f)).mY, 10.24f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		IsTrue(maj::math::Helper<float>::areSame(vector.clamp(maj::math::Vec2<float>(1.f, 2.f), maj::math::Vec2<float>(10.25f, 9.f)).mX, 1.f));
		IsTrue(maj::math::Helper<float>::areSame(vector.clamp(maj::math::Vec2<float>(1.f, 2.f), maj::math::Vec2<float>(10.25f, 9.f)).mY, 9.0f));

		vector = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		IsTrue(maj::math::Helper<float>::areSame(vector.clamp(maj::math::Vec2<float>(1.f, std::numeric_limits<float>::lowest()), maj::math::Vec2<float>(std::numeric_limits<float>::max(), 120.f)).mX, 1.f));
		IsTrue(maj::math::Helper<float>::areSame(vector.clamp(maj::math::Vec2<float>(1.f, std::numeric_limits<float>::lowest()), maj::math::Vec2<float>(std::numeric_limits<float>::max(), 120.f)).mY, 120.f));

	}

	TEST_METHOD("* float")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);
		maj::math::Vec2<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector6(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec2 vec0;
		glm::vec2 vec1(10.0f, 10.0f);
		glm::vec2 vec2(52.0f, 1.0f);
		glm::vec2 vec3(10.5f, 10.25f);
		glm::vec2 vec4(52.7f, 1.69f);

		maj::math::Vec2<float> res = vector0 * 10.f;
		glm::vec2 resGLM = vec0 * 10.f;
		IsTrue(maj::math::Helper<float>::areSame(res.mX, resGLM.x));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, resGLM.y));

		res = vector1 * 10.f;
		resGLM = vec1 * 10.f;
		IsTrue(maj::math::Helper<float>::areSame(res.mX, resGLM.x));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, resGLM.y));

		res = vector3 * 10.f;
		resGLM = vec3 * 10.f;
		IsTrue(maj::math::Helper<float>::areSame(res.mX, resGLM.x));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, resGLM.y));

		res = vector2 * PI;
		resGLM = vec2 * PI;
		IsTrue(maj::math::Helper<float>::areSame(res.mX, resGLM.x));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, resGLM.y));

		res = vector4 * PI;
		resGLM = vec4 * PI;
		IsTrue(maj::math::Helper<float>::areSame(res.mX, resGLM.x));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, resGLM.y));

		res = vector1 * 0.f;
		resGLM = vec1 * 0.0f;
		IsTrue(maj::math::Helper<float>::areSame(res.mX, resGLM.x));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, resGLM.y));

		res = vector3 * 0.f;
		resGLM = vec3 * 0.0f;
		AreEqual(resGLM.x, res.mX, FLT_EPSILON);
		AreEqual(resGLM.x, res.mY, FLT_EPSILON);


		res = vector5 * std::numeric_limits<float>::lowest();
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		res = vector6 * std::numeric_limits<float>::max();
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		res = vector7 * std::numeric_limits<float>::lowest();
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		res = vector8 * std::numeric_limits<float>::max();
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
	}

	TEST_METHOD("/ float")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);
		maj::math::Vec2<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector6(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec2 vec0;
		glm::vec2 vec1(10.0f, 10.0f);
		glm::vec2 vec2(52.0f, 1.0f);
		glm::vec2 vec3(10.5f, 10.25f);
		glm::vec2 vec4(52.7f, 1.69f);

		maj::math::Vec2<float> res = vector0 / 10.f;
		glm::vec2 resGLM = vec0 / 10.f;
		AreEqual(resGLM.x, res.mX, 10E-5);
		AreEqual(resGLM.y, res.mY, 10E-5);

		res = vector1 / 10.f;
		resGLM = vec1 / 10.f;
		AreEqual(resGLM.x, res.mX, 10E-5);
		AreEqual(resGLM.y, res.mY, 10E-5);


		res = vector3 / 10.f;
		resGLM = vec3 / 10.f;
		AreEqual(resGLM.x, res.mX, 10E-5);
		AreEqual(resGLM.y, res.mY, 10E-5);

		res = vector2 / PI;
		resGLM = vec2 / PI;
		AreEqual(resGLM.x, res.mX, 10E-5);
		AreEqual(resGLM.y, res.mY, 10E-5);

		res = vector4 / PI;
		resGLM = vec4 / PI;
		AreEqual(resGLM.x, res.mX, 10E-5);
		AreEqual(resGLM.y, res.mY, 10E-5);


		res = vector5 / std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, -1.0f));
		res = vector6 / std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, -1.0f));
		res = vector7 / std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, 1.0f));
		res = vector8 / std::numeric_limits<float>::max();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, 1.0f));
	}

	TEST_METHOD("+ Vec2")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);
		maj::math::Vec2<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec2 vec0;
		glm::vec2 vec1(10.0f, 10.0f);
		glm::vec2 vec2(52.0f, 1.0f);
		glm::vec2 vec3(10.5f, 10.25f);
		glm::vec2 vec4(52.7f, 1.69f);

		maj::math::Vec2<float> res = vector0 + vector1;
		glm::vec2 resGLM = vec0 + vec1;
		AreEqual(resGLM.x, res.mX, FLT_EPSILON);
		AreEqual(resGLM.y, res.mY, FLT_EPSILON);

		res = vector1 + ( -vector2 );
		resGLM = vec1 + ( -vec2 );
		AreEqual(resGLM.x, res.mX, FLT_EPSILON);
		AreEqual(resGLM.y, res.mY, FLT_EPSILON);

		res = vector3 + vector1;
		resGLM = vec3 + vec1;
		AreEqual(resGLM.x, res.mX, FLT_EPSILON);
		AreEqual(resGLM.y, res.mY, FLT_EPSILON);

		res = vector5 + vector6;
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 0.f));;
		IsTrue(maj::math::Helper<float>::areSame(res.mY, 0.f));
		res = vector7 + vector8;
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 0.f));;
		IsTrue(maj::math::Helper<float>::areSame(res.mY, 0.f));
	}

	TEST_METHOD("- Vec2")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);
		maj::math::Vec2<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec2 vec0;
		glm::vec2 vec1(10.0f, 10.0f);
		glm::vec2 vec2(52.0f, 1.0f);
		glm::vec2 vec3(10.5f, 10.25f);
		glm::vec2 vec4(52.7f, 1.69f);

		maj::math::Vec2<float> res = vector0 - vector1;
		glm::vec2 resGLM = vec0 - vec1;
		AreEqual(resGLM.x, res.mX, FLT_EPSILON);
		AreEqual(resGLM.y, res.mY, FLT_EPSILON);

		res = vector4 - vector1;
		resGLM = vec4 - vec1;
		AreEqual(resGLM.x, res.mX, FLT_EPSILON);
		AreEqual(resGLM.y, res.mY, FLT_EPSILON);

		res = vector3 - vector3;
		resGLM = vec3 - vec3;
		AreEqual(resGLM.x, res.mX, FLT_EPSILON);
		AreEqual(resGLM.y, res.mY, FLT_EPSILON);

		res = vector5 - std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 0.f));
		IsTrue(isinf(res.mY));
		res = vector6 - std::numeric_limits<float>::max();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 0.f));
		IsTrue(isinf(res.mY));
		res = vector7 - std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, 0.f));
		res = vector8 - std::numeric_limits<float>::max();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, 0.f));
	}

	TEST_METHOD("scale")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);
		maj::math::Vec2<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());


		maj::math::Vec2<float> res = vector0.scaled(vector1);
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, 0.0f));

		res = vector1.scaled(vector2);
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 520.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, 10.0f));

		res = vector3.scaled(vector4);
		AreEqual(res.mX, 553.350008f, FLT_EPSILON);
		AreEqual(res.mY, 17.32250059f, FLT_EPSILON);

		res = vector5.scaled(std::numeric_limits<float>::lowest());
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		res = vector6.scaled(std::numeric_limits<float>::max());
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		res = vector7.scaled(std::numeric_limits<float>::lowest());
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		res = vector8.scaled(std::numeric_limits<float>::max());
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
	}

	TEST_METHOD("-")
	{
		maj::math::Vec2<float> vector0;
		vector0 = -vector0;
		IsTrue(maj::math::Helper<float>::areSame(vector0.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector0.mY, 0.0f));


		vector0 = maj::math::Vec2<float>(10.0f, -10.0f);;
		vector0 = -vector0;
		AreEqual(vector0.mX, -10.0f, FLT_EPSILON);
		AreEqual(vector0.mY, 10.0f, FLT_EPSILON);
		vector0 = maj::math::Vec2<float>(-10.5f, 10.25f);;
		vector0 = -vector0;
		AreEqual(vector0.mX, 10.5f, FLT_EPSILON);
		AreEqual(vector0.mY, -10.25f, FLT_EPSILON);

		vector0 = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		vector0 = -vector0;
		AreEqual(vector0.mX, std::numeric_limits<float>::max(), FLT_EPSILON);
		AreEqual(vector0.mY, std::numeric_limits<float>::lowest(), FLT_EPSILON);

		vector0 = maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		vector0 = -vector0;
		AreEqual(vector0.mX, std::numeric_limits<float>::lowest(), FLT_EPSILON);
		AreEqual(vector0.mY, std::numeric_limits<float>::max(), FLT_EPSILON);

		vector0 = maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		vector0 = -vector0;
		AreEqual(vector0.mX, std::numeric_limits<float>::max(), FLT_EPSILON);
		AreEqual(vector0.mY, std::numeric_limits<float>::max(), FLT_EPSILON);

		vector0 = maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		vector0 = -vector0;
		AreEqual(vector0.mX, std::numeric_limits<float>::lowest(), FLT_EPSILON);
		AreEqual(vector0.mY, std::numeric_limits<float>::lowest(), FLT_EPSILON);

	}

	TEST_METHOD("+= Vec2")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);
		maj::math::Vec2<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());


		glm::vec2 vec0;
		glm::vec2 vec1(10.0f, 10.0f);
		glm::vec2 vec2(52.0f, 1.0f);
		glm::vec2 vec3(10.5f, 10.25f);
		glm::vec2 vec4(52.7f, 1.69f);

		vector0 += vector2;
		vec0 += vec2;
		AreEqual(vector0.mX, vec0.x, FLT_EPSILON);
		AreEqual(vector0.mY, vec0.y, FLT_EPSILON);

		vector1 += vector4;
		vec1 += vec4;
		AreEqual(vector1.mX, vec1.x, FLT_EPSILON);
		AreEqual(vector1.mY, vec1.y, FLT_EPSILON);

		vector3 += vector4;
		vec3 += vec4;
		AreEqual(vector3.mX, vec3.x, FLT_EPSILON);
		AreEqual(vector3.mY, vec3.y, FLT_EPSILON);

		vector5 += std::numeric_limits<float>::lowest();
		IsTrue(isinf(vector5.mX));
		IsTrue(maj::math::Helper<float>::areSame(vector5.mY, 0.f));

		vector6 += std::numeric_limits<float>::max();
		IsTrue(isinf(vector6.mX));
		IsTrue(maj::math::Helper<float>::areSame(vector6.mY, 0.f));

		vector7 += std::numeric_limits<float>::lowest();
		IsTrue(isinf(vector7.mX));
		IsTrue(isinf(vector7.mY));

		vector8 += std::numeric_limits<float>::max();
		IsTrue(isinf(vector8.mX));
		IsTrue(isinf(vector8.mY));
	}

	TEST_METHOD("-= Vec2")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);
		maj::math::Vec2<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec2 vec0;
		glm::vec2 vec1(10.0f, 10.0f);
		glm::vec2 vec2(52.0f, 1.0f);
		glm::vec2 vec3(10.5f, 10.25f);
		glm::vec2 vec4(52.7f, 1.69f);

		vector0 -= vector2;
		vec0 -= vec2;
		AreEqual(vector0.mX, vec0.x, FLT_EPSILON);
		AreEqual(vector0.mY, vec0.y, FLT_EPSILON);


		vector1 -= vector4;
		vec1 -= vec4;
		AreEqual(vector1.mX, vec1.x, FLT_EPSILON);
		AreEqual(vector1.mY, vec1.y, FLT_EPSILON);

		vector3 -= vector4;
		vec3 -= vec4;
		AreEqual(vector3.mX, vec3.x, FLT_EPSILON);
		AreEqual(vector3.mY, vec3.y, FLT_EPSILON);

		vector4 -= vector4;
		vec4 -= vec4;
		AreEqual(vector4.mX, vec4.x, FLT_EPSILON);
		AreEqual(vector4.mY, vec4.y, FLT_EPSILON);

		vector5 -= std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(vector5.mX, 0.f));
		IsTrue(isinf(vector5.mY));
		vector6 -= std::numeric_limits<float>::max();
		IsTrue(maj::math::Helper<float>::areSame(vector6.mX, 0.f));
		IsTrue(isinf(vector6.mY));
		vector7 -= std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(vector7.mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector7.mY, 0.f));
		vector8 -= std::numeric_limits<float>::max();
		IsTrue(maj::math::Helper<float>::areSame(vector8.mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector8.mY, 0.f));
	}

	TEST_METHOD("*= float")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);
		maj::math::Vec2<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector6(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec2 vec0;
		glm::vec2 vec1(10.0f, 10.0f);
		glm::vec2 vec2(52.0f, 1.0f);
		glm::vec2 vec3(10.5f, 10.25f);
		glm::vec2 vec4(52.7f, 1.69f);

		vector0 *= 10.f;
		vec0 *= 10.f;
		AreEqual(vector0.mX, vec0.x, FLT_EPSILON);
		AreEqual(vector0.mY, vec0.y, FLT_EPSILON);

		vector1 *= 10.f;
		vec1 *= 10.f;
		AreEqual(vector1.mX, vec1.x, FLT_EPSILON);
		AreEqual(vector1.mY, vec1.y, FLT_EPSILON);

		vector3 *= 10.f;
		vec3 *= 10.f;
		AreEqual(vector3.mX, vec3.x, FLT_EPSILON);
		AreEqual(vector3.mY, vec3.y, FLT_EPSILON);

		vector2 *= PI;
		vec2 *= PI;
		AreEqual(vector2.mX, vec2.x, FLT_EPSILON);
		AreEqual(vector2.mY, vec2.y, FLT_EPSILON);

		vector4 *= PI;
		vec4 *= PI;
		AreEqual(vector4.mX, vec4.x, FLT_EPSILON);
		AreEqual(vector4.mY, vec4.y, FLT_EPSILON);

		vector4 *= 0.0f;
		vec4 *= 0.0f;
		AreEqual(vector4.mX, vec4.x, FLT_EPSILON);
		AreEqual(vector4.mY, vec4.y, FLT_EPSILON);

		vector5 *= std::numeric_limits<float>::lowest();
		IsTrue(isinf(vector5.mX));
		IsTrue(isinf(vector5.mY));
		vector6 *= std::numeric_limits<float>::max();
		IsTrue(isinf(vector6.mX));
		IsTrue(isinf(vector6.mY));
		vector7 *= std::numeric_limits<float>::lowest();
		IsTrue(isinf(vector7.mX));
		IsTrue(isinf(vector7.mY));
		vector8 *= std::numeric_limits<float>::max();
		IsTrue(isinf(vector8.mX));
		IsTrue(isinf(vector8.mY));
	}

	TEST_METHOD("/= float")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);
		maj::math::Vec2<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector6(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec2 vec0;
		glm::vec2 vec1(10.0f, 10.0f);
		glm::vec2 vec2(52.0f, 1.0f);
		glm::vec2 vec3(10.5f, 10.25f);
		glm::vec2 vec4(52.7f, 1.69f);

		vector0 /= 10.f;
		glm::vec2 resGLM = vec0 / 10.f;
		AreEqual(resGLM.x, vector0.mX, 10E-5);
		AreEqual(resGLM.y, vector0.mY, 10E-5);

		vector1 /= 10.f;
		resGLM = vec1 / 10.f;
		AreEqual(resGLM.x, vector1.mX, 10E-5);
		AreEqual(resGLM.y, vector1.mY, 10E-5);


		vector3 /= 10.f;
		resGLM = vec3 / 10.f;
		AreEqual(resGLM.x, vector3.mX, 10E-5);
		AreEqual(resGLM.y, vector3.mY, 10E-5);

		vector2 /= PI;
		resGLM = vec2 / PI;
		AreEqual(resGLM.x, vector2.mX, 10E-5);
		AreEqual(resGLM.y, vector2.mY, 10E-5);

		vector4 /= PI;
		resGLM = vec4 / PI;
		AreEqual(resGLM.x, vector4.mX, 10E-5);
		AreEqual(resGLM.y, vector4.mY, 10E-5);


		vector5 /= std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(vector5.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector5.mY, -1.0f));
		vector6 /= std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(vector6.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector6.mY, -1.0f));
		vector7 /= std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(vector7.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector7.mY, 1.0f));
		vector8 /= std::numeric_limits<float>::max();
		IsTrue(maj::math::Helper<float>::areSame(vector8.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector8.mY, 1.0f));
	}

	TEST_METHOD("[]")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);
		maj::math::Vec2<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		IsTrue(maj::math::Helper<float>::areSame(vector0[0u], 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector0[1u], 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector1[0u], 10.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector1[1u], 10.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector3[0u], 10.5f));
		IsTrue(maj::math::Helper<float>::areSame(vector3[1u], 10.25f));

		IsFalse(maj::math::Helper<float>::areSame(vector3[2u], 12.0025f));

		IsTrue(maj::math::Helper<float>::areSame(vector5[0u], std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(vector5[1u], std::numeric_limits<float>::max()));
		IsTrue(maj::math::Helper<float>::areSame(vector6[0u], std::numeric_limits<float>::max()));
		IsTrue(maj::math::Helper<float>::areSame(vector6[1u], std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(vector7[0u], std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(vector7[1u], std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(vector8[0u], std::numeric_limits<float>::max()));
		IsTrue(maj::math::Helper<float>::areSame(vector8[1u], std::numeric_limits<float>::max()));
	}

	TEST_METHOD("==")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);
		maj::math::Vec2<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		IsTrue(vector0 == maj::math::Vec2<float>(0.f, 0.f));
		IsTrue(vector2 == maj::math::Vec2<float>(52.0f, 1.0f));
		IsTrue(vector4 == maj::math::Vec2<float>(52.7f, 1.69f));


		IsTrue(vector5 == maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		IsTrue(vector6 == maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest()));
		IsTrue(vector7 == maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()));
		IsTrue(vector8 == maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));



	}

	TEST_METHOD("!=")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);
		maj::math::Vec2<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec2<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec2<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		IsTrue(vector0 != maj::math::Vec2<float>(0.f, 1.f));
		IsTrue(vector2 != maj::math::Vec2<float>(5.0f, 1.0f));
		IsTrue(vector4 != maj::math::Vec2<float>(52.8f, 1.68f));


		IsTrue(vector7 != maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		IsTrue(vector8 != maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest()));
		IsTrue(vector5 != maj::math::Vec2<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()));
		IsTrue(vector6 != maj::math::Vec2<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));

	}

	TEST_METHOD("lerp")
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 10.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 10.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);

		glm::vec2 vec0;
		glm::vec2 vec1(10.0f, 10.0f);
		glm::vec2 vec2(52.0f, 1.0f);
		glm::vec2 vec3(10.5f, 10.25f);
		glm::vec2 vec4(52.7f, 1.69f);

		maj::math::Vec2<float> test = maj::math::Vec2<float>::lerp(vector0, vector1, 0.5f);
		glm::vec2 testGlm = glm::lerp(vec0, vec1, 0.5f);
		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);

		test = maj::math::Vec2<float>::lerp(vector3, vector4, 0.5f);
		testGlm = glm::lerp(vec3, vec4, 0.5f);
		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);

		test = maj::math::Vec2<float>::lerp(vector2, vector4, 1.f);
		testGlm = glm::lerp(vec2, vec4, 1.f);
		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);

		test = maj::math::Vec2<float>::lerp(vector1, vector3, 0.0f);
		testGlm = glm::lerp(vec1, vec3, 0.0f);
		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);
	}

	TEST_METHOD("Slerp")	//	pas slerp dans glm
	{
		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 11.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 11.25f);
		maj::math::Vec2<float> vector4(52.7f, 1.69f);

		maj::math::Vec2<float> test = maj::math::Vec2<float>::slerp(vector1, vector2, 0.5f);

		AreEqual(13.5367800f, test.mX, 10E-5);
		AreEqual(6.1445480, test.mY, 10E-5);

		test = maj::math::Vec2<float>::slerp(vector3, vector4, 0.5f);

		AreEqual(14.0136000f, test.mX, 10E-5);
		AreEqual(6.3585760f, test.mY, 10E-5);

		test = maj::math::Vec2<float>::slerp(vector3, vector4, 0.0f);

		AreEqual(10.5000000f, test.mX, 10E-5);
		AreEqual(11.2500000f, test.mY, 10E-5);

		test = maj::math::Vec2<float>::slerp(vector1, vector2, 1.0f);

		AreEqual(14.8633200f, test.mX, 10E-5);
		AreEqual(0.2858325f, test.mY, 10E-5);

		test = maj::math::Vec2<float>::slerp(vector0, vector2, 0.5f);

		AreEqual(0.0f, test.mX, 10E-5);
		AreEqual(0.0f, test.mY, 10E-5);

		test = maj::math::Vec2<float>::slerp(vector0, vector3, 0.5f);
		AreEqual(0.0f, test.mX, 10E-5);
		AreEqual(0.0f, test.mY, 10E-5);
	}

	TEST_METHOD("float / ")
	{
		glm::vec2 vec0;
		glm::vec2 vec1(10.0f, 11.0f);
		glm::vec2 vec2(52.0f, 1.0f);
		glm::vec2 vec3(10.5f, 11.25f);
		glm::vec2 vec4(52.007f, 1.69f);

		maj::math::Vec2<float> vector0;
		maj::math::Vec2<float> vector1(10.0f, 11.0f);
		maj::math::Vec2<float> vector2(52.0f, 1.0f);
		maj::math::Vec2<float> vector3(10.5f, 11.25f);
		maj::math::Vec2<float> vector4(52.007f, 1.69f);


		maj::math::Vec2<float> res = 10.f / vector0;
		AreEqual(0.0f, res.mX, 10E-5);
		AreEqual(0.0f, res.mY, 10E-5);

		res = 10.f / vector1;
		glm::vec2 resGlm = 10.0f / vec1;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);

		res = 10.f / vector3;
		resGlm = 10.f / vec3;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);

		res = PI / vector2;
		resGlm = PI / vec2;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);

		res = PI / vector4;
		resGlm = PI / vec4;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
	}
};