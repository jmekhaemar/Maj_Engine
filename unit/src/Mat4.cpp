#include "Test.h"

#include <Mat4.h>

#define GLM_FORCE_LEFT_HANDED
#include <glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS

#include <gtx/euler_angles.hpp>
#include <gtx/transform.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/matrix_transform.hpp>

TEST_CLASS("Mat4")
{
	TEST_METHOD("constructor")
	{
		maj::math::Mat4<float> mat;
		glm::mat4 matt { 1.f };

		AreEqual(mat[0u], matt[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], matt[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], matt[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], matt[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], matt[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], matt[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], matt[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], matt[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], matt[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], matt[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], matt[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], matt[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], matt[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], matt[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], matt[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], matt[3u][3u], FLT_EPSILON);

		maj::math::Mat4<float> mat1 { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f };
		glm::mat4 matt1 { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f };

		AreEqual(mat1[0u], matt1[0u][0u], FLT_EPSILON);
		AreEqual(mat1[1u], matt1[0u][1u], FLT_EPSILON);
		AreEqual(mat1[2u], matt1[0u][2u], FLT_EPSILON);
		AreEqual(mat1[3u], matt1[0u][3u], FLT_EPSILON);
		AreEqual(mat1[4u], matt1[1u][0u], FLT_EPSILON);
		AreEqual(mat1[5u], matt1[1u][1u], FLT_EPSILON);
		AreEqual(mat1[6u], matt1[1u][2u], FLT_EPSILON);
		AreEqual(mat1[7u], matt1[1u][3u], FLT_EPSILON);
		AreEqual(mat1[8u], matt1[2u][0u], FLT_EPSILON);
		AreEqual(mat1[9u], matt1[2u][1u], FLT_EPSILON);
		AreEqual(mat1[10u], matt1[2u][2u], FLT_EPSILON);
		AreEqual(mat1[11u], matt1[2u][3u], FLT_EPSILON);
		AreEqual(mat1[12u], matt1[3u][0u], FLT_EPSILON);
		AreEqual(mat1[13u], matt1[3u][1u], FLT_EPSILON);
		AreEqual(mat1[14u], matt1[3u][2u], FLT_EPSILON);
		AreEqual(mat1[15u], matt1[3u][3u], FLT_EPSILON);

		maj::math::Mat4<float> mat2 { maj::math::Mat4<float>::skZero };
		glm::mat4 matt2 { 0.f };

		AreEqual(mat2[0u], matt2[0u][0u], FLT_EPSILON);
		AreEqual(mat2[1u], matt2[0u][1u], FLT_EPSILON);
		AreEqual(mat2[2u], matt2[0u][2u], FLT_EPSILON);
		AreEqual(mat2[3u], matt2[0u][3u], FLT_EPSILON);
		AreEqual(mat2[4u], matt2[1u][0u], FLT_EPSILON);
		AreEqual(mat2[5u], matt2[1u][1u], FLT_EPSILON);
		AreEqual(mat2[6u], matt2[1u][2u], FLT_EPSILON);
		AreEqual(mat2[7u], matt2[1u][3u], FLT_EPSILON);
		AreEqual(mat2[8u], matt2[2u][0u], FLT_EPSILON);
		AreEqual(mat2[9u], matt2[2u][1u], FLT_EPSILON);
		AreEqual(mat2[10u], matt2[2u][2u], FLT_EPSILON);
		AreEqual(mat2[11u], matt2[2u][3u], FLT_EPSILON);
		AreEqual(mat2[12u], matt2[3u][0u], FLT_EPSILON);
		AreEqual(mat2[13u], matt2[3u][1u], FLT_EPSILON);
		AreEqual(mat2[14u], matt2[3u][2u], FLT_EPSILON);
		AreEqual(mat2[15u], matt2[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("==")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);

		maj::math::Mat4<float> mat3;
		maj::math::Mat4<float> mat4(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
		maj::math::Mat4<float> mat5(maj::math::Mat4<float>::skZero);

		IsTrue(mat == mat3);
		IsTrue(mat1 == mat4);
		IsTrue(mat2 == mat5);
	}

	TEST_METHOD("elem")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);

		float* test = mat.elem();

		AreEqual(test[0u], 1.f, FLT_EPSILON);
		AreEqual(test[1u], 0.f, FLT_EPSILON);
		AreEqual(test[2u], 0.f, FLT_EPSILON);
		AreEqual(test[3u], 0.f, FLT_EPSILON);
		AreEqual(test[4u], 0.f, FLT_EPSILON);
		AreEqual(test[5u], 1.f, FLT_EPSILON);
		AreEqual(test[6u], 0.f, FLT_EPSILON);
		AreEqual(test[7u], 0.f, FLT_EPSILON);
		AreEqual(test[8u], 0.f, FLT_EPSILON);
		AreEqual(test[9u], 0.f, FLT_EPSILON);
		AreEqual(test[10u], 1.f, FLT_EPSILON);
		AreEqual(test[11u], 0.f, FLT_EPSILON);
		AreEqual(test[12u], 0.f, FLT_EPSILON);
		AreEqual(test[13u], 0.f, FLT_EPSILON);
		AreEqual(test[14u], 0.f, FLT_EPSILON);
		AreEqual(test[15u], 1.f, FLT_EPSILON);

		test = mat1.elem();
		AreEqual(test[0u], 0.f, FLT_EPSILON);
		AreEqual(test[1u], 1.f, FLT_EPSILON);
		AreEqual(test[2u], 2.f, FLT_EPSILON);
		AreEqual(test[3u], 3.f, FLT_EPSILON);
		AreEqual(test[4u], 4.f, FLT_EPSILON);
		AreEqual(test[5u], 5.f, FLT_EPSILON);
		AreEqual(test[6u], 6.f, FLT_EPSILON);
		AreEqual(test[7u], 7.f, FLT_EPSILON);
		AreEqual(test[8u], 8.f, FLT_EPSILON);
		AreEqual(test[9u], 9.f, FLT_EPSILON);
		AreEqual(test[10u], 10.f, FLT_EPSILON);
		AreEqual(test[11u], 11.f, FLT_EPSILON);
		AreEqual(test[12u], 12.f, FLT_EPSILON);
		AreEqual(test[13u], 13.f, FLT_EPSILON);
		AreEqual(test[14u], 14.f, FLT_EPSILON);
		AreEqual(test[15u], 15.f, FLT_EPSILON);

		test = mat2.elem();
		AreEqual(test[0u], 0.f, FLT_EPSILON);
		AreEqual(test[1u], 0.f, FLT_EPSILON);
		AreEqual(test[2u], 0.f, FLT_EPSILON);
		AreEqual(test[3u], 0.f, FLT_EPSILON);
		AreEqual(test[4u], 0.f, FLT_EPSILON);
		AreEqual(test[5u], 0.f, FLT_EPSILON);
		AreEqual(test[6u], 0.f, FLT_EPSILON);
		AreEqual(test[7u], 0.f, FLT_EPSILON);
		AreEqual(test[8u], 0.f, FLT_EPSILON);
		AreEqual(test[9u], 0.f, FLT_EPSILON);
		AreEqual(test[10u], 0.f, FLT_EPSILON);
		AreEqual(test[11u], 0.f, FLT_EPSILON);
		AreEqual(test[12u], 0.f, FLT_EPSILON);
		AreEqual(test[13u], 0.f, FLT_EPSILON);
		AreEqual(test[14u], 0.f, FLT_EPSILON);
		AreEqual(test[15u], 0.f, FLT_EPSILON);
	}

	TEST_METHOD("!=")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.f, 2.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);

		maj::math::Mat4<float> mat3(0.0f, 0.0f, 0.005f, 0.0f, 0.0f, 0.0f, 0.0f, 0.005f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.005f, 0.0f, 0.0f);
		maj::math::Mat4<float> mat4(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
		maj::math::Mat4<float> mat5(0.005f, 0.0f, 0.005f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

		IsTrue(mat != mat3);
		IsTrue(mat1 != mat4);
		IsTrue(mat2 != mat5);
	}

	TEST_METHOD("isOrtho")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.f, 2.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);
		maj::math::Mat4<float> mat3(0.f, 0.f, 0.f, 1.f,
									0.f, 0.f, 1.f, 0.f,
									1.f, 0.f, 0.f, 0.f,
									0.f, 1.f, 0.f, 0.f);
		maj::math::Mat4<float> mat4(-0.5f, 0.5f, -0.5f, 0.5f,
									-0.5f, -0.5f, 0.5f, 0.5f,
									0.5f, -0.5f, -0.5f, 0.5f,
									-0.5f, -0.5f, -0.5f, -0.5f);


		IsTrue(mat.isOrtho());
		IsFalse(mat1.isOrtho());
		IsFalse(mat2.isOrtho());
		IsTrue(mat3.isOrtho());
		IsTrue(mat4.isOrtho());
	}

	TEST_METHOD("isAffine")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.f, 2.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);
		maj::math::Mat4<float> mat3(0.f, 0.f, 0.f, 1.f,
									0.f, 0.f, 1.f, 0.f,
									1.f, 0.f, 0.f, 0.f,
									0.f, 1.f, 0.f, 0.f);
		maj::math::Mat4<float> mat4(-0.5f, 0.5f, -0.5f, 0.5f,
									-0.5f, -0.5f, 0.5f, 0.5f,
									0.5f, -0.5f, -0.5f, 0.5f,
									-0.5f, -0.5f, -0.5f, -0.5f);


		IsTrue(mat.isAffine());
		IsFalse(mat1.isAffine());
		IsTrue(mat2.isAffine());
		IsFalse(mat3.isAffine());
		IsFalse(mat4.isAffine());
	}

	TEST_METHOD("transposed")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1 { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f };
		maj::math::Mat4<float> mat2 { maj::math::Mat4<float>::skZero };
		maj::math::Mat4<float> mat3 { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f };

		glm::mat4 matt {};
		glm::mat4 matt1 { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f };
		glm::mat4 matt2 { 0.f };
		glm::mat4 matt3 { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f };

		maj::math::Mat4<float> test = mat1.transposed();
		glm::mat4 test2 = glm::transpose(matt1);

		AreEqual(test[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(test[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(test[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(test[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(test[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(test[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(test[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(test[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(test[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(test[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(test[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(test[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(test[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(test[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(test[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(test[15u], test2[3u][3u], FLT_EPSILON);

		test = mat2.transposed();
		test2 = glm::transpose(matt2);

		AreEqual(test[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(test[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(test[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(test[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(test[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(test[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(test[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(test[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(test[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(test[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(test[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(test[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(test[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(test[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(test[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(test[15u], test2[3u][3u], FLT_EPSILON);

		test = mat3.transposed();
		test2 = glm::transpose(matt3);

		AreEqual(test[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(test[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(test[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(test[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(test[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(test[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(test[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(test[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(test[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(test[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(test[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(test[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(test[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(test[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(test[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(test[15u], test2[3u][3u], FLT_EPSILON);

		test = mat.transposed();
		test2 = glm::transpose(matt);

		AreEqual(test[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(test[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(test[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(test[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(test[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(test[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(test[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(test[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(test[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(test[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(test[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(test[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(test[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(test[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(test[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(test[15u], test2[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("inversed")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1 { -0.5f, 0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f, 0.5f,
			0.5f, -0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, -0.5f, -0.5f };
		maj::math::Mat4<float> mat2 { maj::math::Mat4<float>::skZero };
		maj::math::Mat4<float> mat3 { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f };

		glm::mat4 matt {};
		glm::mat4 matt1 { -0.5f, 0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f, 0.5f,
			0.5f, -0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, -0.5f, -0.5f };
		glm::mat4 matt3(0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f);

		maj::math::Mat4<float> test { mat3.inversed() };
		glm::mat4 test2 { glm::inverse(matt3) };

		AreEqual(test[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(test[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(test[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(test[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(test[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(test[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(test[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(test[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(test[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(test[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(test[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(test[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(test[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(test[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(test[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(test[15u], test2[3u][3u], FLT_EPSILON);

		test = mat.inversed();
		test2 = glm::inverse(matt);

		AreEqual(test[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(test[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(test[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(test[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(test[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(test[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(test[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(test[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(test[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(test[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(test[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(test[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(test[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(test[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(test[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(test[15u], test2[3u][3u], FLT_EPSILON);

		test = mat1.inversed();
		test2 = glm::inverse(matt1);

		AreEqual(test[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(test[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(test[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(test[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(test[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(test[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(test[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(test[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(test[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(test[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(test[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(test[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(test[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(test[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(test[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(test[15u], test2[3u][3u], FLT_EPSILON);

		test = mat2.inversed();

		AreEqual(test[0u], 1.f, FLT_EPSILON);
		AreEqual(test[1u], 0.f, FLT_EPSILON);
		AreEqual(test[2u], 0.f, FLT_EPSILON);
		AreEqual(test[3u], 0.f, FLT_EPSILON);
		AreEqual(test[4u], 0.f, FLT_EPSILON);
		AreEqual(test[5u], 1.f, FLT_EPSILON);
		AreEqual(test[6u], 0.f, FLT_EPSILON);
		AreEqual(test[7u], 0.f, FLT_EPSILON);
		AreEqual(test[8u], 0.f, FLT_EPSILON);
		AreEqual(test[9u], 0.f, FLT_EPSILON);
		AreEqual(test[10u], 1.f, FLT_EPSILON);
		AreEqual(test[11u], 0.f, FLT_EPSILON);
		AreEqual(test[12u], 0.f, FLT_EPSILON);
		AreEqual(test[13u], 0.f, FLT_EPSILON);
		AreEqual(test[14u], 0.f, FLT_EPSILON);
		AreEqual(test[15u], 1.f, FLT_EPSILON);
	}

	TEST_METHOD("transpose")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1 { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f };
		maj::math::Mat4<float> mat2 { maj::math::Mat4<float>::skZero };
		maj::math::Mat4<float> mat3 { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f };

		glm::mat4 matt {};
		glm::mat4 matt1 { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f };
		glm::mat4 matt2 { 0.f };
		glm::mat4 matt3 { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f };

		mat1.transpose();
		glm::mat4 test2 { glm::transpose(matt1) };

		AreEqual(mat1[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(mat1[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(mat1[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(mat1[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(mat1[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(mat1[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(mat1[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(mat1[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(mat1[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(mat1[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(mat1[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(mat1[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(mat1[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(mat1[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(mat1[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(mat1[15u], test2[3u][3u], FLT_EPSILON);

		mat2.transpose();
		test2 = glm::transpose(matt2);

		AreEqual(mat2[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(mat2[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(mat2[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(mat2[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(mat2[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(mat2[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(mat2[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(mat2[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(mat2[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(mat2[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(mat2[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(mat2[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(mat2[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(mat2[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(mat2[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(mat2[15u], test2[3u][3u], FLT_EPSILON);

		mat3.transpose();
		test2 = glm::transpose(matt3);

		AreEqual(mat3[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(mat3[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(mat3[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(mat3[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(mat3[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(mat3[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(mat3[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(mat3[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(mat3[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(mat3[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(mat3[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(mat3[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(mat3[15u], test2[3u][3u], FLT_EPSILON);

		mat.transpose();
		test2 = glm::transpose(matt);

		AreEqual(mat[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], test2[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("inverse")
	{
		maj::math::Mat4<float> mat1 { -0.5f, 0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f, 0.5f,
			0.5f, -0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, -0.5f, -0.5f };
		maj::math::Mat4<float> mat2 {};
		maj::math::Mat4<float> mat3 { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f };

		glm::mat4 matt1 { -0.5f, 0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f, 0.5f,
			0.5f, -0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, -0.5f, -0.5f };
		glm::mat4 matt2 { 1.f };
		glm::mat4 matt3(0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f);

		mat1.inverse();
		glm::mat4 test2 { glm::inverse(matt1) };

		AreEqual(mat1[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(mat1[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(mat1[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(mat1[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(mat1[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(mat1[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(mat1[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(mat1[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(mat1[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(mat1[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(mat1[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(mat1[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(mat1[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(mat1[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(mat1[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(mat1[15u], test2[3u][3u], FLT_EPSILON);

		mat2.inverse();
		test2 = glm::inverse(matt2);

		AreEqual(mat2[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(mat2[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(mat2[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(mat2[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(mat2[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(mat2[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(mat2[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(mat2[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(mat2[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(mat2[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(mat2[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(mat2[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(mat2[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(mat2[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(mat2[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(mat2[15u], test2[3u][3u], FLT_EPSILON);

		mat3.inverse();
		test2 = glm::inverse(matt3);

		AreEqual(mat3[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], test2[0u][3u], FLT_EPSILON);
		AreEqual(mat3[4u], test2[1u][0u], FLT_EPSILON);
		AreEqual(mat3[5u], test2[1u][1u], FLT_EPSILON);
		AreEqual(mat3[6u], test2[1u][2u], FLT_EPSILON);
		AreEqual(mat3[7u], test2[1u][3u], FLT_EPSILON);
		AreEqual(mat3[8u], test2[2u][0u], FLT_EPSILON);
		AreEqual(mat3[9u], test2[2u][1u], FLT_EPSILON);
		AreEqual(mat3[10u], test2[2u][2u], FLT_EPSILON);
		AreEqual(mat3[11u], test2[2u][3u], FLT_EPSILON);
		AreEqual(mat3[12u], test2[3u][0u], FLT_EPSILON);
		AreEqual(mat3[13u], test2[3u][1u], FLT_EPSILON);
		AreEqual(mat3[14u], test2[3u][2u], FLT_EPSILON);
		AreEqual(mat3[15u], test2[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("applyTranslation")
	{
		maj::math::Mat4<float> mat0;
		maj::math::Mat4<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);
		maj::math::Mat4<float> mat3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f, 19.045f, 10.1f, 11.2f, 12.3f, 13.124f, 14.256f, 15.249f);

		maj::math::Vec3<float> v1(10.0f, 11.0f, 12.0f);
		maj::math::Vec3<float> v2(10.4f, 11.25f, 12.45f);
		maj::math::Vec3<float> v3(0.f, 1.f, 0.f);

		glm::mat4 matt0 {};
		glm::mat4 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		glm::mat4 matt2 { 0.f };
		glm::mat4 matt3(0.f, 0.5f, 0.25f, 0.55f,
						5.25f, 45.25f, 6.95f, 17.8f,
						28.5f, 19.045f, 10.1f, 11.2f,
						12.3f, 13.124f, 14.256f, 15.249f);

		glm::vec3 u1 { 10.0f, 11.0f, 12.0f };
		glm::vec3 u2 { 10.4f, 11.25f, 12.45f };
		glm::vec3 u3 { 0.f, 1.f, 0.f };

		mat0.applyTranslation(v1);
		mat1.applyTranslation(v1);
		mat2.applyTranslation(v2);
		mat3.applyTranslation(v3);

		glm::mat4 gmat { glm::translate(matt0, u1) };

		AreEqual(mat0[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat0[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat0[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat0[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat0[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat0[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat0[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat0[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat0[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat0[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat0[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat0[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat0[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat0[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat0[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat0[15u], gmat[3u][3u], FLT_EPSILON);

		gmat = glm::translate(matt1, u1);

		AreEqual(mat1[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat1[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat1[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat1[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat1[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat1[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat1[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat1[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat1[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat1[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat1[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat1[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat1[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat1[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat1[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat1[15u], gmat[3u][3u], FLT_EPSILON);

		gmat = glm::translate(matt2, u2);

		AreEqual(mat2[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat2[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat2[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat2[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat2[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat2[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat2[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat2[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat2[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat2[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat2[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat2[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat2[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat2[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat2[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat2[15u], gmat[3u][3u], FLT_EPSILON);

		gmat = glm::translate(matt3, u3);

		AreEqual(mat3[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat3[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat3[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat3[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat3[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat3[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat3[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat3[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat3[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat3[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat3[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat3[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat3[15u], gmat[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("applyRotation")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);
		maj::math::Mat4<float> mat3(0.f, 0.5f, 0.25f, 0.55f,
									5.25f, 45.25f, 6.95f, 17.8f,
									28.5f, 19.045f, 10.1f, 11.2f,
									12.3f, 13.124f, 14.256f, 15.249f);

		glm::mat4 matt(1);
		glm::mat4 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		glm::mat4 matt2 {};
		glm::mat4 matt3(0.f, 0.5f, 0.25f, 0.55f,
						5.25f, 45.25f, 6.95f, 17.8f,
						28.5f, 19.045f, 10.1f, 11.2f,
						12.3f, 13.124f, 14.256f, 15.249f);

		maj::math::Quaternion<float> quat;
		maj::math::Quaternion<float> quat2(10.0f, 1.0, 5.0f, 1.0f);

		glm::quat quatG(1.0f, 0.0f, 0.0f, 0.0f);
		quatG = glm::normalize(quatG);
		glm::quat quatG2(10.0f, 1.0, 5.0f, 1.0f);
		quatG2 = glm::normalize(quatG2);

		mat.applyRotation(quat);
		glm::mat4 resGlm { glm::rotate(matt, glm::angle(quatG), glm::axis(quatG)) };

		AreEqual(mat[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], resGlm[3u][3u], FLT_EPSILON);

		mat3.applyRotation(quat2);
		resGlm = glm::rotate(matt3, glm::angle(quatG2), glm::axis(quatG2));

		IsTrue(maj::math::Helper<float>::areSame(mat3[0u], resGlm[0u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[1u], resGlm[0u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[2u], resGlm[0u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[3u], resGlm[0u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[4u], resGlm[1u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[5u], resGlm[1u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[6u], resGlm[1u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[7u], resGlm[1u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[8u], resGlm[2u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[9u], resGlm[2u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[10u], resGlm[2u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[11u], resGlm[2u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[12u], resGlm[3u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[13u], resGlm[3u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[14u], resGlm[3u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat3[15u], resGlm[3u][3u]));
	}

	TEST_METHOD("applyScale")
	{
		maj::math::Mat4<float> mat0;
		maj::math::Mat4<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);
		maj::math::Mat4<float> mat3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f, 19.045f, 10.1f, 11.2f, 12.3f, 13.124f, 14.256f, 15.249f);

		maj::math::Vec3<float> v1(10.0f, 11.0f, 12.0f);
		maj::math::Vec3<float> v2(10.4f, 11.25f, 12.45f);
		maj::math::Vec3<float> v3(0.f, 1.f, 0.f);

		glm::mat4 matt0 {};
		glm::mat4 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		glm::mat4 matt2 { 0.f };
		glm::mat4 matt3(0.f, 0.5f, 0.25f, 0.55f,
						5.25f, 45.25f, 6.95f, 17.8f,
						28.5f, 19.045f, 10.1f, 11.2f,
						12.3f, 13.124f, 14.256f, 15.249f);

		glm::vec3 u1 { 10.0f, 11.0f, 12.0f };
		glm::vec3 u2 { 10.4f, 11.25f, 12.45f };
		glm::vec3 u3 { 0.f, 1.f, 0.f };

		mat0.applyScale(v1);
		mat1.applyScale(v1);
		mat2.applyScale(v2);
		mat3.applyScale(v3);

		glm::mat4 gmat { glm::scale(matt0, u1) };

		AreEqual(mat0[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat0[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat0[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat0[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat0[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat0[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat0[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat0[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat0[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat0[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat0[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat0[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat0[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat0[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat0[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat0[15u], gmat[3u][3u], FLT_EPSILON);

		gmat = glm::scale(matt1, u1);

		AreEqual(mat1[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat1[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat1[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat1[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat1[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat1[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat1[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat1[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat1[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat1[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat1[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat1[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat1[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat1[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat1[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat1[15u], gmat[3u][3u], FLT_EPSILON);

		gmat = glm::scale(matt2, u2);

		AreEqual(mat2[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat2[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat2[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat2[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat2[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat2[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat2[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat2[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat2[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat2[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat2[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat2[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat2[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat2[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat2[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat2[15u], gmat[3u][3u], FLT_EPSILON);

		gmat = glm::scale(matt3, u3);

		AreEqual(mat3[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat3[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat3[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat3[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat3[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat3[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat3[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat3[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat3[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat3[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat3[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat3[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat3[15u], gmat[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("* Mat4")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
		maj::math::Mat4<float> mat2(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f, 19.045f, 10.1f, 11.2f, 12.3f, 13.124f, 14.256f, 15.249f);
		maj::math::Mat4<float> mat3(maj::math::Mat4<float>::skZero);

		glm::mat4 matt;
		glm::mat4 matt1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
		glm::mat4 matt2(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f, 19.045f, 10.1f, 11.2f, 12.3f, 13.124f, 14.256f, 15.249f);
		glm::mat4 matt3(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);

		maj::math::Mat4<float> res = mat * mat1;
		glm::mat4 resGlm = matt * matt1;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);

		res = mat2 * mat1;
		resGlm = matt2 * matt1;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);

		res = mat3 * mat1;
		resGlm = matt3 * matt1;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("* Vec3")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1 { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f };
		maj::math::Mat4<float> mat2 { 0.f, 0.5f, 0.25f, 0.55f,
			5.25f, 45.25f, 6.95f, 17.8f,
			28.5f, 19.045f, 10.1f, 11.2f,
			12.3f, 13.124f, 14.256f, 15.249f };
		maj::math::Mat4<float> mat3 { maj::math::Mat4<float>::skZero };

		maj::math::Vec3<float> vec0;
		maj::math::Vec3<float> vec1 { 10.0f, 11.0f, 12.0f };
		maj::math::Vec3<float> vec2 { 10.4f, 1.2514f, 2.4545f };

		glm::mat4 matt;
		glm::mat4 matt1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
		glm::mat4 matt2(0.f, 0.5f, 0.25f, 0.55f,
						5.25f, 45.25f, 6.95f, 17.8f,
						28.5f, 19.045f, 10.1f, 11.2f,
						12.3f, 13.124f, 14.256f, 15.249f);
		glm::mat4 matt3(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);

		glm::vec4 vec00;
		glm::vec4 vec01 { 10.0f, 11.0f, 12.0f, 1.0f };
		glm::vec4 vec02 { 10.4f, 1.2514f, 2.4545f, 1.f };

		maj::math::Vec3<float> res { mat * vec0 };
		glm::vec3 resGlm { matt * vec00 };

		IsTrue(maj::math::Helper<float>::areSame(res[0u], resGlm[0u]));
		IsTrue(maj::math::Helper<float>::areSame(res[1u], resGlm[1u]));
		IsTrue(maj::math::Helper<float>::areSame(res[2u], resGlm[2u]));

		res = mat1 * vec1;
		resGlm = matt1 * vec01;

		IsTrue(maj::math::Helper<float>::areSame(res[0u], resGlm[0u]));
		IsTrue(maj::math::Helper<float>::areSame(res[1u], resGlm[1u]));
		IsTrue(maj::math::Helper<float>::areSame(res[2u], resGlm[2u]));

		res = mat2 * vec2;
		resGlm = matt2 * vec02;

		IsTrue(maj::math::Helper<float>::areSame(res[0u], resGlm[0u]));
		IsTrue(maj::math::Helper<float>::areSame(res[1u], resGlm[1u]));
		IsTrue(maj::math::Helper<float>::areSame(res[2u], resGlm[2u]));
	}

	TEST_METHOD("* Vec4")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1 { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f };
		maj::math::Mat4<float> mat2 { 0.f, 0.5f, 0.25f, 0.55f,
			5.25f, 45.25f, 6.95f, 17.8f,
			28.5f, 19.045f, 10.1f, 11.2f,
			12.3f, 13.124f, 14.256f, 15.249f };
		maj::math::Mat4<float> mat3 { maj::math::Mat4<float>::skZero };

		maj::math::Vec4<float> v0;
		maj::math::Vec4<float> v1 { 10.0f, 0.0f, 12.0f, 1.0f };
		maj::math::Vec4<float> v2 { 10.4f, 1.2514f, 2.4545f, 1.f };
		maj::math::Vec4<float> v3 { 1.2568f, 4.258f, 0.f, 0.f };

		glm::mat4 matt;
		glm::mat4 matt1 { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f };
		glm::mat4 matt2 { 0.f, 0.5f, 0.25f, 0.55f,
			5.25f, 45.25f, 6.95f, 17.8f,
			28.5f, 19.045f, 10.1f, 11.2f,
			12.3f, 13.124f, 14.256f, 15.249f };
		glm::mat4 matt3 { 0.f };

		glm::vec4 u0;
		glm::vec4 u1 { 10.0f, 0.0f, 12.0f, 1.0f };
		glm::vec4 u2 { 10.4f, 1.2514f, 2.4545f, 1.f };
		glm::vec4 u3 { 1.2568, 4.258f, 0.f, 0.f };

		maj::math::Vec4<float> res { mat * v0 };
		glm::vec4 resGlm { matt * u0 };

		IsTrue(maj::math::Helper<float>::areSame(res[0u], resGlm[0u]));
		IsTrue(maj::math::Helper<float>::areSame(res[1u], resGlm[1u]));
		IsTrue(maj::math::Helper<float>::areSame(res[2u], resGlm[2u]));

		res = mat * v1;
		resGlm = matt * u1;

		IsTrue(maj::math::Helper<float>::areSame(res[0u], resGlm[0u]));
		IsTrue(maj::math::Helper<float>::areSame(res[1u], resGlm[1u]));
		IsTrue(maj::math::Helper<float>::areSame(res[2u], resGlm[2u]));

		res = mat1 * v1;
		resGlm = matt1 * u1;

		IsTrue(maj::math::Helper<float>::areSame(res[0u], resGlm[0u]));
		IsTrue(maj::math::Helper<float>::areSame(res[1u], resGlm[1u]));
		IsTrue(maj::math::Helper<float>::areSame(res[2u], resGlm[2u]));

		res = mat2 * v2;
		resGlm = matt2 * u2;

		IsTrue(maj::math::Helper<float>::areSame(res[0u], resGlm[0u]));
		IsTrue(maj::math::Helper<float>::areSame(res[1u], resGlm[1u]));
		IsTrue(maj::math::Helper<float>::areSame(res[2u], resGlm[2u]));

		res = mat2 * v3;
		resGlm = matt2 * u3;

		IsTrue(maj::math::Helper<float>::areSame(res[0u], resGlm[0u]));
		IsTrue(maj::math::Helper<float>::areSame(res[1u], resGlm[1u]));
		IsTrue(maj::math::Helper<float>::areSame(res[2u], resGlm[2u]));

		res = mat3 * v2;
		resGlm = matt3 * u2;

		IsTrue(maj::math::Helper<float>::areSame(res[0u], resGlm[0u]));
		IsTrue(maj::math::Helper<float>::areSame(res[1u], resGlm[1u]));
		IsTrue(maj::math::Helper<float>::areSame(res[2u], resGlm[2u]));
	}

	TEST_METHOD("* float")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);
		maj::math::Mat4<float> mat3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f, 19.045f, 10.1f, 11.2f, 12.3f, 13.124f, 14.256f, 15.249f);

		glm::mat4 matt(1);
		glm::mat4 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		glm::mat4 matt2 {};
		glm::mat4 matt3(0.f, 0.5f, 0.25f, 0.55f,
						5.25f, 45.25f, 6.95f, 17.8f,
						28.5f, 19.045f, 10.1f, 11.2f,
						12.3f, 13.124f, 14.256f, 15.249f);

		maj::math::Mat4<float> res = mat * 10.0f;
		glm::mat4 resGlm = matt * 10.0f;

		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);

		res = mat1 * 10.0f;
		resGlm = matt1 * 10.0f;

		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);

		res = mat3 * 10.0f;
		resGlm = matt3 * 10.0f;

		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("/ float")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);
		maj::math::Mat4<float> mat3 { 0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f, 19.045f, 10.1f, 11.2f, 12.3f, 13.124f, 14.256f, 15.249f };

		glm::mat4 matt(1);
		glm::mat4 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		glm::mat4 matt2 {};
		glm::mat4 matt3 { 0.f, 0.5f, 0.25f, 0.55f,
			5.25f, 45.25f, 6.95f, 17.8f,
			28.5f, 19.045f, 10.1f, 11.2f,
			12.3f, 13.124f, 14.256f, 15.249f };

		maj::math::Mat4<float> res = mat / 10.0f;
		glm::mat4 resGlm = matt / 10.0f;

		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);

		res = mat1 / 10.45f;
		resGlm = matt1 / 10.45f;

		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);

		res = mat3 / 0.0f;

		AreEqual(res[0u], 1.0f, FLT_EPSILON);
		AreEqual(res[1u], 0.0f, FLT_EPSILON);
		AreEqual(res[2u], 0.0f, FLT_EPSILON);
		AreEqual(res[3u], 0.0f, FLT_EPSILON);
		AreEqual(res[4u], 0.0f, FLT_EPSILON);
		AreEqual(res[5u], 1.0f, FLT_EPSILON);
		AreEqual(res[6u], 0.0f, FLT_EPSILON);
		AreEqual(res[7u], 0.0f, FLT_EPSILON);
		AreEqual(res[8u], 0.0f, FLT_EPSILON);
		AreEqual(res[9u], 0.0f, FLT_EPSILON);
		AreEqual(res[10u], 1.0f, FLT_EPSILON);
		AreEqual(res[11u], 0.0f, FLT_EPSILON);
		AreEqual(res[12u], 0.0f, FLT_EPSILON);
		AreEqual(res[13u], 0.0f, FLT_EPSILON);
		AreEqual(res[14u], 0.0f, FLT_EPSILON);
		AreEqual(res[15u], 1.0f, FLT_EPSILON);
	}

	TEST_METHOD("-")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);
		maj::math::Mat4<float> mat3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f, 19.045f, 10.1f, 11.2f, 12.3f, 13.124f, 14.256f, 15.249f);

		glm::mat4 matt(1);
		glm::mat4 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		glm::mat4 matt2 {};
		glm::mat4 matt3(0.f, 0.5f, 0.25f, 0.55f,
						5.25f, 45.25f, 6.95f, 17.8f,
						28.5f, 19.045f, 10.1f, 11.2f,
						12.3f, 13.124f, 14.256f, 15.249f);

		maj::math::Mat4<float> res = -mat;
		glm::mat4 resGlm = -matt;

		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);

		res = -mat1;
		resGlm = -matt1;

		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);

		res = -mat3;
		resGlm = -matt3;

		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("=")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);
		maj::math::Mat4<float> mat3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f, 19.045f, 10.1f, 11.2f, 12.3f, 13.124f, 14.256f, 15.249f);

		glm::mat4 matt(1);
		glm::mat4 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		glm::mat4 matt2 {};
		glm::mat4 matt3(0.f, 0.5f, 0.25f, 0.55f,
						5.25f, 45.25f, 6.95f, 17.8f,
						28.5f, 19.045f, 10.1f, 11.2f,
						12.3f, 13.124f, 14.256f, 15.249f);

		maj::math::Mat4<float> res = mat;
		glm::mat4 resGlm = matt;

		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);

		res = mat1;
		resGlm = matt1;

		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);

		res = mat3;
		resGlm = matt3;

		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[0u][3u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[1u][3u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[9u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[10u], resGlm[2u][2u], FLT_EPSILON);
		AreEqual(res[11u], resGlm[2u][3u], FLT_EPSILON);
		AreEqual(res[12u], resGlm[3u][0u], FLT_EPSILON);
		AreEqual(res[13u], resGlm[3u][1u], FLT_EPSILON);
		AreEqual(res[14u], resGlm[3u][2u], FLT_EPSILON);
		AreEqual(res[15u], resGlm[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("*= float")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);
		maj::math::Mat4<float> mat3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f, 19.045f, 10.1f, 11.2f, 12.3f, 13.124f, 14.256f, 15.249f);

		glm::mat4 matt(1);
		glm::mat4 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		glm::mat4 matt2 {};
		glm::mat4 matt3(0.f, 0.5f, 0.25f, 0.55f,
						5.25f, 45.25f, 6.95f, 17.8f,
						28.5f, 19.045f, 10.1f, 11.2f,
						12.3f, 13.124f, 14.256f, 15.249f);

		mat *= 10.0f;
		matt *= 10.0f;

		AreEqual(mat[0u], matt[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], matt[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], matt[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], matt[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], matt[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], matt[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], matt[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], matt[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], matt[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], matt[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], matt[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], matt[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], matt[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], matt[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], matt[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], matt[3u][3u], FLT_EPSILON);

		mat1 *= 10.25f;
		matt1 *= 10.25f;

		AreEqual(mat1[0u], matt1[0u][0u], FLT_EPSILON);
		AreEqual(mat1[1u], matt1[0u][1u], FLT_EPSILON);
		AreEqual(mat1[2u], matt1[0u][2u], FLT_EPSILON);
		AreEqual(mat1[3u], matt1[0u][3u], FLT_EPSILON);
		AreEqual(mat1[4u], matt1[1u][0u], FLT_EPSILON);
		AreEqual(mat1[5u], matt1[1u][1u], FLT_EPSILON);
		AreEqual(mat1[6u], matt1[1u][2u], FLT_EPSILON);
		AreEqual(mat1[7u], matt1[1u][3u], FLT_EPSILON);
		AreEqual(mat1[8u], matt1[2u][0u], FLT_EPSILON);
		AreEqual(mat1[9u], matt1[2u][1u], FLT_EPSILON);
		AreEqual(mat1[10u], matt1[2u][2u], FLT_EPSILON);
		AreEqual(mat1[11u], matt1[2u][3u], FLT_EPSILON);
		AreEqual(mat1[12u], matt1[3u][0u], FLT_EPSILON);
		AreEqual(mat1[13u], matt1[3u][1u], FLT_EPSILON);
		AreEqual(mat1[14u], matt1[3u][2u], FLT_EPSILON);
		AreEqual(mat1[15u], matt1[3u][3u], FLT_EPSILON);

		mat3 *= PI;
		matt3 *= PI;

		AreEqual(mat3[0u], matt3[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], matt3[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], matt3[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], matt3[0u][3u], FLT_EPSILON);
		AreEqual(mat3[4u], matt3[1u][0u], FLT_EPSILON);
		AreEqual(mat3[5u], matt3[1u][1u], FLT_EPSILON);
		AreEqual(mat3[6u], matt3[1u][2u], FLT_EPSILON);
		AreEqual(mat3[7u], matt3[1u][3u], FLT_EPSILON);
		AreEqual(mat3[8u], matt3[2u][0u], FLT_EPSILON);
		AreEqual(mat3[9u], matt3[2u][1u], FLT_EPSILON);
		AreEqual(mat3[10u], matt3[2u][2u], FLT_EPSILON);
		AreEqual(mat3[11u], matt3[2u][3u], FLT_EPSILON);
		AreEqual(mat3[12u], matt3[3u][0u], FLT_EPSILON);
		AreEqual(mat3[13u], matt3[3u][1u], FLT_EPSILON);
		AreEqual(mat3[14u], matt3[3u][2u], FLT_EPSILON);
		AreEqual(mat3[15u], matt3[3u][3u], FLT_EPSILON);

		mat3 *= 0.0f;
		matt3 *= 0.0f;

		AreEqual(mat3[0u], matt3[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], matt3[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], matt3[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], matt3[0u][3u], FLT_EPSILON);
		AreEqual(mat3[4u], matt3[1u][0u], FLT_EPSILON);
		AreEqual(mat3[5u], matt3[1u][1u], FLT_EPSILON);
		AreEqual(mat3[6u], matt3[1u][2u], FLT_EPSILON);
		AreEqual(mat3[7u], matt3[1u][3u], FLT_EPSILON);
		AreEqual(mat3[8u], matt3[2u][0u], FLT_EPSILON);
		AreEqual(mat3[9u], matt3[2u][1u], FLT_EPSILON);
		AreEqual(mat3[10u], matt3[2u][2u], FLT_EPSILON);
		AreEqual(mat3[11u], matt3[2u][3u], FLT_EPSILON);
		AreEqual(mat3[12u], matt3[3u][0u], FLT_EPSILON);
		AreEqual(mat3[13u], matt3[3u][1u], FLT_EPSILON);
		AreEqual(mat3[14u], matt3[3u][2u], FLT_EPSILON);
		AreEqual(mat3[15u], matt3[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("/= float")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		maj::math::Mat4<float> mat2(maj::math::Mat4<float>::skZero);
		maj::math::Mat4<float> mat3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f, 19.045f, 10.1f, 11.2f, 12.3f, 13.124f, 14.256f, 15.249f);

		glm::mat4 matt(1);
		glm::mat4 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f);
		glm::mat4 matt2 {};
		glm::mat4 matt3(0.f, 0.5f, 0.25f, 0.55f,
						5.25f, 45.25f, 6.95f, 17.8f,
						28.5f, 19.045f, 10.1f, 11.2f,
						12.3f, 13.124f, 14.256f, 15.249f);

		mat /= 10.0f;
		matt /= 10.0f;

		AreEqual(mat[0u], matt[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], matt[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], matt[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], matt[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], matt[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], matt[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], matt[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], matt[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], matt[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], matt[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], matt[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], matt[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], matt[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], matt[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], matt[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], matt[3u][3u], FLT_EPSILON);

		mat1 /= 10.45f;
		matt1 /= 10.45f;

		AreEqual(mat1[0u], matt1[0u][0u], FLT_EPSILON);
		AreEqual(mat1[1u], matt1[0u][1u], FLT_EPSILON);
		AreEqual(mat1[2u], matt1[0u][2u], FLT_EPSILON);
		AreEqual(mat1[3u], matt1[0u][3u], FLT_EPSILON);
		AreEqual(mat1[4u], matt1[1u][0u], FLT_EPSILON);
		AreEqual(mat1[5u], matt1[1u][1u], FLT_EPSILON);
		AreEqual(mat1[6u], matt1[1u][2u], FLT_EPSILON);
		AreEqual(mat1[7u], matt1[1u][3u], FLT_EPSILON);
		AreEqual(mat1[8u], matt1[2u][0u], FLT_EPSILON);
		AreEqual(mat1[9u], matt1[2u][1u], FLT_EPSILON);
		AreEqual(mat1[10u], matt1[2u][2u], FLT_EPSILON);
		AreEqual(mat1[11u], matt1[2u][3u], FLT_EPSILON);
		AreEqual(mat1[12u], matt1[3u][0u], FLT_EPSILON);
		AreEqual(mat1[13u], matt1[3u][1u], FLT_EPSILON);
		AreEqual(mat1[14u], matt1[3u][2u], FLT_EPSILON);
		AreEqual(mat1[15u], matt1[3u][3u], FLT_EPSILON);

		mat3 /= PI;
		matt3 /= PI;

		AreEqual(mat3[0u], matt3[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], matt3[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], matt3[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], matt3[0u][3u], FLT_EPSILON);
		AreEqual(mat3[4u], matt3[1u][0u], FLT_EPSILON);
		AreEqual(mat3[5u], matt3[1u][1u], FLT_EPSILON);
		AreEqual(mat3[6u], matt3[1u][2u], FLT_EPSILON);
		AreEqual(mat3[7u], matt3[1u][3u], FLT_EPSILON);
		AreEqual(mat3[8u], matt3[2u][0u], FLT_EPSILON);
		AreEqual(mat3[9u], matt3[2u][1u], FLT_EPSILON);
		AreEqual(mat3[10u], matt3[2u][2u], FLT_EPSILON);
		AreEqual(mat3[11u], matt3[2u][3u], FLT_EPSILON);
		AreEqual(mat3[12u], matt3[3u][0u], FLT_EPSILON);
		AreEqual(mat3[13u], matt3[3u][1u], FLT_EPSILON);
		AreEqual(mat3[14u], matt3[3u][2u], FLT_EPSILON);
		AreEqual(mat3[15u], matt3[3u][3u], FLT_EPSILON);


		maj::math::Mat4<float> test { mat3 };
		mat3 /= 0.0f;

		IsTrue(mat3 == test);

	}

	TEST_METHOD("*= Mat4")
	{
		maj::math::Mat4<float> mat;
		maj::math::Mat4<float> mat1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
		maj::math::Mat4<float> mat2(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f, 19.045f, 10.1f, 11.2f, 12.3f, 13.124f, 14.256f, 15.249f);
		maj::math::Mat4<float> mat3(maj::math::Mat4<float>::skZero);

		glm::mat4 matt;
		glm::mat4 matt1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f);
		glm::mat4 matt2(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f, 19.045f, 10.1f, 11.2f, 12.3f, 13.124f, 14.256f, 15.249f);
		glm::mat4 matt3(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);

		mat *= mat1;
		matt *= matt1;

		AreEqual(mat[0u], matt[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], matt[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], matt[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], matt[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], matt[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], matt[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], matt[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], matt[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], matt[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], matt[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], matt[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], matt[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], matt[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], matt[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], matt[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], matt[3u][3u], FLT_EPSILON);

		mat2 *= mat1;
		matt2 *= matt1;

		AreEqual(mat2[0u], matt2[0u][0u], FLT_EPSILON);
		AreEqual(mat2[1u], matt2[0u][1u], FLT_EPSILON);
		AreEqual(mat2[2u], matt2[0u][2u], FLT_EPSILON);
		AreEqual(mat2[3u], matt2[0u][3u], FLT_EPSILON);
		AreEqual(mat2[4u], matt2[1u][0u], FLT_EPSILON);
		AreEqual(mat2[5u], matt2[1u][1u], FLT_EPSILON);
		AreEqual(mat2[6u], matt2[1u][2u], FLT_EPSILON);
		AreEqual(mat2[7u], matt2[1u][3u], FLT_EPSILON);
		AreEqual(mat2[8u], matt2[2u][0u], FLT_EPSILON);
		AreEqual(mat2[9u], matt2[2u][1u], FLT_EPSILON);
		AreEqual(mat2[10u], matt2[2u][2u], FLT_EPSILON);
		AreEqual(mat2[11u], matt2[2u][3u], FLT_EPSILON);
		AreEqual(mat2[12u], matt2[3u][0u], FLT_EPSILON);
		AreEqual(mat2[13u], matt2[3u][1u], FLT_EPSILON);
		AreEqual(mat2[14u], matt2[3u][2u], FLT_EPSILON);
		AreEqual(mat2[15u], matt2[3u][3u], FLT_EPSILON);

		mat3 *= mat1;
		matt3 *= matt1;

		AreEqual(mat3[0u], matt3[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], matt3[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], matt3[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], matt3[0u][3u], FLT_EPSILON);
		AreEqual(mat3[4u], matt3[1u][0u], FLT_EPSILON);
		AreEqual(mat3[5u], matt3[1u][1u], FLT_EPSILON);
		AreEqual(mat3[6u], matt3[1u][2u], FLT_EPSILON);
		AreEqual(mat3[7u], matt3[1u][3u], FLT_EPSILON);
		AreEqual(mat3[8u], matt3[2u][0u], FLT_EPSILON);
		AreEqual(mat3[9u], matt3[2u][1u], FLT_EPSILON);
		AreEqual(mat3[10u], matt3[2u][2u], FLT_EPSILON);
		AreEqual(mat3[11u], matt3[2u][3u], FLT_EPSILON);
		AreEqual(mat3[12u], matt3[3u][0u], FLT_EPSILON);
		AreEqual(mat3[13u], matt3[3u][1u], FLT_EPSILON);
		AreEqual(mat3[14u], matt3[3u][2u], FLT_EPSILON);
		AreEqual(mat3[15u], matt3[3u][3u], FLT_EPSILON);

		mat2 *= mat2;
		matt2 *= matt2;

		AreEqual(mat2[0u], matt2[0u][0u], FLT_EPSILON);
		AreEqual(mat2[1u], matt2[0u][1u], FLT_EPSILON);
		AreEqual(mat2[2u], matt2[0u][2u], FLT_EPSILON);
		AreEqual(mat2[3u], matt2[0u][3u], FLT_EPSILON);
		AreEqual(mat2[4u], matt2[1u][0u], FLT_EPSILON);
		AreEqual(mat2[5u], matt2[1u][1u], FLT_EPSILON);
		AreEqual(mat2[6u], matt2[1u][2u], FLT_EPSILON);
		AreEqual(mat2[7u], matt2[1u][3u], FLT_EPSILON);
		AreEqual(mat2[8u], matt2[2u][0u], FLT_EPSILON);
		AreEqual(mat2[9u], matt2[2u][1u], FLT_EPSILON);
		AreEqual(mat2[10u], matt2[2u][2u], FLT_EPSILON);
		AreEqual(mat2[11u], matt2[2u][3u], FLT_EPSILON);
		AreEqual(mat2[12u], matt2[3u][0u], FLT_EPSILON);
		AreEqual(mat2[13u], matt2[3u][1u], FLT_EPSILON);
		AreEqual(mat2[14u], matt2[3u][2u], FLT_EPSILON);
		AreEqual(mat2[15u], matt2[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("makeTranslation")
	{
		maj::math::Mat4<float> mat { maj::math::Mat4<float>::makeTranslation({}) };
		glm::mat4 gmat { glm::translate(glm::vec3 {}) };

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeTranslation({ 1.f, 1.f, 1.f });
		gmat = glm::translate(glm::vec3 { 1.f, 1.f, 1.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeTranslation({ 25.684f, 45.21f, 1.25687f });
		gmat = glm::translate(glm::vec3 { 25.684f, 45.21f, 1.25687f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeTranslation({ std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), 0.f });
		gmat = glm::translate(glm::vec3 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), 0.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("makeRotationX")
	{
		maj::math::Mat4<float> mat { maj::math::Mat4<float>::makeRotationX(0.f) };
		glm::mat4 gmat { glm::rotate(0.f, glm::vec3 { 1.f, 0.f, 0.f }) };

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotationX(10.51f);
		gmat = glm::rotate(10.51f * DEG2RAD, glm::vec3 { 1.f, 0.f, 0.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotationX(90.0f);
		gmat = glm::rotate(90.0f * DEG2RAD, glm::vec3 { 1.f, 0.f, 0.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotationX(std::numeric_limits<float>::lowest());
		gmat = glm::rotate(std::numeric_limits<float>::lowest() * DEG2RAD, glm::vec3 { 1.f, 0.f, 0.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotationX(std::numeric_limits<float>::max());
		gmat = glm::rotate(std::numeric_limits<float>::max() * DEG2RAD, glm::vec3 { 1.f, 0.f, 0.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("makeRotationY")
	{
		maj::math::Mat4<float> mat { maj::math::Mat4<float>::makeRotationY(0.f) };
		glm::mat4 gmat { glm::rotate(0.f, glm::vec3 { 0.f, 1.f, 0.f }) };

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotationY(10.51f);
		gmat = glm::rotate(10.51f * DEG2RAD, glm::vec3 { 0.f, 1.f, 0.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotationY(90.0f);
		gmat = glm::rotate(90.0f * DEG2RAD, glm::vec3 { 0.f, 1.f, 0.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotationY(std::numeric_limits<float>::lowest());
		gmat = glm::rotate(std::numeric_limits<float>::lowest() * DEG2RAD, glm::vec3 { 0.f, 1.f, 0.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotationY(std::numeric_limits<float>::max());
		gmat = glm::rotate(std::numeric_limits<float>::max() * DEG2RAD, glm::vec3 { 0.f, 1.f, 0.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("makeRotationZ")
	{
		maj::math::Mat4<float> mat { maj::math::Mat4<float>::makeRotationZ(0.f) };
		glm::mat4 gmat { glm::rotate(0.f, glm::vec3 { 0.f, 0.f, 1.f }) };

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotationZ(10.51f);
		gmat = glm::rotate(10.51f * DEG2RAD, glm::vec3 { 0.f, 0.f, 1.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotationZ(90.0f);
		gmat = glm::rotate(90.0f * DEG2RAD, glm::vec3 { 0.f, 0.f, 1.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotationZ(std::numeric_limits<float>::lowest());
		gmat = glm::rotate(std::numeric_limits<float>::lowest() * DEG2RAD, glm::vec3 { 0.f, 0.f, 1.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotationZ(std::numeric_limits<float>::max());
		gmat = glm::rotate(std::numeric_limits<float>::max() * DEG2RAD, glm::vec3 { 0.f, 0.f, 1.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("makeScale")
	{
		maj::math::Mat4<float> mat { maj::math::Mat4<float>::makeScale({}) };
		glm::mat4 gmat { glm::scale(glm::vec3 {}) };

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeScale({ 1.f, 1.f, 1.f });
		gmat = glm::scale(glm::vec3 { 1.f, 1.f, 1.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeScale({ 25.684f, 45.21f, 1.25687f });
		gmat = glm::scale(glm::vec3 { 25.684f, 45.21f, 1.25687f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeScale({ std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), 0.f });
		gmat = glm::scale(glm::vec3 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), 0.f });

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("makeRotation quat")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 10.0f, 1.f, 5.0f, 1.0f };
		maj::math::Quaternion<float> q3 { 90.f, { 1.f, 0.0f, 0.0f } };
		maj::math::Quaternion<float> q4 { 0.f, std::numeric_limits<float>::lowest(), 1.f, std::numeric_limits<float>::max() };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.0f, 0.0f, 0.0f, 0.0f };
		glm::quat g2 { 10.0f, 1.0, 5.0f, 1.0f };
		glm::quat g3 { glm::angleAxis(90.f * DEG2RAD, glm::vec3 { 1.f, 0.0f, 0.0f }) };
		glm::quat g4 { 0.f, std::numeric_limits<float>::lowest(), 1.f, std::numeric_limits<float>::max() };

		maj::math::Mat4<float> mat { maj::math::Mat4<float>::makeRotation(q0) };
		glm::mat4 gmat { glm::toMat4(glm::normalize(g0)) };

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotation(q1);
		gmat = glm::toMat4(glm::normalize(g1));

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotation(q2);
		gmat = glm::toMat4(glm::normalize(g2));

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotation(q3);
		gmat = glm::toMat4(glm::normalize(g3));

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotation(q4);
		gmat = glm::toMat4(glm::normalize(g4));

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);
	}

	TEST_METHOD("makeRotation euler")
	{
		maj::math::Vec3<float> v1 { 0.f, 0.f, 0.f };
		maj::math::Vec3<float> v2 { 10.0f, 1.4782f, 5.2589f };
		maj::math::Vec3<float> v3 { 90.f, 0.f, 135.0f };
		maj::math::Vec3<float> v4 { std::numeric_limits<float>::lowest(), 0.f, std::numeric_limits<float>::max() };

		glm::vec3 u1 { 0.f, 0.f, 0.f };
		glm::vec3 u2 { 10.0f * DEG2RAD, 1.4782f * DEG2RAD, 5.2589f * DEG2RAD };
		glm::vec3 u3 { 90.f * DEG2RAD, 0.f, 135.f * DEG2RAD };
		glm::vec3 u4 { std::numeric_limits<float>::lowest() * DEG2RAD, 0.f, std::numeric_limits<float>::max() * DEG2RAD };

		maj::math::Mat4<float> mat { maj::math::Mat4<float>::makeRotation(v1) };
		glm::mat4 gmat { glm::eulerAngleXYZ(u1.x, u1.y, u1.z) };

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotation(v2);
		gmat = glm::eulerAngleXYZ(u2.x, u2.y, u2.z);

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotation(v3);
		gmat = glm::eulerAngleXYZ(u3.x, u3.y, u3.z);

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);

		mat = maj::math::Mat4<float>::makeRotation(v4);
		gmat = glm::eulerAngleXYZ(u4.x, u4.y, u4.z);

		AreEqual(mat[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], gmat[0u][3u], FLT_EPSILON);
		AreEqual(mat[4u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(mat[5u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(mat[6u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(mat[7u], gmat[1u][3u], FLT_EPSILON);
		AreEqual(mat[8u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(mat[9u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(mat[10u], gmat[2u][2u], FLT_EPSILON);
		AreEqual(mat[11u], gmat[2u][3u], FLT_EPSILON);
		AreEqual(mat[12u], gmat[3u][0u], FLT_EPSILON);
		AreEqual(mat[13u], gmat[3u][1u], FLT_EPSILON);
		AreEqual(mat[14u], gmat[3u][2u], FLT_EPSILON);
		AreEqual(mat[15u], gmat[3u][3u], FLT_EPSILON);
	}
}