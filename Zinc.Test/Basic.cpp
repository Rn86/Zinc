#include <stdafx.h>

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning( disable : 4503 )
#endif

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
			long double expected = 1;
			long double actual = expr();
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
			auto rez = bind(expr, _a, 3);
			std::string expected = "(((3+(2+3))-1)/((2*3)+1))";
			std::string actual = rez;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(VariableBindValue)
		{
			auto expr = (_a + (2 + _a) - _b) / (2 * _a + 1);
			auto bind1 = bind(expr, _a, 3);
			auto bind2 = bind(bind1, _b, 1);
			long double expected = 1;
			long double actual = bind2();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(VariableBindString)
		{
			auto expr = (_a + (2 + _a) - _b) / (2 * _a + 1);
			auto rez = bind(expr, _a, 3);
			std::string expected = "(((3+(2+3))-b)/((2*3)+1))";
			std::string actual = rez;
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
			auto rez = bind(expr, _a, 2);
			std::string expected = "(sin(2)+b)";
			std::string actual = rez;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(BindSinusValue)
		{
			auto expr = sin(_a) + _b;
			auto bind1 = bind(expr, _a, _pi);
			auto bind2 = bind(bind1, _b, -1);
			long double expected = -1;
			long double actual = bind2();
			Assert::IsTrue(std::abs(expected - actual) < std::numeric_limits<float>::epsilon());
		}

		ZN_TEST_METHOD(VariableValue)
		{
			auto expr = _a + 1;
			auto value = expr();
			auto rez = bind(value, _a, 2);
			int expected = 3;
			int actual = rez();
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
			auto rez = bind(expr, _a, 2);
			std::string expected = "(cos(2)+b)";
			std::string actual = rez;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(UnaryOperatorAdditionBind)
		{
			auto expr = ++_a;
			auto rez = bind(expr, _a, 2);
			int expected = 3;
			int actual = rez();
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
			auto rez = bind(expr, _a, 2);
			int expected = 1;
			int actual = rez();
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
			auto rez = bind(expr, _a, 2);
			int expected = -2;
			int actual = rez();
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
			auto rez = bind(expr, _a, sin(_b));
			std::string expected = "(sin(b)+1)";
			std::string actual = rez;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(BindComplex2)
		{
			auto expr = _a + 1;
			auto rez = bind(expr, _a, sin(_b));
			std::string expected = "(sin(b)+1)";
			std::string actual = rez;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(PowerTest1)
		{
			auto expr = power<-2>(2);
			auto expected = 1 / 4;
			auto actual = expr();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(PowerTest2)
		{
			auto expr = power<2>(-2);
			auto expected = 4;
			auto actual = expr();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(PowerTest3)
		{
			auto expr = power<3>(2) / factorial<3>();
			auto expected = 4.0f / 3.0f;
			auto actual = expr();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(PowerTest4)
		{
			auto expr = power<5>(-1);
			auto expected = -1;
			auto actual = expr();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(ExpandIssue1)
		{
			auto expr = (power<1>(-1) / 6) * power<3>(2);
			auto expected = 4 / 3;
			auto actual = expr();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(ExpandIssue2)
		{
			auto expr = power<1>(-1) / 6;
			auto expected = 1 / 6;
			auto actual = expr();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(SeriesTest1)
		{
			auto expr = sin(_pi / 2);
			auto expan = expand<5>(expr);
			auto expected = 1;
			auto actual = expan();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(SeriesTest2)
		{
			auto expr = cos(_pi);
			auto expan = expand<5>(expr);
			auto expected = -1;
			auto actual = expan();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(SeriesTest3)
		{
			auto expr = _e ^ 2;
			auto expected = 7.3890560989306502272304274605750L;
			auto actual = expr();
			std::string string = expr;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(SeriesTest4)
		{
			auto expr = ln(1.5);
			auto expected = 0;
			auto actual = expr();
			std::string string = expr;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(DerivativeTest1)
		{
			auto expr = sin(2);
			auto expected = cos(2)*0;
			auto actual = derive(expr);
			bool same = std::is_same<decltype(expected), decltype(actual)>::value;
			Assert::IsTrue(same);
		}

		ZN_TEST_METHOD(DerivativeTest2)
		{
			auto expr = sin(_x);
			auto expected = cos(_x)*1;
			auto actual = derive(expr);
			bool same = std::is_same<decltype(expected), decltype(actual)>::value;
			Assert::IsTrue(same);
		}

		ZN_TEST_METHOD(DerivativeTest3)
		{
			auto expr = sin(_x);
			auto exp = cos(_x) * 1;
			auto act = derive(expr);
			auto bind1 = bind(exp, _x, _pi);
			auto bind2 = bind(act, _x, _pi);
			auto expected = bind1();
			auto actual = bind2();
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(DerivativeTest4)
		{
			auto expr = power<3>(_x);
			auto exp = 3 * power<2>(_x) * 1;
			auto act = derive(expr);
			auto bind1 = bind(exp, _x, 2);
			auto bind2 = bind(act, _x, 2);
			auto expected = bind1();
			auto actual = bind2();
			Assert::IsTrue(expected == actual);
			Assert::IsTrue(expected == 12);
			Assert::IsTrue(actual == 12);
		}

		ZN_TEST_METHOD(DerivativeTest5)
		{
			auto expr = _x * _y;
			auto derived = derive(expr);
			std::string expected = "((1*y)+(x*1))";
			std::string actual = derived;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(DerivativeTest6)
		{
			auto expr = _x * sin(_y);
			auto derived = derive(expr);
			std::string expected = "((1*sin(y))+(x*(cos(y)*1)))";
			std::string actual = derived;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(DerivativeTest7)
		{
			auto expr = _x / sin(_y);
			auto derived = derive(expr);
			std::string expected = "(((1*sin(y))-(x*(cos(y)*1)))/((sin(y))^2))";
			std::string actual = derived;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(Simplification1)
		{
			auto expr = (_x + 2) / 2;
			auto simple = simplify(expr);
			std::string expected = "((x+2)/2)";
			std::string actual = simple;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(Simplification2)
		{
			auto _2 = Numeric<int>(2);
			auto expr = (_2 + _2) - (_x / 2);
			auto simple = simplify(expr);
			std::string expected = "((((2+2)*2)-((x/2)*2))/2)";
			std::string actual = simple;
			Assert::IsTrue(expected == actual);
		}

		ZN_TEST_METHOD(LimTest1)
		{
			auto expr = _x / _x;
			auto limit = lim(_x.to<0>(), expr);
			Assert::IsTrue(limit == 1);
		}

		ZN_TEST_METHOD(LimTest2)
		{
			auto expr = _x / sin(_x);
			auto limit = lim(_x.to<0>(), expr);
			Assert::IsTrue(limit == 1);
		}

		ZN_TEST_METHOD(LimTest3)
		{
			auto expr = (sin(_x) - _x) / (_x * sin(_x));
			auto limit = lim(_x.to<0>(), expr);
			Assert::IsTrue(limit == 0);
		}

		ZN_TEST_METHOD(LimTest4)
		{
			auto expr = _x + 2;
			auto limit = lim(_x.to<1>(), expr);
			Assert::IsTrue(limit == 3);
		}

		ZN_TEST_METHOD(LimitTest5)
		{
			auto _2 = Numeric<int>(2);
			auto expr = (_2 + 2) - (_x / 2);
			auto limit = lim(_x.to<4>(), expr);
			Assert::IsTrue(limit == 2);
		}

		ZN_TEST_METHOD(LimitTest6)
		{
			auto expr = (power<2>(_x) - 1) / (_x - 1);
			auto limit = lim(_x.to<1>(), expr);
			Assert::IsTrue(limit == 2);
		}

		ZN_TEST_METHOD(LimitTest7)
		{
			auto expr = ((power<2>(_x) - 1) / (_x - 1)) - 1;
			auto limit = lim(_x.to<1>(), expr);
			Assert::IsTrue(limit == 1);
		}

		ZN_TEST_METHOD(LimitTest8)
		{
			auto expr = 1 / _x;
			auto limit = lim(_x.to<1>(), expr);
			Assert::IsTrue(limit == 1);
		}

		ZN_TEST_METHOD(LimitTest9)
		{
			auto expr = 1 / _x;
			auto limit = lim(_x.to<0>(), expr);
			Assert::IsTrue(limit == std::numeric_limits<long double>::infinity());
		}

		ZN_TEST_METHOD(LimitTest10)
		{
			auto expr = _x / _x;
			auto limit = lim(_x.to<0>(), expr);
			Assert::IsTrue(limit == 1);
		}
	};

}}

#ifdef _MSC_VER
	#pragma warning( pop )
#endif

