#include <cmath>
#include <iomanip>
#include <iostream>

#include <glm/glm.hpp>

using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Matrix4 = glm::mat4;

namespace
{
    // ========================================================================
    // 第一部分：输出工具
    // ========================================================================

    void PrintSection(const char* title)
    {
        std::cout << "\n========== " << title << " ==========\n";
    }

    void PrintSubsection(const char* title)
    {
        std::cout << "---------- " << title << " ----------\n";
    }

    void PrintVector(const char* name, const Vector3& value)
    {
        std::cout << std::left << std::setw(24) << name
                  << ": (" << value.x << ", "
                  << value.y << ", "
                  << value.z << ")\n";
    }

    void PrintMatrix(const char* name, const Matrix4& matrix)
    {
        std::cout << name << ":\n";

        for (int row = 0; row < 4; ++row)
        {
            std::cout << "  [ ";
            for (int column = 0; column < 4; ++column)
                std::cout << std::setw(8) << matrix[column][row];
            std::cout << " ]\n";
        }
    }

    // ========================================================================
    // 第二部分：角度和矩阵构造
    // ========================================================================

    float DegreesToRadians(float degrees)
    {
        return glm::radians(degrees);
    }

    Vector3 DegreesToRadians(const Vector3& degrees)
    {
        return glm::radians(degrees);
    }

    // GLM 使用列向量约定：变换写成 Matrix * Vector。
    Matrix4 MakeTranslationMatrix(const Vector3& translation)
    {
        return Matrix4(
            Vector4(1.0f,           0.0f,           0.0f,           0.0f),
            Vector4(0.0f,           1.0f,           0.0f,           0.0f),
            Vector4(0.0f,           0.0f,           1.0f,           0.0f),
            Vector4(translation.x,  translation.y,  translation.z,  1.0f));
    }

    Matrix4 MakeScaleMatrix(const Vector3& scale)
    {
        // 缩放只需要修改主对角线：x、y、z 分别独立缩放。
        return Matrix4(
            Vector4(scale.x, 0.0f,    0.0f,    0.0f),
            Vector4(0.0f,    scale.y, 0.0f,    0.0f),
            Vector4(0.0f,    0.0f,    scale.z, 0.0f),
            Vector4(0.0f,    0.0f,    0.0f,    1.0f));
    }

    Matrix4 MakeRotationXMatrix(float angle)
    {
        // 绕 X 轴旋转只改变 y、z 坐标，x 坐标保持不变。
        const float c = std::cos(angle);
        const float s = std::sin(angle);

        return Matrix4(
            Vector4(1.0f, 0.0f, 0.0f, 0.0f),
            Vector4(0.0f, c,    s,    0.0f),
            Vector4(0.0f, -s,   c,    0.0f),
            Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    Matrix4 MakeRotationYMatrix(float angle)
    {
        // 绕 Y 轴旋转只改变 x、z 坐标，y 坐标保持不变。
        const float c = std::cos(angle);
        const float s = std::sin(angle);

        return Matrix4(
            Vector4(c,    0.0f, -s,   0.0f),
            Vector4(0.0f, 1.0f, 0.0f, 0.0f),
            Vector4(s,    0.0f, c,    0.0f),
            Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    Matrix4 MakeRotationZMatrix(float angle)
    {
        // 绕 Z 轴旋转只改变 x、y 坐标，z 坐标保持不变。
        const float c = std::cos(angle);
        const float s = std::sin(angle);

        return Matrix4(
            Vector4(c,    s,    0.0f, 0.0f),
            Vector4(-s,   c,    0.0f, 0.0f),
            Vector4(0.0f, 0.0f, 1.0f, 0.0f),
            Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    Matrix4 MakeEulerRotationMatrix(const Vector3& eulerAngles)
    {
        // 欧拉角本质上是三个轴旋转矩阵的组合。
        // 矩阵乘法从右向左生效，因此这里的执行顺序是 X、Y、Z。
        const Matrix4 rotateX = MakeRotationXMatrix(eulerAngles.x);
        const Matrix4 rotateY = MakeRotationYMatrix(eulerAngles.y);
        const Matrix4 rotateZ = MakeRotationZMatrix(eulerAngles.z);

        return rotateZ * rotateY * rotateX;
    }

    // ========================================================================
    // 第三部分：向量基础运算
    // ========================================================================

    void RunVectorBasicsDemo()
    {
        PrintSection("向量基础运算");

        const Vector3 a(1.0f, 2.0f, 3.0f);
        const Vector3 b(4.0f, 5.0f, 6.0f);

        PrintVector("a", a);
        PrintVector("b", b);
        PrintVector("a + b", a + b);
        PrintVector("a - b", a - b);
        PrintVector("a * 2", a * 2.0f);
        PrintVector("cross(a, b)", glm::cross(a, b));
        PrintVector("normalize(a)", glm::normalize(a));

        std::cout << std::left << std::setw(24)
                  << "dot(a, b)" << ": " << glm::dot(a, b) << '\n';
        std::cout << std::left << std::setw(24)
                  << "length(a)" << ": " << glm::length(a) << '\n';
    }

    // ========================================================================
    // 第四部分：矩阵和变换
    // ========================================================================

    void RunMatrixBasicsDemo()
    {
        PrintSection("矩阵构造、转置和求逆");

        // 直接写出矩阵的四个列向量；按行查看为：
        //   2  1  0  5
        //   0  3  1  6
        //   0  0  4  7
        //   0  0  0  1
        const Matrix4 matrix(
            Vector4(2.0f, 0.0f, 0.0f, 0.0f),
            Vector4(1.0f, 3.0f, 0.0f, 0.0f),
            Vector4(0.0f, 1.0f, 4.0f, 0.0f),
            Vector4(5.0f, 6.0f, 7.0f, 1.0f));

        // 转置交换行和列；逆矩阵用于撤销原矩阵的变换。
        // 这里保留完整矩阵输出，便于观察每个元素的变化。
        PrintMatrix("matrix", matrix);
        PrintMatrix("transpose(matrix)", glm::transpose(matrix));
        PrintMatrix("inverse(matrix)", glm::inverse(matrix));

        PrintSubsection("向量平移");

        const Vector3 point(1.0f, 2.0f, 3.0f);
        const Vector3 translation(10.0f, 20.0f, 30.0f);

        PrintVector("point", point);
        PrintVector("translation", translation);
        PrintVector("直接相加", point + translation);

        const Matrix4 translationMatrix = MakeTranslationMatrix(translation);
        const Vector4 translatedPoint = translationMatrix * Vector4(point, 1.0f);
        // 点用 w=1 才能让平移矩阵的最后一列参与计算。
        PrintVector("矩阵计算", Vector3(translatedPoint));

        PrintSubsection("向量旋转");

        const Vector3 rotationPoint(1.0f, 0.0f, 0.0f);
        const float angleDegrees = 90.0f;
        const float angleRadians = DegreesToRadians(angleDegrees);
        const float c = std::cos(angleRadians);
        const float s = std::sin(angleRadians);

        const Vector3 rotatedDirectly(
            rotationPoint.x * c - rotationPoint.y * s,
            rotationPoint.x * s + rotationPoint.y * c,
            rotationPoint.z);

        PrintVector("rotation point", rotationPoint);
        std::cout << "rotation angle          : " << angleDegrees << " degrees\n";
        PrintVector("直接计算", rotatedDirectly);

        const Matrix4 rotationMatrix = MakeRotationZMatrix(angleRadians);
        const Vector4 rotatedPoint = rotationMatrix * Vector4(rotationPoint, 1.0f);
        // 直接公式和矩阵结果应该一致。
        PrintVector("矩阵计算", Vector3(rotatedPoint));

        PrintSubsection("欧拉角旋转");

        const Vector3 eulerDegrees(30.0f, 45.0f, 90.0f);
        const Matrix4 eulerRotation = MakeEulerRotationMatrix(DegreesToRadians(eulerDegrees));

        const Vector4 eulerRotatedPoint = eulerRotation * Vector4(rotationPoint, 1.0f);
        PrintVector("euler degrees", eulerDegrees);
        PrintVector("euler rotated point", Vector3(eulerRotatedPoint));

        PrintSubsection("向量缩放");

        const Vector3 scale(2.0f, 3.0f, 4.0f);
        const Vector3 scalePoint(1.0f, 2.0f, 3.0f);

        PrintVector("scale point", scalePoint);
        PrintVector("scale", scale);
        PrintVector("直接计算", scalePoint * scale);

        const Matrix4 scaleMatrix = MakeScaleMatrix(scale);
        const Vector4 scaledPoint = scaleMatrix * Vector4(scalePoint, 1.0f);
        // 缩放矩阵和逐分量相乘应该得到相同结果。
        PrintVector("矩阵计算", Vector3(scaledPoint));
    }

    // ========================================================================
    // 第五部分：向量和矩阵的四则运算
    // ========================================================================

    void RunArithmeticDemo()
    {
        PrintSection("向量和矩阵的加减乘除");

        const Vector3 a(1.0f, 2.0f, 3.0f);
        const Vector3 b(4.0f, 5.0f, 6.0f);

        PrintSubsection("向量运算");
        PrintVector("a + b", a + b);
        PrintVector("a - b", a - b);
        PrintVector("a * 2", a * 2.0f);
        PrintVector("a / 2", a / 2.0f);
        PrintVector("cross(a, b)", glm::cross(a, b));
        std::cout << std::left << std::setw(24)
                  << "dot(a, b)" << ": " << glm::dot(a, b) << '\n';

        const Matrix4 matrixA(
            Vector4(2.0f, 0.0f, 0.0f, 0.0f),
            Vector4(0.0f, 2.0f, 0.0f, 0.0f),
            Vector4(0.0f, 0.0f, 2.0f, 0.0f),
            Vector4(0.0f, 0.0f, 0.0f, 1.0f));
        const Matrix4 matrixB = MakeTranslationMatrix(Vector3(3.0f, 4.0f, 5.0f));

        PrintSubsection("矩阵运算");
        // 矩阵加减是逐元素运算；矩阵乘法是行乘列。
        // 矩阵除法没有普通定义，通常用 A * inverse(B) 表示。
        const Matrix4 matrixSum = matrixA + matrixB;
        const Matrix4 matrixDifference = matrixA - matrixB;
        const Matrix4 matrixProduct = matrixA * matrixB;
        const Matrix4 matrixQuotient = matrixA * glm::inverse(matrixB);
        (void)matrixSum;
        (void)matrixDifference;
        (void)matrixProduct;
        (void)matrixQuotient;

        const Vector4 vector4(a, 1.0f);
        PrintVector("matrixA * vector", Vector3(matrixA * vector4));
    }
}

int main()
{
    std::cout << std::fixed << std::setprecision(3);
    RunVectorBasicsDemo();
    RunMatrixBasicsDemo();
    RunArithmeticDemo();
    return 0;
}
