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
		std::cout << std::left << std::setw(20) << name
			<< ": (" << vector.x << ", " << vector.y
			<< ", " << vector.z << ")\n";
	}

	void PrintMatrix(const char* name, const Matrix4& matrix)
	{
		std::cout << name << ":\n";

		// GLM 按 matrix[列][行] 访问矩阵，这里按常见的行列形式打印。
		for (int row = 0; row < 4; ++row)
		{
			std::cout << "  [ ";

			for (int column = 0; column < 4; ++column)
				std::cout << std::setw(8) << matrix[column][row];

			std::cout << " ]\n";
		}
	}

	void RunVectorBasicsDemo()
	{
		std::cout << "========== Vector Basics ==========\n";

		const Vector3 a(1.0f, 2.0f, 3.0f);
		const Vector3 b(4.0f, 5.0f, 6.0f);

		PrintVector("a", a);
		PrintVector("b", b);

		// 向量加法、减法和标量乘法。
		PrintVector("a + b", a + b);
		PrintVector("a - b", a - b);
		PrintVector("a * 2", a * 2.0f);

		// 点积返回标量，可用于计算夹角和判断方向关系。
		std::cout << std::left << std::setw(20)
			<< "dot(a, b)" << ": " << glm::dot(a, b) << '\n';

		// 叉积返回垂直于 a 和 b 的向量。
		PrintVector("cross(a, b)", glm::cross(a, b));

		// 向量长度和归一化。
		std::cout << std::left << std::setw(20)
			<< "length(a)" << ": " << glm::length(a) << '\n';
		PrintVector("normalize(a)", glm::normalize(a));
	}

	Matrix4 MakeTranslationMatrix(const Vector3& translation)
	{
		// GLM 按列构造矩阵，下面直接展开平移矩阵的全部 16 个元素。
		// 按行查看时为：
		//
		//   1  0  0  tx
		//   0  1  0  ty
		//   0  0  1  tz
		//   0  0  0  1
		//
		return Matrix4(
			Vector4(1.0f,           0.0f,           0.0f,           0.0f),
			Vector4(0.0f,           1.0f,           0.0f,           0.0f),
			Vector4(0.0f,           0.0f,           1.0f,           0.0f),
			Vector4(translation.x,  translation.y,  translation.z,  1.0f));
	}

	void RunMatrixBasicsDemo()
	{
		std::cout << "\n========== 矩阵构造、转置和求逆 ==========\n";

		// 直接传入矩阵的全部 16 个元素。
		// GLM 按列构造，因此这里依次传入第 1 到第 4 列。
		// 按通常的“行”查看时，矩阵内容为：
		//
		//   2  1  0  5
		//   0  3  1  6
		//   0  0  4  7
		//   0  0  0  1
		//
		Matrix4 matrix(
			Vector4(2.0f, 0.0f, 0.0f, 0.0f),
			Vector4(1.0f, 3.0f, 0.0f, 0.0f),
			Vector4(0.0f, 1.0f, 4.0f, 0.0f),
			Vector4(5.0f, 6.0f, 7.0f, 1.0f));

		PrintMatrix("matrix", matrix);

		// 转置：交换矩阵的行和列。
		Matrix4 transposed = glm::transpose(matrix);
		PrintMatrix("transpose(matrix)", transposed);

		// 求逆：构造一个可以撤销 matrix 变换的矩阵。
		Matrix4 inverse = glm::inverse(matrix);
		PrintMatrix("inverse(matrix)", inverse);

		// 原矩阵乘以逆矩阵，结果应接近单位矩阵。
		PrintMatrix("matrix * inverse", matrix * inverse);

		std::cout << "\n---------- 向量平移：直接计算和矩阵计算 ----------\n";

		const Vector3 point(1.0f, 2.0f, 3.0f);
		const Vector3 translation(10.0f, 20.0f, 30.0f);

		PrintVector("point", point);
		PrintVector("translation", translation);

		// 直接用向量相加完成平移。
		const Vector3 movedDirectly = point + translation;
		PrintVector("point + translation", movedDirectly);

		// 使用平移矩阵时，点必须扩展为 w=1 的齐次坐标。
		Matrix4 translationMatrix = MakeTranslationMatrix(translation);

		PrintMatrix("translation matrix", translationMatrix);

		const Vector4 point4(point, 1.0f);
		const Vector4 movedByMatrix4 = translationMatrix * point4;
		const Vector3 movedByMatrix(movedByMatrix4);

		PrintVector("matrix * point", movedByMatrix);
	}

	void RunVectorMatrixArithmeticDemo()
	{
		std::cout << "\n========== 向量和矩阵的加减乘除 ==========\n";

		const Vector3 a(1.0f, 2.0f, 3.0f);
		const Vector3 b(4.0f, 5.0f, 6.0f);

		std::cout << "---------- 向量运算 ----------\n";
		PrintVector("a + b", a + b);
		PrintVector("a - b", a - b);
		PrintVector("a * 2", a * 2.0f);
		PrintVector("a / 2", a / 2.0f);
		std::cout << std::left << std::setw(20)
			<< "dot(a, b)" << ": " << glm::dot(a, b) << '\n';
		PrintVector("cross(a, b)", glm::cross(a, b));

		std::cout << "\n---------- 矩阵运算 ----------\n";
		const Matrix4 matrixA(
			Vector4(2.0f, 0.0f, 0.0f, 0.0f),
			Vector4(0.0f, 2.0f, 0.0f, 0.0f),
			Vector4(0.0f, 0.0f, 2.0f, 0.0f),
			Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		const Matrix4 matrixB(
			Vector4(1.0f, 0.0f, 0.0f, 0.0f),
			Vector4(0.0f, 1.0f, 0.0f, 0.0f),
			Vector4(0.0f, 0.0f, 1.0f, 0.0f),
			Vector4(3.0f, 4.0f, 5.0f, 1.0f));

		PrintMatrix("matrixA + matrixB", matrixA + matrixB);
		PrintMatrix("matrixA - matrixB", matrixA - matrixB);
		PrintMatrix("matrixA * 2", matrixA * 2.0f);
		PrintMatrix("matrixA / 2", matrixA / 2.0f);
		PrintMatrix("matrixA * matrixB", matrixA * matrixB);
		PrintMatrix("matrixA / matrixB", matrixA * glm::inverse(matrixB));

		const Vector4 vector4(a, 1.0f);
		PrintVector("matrixA * vector", Vector3(matrixA * vector4));
	}
}

int main()
{
	std::cout << std::fixed << std::setprecision(3);
	RunVectorBasicsDemo();
	RunMatrixBasicsDemo();
	RunVectorMatrixArithmeticDemo();
	return 0;
}
