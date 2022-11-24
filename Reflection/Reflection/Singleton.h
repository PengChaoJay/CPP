#pragma once
namespace zpc
{
	namespace utility
	{
		template <typename T>
		class Singleton
		{
		public:
			static T* instance()
			{
				if (m_instance == nullptr)
				{
					m_instance = new T();
				}
				return m_instance;

			}
		private:
			static T* m_instance;
		private:
			Singleton() {}
			~Singleton() {}
			Singleton(const Singleton<T>&) = delete;
			Singleton <T>& operator = (const Singleton<T>&) = delete;
		};
		template<typename T>
		T* Singleton<T>::m_instance = nullptr;
	}
}