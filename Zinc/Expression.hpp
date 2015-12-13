#ifndef ZN_EXPRESSION_HPP_INCLUDED
#define ZN_EXPRESSION_HPP_INCLUDED

#include <string>
#include <sstream>
#include <type_traits>
#include <cmath>

namespace Zinc
{
	template <typename T>
	struct TypeTraits;

	template <typename T>
	struct Numeric;

	template <typename T, intmax_t N, intmax_t D = 1>
	struct Constant;

	template <class T1, class T2, typename T3>
	struct Binder;

	template <class T>
	struct Simplifier;

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
	public:
		Numeric(const T & value)
			: m_value(value)
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

	private:
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
	};

	static constexpr Constant<long double, 314159265358979, 100000000000000> _pi{};
	static constexpr Constant<long double, 271828182845904, 100000000000000> _e{};

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
			return value * Pow<p - 1>::Get(value);
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

	template <int p>
	struct IsNegative
	{
		static const bool value = p < 0;
	};

	template<int p>
	struct Pow : PowBase<p, IsNegative<p>::value>{};

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
	struct PosfixExpression : Expression<PosfixExpression<Operator, Operand> >
	{
	public:
		PosfixExpression(const Operand& operand)
			: m_operand(operand)
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
	static inline PosfixExpression<Power<p>, typename ExpressionOperator<T>::type> power(const T & value)
	{
		return{ ExpressionOperator<T>::GetParam(value) };
	}

	template <typename T>
	static inline auto ln(const T & value)
	{
		return TaylorLn<5, T>::Get(value);
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

	struct Sinus
	{
	public:
		template <typename T>
		auto operator()(const T && operand) const
		{
			return Sin<HasVariables<T>::value>::Operate(operand);
		}

		template <typename T>
		auto operator()(const Expression<T> && operand) const
		{
			return Sin<HasVariables<T>::value>::Operate(operand());
		}

		operator std::string() const
		{
			return "sin";
		}

	private:
		template <bool vars>
		struct Sin
		{
			template <typename T>
			static inline auto Operate(const T & value)
			{
				return ::sin(value);
			}

			static inline auto Operate(float value)
			{
				return ::sinf(value);
			}

			static inline auto Operate(double value)
			{
				return ::sin(value);
			}

			static inline auto Operate(long double value)
			{
				return ::sinl(value);
			}
		};
		template <>
		struct Sin<true>
		{
			template <typename T>
			static inline FunctionExpression<Sinus, T> Operate(const Expression<T> & expr)
			{
				return{ expr() };
			}
		};
	};

	struct Cosinus
	{
	public:
		template <typename T>
		auto operator()(const T && operand) const
		{
			return Cos<HasVariables<T>::value>::Operate(operand);
		}

		template <typename T>
		auto operator()(const Expression<T> && operand) const
		{
			return Cos<HasVariables<T>::value>::Operate(operand());
		}

		operator std::string() const
		{
			return "cos";
		}

	private:
		template <bool vars>
		struct Cos
		{
			template <typename T>
			static inline auto Operate(const T & value)
			{
				return ::cos(value);
			}

			static inline auto Operate(float value)
			{
				return ::cosf(value);
			}

			static inline auto Operate(double value)
			{
				return ::cos(value);
			}

			static inline auto Operate(long double value)
			{
				return ::cosl(value);
			}
		};
		template <>
		struct Cos<true>
		{
			template <typename T>
			static inline FunctionExpression<Cosinus, T> Operate(const Expression<T> & expr)
			{
				return{ expr() };
			}
		};
	};

	struct Tangent
	{
	public:
		template <typename T>
		auto operator()(const T && operand) const
		{
			return Tan<HasVariables<T>::value>::Operate(operand);
		}

		template <typename T>
		auto operator()(const Expression<T> && operand) const
		{
			return Tan<HasVariables<T>::value>::Operate(operand());
		}

		operator std::string() const
		{
			return "tan";
		}

	private:
		template <bool vars>
		struct Tan
		{
			template <typename T>
			static inline auto Operate(const T & value)
			{
				return ::tan(value);
			}

			static inline auto Operate(float value)
			{
				return ::tanf(value);
			}

			static inline auto Operate(double value)
			{
				return ::tan(value);
			}

			static inline auto Operate(long double value)
			{
				return ::tanl(value);
			}
		};
		template <>
		struct Tan<true>
		{
			template <typename T>
			static inline FunctionExpression<Tangent, T> Operate(const Expression<T> & expr)
			{
				return{ expr() };
			}
		};
	};

	template <typename T>
	static inline FunctionExpression<Sinus, T> sin(const T& operand)
	{
		return{ operand };
	}

	template <typename T>
	static inline FunctionExpression<Cosinus, T> cos(const T& operand)
	{
		return{ operand };
	}

	template <typename T>
	static inline FunctionExpression<Tangent, T> tan(const T& operand)
	{
		return{ operand };
	}

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
			return (power<terms+1>(-1) / terms) * power<(terms)>(-1 + x);
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



	template <size_t terms, class T>
	struct Expander;
	template <size_t terms, class T>
	struct Expander<terms, Expression<T>>
	{
		static inline auto Expand(const T & exp)
		{
			return Series<terms, T>::Get(exp());
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
	static inline typename Binder<T, T1, typename ExpressionOperator<T2>::type>::type Bind(const Expression<T> & expr, const T1& var, const T2& value)
	{
		return Binder<T, T1, typename ExpressionOperator<T2>::type>::Bind(expr(), var, ExpressionOperator<T2>::GetParam(value));
	}

	template <typename T>
	struct Simplifier
	{

	};

	template <typename T>
	static inline typename Simplifier<T>::type Simplify(const Expression<T> & expr)
	{
		return Simplifier<T>::Simplify(expr());
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
	static inline auto operator^(const Constant<long double, 271828182845904, 100000000000000> & e, const T & pow)
	{
		return TaylorE<5, T>::Get(pow);
	}
}

#endif //!ZN_EXPRESSION_HPP_INCLUDED
