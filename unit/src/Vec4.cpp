#include "Test.h"

#include <Vec4.h>

#define GLM_FORCE_LEFT_HANDED
#include <glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS

#include <gtx/vector_angle.hpp>
#include <gtx/compatibility.hpp>
#include <gtc/matrix_transform.hpp>

TEST_CLASS("Vec4")
{
	TEST_METHOD("constructor")
	{
		maj::math::Vec4<float> vector;
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mZ, 0.0f));
		AreEqual(0.0f, vector.mW, FLT_EPSILON);

		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.0f, 13.0f);
		IsTrue(maj::math::Helper<float>::areSame(vector1.mX, 10.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector1.mY, 11.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector1.mZ, 12.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector1.mW, 13.0f));

		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		AreEqual(10.5f, vector3.mX, FLT_EPSILON);
		AreEqual(11.25f, vector3.mY, FLT_EPSILON);
		AreEqual(12.0025f, vector3.mZ, FLT_EPSILON);
		IsTrue(maj::math::Helper<float>::areSame(vector3.mW, 13.0268f));


		maj::math::Vec4<float> vector2(maj::math::Vec4<float>::skUp);
		IsTrue(maj::math::Helper<float>::areSame(vector2.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector2.mY, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector2.mZ, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector2.mW, 0.0f));

		maj::math::Vec4<float> vector4(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
		IsTrue(maj::math::Helper<float>::areSame(vector4.mX, std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(vector4.mY, std::numeric_limits<float>::max()));
		IsTrue(maj::math::Helper<float>::areSame(vector4.mZ, std::numeric_limits<float>::max()));
		IsTrue(maj::math::Helper<float>::areSame(vector4.mW, std::numeric_limits<float>::lowest()));
	}

	TEST_METHOD("norm")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.f);
		glm::vec4 vec2(10.5f, 11.25f, 12.0025f, 13.0268f);

		maj::math::Vec4<float> vector;
		AreEqual(0.f, vector.norm(), FLT_EPSILON);

		vector = maj::math::Vec4<float>(10.0f, 11.0f, 12.f, 13.f);
		AreEqual(glm::length(vec1), vector.norm(), FLT_EPSILON);

		vector = maj::math::Vec4<float>(10.5f, 11.25f, 12.0025f, 13.0268f);
		AreEqual(glm::length(vec2), vector.norm(), FLT_EPSILON);
	}

	TEST_METHOD("sqr norm")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(10.5f, 11.25f, 12.0025f, 13.0268f);

		maj::math::Vec4<float> vector;
		AreEqual(0.f, vector.norm(), FLT_EPSILON);

		vector = maj::math::Vec4<float>(10.0f, 11.0f, 12.f, 13.0f);
		AreEqual(glm::length2(vec1), vector.sqrNorm(), FLT_EPSILON);

		vector = maj::math::Vec4<float>(10.5f, 11.25f, 12.0025f, 13.0268f);
		AreEqual(glm::length2(vec2), vector.sqrNorm(), FLT_EPSILON);
	}

	TEST_METHOD("normalized")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(10.5f, 11.25f, 12.0025f, 13.0268f);

		maj::math::Vec4<float> vector;
		glm::vec4 test;
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mZ, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.normalized().mW, 0.0f));


		vector = maj::math::Vec4<float>(10.0f, 11.0f, 12.f, 13.0f);
		test = glm::normalize(vec1);
		AreEqual(test.x, vector.normalized().mX, FLT_EPSILON);
		AreEqual(test.y, vector.normalized().mY, FLT_EPSILON);
		AreEqual(test.z, vector.normalized().mZ, FLT_EPSILON);
		AreEqual(test.w, vector.normalized().mW, FLT_EPSILON);


		vector = maj::math::Vec4<float>(10.5f, 11.25f, 12.0025f, 13.0268f);
		test = glm::normalize(vec2);
		AreEqual(test.x, vector.normalized().mX, FLT_EPSILON);
		AreEqual(test.y, vector.normalized().mY, FLT_EPSILON);
		AreEqual(test.z, vector.normalized().mZ, FLT_EPSILON);
		AreEqual(test.w, vector.normalized().mW, FLT_EPSILON);

	}

	TEST_METHOD("angle")
	{
		maj::math::Vec4<float> vector;
		maj::math::Vec4<float> vector0(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector1(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector2(10.5f, 11.25f, 12.025f, 13.0268f);
		maj::math::Vec4<float> vector3(52.007f, 1.69f, 42.435f, 82.56f);

		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(52.0f, 1.0f, 33.f, 42.0f);
		glm::vec4 vec3(10.5f, 11.25f, 12.025f, 13.0268f);
		glm::vec4 vec4(52.007f, 1.69f, 42.435f, 82.56f);
		vec1 = glm::normalize(vec1);
		vec2 = glm::normalize(vec2);
		vec3 = glm::normalize(vec3);
		vec4 = glm::normalize(vec4);

		AreEqual(vector.angle(vector), 0.0f, FLT_EPSILON);
		AreEqual(vector0.angle(vector), 0.0f, FLT_EPSILON);
		AreEqual(vector.angle(vector0), 0.0f, FLT_EPSILON);
		AreEqual(vector0.angle(vector1), glm::degrees(glm::angle(vec1, vec2)), FLT_EPSILON);
		AreEqual(0.0f, vector1.angle(vector1), FLT_EPSILON);
		AreEqual(0.0f, maj::math::Vec4<float>::skForward.angle(maj::math::Vec4<float>::skForward * 2), FLT_EPSILON);
		AreEqual(vector1.angle(vector), 0.0f, FLT_EPSILON);
		AreEqual(vector.angle(vector1), 0.0f, FLT_EPSILON);


		float angleMe = vector2.angle(vector3);
		float angleGLM = glm::degrees(glm::angle(vec3, vec4));
		IsTrue(maj::math::Helper<float>::areSame(angleMe, angleGLM));
	}

	TEST_METHOD("inversed")
	{
		maj::math::Vec4<float> vector;
		AreEqual(0.0f, vector.inversed().mX, FLT_EPSILON);
		AreEqual(0.0f, vector.inversed().mY, FLT_EPSILON);
		AreEqual(0.0f, vector.inversed().mZ, FLT_EPSILON);
		AreEqual(0.0f, vector.inversed().mW, FLT_EPSILON);

		vector = maj::math::Vec4<float>(10.0f, 11.0f, 12.f, 13.0f);
		AreEqual(1.0f / 10.0f, vector.inversed().mX, FLT_EPSILON);
		AreEqual(1.0f / 11.0f, vector.inversed().mY, FLT_EPSILON);
		AreEqual(1.0f / 12.0f, vector.inversed().mZ, FLT_EPSILON);
		AreEqual(1.0f / 13.0f, vector.inversed().mW, FLT_EPSILON);


		vector = maj::math::Vec4<float>(10.5f, 11.25f, 12.025f, 13.0268f);
		AreEqual(1.0f / 10.5f, vector.inversed().mX, FLT_EPSILON);
		AreEqual(1.0f / 11.25f, vector.inversed().mY, FLT_EPSILON);
		AreEqual(1.0f / 12.025f, vector.inversed().mZ, FLT_EPSILON);
		AreEqual(1.0f / 13.0268f, vector.inversed().mW, FLT_EPSILON);

	}

	TEST_METHOD("homogenized")
	{
			glm::vec4 vec0;
			glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
			glm::vec4 vec2(10.5f, 11.25f, 12.0025f, 13.0268f);

			maj::math::Vec4<float> vector;
			glm::vec4 test;
			IsTrue(maj::math::Helper<float>::areSame(vector.homogenized().mX, 0.0f));
			IsTrue(maj::math::Helper<float>::areSame(vector.homogenized().mY, 0.0f));
			IsTrue(maj::math::Helper<float>::areSame(vector.homogenized().mZ, 0.0f));
			IsTrue(maj::math::Helper<float>::areSame(vector.homogenized().mW, 0.0f));


			vector = maj::math::Vec4<float>(10.0f, 11.0f, 12.f, 13.0f);
			test = glm::vec4(10.0f / 13.0f, 11.0f / 13.0f, 12.f / 13.0f, 1.0f);
			AreEqual(test.x, vector.homogenized().mX, FLT_EPSILON);
			AreEqual(test.y, vector.homogenized().mY, FLT_EPSILON);
			AreEqual(test.z, vector.homogenized().mZ, FLT_EPSILON);
			AreEqual(test.w, vector.homogenized().mW, FLT_EPSILON);


			vector = maj::math::Vec4<float>(10.5f, 11.25f, 12.0025f, 13.0268f);
			test = glm::vec4(10.5f / 13.0268f, 11.25f / 13.0268f, 12.0025f / 13.0268f, 1.0f);
			AreEqual(test.x, vector.homogenized().mX, FLT_EPSILON);
			AreEqual(test.y, vector.homogenized().mY, FLT_EPSILON);
			AreEqual(test.z, vector.homogenized().mZ, FLT_EPSILON);
			AreEqual(test.w, vector.homogenized().mW, FLT_EPSILON);

	}

	TEST_METHOD("normalize")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(52.0f, 1.0f, 33.f, 42.0f);
		glm::vec4 vec3(10.5f, 11.25f, 12.25f, 13.68f);
		glm::vec4 vec4(52.007f, 1.69f, 42.435f, 82.56f);

		maj::math::Vec4<float> vector;
		vector.normalize();
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mZ, 0.0f));

		vector = maj::math::Vec4<float>(10.0f, 11.0f, 12.f, 13.0f);
		vector.normalize();
		glm::vec4 test = glm::normalize(vec1);
		AreEqual(test.x, vector.mX, FLT_EPSILON);
		AreEqual(test.y, vector.mY, FLT_EPSILON);
		AreEqual(test.z, vector.mZ, FLT_EPSILON);
		AreEqual(test.w, vector.mW, FLT_EPSILON);


		vector = maj::math::Vec4<float>(52.007f, 1.69f, 42.435f, 82.56f);
		vector.normalize();
		test = glm::normalize(vec4);
		IsTrue(maj::math::Helper<float>::areSame(test.x, vector.mX));
		IsTrue(maj::math::Helper<float>::areSame(test.y, vector.mY));
		IsTrue(maj::math::Helper<float>::areSame(test.z, vector.mZ));
		IsTrue(maj::math::Helper<float>::areSame(test.w, vector.mW));

	}

	TEST_METHOD("dot")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(52.0f, 1.0f, 33.f, 42.0f);
		glm::vec4 vec3(10.5f, 11.25f, 12.25f, 13.68f);
		glm::vec4 vec4(52.07f, 1.69f, 42.45f, 8.256f);

		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.25f, 13.68f);
		maj::math::Vec4<float> vector4(52.07f, 1.69f, 42.45f, 8.256f);

		IsTrue(maj::math::Helper<float>::areSame(vector0.dot(vector1), 0.0f));

		AreEqual(glm::dot(vec1, vec2), vector1.dot(vector2), FLT_EPSILON);

		float val1 = glm::dot(vec3, vec4);
		float val2 = vector3.dot(vector4);
		IsTrue(maj::math::Helper<float>::areSame(val1, val2));

		val1 = glm::dot(vec2, vec4);
		val2 = vector2.dot(vector4);
		IsTrue(maj::math::Helper<float>::areSame(val1, val2));

		val1 = glm::dot(vec3, vec3);
		val2 = vector3.dot(vector3);
		IsTrue(maj::math::Helper<float>::areSame(val1, val2));
	}

	TEST_METHOD("homogenize")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(10.5f, 11.25f, 12.0025f, 13.0268f);

		maj::math::Vec4<float> vector;
		vector.homogenize();
		glm::vec4 test;
		IsTrue(maj::math::Helper<float>::areSame(vector.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mZ, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector.mW, 0.0f));


		vector = maj::math::Vec4<float>(10.0f, 11.0f, 12.f, 13.0f);
		vector.homogenize();
		test = glm::vec4(10.0f / 13.0f, 11.0f / 13.0f, 12.f / 13.0f, 1.0f);
		AreEqual(test.x, vector.mX, FLT_EPSILON);
		AreEqual(test.y, vector.mY, FLT_EPSILON);
		AreEqual(test.z, vector.mZ, FLT_EPSILON);
		AreEqual(test.w, vector.mW, FLT_EPSILON);


		vector = maj::math::Vec4<float>(10.5f, 11.25f, 12.0025f, 13.0268f);
		vector.homogenize();
		test = glm::vec4(10.5f / 13.0268f, 11.25f / 13.0268f, 12.0025f / 13.0268f, 1.0f);
		AreEqual(test.x, vector.mX, FLT_EPSILON);
		AreEqual(test.y, vector.mY, FLT_EPSILON);
		AreEqual(test.z, vector.mZ, FLT_EPSILON);
		AreEqual(test.w, vector.mW, FLT_EPSILON);

	}


	TEST_METHOD("inverse")
	{
		maj::math::Vec4<float> vector;
		vector.inverse();
		AreEqual(0.0f, vector.mX, FLT_EPSILON);
		AreEqual(0.0f, vector.mY, FLT_EPSILON);
		AreEqual(0.0f, vector.mZ, FLT_EPSILON);

		vector = maj::math::Vec4<float>(10.0f, 11.0f, 12.f, 13.0f);
		vector.inverse();
		AreEqual(1.0f / 10.0f, vector.mX, FLT_EPSILON);
		AreEqual(1.0f / 11.0f, vector.mY, FLT_EPSILON);
		AreEqual(1.0f / 12.0f, vector.mZ, FLT_EPSILON);

		vector = maj::math::Vec4<float>(10.5f, 11.25f, 12.0025f, 13.0268f);
		vector.inverse();
		AreEqual(1.0f / 10.5f, vector.mX, FLT_EPSILON);
		AreEqual(1.0f / 11.25f, vector.mY, FLT_EPSILON);
		AreEqual(1.0f / 12.0025f, vector.mZ, FLT_EPSILON);
	}

	TEST_METHOD("clamp")
	{

		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);

		maj::math::Vec4<float> vector;
		AreEqual(1.0f, vector.clamp(maj::math::Vec4<float>(1.f, 2.f, 3.f, 4.f), maj::math::Vec4<float>(10.f, 11.f, 12.f, 13.f)).mX, FLT_EPSILON);
		AreEqual(2.0f, vector.clamp(maj::math::Vec4<float>(1.f, 2.f, 3.f, 4.f), maj::math::Vec4<float>(10.f, 11.f, 12.f, 13.f)).mY, FLT_EPSILON);
		AreEqual(3.0f, vector.clamp(maj::math::Vec4<float>(1.f, 2.f, 3.f, 4.f), maj::math::Vec4<float>(10.f, 11.f, 12.f, 13.f)).mZ, FLT_EPSILON);
		AreEqual(4.0f, vector.clamp(maj::math::Vec4<float>(1.f, 2.f, 3.f, 4.f), maj::math::Vec4<float>(10.f, 11.f, 12.f, 13.f)).mW, FLT_EPSILON);

		vector = vector1;
		AreEqual(9.0f, vector.clamp(maj::math::Vec4<float>(1.f, 2.f, 13.f, 18.f), maj::math::Vec4<float>(9.f, 9.f, 15.f, 40.f)).mX, FLT_EPSILON);
		AreEqual(9.0f, vector.clamp(maj::math::Vec4<float>(1.f, 2.f, 13.f, 18.f), maj::math::Vec4<float>(9.f, 9.f, 15.f, 40.f)).mY, FLT_EPSILON);
		AreEqual(13.f, vector.clamp(maj::math::Vec4<float>(1.f, 2.f, 13.f, 18.f), maj::math::Vec4<float>(9.f, 9.f, 15.f, 40.f)).mZ, FLT_EPSILON);
		AreEqual(18.f, vector.clamp(maj::math::Vec4<float>(1.f, 2.f, 13.f, 18.f), maj::math::Vec4<float>(9.f, 9.f, 15.f, 40.f)).mW, FLT_EPSILON);

		vector = vector3;
		AreEqual(10.25f, vector.clamp(maj::math::Vec4<float>(1.f, 2.f, 12.0026f, 5.f), maj::math::Vec4<float>(10.25f, 10.24f, 150.50f, 11.f)).mX, FLT_EPSILON);
		AreEqual(10.24f, vector.clamp(maj::math::Vec4<float>(1.f, 2.f, 12.0026f, 5.f), maj::math::Vec4<float>(10.25f, 10.24f, 150.50f, 11.f)).mY, FLT_EPSILON);
		AreEqual(12.0026f, vector.clamp(maj::math::Vec4<float>(1.f, 2.f, 12.0026f, 5.f), maj::math::Vec4<float>(10.25f, 10.24f, 150.50f, 11.f)).mZ, FLT_EPSILON);
		AreEqual(11.f, vector.clamp(maj::math::Vec4<float>(1.f, 2.f, 12.0026f, 5.f), maj::math::Vec4<float>(10.25f, 10.24f, 150.50f, 11.f)).mW, FLT_EPSILON);

	}

	TEST_METHOD("* float")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(52.0f, 1.0f, 33.f, 42.0f);
		glm::vec4 vec3(10.5f, 11.25f, 12.0025f, 13.0268f);
		glm::vec4 vec4(52.007f, 1.69f, 42.4235f, 82.56f);

		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);

		maj::math::Vec4<float> res = vector0 * 10.f;
		glm::vec4 resGlm = vec0 * 10.0f;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);


		res = vector1 * 10.f;
		resGlm = vec1 * 10.0f;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);

		res = vector3 * 10.f;
		resGlm = vec3 * 10.0f;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);

		res = vector2 * PI;
		resGlm = vec2 * PI;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);

		res = vector4 * PI;
		resGlm = vec4 * PI;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);

		res = vector1 * 0.f;
		resGlm = vec1 * 0.0f;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);

		res = vector4 * 0.f;
		resGlm = vec4 * 0.0f;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);
	}

	TEST_METHOD("/ float")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(52.0f, 1.0f, 33.f, 42.0f);
		glm::vec4 vec3(10.5f, 11.25f, 12.0025f, 13.0268f);
		glm::vec4 vec4(52.007f, 1.69f, 42.4235f, 82.56f);

		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);


		maj::math::Vec4<float> res = vector0 / 10.f;
		glm::vec4 resGlm = vec0 / 10.f;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);
		AreEqual(resGlm.w, res.mW, 10E-5);

		res = vector1 / 10.f;
		resGlm = vec1 / 10.f;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);
		AreEqual(resGlm.w, res.mW, 10E-5);

		res = vector3 / 10.f;
		resGlm = vec3 / 10.f;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);
		AreEqual(resGlm.w, res.mW, 10E-5);

		res = vector2 / PI;
		resGlm = vec2 / PI;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);
		AreEqual(resGlm.w, res.mW, 10E-5);

		res = vector4 / PI;
		resGlm = vec4 / PI;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);
		AreEqual(resGlm.w, res.mW, 10E-5);
	}

	TEST_METHOD("+ Vec4")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(52.0f, 1.0f, 33.f, 42.0f);
		glm::vec4 vec3(10.5f, 11.25f, 12.0025f, 13.0268f);
		glm::vec4 vec4(52.007f, 1.69f, 42.4235f, 82.56f);

		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);

		maj::math::Vec4<float> res = vector0 + vector1;
		glm::vec4 resGlm = vec0 + vec1;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);

		res = vector1 + ( -vector2 );
		resGlm = vec1 + ( -vec2 );
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);

		res = vector3 + vector4;
		resGlm = vec3 + vec4;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);

		res = vector4 + vector4;
		resGlm = vec4 + vec4;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);
	}

	TEST_METHOD("- Vec4")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(52.0f, 1.0f, 33.f, 42.0f);
		glm::vec4 vec3(10.5f, 11.25f, 12.0025f, 13.0268f);
		glm::vec4 vec4(52.007f, 1.69f, 42.4235f, 82.56f);

		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);


		maj::math::Vec4<float> res = vector0 - vector1;
		glm::vec4 resGlm = vec0 - vec1;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);

		res = vector1 - vector4;
		resGlm = vec1 - vec4;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);

		res = vector3 - vector4;
		resGlm = vec3 - vec4;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);

		res = vector3 - vector3;
		resGlm = vec3 - vec3;
		AreEqual(resGlm.x, res.mX, FLT_EPSILON);
		AreEqual(resGlm.y, res.mY, FLT_EPSILON);
		AreEqual(resGlm.z, res.mZ, FLT_EPSILON);
		AreEqual(resGlm.w, res.mW, FLT_EPSILON);
	}

	TEST_METHOD("scale")
	{
		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);

		maj::math::Vec4<float> res = vector0.scaled(vector1);
		AreEqual(0.0f, res.mX, FLT_EPSILON);
		AreEqual(0.0f, res.mY, FLT_EPSILON);
		AreEqual(0.0f, res.mZ, FLT_EPSILON);
		AreEqual(0.0f, res.mW, FLT_EPSILON);


		res = vector1.scaled(vector4);
		AreEqual(10.0f * 52.007f, res.mX, FLT_EPSILON);
		AreEqual(11.f  * 1.69f, res.mY, FLT_EPSILON);
		AreEqual(12.0f * 42.4235f, res.mZ, FLT_EPSILON);
		AreEqual(13.0f * 82.56f, res.mW, FLT_EPSILON);


		res = vector3.scaled(vector4);
		AreEqual(10.50f   * 52.007f, res.mX, FLT_EPSILON);
		AreEqual(11.25f   * 1.69f, res.mY, FLT_EPSILON);
		AreEqual(12.0025f * 42.4235f, res.mZ, FLT_EPSILON);
		AreEqual(13.0268f * 82.56f, res.mW, FLT_EPSILON);

	}

	TEST_METHOD("-")
	{
		maj::math::Vec4<float> vector;
		maj::math::Vec4<float> vector0 = -vector;
		AreEqual(vector0.mX, 0.0f, FLT_EPSILON);
		AreEqual(vector0.mY, 0.0f, FLT_EPSILON);
		AreEqual(vector0.mZ, 0.0f, FLT_EPSILON);
		AreEqual(vector0.mW, 0.0f, FLT_EPSILON);


		vector0 = maj::math::Vec4<float>(10.0f, -11.0f, -12.f, 13.0f);
		vector0 = -vector0;
		AreEqual(vector0.mX, -10.0f, FLT_EPSILON);
		AreEqual(vector0.mY, 11.0f, FLT_EPSILON);
		AreEqual(vector0.mZ, 12.0f, FLT_EPSILON);
		AreEqual(vector0.mW, -13.0f, FLT_EPSILON);

		vector0 = maj::math::Vec4<float>(-10.5f, 11.25f, -12.0025f, 13.0268f);
		vector0 = -vector0;
		AreEqual(vector0.mX, 10.5f, FLT_EPSILON);
		AreEqual(vector0.mY, -11.25f, FLT_EPSILON);
		AreEqual(vector0.mZ, 12.0025f, FLT_EPSILON);
		AreEqual(vector0.mW, -13.0268f, FLT_EPSILON);

	}

	TEST_METHOD("+= Vec4")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(52.0f, 1.0f, 33.f, 42.0f);
		glm::vec4 vec3(10.5f, 11.25f, 12.0025f, 13.0268f);
		glm::vec4 vec4(52.007f, 1.69f, 42.4235f, 82.56f);

		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);

		vector0 += vector1;
		vec0 += vec1;
		AreEqual(vector0.mX, vec0.x, FLT_EPSILON);
		AreEqual(vector0.mY, vec0.y, FLT_EPSILON);
		AreEqual(vector0.mZ, vec0.z, FLT_EPSILON);
		AreEqual(vector0.mW, vec0.w, FLT_EPSILON);


		vector1 += vector4;
		vec1 += vec4;
		AreEqual(vector1.mX, vec1.x, FLT_EPSILON);
		AreEqual(vector1.mY, vec1.y, FLT_EPSILON);
		AreEqual(vector1.mZ, vec1.z, FLT_EPSILON);
		AreEqual(vector1.mW, vec1.w, FLT_EPSILON);


		vector3 += vector4;
		vec3 += vec4;
		AreEqual(vector3.mX, vec3.x, FLT_EPSILON);
		AreEqual(vector3.mY, vec3.y, FLT_EPSILON);
		AreEqual(vector3.mZ, vec3.z, FLT_EPSILON);
		AreEqual(vector3.mW, vec3.w, FLT_EPSILON);
		AreEqual(vector3.mW, vec3.w, FLT_EPSILON);

		vector4 += vector4;
		vec4 += vec4;
		AreEqual(vector4.mX, vec4.x, FLT_EPSILON);
		AreEqual(vector4.mY, vec4.y, FLT_EPSILON);
		AreEqual(vector4.mZ, vec4.z, FLT_EPSILON);
		AreEqual(vector4.mW, vec4.w, FLT_EPSILON);
	}

	TEST_METHOD("-= Vec4")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(52.0f, 1.0f, 33.f, 42.0f);
		glm::vec4 vec3(10.5f, 11.25f, 12.0025f, 13.0268f);
		glm::vec4 vec4(52.007f, 1.69f, 42.4235f, 82.56f);

		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);

		vector0 -= vector1;
		vec0 -= vec1;
		AreEqual(vector0.mX, vec0.x, FLT_EPSILON);
		AreEqual(vector0.mY, vec0.y, FLT_EPSILON);
		AreEqual(vector0.mZ, vec0.z, FLT_EPSILON);
		AreEqual(vector0.mW, vec0.w, FLT_EPSILON);


		vector1 -= vector4;
		vec1 -= vec4;
		AreEqual(vector1.mX, vec1.x, FLT_EPSILON);
		AreEqual(vector1.mY, vec1.y, FLT_EPSILON);
		AreEqual(vector1.mZ, vec1.z, FLT_EPSILON);
		AreEqual(vector1.mW, vec1.w, FLT_EPSILON);

		vector3 -= vector4;
		vec3 -= vec4;
		AreEqual(vector3.mX, vec3.x, FLT_EPSILON);
		AreEqual(vector3.mY, vec3.y, FLT_EPSILON);
		AreEqual(vector3.mZ, vec3.z, FLT_EPSILON);
		AreEqual(vector3.mW, vec3.w, FLT_EPSILON);

		vector4 -= vector4;
		vec4 -= vec4;
		AreEqual(vector4.mX, vec4.x, FLT_EPSILON);
		AreEqual(vector4.mY, vec4.y, FLT_EPSILON);
		AreEqual(vector4.mZ, vec4.z, FLT_EPSILON);
		AreEqual(vector4.mW, vec4.w, FLT_EPSILON);
	}

	TEST_METHOD("*= float")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(52.0f, 1.0f, 33.f, 42.0f);
		glm::vec4 vec3(10.5f, 11.25f, 12.0025f, 13.0268f);
		glm::vec4 vec4(52.007f, 1.69f, 42.4235f, 82.56f);

		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);

		vector0 *= 10.f;
		vec0 *= 10.f;
		AreEqual(vec0.x, vector0.mX, FLT_EPSILON);
		AreEqual(vec0.y, vector0.mY, FLT_EPSILON);
		AreEqual(vec0.z, vector0.mZ, FLT_EPSILON);
		AreEqual(vec0.w, vector0.mW, FLT_EPSILON);

		vector1 *= 10.f;
		vec1 *= 10.f;
		AreEqual(vec1.x, vector1.mX, FLT_EPSILON);
		AreEqual(vec1.y, vector1.mY, FLT_EPSILON);
		AreEqual(vec1.z, vector1.mZ, FLT_EPSILON);
		AreEqual(vec1.w, vector1.mW, FLT_EPSILON);


		vector3 *= 10.f;
		vec3 *= 10.f;
		AreEqual(vec3.x, vector3.mX, FLT_EPSILON);
		AreEqual(vec3.y, vector3.mY, FLT_EPSILON);
		AreEqual(vec3.z, vector3.mZ, FLT_EPSILON);
		AreEqual(vec3.w, vector3.mW, FLT_EPSILON);

		vector2 *= PI;
		vec2 *= PI;
		AreEqual(vec2.x, vector2.mX, FLT_EPSILON);
		AreEqual(vec2.y, vector2.mY, FLT_EPSILON);
		AreEqual(vec2.z, vector2.mZ, FLT_EPSILON);
		AreEqual(vec2.w, vector2.mW, FLT_EPSILON);

		vector4 *= PI;
		vec4 *= PI;
		AreEqual(vec4.x, vector4.mX, FLT_EPSILON);
		AreEqual(vec4.y, vector4.mY, FLT_EPSILON);
		AreEqual(vec4.z, vector4.mZ, FLT_EPSILON);
		AreEqual(vec4.w, vector4.mW, FLT_EPSILON);

		vector4 *= 0.0f;
		vec4 *= 0.0f;
		AreEqual(vec4.x, vector4.mX, FLT_EPSILON);
		AreEqual(vec4.y, vector4.mY, FLT_EPSILON);
		AreEqual(vec4.z, vector4.mZ, FLT_EPSILON);
		AreEqual(vec4.w, vector4.mW, FLT_EPSILON);
	}

	TEST_METHOD("/= float")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(52.0f, 1.0f, 33.f, 42.0f);
		glm::vec4 vec3(10.5f, 11.25f, 12.0025f, 13.0268f);
		glm::vec4 vec4(52.007f, 1.69f, 42.4235f, 82.56f);

		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);


		vector0 /= 10.f;
		glm::vec4 resGlm = vec0 / 10.f;
		AreEqual(resGlm.x, vector0.mX, 10E-5);
		AreEqual(resGlm.y, vector0.mY, 10E-5);
		AreEqual(resGlm.z, vector0.mZ, 10E-5);
		AreEqual(resGlm.w, vector0.mW, 10E-5);

		vector1 /= 10.f;
		resGlm = vec1 / 10.f;
		AreEqual(resGlm.x, vector1.mX, 10E-5);
		AreEqual(resGlm.y, vector1.mY, 10E-5);
		AreEqual(resGlm.z, vector1.mZ, 10E-5);
		AreEqual(resGlm.w, vector1.mW, 10E-5);

		vector3 /= 10.f;
		resGlm = vec3 / 10.f;
		AreEqual(resGlm.x, vector3.mX, 10E-5);
		AreEqual(resGlm.y, vector3.mY, 10E-5);
		AreEqual(resGlm.z, vector3.mZ, 10E-5);
		AreEqual(resGlm.w, vector3.mW, 10E-5);

		vector2 /= PI;
		resGlm = vec2 / PI;
		AreEqual(resGlm.x, vector2.mX, 10E-5);
		AreEqual(resGlm.y, vector2.mY, 10E-5);
		AreEqual(resGlm.z, vector2.mZ, 10E-5);
		AreEqual(resGlm.w, vector2.mW, 10E-5);

		vector4 /= PI;
		resGlm = vec4 / PI;
		AreEqual(resGlm.x, vector4.mX, 10E-5);
		AreEqual(resGlm.y, vector4.mY, 10E-5);
		AreEqual(resGlm.z, vector4.mZ, 10E-5);
		AreEqual(resGlm.w, vector4.mW, 10E-5);
	}

	TEST_METHOD("[]")
	{
		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);

		IsTrue(maj::math::Helper<float>::areSame(vector0[0u], 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector0[1u], 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector0[2u], 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector0[3u], 0.0f));


		IsTrue(maj::math::Helper<float>::areSame(vector1[0u], 10.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector1[1u], 11.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector1[2u], 12.0f));
		IsTrue(maj::math::Helper<float>::areSame(vector1[3u], 13.0f));

		IsTrue(maj::math::Helper<float>::areSame(vector3[0u], 10.5f));
		IsTrue(maj::math::Helper<float>::areSame(vector3[1u], 11.25f));
		IsTrue(maj::math::Helper<float>::areSame(vector3[2u], 12.0025f));
		IsTrue(maj::math::Helper<float>::areSame(vector3[3u], 13.0268f));
		IsFalse(maj::math::Helper<float>::areSame(vector3[4u], 13.0268f));
	}

	TEST_METHOD("==")
	{
		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);

		IsTrue(vector0 == maj::math::Vec4<float>(0.f, 0.f, 0.f, 0.f));
		IsTrue(vector2 == maj::math::Vec4<float>(52.0f, 1.0f, 33.f, 42.0f));
		IsTrue(vector4 == maj::math::Vec4<float>(52.007f, 1.69f, 42.4235f, 82.56f));
	}

	TEST_METHOD("!=")
	{
		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);

		IsTrue(vector0 != maj::math::Vec4<float>(0.f, 1.f, 0.0f, 0.0f));
		IsTrue(vector2 != maj::math::Vec4<float>(5.0f, 1.0f, 0.0f, 0.0f));
		IsTrue(vector4 != maj::math::Vec4<float>(52.8f, 1.68f, 0.0f, 0.0f));
	}

	TEST_METHOD("lerp")	//	precision problem
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(52.0f, 1.0f, 33.f, 42.0f);
		glm::vec4 vec3(10.5f, 11.25f, 12.25f, 13.68f);
		glm::vec4 vec4(52.7f, 1.69f, 42.35f, 82.56f);

		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.25f, 13.68f);
		maj::math::Vec4<float> vector4(52.7f, 1.69f, 42.35f, 82.56f);

		maj::math::Vec4<float> test = maj::math::Vec4<float>::lerp(vector1, vector2, 0.5f);
		glm::vec4 testGlm = glm::lerp(vec1, vec2, 0.5f);

		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);
		AreEqual(testGlm.z, test.mZ, FLT_EPSILON);
		AreEqual(testGlm.w, test.mW, FLT_EPSILON);

		test = maj::math::Vec4<float>::lerp(vector3, vector4, 0.5f);
		testGlm = glm::lerp(vec3, vec4, 0.5f);

		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);
		AreEqual(testGlm.z, test.mZ, FLT_EPSILON);
		IsTrue(maj::math::Helper<float>::areSame(testGlm.z, test.mZ));
		IsTrue(maj::math::Helper<float>::areSame(testGlm.w, test.mW));

		test = maj::math::Vec4<float>::lerp(vector3, vector4, 0.0f);
		testGlm = glm::lerp(vec3, vec4, 0.0f);

		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);
		IsTrue(maj::math::Helper<float>::areSame(testGlm.z, test.mZ));
		IsTrue(maj::math::Helper<float>::areSame(testGlm.w, test.mW));

		test = maj::math::Vec4<float>::lerp(vector1, vector2, 1.0f);
		testGlm = glm::lerp(vec1, vec2, 1.0f);

		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);
		AreEqual(testGlm.z, test.mZ, FLT_EPSILON);
		IsTrue(maj::math::Helper<float>::areSame(testGlm.w, test.mW));

		test = maj::math::Vec4<float>::lerp(vector0, vector2, 0.5f);
		testGlm = glm::lerp(vec0, vec2, 0.5f);

		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);
		AreEqual(testGlm.z, test.mZ, FLT_EPSILON);
		IsTrue(maj::math::Helper<float>::areSame(testGlm.w, test.mW));

		test = maj::math::Vec4<float>::lerp(vector0, vector3, 0.5f);
		testGlm = glm::lerp(vec0, vec3, 0.5f);

		AreEqual(testGlm.x, test.mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.mY, FLT_EPSILON);
		AreEqual(testGlm.z, test.mZ, FLT_EPSILON);
		IsTrue(maj::math::Helper<float>::areSame(testGlm.w, test.mW));
	}

	TEST_METHOD("slerp")	//	pas slerp dans glm
	{
		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 0.f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 0.f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.25f, 0.f);
		maj::math::Vec4<float> vector4(52.7f, 1.69f, 42.35f, 0.f);

		maj::math::Vec4<float> test = maj::math::Vec4<float>::slerp(vector1, vector2, 0.5f);

		AreEqual(13.8181900f, test.mX, 10E-5);
		AreEqual(5.9813820f, test.mY, 10E-5);
		AreEqual(11.7592800f, test.mZ, 10E-5);
		IsTrue(maj::math::Helper<float>::areSame(0.0f, test.mW));

		test = maj::math::Vec4<float>::slerp(vector3, vector4, 0.5f);

		AreEqual(13.5360700f, test.mX, 10E-5);
		AreEqual(6.1537400f, test.mY, 10E-5);
		AreEqual(12.8756100f, test.mZ, 10E-5);
		IsTrue(maj::math::Helper<float>::areSame(0.0f, test.mW));

		test = maj::math::Vec4<float>::slerp(vector3, vector4, 0.0f);

		AreEqual(10.5000000f, test.mX, 10E-5);
		AreEqual(11.2500000f, test.mY, 10E-5);
		AreEqual(12.2500000f, test.mZ, 10E-5);
		IsTrue(maj::math::Helper<float>::areSame(0.0f, test.mW));

		test = maj::math::Vec4<float>::slerp(vector1, vector2, 1.0f);

		AreEqual(16.1287600f, test.mX, 10E-5);
		AreEqual(0.3101711f, test.mY, 10E-5);
		AreEqual(10.2355600f, test.mZ, 10E-5);
		IsTrue(maj::math::Helper<float>::areSame(0.0f, test.mW));

		test = maj::math::Vec4<float>::slerp(vector0, vector2, 0.5f);

		AreEqual(0.0f, test.mX, 10E-5);
		AreEqual(0.0f, test.mY, 10E-5);
		AreEqual(0.0f, test.mZ, 10E-5);
		IsTrue(maj::math::Helper<float>::areSame(0.0f, test.mW));

		test = maj::math::Vec4<float>::slerp(vector0, vector3, 0.5f);

		AreEqual(0.0f, test.mX, 10E-5);
		AreEqual(0.0f, test.mY, 10E-5);
		AreEqual(0.0f, test.mZ, 10E-5);
		IsTrue(maj::math::Helper<float>::areSame(0.0f, test.mW));
	}


	TEST_METHOD("float / ")
	{
		glm::vec4 vec0;
		glm::vec4 vec1(10.0f, 11.0f, 12.f, 13.0f);
		glm::vec4 vec2(52.0f, 1.0f, 33.f, 42.0f);
		glm::vec4 vec3(10.5f, 11.25f, 12.0025f, 13.0268f);
		glm::vec4 vec4(52.007f, 1.69f, 42.4235f, 82.56f);

		maj::math::Vec4<float> vector0;
		maj::math::Vec4<float> vector1(10.0f, 11.0f, 12.f, 13.0f);
		maj::math::Vec4<float> vector2(52.0f, 1.0f, 33.f, 42.0f);
		maj::math::Vec4<float> vector3(10.5f, 11.25f, 12.0025f, 13.0268f);
		maj::math::Vec4<float> vector4(52.007f, 1.69f, 42.4235f, 82.56f);


		maj::math::Vec4<float> res = 10.f / vector0;
		AreEqual(0.0f, res.mX, 10E-5);
		AreEqual(0.0f, res.mY, 10E-5);
		AreEqual(0.0f, res.mZ, 10E-5);
		AreEqual(0.0f, res.mW, 10E-5);

		res = 10.f / vector1;
		glm::vec4 resGlm = 10.0f / vec1 ;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);
		AreEqual(resGlm.w, res.mW, 10E-5);

		res = 10.f / vector3;
		resGlm = 10.f / vec3;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);
		AreEqual(resGlm.w, res.mW, 10E-5);

		res = PI / vector2;
		resGlm =  PI / vec2;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);
		AreEqual(resGlm.w, res.mW, 10E-5);

		res = PI / vector4;
		resGlm = PI / vec4;
		AreEqual(resGlm.x, res.mX, 10E-5);
		AreEqual(resGlm.y, res.mY, 10E-5);
		AreEqual(resGlm.z, res.mZ, 10E-5);
		AreEqual(resGlm.w, res.mW, 10E-5);
	}


}