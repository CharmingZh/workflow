#ifndef _BENCHMARK_ARGS_H_
#define _BENCHMARK_ARGS_H_

#include <algorithm>
#include <numeric>
#include <string>

namespace details
{
	inline bool extract(const char * p, size_t & t)
	{
		char * e;
		long long ll = std::strtoll(p, &e, 0);
		if (*e || ll < 0)
		{
			return false;
		}
		t = static_cast<size_t>(ll);
		return true;
	}

	inline bool extract(const char * p, unsigned short & t)
	{
		char * e;
		long long ll = std::strtoll(p, &e, 0);
		if (*e
		    || ll < static_cast<long long>(std::numeric_limits<unsigned short>::min())
		    || ll > static_cast<long long>(std::numeric_limits<unsigned short>::max())
			)
		{
			return false;
		}
		t = static_cast<unsigned short>(ll);
		return true;
	}

	inline bool extract(const char * p, std::string & t)
	{
		t = p;
		return true;
	}

	inline bool extract(const char * p, const char *& t)
	{
		t = p;
		return true;
	}

	template <typename Arg>
	inline bool extract_impl(size_t todo, int & index, bool & r, char **& p, Arg & arg)
	{
		if (!r || index >= todo)
		{
			return false;
		}

		r = extract(*p, arg);

		if (!r)
		{
			return false;
		}

		index++;
		p++;

		return true;
	}

	template <typename ... ARGS>
	inline int parse(size_t todo, char ** p, ARGS & ... args)
	{
		int index = 0;
		bool r = true;
		(void)std::initializer_list<int>{(extract_impl(todo, index, r, p, args), 0)...};
		return index;
	}

	template <typename ... ARGS>
	inline int parse_args(int & argc, char ** argv, ARGS & ... args)
	{
		if (argc <= 1)
		{
			return 0;
		}

		int length = argc - 1;
		char ** begin = argv + 1;
		char ** end = begin + length;

		int done = parse(length, begin, args ...);
		std::rotate(begin, begin + done, end);
		std::reverse(end - done, end);

		argc -= done;
		return done;
	}
}

template <typename ... ARGS>
inline static int parse_args(int & argc, char ** argv, ARGS & ... args)
{
	return details::parse_args(argc, argv, args ...);
}

#endif //_BENCHMARK_ARGS_H_
