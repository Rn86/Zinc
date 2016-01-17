#ifndef ZINC_HPP_INCLUDED
#define ZINC_HPP_INCLUDED

#include <string>
#include <sstream>
#include <type_traits>
#include <cmath>

namespace Zinc
{
	static constexpr int CONFIDENCE_LEVEL = 5;

	template <size_t terms, class T>
	struct Expander;

	template <typename T>
	struct TypeTraits;

	template <typename T>
	struct Numeric;

	template <typename T, intmax_t N, intmax_t D = 1>
	struct Constant;

	template <class T1, class T2, typename T3>
	struct Binder;

	template <class T>
	struct Derivation;

	template <char id>
	struct LimitParam
	{
		LimitParam(int to)
			: m_to(to)
		{
		}

		int m_to;
	};

	template <class T>
	struct Expression
	{
	public:
		T const& operator()() const
		{
			return Self();
		}

		operator std::string() const
		{
			std::ostringstream ss;
			ss << (std::string)Self();
			return ss.str();
		}

	private:
		inline T const& Self() const
		{
			return static_cast<const T&>(*this);
		}
	};

	template <typename T>
	struct Numeric : Expression<Numeric<T> >
	{
		Numeric(const T & value)
			: m_value(value)
		{
		}

		Numeric(const Numeric & expr)
			: m_value(expr.m_value)
		{
		}

		auto operator()() const
		{
			return m_value;
		}

		operator std::string() const
		{
			return std::to_string(m_value);
		}

		T m_value;
	};

	template <typename T, intmax_t N, intmax_t D>
	struct Constant : Expression<Constant<T, N, D> >
	{
		constexpr T operator()() const
		{
			return (T)N / (T)D;
		}

		operator std::string() const
		{
			return TypeTraits<Constant<T, N, D> >::name;
		}
	};

	template <char id>
	struct Variable : Expression<Variable<id> >
	{
		constexpr Variable<id> operator()() const
		{
			return{};
		}

		operator std::string() const
		{
			return std::string(1, id);
		}

		LimitParam<id> to(int to) const
		{
			return{ to };
		}
	};

	static constexpr Constant<long double, 314159265358979, 100000000000000> _pi{};
	static constexpr Constant<long double, 271828182845904, 100000000000000> _e{};
	static constexpr Constant<int, 0> _0{};

	template <>
	struct TypeTraits<Constant<long double, 314159265358979, 100000000000000> >
	{
		static constexpr auto name = "pi";
	};
	template <>
	struct TypeTraits<Constant<long double, 271828182845904, 100000000000000> >
	{
		static constexpr auto name = "e";
	};

	static constexpr Variable<'a'> _a{};
	static constexpr Variable<'b'> _b{};
	static constexpr Variable<'c'> _c{};
	static constexpr Variable<'d'> _d{};
	static constexpr Variable<'f'> _f{};
	static constexpr Variable<'g'> _g{};
	static constexpr Variable<'h'> _h{};
	static constexpr Variable<'i'> _i{};
	static constexpr Variable<'j'> _j{};
	static constexpr Variable<'k'> _k{};
	static constexpr Variable<'l'> _l{};
	static constexpr Variable<'m'> _m{};
	static constexpr Variable<'n'> _n{};
	static constexpr Variable<'o'> _o{};
	static constexpr Variable<'p'> _p{};
	static constexpr Variable<'q'> _q{};
	static constexpr Variable<'r'> _r{};
	static constexpr Variable<'s'> _s{};
	static constexpr Variable<'t'> _t{};
	static constexpr Variable<'u'> _u{};
	static constexpr Variable<'v'> _v{};
	static constexpr Variable<'w'> _w{};
	static constexpr Variable<'x'> _x{};
	static constexpr Variable<'y'> _y{};
	static constexpr Variable<'z'> _z{};

	/*
	Operators
	*/
	struct UnaryMinus
	{
	public:
		template <typename T>
		auto operator()(T && value) const
		{
			return -value;
		}

		operator std::string() const
		{
			return "-";
		}
	};

	struct UnaryAddition
	{
	public:
		template <typename T>
		auto operator()(T && value) const
		{
			return ++value;
		}

		operator std::string() const
		{
			return "++";
		}
	};

	struct UnarySubtraction
	{
	public:
		template <typename T>
		auto operator()(T && value) const
		{
			return --value;
		}

		operator std::string() const
		{
			return "--";
		}
	};

	struct Addition
	{
	public:
		template <typename T1, typename T2>
		auto operator()(const T1 && lhs, const T2 && rhs) const
		{
			return lhs + rhs;
		}

		operator std::string() const
		{
			return "+";
		}
	};

	struct Subtraction
	{
	public:
		template <typename T1, typename T2>
		auto operator()(const T1 && lhs, const T2 && rhs) const
		{
			return lhs - rhs;
		}

		operator std::string() const
		{
			return "-";
		}
	};

	struct Multiplication
	{
	public:
		template <typename T1, typename T2>
		auto operator()(const T1 && lhs, const T2 && rhs) const
		{
			return lhs * rhs;
		}

		operator std::string() const
		{
			return "*";
		}
	};

	template <typename T1, typename T2, bool fund>
	struct Div
	{
		static inline auto Get(const T1 & lhs, const T2 & rhs)
		{
			return lhs / rhs;
		}
	};
	template <typename T1, typename T2>
	struct Div<T1, T2, true>
	{
		static inline auto Get(const T1 & lhs, const T2 & rhs)
		{
			return (long double)lhs / (long double)rhs;
		}
	};

	struct Division
	{
	public:
		template <typename T1, typename T2>
		auto operator()(const T1 && lhs, const T2 && rhs) const
		{
			return Div<T1, T2, std::is_fundamental<T1>::value || std::is_fundamental<T2>::value>::Get(lhs, rhs);
		}

		operator std::string() const
		{
			return "/";
		}
	};

	template <std::uintmax_t f>
	struct Factorial
	{
		static constexpr std::uintmax_t value = f * Factorial<f - 1>::value;
	};
	template<>
	struct Factorial<0>
	{
		static const std::uintmax_t value = 1;
	};

	template <std::uintmax_t f>
	static inline auto factorial()
	{
		return Factorial<f>::value;
	}

	template <int p, bool s>
	struct PowBase;
	template <int p>
	struct PowBase<p, false>
	{
		template <typename T>
		static inline auto Get(const T & value)
		{
			return value * PowBase<p - 1, false>::Get(value);
		}
	};
	template <int p>
	struct PowBase<p, true>
	{
		template <typename T>
		static inline auto Get(const T & value)
		{
			return 1 / PowBase<-p, false>::Get(value);
		}
	};
	template <>
	struct PowBase<0, true>
	{
		template <typename T>
		static inline auto Get(const T &)
		{
			return 1;
		}
	};
	template <>
	struct PowBase<0, false>
	{
		template <typename T>
		static inline auto Get(const T &)
		{
			return 1;
		}
	};

	template<int p>
	struct Pow : PowBase<p, p < 0> {};

	template <int p>
	struct Power
	{
	public:
		template <typename T>
		auto operator()(const T && operand) const
		{
			return Pow<p>::Get(operand);
		}

		operator std::string() const
		{
			std::ostringstream ss;
			ss << "^" << p;
			return ss.str();
		}
	};

	template <class Operator, class Operand>
	struct UnaryExpression : Expression<UnaryExpression<Operator, Operand> >
	{
	public:
		UnaryExpression(const Operand& operand)
			: m_operand(operand)
		{
		}

		UnaryExpression(const UnaryExpression& expr)
			: m_operand(expr.m_operand)
		{
		}

		auto operator()() const
		{
			return m_operation(m_operand());
		}

		operator std::string() const
		{
			std::ostringstream ss;
			ss << "(";
			ss << (std::string)m_operation;
			ss << (std::string)m_operand;
			ss << ")";
			return ss.str();
		}

		Operator m_operation;
		Operand m_operand;
	};

	template <class Operator, class Operand>
	struct PostfixExpression : Expression<PostfixExpression<Operator, Operand> >
	{
		PostfixExpression(const Operand& operand)
			: m_operand(operand)
		{
		}

		PostfixExpression(const PostfixExpression& expr)
			: m_operand(expr.m_operand)
		{
		}

		auto operator()() const
		{
			return m_operation(m_operand());
		}

		operator std::string() const
		{
			std::ostringstream ss;
			ss << "((";
			ss << (std::string)m_operand;
			ss << ")";
			ss << (std::string)m_operation;
			ss << ")";
			return ss.str();
		}

		Operator m_operation;
		Operand m_operand;
	};

	template <class Operator, class LeftOperand, class RightOperand>
	struct BinarryExpression : Expression<BinarryExpression<Operator, LeftOperand, RightOperand> >
	{
	public:
		BinarryExpression(const LeftOperand& leftOperand, const RightOperand& rightOperand)
			: m_leftOperand(leftOperand),
			m_rightOperand(rightOperand)
		{
		}

		BinarryExpression(const BinarryExpression & expr)
			: m_leftOperand(expr.m_leftOperand),
			m_rightOperand(expr.m_rightOperand)
		{
		}

		auto operator()() const
		{
			return m_operation(m_leftOperand(), m_rightOperand());
		}

		operator std::string() const
		{
			std::ostringstream ss;
			ss << "(";
			ss << (std::string)m_leftOperand;
			ss << (std::string)m_operation;
			ss << (std::string)m_rightOperand;
			ss << ")";
			return ss.str();
		}

		Operator m_operation;
		LeftOperand m_leftOperand;
		RightOperand m_rightOperand;
	};

	template <class F, class T>
	struct FunctionExpression : Expression<FunctionExpression<F, T> >
	{
	public:
		FunctionExpression(const T& operand)
			: m_operand(operand)
		{
		}

		auto operator()() const
		{
			return m_function(m_operand());
		}

		operator std::string() const
		{
			std::ostringstream ss;
			ss << (std::string)m_function;
			ss << "(";
			ss << (std::string)m_operand;
			ss << ")";
			return ss.str();
		}

		T m_operand;
		F m_function;
	};

	template <typename T, bool fund>
	struct ExpressionOperatorBase
	{
		typedef T type;
		static inline type GetParam(const Expression<T>& arg)
		{
			return arg();
		}
	};
	template <typename T>
	struct ExpressionOperatorBase<T, false>
	{
		typedef Numeric<T> type;
		static inline type GetParam(const T& arg)
		{
			return{ arg };
		}
	};
	template <typename T>
	struct ExpressionOperator : ExpressionOperatorBase<T, std::is_base_of<Expression<T>, T>::value > { };

	template <int p, typename T>
	static inline PostfixExpression<Power<p>, typename ExpressionOperator<T>::type> power(const T & value)
	{
		return{ ExpressionOperator<T>::GetParam(value) };
	}

	template <typename T>
	struct HasVariables
	{
		static constexpr bool value = false;
	};
	template <typename T>
	struct HasVariables<Expression<T> >
	{
		static constexpr bool value = HasVariables<T>::value;
	};
	template <char id>
	struct HasVariables<Variable<id> >
	{
		static constexpr bool value = true;
	};
	template <class T1, class T2>
	struct HasVariables<UnaryExpression<T1, T2> >
	{
		static constexpr bool value = HasVariables<T2>::value;
	};
	template <class T1, class T2, class T3>
	struct HasVariables<BinarryExpression<T1, T2, T3> >
	{
		static constexpr bool value = HasVariables<T2>::value || HasVariables<T3>::value;
	};
	template <class T1, class T2>
	struct HasVariables<FunctionExpression<T1, T2> >
	{
		static constexpr bool value = HasVariables<T2>::value;
	};

	template <typename T1, intmax_t N, intmax_t D, typename T2, typename T3>
	struct Binder<Constant<T1, N, D>, T2, T3>
	{
		typedef Constant<T1, N, D> type;
		static inline type Bind(const Constant<T1, N, D>& exp, const T2&, const T3&)
		{
			return exp;
		}
	};

	template <typename T1, typename T2, typename T3>
	struct Binder<Numeric<T1>, T2, T3>
	{
		typedef Numeric<T1> type;
		static inline type Bind(const Numeric<T1>& exp, const T2&, const T3&)
		{
			return exp;
		}
	};

	template <char id, typename T>
	struct Binder<Variable<id>, Variable<id>, T>
	{
		typedef T type;
		static inline type Bind(const Variable<id>&, const Variable<id>&, const T& value)
		{
			return value;
		}
	};

	template <char id1, char id2, typename T>
	struct Binder<Variable<id1>, Variable<id2>, T>
	{
		typedef Variable<id1> type;
		static inline type Bind(const Variable<id1>& exp, const Variable<id2>&, const T&)
		{
			return exp;
		}
	};

	template <class Operator, class Operand, class T1, typename T2>
	struct Binder<UnaryExpression<Operator, Operand>, T1, T2>
	{
		typedef UnaryExpression<Operator, typename Binder<Operand, T1, T2>::type> type;
		static inline type Bind(const UnaryExpression<Operator, Operand>& exp, const T1& var, const T2& value)
		{
			return{ Binder<Operand, T1, T2>::Bind(exp.m_operand, var, value) };
		}
	};

	template <class Operator, class LeftOperand, class RightOperand, class T1, typename T2>
	struct Binder<BinarryExpression<Operator, LeftOperand, RightOperand>, T1, T2>
	{
		typedef BinarryExpression<Operator, typename Binder<LeftOperand, T1, T2>::type, typename Binder<RightOperand, T1, T2>::type> type;
		static inline type Bind(const BinarryExpression<Operator, LeftOperand, RightOperand>& exp, const T1& var, const T2& value)
		{
			return{ Binder<LeftOperand, T1, T2>::Bind(exp.m_leftOperand, var, value), Binder<RightOperand, T1, T2>::Bind(exp.m_rightOperand, var, value) };
		}
	};

	template <int p, class T, class T1, typename T2>
	struct Binder<PostfixExpression<Power<p>, T>, T1, T2>
	{
		typedef PostfixExpression<Power<p>, typename Binder<T, T1, T2>::type> type;
		static inline type Bind(const PostfixExpression<Power<p>, T> & exp, const T1& var, const T2& value)
		{
			return{ Binder<T, T1, T2>::Bind(exp.m_operand, var, value) };
		}
	};

	template <class F, class T1, class T2, typename T3>
	struct Binder<FunctionExpression<F, T1>, T2, T3>
	{
		typedef FunctionExpression<F, typename Binder<T1, T2, T3>::type> type;
		static inline type Bind(const FunctionExpression<F, T1>& exp, const T2& var, const T3& value)
		{
			return{ Binder<T1, T2, T3>::Bind(exp.m_operand, var, value) };
		}
	};

	template <class T1, class T2, typename T3>
	struct Binder<Expression<T1>, T2, T3>
	{
		typedef typename Binder<T1, T2, T3>::type type;
		static inline type Bind(const T1& exp, const T2& var, const T3& value)
		{
			return Binder<T1, T2, T3>::Bind(exp, var, value);
		}
	};

	template <size_t terms, typename T>
	struct TaylorSineTerm
	{
		static inline auto Get(const T & x)
		{
			return (power<terms>(-1) / factorial<(2 * terms) + 1>()) * power<(2 * terms) + 1>(x);
		}
	};
	template<size_t terms, typename T>
	struct TaylorSine
	{
		static inline auto Get(const T & x)
		{
			return TaylorSineTerm<terms, T>::Get(x) + TaylorSine<terms - 1, T>::Get(x);
		}
	};
	template <typename T>
	struct TaylorSine<0, T>
	{
		static inline auto Get(const T & x)
		{
			return TaylorSineTerm<0, T>::Get(x);
		}
	};

	template <size_t terms, typename T>
	struct TaylorCosineTerm
	{
		static inline auto Get(const T & x)
		{
			return (power<terms>(-1) / factorial<(2 * terms)>()) * power<(2 * terms)>(x);
		}
	};
	template<size_t terms, typename T>
	struct TaylorCosine
	{
		static inline auto Get(const T & x)
		{
			return TaylorCosineTerm<terms, T>::Get(x) + TaylorCosine<terms - 1, T>::Get(x);
		}
	};
	template <typename T>
	struct TaylorCosine<0, T>
	{
		static inline auto Get(const T & x)
		{
			return TaylorCosineTerm<0, T>::Get(x);
		}
	};

	template <size_t terms, typename T>
	struct TaylorETerm
	{
		static inline auto Get(const T & x)
		{
			return power<(terms)>(x) / factorial<(terms)>();
		}
	};
	template<size_t terms, typename T>
	struct TaylorE
	{
		static inline auto Get(const T & x)
		{
			return TaylorETerm<terms, T>::Get(x) + TaylorE<terms - 1, T>::Get(x);
		}
	};
	template <typename T>
	struct TaylorE<0, T>
	{
		static inline auto Get(const T & x)
		{
			return TaylorETerm<0, T>::Get(x);
		}
	};

	template <size_t terms, typename T>
	struct TaylorLnTerm
	{
		static inline auto Get(const T & x)
		{
			return (power<terms + 1>(-1) / terms) * power<(terms)>(-1 + x);
		}
	};
	template<size_t terms, typename T>
	struct TaylorLn
	{
		static inline auto Get(const T & x)
		{
			return TaylorLnTerm<terms, T>::Get(x) + TaylorLn<terms - 1, T>::Get(x);
		}
	};
	template <typename T>
	struct TaylorLn<1, T>
	{
		static inline auto Get(const T & x)
		{
			return TaylorLnTerm<1, T>::Get(x);
		}
	};

	template <typename T>
	static inline auto ln(const T & value)
	{
		return TaylorLn<CONFIDENCE_LEVEL, T>::Get(value);
	}

	struct Sinus
	{
	public:
		template <typename T>
		auto operator()(const T && operand) const
		{
			return Expander<CONFIDENCE_LEVEL, FunctionExpression<Sinus, T>>::Expand(operand)();
		}

		operator std::string() const
		{
			return "sin";
		}
	};

	struct Cosinus
	{
	public:
		template <typename T>
		auto operator()(const T && operand) const
		{
			return Expander<CONFIDENCE_LEVEL, FunctionExpression<Cosinus, T>>::Expand(operand)();
		}

		operator std::string() const
		{
			return "cos";
		}
	};

	template <typename T>
	static inline FunctionExpression<Sinus, typename ExpressionOperator<T>::type> sin(const T& operand)
	{
		return{ ExpressionOperator<T>::GetParam(operand) };
	}

	template <typename T>
	static inline FunctionExpression<Cosinus, typename ExpressionOperator<T>::type> cos(const T& operand)
	{
		return{ ExpressionOperator<T>::GetParam(operand) };
	}

	template <size_t terms, class T>
	struct Expander<terms, Expression<T>>
	{
		static inline auto Expand(const T & exp)
		{
			return Expander<terms, T>::Get(exp());
		}
	};
	template <size_t terms, class T>
	struct Expander<terms, FunctionExpression<Sinus, T>>
	{
		static inline auto Expand(const FunctionExpression<Sinus, T> & exp)
		{
			return TaylorSine<terms, T>::Get(exp.m_operand);
		}
	};
	template <size_t terms, class T>
	struct Expander<terms, FunctionExpression<Cosinus, T>>
	{
		static inline auto Expand(const FunctionExpression<Cosinus, T> & exp)
		{
			return TaylorCosine<terms, T>::Get(exp.m_operand);
		}
	};

	template <size_t terms, class T>
	static inline auto expand(const T & exp)
	{
		return Expander<terms, T>::Expand(exp);
	}

	template <class T, typename T1, typename T2>
	static inline typename Binder<T, T1, typename ExpressionOperator<T2>::type>::type bind(const Expression<T> & expr, const T1& var, const T2& value)
	{
		return Binder<T, T1, typename ExpressionOperator<T2>::type>::Bind(expr(), var, ExpressionOperator<T2>::GetParam(value));
	}

	template <class T>
	static inline auto derive(const Expression<T> & exp)
	{
		return Derivation<T>::Derive(exp());
	}

	template <class T>
	struct Derivation<Expression<T>>
	{
		typedef typename Derivation<T>::type type;
		static inline type Derive(const T& exp)
		{
			return Derivation<T>::Derive(exp);
		}
	};

	template <class T1, class T2>
	struct Derivation<BinarryExpression<Addition, T1, T2>>
	{
		typedef BinarryExpression<Addition, typename Derivation<T1>::type, typename Derivation<T2>::type> type;
		static inline type Derive(const BinarryExpression<Addition, T1, T2> & exp)
		{
			return{ Derivation<T1>::Derive(exp.m_leftOperand), Derivation<T2>::Derive(exp.m_rightOperand) };
		}
	};

	template <class T1, class T2>
	struct Derivation<BinarryExpression<Subtraction, T1, T2>>
	{
		typedef BinarryExpression<Subtraction, typename Derivation<T1>::type, typename Derivation<T2>::type> type;
		static inline type Derive(const BinarryExpression<Subtraction, T1, T2> & exp)
		{
			return{ Derivation<T1>::Derive(exp.m_leftOperand), Derivation<T2>::Derive(exp.m_rightOperand) };
		}
	};

	template <class T1, class T2>
	struct Derivation<BinarryExpression<Multiplication, T1, T2>>
	{
		typedef
			BinarryExpression<
			Addition,
			BinarryExpression<
			Multiplication,
			typename Derivation<T1>::type,
			T2
			>,
			BinarryExpression<
			Multiplication,
			T1,
			typename Derivation<T2>::type
			>
			> type;
		static inline type Derive(const BinarryExpression<Multiplication, T1, T2> & exp)
		{
			return
			{
				{ Derivation<T1>::Derive(exp.m_leftOperand), exp.m_rightOperand },
				{ exp.m_leftOperand, Derivation<T2>::Derive(exp.m_rightOperand) }
			};
		}
	};

	template <class T1, class T2>
	struct Derivation<BinarryExpression<Division, T1, T2>>
	{
		typedef
			BinarryExpression<
			Division,
			BinarryExpression<
			Subtraction,
			BinarryExpression<
			Multiplication,
			typename Derivation<T1>::type,
			T2
			>,
			BinarryExpression<
			Multiplication,
			T1,
			typename Derivation<T2>::type
			>
			>,
			PostfixExpression<Power<2>, T2>
			> type;
		static inline type Derive(const BinarryExpression<Division, T1, T2> & exp)
		{
			return
			{
				{
					{ Derivation<T1>::Derive(exp.m_leftOperand), exp.m_rightOperand },
					{ exp.m_leftOperand, Derivation<T2>::Derive(exp.m_rightOperand) }
				},
				power<2>(exp.m_rightOperand)
			};
		}
	};

	template <class Operator, class Operand>
	struct Derivation<UnaryExpression<Operator, Operand> >
	{
		typedef
			UnaryExpression<
			Operator,
			typename Derivation<Operand>::type
			> type;
		static inline type Derive(const UnaryExpression<Operator, Operand> & exp)
		{
			return{ Derivation<Operand>::Derive(exp.m_operand) };
		}
	};

	template <typename T>
	struct Derivation<Numeric<T>>
	{
		typedef Numeric<int> type;
		static inline type Derive(const Numeric<T> &)
		{
			return{ 0 };
		}
	};

	template <typename T, intmax_t N, intmax_t D>
	struct Derivation<Constant<T, N, D>>
	{
		typedef Numeric<int> type;
		static inline type Derive(const Constant<T, N, D> &)
		{
			return{ 0 };
		}
	};

	template <typename T>
	struct Derivation<FunctionExpression<Sinus, T>>
	{
		typedef BinarryExpression<Multiplication, FunctionExpression<Cosinus, T>, typename Derivation<T>::type> type;
		static inline type Derive(const FunctionExpression<Sinus, T> & exp)
		{
			return
			{
				cos(exp.m_operand),
				Derivation<T>::Derive(exp.m_operand)
			};
		}
	};

	template <typename T>
	struct Derivation<FunctionExpression<Cosinus, T>>
	{
		typedef BinarryExpression<Multiplication, UnaryExpression<UnaryMinus, FunctionExpression<Sinus, T> >, typename Derivation<T>::type> type;
		static inline type Derive(const FunctionExpression<Cosinus, T> & exp)
		{
			return
			{
				-sin(exp.m_operand),
				Derivation<T>::Derive(exp.m_operand)
			};
		}
	};

	template <int p, typename T>
	struct Derivation<PostfixExpression<Power<p>, T>>
	{
		typedef
			BinarryExpression<
			Multiplication,
			BinarryExpression<
			Multiplication,
			Numeric<int>,
			PostfixExpression<
			Power<p - 1>,
			T>
			>,
			typename Derivation<T>::type
			> type;
		static inline type Derive(const PostfixExpression<Power<p>, T> & exp)
		{
			return
			{
				{ p, power<p - 1>(exp.m_operand) },
				{ Derivation<T>::Derive(exp.m_operand) }
			};
		}
	};

	template <char id>
	struct Derivation<Variable<id>>
	{
		typedef Numeric<int> type;
		static inline type Derive(const Variable<id> &)
		{
			return{ 1 };
		}
	};

	template <class T>
	struct Lopital
	{
		typedef T type;
		static inline type Get(const T & expr)
		{
			return expr;
		}
	};

	template <class T>
	struct Lopital<Expression<T>>
	{
		typedef typename Lopital<T>::type type;
		static inline type Get(const Expression<T> & expr)
		{
			return Lopital<T>::Get(expr());
		}
	};

	template <class T1, class T2>
	struct Lopital<BinarryExpression<Division, T1, T2> >
	{
		typedef BinarryExpression<
			Division,
			typename Derivation<T1>::type,
			typename Derivation<T2>::type
		> type;
		static inline type Get(const BinarryExpression<Division, T1, T2> & expr)
		{
			return{ derive(expr.m_leftOperand), derive(expr.m_rightOperand) };
		}
	};

	template <class T>
	static inline auto lopital(const T& expr)
	{
		return Lopital<T>::Get(expr);
	}

	template <typename T>
	struct HasDivision;
	template <char id>
	struct HasDivision<Variable<id>>
	{
		static constexpr bool value = false;
	};
	template <typename T>
	struct HasDivision<Numeric<T>>
	{
		static constexpr bool value = false;
	};
	template <typename T, intmax_t N, intmax_t D>
	struct HasDivision<Constant<T, N, D>>
	{
		static constexpr bool value = false;
	};
	template <class Operator, class LeftOperand, class RightOperand>
	struct HasDivision<BinarryExpression<Operator, LeftOperand, RightOperand>>
	{
		static constexpr bool value = HasDivision<LeftOperand>::value || HasDivision<RightOperand>::value;
	};
	template <class LeftOperand, class RightOperand>
	struct HasDivision<BinarryExpression<Division, LeftOperand, RightOperand>>
	{
		static constexpr bool value = true;
	};
	template <class Operator, class Operand>
	struct HasDivision<UnaryExpression<Operator, Operand>>
	{
		static constexpr bool value = HasDivision<Operand>::value;
	};
	template <class F, class T>
	struct HasDivision<FunctionExpression<F, T>>
	{
		static constexpr bool value = HasDivision<T>::value;
	};
	template <class Operator, class Operand>
	struct HasDivision<PostfixExpression<Operator, Operand>>
	{
		static constexpr bool value = HasDivision<Operand>::value;
	};
	template <class T>
	struct HasDivision<Expression<T>>
	{
		static constexpr bool value = HasDivision<T>::value;
	};

	template <class T1, class T2, bool first, bool second>
	struct DivisionSegregator;
	template <class T1, class T2>
	struct DivisionSegregator<T1, T2, true, false>
	{
		typedef T1 type;
		static inline type Get(const T1 & t1, const T2 &)
		{
			return t1;
		}
	};
	template <class T1, class T2>
	struct DivisionSegregator<T1, T2, false, true>
	{
		typedef T2 type;
		static inline type Get(const T1 &, const T2 & t2)
		{
			return t2;
		}
	};
	template <class T1, class T2>
	struct DivisionSegregator<T1, T2, true, true>
	{
		typedef T1 type;
		static inline type Get(const T1 & t1, const T2 &)
		{
			return t1;
		}
	};

	template <typename T>
	struct FirstDivision;
	template <class Operator, class LeftOperand, class RightOperand>
	struct FirstDivision<BinarryExpression<Operator, LeftOperand, RightOperand>>
	{
		static constexpr bool value = false;
		typedef typename FirstDivision<
			typename DivisionSegregator<
			LeftOperand,
			RightOperand,
			HasDivision<LeftOperand>::value,
			HasDivision<RightOperand>::value
			>::type
		>::type type;
		static inline type Get(const BinarryExpression<Operator, LeftOperand, RightOperand> & expr)
		{
			return FirstDivision<
				typename DivisionSegregator<
				LeftOperand,
				RightOperand,
				HasDivision<LeftOperand>::value,
				HasDivision<RightOperand>::value
				>::type
			>::Get(
				DivisionSegregator<
				LeftOperand,
				RightOperand,
				HasDivision<LeftOperand>::value,
				HasDivision<RightOperand>::value
				>::Get(expr.m_leftOperand, expr.m_rightOperand)
				);
		}
	};
	template <class LeftOperand, class RightOperand>
	struct FirstDivision<BinarryExpression<Division, LeftOperand, RightOperand>>
	{
		static constexpr bool value = true;
		typedef BinarryExpression<Division, LeftOperand, RightOperand> type;
		static inline type Get(const BinarryExpression<Division, LeftOperand, RightOperand> & expr)
		{
			return{ expr.m_leftOperand, expr.m_rightOperand };
		}
	};
	template <class F, class T>
	struct FirstDivision<UnaryExpression<F, T>>
	{
		static constexpr bool value = false;
		typedef typename FirstDivision<T>::type type;
		static inline type Get(const UnaryExpression<F, T> & expr)
		{
			return FirstDivision<T>::Get(expr.m_operand);
		}
	};
	template <class F, class T>
	struct FirstDivision<FunctionExpression<F, T>>
	{
		static constexpr bool value = false;
		typedef typename FirstDivision<T>::type type;
		static inline type Get(const FunctionExpression<F, T> & expr)
		{
			return FirstDivision<T>::Get(expr.m_operand);
		}
	};
	template <class T>
	struct FirstDivision<Expression<T>>
	{
		static constexpr bool value = FirstDivision<T>::value;
		typedef typename FirstDivision<T>::type type;
		static inline type Get(const Expression<T> & expr)
		{
			return FirstDivision<T>::Get(expr());
		}
	};

	template <typename T1, typename T>
	struct SimplifierBase;
	template <class O, class L1, class R1, class L2, class R2>
	struct SimplifierBase<BinarryExpression<O, L1, R1>, BinarryExpression<Division, L2, R2>>
	{
		typedef BinarryExpression<
			Division,
			BinarryExpression<
			O,
			BinarryExpression<
			Multiplication,
			L1,
			R2
			>,
			BinarryExpression<
			Multiplication,
			R1,
			R2
			>
			>,
			R2
		> type;

		static inline type Simplify(const BinarryExpression<O, L1, R1> & lhs, const BinarryExpression<Division, L2, R2> & rhs)
		{
			return
			{
				{
					lhs.m_leftOperand * rhs.m_rightOperand,
					lhs.m_rightOperand * rhs.m_rightOperand
				},
				rhs.m_rightOperand
			};
		}
	};
	template <typename T1, typename T>
	struct SimplifierBase<Expression<T1>, T>
	{
		typedef typename SimplifierBase<T1, T>::type type;
		static inline type Simplify(const Expression<T1> & lhs, const T & rhs)
		{
			return SimplifierBase<T1, T>::Simplify(lhs(), rhs);
		}
	};

	template <typename T1, typename T, bool first>
	struct Simplifier
	{
		typedef typename SimplifierBase<T1, T>::type type;
		static inline type Simplify(const T1 & lhs, const T & rhs)
		{
			return SimplifierBase<T1, T>::Simplify(lhs, rhs);
		}
	};
	template <typename T1, typename T>
	struct Simplifier<T1, T, true>
	{
		typedef T1 type;
		static inline type Simplify(const T1 & expr, const T &)
		{
			return expr;
		}
	};
	template <typename T1, typename T>
	struct Simplifier<Expression<T1>, T, true>
	{
		typedef T1 type;
		static inline type Simplify(const Expression<T1> & expr, const T &)
		{
			return expr();
		}
	};

	template <class T>
	static inline auto simplify(const T & expr)
	{
		return Simplifier<T, typename FirstDivision<T>::type, FirstDivision<T>::value>::Simplify(expr, FirstDivision<T>::Get(expr));
	}


	template <typename T>
	struct CanBeLimited
	{
		static inline bool Get(const T &)
		{
			return true;
		}
	};

	template <typename T>
	struct CanBeLimitedEx
	{
		static inline bool Get(const T & expr)
		{
			return CanBeLimited<T>::Get(expr);
		}
	};
	template <>
	struct CanBeLimitedEx<Numeric<int>>
	{
		static inline bool Get(const Numeric<int> & expr)
		{
			return expr.m_value != 0;
		}
	};

	template <class Operator, class LeftOperand, class RightOperand>
	struct CanBeLimited<BinarryExpression<Operator, LeftOperand, RightOperand>>
	{
		static inline bool Get(const BinarryExpression<Operator, LeftOperand, RightOperand> & expr)
		{
			return CanBeLimited<LeftOperand>::Get(expr.m_leftOperand) && CanBeLimited<RightOperand>::Get(expr.m_rightOperand);
		}
	};
	template <class LeftOperand, class RightOperand>
	struct CanBeLimited<BinarryExpression<Division, LeftOperand, RightOperand>>
	{
		static inline bool Get(const BinarryExpression<Division, LeftOperand, RightOperand> & expr)
		{
			return CanBeLimited<LeftOperand>::Get(expr.m_leftOperand) && CanBeLimitedEx<RightOperand>::Get(expr.m_rightOperand);
		}
	};
	template <class Operator, class Operand>
	struct CanBeLimited<UnaryExpression<Operator, Operand>>
	{
		static inline bool Get(const UnaryExpression<Operator, Operand> & expr)
		{
			return CanBeLimited<Operand>::Get(expr.m_operand);
		}
	};
	template <class F, class T>
	struct CanBeLimited<FunctionExpression<F, T>>
	{
		static inline bool Get(const FunctionExpression<F, T> & expr)
		{
			return CanBeLimited<T>::Get(expr.m_operand);
		}
	};
	template <class Operator, class Operand>
	struct CanBeLimited<PostfixExpression<Operator, Operand>>
	{
		static inline bool Get(const PostfixExpression<Operator, Operand> & expr)
		{
			return CanBeLimited<Operand>::Get(expr.m_operand);
		}
	};
	template <class T>
	struct CanBeLimited<Expression<T>>
	{
		static inline bool Get(const Expression<T> & expr)
		{
			return CanBeLimited<T>::Get(expr());
		}
	};

	template <char id, class T, bool div>
	struct Limit
	{
		static inline long double Get(const LimitParam<id> & param, const Expression<T> & expr)
		{
			auto var = Variable<id>();
			int to = param.m_to;
			auto binded = bind(expr, var, to);
			return binded();
		}
	};
	template <char id, class T>
	struct Limit <id, T, true>
	{
		static inline long double Get(const LimitParam<id> & param, const Expression<T> & expr)
		{
			auto var = Variable<id>();
			int to = param.m_to;

			long double result = 0;

			auto derive1 = simplify(expr);
			if (!CanBeLimited<decltype(derive1)>::Get(derive1)) return result;
			auto bind1 = bind(derive1, var, to);
			result = bind1();
			if (!std::isnan(result)) return result;

			auto derive2 = lopital(derive1);
			if (!CanBeLimited<decltype(derive2)>::Get(derive2)) return result;
			auto bind2 = bind(derive2, var, to);
			result = bind2();
			if (!std::isnan(result)) return result;

			auto derive3 = lopital(derive2);
			if (!CanBeLimited<decltype(derive3)>::Get(derive3)) return result;
			auto bind3 = bind(derive3, var, to);
			result = bind3();
			if (!std::isnan(result)) return result;

			auto derive4 = lopital(derive3);
			if (!CanBeLimited<decltype(derive4)>::Get(derive4)) return result;
			auto bind4 = bind(derive4, var, to);
			result = bind4();
			if (!std::isnan(result)) return result;

			auto derive5 = lopital(derive4);
			if (!CanBeLimited<decltype(derive5)>::Get(derive5)) return result;
			auto bind5 = bind(derive5, var, to);
			result = bind5();
			if (!std::isnan(result)) return result;

			return 0;
		}
	};

	template<char id, class T>
	static inline long double lim(const LimitParam<id> && param, const Expression<T> & expr)
	{
		return Limit<id, T, HasDivision<T>::value>::Get(param, expr);
	}

	template <class T>
	static inline UnaryExpression<UnaryMinus, T> operator-(const Expression<T>& value)
	{
		return UnaryExpression<UnaryMinus, T>(value());
	}

	template <class T>
	static inline UnaryExpression<UnaryAddition, T> operator++(const Expression<T>& value)
	{
		return{ value() };
	}

	template <class T>
	static inline UnaryExpression<UnarySubtraction, T> operator--(const Expression<T>& value)
	{
		return{ value() };
	}

	template <class Lhs, class Rhs>
	static inline BinarryExpression<Addition, typename ExpressionOperator<Lhs>::type, typename ExpressionOperator<Rhs>::type> operator+(const Lhs& d1, const Rhs& d2)
	{
		return{ ExpressionOperator<Lhs>::GetParam(d1), ExpressionOperator<Rhs>::GetParam(d2) };
	}

	template <class Lhs, class Rhs>
	static inline BinarryExpression<Subtraction, typename ExpressionOperator<Lhs>::type, typename ExpressionOperator<Rhs>::type> operator-(const Lhs& d1, const Rhs& d2)
	{
		return{ ExpressionOperator<Lhs>::GetParam(d1), ExpressionOperator<Rhs>::GetParam(d2) };
	}

	template <class Lhs, class Rhs>
	static inline BinarryExpression<Multiplication, typename ExpressionOperator<Lhs>::type, typename ExpressionOperator<Rhs>::type> operator*(const Lhs& d1, const Rhs& d2)
	{
		return{ ExpressionOperator<Lhs>::GetParam(d1), ExpressionOperator<Rhs>::GetParam(d2) };
	}

	template <class Lhs, class Rhs>
	static inline BinarryExpression<Division, typename ExpressionOperator<Lhs>::type, typename ExpressionOperator<Rhs>::type> operator/(const Lhs& d1, const Rhs& d2)
	{
		return{ ExpressionOperator<Lhs>::GetParam(d1), ExpressionOperator<Rhs>::GetParam(d2) };
	}

	template <typename T>
	static inline auto operator^(const Constant<long double, 271828182845904, 100000000000000> &, const T & pow)
	{
		return TaylorE<CONFIDENCE_LEVEL, T>::Get(pow);
	}
}

#endif //!ZINC_HPP_INCLUDED
