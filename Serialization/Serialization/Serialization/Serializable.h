#pragma once
namespace zpc {
	namespace serialize {
		class DataStream;
		class Serializable
		{
		public:
			virtual void serializ(DataStream& stream) const = 0;
			virtual bool unserializ(DataStream& stream)= 0;
		};
	}
}