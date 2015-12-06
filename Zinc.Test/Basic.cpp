#include <stdafx.h>

namespace Zinc { namespace Tests {

	ZN_TEST_CLASS(BasicTests)
	{
	public:
		ZN_TEST_METHOD(ExpressionToStringA)
		{
			auto x = Numeric<int>(3);
			auto y = Numeric<int>(2);
			auto expr = (x + (2 + x) - 1) / (2 * x + 1);
			std::string expected = "(((3+(2+3))-1)/((2*3)+1))";
			std::string actual = expr;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(ExpressionValue)
		{
			auto x = Numeric<int>(3);
			auto expr = (x + (2 + x) - 1) / (2 * x + 1);
			size_t expected = 1;
			size_t actual = expr();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(VariableExpression)
		{
			auto expr = _a + 1;
			std::string expected = "(a+1)";
			std::string actual = expr;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(VariableBindExpressionA)
		{
			auto expr = (_a + (2 + _a) - 1) / (2 * _a + 1);
			auto bind = Bind(expr, _a, 3);
			std::string expected = "(((3+(2+3))-1)/((2*3)+1))";
			std::string actual = bind;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(VariableBindValue)
		{
			auto expr = (_a + (2 + _a) - _b) / (2 * _a + 1);
			auto bind1 = Bind(expr, _a, 3);
			auto bind2 = Bind(bind1, _b, 1);
			size_t expected = 1;
			size_t actual = bind2();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(VariableBindString)
		{
			auto expr = (_a + (2 + _a) - _b) / (2 * _a + 1);
			auto bind = Bind(expr, _a, 3);
			std::string expected = "(((3+(2+3))-b)/((2*3)+1))";
			std::string actual = bind;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(SinusExpressionString)
		{
			auto x = Numeric<int>(3);
			auto expr = sin(x) + 1;
			std::string expected = "(sin(3)+1)";
			std::string actual = expr;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(SinusExpressionValue)
		{
			auto expr = sin(_a) + _b;
			auto bind = Bind(expr, _a, 2);
			std::string expected = "(sin(2)+b)";
			std::string actual = bind;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(BindSinusValue)
		{
			auto expr = sin(_a) + _b;
			auto bind1 = Bind(expr, _a, _pi);
			auto bind2 = Bind(bind1, _b, -1);
			long double expected = -1;
			long double actual = bind2();
			Assert::IsTrue(std::abs(expected - actual) < std::numeric_limits<float>::epsilon());
		}

		ZN_TEST_METHOD(VariableValue)
		{
			auto expr = _a + 1;
			auto value = expr();
			auto bind = Bind(value, _a, 2);
			int expected = 3;
			int actual = bind();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(VariableExpressionToString)
		{
			auto expr = _a + 1 + 2;
			auto value = expr();
			std::string expected = "((a+1)+2)";
			std::string actual = value;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(MultiConstTypeValue)
		{
			auto c1 = Numeric<int>(1);
			auto c2 = Numeric<double>(1.7);
			auto expr = c1 + c2;
			double expected = 2.7;
			double actual = expr();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(BindCosinus)
		{
			auto expr = cos(_a) + _b;
			auto bind = Bind(expr, _a, 2);
			std::string expected = "(cos(2)+b)";
			std::string actual = bind;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(BindTangent)
		{
			auto expr = tan(_a) + _b;
			auto bind = Bind(expr, _a, 2);
			std::string expected = "(tan(2)+b)";
			std::string actual = bind;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(BindTangent2)
		{
			auto expr = tan(_a) + _b;
			auto bind = Bind(expr, _b, -2);
			std::string expected = "(tan(a)-2)";
			std::string actual = bind;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(UnaryOperatorAdditionBind)
		{
			auto expr = ++_a;
			auto bind = Bind(expr, _a, 2);
			int expected = 3;
			int actual = bind();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(UnaryOperatorAdditionString)
		{
			auto expr = ++_a - 1;
			std::string expected = "((++a)-1)";
			std::string actual = expr;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(UnaryOperatorSubtractionBind)
		{
			auto expr = --_a;
			auto bind = Bind(expr, _a, 2);
			int expected = 1;
			int actual = bind();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(UnaryOperatorSubtractionString)
		{
			auto expr = --_a - 1;
			std::string expected = "((--a)-1)";
			std::string actual = expr;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(UnaryOperatorMinusBind)
		{
			auto expr = -_a;
			auto bind = Bind(expr, _a, 2);
			int expected = -2;
			int actual = bind();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(UnaryOperatorMinusString)
		{
			auto expr = -_a - 1;
			std::string expected = "((-a)-1)";
			std::string actual = expr;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(BindComplex)
		{
			auto expr = _a + 1;
			auto bind = Bind(expr, _a, sin(_b));
			std::string expected = "(sin(b)+1)";
			std::string actual = bind;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(BindComplex2)
		{
			auto expr = _a + 1;
			auto bind = Bind(expr, _a, sin(_b));
			std::string expected = "(sin(b)+1)";
			std::string actual = bind;
			Assert::IsTrue(expected == actual);
		}

	};

}}