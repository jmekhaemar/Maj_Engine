#include "Test.h"

#include <Transform.h>

#define GLM_FORCE_LEFT_HANDED
#include <glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS

#include <gtx/vector_angle.hpp>
#include <gtx/compatibility.hpp>

#include <gtc/matrix_transform.hpp>

TEST_CLASS("Transform")
{
	/*TEST_METHOD("constructor")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(10.05f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(50.0f,11.02f,1.0654f) };
		maj::math::Transform<float> transTest { maj::math::Vec3<float>(10.05f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(50.0f,11.02f,1.0654f) };
		maj::math::Transform<float> trans3 { transTest };

		maj::math::Vec3<float> pos;
		maj::math::Quaternion<float> rot;
		maj::math::Vec3<float> scale { 1.0f };

		IsTrue(trans.position() == pos);
		IsTrue(trans.rotation() == rot);
		IsTrue(trans.scale() == scale);

		pos = maj::math::Vec3<float>(10.05f);
		scale = maj::math::Vec3<float>(50.0f, 11.02f, 1.0654f);
		IsTrue(trans2.position() == maj::math::Vec3<float>(10.05f));
		IsTrue(trans2.rotation() == maj::math::Quaternion<float>());
		IsTrue(trans2.scale() == maj::math::Vec3<float>(50.0f, 11.02f, 1.0654f));

		IsTrue(trans3.position() == transTest.position());
		IsTrue(trans3.rotation() == transTest.rotation());
		IsTrue(trans3.scale() == transTest.scale());

	}

	TEST_METHOD("==")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(10.05f,20.1f,30.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(50.0f,11.02f,1.0654f) };
		maj::math::Transform<float> transTest { maj::math::Vec3<float>(2.05f,8.156f,75.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };
		maj::math::Transform<float> trans3 { transTest };

		maj::math::Transform<float> T;
		maj::math::Transform<float> T1 { maj::math::Vec3<float>(10.05f,20.1f,30.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(50.0f,11.02f,1.0654f) };
		maj::math::Transform<float> T2 { maj::math::Vec3<float>(2.05f,8.156f,75.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };

		IsTrue(trans == T);
		IsTrue(trans2 == T1);
		IsTrue(trans3 == T2);
	}

	TEST_METHOD("!=")
	{
		maj::math::Transform<float> trans(maj::math::Vec3<float>(), maj::math::Quaternion<float>(10.0f, 55.0f, 65.0f, 8.0f), maj::math::Vec3<float>());
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(10.05f,2.1f,30.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(50.0f,11.0222f,1.0654f) };
		maj::math::Transform<float> transTest { maj::math::Vec3<float>(2.05f,8.156f,75.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };
		maj::math::Transform<float> trans3 { transTest };

		maj::math::Transform<float> T;
		maj::math::Transform<float> T1 { maj::math::Vec3<float>(10.05f,20.1f,30.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(50.0f,11.02f,1.0654f) };
		maj::math::Transform<float> T2 { maj::math::Vec3<float>(2.05f,8.16556f,75.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(6.54f,3.7895f,5.3468f) };

		IsTrue(trans != T);
		IsTrue(trans2 != T1);
		IsTrue(trans3 != T2);
	}

	TEST_METHOD("inversed")
	{
		maj::math::Transform<float> trans(maj::math::Vec3<float>(), maj::math::Quaternion<float>(10.0f, 55.0f, 65.0f, 8.0f), maj::math::Vec3<float>());
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(10.05f,2.1f,30.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(50.0f,11.02f,1.0654f) };
		maj::math::Transform<float> transTest { maj::math::Vec3<float>(2.05f,8.156f,75.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };
		maj::math::Transform<float> trans3 { transTest };

		maj::math::Transform<float> T;
		maj::math::Transform<float> T1 { maj::math::Vec3<float>(10.05f,2.1f,30.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(50.0f,11.02f,1.0654f) };
		maj::math::Transform<float> T2 { maj::math::Vec3<float>(2.05f,8.156f,75.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };

		maj::math::Transform<float> res = trans.inversed().inversed();

		IsTrue(res.position() == trans.position());
		IsTrue(res.rotation() == trans.rotation());
		IsTrue(res.scale() == trans.scale());

		res = trans2.inversed().inversed();
		IsTrue(res.position() == T1.position());
		IsTrue(res.rotation() == T1.rotation());
		IsTrue(res.scale() == T1.scale());

		res = trans3.inversed().inversed();
		IsTrue(res.position() == T2.position());
		IsTrue(res.rotation() == T2.rotation());
		IsTrue(res.scale() == T2.scale());
	}

	TEST_METHOD("inverse")
	{
		maj::math::Transform<float> trans(maj::math::Vec3<float>(), maj::math::Quaternion<float>(10.0f, 55.0f, 65.0f, 8.0f), maj::math::Vec3<float>());
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(10.05f,2.1f,30.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(50.0f,11.02f,1.0654f) };
		maj::math::Transform<float> transTest { maj::math::Vec3<float>(2.05f,8.156f,75.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };
		maj::math::Transform<float> trans3 { transTest };

		maj::math::Transform<float> T(maj::math::Vec3<float>(), maj::math::Quaternion<float>(10.0f, 55.0f, 65.0f, 8.0f), maj::math::Vec3<float>());
		maj::math::Transform<float> T1 { maj::math::Vec3<float>(10.05f,2.1f,30.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(50.0f,11.02f,1.0654f) };
		maj::math::Transform<float> T2 { maj::math::Vec3<float>(2.05f,8.156f,75.052f),maj::math::Quaternion<float>(),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };

		trans.inverse();
		trans.inverse();
		IsTrue(trans.position() == T.position());
		IsTrue(trans.rotation() == T.rotation());
		IsTrue(trans.scale() == T.scale());

		trans2.inverse();
		trans2.inverse();
		IsTrue(trans2.position() == T1.position());
		IsTrue(trans2.rotation() == T1.rotation());
		IsTrue(trans2.scale() == T1.scale());

		trans3.inverse();
		trans3.inverse();
		IsTrue(trans3.position() == T2.position());
		IsTrue(trans3.rotation() == T2.rotation());
		IsTrue(trans3.scale() == T2.scale());
	}

	TEST_METHOD("matrix")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(1.005f,2.1f,1.5052f),maj::math::Quaternion<float>(10.0f, 55.0f, 65.0f, 8.0f),maj::math::Vec3<float>(5.0f,1.102f,1.0654f) };
		maj::math::Transform<float> trans3 { maj::math::Vec3<float>(2.05f,8.156f,2.5052f),maj::math::Quaternion<float>(10.541f, 55.751f, 65.987f, 8.002f),maj::math::Vec3<float>(1.54f,1.795f,3.3468f) };

		glm::vec3 pos(1.005f, 2.1f, 1.5052f);
		glm::quat rot(10.0f, 55.0f, 65.0f, 8.0f);
		glm::vec3 scale(5.0f, 1.102f, 1.0654f);
		rot = glm::normalize(rot);
		glm::mat4 test2 = glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot));
		maj::math::Mat4<float> test = trans2.matrix();

		IsTrue(maj::math::Helper<float>::areSame(test[0u], test2[0u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[1u], test2[0u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[2u], test2[0u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[3u], test2[0u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[4u], test2[1u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[5u], test2[1u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[6u], test2[1u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[7u], test2[1u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[8u], test2[2u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[9u], test2[2u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[10u], test2[2u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[11u], test2[2u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[12u], test2[3u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[13u], test2[3u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[14u], test2[3u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[15u], test2[3u][3u]));

		pos = glm::vec3(2.05f, 8.156f, 2.5052f);
		rot = glm::quat(10.541f, 55.751f, 65.987f, 8.002f);
		scale = glm::vec3(1.54f, 1.795f, 3.3468f);
		rot = glm::normalize(rot);
		test2 = glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot));
		test = trans3.matrix();

		IsTrue(maj::math::Helper<float>::areSame(test[0u], test2[0u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[1u], test2[0u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[2u], test2[0u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[3u], test2[0u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[4u], test2[1u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[5u], test2[1u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[6u], test2[1u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[7u], test2[1u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[8u], test2[2u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[9u], test2[2u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[10u], test2[2u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[11u], test2[2u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[12u], test2[3u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[13u], test2[3u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[14u], test2[3u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[15u], test2[3u][3u]));

		pos = glm::vec3(0.0f, 0.0f, 0.0f);
		rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
		rot = glm::normalize(rot);
		test2 = glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot));
		test = trans.matrix();

		IsTrue(maj::math::Helper<float>::areSame(test[0u], test2[0u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[1u], test2[0u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[2u], test2[0u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[3u], test2[0u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[4u], test2[1u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[5u], test2[1u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[6u], test2[1u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[7u], test2[1u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[8u], test2[2u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[9u], test2[2u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[10u], test2[2u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[11u], test2[2u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[12u], test2[3u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[13u], test2[3u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[14u], test2[3u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[15u], test2[3u][3u]));
	}

	TEST_METHOD("forward")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(10.05f,2.1f,30.052f),maj::math::Quaternion<float>(10.0f, 55.0f, 65.0f, 8.0f),maj::math::Vec3<float>(50.0f,11.02f,1.0654f) };
		maj::math::Transform<float> trans3 { maj::math::Vec3<float>(2.05f,8.156f,75.052f),maj::math::Quaternion<float>(10.55f, 56.654f, 68.659f, 54.5648f),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };

		glm::quat rot(1.0f, 0.0f, 0.0f, 0.0f);
		glm::quat rot1(10.0f, 55.0f, 65.0f, 8.0f);
		glm::quat rot2(10.55f, 56.654f, 68.659f, 54.5648f);

		glm::vec3 test2 = glm::normalize(rot) * glm::vec3(0.0f, 0.0f, 1.0f);
		maj::math::Vec3<float> test { trans.forward() };

		AreEqual(test.mX, test2.x, FLT_EPSILON);
		AreEqual(test.mY, test2.y, FLT_EPSILON);
		AreEqual(test.mZ, test2.z, FLT_EPSILON);

		test2 = glm::normalize(rot1) * glm::vec3(0.0f, 0.0f, 1.0f);
		test = trans2.forward();

		AreEqual(test.mX, test2.x, FLT_EPSILON);
		AreEqual(test.mY, test2.y, FLT_EPSILON);
		AreEqual(test.mZ, test2.z, FLT_EPSILON);

		test2 = glm::normalize(rot2) * glm::vec3(0.0f, 0.0f, 1.0f);
		test = trans3.forward();

		AreEqual(test.mX, test2.x, FLT_EPSILON);
		AreEqual(test.mY, test2.y, FLT_EPSILON);
		AreEqual(test.mZ, test2.z, FLT_EPSILON);
	}

	TEST_METHOD("up")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2{ maj::math::Vec3<float>(10.05f,2.1f,30.052f),maj::math::Quaternion<float>(10.0f, 55.0f, 65.0f, 8.0f),maj::math::Vec3<float>(50.0f,11.02f,1.0654f) };
		maj::math::Transform<float> trans3{ maj::math::Vec3<float>(2.05f,8.156f,75.052f),maj::math::Quaternion<float>(10.55f, 56.654f, 68.659f, 54.5648f),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };

		glm::quat rot(1.0f, 0.0f, 0.0f, 0.0f);
		glm::quat rot1(10.0f, 55.0f, 65.0f, 8.0f);
		glm::quat rot2(10.55f, 56.654f, 68.659f, 54.5648f);

		glm::vec3 test2 = glm::normalize(rot) * glm::vec3(0.0f, 1.0f, 0.0f);
		maj::math::Vec3<float> test{ trans.up() };

		AreEqual(test.mX, test2.x, FLT_EPSILON);
		AreEqual(test.mY, test2.y, FLT_EPSILON);
		AreEqual(test.mZ, test2.z, FLT_EPSILON);

		test2 = glm::normalize(rot1) * glm::vec3(0.0f, 1.0f, 0.0f);
		test = trans2.up();

		AreEqual(test.mX, test2.x, FLT_EPSILON);
		AreEqual(test.mY, test2.y, FLT_EPSILON);
		AreEqual(test.mZ, test2.z, FLT_EPSILON);

		test2 = glm::normalize(rot2) * glm::vec3(0.0f, 1.0f, 0.0f);
		test = trans3.up();

		AreEqual(test.mX, test2.x, FLT_EPSILON);
		AreEqual(test.mY, test2.y, FLT_EPSILON);
		AreEqual(test.mZ, test2.z, FLT_EPSILON);
	}

	TEST_METHOD("right")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2{ maj::math::Vec3<float>(10.05f,2.1f,30.052f),maj::math::Quaternion<float>(10.0f, 55.0f, 65.0f, 8.0f),maj::math::Vec3<float>(50.0f,11.02f,1.0654f) };
		maj::math::Transform<float> trans3{ maj::math::Vec3<float>(2.05f,8.156f,75.052f),maj::math::Quaternion<float>(10.55f, 56.654f, 68.659f, 54.5648f),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };

		glm::quat rot(1.0f, 0.0f, 0.0f, 0.0f);
		glm::quat rot1(10.0f, 55.0f, 65.0f, 8.0f);
		glm::quat rot2(10.55f, 56.654f, 68.659f, 54.5648f);

		glm::vec3 test2 = glm::normalize(rot) * glm::vec3(1.0f, 0.0f, 0.0f);
		maj::math::Vec3<float> test{ trans.right() };

		AreEqual(test.mX, test2.x, FLT_EPSILON);
		AreEqual(test.mY, test2.y, FLT_EPSILON);
		AreEqual(test.mZ, test2.z, FLT_EPSILON);

		test2 = glm::normalize(rot1) * glm::vec3(1.0f, 0.0f, 0.0f);
		test = trans2.right();

		AreEqual(test.mX, test2.x, FLT_EPSILON);
		AreEqual(test.mY, test2.y, FLT_EPSILON);
		AreEqual(test.mZ, test2.z, FLT_EPSILON);

		test2 = glm::normalize(rot2) * glm::vec3(1.0f, 0.0f, 0.0f);
		test = trans3.right();

		AreEqual(test.mX, test2.x, FLT_EPSILON);
		AreEqual(test.mY, test2.y, FLT_EPSILON);
		AreEqual(test.mZ, test2.z, FLT_EPSILON);
	}

	TEST_METHOD("getter setter")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(10.05f,2.1f,30.052f),maj::math::Quaternion<float>(10.0f, 55.0f, 65.0f, 8.0f),maj::math::Vec3<float>(50.0f,11.02f,1.0654f) };
		maj::math::Transform<float> trans3 { maj::math::Vec3<float>(2.05f,8.156f,75.052f),maj::math::Quaternion<float>(10.55f, 56.654f, 68.659f, 54.5648f),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };

		trans.position(maj::math::Vec3<float>(0.0f, 1.0f, 3.0f));
		trans.rotation(maj::math::Quaternion<float>(0.0f, 1.0f, 3.f, 32.0f));
		trans.scale(maj::math::Vec3<float>(1.0f, 0.0f, 3.0f));

		trans2.position(maj::math::Vec3<float>(0.0f, 0.1f, 3.12f));
		trans2.rotation(maj::math::Quaternion<float>(0.0f, 0.1f, 3.12f, 32.01f));
		trans2.scale(maj::math::Vec3<float>(0.1f, 0.0f, 3.12f));

		trans3.position(maj::math::Vec3<float>());
		trans3.rotation(maj::math::Quaternion<float>());
		trans3.scale(maj::math::Vec3<float>());

		IsTrue(trans.position() == maj::math::Vec3<float>(0.0f, 1.0f, 3.0f));
		IsTrue(trans.rotation() == maj::math::Quaternion<float>(0.0f, 1.0f, 3.0f, 32.0f).normalized());
		IsTrue(trans.scale() == maj::math::Vec3<float>(1.0f, 0.0f, 3.0f));

		IsTrue(trans2.position() == maj::math::Vec3<float>(0.0f, 0.1f, 3.12f));
		IsTrue(trans2.rotation() == maj::math::Quaternion<float>(0.0f, 0.1f, 3.12f, 32.01f).normalized());
		IsTrue(trans2.scale() == maj::math::Vec3<float>(0.1f, 0.0f, 3.12f));

		IsTrue(trans3.position() == maj::math::Vec3<float>());
		IsTrue(trans3.rotation() == maj::math::Quaternion<float>());
		IsTrue(trans3.scale() == maj::math::Vec3<float>());

		trans3.position(maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		trans3.rotation(maj::math::Quaternion<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));
		trans3.scale(maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));

		IsTrue(trans3.position() == maj::math::Vec3<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		IsTrue(trans3.rotation() == maj::math::Quaternion<float>(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()).normalized());
		IsTrue(trans3.scale() == maj::math::Vec3<float>(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
	}

	TEST_METHOD("lookAt")
	{
		// Inverse glm because it creates a view matrix and we want it in model space
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(10.0f, 2.0f, 30.0f),maj::math::Quaternion<float>(10.0f, 55.0f, 65.0f, 8.0f),maj::math::Vec3<float>(1.0f,1.0f,1.0f) };
		maj::math::Transform<float> trans3 { maj::math::Vec3<float>(2.05f,8.156f,15.052f),maj::math::Quaternion<float>(10.55f, 56.654f, 68.659f, 54.5648f),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };

		trans2.lookAt(trans.position());

		glm::mat4 test2 { glm::inverse(glm::lookAtLH(glm::vec3(0.0f, 0.0f, 0.0f), -glm::vec3(10.0f, 2.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f))) };
		test2 = glm::translate(glm::vec3(10.0f, 2.0f, 30.0f)) * test2;
		maj::math::Mat4<float> test { trans2.matrix() };

		IsTrue(maj::math::Helper<float>::areSame(test[0u], test2[0u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[1u], test2[0u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[2u], test2[0u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[3u], test2[0u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[4u], test2[1u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[5u], test2[1u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[6u], test2[1u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[7u], test2[1u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[8u], test2[2u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[9u], test2[2u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[10u], test2[2u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[11u], test2[2u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[12u], test2[3u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[13u], test2[3u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[14u], test2[3u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[15u], test2[3u][3u]));

		trans3.lookAt(trans.position());

		test2 = glm::inverse(glm::lookAtLH(glm::vec3(0.0f, 0.0f, 0.0f), -glm::vec3(2.05f, 8.156f, 15.052f), glm::vec3(0.0f, 1.0f, 0.0f)));
		glm::quat rot { glm::normalize(glm::quat_cast(test2)) };
		test2 = glm::translate(glm::vec3(2.05f, 8.156f, 15.052f)) * glm::rotate(glm::scale(glm::vec3(6.54f, 3.795f, 5.3468f)), glm::angle(rot), glm::axis(rot));
		test = trans3.matrix();

		IsTrue(maj::math::Helper<float>::areSame(test[0u], test2[0u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[1u], test2[0u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[2u], test2[0u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[3u], test2[0u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[4u], test2[1u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[5u], test2[1u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[6u], test2[1u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[7u], test2[1u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[8u], test2[2u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[9u], test2[2u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[10u], test2[2u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[11u], test2[2u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(test[12u], test2[3u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(test[13u], test2[3u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(test[14u], test2[3u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(test[15u], test2[3u][3u]));

		trans.lookAt(trans.position());
		test = trans.matrix();

		AreEqual(test[0u], 1.0f, FLT_EPSILON);
		AreEqual(test[1u], 0.0f, FLT_EPSILON);
		AreEqual(test[2u], 0.0f, FLT_EPSILON);
		AreEqual(test[3u], 0.0f, FLT_EPSILON);
		AreEqual(test[4u], 0.0f, FLT_EPSILON);
		AreEqual(test[5u], 1.0f, FLT_EPSILON);
		AreEqual(test[6u], 0.0f, FLT_EPSILON);
		AreEqual(test[7u], 0.0f, FLT_EPSILON);
		AreEqual(test[8u], 0.0f, FLT_EPSILON);
		AreEqual(test[9u], 0.0f, FLT_EPSILON);
		AreEqual(test[10u], 1.0f, FLT_EPSILON);
		AreEqual(test[11u], 0.0f, FLT_EPSILON);
		AreEqual(test[12u], 0.0f, FLT_EPSILON);
		AreEqual(test[13u], 0.0f, FLT_EPSILON);
		AreEqual(test[14u], 0.0f, FLT_EPSILON);
		AreEqual(test[15u], 1.0f, FLT_EPSILON);
	}

	TEST_METHOD("rotate")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(10.0f, 2.0f, 30.0f),maj::math::Quaternion<float>(1.0f, 5.0f, 6.0f, 8.0f),maj::math::Vec3<float>(1.0f,1.0f,1.0f) };
		maj::math::Transform<float> trans3 { maj::math::Vec3<float>(2.05f,8.156f,75.052f),maj::math::Quaternion<float>(1.55f, 6.654f, 8.659f, 4.5648f),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };

		glm::quat rot(1.0f, 0.0f, 0.0f, 0.0f);
		glm::quat rot2(1.0f, 5.0f, 6.0f, 8.0f);
		glm::quat rot3(1.55f, 6.654f, 8.659f, 4.5648f);

		glm::quat test2 = rot * rot;
		maj::math::Transform<float> test = trans;
		test.rotate(trans.rotation());
		AreEqual(test2.x, test.rotation().mX, FLT_EPSILON);
		AreEqual(test2.y, test.rotation().mY, FLT_EPSILON);
		AreEqual(test2.z, test.rotation().mZ, FLT_EPSILON);
		AreEqual(test2.w, test.rotation().mW, FLT_EPSILON);

		test2 = glm::normalize(rot2 * rot2);
		test = trans2;
		test.rotate(trans2.rotation());
		AreEqual(test2.x, test.rotation().mX, FLT_EPSILON);
		AreEqual(test2.y, test.rotation().mY, FLT_EPSILON);
		AreEqual(test2.z, test.rotation().mZ, FLT_EPSILON);
		AreEqual(test2.w, test.rotation().mW, FLT_EPSILON);

		test2 = glm::normalize(rot3 * rot2);
		test = trans3;
		test.rotate(trans2.rotation());
		IsTrue(maj::math::Helper<float>::areSame(test2.x, test.rotation().mX));
		IsTrue(maj::math::Helper<float>::areSame(test2.y, test.rotation().mY));
		IsTrue(maj::math::Helper<float>::areSame(test2.z, test.rotation().mZ));
		IsTrue(maj::math::Helper<float>::areSame(test2.w, test.rotation().mW));

	}

	TEST_METHOD("translate")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(10.0f, 2.0f, 30.0f),maj::math::Quaternion<float>(1.0f, 5.0f, 6.0f, 8.0f),maj::math::Vec3<float>(1.0f,1.0f,1.0f) };
		maj::math::Transform<float> trans3 { maj::math::Vec3<float>(2.05f,8.156f,75.052f),maj::math::Quaternion<float>(1.55f, 6.654f, 8.659f, 4.5648f),maj::math::Vec3<float>(6.54f,3.795f,5.3468f) };

		glm::vec3 rot(0.0f, 0.0f, 0.0f);
		glm::vec3 rot2(10.0f, 2.0f, 30.0f);
		glm::vec3 rot3(2.05f, 8.156f, 75.052f);

		glm::vec3 test2 = rot + rot;
		maj::math::Transform<float> test = trans;
		test.translate(trans.position());
		AreEqual(test2.x, test.position().mX, FLT_EPSILON);
		AreEqual(test2.y, test.position().mY, FLT_EPSILON);
		AreEqual(test2.z, test.position().mZ, FLT_EPSILON);

		test2 = rot2 + rot2;
		test = trans2;
		test.translate(trans2.position());
		AreEqual(test2.x, test.position().mX, FLT_EPSILON);
		AreEqual(test2.y, test.position().mY, FLT_EPSILON);
		AreEqual(test2.z, test.position().mZ, FLT_EPSILON);

		test2 = rot3 + rot2;
		test = trans3;
		test.translate(trans2.position());
		IsTrue(maj::math::Helper<float>::areSame(test2.x, test.position().mX));
		IsTrue(maj::math::Helper<float>::areSame(test2.y, test.position().mY));
		IsTrue(maj::math::Helper<float>::areSame(test2.z, test.position().mZ));
	}

	TEST_METHOD("* Vec3")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(6.0f, 2.0f, 7.0f),maj::math::Quaternion<float>(1.0f, 5.0f, 6.0f, 8.0f),maj::math::Vec3<float>(1.0f,1.0f,1.0f) };
		maj::math::Transform<float> trans3 { maj::math::Vec3<float>(2.05f,4.156f,5.052f),maj::math::Quaternion<float>(1.55f, 6.654f, 8.659f, 4.5648f),maj::math::Vec3<float>(0.54f,1.795f,1.3468f) };

		glm::vec4 vec1(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 vec2(6.0f, 8.0f, 4.0f, 1.0f);
		glm::vec4 vec3(3.54f, 2.561f, 4.546f, 1.0f);

		glm::quat rot(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 pos(0.0f, 0.0f, 0.0f);
		glm::vec3 scale(1.0f, 1.0f, 1.0f);

		rot = glm::normalize(rot);
		glm::vec3 test2 = glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot)) * vec1;
		maj::math::Vec3<float> test = trans * maj::math::Vec3<float>(0.0f, 0.0f, 0.0f);

		AreEqual(test2.x, test.mX, FLT_EPSILON);
		AreEqual(test2.y, test.mY, FLT_EPSILON);
		AreEqual(test2.z, test.mZ, FLT_EPSILON);

		rot = glm::quat(1.0f, 5.0f, 6.0f, 8.0f);
		pos = glm::vec3(6.0f, 2.0f, 7.0f);
		scale = glm::vec3(1.0f, 1.0f, 1.0f);

		rot = glm::normalize(rot);
		test2 = (glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot))) * vec2;
		test = trans2 * maj::math::Vec3<float>(6.0f, 8.0f, 4.0f);

		IsTrue(maj::math::Helper<float>::areSame(test2.x, test.mX));
		IsTrue(maj::math::Helper<float>::areSame(test2.y, test.mY));
		IsTrue(maj::math::Helper<float>::areSame(test2.z, test.mZ));

		rot = glm::quat(1.55f, 6.654f, 8.659f, 4.5648f);
		pos = glm::vec3(2.05f, 4.156f, 5.052f);
		scale = glm::vec3(0.54f, 1.795f, 1.3468f);

		rot = glm::normalize(rot);
		test2 = (glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot))) * vec3;
		test = trans3 * maj::math::Vec3<float>(3.54f, 2.561f, 4.546f);

		IsTrue(maj::math::Helper<float>::areSame(test2.x, test.mX));
		IsTrue(maj::math::Helper<float>::areSame(test2.y, test.mY));
		IsTrue(maj::math::Helper<float>::areSame(test2.z, test.mZ));

		rot = glm::quat(1.55f, 6.654f, 8.659f, 4.5648f);
		pos = glm::vec3(2.05f, 4.156f, 5.052f);
		scale = glm::vec3(0.54f, 1.795f, 1.3468f);

		rot = glm::normalize(rot);
		test2 = (glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot))) * vec1;
		test = trans3 * maj::math::Vec3<float>();

		IsTrue(maj::math::Helper<float>::areSame(test2.x, test.mX));
		IsTrue(maj::math::Helper<float>::areSame(test2.y, test.mY));
		IsTrue(maj::math::Helper<float>::areSame(test2.z, test.mZ));
	}

	TEST_METHOD("* Vec4")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(10.0f, 2.0f, 30.0f),maj::math::Quaternion<float>(1.0f, 5.0f, 6.0f, 8.0f),maj::math::Vec3<float>(1.0f,1.0f,1.0f) };
		maj::math::Transform<float> trans3 { maj::math::Vec3<float>(2.05f,8.156f,5.052f),maj::math::Quaternion<float>(1.55f, 6.654f, 8.659f, 4.5648f),maj::math::Vec3<float>(0.54f,1.795f,1.3468f) };

		glm::vec4 vec1(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 vec2(6.0f, 8.0f, 4.0f, 1.0f);
		glm::vec4 vec3(3.54f, 2.561f, 4.546f, 1.0f);
		glm::vec4 vec4(3.54f, 2.561f, 4.546f, 0.0f);

		glm::quat rot(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 pos(0.0f, 0.0f, 0.0f);
		glm::vec3 scale(1.0f, 1.0f, 1.0f);

		rot = glm::normalize(rot);
		glm::vec4 test2 = glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot)) * vec1;
		maj::math::Vec4<float> test = trans * maj::math::Vec4<float>(0.0f, 0.0f, 0.0f, 1.0f);
		AreEqual(test2.x, test.mX, FLT_EPSILON);
		AreEqual(test2.y, test.mY, FLT_EPSILON);
		AreEqual(test2.z, test.mZ, FLT_EPSILON);
		AreEqual(test2.w, test.mW, FLT_EPSILON);

		rot = glm::quat(1.0f, 5.0f, 6.0f, 8.0f);
		pos = glm::vec3(10.0f, 2.0f, 30.0f);
		scale = glm::vec3(1.0f, 1.0f, 1.0f);

		rot = glm::normalize(rot);
		test2 = glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot)) * vec2;
		test = trans2 * maj::math::Vec4<float>(6.0f, 8.0f, 4.0f, 1.0f);
		IsTrue(maj::math::Helper<float>::areSame(test2.x, test.mX));
		IsTrue(maj::math::Helper<float>::areSame(test2.y, test.mY));
		IsTrue(maj::math::Helper<float>::areSame(test2.z, test.mZ));
		IsTrue(maj::math::Helper<float>::areSame(test2.w, test.mW));

		rot = glm::quat(1.55f, 6.654f, 8.659f, 4.5648f);
		pos = glm::vec3(2.05f, 8.156f, 5.052f);
		scale = glm::vec3(0.54f, 1.795f, 1.3468f);

		rot = glm::normalize(rot);
		test2 = glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot)) * vec3;
		test = trans3 * maj::math::Vec4<float>(3.54f, 2.561f, 4.546f, 1.0f);
		IsTrue(maj::math::Helper<float>::areSame(test2.x, test.mX));
		IsTrue(maj::math::Helper<float>::areSame(test2.y, test.mY));
		IsTrue(maj::math::Helper<float>::areSame(test2.z, test.mZ));
		IsTrue(maj::math::Helper<float>::areSame(test2.w, test.mW));

		rot = glm::normalize(rot);
		test2 = glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot)) * vec1;
		test = trans3 * maj::math::Vec4<float>(0.0f, 0.0f, 0.0f, 1.0f);
		IsTrue(maj::math::Helper<float>::areSame(test2.x, test.mX));
		IsTrue(maj::math::Helper<float>::areSame(test2.y, test.mY));
		IsTrue(maj::math::Helper<float>::areSame(test2.z, test.mZ));
		IsTrue(maj::math::Helper<float>::areSame(test2.w, test.mW));

		rot = glm::normalize(rot);
		test2 = glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot)) * vec4;
		test = trans3 * maj::math::Vec4<float>(3.54f, 2.561f, 4.546f, 0.0f);
		AreEqual(test2.x, test.mX, 1E-5);
		AreEqual(test2.y, test.mY, 1E-5);
		AreEqual(test2.z, test.mZ, 1E-5);
		AreEqual(test2.w, test.mW, 1E-5);
	}
	
	TEST_METHOD("* Transform")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(1.0f, 2.0f, 3.0f),
			maj::math::Quaternion<float>(1.0f, 5.0f, 6.0f, 8.0f),
			maj::math::Vec3<float>(1.0f,1.0f,1.0f) };

		maj::math::Transform<float> trans3 { maj::math::Vec3<float>(2.05f, 8.15f, 5.52f),
			maj::math::Quaternion<float>(1.55f, 6.65f, 8.65f, 4.56f),
			maj::math::Vec3<float>(0.54f, 1.79f, 1.34f) };

		glm::quat rot(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 pos(0.0f, 0.0f, 0.0f);
		glm::vec3 scale(1.0f, 1.0f, 1.0f);

		rot = glm::normalize(rot);
		glm::mat4 test2 = glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot));
		maj::math::Transform<float> test = trans * trans;
		maj::math::Mat4<float> mat { test.matrix() };
		
		IsTrue(maj::math::Helper<float>::areSame(mat[0u], test2[0u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[1u], test2[0u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[2u], test2[0u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[3u], test2[0u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[4u], test2[1u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[5u], test2[1u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[6u], test2[1u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[7u], test2[1u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[8u], test2[2u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[9u], test2[2u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[10u], test2[2u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[11u], test2[2u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[12u], test2[3u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[13u], test2[3u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[14u], test2[3u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[15u], test2[3u][3u]));

		rot = glm::quat(1.0f, 5.0f, 6.0f, 8.0f);
		pos = glm::vec3(1.0f, 2.0f, 3.0f);
		scale = glm::vec3(1.0f, 1.0f, 1.0f);

		glm::quat test2quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 test2scale = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 test2Trans = glm::vec3(0.0f, 0.0f, 0.0f);

		test2quat = glm::normalize(test2quat);
		rot = glm::normalize(rot);
		glm::mat4 test1 = glm::translate(test2Trans) * glm::rotate(glm::scale(test2scale), glm::angle(test2quat), glm::axis(test2quat));
		test2 = test1 * glm::translate(pos) * glm::rotate(glm::scale(scale), glm::angle(rot), glm::axis(rot));
		test = trans2 * trans;
		mat = test.matrix();

		IsTrue(maj::math::Helper<float>::areSame(mat[0u], test2[0u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[1u], test2[0u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[2u], test2[0u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[3u], test2[0u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[4u], test2[1u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[5u], test2[1u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[6u], test2[1u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[7u], test2[1u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[8u], test2[2u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[9u], test2[2u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[10u], test2[2u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[11u], test2[2u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[12u], test2[3u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[13u], test2[3u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[14u], test2[3u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[15u], test2[3u][3u]));

		test2quat = glm::quat(1.55f, 6.65f, 8.65f, 4.56f);
		test2scale = glm::vec3(0.54f, 1.79f, 1.34f);
		test2Trans = glm::vec3(2.05f, 8.15f, 5.52f); 

		rot = glm::quat(1.0f, 5.0f, 6.0f, 8.0f);
		pos = glm::vec3(1.0f, 2.0f, 3.0f); 
		scale = glm::vec3(1.0f, 1.0f, 1.0f);

		test2scale *= scale;
		test2quat = glm::normalize(rot * test2quat);
		test2 = glm::translate(pos + glm::normalize(rot) * (scale * test2Trans)) * glm::rotate(glm::scale(test2scale), glm::angle(test2quat), glm::axis(test2quat));

		test = trans2 * trans3;
		mat = test.matrix();

		IsTrue(maj::math::Helper<float>::areSame(mat[0u], test2[0u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[1u], test2[0u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[2u], test2[0u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[3u], test2[0u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[4u], test2[1u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[5u], test2[1u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[6u], test2[1u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[7u], test2[1u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[8u], test2[2u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[9u], test2[2u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[10u], test2[2u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[11u], test2[2u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[12u], test2[3u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[13u], test2[3u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[14u], test2[3u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[15u], test2[3u][3u]));
	}

	TEST_METHOD("*= Transform")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(1.0f, 2.0f, 3.0f),
			maj::math::Quaternion<float>(1.0f, 5.0f, 6.0f, 8.0f),
			maj::math::Vec3<float>(1.0f,1.0f,1.0f) };

		maj::math::Transform<float> trans3 { maj::math::Vec3<float>(2.05f, 8.15f, 5.52f),
			maj::math::Quaternion<float>(1.55f, 6.65f, 7.65f, 4.56f),
			maj::math::Vec3<float>(6.54f, 3.79f, 5.34f) };

		glm::quat rot = glm::quat(1.55f, 6.65f, 7.65f, 4.56f);
		glm::vec3 scale = glm::vec3(6.54f, 3.79f, 5.34f);
		glm::vec3 pos = glm::vec3(2.05f, 8.15f, 5.52f);

		glm::quat test2quat = glm::quat(1.0f, 5.0f, 6.0f, 8.0f); 
		glm::vec3 test2scale = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 test2Trans = glm::vec3(1.0f, 2.0f, 3.0f); 

		test2scale *= scale;
		test2quat = glm::normalize(rot * test2quat);
		glm::mat4 test2 = glm::translate(pos + glm::normalize(rot) * (scale * test2Trans)) * glm::rotate(glm::scale(test2scale), glm::angle(test2quat), glm::axis(test2quat));

		trans3 *= trans2;
		maj::math::Mat4<float> mat { trans3.matrix() };

		IsTrue(maj::math::Helper<float>::areSame(mat[0u], test2[0u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[1u], test2[0u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[2u], test2[0u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[3u], test2[0u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[4u], test2[1u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[5u], test2[1u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[6u], test2[1u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[7u], test2[1u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[8u], test2[2u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[9u], test2[2u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[10u], test2[2u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[11u], test2[2u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[12u], test2[3u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[13u], test2[3u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[14u], test2[3u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[15u], test2[3u][3u]));

		test2quat = glm::quat(1.0f, 5.0f, 6.0f, 8.0f);
		test2Trans = glm::vec3(1.0f, 2.0f, 3.0f);
		test2scale = glm::vec3(1.0f, 1.0f, 1.0f);

		rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
		pos = glm::vec3(0.0f, 0.0f, 0.0f);

		test2scale *= scale;
		test2quat = glm::normalize(rot * test2quat);
		test2 = glm::translate(pos + glm::normalize(rot) * (scale * test2Trans)) * glm::rotate(glm::scale(test2scale), glm::angle(test2quat), glm::axis(test2quat));

		trans2 *= trans;
		mat = trans2.matrix();

		IsTrue(maj::math::Helper<float>::areSame(mat[0u], test2[0u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[1u], test2[0u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[2u], test2[0u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[3u], test2[0u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[4u], test2[1u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[5u], test2[1u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[6u], test2[1u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[7u], test2[1u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[8u], test2[2u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[9u], test2[2u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[10u], test2[2u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[11u], test2[2u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[12u], test2[3u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[13u], test2[3u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[14u], test2[3u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[15u], test2[3u][3u]));

		test2quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		test2Trans = glm::vec3(0.0f, 0.0f, 0.0f);
		test2scale = glm::vec3(1.0f, 1.0f, 1.0f);

		test2scale *= scale;
		test2quat = glm::normalize(rot * test2quat);
		test2 = glm::translate(pos + glm::normalize(rot) * (scale * test2Trans)) * glm::rotate(glm::scale(test2scale), glm::angle(test2quat), glm::axis(test2quat));

		trans *= trans;
		mat = trans.matrix();

		IsTrue(maj::math::Helper<float>::areSame(mat[0u], test2[0u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[1u], test2[0u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[2u], test2[0u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[3u], test2[0u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[4u], test2[1u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[5u], test2[1u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[6u], test2[1u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[7u], test2[1u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[8u], test2[2u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[9u], test2[2u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[10u], test2[2u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[11u], test2[2u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[12u], test2[3u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[13u], test2[3u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[14u], test2[3u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(mat[15u], test2[3u][3u]));
	}

	TEST_METHOD("rotateAround")
	{
		maj::math::Transform<float> trans;
		maj::math::Transform<float> trans2 { maj::math::Vec3<float>(1.0f, 2.0f, 3.0f),
			maj::math::Quaternion<float>(1.0f, 5.0f, 6.0f, 8.0f),
			maj::math::Vec3<float>(1.0f,1.0f,1.0f) };

		maj::math::Transform<float> trans3 { maj::math::Vec3<float>(2.05f, 8.15f, 5.52f),
			maj::math::Quaternion<float>(1.55f, 6.65f, 7.65f, 4.56f),
			maj::math::Vec3<float>(6.54f, 3.79f, 5.34f) };

		glm::quat rot = glm::quat(1.55f, 6.65f, 7.65f, 4.56f);
		glm::vec3 pos = glm::vec3(2.05f, 8.15f, 5.52f);
		glm::vec3 scale = glm::vec3(6.54f, 3.79f, 5.34f);

		glm::quat rot1 = glm::quat(1.0f, 5.0f, 6.0f, 8.0f);
		glm::vec3 pos1 = glm::vec3(1.0f, 2.0f, 3.0f);
		glm::vec3 scale1 = glm::vec3(1.0f, 1.0f, 1.0f);

		glm::quat rot2 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 pos2 = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale2 = glm::vec3(1.0f, 1.0f, 1.0f);


		trans.rotateAround(maj::math::Vec3<float>(0.0f, 0.0f, 0.0f), 45.0f);

		glm::vec3 direction = pos2 - glm::vec3(0.0f, 0.0f, 0.0f);
		glm::quat rotation = glm::angleAxis(45.0f * DEG2RAD, glm::vec3(0.0f, 1.0f, 0.0f));
		direction = rotation * direction;
		glm::vec3 newPos = glm::vec3(0.0f, 0.0f, 0.0f) + direction;

		AreEqual(newPos.x, trans.position().mX, FLT_EPSILON);
		AreEqual(newPos.y, trans.position().mY, FLT_EPSILON);
		AreEqual(newPos.z, trans.position().mZ, FLT_EPSILON);


		trans2.rotateAround(maj::math::Vec3<float>(0.0f, 0.0f, 0.0f), 180.f);

		direction = pos1 - glm::vec3(0.0f, 0.0f, 0.0f);
		rotation = glm::angleAxis(180.f * DEG2RAD, glm::vec3(0.0f, 1.0f, 0.0f));
		direction = rotation * direction;
		newPos = glm::vec3(0.0f, 0.0f, 0.0f) + direction;

		AreEqual(newPos.x, trans2.position().mX, FLT_EPSILON);
		AreEqual(newPos.y, trans2.position().mY, FLT_EPSILON);
		AreEqual(newPos.z, trans2.position().mZ, FLT_EPSILON);

		trans3.rotateAround(maj::math::Vec3<float>(PI, PI, PI), 1.0f, maj::math::Vec3<float>::skForward);

		direction = pos - glm::vec3(PI, PI, PI);
		rotation = glm::angleAxis(1.0f * DEG2RAD, glm::vec3(0.0f, 0.0f, 1.0f));
		direction = rotation * direction;
		newPos = glm::vec3(PI, PI, PI) + direction;

		AreEqual(newPos.x, trans3.position().mX, FLT_EPSILON);
		AreEqual(newPos.y, trans3.position().mY, FLT_EPSILON);
		AreEqual(newPos.z, trans3.position().mZ, FLT_EPSILON);
	}

	TEST_METHOD("lerp")
	{
		maj::math::Transform<float> trans0({ 0.f, 0.f, 0.f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 2.f, 2.f, 2.f });
		maj::math::Transform<float> trans1({ 10.0f, 11.0f, 12.f }, { 1.f , 0.f, 0.f, 0.f }, { 0.5f, 0.5f, 0.5f });
		maj::math::Transform<float> trans2({ 52.0f, 1.0f, 33.f }, { 2.0f, -2.4589f, 47.5289627f, 0.05f }, { 1.f, 1.f, 1.f });
		maj::math::Transform<float> trans3({ 10.5f, 11.25f, 12.0025f }, { PI, 1.0f, 5.0f, 4.258f }, { 1.f, 1.f, 1.f });
		maj::math::Transform<float> trans4({ 52.007f, 1.69f, 42.4235f }, { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() }, { 1.f, 1.f, 1.f });

		glm::quat rot = glm::quat(0.f, 0.f, 0.f, 0.f);
		glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 scale = glm::vec3(2.0f, 2.0f, 2.0f);

		glm::quat rot1 = glm::quat(1.f, 0.f, 0.f, 0.f);
		glm::vec3 pos1 = glm::vec3(10.0f, 11.0f, 12.0f);
		glm::vec3 scale1 = glm::vec3(0.5f, 0.5f, 0.5f);

		glm::quat rot2 = glm::quat(2.0f, -2.4589f, 47.5289627f, 0.05f);
		glm::vec3 pos2 = glm::vec3(52.0f, 1.0f, 33.0f);
		glm::vec3 scale2 = glm::vec3(1.0f, 1.0f, 1.0f);

		glm::quat rot3 = glm::quat(PI, 1.0f, 5.0f, 4.258f);
		glm::vec3 pos3 = glm::vec3(10.5f, 11.25f, 12.0025f);
		glm::vec3 scale3 = glm::vec3(1.0f, 1.0f, 1.0f);

		glm::quat rot4 = glm::quat(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		glm::vec3 pos4 = glm::vec3(52.007f, 1.69f, 42.4235f);
		glm::vec3 scale4 = glm::vec3(1.0f, 1.0f, 1.0f);

		maj::math::Transform<float> test = maj::math::Transform<float>::lerp(trans1, trans2, 0.5f);
		glm::vec3 testGlm = glm::lerp(pos1, pos2, 0.5f);
		AreEqual(testGlm.x, test.position().mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.position().mY, FLT_EPSILON);
		AreEqual(testGlm.z, test.position().mZ, FLT_EPSILON);
		glm::quat g{ glm::slerp(glm::normalize(rot1), glm::normalize(rot2), 0.5f) };
		AreEqual(test.rotation().mW, g.w, FLT_EPSILON);
		AreEqual(test.rotation().mX, g.x, FLT_EPSILON);
		AreEqual(test.rotation().mY, g.y, FLT_EPSILON);
		AreEqual(test.rotation().mZ, g.z, FLT_EPSILON);
		testGlm = glm::lerp(scale1, scale2, 0.5f);
		AreEqual(testGlm.x, test.scale().mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.scale().mY, FLT_EPSILON);
		AreEqual(testGlm.z, test.scale().mZ, FLT_EPSILON);

		test = maj::math::Transform<float>::lerp(trans3, trans4, 0.5f);	// Same with glm, except 1e-5 precision
		testGlm = glm::lerp(pos3, pos4, 0.5f);
		AreEqual(testGlm.x, test.position().mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.position().mY, FLT_EPSILON);
		IsTrue(maj::math::Helper<float>::areSame(testGlm.z, test.position().mZ));
		g = glm::normalize(glm::slerp(glm::normalize(rot3), glm::normalize(rot4), 0.5f));
		AreEqual(test.rotation().mW, g.w, FLT_EPSILON);
		AreEqual(test.rotation().mX, g.x, FLT_EPSILON);
		AreEqual(test.rotation().mY, g.y, FLT_EPSILON);
		AreEqual(test.rotation().mZ, g.z, FLT_EPSILON);
		testGlm = glm::lerp(scale3, scale4, 0.5f);
		AreEqual(testGlm.x, test.scale().mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.scale().mY, FLT_EPSILON);
		AreEqual(testGlm.z, test.scale().mZ, FLT_EPSILON);

		test = maj::math::Transform<float>::lerp(trans3, trans4, 0.0f);
		testGlm = glm::lerp(pos3, pos4, 0.0f);
		AreEqual(testGlm.x, test.position().mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.position().mY, FLT_EPSILON);
		IsTrue(maj::math::Helper<float>::areSame(testGlm.z, test.position().mZ));
		g = glm::slerp(glm::normalize(rot3), glm::normalize(rot4), 0.0f);
		AreEqual(test.rotation().mW, g.w, FLT_EPSILON);
		AreEqual(test.rotation().mX, g.x, FLT_EPSILON);
		AreEqual(test.rotation().mY, g.y, FLT_EPSILON);
		AreEqual(test.rotation().mZ, g.z, FLT_EPSILON);
		testGlm = glm::lerp(scale3, scale4, 0.f);
		AreEqual(testGlm.x, test.scale().mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.scale().mY, FLT_EPSILON);
		AreEqual(testGlm.z, test.scale().mZ, FLT_EPSILON);

		test = maj::math::Transform<float>::lerp(trans1, trans2, 1.0f);
		testGlm = glm::lerp(pos1, pos2, 1.0f);
		AreEqual(testGlm.x, test.position().mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.position().mY, FLT_EPSILON);
		AreEqual(testGlm.z, test.position().mZ, FLT_EPSILON);
		g = glm::slerp(glm::normalize(rot1), glm::normalize(rot2), 1.0f);
		AreEqual(test.rotation().mW, g.w, FLT_EPSILON);
		AreEqual(test.rotation().mX, g.x, FLT_EPSILON);
		AreEqual(test.rotation().mY, g.y, FLT_EPSILON);
		AreEqual(test.rotation().mZ, g.z, FLT_EPSILON);
		testGlm = glm::lerp(scale1, scale2, 1.f);
		AreEqual(testGlm.x, test.scale().mX, FLT_EPSILON);
		AreEqual(testGlm.y, test.scale().mY, FLT_EPSILON);
		AreEqual(testGlm.z, test.scale().mZ, FLT_EPSILON);

	}*/
};