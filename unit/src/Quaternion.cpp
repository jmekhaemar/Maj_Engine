#include "Test.h"

#include <Quaternion.h>
#include <Mat4.h>

#define GLM_FORCE_LEFT_HANDED
#include <glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <gtx/quaternion.hpp>
#include <gtc/matrix_transform.hpp>
#include <vec3.hpp>

TEST_CLASS("Quaternion")
{
	TEST_METHOD("constructor")
	{
		maj::math::Quaternion<float> q1;
		IsTrue(maj::math::Helper<float>::areSame(q1.mW, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(q1.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(q1.mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(q1.mZ, 0.0f));

		maj::math::Quaternion<float> q2 { 2.0f, 2.4589f, 47.5289627f, 0.05f };
		IsTrue(maj::math::Helper<float>::areSame(q2.mW, 2.0f));
		IsTrue(maj::math::Helper<float>::areSame(q2.mX, 2.4589f));
		IsTrue(maj::math::Helper<float>::areSame(q2.mY, 47.5289627f));
		IsTrue(maj::math::Helper<float>::areSame(q2.mZ, 0.05f));

		maj::math::Quaternion<float> q3 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		IsTrue(maj::math::Helper<float>::areSame(q3.mW, std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(q3.mX, std::numeric_limits<float>::max()));
		IsTrue(maj::math::Helper<float>::areSame(q3.mY, std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(q3.mZ, std::numeric_limits<float>::max()));

		maj::math::Quaternion<float> q4 { maj::math::Quaternion<float>::skIdentity };
		IsTrue(maj::math::Helper<float>::areSame(q4.mW, 1.0f));
		IsTrue(maj::math::Helper<float>::areSame(q4.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(q4.mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(q4.mZ, 0.0f));

		glm::quat g { glm::angleAxis(90.f * DEG2RAD, glm::normalize(glm::vec3 { 1.0f, 5.0f, 4.258f })) };
		maj::math::Quaternion<float> q5 { 90.f, { 1.0f, 5.0f, 4.258f } };

		IsTrue(maj::math::Helper<float>::areSame(q5.mW, g.w));
		IsTrue(maj::math::Helper<float>::areSame(q5.mX, g.x));
		IsTrue(maj::math::Helper<float>::areSame(q5.mY, g.y));
		IsTrue(maj::math::Helper<float>::areSame(q5.mZ, g.z));
	}

	TEST_METHOD("modulus")
	{
		maj::math::Quaternion<float> q1;
		glm::quat g1 { 1.f, {} };
		AreEqual(q1.modulus(), glm::length(g1), FLT_EPSILON);

		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		AreEqual(q2.modulus(), glm::length(g2), FLT_EPSILON);

		maj::math::Quaternion<float> q3 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		IsTrue(std::isinf(q3.modulus()));

		maj::math::Quaternion<float> q4 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g4 { 0.f, 0.f, 0.f, 0.f };
		AreEqual(q4.modulus(), glm::length(g4), FLT_EPSILON);

		maj::math::Quaternion<float> q5 { 90.f, { 1.0f, 5.0f, 4.258f } };
		glm::quat g5 { glm::angleAxis(90.f * DEG2RAD, glm::normalize(glm::vec3 { 1.0f, 5.0f, 4.258f })) };
		AreEqual(q5.modulus(), glm::length(g5), FLT_EPSILON);
	}

	TEST_METHOD("sqr modulus")
	{
		maj::math::Quaternion<float> q1;
		glm::quat g1 { 1.f, {} };
		AreEqual(q1.sqrModulus(), glm::length2(g1), FLT_EPSILON);

		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		AreEqual(q2.sqrModulus(), glm::length2(g2), FLT_EPSILON);

		maj::math::Quaternion<float> q3 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		IsTrue(std::isinf(q3.sqrModulus()));

		maj::math::Quaternion<float> q4 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g4 { 0.f, 0.f, 0.f, 0.f };
		AreEqual(q4.sqrModulus(), glm::length2(g4), FLT_EPSILON);

		maj::math::Quaternion<float> q5 { PI * RAD2DEG, { 1.0f, 5.0f, 4.258f } };
		glm::quat g5 { glm::angleAxis(PI, glm::normalize(glm::vec3 { 1.0f, 5.0f, 4.258f })) };
		AreEqual(q5.sqrModulus(), glm::length2(g5), FLT_EPSILON);
	}

	TEST_METHOD("conjugate")
	{
		maj::math::Quaternion<float> q1;
		q1.conjugate();
		glm::quat g1 { glm::conjugate(glm::quat { 1.f, {} }) };

		AreEqual(q1.mW, g1.w, FLT_EPSILON);
		AreEqual(q1.mX, g1.x, FLT_EPSILON);
		AreEqual(q1.mY, g1.y, FLT_EPSILON);
		AreEqual(q1.mZ, g1.z, FLT_EPSILON);

		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		q2.conjugate();
		glm::quat g2 { glm::conjugate(glm::quat { 2.0f, -2.4589f, 47.5289627f, 0.05f }) };

		AreEqual(q2.mW, g2.w, FLT_EPSILON);
		AreEqual(q2.mX, g2.x, FLT_EPSILON);
		AreEqual(q2.mY, g2.y, FLT_EPSILON);
		AreEqual(q2.mZ, g2.z, FLT_EPSILON);

		maj::math::Quaternion<float> q3 { 0.f, 0.f, 0.f, 0.f };
		q3.conjugate();
		glm::quat g3 { glm::conjugate(glm::quat { 0.f, 0.f, 0.f, 0.f }) };

		AreEqual(q3.mW, g3.w, FLT_EPSILON);
		AreEqual(q3.mX, g3.x, FLT_EPSILON);
		AreEqual(q3.mY, g3.y, FLT_EPSILON);
		AreEqual(q3.mZ, g3.z, FLT_EPSILON);
	}

	TEST_METHOD("conjugated")
	{
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> cq1 { q1.conjugated() };
		glm::quat g1 { glm::conjugate(glm::quat { 1.f, {} }) };

		AreEqual(cq1.mW, g1.w, FLT_EPSILON);
		AreEqual(cq1.mX, g1.x, FLT_EPSILON);
		AreEqual(cq1.mY, g1.y, FLT_EPSILON);
		AreEqual(cq1.mZ, g1.z, FLT_EPSILON);

		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> cq2 { q2.conjugated() };
		glm::quat g2 { glm::conjugate(glm::quat { 2.0f, -2.4589f, 47.5289627f, 0.05f }) };

		AreEqual(cq2.mW, g2.w, FLT_EPSILON);
		AreEqual(cq2.mX, g2.x, FLT_EPSILON);
		AreEqual(cq2.mY, g2.y, FLT_EPSILON);
		AreEqual(cq2.mZ, g2.z, FLT_EPSILON);

		maj::math::Quaternion<float> q3 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> cq3 { q3.conjugated() };
		glm::quat g3 { glm::conjugate(glm::quat { 0.f, 0.f, 0.f, 0.f }) };

		AreEqual(cq3.mW, g3.w, FLT_EPSILON);
		AreEqual(cq3.mX, g3.x, FLT_EPSILON);
		AreEqual(cq3.mY, g3.y, FLT_EPSILON);
		AreEqual(cq3.mZ, g3.z, FLT_EPSILON);
	}

	TEST_METHOD("inverse")
	{
		maj::math::Quaternion<float> q1;
		q1.inverse();
		glm::quat g1 { glm::inverse(glm::quat { 1.f, {} }) };

		AreEqual(q1.mW, g1.w, FLT_EPSILON);
		AreEqual(q1.mX, g1.x, FLT_EPSILON);
		AreEqual(q1.mY, g1.y, FLT_EPSILON);
		AreEqual(q1.mZ, g1.z, FLT_EPSILON);

		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		q2.inverse();
		glm::quat g2 { glm::inverse(glm::quat { 2.0f, -2.4589f, 47.5289627f, 0.05f }) };

		AreEqual(q2.mW, g2.w, FLT_EPSILON);
		AreEqual(q2.mX, g2.x, FLT_EPSILON);
		AreEqual(q2.mY, g2.y, FLT_EPSILON);
		AreEqual(q2.mZ, g2.z, FLT_EPSILON);

		maj::math::Quaternion<float> q3 { 0.f, 0.f, 0.f, 0.f };
		q3.inverse();

		AreEqual(q3.mW, 0.0f, FLT_EPSILON);
		AreEqual(q3.mX, 0.0f, FLT_EPSILON);
		AreEqual(q3.mY, 0.0f, FLT_EPSILON);
		AreEqual(q3.mZ, 0.0f, FLT_EPSILON);

		maj::math::Quaternion<float> q4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		q4.inverse();
		glm::quat g4 { glm::inverse(glm::quat { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() }) };

		AreEqual(q4.mW, g4.w, FLT_EPSILON);
		AreEqual(q4.mX, g4.x, FLT_EPSILON);
		AreEqual(q4.mY, g4.y, FLT_EPSILON);
		AreEqual(q4.mZ, g4.z, FLT_EPSILON);
	}

	TEST_METHOD("inversed")
	{
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> iq1 { q1.inversed() };
		glm::quat g1 { glm::inverse(glm::quat { 1.f, {} }) };

		AreEqual(iq1.mW, g1.w, FLT_EPSILON);
		AreEqual(iq1.mX, g1.x, FLT_EPSILON);
		AreEqual(iq1.mY, g1.y, FLT_EPSILON);
		AreEqual(iq1.mZ, g1.z, FLT_EPSILON);

		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> iq2 { q2.inversed() };
		glm::quat g2 { glm::inverse(glm::quat { 2.0f, -2.4589f, 47.5289627f, 0.05f }) };

		AreEqual(iq2.mW, g2.w, FLT_EPSILON);
		AreEqual(iq2.mX, g2.x, FLT_EPSILON);
		AreEqual(iq2.mY, g2.y, FLT_EPSILON);
		AreEqual(iq2.mZ, g2.z, FLT_EPSILON);

		maj::math::Quaternion<float> q3 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> iq3 { q3.inversed() };

		AreEqual(iq3.mW, 1.0f, FLT_EPSILON);
		AreEqual(iq3.mX, 0.0f, FLT_EPSILON);
		AreEqual(iq3.mY, 0.0f, FLT_EPSILON);
		AreEqual(iq3.mZ, 0.0f, FLT_EPSILON);

		maj::math::Quaternion<float> q4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		maj::math::Quaternion<float> iq4 { q4.inversed() };
		glm::quat g4 { glm::inverse(glm::quat { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() }) };

		AreEqual(iq4.mW, g4.w, FLT_EPSILON);
		AreEqual(iq4.mX, g4.x, FLT_EPSILON);
		AreEqual(iq4.mY, g4.y, FLT_EPSILON);
		AreEqual(iq4.mZ, g4.z, FLT_EPSILON);
	}

	TEST_METHOD("normalize")
	{
		maj::math::Quaternion<float> q1;
		q1.normalize();
		glm::quat g1 { glm::normalize(glm::quat { 1.f, {} }) };

		AreEqual(q1.mW, g1.w, FLT_EPSILON);
		AreEqual(q1.mX, g1.x, FLT_EPSILON);
		AreEqual(q1.mY, g1.y, FLT_EPSILON);
		AreEqual(q1.mZ, g1.z, FLT_EPSILON);

		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		q2.normalize();
		glm::quat g2 { glm::normalize(glm::quat { 2.0f, -2.4589f, 47.5289627f, 0.05f }) };

		AreEqual(q2.mW, g2.w, FLT_EPSILON);
		AreEqual(q2.mX, g2.x, FLT_EPSILON);
		AreEqual(q2.mY, g2.y, FLT_EPSILON);
		AreEqual(q2.mZ, g2.z, FLT_EPSILON);

		maj::math::Quaternion<float> q3 { 0.f, 0.f, 0.f, 0.f };
		q3.normalize();

		AreEqual(q3.mW, 0.0f, FLT_EPSILON);
		AreEqual(q3.mX, 0.0f, FLT_EPSILON);
		AreEqual(q3.mY, 0.0f, FLT_EPSILON);
		AreEqual(q3.mZ, 0.0f, FLT_EPSILON);

		maj::math::Quaternion<float> q4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		q4.normalize();
		glm::quat g4 { glm::normalize(glm::quat { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() }) };

		AreEqual(q4.mW, g4.w, FLT_EPSILON);
		AreEqual(q4.mX, g4.x, FLT_EPSILON);
		AreEqual(q4.mY, g4.y, FLT_EPSILON);
		AreEqual(q4.mZ, g4.z, FLT_EPSILON);
	}

	TEST_METHOD("normalized")
	{
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> iq1 { q1.normalized() };
		glm::quat g1 { glm::normalize(glm::quat { 1.f, {} }) };

		AreEqual(iq1.mW, g1.w, FLT_EPSILON);
		AreEqual(iq1.mX, g1.x, FLT_EPSILON);
		AreEqual(iq1.mY, g1.y, FLT_EPSILON);
		AreEqual(iq1.mZ, g1.z, FLT_EPSILON);

		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> iq2 { q2.normalized() };
		glm::quat g2 { glm::normalize(glm::quat { 2.0f, -2.4589f, 47.5289627f, 0.05f }) };

		AreEqual(iq2.mW, g2.w, FLT_EPSILON);
		AreEqual(iq2.mX, g2.x, FLT_EPSILON);
		AreEqual(iq2.mY, g2.y, FLT_EPSILON);
		AreEqual(iq2.mZ, g2.z, FLT_EPSILON);

		maj::math::Quaternion<float> q3 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> iq3 { q3.normalized() };

		AreEqual(iq3.mW, 1.0f, FLT_EPSILON);
		AreEqual(iq3.mX, 0.0f, FLT_EPSILON);
		AreEqual(iq3.mY, 0.0f, FLT_EPSILON);
		AreEqual(iq3.mZ, 0.0f, FLT_EPSILON);

		maj::math::Quaternion<float> q4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		maj::math::Quaternion<float> iq4 { q4.normalized() };
		glm::quat g4 { glm::normalize(glm::quat { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() }) };

		AreEqual(iq4.mW, g4.w, FLT_EPSILON);
		AreEqual(iq4.mX, g4.x, FLT_EPSILON);
		AreEqual(iq4.mY, g4.y, FLT_EPSILON);
		AreEqual(iq4.mZ, g4.z, FLT_EPSILON);
	}

	TEST_METHOD("dot")
	{
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> q3 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q4 { PI * RAD2DEG, { 1.0f, 5.0f, 4.258f } };
		maj::math::Quaternion<float> q5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		maj::math::Quaternion<float> q6 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

		glm::quat g1 { 1.f, {} };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g3 {};
		glm::quat g4 { glm::angleAxis(PI, glm::normalize(glm::vec3 { 1.0f, 5.0f, 4.258f })) };
		glm::quat g5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		glm::quat g6 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

		// q1.q1
		AreEqual(q1.dot(q1), glm::dot(g1, g1), FLT_EPSILON);

		// q1.q3
		AreEqual(q1.dot(q3), 0.0f, FLT_EPSILON);

		// q3.q3
		AreEqual(q3.dot(q3), 0.0f, FLT_EPSILON);

		// q2.q2
		AreEqual(q2.dot(q2), glm::dot(g2, g2), FLT_EPSILON);

		// q1.q2
		AreEqual(q1.dot(q2), glm::dot(g1, g2), FLT_EPSILON);

		// q4.q2
		AreEqual(q4.dot(q2), glm::dot(g4, g2), FLT_EPSILON);

		// q5.q5
		IsTrue(std::isinf(q5.dot(q5)));

		// q6.q5
		IsTrue(std::isnan(q6.dot(q5)));
	}

	TEST_METHOD("clamp")
	{
		maj::math::Quaternion<float> q1 { 0.f, 0.f, 0.f, 0.0500000000001f };
		maj::math::Quaternion<float> q2 { 2.0f, 2.4588777777f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> q3 { PI, 1.0f, 5.0f, 4.258f };
		maj::math::Quaternion<float> q4 { 2.55f, 2.4589f, 47.528963f, 4.259f };
		maj::math::Quaternion<float> q5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
		maj::math::Quaternion<float> q6 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
		maj::math::Quaternion<float> q7 { std::numeric_limits<float>::lowest(), 0.0f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };

		// q2 < q3 < q4
		maj::math::Quaternion<float> p1 { q3.clamp(q2, q4) };
		AreEqual(2.55f, p1.mW, FLT_EPSILON);
		AreEqual(2.4588777777f, p1.mX, FLT_EPSILON);
		AreEqual(47.5289627f, p1.mY, FLT_EPSILON);
		AreEqual(4.258f, p1.mZ, FLT_EPSILON);

		// q2 < q2 < q4
		maj::math::Quaternion<float> p2 { q2.clamp(q2, q4) };
		AreEqual(2.0f, p2.mW, FLT_EPSILON);
		AreEqual(2.4588777777f, p2.mX, FLT_EPSILON);
		AreEqual(47.5289627f, p2.mY, FLT_EPSILON);
		AreEqual(0.05f, p2.mZ, FLT_EPSILON);

		// 
		maj::math::Quaternion<float> p3 { q1.clamp({ -2.0f, 2.4588766666f, -47.5289627f, -0.05f }, q2) };
		AreEqual(0.0f, p3.mW, FLT_EPSILON);
		AreEqual(2.4588766666f, p3.mX, FLT_EPSILON);
		AreEqual(0.0f, p3.mY, FLT_EPSILON);
		AreEqual(0.05f, p3.mZ, FLT_EPSILON);

		// q5 < q7 < q6
		maj::math::Quaternion<float> p4 { q7.clamp(q5, q6) };
		AreEqual(std::numeric_limits<float>::lowest(), p4.mW, FLT_EPSILON);
		AreEqual(0.0f, p4.mX, FLT_EPSILON);
		AreEqual(std::numeric_limits<float>::lowest(), p4.mY, FLT_EPSILON);
		AreEqual(std::numeric_limits<float>::max(), p4.mZ, FLT_EPSILON);
	}

	TEST_METHOD("toEuler")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { PI * 2.f, 1.0f, 2.5f, 0.258f };
		maj::math::Quaternion<float> q3 { 2.55f, 2.458f, 47.52f, 4.259f };
		maj::math::Quaternion<float> q4 { PI, -1.0f, 0.0f, 0.f };
		maj::math::Quaternion<float> q5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { PI * 2.f, 1.0f, 2.5f, 0.258f };
		glm::quat g3 { 2.55f, 2.458f, 47.52f, 4.259f };
		glm::quat g4 { PI, -1.0f, 0.0f, 0.f };
		glm::quat g5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };

		maj::math::Vec3<float> v { q0.toEuler() };
		glm::vec3 u { glm::eulerAngles(glm::normalize(g0)) * RAD2DEG };

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);

		v = q1.toEuler();
		u = glm::eulerAngles(glm::normalize(g1)) * RAD2DEG;

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);

		v = q2.toEuler();
		u = glm::eulerAngles(glm::normalize(g2)) * RAD2DEG;

		IsTrue(maj::math::Helper<float>::areSame(v.mX, u.x));
		IsTrue(maj::math::Helper<float>::areSame(v.mY, u.y));
		IsTrue(maj::math::Helper<float>::areSame(v.mZ, u.z));

		v = q3.toEuler();
		u = glm::eulerAngles(glm::normalize(g3)) * RAD2DEG;

		IsTrue(maj::math::Helper<float>::areSame(v.mX, u.x));
		IsTrue(maj::math::Helper<float>::areSame(v.mY, u.y));
		IsTrue(maj::math::Helper<float>::areSame(v.mZ, u.z));

		v = q4.toEuler();
		u = glm::eulerAngles(glm::normalize(g4)) * RAD2DEG;

		IsTrue(maj::math::Helper<float>::areSame(v.mX, u.x));
		IsTrue(maj::math::Helper<float>::areSame(v.mY, u.y));
		IsTrue(maj::math::Helper<float>::areSame(v.mZ, u.z));

		v = q5.toEuler();
		u = glm::eulerAngles(glm::normalize(g5)) * RAD2DEG;

		IsTrue(maj::math::Helper<float>::areSame(v.mX, u.x));
		IsTrue(maj::math::Helper<float>::areSame(v.mY, u.y));
		IsTrue(maj::math::Helper<float>::areSame(v.mZ, u.z));
	}

	TEST_METHOD("toAxis")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { PI * 2.f, 1.0f, 2.5f, 0.258f };
		maj::math::Quaternion<float> q3 { -0.5f, 0.0f, 2.5f, 0.258f };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { PI * 2.f, 1.0f, 2.5f, 0.258f };
		glm::quat g3 { -0.5f, 0.0f, 2.5f, 0.258f };

		maj::math::Vec3<float> v { q0.toAxis() };
		glm::vec3 u { glm::axis(g0) };

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);

		v = q1.toAxis();
		u = glm::axis(g1);

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);

		v = q2.toAxis();
		u = glm::axis(g2);

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);

		v = q3.toAxis();
		u = glm::axis(g3);

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);
	}

	TEST_METHOD("toAngle")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { PI * 2.f, 1.0f, 2.5f, 0.258f };
		maj::math::Quaternion<float> q3 { -0.5f, 0.0f, 2.5f, 0.258f };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { PI * 2.f, 1.0f, 2.5f, 0.258f };
		glm::quat g3 { -0.5f, 0.0f, 2.5f, 0.258f };

		float a { q0.toAngle() };
		float b { glm::angle(g0)* RAD2DEG };

		AreEqual(a, b, FLT_EPSILON);

		a = q1.toAngle();
		b = glm::angle(g1) * RAD2DEG;

		AreEqual(a, b, FLT_EPSILON);

		a = q2.toAngle();
		b = glm::angle(g2) * RAD2DEG;

		AreEqual(a, b, FLT_EPSILON);

		a = q3.toAngle();
		b = glm::angle(g3) * RAD2DEG;

		AreEqual(a, b, FLT_EPSILON);
	}

	TEST_METHOD("-")
	{
		maj::math::Quaternion<float> q1;
		q1 = -q1;

		IsTrue(maj::math::Helper<float>::areSame(q1.mW, -1.0f));
		IsTrue(maj::math::Helper<float>::areSame(q1.mX, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(q1.mY, 0.0f));
		IsTrue(maj::math::Helper<float>::areSame(q1.mZ, 0.0f));

		maj::math::Quaternion<float> q2 { 2.0f, 2.4589f, 47.5289627f, 0.05f };
		q2 = -q2;

		IsTrue(maj::math::Helper<float>::areSame(q2.mW, -2.0f));
		IsTrue(maj::math::Helper<float>::areSame(q2.mX, -2.4589f));
		IsTrue(maj::math::Helper<float>::areSame(q2.mY, -47.5289627f));
		IsTrue(maj::math::Helper<float>::areSame(q2.mZ, -0.05f));

		maj::math::Quaternion<float> q3 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		q3 = -q3;

		IsTrue(maj::math::Helper<float>::areSame(q3.mW, -std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(q3.mX, -std::numeric_limits<float>::max()));
		IsTrue(maj::math::Helper<float>::areSame(q3.mY, -std::numeric_limits<float>::lowest()));
		IsTrue(maj::math::Helper<float>::areSame(q3.mZ, -std::numeric_limits<float>::max()));

		maj::math::Quaternion<float> q4 { PI,  1.0f, 5.0f, 4.258f };
		q4 = -q4;

		IsTrue(maj::math::Helper<float>::areSame(q4.mW, -PI));
		IsTrue(maj::math::Helper<float>::areSame(q4.mX, -1.0f));
		IsTrue(maj::math::Helper<float>::areSame(q4.mY, -5.0f));
		IsTrue(maj::math::Helper<float>::areSame(q4.mZ, -4.258f));
	}

	TEST_METHOD("* float")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> p0 { q0 * PI };
		glm::quat g0 { glm::quat { 0.f, 0.f, 0.f, 0.f } *PI };

		AreEqual(p0.mW, g0.w, FLT_EPSILON);
		AreEqual(p0.mX, g0.x, FLT_EPSILON);
		AreEqual(p0.mY, g0.y, FLT_EPSILON);
		AreEqual(p0.mZ, g0.z, FLT_EPSILON);

		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> p1 { q1 * 2.f };
		glm::quat g1 { glm::quat { 1.f, {} } *2.f };

		AreEqual(p1.mW, g1.w, FLT_EPSILON);
		AreEqual(p1.mX, g1.x, FLT_EPSILON);
		AreEqual(p1.mY, g1.y, FLT_EPSILON);
		AreEqual(p1.mZ, g1.z, FLT_EPSILON);

		maj::math::Quaternion<float> q2 { 2.0f, 2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> p2 { q2 * 0.5689952f };
		glm::quat g2 { glm::quat { 2.0f, 2.4589f, 47.5289627f, 0.05f } *0.5689952f };

		AreEqual(p2.mW, g2.w, FLT_EPSILON);
		AreEqual(p2.mX, g2.x, FLT_EPSILON);
		AreEqual(p2.mY, g2.y, FLT_EPSILON);
		AreEqual(p2.mZ, g2.z, FLT_EPSILON);

		maj::math::Quaternion<float> q3 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		maj::math::Quaternion<float> p3 { q3 * 0.25f };
		glm::quat g3 { glm::quat { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() } *0.25f };

		AreEqual(p3.mW, g3.w, FLT_EPSILON);
		AreEqual(p3.mX, g3.x, FLT_EPSILON);
		AreEqual(p3.mY, g3.y, FLT_EPSILON);
		AreEqual(p3.mZ, g3.z, FLT_EPSILON);

		maj::math::Quaternion<float> q4 { PI, 1.0f, 5.0f, 4.258f };
		maj::math::Quaternion<float> p4 { q4 * std::numeric_limits<float>::max() };

		IsTrue(std::isinf(p4.mW));
		AreEqual(p4.mX, std::numeric_limits<float>::max(), FLT_EPSILON);
		IsTrue(std::isinf(p4.mY));
		IsTrue(std::isinf(p4.mZ));
	}

	TEST_METHOD("/ float")
	{
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> p1 { q1 / 2.f };
		glm::quat g1 { glm::quat { 1.f, {} } / 2.f };

		AreEqual(p1.mW, g1.w, FLT_EPSILON);
		AreEqual(p1.mX, g1.x, FLT_EPSILON);
		AreEqual(p1.mY, g1.y, FLT_EPSILON);
		AreEqual(p1.mZ, g1.z, FLT_EPSILON);

		maj::math::Quaternion<float> q2 { 2.0f, 2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> p2 { q2 / 0.5689952f };
		glm::quat g2 { glm::quat { 2.0f, 2.4589f, 47.5289627f, 0.05f } / 0.5689952f };

		AreEqual(p2.mW, g2.w, FLT_EPSILON);
		AreEqual(p2.mX, g2.x, FLT_EPSILON);
		AreEqual(p2.mY, g2.y, FLT_EPSILON);
		AreEqual(p2.mZ, g2.z, FLT_EPSILON);

		maj::math::Quaternion<float> q3 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		maj::math::Quaternion<float> p3 { q3 / 0.25f };
		IsTrue(std::isinf(p3.mW));
		IsTrue(std::isinf(p3.mX));
		IsTrue(std::isinf(p3.mY));
		IsTrue(std::isinf(p3.mZ));

		maj::math::Quaternion<float> q4 { PI, 1.0f, 5.0f, 4.258f };
		maj::math::Quaternion<float> p4 { q4 / std::numeric_limits<float>::max() };
		glm::quat g4 { glm::quat { PI, 1.0f, 5.0f, 4.258f } / std::numeric_limits<float>::max() };

		AreEqual(p4.mW, g4.w, FLT_EPSILON);
		AreEqual(p4.mX, g4.x, FLT_EPSILON);
		AreEqual(p4.mY, g4.y, FLT_EPSILON);
		AreEqual(p4.mZ, g4.z, FLT_EPSILON);

		maj::math::Quaternion<float> p5 { q4 / 0.f };

		AreEqual(p5.mW, 1.0f, FLT_EPSILON);
		AreEqual(p5.mX, 0.0f, FLT_EPSILON);
		AreEqual(p5.mY, 0.0f, FLT_EPSILON);
		AreEqual(p5.mZ, 0.0f, FLT_EPSILON);
	}

	TEST_METHOD("+ Quat")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> q3 { PI, 1.0f, 5.0f, 4.258f };
		maj::math::Quaternion<float> q4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		maj::math::Quaternion<float> q5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g3 { PI, 1.0f, 5.0f, 4.258f };
		glm::quat g4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		glm::quat g5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

		// q0 + q0
		maj::math::Quaternion<float> p { q0 + q0 };
		glm::quat g { g0 + g0 };

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 + q1
		p = q0 + q1;
		g = g0 + g1;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q1 + q2
		p = q1 + q2;
		g = g1 + g2;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 + q2
		p = q0 + q2;
		g = g0 + g2;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q2 + q3
		p = q2 + q3;
		g = g2 + g3;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q3 + q4
		p = q3 + q4;
		g = g3 + g4;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q4 + q5
		p = q4 + q5;

		IsTrue(std::isinf(p.mW));
		AreEqual(p.mX, 0.0f, FLT_EPSILON);
		AreEqual(p.mY, 0.0f, FLT_EPSILON);
		IsTrue(std::isinf(p.mZ));
	}

	TEST_METHOD("- Quat")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> q3 { PI, 1.0f, 5.0f, 4.258f };
		maj::math::Quaternion<float> q4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		maj::math::Quaternion<float> q5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g3 { PI, 1.0f, 5.0f, 4.258f };
		glm::quat g4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		glm::quat g5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

		// q0 - q0
		maj::math::Quaternion<float> p { q0 - q0 };
		glm::quat g { g0 + ( -g0 ) };

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 - q1
		p = q0 - q1;
		g = g0 + ( -g1 );

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q1 - q2
		p = q1 - q2;
		g = g1 + ( -g2 );

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 - q2
		p = q0 - q2;
		g = g0 + ( -g2 );

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q2 - q3
		p = q2 - q3;
		g = g2 + ( -g3 );

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q3 - q4
		p = q3 - q4;
		g = g3 + ( -g4 );

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q4 - q5
		p = q4 - q5;

		AreEqual(p.mW, 0.0f, FLT_EPSILON);
		IsTrue(std::isinf(p.mX));
		IsTrue(std::isinf(p.mY));
		AreEqual(p.mZ, 0.0f, FLT_EPSILON);
	}

	TEST_METHOD("* Quat")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> q3 { PI, 1.0f, 5.0f, 4.258f };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g3 { PI, 1.0f, 5.0f, 4.258f };

		// q0 * q0
		maj::math::Quaternion<float> p { q0 * q0 };
		glm::quat g { g0 * g0 };

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 * q1
		p = q0 * q1;
		g = g0 * g1;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q1 * q2
		p = q1 * q2;
		g = g1 * g2;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 * q2
		p = q0 * q2;
		g = g0 * g2;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q2 * q3
		p = q2 * q3;
		g = g2 * g3;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);
	}

	TEST_METHOD("/ Quat")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> q3 { PI, 1.0f, 5.0f, 4.258f };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g3 { PI, 1.0f, 5.0f, 4.258f };

		// q0 / q0
		maj::math::Quaternion<float> p { q0 / q0 };

		AreEqual(p.mW, 1.0f, FLT_EPSILON);
		AreEqual(p.mX, 0.0f, FLT_EPSILON);
		AreEqual(p.mY, 0.0f, FLT_EPSILON);
		AreEqual(p.mZ, 0.0f, FLT_EPSILON);

		// q0 / q1
		p = q0 / q1;
		glm::quat g { g0 * glm::inverse(g1) };

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q1 / q2
		p = q1 / q2;
		g = g1 * glm::inverse(g2);

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 / q2
		p = q0 / q2;
		g = g0 * glm::inverse(g2);

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q2 / q3
		p = q2 / q3;
		g = g2 * glm::inverse(g3);

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);
	}

	TEST_METHOD("* Vec3")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };

		maj::math::Vec3<float> v0;
		maj::math::Vec3<float> v1 { 1.f };
		maj::math::Vec3<float> v2 { maj::math::Vec3<float>::skRight };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };

		glm::vec3 u0 { 0.0f, 0.0f, 0.0f };
		glm::vec3 u1 { 1.0f, 1.0f, 1.0f };
		glm::vec3 u2 { 1.0f, 0.0f, 0.0f };

		// q0 * v0
		maj::math::Vec3<float> v { q0 * v0 };
		glm::vec3 u { g0 * u0 };

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);

		// q0 * v1
		v = q0 * v1;
		u = g0 * u1;

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);

		// q0 * v2
		v = q0 * v2;
		u = g0 * u2;

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);

		// q1 * v0
		v = q1 * v0;
		u = g1 * u0;

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);

		// q1 * v1
		v = q1 * v1;
		u = g1 * u1;

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);

		// q1 * v2
		v = q1 * v2;
		u = g1 * u2;

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);

		// q2 * v0
		v = q2 * v0;
		u = g2 * u0;

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);

		// q2 * v1
		v = q2 * v1;
		u = g2 * u1;

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);

		// q2 * v2
		v = q2 * v2;
		u = g2 * u2;

		AreEqual(v.mX, u.x, FLT_EPSILON);
		AreEqual(v.mY, u.y, FLT_EPSILON);
		AreEqual(v.mZ, u.z, FLT_EPSILON);
	}

	TEST_METHOD("+= Quat")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> q3 { PI, 1.0f, 5.0f, 4.258f };
		maj::math::Quaternion<float> q4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		maj::math::Quaternion<float> q5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g3 { PI, 1.0f, 5.0f, 4.258f };
		glm::quat g4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		glm::quat g5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

		// q0 += q0
		maj::math::Quaternion<float> p { q0 };
		glm::quat g { g0 };
		p += q0;
		g += g0;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 += q1
		p += q1;
		g += g1;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q1 += q2
		p += q2;
		g += g2;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 += q2
		p = q0;
		p += q2;
		g = g0;
		g += g2;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q2 += q3
		p += q3;
		g += g3;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q3 += q4
		p = q3;
		p += q4;
		g = g3;
		g += g4;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q4 += q5
		p = q4;
		p += q5;

		IsTrue(std::isinf(p.mW));
		AreEqual(p.mX, 0.0f, FLT_EPSILON);
		AreEqual(p.mY, 0.0f, FLT_EPSILON);
		IsTrue(std::isinf(p.mZ));
	}

	TEST_METHOD("-= Quat")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> q3 { PI, 1.0f, 5.0f, 4.258f };
		maj::math::Quaternion<float> q4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		maj::math::Quaternion<float> q5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g3 { PI, 1.0f, 5.0f, 4.258f };
		glm::quat g4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		glm::quat g5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

		// q0 -= q0
		maj::math::Quaternion<float> p { q0 };
		glm::quat g { g0 };
		p -= q0;
		g -= g0;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 -= q1
		p -= q1;
		g -= g1;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q1 -= q2
		p -= q2;
		g -= g2;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 -= q2
		p = q0;
		p -= q2;
		g = g0;
		g -= g2;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q2 -= q3
		p -= q3;
		g -= g3;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q3 -= q4
		p = q3;
		p -= q4;
		g = g3;
		g -= g4;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q4 -= q5
		p = q4;
		p -= q5;

		AreEqual(p.mW, 0.0f, FLT_EPSILON);
		IsTrue(std::isinf(p.mX));
		IsTrue(std::isinf(p.mY));
		AreEqual(p.mZ, 0.0f, FLT_EPSILON);
	}

	TEST_METHOD("*= Quat")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> q3 { PI, 1.0f, 5.0f, 4.258f };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g3 { PI, 1.0f, 5.0f, 4.258f };

		// q0 *= q0
		maj::math::Quaternion<float> p { q0 };
		p *= q0;
		glm::quat g { g0 };
		g *= g0;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 *= q1
		p *= q1;
		g *= g1;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 *= q2
		p *= q2;
		g *= g2;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q1 *= q2
		p = q1;
		p *= q2;
		g = g1;
		g *= g2;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q2 *= q3
		p *= q3;
		g *= g3;

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);
	}

	TEST_METHOD("/= Quat")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> q3 { PI, 1.0f, 5.0f, 4.258f };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g3 { PI, 1.0f, 5.0f, 4.258f };

		// q0 /= q0
		maj::math::Quaternion<float> p { q0 };
		p /= q0;

		AreEqual(p.mW, q0.mW, FLT_EPSILON);
		AreEqual(p.mX, q0.mX, FLT_EPSILON);
		AreEqual(p.mY, q0.mY, FLT_EPSILON);
		AreEqual(p.mZ, q0.mZ, FLT_EPSILON);

		// q0 /= q1
		p /= q1;
		glm::quat g { g0 };
		g *= glm::inverse(g1);

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q1 /= q2
		p = q1;
		p /= q2;
		g = g1;
		g *= glm::inverse(g2);

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q0 /= q2
		p = q0;
		p /= q2;
		g = g0;
		g *= glm::inverse(g2);

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);

		// q2 /= q3
		p = q2;
		p /= q3;
		g = g2;
		g *= glm::inverse(g3);

		AreEqual(p.mW, g.w, FLT_EPSILON);
		AreEqual(p.mX, g.x, FLT_EPSILON);
		AreEqual(p.mY, g.y, FLT_EPSILON);
		AreEqual(p.mZ, g.z, FLT_EPSILON);
	}

	TEST_METHOD("*= float")
	{
		maj::math::Quaternion<float> q0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };

		q0 *= PI;
		g0 *= PI;
		AreEqual(q0.mW, g0.w, FLT_EPSILON);
		AreEqual(q0.mX, g0.x, FLT_EPSILON);
		AreEqual(q0.mY, g0.y, FLT_EPSILON);
		AreEqual(q0.mZ, g0.z, FLT_EPSILON);

		maj::math::Quaternion<float> q1;
		glm::quat g1 { 1.f, {} };

		q1 *= 2.f;
		g1 *= 2.f;
		AreEqual(q1.mW, g1.w, FLT_EPSILON);
		AreEqual(q1.mX, g1.x, FLT_EPSILON);
		AreEqual(q1.mY, g1.y, FLT_EPSILON);
		AreEqual(q1.mZ, g1.z, FLT_EPSILON);

		maj::math::Quaternion<float> q2 { 2.0f, 2.4589f, 47.5289627f, 0.05f };
		glm::quat g2 { 2.0f, 2.4589f, 47.5289627f, 0.05f };

		q2 *= 0.5689952f;
		g2 *= 0.5689952f;
		AreEqual(q2.mW, g2.w, FLT_EPSILON);
		AreEqual(q2.mX, g2.x, FLT_EPSILON);
		AreEqual(q2.mY, g2.y, FLT_EPSILON);
		AreEqual(q2.mZ, g2.z, FLT_EPSILON);

		maj::math::Quaternion<float> q3 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		glm::quat g3 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };

		q3 *= 0.25f;
		g3 *= 0.25f;
		AreEqual(q3.mW, g3.w, FLT_EPSILON);
		AreEqual(q3.mX, g3.x, FLT_EPSILON);
		AreEqual(q3.mY, g3.y, FLT_EPSILON);
		AreEqual(q3.mZ, g3.z, FLT_EPSILON);

		maj::math::Quaternion<float> q4 { PI, 1.0f, 5.0f, 4.258f };
		q4 *= std::numeric_limits<float>::max();
		IsTrue(std::isinf(q4.mW));
		AreEqual(q4.mX, std::numeric_limits<float>::max(), FLT_EPSILON);
		IsTrue(std::isinf(q4.mY));
		IsTrue(std::isinf(q4.mZ));
	}

	TEST_METHOD("/= float")
	{
		maj::math::Quaternion<float> q1;
		q1 /= 2.f;
		glm::quat g1 { glm::quat { 1.f, {} } / 2.f };

		AreEqual(q1.mW, g1.w, FLT_EPSILON);
		AreEqual(q1.mX, g1.x, FLT_EPSILON);
		AreEqual(q1.mY, g1.y, FLT_EPSILON);
		AreEqual(q1.mZ, g1.z, FLT_EPSILON);

		maj::math::Quaternion<float> q2 { 2.0f, 2.4589f, 47.5289627f, 0.05f };
		q2 /= 0.5689952f;
		glm::quat g2 { glm::quat { 2.0f, 2.4589f, 47.5289627f, 0.05f } / 0.5689952f };

		AreEqual(q2.mW, g2.w, FLT_EPSILON);
		AreEqual(q2.mX, g2.x, FLT_EPSILON);
		AreEqual(q2.mY, g2.y, FLT_EPSILON);
		AreEqual(q2.mZ, g2.z, FLT_EPSILON);

		maj::math::Quaternion<float> q3 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		q3 /= 0.25f;

		IsTrue(std::isinf(q3.mW));
		IsTrue(std::isinf(q3.mX));
		IsTrue(std::isinf(q3.mY));
		IsTrue(std::isinf(q3.mZ));

		maj::math::Quaternion<float> q4 { PI, 1.0f, 5.0f, 4.258f };
		q4 /= std::numeric_limits<float>::max();
		glm::quat g4 { glm::quat { PI, 1.0f, 5.0f, 4.258f } / std::numeric_limits<float>::max() };

		AreEqual(q4.mW, g4.w, FLT_EPSILON);
		AreEqual(q4.mX, g4.x, FLT_EPSILON);
		AreEqual(q4.mY, g4.y, FLT_EPSILON);
		AreEqual(q4.mZ, g4.z, FLT_EPSILON);

		q2 /= 0.f;

		AreEqual(q2.mW, g2.w, FLT_EPSILON);
		AreEqual(q2.mX, g2.x, FLT_EPSILON);
		AreEqual(q2.mY, g2.y, FLT_EPSILON);
		AreEqual(q2.mZ, g2.z, FLT_EPSILON);
	}

	TEST_METHOD("==")
	{
		// Works in 1E-7

		maj::math::Quaternion<float> q1 { 10.568f, 0.f, 0.58f, 0.05000001f };
		maj::math::Quaternion<float> q2 { 10.568f, 0.f, 0.58f, 0.05f };

		maj::math::Quaternion<float> q3 { 2.0f, 2.4588777777f, 4.52896527f, 4.259f };
		maj::math::Quaternion<float> q4 { 2.0f, 2.4588777766f, 4.5289653f, 4.259f };

		maj::math::Quaternion<float> q5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
		maj::math::Quaternion<float> q6 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

		IsTrue(q1 == q2);
		IsTrue(q3 == q4);
		IsTrue(q5 == q5);
		IsTrue(q6 == q6);
	}

	TEST_METHOD("!=")
	{
		maj::math::Quaternion<float> q1 { 10.568f, 0.f, 0.58f, 0.05005f };
		maj::math::Quaternion<float> q2 { 10.568f, 0.f, 0.58f, 0.05f };

		maj::math::Quaternion<float> q3 { 2.0f, 2.45887f, 4.52827f, 4.259f };
		maj::math::Quaternion<float> q4 { 2.0f, 2.458866f, 4.5283f, 4.259f };

		maj::math::Quaternion<float> q5 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
		maj::math::Quaternion<float> q6 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };

		IsTrue(q1 != q2);
		IsTrue(q3 != q4);
		IsTrue(q5 != q6);
	}

	TEST_METHOD("fromEuler")
	{
		maj::math::Quaternion<float> q { maj::math::Quaternion<float>::fromEuler({}) };
		glm::quat g { glm::vec3 { 0.f, 0.f, 0.f } };

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		q = maj::math::Quaternion<float>::fromEuler(maj::math::Vec3<float>{1.f, 1.f, 1.f } *RAD2DEG);
		g = glm::quat { glm::vec3 { 1.f, 1.f, 1.f } };

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		q = maj::math::Quaternion<float>::fromEuler(maj::math::Vec3<float>{ PI, 5.0f * PI, 0.0f } *RAD2DEG);
		g = glm::quat { glm::vec3 { PI, 5.0f * PI, 0.0f } };

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		q = maj::math::Quaternion<float>::fromEuler({ std::numeric_limits<float>::lowest(), 0.0f, std::numeric_limits<float>::max() });
		g = glm::quat { glm::vec3 { std::numeric_limits<float>::lowest(), 0.0f, std::numeric_limits<float>::max() } *DEG2RAD };

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);
	}

	TEST_METHOD("fromEuler float")
	{
		maj::math::Quaternion<float> q { maj::math::Quaternion<float>::fromEuler(0.0f, 0.0f, 0.0f) };
		glm::quat g { glm::vec3 { 0.f, 0.f, 0.f } };

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		q = maj::math::Quaternion<float>::fromEuler(RAD2DEG, RAD2DEG, RAD2DEG);
		g = glm::quat { glm::vec3 { 1.f, 1.f, 1.f } };

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		q = maj::math::Quaternion<float>::fromEuler(PI * RAD2DEG, 5.0f * PI * RAD2DEG, 0.0f);
		g = glm::quat { glm::vec3 { PI, 5.0f * PI, 0.0f } };

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		q = maj::math::Quaternion<float>::fromEuler(std::numeric_limits<float>::lowest(), 0.0f, std::numeric_limits<float>::max());
		g = glm::quat { glm::vec3 { std::numeric_limits<float>::lowest(), 0.0f, std::numeric_limits<float>::max() } *DEG2RAD };

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);
	}

	TEST_METHOD("lerp")
	{
		maj::math::Quaternion<float> q0 { 0.0f, 0.0f, 0.0f, 0.0f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> q3 { PI, 1.0f, 5.0f, 4.258f };
		maj::math::Quaternion<float> q4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g3 { PI, 1.0f, 5.0f, 4.258f };
		glm::quat g4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };

		// q0 to q0 by 0
		maj::math::Quaternion<float> q { maj::math::Quaternion<float>::lerp(q0, q0, 0.0f) };
		glm::quat g { glm::lerp(g0, g0, 0.0f) };

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q0 to q0 by 0.58
		q = maj::math::Quaternion<float>::lerp(q0, q0, 0.58f);
		g = glm::lerp(g0, g0, 0.58f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q0 to q1 by 0.58
		q = maj::math::Quaternion<float>::lerp(q0, q1, 0.58f);
		g = glm::lerp(g0, g1, 0.58f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q0 to q1 by 1
		q = maj::math::Quaternion<float>::lerp(q0, q1, 1.0f);
		g = glm::lerp(g0, g1, 1.0f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q1 to q2 by 0.58
		q = maj::math::Quaternion<float>::lerp(q1, q2, 0.58f);
		g = glm::lerp(g1, g2, 0.58f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q1 to q2 by 1
		q = maj::math::Quaternion<float>::lerp(q1, q2, 1.0f);
		g = glm::lerp(g1, g2, 1.0f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q2 to q3 by 1
		q = maj::math::Quaternion<float>::lerp(q2, q3, 1.0f);
		g = glm::lerp(g2, g3, 1.0f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q1 to q4 by 0.58
		q = maj::math::Quaternion<float>::lerp(q1, q4, 0.58f);
		g = glm::lerp(g1, g4, 0.58f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q1 to q4 by 1
		q = maj::math::Quaternion<float>::lerp(q1, q4, 1.0f);
		g = glm::lerp(g1, g4, 1.0f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);
	}

	TEST_METHOD("nlerp")
	{
		maj::math::Quaternion<float> q0 { 0.0f, 0.0f, 0.0f, 0.0f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> q3 { PI, 1.0f, 5.0f, 4.258f };
		maj::math::Quaternion<float> q4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g3 { PI, 1.0f, 5.0f, 4.258f };
		glm::quat g4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };

		// q0 to q0 by 0
		maj::math::Quaternion<float> q { maj::math::Quaternion<float>::nlerp(q0, q0, 0.0f) };
		glm::quat g { glm::normalize(glm::lerp(g0, g0, 0.0f)) };

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q0 to q0 by 0.58
		q = maj::math::Quaternion<float>::nlerp(q0, q0, 0.58f);
		g = glm::normalize(glm::lerp(g0, g0, 0.58f));

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q0 to q1 by 0.58
		q = maj::math::Quaternion<float>::nlerp(q0, q1, 0.58f);
		g = glm::normalize(glm::lerp(g0, g1, 0.58f));

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q0 to q1 by 1
		q = maj::math::Quaternion<float>::nlerp(q0, q1, 1.0f);
		g = glm::normalize(glm::lerp(g0, g1, 1.0f));

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q1 to q2 by 0.58
		q = maj::math::Quaternion<float>::nlerp(q1, q2, 0.58f);
		g = glm::normalize(glm::lerp(g1, g2, 0.58f));

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q1 to q2 by 1
		q = maj::math::Quaternion<float>::nlerp(q1, q2, 1.0f);
		g = glm::normalize(glm::lerp(g1, g2, 1.0f));

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q2 to q3 by 1
		q = maj::math::Quaternion<float>::nlerp(q2, q3, 1.0f);
		g = glm::normalize(glm::lerp(g2, g3, 1.0f));

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q1 to q4 by 0.58
		q = maj::math::Quaternion<float>::nlerp(q1, q4, 0.58f);
		g = glm::normalize(glm::lerp(g1, g4, 0.58f));

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q1 to q4 by 1
		q = maj::math::Quaternion<float>::nlerp(q1, q4, 1.0f);
		g = glm::normalize(glm::lerp(g1, g4, 1.0f));

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);
	}

	TEST_METHOD("slerp")
	{
		maj::math::Quaternion<float> q0 { 0.0f, 0.0f, 0.0f, 0.0f };
		maj::math::Quaternion<float> q1;
		maj::math::Quaternion<float> q2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		maj::math::Quaternion<float> q3 { PI, 1.0f, 5.0f, 4.258f };
		maj::math::Quaternion<float> q4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		maj::math::Quaternion<float> q5 { -PI, 0.f, 1.0f, 0.f };
		maj::math::Quaternion<float> q6 { 25.2566f, 1.f, 0.0f, 0.f };
		maj::math::Quaternion<float> q7 { PI / 2.f, 0.f, 0.0f, 0.f };

		glm::quat g0 { 0.f, 0.f, 0.f, 0.f };
		glm::quat g1 { 1.f, 0.f, 0.f, 0.f };
		glm::quat g2 { 2.0f, -2.4589f, 47.5289627f, 0.05f };
		glm::quat g3 { PI, 1.0f, 5.0f, 4.258f };
		glm::quat g4 { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() };
		glm::quat g5 { -PI, 0.f, 1.f, 0.f };
		glm::quat g6 { 25.2566f, 1.f, 0.f, 0.f };
		glm::quat g7 { PI / 2.f, 0.f, 0.f, 0.f };

		// q0 to q0 by 0
		maj::math::Quaternion<float> q { maj::math::Quaternion<float>::slerp(q0, q0, 0.0f) };
		glm::quat g { glm::slerp(g0, g0, 0.0f) };

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q0 to q0 by 0.58
		q = maj::math::Quaternion<float>::slerp(q0, q0, 0.58f);
		g = glm::slerp(g0, g0, 0.58f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q0 to q1 by 0.58
		q = maj::math::Quaternion<float>::slerp(q0, q1, 0.58f);
		g = glm::slerp(g0, glm::normalize(g1), 0.58f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q0 to q1 by 1
		q = maj::math::Quaternion<float>::slerp(q0, q1, 1.0f);
		g = glm::slerp(g0, glm::normalize(g1), 1.0f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q1 to q2 by 0.58
		q = maj::math::Quaternion<float>::slerp(q1, q2, 0.58f);
		g = glm::slerp(glm::normalize(g1), glm::normalize(g2), 0.58f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q1 to q2 by 1
		q = maj::math::Quaternion<float>::slerp(q1, q2, 1.0f);
		g = glm::slerp(glm::normalize(g1), glm::normalize(g2), 1.0f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q2 to q3 by 1.256
		q = maj::math::Quaternion<float>::slerp(q2, q3, 1.256f);
		g = glm::slerp(glm::normalize(g2), glm::normalize(g3), 1.f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q1 to q4 by 0.58
		q = maj::math::Quaternion<float>::slerp(q1, q4, 0.58f);
		g = glm::slerp(glm::normalize(g1), glm::normalize(g4), 0.58f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q1 to q4 by 1
		q = maj::math::Quaternion<float>::slerp(q1, q4, 1.0f);
		g = glm::slerp(glm::normalize(g1), glm::normalize(g4), 1.0f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q5 to q6 by 0.2556892889
		q = maj::math::Quaternion<float>::slerp(q5, q6, 0.2556892889f);
		g = glm::slerp(glm::normalize(g5), glm::normalize(g6), 0.2556892889f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);

		// q1 to q7 by 1.23
		q = maj::math::Quaternion<float>::slerp(q1, q7, 0.798753f);
		g = glm::slerp(glm::normalize(g1), glm::normalize(g7), 0.798753f);

		AreEqual(q.mW, g.w, FLT_EPSILON);
		AreEqual(q.mX, g.x, FLT_EPSILON);
		AreEqual(q.mY, g.y, FLT_EPSILON);
		AreEqual(q.mZ, g.z, FLT_EPSILON);
	}

	TEST_METHOD("lookRotation")
	{
		// Inverse glm because it creates a view matrix and we want it in model space
		maj::math::Quaternion<float> q { maj::math::Quaternion<float>::lookRotation({ 0.f, 0.f, 1.f }, { 0.f, 1.f, 0.f }) };
		glm::mat4 g { glm::inverse(glm::lookAtLH(glm::vec3 { 0.f, 0.f, 0.f }, glm::vec3 { 0.f, 0.f, 1.f }, glm::vec3 { 0.f, 1.f, 0.f })) };

		maj::math::Mat4<float> m { maj::math::Mat4<float>::makeRotation(q) };

		AreEqual(m[0u], g[0u][0u], FLT_EPSILON);
		AreEqual(m[1u], g[0u][1u], FLT_EPSILON);
		AreEqual(m[2u], g[0u][2u], FLT_EPSILON);
		AreEqual(m[3u], g[0u][3u], FLT_EPSILON);
		AreEqual(m[4u], g[1u][0u], FLT_EPSILON);
		AreEqual(m[5u], g[1u][1u], FLT_EPSILON);
		AreEqual(m[6u], g[1u][2u], FLT_EPSILON);
		AreEqual(m[7u], g[1u][3u], FLT_EPSILON);
		AreEqual(m[8u], g[2u][0u], FLT_EPSILON);
		AreEqual(m[9u], g[2u][1u], FLT_EPSILON);
		AreEqual(m[10u], g[2u][2u], FLT_EPSILON);
		AreEqual(m[11u], g[2u][3u], FLT_EPSILON);
		AreEqual(m[12u], g[3u][0u], FLT_EPSILON);
		AreEqual(m[13u], g[3u][1u], FLT_EPSILON);
		AreEqual(m[14u], g[3u][2u], FLT_EPSILON);
		AreEqual(m[15u], g[3u][3u], FLT_EPSILON);

		q = maj::math::Quaternion<float>::lookRotation({ 0.f, 0.f, 0.f }, { 0.f, -1.f, 0.f });
		m = maj::math::Mat4<float>::makeRotation(q);

		IsTrue(m == maj::math::Mat4<float>{});

		q = maj::math::Quaternion<float>::lookRotation({ 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f });
		m = maj::math::Mat4<float>::makeRotation(q);

		IsTrue(m == maj::math::Mat4<float>{});

		q = maj::math::Quaternion<float>::lookRotation({ 0.f, 1.f, 0.f }, { 0.f, -1.f, 0.f });
		m = maj::math::Mat4<float>::makeRotation(q);

		IsTrue(m == maj::math::Mat4<float>{});

		q = maj::math::Quaternion<float>::lookRotation({ 0.f, 1.f, 0.f }, { -1.f, 0.f, 0.f });
		m = maj::math::Mat4<float>::makeRotation(q);
		g = glm::inverse(glm::lookAtLH(glm::vec3 { 0.f, 0.f, 0.f }, glm::vec3 { 0.f, 1.f, 0.f }, glm::vec3 { -1.f, 0.f, 0.f }));

		AreEqual(m[0u], g[0u][0u], FLT_EPSILON);
		AreEqual(m[1u], g[0u][1u], FLT_EPSILON);
		AreEqual(m[2u], g[0u][2u], FLT_EPSILON);
		AreEqual(m[3u], g[0u][3u], FLT_EPSILON);
		AreEqual(m[4u], g[1u][0u], FLT_EPSILON);
		AreEqual(m[5u], g[1u][1u], FLT_EPSILON);
		AreEqual(m[6u], g[1u][2u], FLT_EPSILON);
		AreEqual(m[7u], g[1u][3u], FLT_EPSILON);
		AreEqual(m[8u], g[2u][0u], FLT_EPSILON);
		AreEqual(m[9u], g[2u][1u], FLT_EPSILON);
		AreEqual(m[10u], g[2u][2u], FLT_EPSILON);
		AreEqual(m[11u], g[2u][3u], FLT_EPSILON);
		AreEqual(m[12u], g[3u][0u], FLT_EPSILON);
		AreEqual(m[13u], g[3u][1u], FLT_EPSILON);
		AreEqual(m[14u], g[3u][2u], FLT_EPSILON);
		AreEqual(m[15u], g[3u][3u], FLT_EPSILON);

		q = maj::math::Quaternion<float>::lookRotation({ 0.f, 10.f, 0.f }, { 0.f, 0.f, 2.3f });
		m = maj::math::Mat4<float>::makeRotation(q);
		g = glm::inverse(glm::lookAtLH(glm::vec3 { 0.f, 0.f, 0.f }, glm::vec3 { 0.f, 10.f, 0.f }, glm::vec3 { 0.f, 0.f, 2.3f }));

		// Precision must be 1e-5 to pass
		IsTrue(maj::math::Helper<float>::areSame(m[0u], g[0u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(m[1u], g[0u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(m[2u], g[0u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(m[3u], g[0u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(m[4u], g[1u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(m[5u], g[1u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(m[6u], g[1u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(m[7u], g[1u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(m[8u], g[2u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(m[9u], g[2u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(m[10u], g[2u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(m[11u], g[2u][3u]));
		IsTrue(maj::math::Helper<float>::areSame(m[12u], g[3u][0u]));
		IsTrue(maj::math::Helper<float>::areSame(m[13u], g[3u][1u]));
		IsTrue(maj::math::Helper<float>::areSame(m[14u], g[3u][2u]));
		IsTrue(maj::math::Helper<float>::areSame(m[15u], g[3u][3u]));
	}
};