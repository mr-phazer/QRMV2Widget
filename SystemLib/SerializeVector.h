#pragma once
#include "ISerializeabe.h"

namespace SystemLib
{
	template <class T>
	class SerializableVector :
		public ISerializable
	{
	public:
		SerializableVector()
		{}

		SerializableVector(size_t _size)
		{
			vecData.resize(_size);
		}

		/// <summary>
		/// Reads the contents of the vector into the data_stream
		/// </summary>t
		/// <param name="_DataStream">data_stream that is read into</param>
		/// <returns>true</returns>
		virtual bool read(DataStream& _DataStream)
		{
			for (auto& it : vecData)
			{
				int debug_1 = 1;
			}

			return true;
		};

		/// <summary>
		/// Writes the contents of the vector to the data_stream
		/// </summary>t
		/// <param name="_DataStream">data_stream that is written to</param>
		/// <returns>true</returns>
		virtual bool write(DataStream& _DataStream)
		{
			for (auto& it : vecData)
				_DataStream.writeMem(&it, sizeof(it));

			return true;
		};

		/// <summary>
		/// Returns total size of vector in bytes
		/// </summary>
		/// <returns>Total size in bytes</returns>
		virtual size_t size()
		{
			return sizeof(T) * vecData.size();
		};

		/// <summary>
		/// Returns reference to the vector
		/// </summary>
		/// <returns>reference to vector</returns>
		std::vector<T>& get() {
			return vecData;
		}

	protected:
		std::vector<T> vecData;
	};
}
