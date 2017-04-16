#pragma once

namespace aogl {
	
	template <typename T>
	class garbage_collector {

		std::function<void(T)> deleter;

		std::map<T, int> handles;
	public:
		explicit garbage_collector(std::function<void(T)> _deleter);

		void add(T handle);
		void copy(const T& from, T& to);
		void destroy(T& handle);

	};

	template <typename T>
	garbage_collector<T>::garbage_collector(std::function<void(T)> _deleter) : deleter(_deleter) {}

	template <typename T>
	void garbage_collector<T>::add(T handle) {
		handles.insert(std::make_pair(handle, 1));
	}

	template <typename T>
	void garbage_collector<T>::copy(const T& from, T& to) {
		to = from;
		handles[from] += 1;
	}

	template <typename T>
	void garbage_collector<T>::destroy(T& handle) {
		handles[handle] -= 1;
		if (handles[handle] == 0) {
			deleter(handle);
		}
	}

}
