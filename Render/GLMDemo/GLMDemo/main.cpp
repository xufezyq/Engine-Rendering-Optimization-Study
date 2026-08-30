#include <cmath>
#include <iomanip>
#include <iostream>

#include <glm/glm.hpp>

using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Matrix4 = glm::mat4;

namespace
{
	void PrintVector(const char* name, const Vector3& vector)
	{
		std::cout << std::left << std::setw(24) << name
			<< ": (" << vector.x << ", " << vector.y << ", " << vector.z << ")\n";
	}

	void PrintMatrix(const char* name, const Matrix4& matrix)
	{
		std::cout << name << ":\n";

		// GLM 使用 matrix[column][row] 访问矩阵。
		// 下面为了阅读方便，按照“行”打印矩阵。
		for (int row = 0; row < 4; ++row)
		{
			std::cout << "  [ ";
			for (int column = 0; column < 4; ++column)
			{
				std::cout << std::setw(8) << matrix[column][row];
			}
			std::cout << " ]\n";
		}
	}

	void PrintSectionHeader(const char* title)
	{
		std::cout << "========== " << title << " ==========\n";
	}

	void RunVectorBasicsDemo()
	{
		PrintSectionHeader("1. 向量的基础运算");

		Vector3 a(1.0f, 2.0f, 3.0f);
		Vector3 b(4.0f, 5.0f, 6.0f);

		PrintVector("a", a);
		PrintVector("b", b);
		PrintVector("a + b", a + b);
		PrintVector("a - b", a - b);
		PrintVector("a * 2", a * 2.0f);

		// 点积的结果是一个数，常用于计算夹角或判断两个方向的关系。
		std::cout << "dot(a, b)             : " << glm::dot(a, b) << '\n';

		// 叉积的结果仍然是一个三维向量，并且垂直于 a 和 b。
		PrintVector("cross(a, b)", glm::cross(a, b));

		// length 是向量长度，normalize 将向量变成长度为 1 的单位向量。
		std::cout << "length(a)              : " << glm::length(a) << '\n';
		PrintVector("normalize(a)", glm::normalize(a));
		std::cout << '\n';
	}

	void RunMatrixConstructionDemo()
	{
		PrintSectionHeader("2. 构造矩阵");

		// mat4(1.0f) 创建一个 4x4 单位矩阵：
		//
		//     1  0  0  0
		//     0  1  0  0
		//     0  0  1  0
		//     0  0  0  1
		//
		Matrix4 matrix(1.0f);

		// GLM 的访问顺序是 matrix[列][行]。
		// 例如 matrix[1][2] 表示第 2 列、第 3 行的元素。
		matrix[0][0] = 2.0f;
		matrix[1][1] = 3.0f;
		matrix[2][2] = 4.0f;

		PrintMatrix("matrix", matrix);
		std::cout << '\n';
	}

	void RunMatrixOperationsDemo()
	{
		PrintSectionHeader("3. 矩阵转置和求逆");

		Matrix4 matrix(1.0f);
		matrix[0][0] = 2.0f;
		matrix[1][1] = 3.0f;
		matrix[2][2] = 4.0f;

		Matrix4 transpose = glm::transpose(matrix);
		Matrix4 inverse = glm::inverse(matrix);

		PrintMatrix("matrix", matrix);
		PrintMatrix("transpose(matrix)", transpose);
		PrintMatrix("inverse(matrix)", inverse);

		// 一个矩阵乘以自己的逆矩阵，结果应该是单位矩阵。
		PrintMatrix("matrix * inverse", matrix * inverse);
		std::cout << '\n';
	}

	void RunTranslationDemo()
	{
		PrintSectionHeader("4. 创建平移矩阵");

		float tx = 2.0f;
		float ty = 3.0f;
		float tz = 4.0f;

		// 平移矩阵的数学形式是：
		//
		//     1  0  0  tx
		//     0  1  0  ty
		//     0  0  1  tz
		//     0  0  0  1
		//
		// 先创建单位矩阵，再把平移量写入最后一列。
		Matrix4 translation(1.0f);
		translation[3][0] = tx;
		translation[3][1] = ty;
		translation[3][2] = tz;

		PrintMatrix("translation matrix", translation);

		// 三维点要补成四维向量，并令 w=1。
		// 这样矩阵中的平移量才会参与计算。
		Vector3 point(1.0f, 1.0f, 1.0f);
		Vector4 point4(point, 1.0f);
		Vector4 movedPoint4 = translation * point4;
		Vector3 movedPoint(movedPoint4);

		PrintVector("point before", point);
		PrintVector("point after", movedPoint);
		std::cout << '\n';
	}

	void RunRotationDemo()
	{
		PrintSectionHeader("5. 创建旋转矩阵");

		// 这里创建“绕 Z 轴旋转 angle 角度”的旋转矩阵。
		float angleDegrees = 90.0f;
		float pi = 3.1415926535f;
		float angleRadians = angleDegrees * pi / 180.0f;

		float c = std::cos(angleRadians);
		float s = std::sin(angleRadians);

		// 绕 Z 轴旋转的数学形式是：
		//
		//     cos -sin  0  0
		//     sin  cos  0  0
		//      0    0   1  0
		//      0    0   0  1
		//
		// 注意：GLM 使用 matrix[列][行]，所以赋值时要按照列来写。
		Matrix4 rotation(1.0f);
		rotation[0][0] = c;
		rotation[1][0] = -s;
		rotation[0][1] = s;
		rotation[1][1] = c;

		PrintMatrix("rotation matrix", rotation);
		std::cout << "angle: " << angleDegrees << " degrees\n";

		// 点 (1, 0, 0) 绕 Z 轴旋转 90 度后，应该变成 (0, 1, 0)。
		Vector3 point(1.0f, 0.0f, 0.0f);
		Vector4 rotatedPoint4 = rotation * Vector4(point, 1.0f);
		Vector3 rotatedPoint(rotatedPoint4);

		PrintVector("point before", point);
		PrintVector("point after", rotatedPoint);
	}
}

int main()
{
	std::cout << std::fixed << std::setprecision(3);

	RunVectorBasicsDemo();
	RunMatrixConstructionDemo();
	RunMatrixOperationsDemo();
	RunTranslationDemo();
	RunRotationDemo();

	return 0;
}
