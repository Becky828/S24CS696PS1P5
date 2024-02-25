// Must include the gtest header to use the testing library
#include <gtest/gtest.h>
//#include "../Student.h"
#include "../Studentv2.h"

using namespace student;

namespace {
	Student TestDefaultStudent() { Student s;; return s; }
	Student TestParameterStudent() { Student s("John", "Doe", 3.5, "123456789"); return s; }
}

TEST(StudentTest, StudentDefaultConstructor) {
	EXPECT_EQ(TestDefaultStudent().getFirstName(), "");
	EXPECT_EQ(TestDefaultStudent().getLastName(), "");
	EXPECT_EQ(TestDefaultStudent().getGPA(), 0.0);
	EXPECT_EQ(TestDefaultStudent().getRedID(), "");
}

TEST(StudentTest, StudentConstructorWithParameters) {
	EXPECT_EQ(TestParameterStudent().getFirstName(), "John");
	EXPECT_EQ(TestParameterStudent().getLastName(), "Doe");
	EXPECT_EQ(TestParameterStudent().getGPA(), 3.5);
	EXPECT_EQ(TestParameterStudent().getRedID(), "123456789");
}