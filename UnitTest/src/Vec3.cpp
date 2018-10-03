#include "Test.h"

#include <Vec3.h>

#define GLM_FORCE_LEFT_HANDED
#include <glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS

#include <gtx/vector_angle.hpp>
#include <gtx/compatibility.hpp>

TEST_CLASS("Vec3")
{
	TEST_METHOD("constructor")
	{
		maj::math::Vec3<float> vector;
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mZ, 0.0f));


		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.0f);
		IsTrue(maj::math::Helper<float>::areSame(vector1.mX, 10.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector1.mY, 11.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector1.mZ, 12.0f));

		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		AreEqual(10.5f, vector3.mX, FLT_EPSILON);
		AreEqual(11.25f, vector3.mY, FLT_EPSILON);
		AreEqual(12.0025f, vector3.mZ, FLT_EPSILON);

		maj::math::Vec3<float> vector2(maj::math::Vec3<float>::skUp);
		IsTrue(maj::math::Helper<float>::areSame(vector2.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector2.mY, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector2.mZ, 0.0f));

		maj::math::Vec3<float> vector4(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		IsTrue(maj::math::Helper<float>::areSame(vector4.mX, std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(vector4.mY, std::numeric_limits<float>::max()));
		IsTrue(maj::math::Helper<float>::areSame(vector4.mZ, std::numeric_limits<float>::max()));

	}

	TEST_METHOD("norm")
	{
		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(10.5f, 11.25f, 12.0025f);

		maj::math::Vec3<float> vector;
		AreEqual(0.f, vector.norm(), FLT_EPSILON);

		vector = maj::math::Vec3<float>(10.0f, 11.0f, 12.f);
		AreEqual(glm::length(vec1), vector.norm(), FLT_EPSILON);

		vector = maj::math::Vec3<float>(10.5f, 11.25f, 12.0025f);
		AreEqual(glm::length(vec2), vector.norm(), FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		IsTrue(isinf(vector.norm()));

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		IsTrue(isinf(vector.norm()));

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		IsTrue(isinf(vector.norm()));

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		IsTrue(isinf(vector.norm()));

	}

	TEST_METHOD("sqr norm")
	{
		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(10.5f, 11.25f, 12.0025f);

		maj::math::Vec3<float> vector;
		AreEqual(0.f, vector.norm(), FLT_EPSILON);

		vector = maj::math::Vec3<float>(10.0f, 11.0f, 12.f);
		AreEqual(glm::length2(vec1), vector.sqrNorm(), FLT_EPSILON);

		vector = maj::math::Vec3<float>(10.5f, 11.25f, 12.0025f);
		AreEqual(glm::length2(vec2), vector.sqrNorm(), FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		IsTrue(isinf(vector.sqrNorm()));

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		IsTrue(isinf(vector.sqrNorm()));

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		IsTrue(isinf(vector.sqrNorm()));

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		IsTrue(isinf(vector.sqrNorm()));

	}

	TEST_METHOD("normalized")
	{
		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(10.5f, 11.25f, 12.0025f);

		maj::math::Vec3<float> vector;
		glm::vec3 test;
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mZ, 0.0f));

		vector = maj::math::Vec3<float>(10.0f, 11.0f, 12.f);
		test = glm::normalize(vec1);
		AreEqual(test.x, vector.normalized().mX, FLT_EPSILON);
		AreEqual(test.y, vector.normalized().mY, FLT_EPSILON);
		AreEqual(test.z, vector.normalized().mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(10.5f, 11.25f, 12.0025f);
		test = glm::normalize(vec2);
		AreEqual(test.x, vector.normalized().mX, FLT_EPSILON);
		AreEqual(test.y, vector.normalized().mY, FLT_EPSILON);
		AreEqual(test.z, vector.normalized().mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		AreEqual(0.0f, vector.normalized().mX, FLT_EPSILON);
		AreEqual(0.0f, vector.normalized().mY, FLT_EPSILON);
		AreEqual(0.0f, vector.normalized().mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		AreEqual(0.0f, vector.normalized().mX, FLT_EPSILON);
		AreEqual(0.0f, vector.normalized().mY, FLT_EPSILON);
		AreEqual(0.0f, vector.normalized().mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		AreEqual(0.0f, vector.normalized().mX, FLT_EPSILON);
		AreEqual(0.0f, vector.normalized().mY, FLT_EPSILON);
		AreEqual(0.0f, vector.normalized().mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		AreEqual(0.0f, vector.normalized().mX, FLT_EPSILON);
		AreEqual(0.0f, vector.normalized().mY, FLT_EPSILON);
		AreEqual(0.0f, vector.normalized().mZ, FLT_EPSILON);

	}

	TEST_METHOD("angle")
	{
		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(52.0f, 1.0f, 33.f);
		glm::vec3 vec3(10.5f, 11.25f, 12.025f);
		glm::vec3 vec4(52.007f, 1.69f, 42.435f);
		vec1 = glm::normalize(vec1);
		vec2 = glm::normalize(vec2);
		vec4 = glm::normalize(vec4);
		vec3 = glm::normalize(vec3);

		maj::math::Vec3<float> vector;
		AreEqual(vector.angle(vector), 0.0f, FLT_EPSILON);

		maj::math::Vec3<float> vector0(10.0f, 11.0f, 12.f);

		AreEqual(vector0.angle(vector), 0.0f, FLT_EPSILON);
		AreEqual(vector.angle(vector0), 0.0f, FLT_EPSILON);

		maj::math::Vec3<float> vector1(52.0f, 1.0f, 33.f);
		AreEqual(vector0.angle(vector1), glm::degrees(glm::angle(vec1, vec2)), FLT_EPSILON);

		AreEqual(0.0f, vector1.angle(vector1), FLT_EPSILON);
		AreEqual(0.0f, maj::math::Vec3<float>::skForward.angle(maj::math::Vec3<float>::skForward * 2.f), FLT_EPSILON);

		AreEqual(vector1.angle(vector), 0.0f, FLT_EPSILON);
		AreEqual(vector.angle(vector1), 0.0f, FLT_EPSILON);

		maj::math::Vec3<float> vector2(10.5f, 11.25f, 12.025f);
		maj::math::Vec3<float> vector3(52.007f, 1.69f, 42.435f);

		vec4 = glm::normalize(vec4);
		vec3 = glm::normalize(vec3);
		float angleMe = vector2.angle(vector3);
		float angleGLM = glm::degrees(glm::angle(vec3, vec4));

		AreEqual(angleMe, angleGLM, FLT_EPSILON);
	}

	TEST_METHOD("inversed")
	{
		maj::math::Vec3<float> vector;
		AreEqual(0.0f, vector.inversed().mX, FLT_EPSILON);
		AreEqual(0.0f, vector.inversed().mY, FLT_EPSILON);
		AreEqual(0.0f, vector.inversed().mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(10.0f, 11.0f, 12.f);
		AreEqual(1.0f / 10.0f, vector.inversed().mX, FLT_EPSILON);
		AreEqual(1.0f / 11.0f, vector.inversed().mY, FLT_EPSILON);
		AreEqual(1.0f / 12.0f, vector.inversed().mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(10.5f, 11.25f, 12.0025f);
		AreEqual(1.0f / 10.5f, vector.inversed().mX, FLT_EPSILON);
		AreEqual(1.0f / 11.25f, vector.inversed().mY, FLT_EPSILON);
		AreEqual(1.0f / 12.0025f, vector.inversed().mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		AreEqual(0.0f, vector.inversed().mX, FLT_EPSILON);
		AreEqual(0.0f, vector.inversed().mY, FLT_EPSILON);
		AreEqual(0.0f, vector.inversed().mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		AreEqual(0.0f, vector.inversed().mX, FLT_EPSILON);
		AreEqual(0.0f, vector.inversed().mY, FLT_EPSILON);
		AreEqual(0.0f, vector.inversed().mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		AreEqual(0.0f, vector.inversed().mX, FLT_EPSILON);
		AreEqual(0.0f, vector.inversed().mY, FLT_EPSILON);
		AreEqual(0.0f, vector.inversed().mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		AreEqual(0.0f, vector.inversed().mX, FLT_EPSILON);
		AreEqual(0.0f, vector.inversed().mY, FLT_EPSILON);
		AreEqual(0.0f, vector.inversed().mZ, FLT_EPSILON);
	}

	TEST_METHOD("normalize")
	{

		maj::math::Vec3<float> vector;
		vector.normalize();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mZ, 0.0f));

		vector = maj::math::Vec3<float>(10.0f, 11.0f, 12.f);
		vector.normalize();
		AreEqual(10.0f / std::sqrt(10.0f * 10.0f + 11.0f * 11.0f + 12.0f * 12.0f), vector.mX, FLT_EPSILON);
		AreEqual(11.0f / std::sqrt(10.0f * 10.0f + 11.0f * 11.0f + 12.0f * 12.0f), vector.mY, FLT_EPSILON);
		AreEqual(12.0f / std::sqrt(10.0f * 10.0f + 11.0f * 11.0f + 12.0f * 12.0f), vector.mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(10.5f, 11.25f, 12.0025f);
		vector.normalize();
		AreEqual(10.5f / std::sqrt(10.5f * 10.5f + 11.25f * 11.25f + 12.0025f * 12.0025f), vector.mX, FLT_EPSILON);
		AreEqual(11.25f / std::sqrt(10.5f * 10.5f + 11.25f * 11.25f + 12.0025f * 12.0025f), vector.mY, FLT_EPSILON);
		AreEqual(12.0025f / std::sqrt(10.5f * 10.5f + 11.25f * 11.25f + 12.0025f * 12.0025f), vector.mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		vector.normalize();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mZ, 0.0f));

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		vector.normalize();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mZ, 0.0f));

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		vector.normalize();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mZ, 0.0f));

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		vector.normalize();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mZ, 0.0f));
	}

	TEST_METHOD("dot")
	{

		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector6(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec3<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(52.0f, 1.0f, 33.f);
		glm::vec3 vec3(10.5f, 11.25f, 12.0025f);
		glm::vec3 vec4(52.007f, 1.69f, 42.4235f);

		IsTrue(maj::math::Helper<float>::areSame(vector0.dot(vector1), 0.0f));

		AreEqual(glm::dot(vec1, vec2), vector1.dot(vector2), FLT_EPSILON);
		AreEqual(glm::dot(vec3, vec4), vector3.dot(vector4), FLT_EPSILON);
		AreEqual(glm::dot(vec2, vec4), vector2.dot(vector4), FLT_EPSILON);

		AreEqual(glm::dot(vec3, vec3), vector3.dot(vector3), FLT_EPSILON);
		AreEqual(glm::dot(vec3, -vec3), vector3.dot(-vector3), FLT_EPSILON);

		IsTrue(isinf(vector5.dot(vector6)));
		IsTrue(isinf(vector7.dot(vector8)));
	}

	TEST_METHOD("cross")
	{
		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);

		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(52.0f, 1.0f, 33.f);
		glm::vec3 vec3(10.5f, 11.25f, 12.0025f);
		glm::vec3 vec4(52.007f, 1.69f, 42.4235f);

		maj::math::Vec3<float> res = vector0.cross(vector1);
		AreEqual(0.0f, res.mX, FLT_EPSILON);
		AreEqual(0.0f, res.mY, FLT_EPSILON);
		AreEqual(0.0f, res.mZ, FLT_EPSILON);

		res = vector1.cross(vector2);
		glm::vec3 resGlm = glm::cross(vec1, vec2);
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector3.cross(vector4);
		resGlm = glm::cross(vec3, vec4);
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector2.cross(vector4);
		resGlm = glm::cross(vec2, vec4);
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector3.cross(vector3);
		resGlm = glm::cross(vec3, vec3);
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

	}

	TEST_METHOD("inverse")
	{
		maj::math::Vec3<float> vector;
		vector.inverse();
		AreEqual(0.0f, vector.mX, FLT_EPSILON);
		AreEqual(0.0f, vector.mY, FLT_EPSILON);
		AreEqual(0.0f, vector.mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(10.0f, 11.0f, 12.f);
		vector.inverse();
		AreEqual(1.0f / 10.0f, vector.mX, FLT_EPSILON);
		AreEqual(1.0f / 11.0f, vector.mY, FLT_EPSILON);
		AreEqual(1.0f / 12.0f, vector.mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(10.5f, 11.25f, 12.0025f);
		vector.inverse();
		AreEqual(1.0f / 10.5f, vector.mX, FLT_EPSILON);
		AreEqual(1.0f / 11.25f, vector.mY, FLT_EPSILON);
		AreEqual(1.0f / 12.0025f, vector.mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		vector.inverse();
		AreEqual(0.0f, vector.mX, FLT_EPSILON);
		AreEqual(0.0f, vector.mY, FLT_EPSILON);
		AreEqual(0.0f, vector.mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		vector.inverse();
		AreEqual(0.0f, vector.mX, FLT_EPSILON);
		AreEqual(0.0f, vector.mY, FLT_EPSILON);
		AreEqual(0.0f, vector.mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		vector.inverse();
		AreEqual(0.0f, vector.mX, FLT_EPSILON);
		AreEqual(0.0f, vector.mY, FLT_EPSILON);
		AreEqual(0.0f, vector.mZ, FLT_EPSILON);

		vector = maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		vector.inverse();
		AreEqual(0.0f, vector.mX, FLT_EPSILON);
		AreEqual(0.0f, vector.mY, FLT_EPSILON);
		AreEqual(0.0f, vector.mZ, FLT_EPSILON);

	}

	TEST_METHOD("clamp")
	{
		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);

		maj::math::Vec3<float> vector;
		AreEqual(1.0f, vector.clamp(maj::math::Vec3<float>(1.f, 2.f, 3.f), maj::math::Vec3<float>(10.f, 11.f, 12.f)).mX, FLT_EPSILON);
		AreEqual(2.0f, vector.clamp(maj::math::Vec3<float>(1.f, 2.f, 3.f), maj::math::Vec3<float>(10.f, 11.f, 12.f)).mY, FLT_EPSILON);
		AreEqual(3.0f, vector.clamp(maj::math::Vec3<float>(1.f, 2.f, 3.f), maj::math::Vec3<float>(10.f, 11.f, 12.f)).mZ, FLT_EPSILON);


		vector = maj::math::Vec3<float>(10.0f, 11.0f, 12.f);
		AreEqual(9.0f, vector.clamp(maj::math::Vec3<float>(1.f, 2.f, 13.f), maj::math::Vec3<float>(9.f, 9.f, 15.f)).mX, FLT_EPSILON);
		AreEqual(9.0f, vector.clamp(maj::math::Vec3<float>(1.f, 2.f, 13.f), maj::math::Vec3<float>(9.f, 9.f, 15.f)).mY, FLT_EPSILON);
		AreEqual(13.f, vector.clamp(maj::math::Vec3<float>(1.f, 2.f, 13.f), maj::math::Vec3<float>(9.f, 9.f, 15.f)).mZ, FLT_EPSILON);


		vector = maj::math::Vec3<float>(10.5f, 11.25f, 12.0025f);
		AreEqual(10.25f, vector.clamp(maj::math::Vec3<float>(1.f, 2.f, 12.0026f), maj::math::Vec3<float>(10.25f, 10.24f, 150.50f)).mX, FLT_EPSILON);
		AreEqual(10.24f, vector.clamp(maj::math::Vec3<float>(1.f, 2.f, 12.0026f), maj::math::Vec3<float>(10.25f, 10.24f, 150.50f)).mY, FLT_EPSILON);
		AreEqual(12.0026f, vector.clamp(maj::math::Vec3<float>(1.f, 2.f, 12.0026f), maj::math::Vec3<float>(10.25f, 10.24f, 150.50f)).mZ, FLT_EPSILON);


		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		AreEqual(1.f, vector.clamp(maj::math::Vec3<float>(1.f, 2.f, 2.f), maj::math::Vec3<float>(10.25f, 9.f, 12.f)).mX, FLT_EPSILON);
		AreEqual(9.0f, vector.clamp(maj::math::Vec3<float>(1.f, 2.f, 2.f), maj::math::Vec3<float>(10.25f, 9.f, 12.f)).mY, FLT_EPSILON);
		AreEqual(12.0f, vector.clamp(maj::math::Vec3<float>(1.f, 2.f, 2.f), maj::math::Vec3<float>(10.25f, 9.f, 12.f)).mZ, FLT_EPSILON);


		vector = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		AreEqual(1.f, vector.clamp(maj::math::Vec3<float>(1.f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()), maj::math::Vec3<float>(std::numeric_limits<float>::max(), 120.f, 3.06f)).mX, FLT_EPSILON);
		AreEqual(120.f, vector.clamp(maj::math::Vec3<float>(1.f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()), maj::math::Vec3<float>(std::numeric_limits<float>::max(), 120.f, 3.06f)).mY, FLT_EPSILON);
		AreEqual(120.f, vector.clamp(maj::math::Vec3<float>(1.f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()), maj::math::Vec3<float>(std::numeric_limits<float>::max(), 120.f, 3.06f)).mY, FLT_EPSILON);

	}

	TEST_METHOD("* float")
	{

		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector6(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec3<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(52.0f, 1.0f, 33.f);
		glm::vec3 vec3(10.5f, 11.25f, 12.0025f);
		glm::vec3 vec4(52.007f, 1.69f, 42.4235f);

		maj::math::Vec3<float> res = vector0 * 10.f;
		glm::vec3 resGlm = vec0 * 10.0f;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector1 * 10.f;
		resGlm = vec1 * 10.0f;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector3 * 10.f;
		resGlm = vec3 * 10.0f;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector2 * PI;
		resGlm = vec2 * PI;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector4 * PI;
		resGlm = vec4 * PI;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector1 * 0.f;
		resGlm = vec1 * 0.0f;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector3 * 0.f;
		resGlm = vec3 * 0.0f;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector5 * std::numeric_limits<float>::lowest();
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		IsTrue(isinf(res.mZ));
		res = vector6 * std::numeric_limits<float>::max();
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		IsTrue(isinf(res.mZ));
		res = vector7 * std::numeric_limits<float>::lowest();
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		IsTrue(isinf(res.mZ));
		res = vector8 * std::numeric_limits<float>::max();
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		IsTrue(isinf(res.mZ));
	}

	TEST_METHOD("/ float")
	{
		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector6(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec3<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(52.0f, 1.0f, 33.f);
		glm::vec3 vec3(10.5f, 11.25f, 12.0025f);
		glm::vec3 vec4(52.007f, 1.69f, 42.4235f);


		maj::math::Vec3<float> res = vector0 / 10.f;
		glm::vec3 resGlm = vec0 / 10.f;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);

		res = vector1 / 10.f;
		resGlm = vec1 / 10.f;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);

		res = vector3 / 10.f;
		resGlm = vec3 / 10.f;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);

		res = vector2 / PI;
		resGlm = vec2 / PI;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);

		res = vector4 / PI;
		resGlm = vec4 / PI;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);

		res = vector5 / std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, -1.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mZ, -1.0f));

		res = vector6 / std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, -1.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mZ, -1.0f));

		res = vector7 / std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mZ, 1.0f));

		res = vector8 / std::numeric_limits<float>::max();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(res.mZ, 1.0f));

	}

	TEST_METHOD("+ Vec3")
	{
		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec3<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(52.0f, 1.0f, 33.f);
		glm::vec3 vec3(10.5f, 11.25f, 12.0025f);
		glm::vec3 vec4(52.007f, 1.69f, 42.4235f);

		maj::math::Vec3<float> res = vector0 + vector1;
		glm::vec3 resGlm = vec0 + vec1;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector1 + ( -vector2 );
		resGlm = vec1 + ( -vec2 );
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector3 + vector4;
		resGlm = vec3 + vec4;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector5 + vector6;
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, 0.f));
		IsTrue(isinf(res.mZ));

		res = vector7 + std::numeric_limits<float>::lowest();
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		IsTrue(isinf(res.mZ));

		res = vector8 + std::numeric_limits<float>::max();
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		IsTrue(isinf(res.mZ));

	}

	TEST_METHOD("- Vec3")
	{

		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec3<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(52.0f, 1.0f, 33.f);
		glm::vec3 vec3(10.5f, 11.25f, 12.0025f);
		glm::vec3 vec4(52.007f, 1.69f, 42.4235f);

		maj::math::Vec3<float> res = vector0 - vector1;
		glm::vec3 resGlm = vec0 - vec1;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector1 - vector4;
		resGlm = vec1 - vec4;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector3 - vector4;
		resGlm = vec3 - vec4;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector3 - vector3;
		resGlm = vec3 - vec3;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);

		res = vector5 - std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 0.f));
		IsTrue(isinf(res.mY));
		IsTrue(isinf(res.mZ));

		res = vector6 - std::numeric_limits<float>::max();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 0.f));
		IsTrue(isinf(res.mY));
		IsTrue(maj::math::Helper<float>::areSame(res.mZ, 0.f));

		res = vector7 - std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(res.mZ, 0.f));


		res = vector8 - std::numeric_limits<float>::max();
		IsTrue(maj::math::Helper<float>::areSame(res.mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(res.mY, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(res.mZ, 0.f));

	}

	TEST_METHOD("scale")
	{
		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec3<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		maj::math::Vec3<float> res = vector0.scaled(vector1);
		AreEqual(0.0f, res.mX, FLT_EPSILON);
		AreEqual(0.0f, res.mY, FLT_EPSILON);
		AreEqual(0.0f, res.mZ, FLT_EPSILON);

		res = vector1.scaled(vector4);
		AreEqual(10.0f * 52.007f, res.mX, FLT_EPSILON);
		AreEqual(11.f * 1.69f, res.mY, FLT_EPSILON);
		AreEqual(12.0f * 42.4235f, res.mZ, FLT_EPSILON);

		res = vector3.scaled(vector4);
		AreEqual(10.50f * 52.007f, res.mX, FLT_EPSILON);
		AreEqual(11.25f * 1.69f, res.mY, FLT_EPSILON);
		AreEqual(12.0025f * 42.4235f, res.mZ, FLT_EPSILON);

		res = vector5.scaled(std::numeric_limits<float>::lowest());
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		IsTrue(isinf(res.mZ));

		res = vector6.scaled(std::numeric_limits<float>::max());
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		IsTrue(isinf(res.mZ));

		res = vector7.scaled(std::numeric_limits<float>::lowest());
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		IsTrue(isinf(res.mZ));

		res = vector8.scaled(std::numeric_limits<float>::max());
		IsTrue(isinf(res.mX));
		IsTrue(isinf(res.mY));
		IsTrue(isinf(res.mZ));

	}

	TEST_METHOD("-")
	{
		maj::math::Vec3<float> vector0;
		vector0 = -vector0;
		AreEqual(vector0.mX, 0.0f, FLT_EPSILON);
		AreEqual(vector0.mY, 0.0f, FLT_EPSILON);
		AreEqual(vector0.mZ, 0.0f, FLT_EPSILON);

		vector0 = maj::math::Vec3<float>(10.0f, -11.0f, -12.f);
		vector0 = -vector0;
		AreEqual(vector0.mX, -10.0f, FLT_EPSILON);
		AreEqual(vector0.mY, 11.0f, FLT_EPSILON);
		AreEqual(vector0.mZ, 12.0f, FLT_EPSILON);

		vector0 = maj::math::Vec3<float>(-10.5f, 11.25f, -12.0025f);
		vector0 = -vector0;
		AreEqual(vector0.mX, 10.5f, FLT_EPSILON);
		AreEqual(vector0.mY, -11.25f, FLT_EPSILON);
		AreEqual(vector0.mZ, 12.0025f, FLT_EPSILON);

		vector0 = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		vector0 = -vector0;
		AreEqual(vector0.mX, std::numeric_limits<float>::max(), FLT_EPSILON);
		AreEqual(vector0.mY, std::numeric_limits<float>::lowest(), FLT_EPSILON);
		AreEqual(vector0.mZ, std::numeric_limits<float>::lowest(), FLT_EPSILON);

		vector0 = maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		vector0 = -vector0;
		AreEqual(vector0.mX, std::numeric_limits<float>::lowest(), FLT_EPSILON);
		AreEqual(vector0.mY, std::numeric_limits<float>::max(), FLT_EPSILON);
		AreEqual(vector0.mZ, std::numeric_limits<float>::lowest(), FLT_EPSILON);

		vector0 = maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		vector0 = -vector0;
		AreEqual(vector0.mX, std::numeric_limits<float>::max(), FLT_EPSILON);
		AreEqual(vector0.mY, std::numeric_limits<float>::max(), FLT_EPSILON);
		AreEqual(vector0.mZ, std::numeric_limits<float>::max(), FLT_EPSILON);

		vector0 = maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		vector0 = -vector0;
		AreEqual(vector0.mX, std::numeric_limits<float>::lowest(), FLT_EPSILON);
		AreEqual(vector0.mY, std::numeric_limits<float>::lowest(), FLT_EPSILON);
		AreEqual(vector0.mZ, std::numeric_limits<float>::lowest(), FLT_EPSILON);
	}

	TEST_METHOD("+= Vec3")
	{
		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec3<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(52.0f, 1.0f, 33.f);
		glm::vec3 vec3(10.5f, 11.25f, 12.0025f);
		glm::vec3 vec4(52.007f, 1.69f, 42.4235f);

		vector0 += vector1;
		vec0 += vec1;
		AreEqual(vector0.mX, vec0.x, FLT_EPSILON);
		AreEqual(vector0.mY, vec0.y, FLT_EPSILON);
		AreEqual(vector0.mZ, vec0.z, FLT_EPSILON);

		vector1 += vector4;
		vec1 += vec4;
		AreEqual(vector1.mX, vec1.x, FLT_EPSILON);
		AreEqual(vector1.mY, vec1.y, FLT_EPSILON);
		AreEqual(vector1.mZ, vec1.z, FLT_EPSILON);

		vector3 += vector4;
		vec3 += vec4;
		AreEqual(vector3.mX, vec3.x, FLT_EPSILON);
		AreEqual(vector3.mY, vec3.y, FLT_EPSILON);
		AreEqual(vector3.mZ, vec3.z, FLT_EPSILON);

		vector5 += std::numeric_limits<float>::lowest();
		IsTrue(isinf(vector5.mX));
		IsTrue(maj::math::Helper<float>::areSame(vector5.mY, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector5.mZ, 0.f));

		vector6 += std::numeric_limits<float>::max();
		IsTrue(isinf(vector6.mX));
		IsTrue(maj::math::Helper<float>::areSame(vector6.mY, 0.f));
		IsTrue(isinf(vector6.mZ));

		vector7 += std::numeric_limits<float>::lowest();
		IsTrue(isinf(vector7.mX));
		IsTrue(isinf(vector7.mY));
		IsTrue(isinf(vector7.mZ));

		vector8 += std::numeric_limits<float>::max();
		IsTrue(isinf(vector8.mX));
		IsTrue(isinf(vector8.mY));
		IsTrue(isinf(vector8.mZ));

	}

	TEST_METHOD("-= Vec3")
	{
		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec3<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(52.0f, 1.0f, 33.f);
		glm::vec3 vec3(10.5f, 11.25f, 12.0025f);
		glm::vec3 vec4(52.007f, 1.69f, 42.4235f);

		vector0 -= vector1;
		vec0 -= vec1;
		AreEqual(vector0.mX, vec0.x, FLT_EPSILON);
		AreEqual(vector0.mY, vec0.y, FLT_EPSILON);
		AreEqual(vector0.mZ, vec0.z, FLT_EPSILON);

		vector1 -= vector4;
		vec1 -= vec4;
		AreEqual(vector1.mX, vec1.x, FLT_EPSILON);
		AreEqual(vector1.mY, vec1.y, FLT_EPSILON);
		AreEqual(vector1.mZ, vec1.z, FLT_EPSILON);

		vector3 -= vector4;
		vec3 -= vec4;
		AreEqual(vector3.mX, vec3.x, FLT_EPSILON);
		AreEqual(vector3.mY, vec3.y, FLT_EPSILON);
		AreEqual(vector3.mZ, vec3.z, FLT_EPSILON);

		vector4 -= vector4;
		vec4 -= vec4;
		AreEqual(vector4.mX, vec4.x, FLT_EPSILON);
		AreEqual(vector4.mY, vec4.y, FLT_EPSILON);
		AreEqual(vector4.mZ, vec4.z, FLT_EPSILON);

		vector5 -= std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(vector5.mX, 0.f));
		IsTrue(isinf(vector5.mY));
		IsTrue(isinf(vector5.mZ));

		vector6 -= std::numeric_limits<float>::max();
		IsTrue(maj::math::Helper<float>::areSame(vector6.mX, 0.f));
		IsTrue(isinf(vector6.mY));
		IsTrue(maj::math::Helper<float>::areSame(vector6.mZ, 0.f));

		vector7 -= std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(vector7.mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector7.mY, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector7.mZ, 0.f));

		vector8 -= std::numeric_limits<float>::max();
		IsTrue(maj::math::Helper<float>::areSame(vector8.mX, 0.f));
		IsTrue(maj::math::Helper<float>::areSame(vector8.mY, 0.f));
	}

	TEST_METHOD("*= float")
	{

		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector6(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec3<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(52.0f, 1.0f, 33.f);
		glm::vec3 vec3(10.5f, 11.25f, 12.0025f);
		glm::vec3 vec4(52.007f, 1.69f, 42.4235f);

		vector0 *= 10.f;
		vec0 *= 10.f;
		AreEqual(vec0.x, vector0.mX, FLT_EPSILON);
		AreEqual(vec0.y, vector0.mY, FLT_EPSILON);
		AreEqual(vec0.z, vector0.mZ, FLT_EPSILON);

		vector1 *= 10.f;
		vec1 *= 10.f;
		AreEqual(vec1.x, vector1.mX, FLT_EPSILON);
		AreEqual(vec1.y, vector1.mY, FLT_EPSILON);
		AreEqual(vec1.z, vector1.mZ, FLT_EPSILON);

		vector3 *= 10.f;
		vec3 *= 10.f;
		AreEqual(vec3.x, vector3.mX, FLT_EPSILON);
		AreEqual(vec3.y, vector3.mY, FLT_EPSILON);
		AreEqual(vec3.z, vector3.mZ, FLT_EPSILON);

		vector2 *= PI;
		vec2 *= PI;
		AreEqual(vec2.x, vector2.mX, FLT_EPSILON);
		AreEqual(vec2.y, vector2.mY, FLT_EPSILON);
		AreEqual(vec2.z, vector2.mZ, FLT_EPSILON);

		vector4 *= PI;
		vec4 *= PI;
		AreEqual(vec4.x, vector4.mX, FLT_EPSILON);
		AreEqual(vec4.y, vector4.mY, FLT_EPSILON);
		AreEqual(vec4.z, vector4.mZ, FLT_EPSILON);

		vector4 *= 0.0f;
		vec4 *= 0.0f;
		AreEqual(vec4.x, vector4.mX, FLT_EPSILON);
		AreEqual(vec4.y, vector4.mY, FLT_EPSILON);
		AreEqual(vec4.z, vector4.mZ, FLT_EPSILON);


		vector5 *= std::numeric_limits<float>::lowest();
		IsTrue(isinf(vector5.mX));
		IsTrue(isinf(vector5.mY));
		IsTrue(isinf(vector5.mZ));

		vector6 *= std::numeric_limits<float>::max();
		IsTrue(isinf(vector6.mX));
		IsTrue(isinf(vector6.mY));
		IsTrue(isinf(vector6.mZ));

		vector7 *= std::numeric_limits<float>::lowest();
		IsTrue(isinf(vector7.mX));
		IsTrue(isinf(vector7.mY));
		IsTrue(isinf(vector7.mZ));

		vector8 *= std::numeric_limits<float>::max();
		IsTrue(isinf(vector8.mX));
		IsTrue(isinf(vector8.mY));
		IsTrue(isinf(vector8.mZ));

	}

	TEST_METHOD("/= float")
	{
		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector6(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec3<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(52.0f, 1.0f, 33.f);
		glm::vec3 vec3(10.5f, 11.25f, 12.0025f);
		glm::vec3 vec4(52.007f, 1.69f, 42.4235f);


		vector0 /= 10.f;
		glm::vec3 resGlm = vec0 / 10.f;
		AreEqual(resGlm.x, vector0.mX, 10E-5);
		AreEqual(resGlm.y, vector0.mY, 10E-5);
		AreEqual(resGlm.z, vector0.mZ, 10E-5);

		vector1 /= 10.f;
		resGlm = vec1 / 10.f;
		AreEqual(resGlm.x, vector1.mX, 10E-5);
		AreEqual(resGlm.y, vector1.mY, 10E-5);
		AreEqual(resGlm.z, vector1.mZ, 10E-5);

		vector3 /= 10.f;
		resGlm = vec3 / 10.f;
		AreEqual(resGlm.x, vector3.mX, 10E-5);
		AreEqual(resGlm.y, vector3.mY, 10E-5);
		AreEqual(resGlm.z, vector3.mZ, 10E-5);

		vector2 /= PI;
		resGlm = vec2 / PI;
		AreEqual(resGlm.x, vector2.mX, 10E-5);
		AreEqual(resGlm.y, vector2.mY, 10E-5);
		AreEqual(resGlm.z, vector2.mZ, 10E-5);

		vector4 /= PI;
		resGlm = vec4 / PI;
		AreEqual(resGlm.x, vector4.mX, 10E-5);
		AreEqual(resGlm.y, vector4.mY, 10E-5);
		AreEqual(resGlm.z, vector4.mZ, 10E-5);

		vector5 /= std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(vector5.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector5.mY, -1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector5.mZ, -1.0f));

		vector6 /= std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(vector6.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector6.mY, -1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector6.mZ, -1.0f));

		vector7 /= std::numeric_limits<float>::lowest();
		IsTrue(maj::math::Helper<float>::areSame(vector7.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector7.mY, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector7.mZ, 1.0f));

		vector8 /= std::numeric_limits<float>::max();
		IsTrue(maj::math::Helper<float>::areSame(vector8.mX, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector8.mY, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector8.mZ, 1.0f));

	}

	TEST_METHOD("[]")
	{
		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec3<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		IsTrue(maj::math::Helper<float>::areSame(vector0[0u], 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector0[1u], 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector0[2u], 0.0f));

		IsTrue(maj::math::Helper<float>::areSame(vector1[0u], 10.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector1[1u], 11.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector1[2u], 12.0f));

		IsTrue(maj::math::Helper<float>::areSame(vector3[0u], 10.5f));
		IsTrue(maj::math::Helper<float>::areSame(vector3[1u], 11.25f));
		IsTrue(maj::math::Helper<float>::areSame(vector3[2u], 12.0025f));
		IsFalse(maj::math::Helper<float>::areSame(vector3[3u], 12.0025f));

		IsTrue(maj::math::Helper<float>::areSame(vector5[0u], std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(vector5[1u], std::numeric_limits<float>::max()));
		IsTrue(maj::math::Helper<float>::areSame(vector5[2u], std::numeric_limits<float>::max()));

		IsTrue(maj::math::Helper<float>::areSame(vector6[0u], std::numeric_limits<float>::max()));
		IsTrue(maj::math::Helper<float>::areSame(vector6[1u], std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(vector6[2u], std::numeric_limits<float>::max()));

		IsTrue(maj::math::Helper<float>::areSame(vector7[0u], std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(vector7[1u], std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(vector7[2u], std::numeric_limits<float>::lowest()));

		IsTrue(maj::math::Helper<float>::areSame(vector8[0u], std::numeric_limits<float>::max()));
		IsTrue(maj::math::Helper<float>::areSame(vector8[1u], std::numeric_limits<float>::max()));
		IsTrue(maj::math::Helper<float>::areSame(vector8[2u], std::numeric_limits<float>::max()));
	}

	TEST_METHOD("==")
	{

		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector9(52.0000007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec3<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		IsTrue(vector0 == maj::math::Vec3<float>(0.f, 0.f, 0.f));
		IsTrue(vector2 == maj::math::Vec3<float>(52.0f, 1.0f, 33.f));
		IsTrue(vector4 == maj::math::Vec3<float>(52.007f, 1.69f, 42.4235f));
		IsTrue(vector9 == maj::math::Vec3<float>(52.00000075f, 1.69f, 42.4235f));


		IsTrue(vector5 == maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));
		IsTrue(vector6 == maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		IsTrue(vector7 == maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()));
		IsTrue(vector8 == maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));

	}

	TEST_METHOD("!=")
	{
		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector9(2.12345670f, 1.69f, 42.4235f);
		maj::math::Vec3<float> vector5(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector6(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		maj::math::Vec3<float> vector7(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		maj::math::Vec3<float> vector8(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

		IsTrue(vector0 != maj::math::Vec3<float>(0.f, 1.f, 0.0f));
		IsTrue(vector2 != maj::math::Vec3<float>(5.0f, 1.0f, 0.0f));
		IsTrue(vector4 != maj::math::Vec3<float>(52.8f, 1.68f, 0.0f));

		IsTrue(vector9 != maj::math::Vec3<float>(2.12343680f, 1.69f, 42.4235f));


		IsTrue(vector7 != maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));
		IsTrue(vector8 != maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		IsTrue(vector5 != maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		IsTrue(vector6 != maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));

	}

	TEST_METHOD("lerp")
	{
		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);

		glm::vec3 vec0(0.0f, 0.0f, 0.0f);
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(52.0f, 1.0f, 33.f);
		glm::vec3 vec3(10.5f, 11.25f, 12.0025f);
		glm::vec3 vec4(52.007f, 1.69f, 42.4235f);

		maj::math::Vec3<float> test = maj::math::Vec3<float>::lerp(vector1, vector2, 0.5f);
		glm::vec3 testGlm = glm::lerp(vec1, vec2, 0.5f);
		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);
		AreEqual(testGlm.z, test.mZ, FLT_EPSILON);

		test = maj::math::Vec3<float>::lerp(vector3, vector4, 0.5f);	// Same with glm, except 1e-5 precision
		testGlm = glm::lerp(vec3, vec4, 0.5f);
		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);
		IsTrue(maj::math::Helper<float>::areSame(testGlm.z, test.mZ));

		test = maj::math::Vec3<float>::lerp(vector3, vector4, 0.0f);
		testGlm = glm::lerp(vec3, vec4, 0.0f);
		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);
		IsTrue(maj::math::Helper<float>::areSame(testGlm.z, test.mZ));

		test = maj::math::Vec3<float>::lerp(vector1, vector2, 1.0f);
		testGlm = glm::lerp(vec1, vec2, 1.0f);
		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);
		AreEqual(testGlm.z, test.mZ, FLT_EPSILON);

	}

	TEST_METHOD("Slerp")	//	pas slerp dans glm
	{
		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.25f);
		maj::math::Vec3<float> vector4(52.7f, 1.69f, 42.35f);

		maj::math::Vec3<float> test = maj::math::Vec3<float>::slerp(vector1, vector2, 0.5f);
		AreEqual(13.8181900f, test.mX, 10E-5);
		AreEqual(5.9813820f, test.mY, 10E-5);
		AreEqual(11.7592800f, test.mZ, 10E-5);

		test = maj::math::Vec3<float>::slerp(vector3, vector4, 0.5f);

		AreEqual(13.5360700f, test.mX, 10E-5);
		AreEqual(6.1537400f, test.mY, 10E-5);
		AreEqual(12.8756100f, test.mZ, 10E-5);
	
		test = maj::math::Vec3<float>::slerp(vector3, vector4, 0.0f);
		AreEqual(10.5000000f, test.mX, 10E-5);
		AreEqual(11.2500000f, test.mY, 10E-5);
		AreEqual(12.2500000f, test.mZ, 10E-5);

		test = maj::math::Vec3<float>::slerp(vector1, vector2, 1.0f);

		AreEqual(16.1287600f, test.mX, 10E-5);
		AreEqual(0.3101711f, test.mY, 10E-5);
		AreEqual(10.2355600f, test.mZ, 10E-5);

		test = maj::math::Vec3<float>::slerp(vector0, vector2, 0.5f);

		AreEqual(0.0f, test.mX, 10E-5);
		AreEqual(0.0f, test.mY, 10E-5);
		AreEqual(0.0f, test.mZ, 10E-5);

		test = maj::math::Vec3<float>::slerp(vector0, vector3, 0.5f);

		AreEqual(0.0f, test.mX, 10E-5);
		AreEqual(0.0f, test.mY, 10E-5);
		AreEqual(0.0f, test.mZ, 10E-5);
	}

	TEST_METHOD("float / ")
	{
		glm::vec3 vec0;
		glm::vec3 vec1(10.0f, 11.0f, 12.f);
		glm::vec3 vec2(52.0f, 1.0f, 33.f);
		glm::vec3 vec3(10.5f, 11.25f, 12.0025f);
		glm::vec3 vec4(52.007f, 1.69f, 42.4235f);

		maj::math::Vec3<float> vector0;
		maj::math::Vec3<float> vector1(10.0f, 11.0f, 12.f);
		maj::math::Vec3<float> vector2(52.0f, 1.0f, 33.f);
		maj::math::Vec3<float> vector3(10.5f, 11.25f, 12.0025f);
		maj::math::Vec3<float> vector4(52.007f, 1.69f, 42.4235f);


		maj::math::Vec3<float> res = 10.f / vector0;
		AreEqual(0.0f, res.mX, 10E-5);
		AreEqual(0.0f, res.mY, 10E-5);
		AreEqual(0.0f, res.mZ, 10E-5);

		res = 10.f / vector1;
		glm::vec3 resGlm = 10.0f / vec1;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);

		res = 10.f / vector3;
		resGlm = 10.f / vec3;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);

		res = PI / vector2;
		resGlm = PI / vec2;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);

		res = PI / vector4;
		resGlm = PI / vec4;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);
	}
}