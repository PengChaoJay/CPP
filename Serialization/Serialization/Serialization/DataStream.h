#pragma once
#include <vector>
#include <map>
#include <list>
#include <set>
#include <iostream>
#include <algorithm>
#include "Serializable.h"
using namespace std;
namespace zpc {
	namespace serialize {
		class DataStream {
		public:
			// support DataType
			enum DataType
			{
				BOOL = 0,
				CHAR,
				INT32,
				INT64,
				FLOAT,
				DOUBLE,
				STRING,
				VECTOR,
				LIST,
				MAP,
				SET,
				CUSTOM
			};
			void show() const;
			DataStream() :m_pos(0) {}
			~DataStream() {}
			// data Represent FirstAddress of write to m_buf
			void write(const char* data, int len);
			void write(bool value);
			void write(char value);
			void write(int32_t value);
			void write(int64_t value);
			void write(float value);
			void write(double value);
			void write(const char* value);
			void write(const string& value);
			void write(const Serializable & value);

			template <typename T,typename ...Args>
			void write_args(const T& head, const Args & ...args);
			void write_args();

			template<typename T>
			void write(const std::vector<T>& value);
			
			template<typename T>
			void write(const std::list<T>& value);
			
			template<typename K,typename V>
			void write(const std::map<K,V>& value);

			template<typename T>
			void write(const std::set<T>& value);

			bool read(char* type, int len);

			bool read(bool& value);
			bool read(char& value);
			bool read(int32_t& value);
			bool read(int64_t& value);
			bool read(float& value);
			bool read(double& value);
			bool read(string& value);
			bool read(Serializable & value);
			template <typename T, typename ... Args>
			bool read_args(T& head, Args & ...args);
			bool read_args();

			template<typename T>
			bool read(std::vector<T> & value);
			template<typename T>
			bool read(std::list<T>& value);

			template<typename K, typename V>
			bool read(std::map<K, V>& value);

			template<typename T>
			bool read(std::set<T>& value);


			DataStream& operator <<(bool value);
			DataStream& operator <<(char value);
			DataStream& operator <<(int32_t value);
			DataStream& operator <<(int64_t value);
			DataStream& operator <<(float value);
			DataStream& operator <<(double value);
			DataStream& operator <<(const char* value);
			DataStream& operator <<(const string& value);
			DataStream& operator <<(const Serializable& value);
			template<typename T>
			DataStream& operator <<(const std::vector<T> & value);
			template<typename T>
			DataStream& operator <<(const std::list<T>& value);
			template<typename K,typename V>
			DataStream& operator <<(const std::map<K,V> & value);
			template<typename T>
			DataStream& operator <<(const std::set<T> & value);

			DataStream& operator >> (bool& value);
			DataStream& operator >> (char& value);
			DataStream& operator >> (int32_t& value);
			DataStream& operator >> (int64_t& value);
			DataStream& operator >> (float& value);
			DataStream& operator >> (double& value);
			DataStream& operator >> (string& value);
			DataStream& operator >> (Serializable& value);

			template<typename T>
			DataStream& operator >>(std::vector<T>& value);
			template<typename T>
			DataStream& operator >>(std::list<T>& value);
			template<typename K, typename V>
			DataStream& operator >> (std::map<K, V>& value);
			template<typename T>
			DataStream& operator >> (std::set<T>& value);




		private:
			void reserve(int len);
		private:
			std::vector<char>m_buf;
			int m_pos;
		};

		// expand capcity
		void DataStream::reserve(int len)
		{
			int size = m_buf.size();
			int cap = m_buf.capacity();
			if (size + len > cap)
			{
				while (size + len > cap)
				{
					if (cap == 0)
					{
						cap = 1;
					}
					else
					{
						cap *= 2;
					}
				}
				m_buf.reserve(cap);

			}
		}
		void DataStream::show()const
		{
			int size = m_buf.size();
			std::cout << "data size" << size << std::endl;
		}




		void DataStream::write(const char* data, int len)
		{
			reserve(len);
			int size = m_buf.size();
			m_buf.resize(size + len);
			std::memcpy(&m_buf[size], data, len);
		}
		void DataStream::write(bool value)
		{
			char type = DataType::BOOL;
			write((char*)&type, sizeof(char));
			write((char*)&value, sizeof(char));
		}
		void DataStream::write(char value)
		{
			char type = DataType::CHAR;
			write((char*)&type, sizeof(char));
			write((char*)&value, sizeof(char));
		}
		void DataStream::write(int32_t value)
		{
			char type = DataType::INT32;
			write((char*)&type, sizeof(char));
			write((char*)&value, sizeof(int32_t));
		}
		void DataStream::write(int64_t value)
		{
			char type = DataType::INT64;
			write((char*)&type, sizeof(char));
			write((char*)&value, sizeof(int64_t));
		}
		void DataStream::write(float value)
		{
			char type = DataType::FLOAT;
			write((char*)&type, sizeof(char));
			write((char*)&value, sizeof(float));
		}
		void DataStream::write(double value)
		{
			char type = DataType::DOUBLE;
			write((char*)&type, sizeof(char));
			write((char*)&value, sizeof(double));
		}
		void DataStream::write(const char* value)
		{
			char type = DataType::STRING;
			write((char*)&type, sizeof(char));
			int len = strlen(value);
			write(len);
			write(value, len);
		}
		void DataStream::write(const string& value)
		{
			char type = DataType::STRING;
			write((char*)&type, sizeof(char));
			int len = value.size();
			write(len);
			write(value.data(), len);
		}
		// custom type coding 
		void DataStream::write(const Serializable & value)
		{
			value.serialize(*this);
		}
		//Compound type coding
		template<typename T>
		void DataStream::write(const vector<T>& value)
		{
			char type = DataType::VECTOR;
			write((char*)&type, sizeof(char));
			int len = value.size();
			write(len);
			for (auto it = value.begin();it!=value.end();it++)
			{
				write(*it);
			}
		}

		template<typename T>
		void DataStream::write(const list<T>& value)
		{
			char type = DataType::List;
			write((char*)&type, sizeof(char));
			int len = value.size();
			write(len);
			for (auto it = value.begin(); it != value.end(); it++)
			{
				write(*it);
			}
		}

		template<typename K, typename V>
		void DataStream::write(const map<K, V> & value)
		{
			char type = DataType::MAP;
			write((char*)&type, sizeof(char));
			int len = value.size();
			write(len);
			for (auto it = value.begin();it!=value.end();it++)
			{
				write(it->first);
				write(it->second);
			}
		}

		template<typename T>
		void DataStream::write(const set<T>& value)
		{
			char type = DataType::SET;
			write((char*)&type, sizeof(char));
			int len = value.size();
			write(len);
			for (auto it = value.begin(); it != value.end(); it++)
			{
				write(*it);
			}
		}
		template <typename T, typename ... Args>
		void DataStream::write_args(const T& head, const Args & ...args)
		{
			write(head);
			write_args(args...);
		}
		void DataStream::write_args()
		{

		}



		bool DataStream::read(char* data, int len)
		{
			std::memcpy(data, (char *)&m_buf[m_pos], len);
			m_pos += len;
			return true;
		}

		bool DataStream::read(bool & value)
		{
			if (m_buf[m_pos] != DataType::BOOL)
			{
				return false;
			}
			++m_pos;
			value = m_buf[m_pos];
			++m_pos;
			return true;
		}
		bool DataStream::read(char & value)
		{
			if (m_buf[m_pos] != DataType::CHAR)
			{
				return false;
			}
			++m_pos;
			value = m_buf[m_pos];
			++m_pos;
			return true;
		}
		bool DataStream::read(int32_t& value)
		{
			if (m_buf[m_pos] != DataType::INT32)
			{
				return false;
			}
			++m_pos;
			value = *((int32_t*)(&m_buf[m_pos]));
			m_pos += 4;
			return true;
		}
		bool DataStream::read(int64_t& value)
		{
			if (m_buf[m_pos] != DataType::INT64)
			{
				return false;
			}
			++m_pos;
			value = *((int64_t*)(&m_buf[m_pos]));
			m_pos += 8;
			return true;
		}
		bool DataStream::read(float& value)
		{
			if (m_buf[m_pos] != DataType::FLOAT)
			{
				return false;
			}
			++m_pos;
			value = *((float*)(&m_buf[m_pos]));
			m_pos += 4;
			return true;
		}
		bool DataStream::read(double& value)
		{
			if (m_buf[m_pos] != DataType::DOUBLE)
			{
				return false;
			}
			++m_pos;
			value = *((double*)(&m_buf[m_pos]));
			m_pos += 8;
			return true;
		}
		bool DataStream::read(string& value)
		{
			if (m_buf[m_pos] != DataType::STRING)
			{
				return false;
			}
			++m_pos;
			int len;
			read(len);
			if (len < 0)
			{
				return false;
			}
			value.assign((char*)&m_buf[m_pos], len);
			m_pos += len;
			return true;
		}
		bool DataStream::read(Serializable& value)
		{
			value.unserialize(*this);
			return true;
		}

		template <typename T, typename ... Args>
		bool DataStream::read_args(T& head,  Args & ...args)
		{
			read(head);
			return read_args(args...);
		}
		bool DataStream::read_args()
		{
			return true;
		}



		template<typename T>
		bool DataStream::read(std::vector<T>& value)
		{
			value.clear();
			if (m_buf[m_pos] != DataType::VECTOR)
			{
				return false;
			}
			++m_pos;
			int len;
			read(len);
			for (auto i =0;i<len;i++)
			{
				T v;
				read(v);
				value.emplace_back(v);
			}
			return true;
		}
		template<typename T>
		bool DataStream::read(std::list<T>& value)
		{
			value.clear();
			if (m_buf[m_pos] != DataType::LIST)
			{
				return false;
			}
			++m_pos;
			int len;
			read(len);
			for (auto i =0;i<len;i++)
			{
				T v;
				read(v);
				value.emplace_back(v);
			}
			return true;
		}

		template<typename K, typename V>
		bool DataStream::read(std::map<K, V>& value)
		{
			value.clear();
			if (m_buf[m_pos] != DataType::MAP)
			{
				return false;
			}
			++m_pos;
			int len;
			read(len);
			for (auto i = 0; i < len; i++)
			{
				K k;
				read(k);
				V v;
				read(v);
				value[k] = v;
			}
			return true;

		}

		template<typename T>
		bool DataStream::read(std::set<T>& value)
		{
			value.clear();
			if (m_buf[m_pos] != DataType::SET)
			{
				return false;
			}
			++m_pos;
			int len;
			read(len);
			for (auto i = 0; i < len; i++)
			{
				T v;
				read(v);
				value.emplace(v);
			}
			return true;
		}



		DataStream& DataStream::operator <<(bool value)
		{
			write(value);
			return *this;
		}
		DataStream& DataStream::operator <<(char value)
		{
			write(value);
			return *this;
		}
		DataStream& DataStream::operator <<(int32_t value)
		{
			write(value);
			return *this;
		}
		DataStream& DataStream::operator <<(int64_t value)
		{
			write(value);
			return *this;
		}
		DataStream& DataStream::operator <<(float value)
		{
			write(value);
			return *this;
		}
		DataStream& DataStream::operator <<(double value)
		{
			write(value);
			return *this;
		}
		DataStream& DataStream::operator <<(const char* value)
		{
			write(value);
			return *this;
		}
		DataStream& DataStream::operator <<(const string& value)
		{
			write(value);
			return *this;
		}
		DataStream& DataStream::operator <<(const Serializable& value)
		{
			write(value);
			return *this;
		}

		template<typename T>
		DataStream& DataStream::operator <<(const std::vector<T>& value)
		{
			write(value);
			return *this;
		}
		template<typename T>
		DataStream& DataStream::operator <<(const std::list<T>& value)
		{
			write(value);
			return *this;
		}
		template<typename K, typename V>
		DataStream& DataStream::operator <<(const std::map<K, V>& value)
		{
			write(value);
			return *this;
		}
		template<typename T>
		DataStream& DataStream::operator <<(const std::set<T>& value)
		{
			write(value);
			return *this;
		}





		DataStream& DataStream::operator >> (bool& value)
		{
			read(value);
			return *this;

		}
		DataStream& DataStream::operator >> (char& value)
		{
			read(value);
			return *this;
		}
		DataStream& DataStream::operator >> (int32_t& value)
		{
			read(value);
			return *this;
		}
		DataStream& DataStream::operator >> (int64_t& value)
		{
			read(value);
			return *this;
		}
		DataStream& DataStream::operator >> (float& value)
		{
			read(value);
			return *this;
		}
		DataStream& DataStream::operator >> (double& value)
		{
			read(value);
			return *this;
		}
		DataStream& DataStream::operator >> (string& value)
		{
			read(value);
			return *this;
		}
		DataStream& DataStream::operator >> (Serializable & value)
		{
			read(value);
			return *this;
		}
		template<typename T>
		DataStream& DataStream::operator >>(std::vector<T>& value)
		{
			read(value);
			return *this;
		}
		template<typename T>
		DataStream& DataStream::operator >>(std::list<T>& value)
		{
			read(value);
			return *this;

		}
		template<typename K, typename V>
		DataStream& DataStream::operator >> (std::map<K, V>& value)
		{
			read(value);
			return *this;
		}
		template<typename T>
		DataStream& DataStream::operator >> (std::set<T>& value)
		{
			read(value);
			return *this;
		}

	}
}