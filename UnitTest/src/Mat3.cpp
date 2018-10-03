#include "Test.h"

#include <Mat3.h>

#define GLM_FORCE_LEFT_HANDED
#include <glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS

#include <gtx/euler_angles.hpp>
#include <gtx/transform2.hpp>
#include <gtx/matrix_transform_2d.hpp>

TEST_CLASS(Mat3)
{
	TEST_METHOD("constructor")
	{
		maj::math::Mat3<float> mat;
		AreEqual(mat.at(0u, 0u), 1.f, FLT_EPSILON);
		AreEqual(mat.at(0u, 1u), 0.f, FLT_EPSILON);
		AreEqual(mat.at(0u, 2u), 0.f, FLT_EPSILON);
		AreEqual(mat.at(1u, 0u), 0.f, FLT_EPSILON);
		AreEqual(mat.at(1u, 1u), 1.f, FLT_EPSILON);
		AreEqual(mat.at(1u, 2u), 0.f, FLT_EPSILON);
		AreEqual(mat.at(2u, 0u), 0.f, FLT_EPSILON);
		AreEqual(mat.at(2u, 1u), 0.f, FLT_EPSILON);
		AreEqual(mat.at(2u, 2u), 1.f, FLT_EPSILON);

		maj::math::Mat3<float> mat1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
		AreEqual(mat1.at(0u), 0.f, FLT_EPSILON);
		AreEqual(mat1.at(1u), 1.f, FLT_EPSILON);
		AreEqual(mat1.at(2u), 2.f, FLT_EPSILON);
		AreEqual(mat1.at(3u), 3.f, FLT_EPSILON);
		AreEqual(mat1.at(4u), 4.f, FLT_EPSILON);
		AreEqual(mat1.at(5u), 5.f, FLT_EPSILON);
		AreEqual(mat1.at(6u), 6.f, FLT_EPSILON);
		AreEqual(mat1.at(7u), 7.f, FLT_EPSILON);
		AreEqual(mat1.at(8u), 8.f, FLT_EPSILON);

		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);
		AreEqual(mat2.at(0u, 0u), 0.f, FLT_EPSILON);
		AreEqual(mat2.at(0u, 1u), 0.f, FLT_EPSILON);
		AreEqual(mat2.at(0u, 2u), 0.f, FLT_EPSILON);
		AreEqual(mat2.at(1u, 0u), 0.f, FLT_EPSILON);
		AreEqual(mat2.at(1u, 1u), 0.f, FLT_EPSILON);
		AreEqual(mat2.at(1u, 2u), 0.f, FLT_EPSILON);
		AreEqual(mat2.at(2u, 0u), 0.f, FLT_EPSILON);
		AreEqual(mat2.at(2u, 1u), 0.f, FLT_EPSILON);
		AreEqual(mat2.at(2u, 2u), 0.f, FLT_EPSILON);
	}

	TEST_METHOD("==")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);

		maj::math::Mat3<float> mat3;
		maj::math::Mat3<float> Mat3(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
		maj::math::Mat3<float> mat5(maj::math::Mat3<float>::skZero);

		IsTrue(mat == mat3);
		IsTrue(mat1 == Mat3);
		IsTrue(mat2 == mat5);
	}

	TEST_METHOD("elem")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);

		float* test = mat.elem();

		AreEqual(test[0u], 1.f, FLT_EPSILON);
		AreEqual(test[1u], 0.f, FLT_EPSILON);
		AreEqual(test[2u], 0.f, FLT_EPSILON);
		AreEqual(test[3u], 0.f, FLT_EPSILON);
		AreEqual(test[4u], 1.f, FLT_EPSILON);
		AreEqual(test[5u], 0.f, FLT_EPSILON);
		AreEqual(test[6u], 0.f, FLT_EPSILON);
		AreEqual(test[7u], 0.f, FLT_EPSILON);
		AreEqual(test[8u], 1.f, FLT_EPSILON);

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
	}

	TEST_METHOD("!=")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.f, 2.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);

		maj::math::Mat3<float> mat3(0.0f, 0.0f, 0.005f, 0.0f, 0.0f, 0.0f, 0.0f, 0.005f, 0.0f);
		maj::math::Mat3<float> mat4(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
		maj::math::Mat3<float> mat5(0.005f, 0.0f, 0.005f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

		IsTrue(mat != mat5);
		IsTrue(mat1 != mat4);
		IsTrue(mat2 != mat3);

	}

	TEST_METHOD("isOrtho")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.f, 2.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);
		maj::math::Mat3<float> mat3(0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f);

		maj::math::Mat3<float> mat4(0.0f, -0.80f, -0.60f,
									0.80f, -0.36f, 0.48f,
									0.60f, 0.48f, -0.64f);

		IsTrue(mat.isOrtho());
		IsFalse(mat1.isOrtho());
		IsFalse(mat2.isOrtho());
		IsTrue(mat3.isOrtho());
		IsTrue(mat4.isOrtho());
	}

	TEST_METHOD("transposed")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);
		maj::math::Mat3<float> mat3(0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f);

		glm::mat3 matt(1);
		glm::mat3 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		glm::mat3 matt2 {};
		glm::mat3 matt3(0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f);

		maj::math::Mat3<float> test = mat1.transposed();
		glm::mat3 test2 = glm::transpose(matt1);

		AreEqual(test[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(test[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(test[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(test[3u], test2[1u][0u], FLT_EPSILON);
		AreEqual(test[4u], test2[1u][1u], FLT_EPSILON);
		AreEqual(test[5u], test2[1u][2u], FLT_EPSILON);
		AreEqual(test[6u], test2[2u][0u], FLT_EPSILON);
		AreEqual(test[7u], test2[2u][1u], FLT_EPSILON);
		AreEqual(test[8u], test2[2u][2u], FLT_EPSILON);

		test = mat3.transposed();
		test2 = glm::transpose(matt3);
		AreEqual(test[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(test[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(test[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(test[3u], test2[1u][0u], FLT_EPSILON);
		AreEqual(test[4u], test2[1u][1u], FLT_EPSILON);
		AreEqual(test[5u], test2[1u][2u], FLT_EPSILON);
		AreEqual(test[6u], test2[2u][0u], FLT_EPSILON);
		AreEqual(test[7u], test2[2u][1u], FLT_EPSILON);
		AreEqual(test[8u], test2[2u][2u], FLT_EPSILON);

		test = mat.transposed();
		test2 = glm::transpose(glm::mat3());
		AreEqual(test[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(test[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(test[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(test[3u], test2[1u][0u], FLT_EPSILON);
		AreEqual(test[4u], test2[1u][1u], FLT_EPSILON);
		AreEqual(test[5u], test2[1u][2u], FLT_EPSILON);
		AreEqual(test[6u], test2[2u][0u], FLT_EPSILON);
		AreEqual(test[7u], test2[2u][1u], FLT_EPSILON);
		AreEqual(test[8u], test2[2u][2u], FLT_EPSILON);
	}

	TEST_METHOD("inversed")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.f, 2.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);
		maj::math::Mat3<float> mat3(0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f);

		maj::math::Mat3<float> mat4(0.0f, -0.80f, -0.60f,
									0.80f, -0.36f, 0.48f,
									0.60f, 0.48f, -0.64f);

		glm::mat3 matt(1);
		glm::mat3 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		glm::mat3 matt2 {};
		glm::mat3 matt3 { 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f };
		glm::mat3 matt4(0.0f, -0.80f, -0.60f,
						0.80f, -0.36f, 0.48f,
						0.60f, 0.48f, -0.64f);

		maj::math::Mat3<float> test { mat.inversed() };
		glm::mat3 test2 { glm::inverse(matt) };
		AreEqual(test[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(test[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(test[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(test[3u], test2[1u][0u], FLT_EPSILON);
		AreEqual(test[4u], test2[1u][1u], FLT_EPSILON);
		AreEqual(test[5u], test2[1u][2u], FLT_EPSILON);
		AreEqual(test[6u], test2[2u][0u], FLT_EPSILON);
		AreEqual(test[7u], test2[2u][1u], FLT_EPSILON);
		AreEqual(test[8u], test2[2u][2u], FLT_EPSILON);

		test = mat3.inversed();
		test2 = glm::inverse(matt3);
		AreEqual(test[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(test[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(test[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(test[3u], test2[1u][0u], FLT_EPSILON);
		AreEqual(test[4u], test2[1u][1u], FLT_EPSILON);
		AreEqual(test[5u], test2[1u][2u], FLT_EPSILON);
		AreEqual(test[6u], test2[2u][0u], FLT_EPSILON);
		AreEqual(test[7u], test2[2u][1u], FLT_EPSILON);
		AreEqual(test[8u], test2[2u][2u], FLT_EPSILON);

		test = mat4.inversed();
		test2 = glm::inverse(matt4);
		AreEqual(test[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(test[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(test[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(test[3u], test2[1u][0u], FLT_EPSILON);
		AreEqual(test[4u], test2[1u][1u], FLT_EPSILON);
		AreEqual(test[5u], test2[1u][2u], FLT_EPSILON);
		AreEqual(test[6u], test2[2u][0u], FLT_EPSILON);
		AreEqual(test[7u], test2[2u][1u], FLT_EPSILON);
		AreEqual(test[8u], test2[2u][2u], FLT_EPSILON);
	}

	TEST_METHOD("transpose")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);
		maj::math::Mat3<float> mat3(0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f);

		glm::mat3 matt(1);
		glm::mat3 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		glm::mat3 matt2 {};
		glm::mat3 matt3(0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f);

		mat1.transpose();
		glm::mat3 test2 = glm::transpose(matt1);

		AreEqual(mat1[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(mat1[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(mat1[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(mat1[3u], test2[1u][0u], FLT_EPSILON);
		AreEqual(mat1[4u], test2[1u][1u], FLT_EPSILON);
		AreEqual(mat1[5u], test2[1u][2u], FLT_EPSILON);
		AreEqual(mat1[6u], test2[2u][0u], FLT_EPSILON);
		AreEqual(mat1[7u], test2[2u][1u], FLT_EPSILON);
		AreEqual(mat1[8u], test2[2u][2u], FLT_EPSILON);

		mat3.transpose();
		test2 = glm::transpose(matt3);
		AreEqual(mat3[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], test2[1u][0u], FLT_EPSILON);
		AreEqual(mat3[4u], test2[1u][1u], FLT_EPSILON);
		AreEqual(mat3[5u], test2[1u][2u], FLT_EPSILON);
		AreEqual(mat3[6u], test2[2u][0u], FLT_EPSILON);
		AreEqual(mat3[7u], test2[2u][1u], FLT_EPSILON);
		AreEqual(mat3[8u], test2[2u][2u], FLT_EPSILON);

		mat.transpose();
		test2 = glm::transpose(matt);
		AreEqual(mat[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], test2[1u][0u], FLT_EPSILON);
		AreEqual(mat[4u], test2[1u][1u], FLT_EPSILON);
		AreEqual(mat[5u], test2[1u][2u], FLT_EPSILON);
		AreEqual(mat[6u], test2[2u][0u], FLT_EPSILON);
		AreEqual(mat[7u], test2[2u][1u], FLT_EPSILON);
		AreEqual(mat[8u], test2[2u][2u], FLT_EPSILON);
	}

	TEST_METHOD("inverse")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.f, 2.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);
		maj::math::Mat3<float> mat3(0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f);

		maj::math::Mat3<float> mat4(0.0f, -0.80f, -0.60f,
									0.80f, -0.36f, 0.48f,
									0.60f, 0.48f, -0.64f);

		glm::mat3 matt(1);
		glm::mat3 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		glm::mat3 matt2 {};
		glm::mat3 matt3 { 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f };
		glm::mat3 matt4(0.0f, -0.80f, -0.60f,
						0.80f, -0.36f, 0.48f,
						0.60f, 0.48f, -0.64f);

		mat.inverse();
		glm::mat3 test2 { glm::inverse(matt) };
		AreEqual(mat[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], test2[1u][0u], FLT_EPSILON);
		AreEqual(mat[4u], test2[1u][1u], FLT_EPSILON);
		AreEqual(mat[5u], test2[1u][2u], FLT_EPSILON);
		AreEqual(mat[6u], test2[2u][0u], FLT_EPSILON);
		AreEqual(mat[7u], test2[2u][1u], FLT_EPSILON);
		AreEqual(mat[8u], test2[2u][2u], FLT_EPSILON);

		mat3.inverse();
		test2 = glm::inverse(matt3);
		AreEqual(mat3[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], test2[1u][0u], FLT_EPSILON);
		AreEqual(mat3[4u], test2[1u][1u], FLT_EPSILON);
		AreEqual(mat3[5u], test2[1u][2u], FLT_EPSILON);
		AreEqual(mat3[6u], test2[2u][0u], FLT_EPSILON);
		AreEqual(mat3[7u], test2[2u][1u], FLT_EPSILON);
		AreEqual(mat3[8u], test2[2u][2u], FLT_EPSILON);

		mat4.inverse();
		test2 = glm::inverse(matt4);
		AreEqual(mat4[0u], test2[0u][0u], FLT_EPSILON);
		AreEqual(mat4[1u], test2[0u][1u], FLT_EPSILON);
		AreEqual(mat4[2u], test2[0u][2u], FLT_EPSILON);
		AreEqual(mat4[3u], test2[1u][0u], FLT_EPSILON);
		AreEqual(mat4[4u], test2[1u][1u], FLT_EPSILON);
		AreEqual(mat4[5u], test2[1u][2u], FLT_EPSILON);
		AreEqual(mat4[6u], test2[2u][0u], FLT_EPSILON);
		AreEqual(mat4[7u], test2[2u][1u], FLT_EPSILON);
		AreEqual(mat4[8u], test2[2u][2u], FLT_EPSILON);

	}

	TEST_METHOD("* Mat3")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
		maj::math::Mat3<float> mat2(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);
		maj::math::Mat3<float> mat3(maj::math::Mat3<float>::skZero);

		glm::mat3 matt(1);
		glm::mat3 matt1(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f);
		glm::mat3 matt2(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);
		glm::mat3 matt3(0);

		maj::math::Mat3<float> res = mat * mat1;
		glm::mat3 resGlm = matt * matt1;

		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][2u], FLT_EPSILON);

		res = mat2 * mat1;
		resGlm = matt2 * matt1;

		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][2u], FLT_EPSILON);

		res = mat3 * mat1;
		resGlm = matt3 * matt1;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][2u], FLT_EPSILON);

	}

	TEST_METHOD("* Vec3")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1 { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f };
		maj::math::Mat3<float> mat2 { 0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f };
		maj::math::Mat3<float> mat3 { maj::math::Mat3<float>::skZero };

		glm::mat3 matt;
		glm::mat3 matt1 { 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f };
		glm::mat3 matt2 { 0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f };
		glm::mat3 matt3 { 0.f };

		maj::math::Vec3<float> v0;
		maj::math::Vec3<float> v1 { 10.0f, 11.0f, 12.0f };
		maj::math::Vec3<float> v2 { 0.f, 11.2558f, 2.45478f };

		glm::vec3 u0;
		glm::vec3 u1 { 10.0f, 11.0f, 12.0f };
		glm::vec3 u2 { 0.f, 11.2558f, 2.45478f };

		maj::math::Vec3<float> res { mat * v0 };
		glm::vec3 resGlm { matt * u0 };

		AreEqual(res.mX, resGlm.x, FLT_EPSILON);
		AreEqual(res.mY, resGlm.y, FLT_EPSILON);
		AreEqual(res.mZ, resGlm.z, FLT_EPSILON);

		res = mat * v1;
		resGlm = matt * u1;

		AreEqual(res.mX, resGlm.x, FLT_EPSILON);
		AreEqual(res.mY, resGlm.y, FLT_EPSILON);
		AreEqual(res.mZ, resGlm.z, FLT_EPSILON);

		res = mat1 * v1;
		resGlm = matt1 * u1;

		AreEqual(res.mX, resGlm.x, FLT_EPSILON);
		AreEqual(res.mY, resGlm.y, FLT_EPSILON);
		AreEqual(res.mZ, resGlm.z, FLT_EPSILON);

		res = mat2 * v2;
		resGlm = matt2 * u2;

		AreEqual(res.mX, resGlm.x, FLT_EPSILON);
		AreEqual(res.mY, resGlm.y, FLT_EPSILON);
		AreEqual(res.mZ, resGlm.z, FLT_EPSILON);
	}

	TEST_METHOD("* float")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);
		maj::math::Mat3<float> mat3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);

		glm::mat3 matt(1);
		glm::mat3 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		glm::mat3 matt2 {};
		glm::mat3 matt3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);


		maj::math::Mat3<float> res = mat * 10.0f;
		glm::mat3 resGlm = matt * 10.0f;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][2u], FLT_EPSILON);

		res = mat1 * 10.0f;
		resGlm = matt1 * 10.0f;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][2u], FLT_EPSILON);


		res = mat3 * 0.0f;
		resGlm = matt3 * 0.0f;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][2u], FLT_EPSILON);

		res = mat3 * PI;
		resGlm = matt3 * PI;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][2u], FLT_EPSILON);
	}

	TEST_METHOD("/ float")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);
		maj::math::Mat3<float> mat3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);

		glm::mat3 matt(1);
		glm::mat3 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		glm::mat3 matt2 {};
		glm::mat3 matt3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);


		maj::math::Mat3<float> res = mat / 10.0f;
		glm::mat3 resGlm = matt / 10.0f;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][2u], FLT_EPSILON);

		res = mat1 / 10.45f;
		resGlm = matt1 / 10.45f;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][2u], FLT_EPSILON);

		res = mat3 / PI;
		resGlm = matt3 / PI;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][2u], FLT_EPSILON);

		res = mat3 / 0.0f;
		AreEqual(res[0u], 1.0f, FLT_EPSILON);
		AreEqual(res[1u], 0.0f, FLT_EPSILON);
		AreEqual(res[2u], 0.0f, FLT_EPSILON);
		AreEqual(res[3u], 0.0f, FLT_EPSILON);
		AreEqual(res[4u], 1.0f, FLT_EPSILON);
		AreEqual(res[5u], 0.0f, FLT_EPSILON);
		AreEqual(res[6u], 0.0f, FLT_EPSILON);
		AreEqual(res[7u], 0.0f, FLT_EPSILON);
		AreEqual(res[8u], 1.0f, FLT_EPSILON);
	}

	TEST_METHOD("-")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);
		maj::math::Mat3<float> mat3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);

		glm::mat3 matt(1);
		glm::mat3 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		glm::mat3 matt2 {};
		glm::mat3 matt3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);


		maj::math::Mat3<float> res = -mat;
		glm::mat3 resGlm = -matt;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][2u], FLT_EPSILON);

		res = -mat1;
		resGlm = -matt1;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][2u], FLT_EPSILON);

		res = -mat3;
		resGlm = -matt3;
		AreEqual(res[0u], resGlm[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], resGlm[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], resGlm[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], resGlm[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], resGlm[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], resGlm[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], resGlm[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], resGlm[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], resGlm[2u][2u], FLT_EPSILON);
	}

	TEST_METHOD("*= float")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);
		maj::math::Mat3<float> mat3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);

		glm::mat3 matt(1);
		glm::mat3 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		glm::mat3 matt2 {};
		glm::mat3 matt3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);

		mat *= 10.0f;
		matt *= 10.0f;

		AreEqual(mat[0u], matt[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], matt[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], matt[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], matt[1u][0u], FLT_EPSILON);
		AreEqual(mat[4u], matt[1u][1u], FLT_EPSILON);
		AreEqual(mat[5u], matt[1u][2u], FLT_EPSILON);
		AreEqual(mat[6u], matt[2u][0u], FLT_EPSILON);
		AreEqual(mat[7u], matt[2u][1u], FLT_EPSILON);
		AreEqual(mat[8u], matt[2u][2u], FLT_EPSILON);

		mat1 *= 10.25f;
		matt1 *= 10.25f;

		AreEqual(mat1[0u], matt1[0u][0u], FLT_EPSILON);
		AreEqual(mat1[1u], matt1[0u][1u], FLT_EPSILON);
		AreEqual(mat1[2u], matt1[0u][2u], FLT_EPSILON);
		AreEqual(mat1[3u], matt1[1u][0u], FLT_EPSILON);
		AreEqual(mat1[4u], matt1[1u][1u], FLT_EPSILON);
		AreEqual(mat1[5u], matt1[1u][2u], FLT_EPSILON);
		AreEqual(mat1[6u], matt1[2u][0u], FLT_EPSILON);
		AreEqual(mat1[7u], matt1[2u][1u], FLT_EPSILON);
		AreEqual(mat1[8u], matt1[2u][2u], FLT_EPSILON);

		mat3 *= 11.82f;
		matt3 *= 11.82f;

		AreEqual(mat3[0u], matt3[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], matt3[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], matt3[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], matt3[1u][0u], FLT_EPSILON);
		AreEqual(mat3[4u], matt3[1u][1u], FLT_EPSILON);
		AreEqual(mat3[5u], matt3[1u][2u], FLT_EPSILON);
		AreEqual(mat3[6u], matt3[2u][0u], FLT_EPSILON);
		AreEqual(mat3[7u], matt3[2u][1u], FLT_EPSILON);
		AreEqual(mat3[8u], matt3[2u][2u], FLT_EPSILON);

		mat3 *= 0.0f;
		matt3 *= 0.0f;

		AreEqual(mat3[0u], matt3[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], matt3[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], matt3[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], matt3[1u][0u], FLT_EPSILON);
		AreEqual(mat3[4u], matt3[1u][1u], FLT_EPSILON);
		AreEqual(mat3[5u], matt3[1u][2u], FLT_EPSILON);
		AreEqual(mat3[6u], matt3[2u][0u], FLT_EPSILON);
		AreEqual(mat3[7u], matt3[2u][1u], FLT_EPSILON);
		AreEqual(mat3[8u], matt3[2u][2u], FLT_EPSILON);
	}

	TEST_METHOD("/= float")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);
		maj::math::Mat3<float> mat3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);

		glm::mat3 matt(1);
		glm::mat3 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		glm::mat3 matt2 {};
		glm::mat3 matt3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);

		mat /= 10.0f;
		matt /= 10.0f;

		AreEqual(mat[0u], matt[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], matt[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], matt[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], matt[1u][0u], FLT_EPSILON);
		AreEqual(mat[4u], matt[1u][1u], FLT_EPSILON);
		AreEqual(mat[5u], matt[1u][2u], FLT_EPSILON);
		AreEqual(mat[6u], matt[2u][0u], FLT_EPSILON);
		AreEqual(mat[7u], matt[2u][1u], FLT_EPSILON);
		AreEqual(mat[8u], matt[2u][2u], FLT_EPSILON);

		mat1 /= 10.45f;
		matt1 /= 10.45f;

		AreEqual(mat1[0u], matt1[0u][0u], FLT_EPSILON);
		AreEqual(mat1[1u], matt1[0u][1u], FLT_EPSILON);
		AreEqual(mat1[2u], matt1[0u][2u], FLT_EPSILON);
		AreEqual(mat1[3u], matt1[1u][0u], FLT_EPSILON);
		AreEqual(mat1[4u], matt1[1u][1u], FLT_EPSILON);
		AreEqual(mat1[5u], matt1[1u][2u], FLT_EPSILON);
		AreEqual(mat1[6u], matt1[2u][0u], FLT_EPSILON);
		AreEqual(mat1[7u], matt1[2u][1u], FLT_EPSILON);
		AreEqual(mat1[8u], matt1[2u][2u], FLT_EPSILON);

		mat3 /= PI;
		matt3 /= PI;

		AreEqual(mat3[0u], matt3[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], matt3[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], matt3[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], matt3[1u][0u], FLT_EPSILON);
		AreEqual(mat3[4u], matt3[1u][1u], FLT_EPSILON);
		AreEqual(mat3[5u], matt3[1u][2u], FLT_EPSILON);
		AreEqual(mat3[6u], matt3[2u][0u], FLT_EPSILON);
		AreEqual(mat3[7u], matt3[2u][1u], FLT_EPSILON);
		AreEqual(mat3[8u], matt3[2u][2u], FLT_EPSILON);

		maj::math::Mat3<float> test { mat3 };
		mat3 /= 0.0f;

		IsTrue(mat3 == test);
	}

	TEST_METHOD("*= Mat3")
	{
		maj::math::Mat3<float> mat;
		maj::math::Mat3<float> mat1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		maj::math::Mat3<float> mat2(maj::math::Mat3<float>::skZero);
		maj::math::Mat3<float> mat3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);

		glm::mat3 matt(1);
		glm::mat3 matt1(0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
		glm::mat3 matt2(0);
		glm::mat3 matt3(0.f, 0.5f, 0.25f, 0.55f, 5.25f, 45.25f, 6.95f, 17.8f, 28.5f);

		mat *= mat1;
		matt *= matt1;

		AreEqual(mat[0u], matt[0u][0u], FLT_EPSILON);
		AreEqual(mat[1u], matt[0u][1u], FLT_EPSILON);
		AreEqual(mat[2u], matt[0u][2u], FLT_EPSILON);
		AreEqual(mat[3u], matt[1u][0u], FLT_EPSILON);
		AreEqual(mat[4u], matt[1u][1u], FLT_EPSILON);
		AreEqual(mat[5u], matt[1u][2u], FLT_EPSILON);
		AreEqual(mat[6u], matt[2u][0u], FLT_EPSILON);
		AreEqual(mat[7u], matt[2u][1u], FLT_EPSILON);
		AreEqual(mat[8u], matt[2u][2u], FLT_EPSILON);

		mat2 *= mat1;
		matt2 *= matt1;

		AreEqual(mat2[0u], matt2[0u][0u], FLT_EPSILON);
		AreEqual(mat2[1u], matt2[0u][1u], FLT_EPSILON);
		AreEqual(mat2[2u], matt2[0u][2u], FLT_EPSILON);
		AreEqual(mat2[3u], matt2[1u][0u], FLT_EPSILON);
		AreEqual(mat2[4u], matt2[1u][1u], FLT_EPSILON);
		AreEqual(mat2[5u], matt2[1u][2u], FLT_EPSILON);
		AreEqual(mat2[6u], matt2[2u][0u], FLT_EPSILON);
		AreEqual(mat2[7u], matt2[2u][1u], FLT_EPSILON);
		AreEqual(mat2[8u], matt2[2u][2u], FLT_EPSILON);

		mat3 *= mat1;
		matt3 *= matt1;

		AreEqual(mat3[0u], matt3[0u][0u], FLT_EPSILON);
		AreEqual(mat3[1u], matt3[0u][1u], FLT_EPSILON);
		AreEqual(mat3[2u], matt3[0u][2u], FLT_EPSILON);
		AreEqual(mat3[3u], matt3[1u][0u], FLT_EPSILON);
		AreEqual(mat3[4u], matt3[1u][1u], FLT_EPSILON);
		AreEqual(mat3[5u], matt3[1u][2u], FLT_EPSILON);
		AreEqual(mat3[6u], matt3[2u][0u], FLT_EPSILON);
		AreEqual(mat3[7u], matt3[2u][1u], FLT_EPSILON);
		AreEqual(mat3[8u], matt3[2u][2u], FLT_EPSILON);
	}

	TEST_METHOD("makeRotationX")
	{
		maj::math::Mat3<float> res { maj::math::Mat3<float>::makeRotationX(10.51f * RAD2DEG) };
		glm::mat3 gmat { glm::rotate(10.51f, glm::vec3 { 1.f, 0.f, 0.f }) };

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		res = maj::math::Mat3<float>::makeRotationX(90.0f);
		gmat = glm::rotate(90.f * DEG2RAD, glm::vec3 { 1.f, 0.f, 0.f });

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		res = maj::math::Mat3<float>::makeRotationX(0.0f);
		gmat = glm::rotate(0.f, glm::vec3 { 1.f, 0.f, 0.f });

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		res = maj::math::Mat3<float>::makeRotationX(std::numeric_limits<float>::lowest());
		gmat = glm::rotate(std::numeric_limits<float>::lowest() * DEG2RAD, glm::vec3 { 1.f, 0.f, 0.f });

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);
	}

	TEST_METHOD("makeRotationY")
	{
		maj::math::Mat3<float> res { maj::math::Mat3<float>::makeRotationY(10.51f * RAD2DEG) };
		glm::mat3 gmat { glm::rotate(10.51f, glm::vec3 { 0.f, 1.f, 0.f }) };

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		res = maj::math::Mat3<float>::makeRotationY(90.0f);
		gmat = glm::rotate(90.f * DEG2RAD, glm::vec3 { 0.f, 1.f, 0.f });

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		res = maj::math::Mat3<float>::makeRotationY(0.0f);
		gmat = glm::rotate(0.f, glm::vec3 { 0.f, 1.f, 0.f });

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		res = maj::math::Mat3<float>::makeRotationY(std::numeric_limits<float>::lowest());
		gmat = glm::rotate(std::numeric_limits<float>::lowest() * DEG2RAD, glm::vec3 { 0.f, 1.f, 0.f });

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);
	}

	TEST_METHOD("makeRotationZ")
	{
		maj::math::Mat3<float> res { maj::math::Mat3<float>::makeRotationZ(10.51f * RAD2DEG) };
		glm::mat3 gmat { glm::rotate(10.51f, glm::vec3 { 0.f, 0.f, 1.f }) };

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		res = maj::math::Mat3<float>::makeRotationZ(90.0f);
		gmat = glm::rotate(90.f * DEG2RAD, glm::vec3 { 0.f, 0.f, 1.f });

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		res = maj::math::Mat3<float>::makeRotationZ(0.0f);
		gmat = glm::rotate(0.f, glm::vec3 { 0.f, 0.f, 1.f });

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		res = maj::math::Mat3<float>::makeRotationZ(std::numeric_limits<float>::lowest());
		gmat = glm::rotate(std::numeric_limits<float>::lowest() * DEG2RAD, glm::vec3 { 0.f, 0.f, 1.f });

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);
	}

	TEST_METHOD("makeRotation quat")
	{
		maj::math::Quaternion<float> q { maj::math::Quaternion<float>::fromEuler({ 10.51f * RAD2DEG, RAD2DEG, 0.f }) };
		glm::quat g { glm::vec3 { 10.51f, 1.f, 0.f } };

		maj::math::Mat3<float> res { maj::math::Mat3<float>::makeRotation(q) };
		glm::mat3 gmat { glm::normalize(g) };

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		q = maj::math::Quaternion<float>::fromEuler({ 90.0f, -90.f, 135.f });
		g = glm::vec3 { 90.f * DEG2RAD, -90.f * DEG2RAD, 135.f * DEG2RAD };

		res = maj::math::Mat3<float>::makeRotation(q);
		gmat = glm::mat3_cast(glm::normalize(g));

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		q = { 0.f, 0.f, 0.f, 0.f };
		g = { 0.f, 0.f, 0.f, 0.f };

		res = maj::math::Mat3<float>::makeRotation(q);
		gmat = glm::mat3_cast(glm::normalize(g));

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		q = { 1.f, 0.f, 0.f, 0.f };
		g = { 1.f, 0.f, 0.f, 0.f };

		res = maj::math::Mat3<float>::makeRotation(q);
		gmat = glm::mat3_cast(glm::normalize(g));

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		q = maj::math::Quaternion<float>::fromEuler({ std::numeric_limits<float>::lowest(), 0.f, std::numeric_limits<float>::max() });
		g = glm::vec3 { std::numeric_limits<float>::lowest() * DEG2RAD, 0.f, std::numeric_limits<float>::max() * DEG2RAD };

		res = maj::math::Mat3<float>::makeRotation(q);
		gmat = glm::mat3_cast(glm::normalize(g));

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);
	}

	TEST_METHOD("makeRotation euler")
	{
		maj::math::Mat3<float> res { maj::math::Mat3<float>::makeRotation({ 10.51f * RAD2DEG, RAD2DEG, 0.f }) };
		glm::mat3 gmat { glm::eulerAngleXYZ(10.51f, 1.f, 0.f) };

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		res = maj::math::Mat3<float>::makeRotation({ 90.0f, -90.f, 135.f });
		gmat = glm::eulerAngleXYZ(90.f * DEG2RAD, -90.f * DEG2RAD, 135.f * DEG2RAD);

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		res = maj::math::Mat3<float>::makeRotation({ 0.f, 0.f, 0.f });
		gmat = glm::eulerAngleXYZ(0.f, 0.f, 0.f);

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);

		res = maj::math::Mat3<float>::makeRotation({ std::numeric_limits<float>::lowest(), 0.f, std::numeric_limits<float>::max() });
		gmat = glm::eulerAngleXYZ(std::numeric_limits<float>::lowest() * DEG2RAD, 0.f, std::numeric_limits<float>::max() * DEG2RAD);

		AreEqual(res[0u], gmat[0u][0u], FLT_EPSILON);
		AreEqual(res[1u], gmat[0u][1u], FLT_EPSILON);
		AreEqual(res[2u], gmat[0u][2u], FLT_EPSILON);
		AreEqual(res[3u], gmat[1u][0u], FLT_EPSILON);
		AreEqual(res[4u], gmat[1u][1u], FLT_EPSILON);
		AreEqual(res[5u], gmat[1u][2u], FLT_EPSILON);
		AreEqual(res[6u], gmat[2u][0u], FLT_EPSILON);
		AreEqual(res[7u], gmat[2u][1u], FLT_EPSILON);
		AreEqual(res[8u], gmat[2u][2u], FLT_EPSILON);
	}
};