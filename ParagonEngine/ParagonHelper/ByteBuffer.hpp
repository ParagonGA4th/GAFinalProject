///Apache 2.0 License인 ByteBuffer-CPP,
///Paragon Engine 사용 용도로 개조.
///특정 데이터에 종속적이지 않게, ID3D10Blob처럼 데이터를 바이너리로 담기 편함.

#ifndef _ByteBuffer_H_
#define _ByteBuffer_H_

// Default number of uint8_ts to allocate in the backing buffer if no size is provided
#define BB_DEFAULT_SIZE 4096

// If defined, utility functions within the class are enabled
#define BB_UTILITY

#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <vector>
#include <memory>

#include <DirectXMath.h>

#ifdef BB_UTILITY
#include <iostream>
#include <cstdio>
#endif


namespace Pg::Util
{
	class ByteBuffer {
		friend class AssetModelLoader;
		friend class ResourceManager;
	public:
		ByteBuffer(uint32_t size = BB_DEFAULT_SIZE);
		ByteBuffer(uint8_t* arr, uint32_t size);
		~ByteBuffer() = default;

		uint8_t* GetStartAddress();

		uint32_t bytesRemaining(); // Number of uint8_ts from the current read position till the end of the buffer
		void clear(); // Clear our the vector and reset read and write positions
		std::unique_ptr<ByteBuffer> clone(); // Return a new instance of a ByteBuffer with the exact same contents and the same state (rpos, wpos)
		//ByteBuffer compact(); // TODO?
		bool equals(ByteBuffer* other); // Compare if the contents are equivalent
		void resize(uint32_t newSize);
		uint32_t size(); // Size of internal vector

		// Basic Searching (Linear)
		template<typename T> int32_t find(T key, uint32_t start = 0) {
			int32_t ret = -1;
			uint32_t len = buf.size();
			for (uint32_t i = start; i < len; i++) {
				T data = read<T>(i);
				// Wasn't actually found, bounds of buffer were exceeded
				if ((key != 0) && (data == 0))
					break;

				// Key was found in array
				if (data == key) {
					ret = (int32_t)i;
					break;
				}
			}
			return ret;
		}

		// Replacement
		void replace(uint8_t key, uint8_t rep, uint32_t start = 0, bool firstOccuranceOnly = false);

		// Read

		uint8_t peek() const; // Relative peek. Reads and returns the next uint8_t in the buffer from the current position but does not increment the read position
		uint8_t get() const; // Relative get method. Reads the uint8_t at the buffers current position then increments the position
		uint8_t get(uint32_t index) const; // Absolute get method. Read uint8_t at index
		void getBytes(uint8_t* buf, uint32_t len) const; // Absolute read into array buf of length len
		char getChar() const; // Relative
		char getChar(uint32_t index) const; // Absolute
		double getDouble() const;
		double getDouble(uint32_t index) const;
		float getFloat() const;
		float getFloat(uint32_t index) const;
		int32_t getInt() const;
		int32_t getInt(uint32_t index) const;
		uint32_t getUnsignedInt() const;
		uint32_t getUnsignedInt(uint32_t index) const;
		uint64_t getLong() const;
		uint64_t getLong(uint32_t index) const;
		uint16_t getShort() const;
		uint16_t getShort(uint32_t index) const;

		// Write

		void put(ByteBuffer* src); // Relative write of the entire contents of another ByteBuffer (src)
		void put(uint8_t b); // Relative write
		void put(uint8_t b, uint32_t index); // Absolute write at index
		void putBytes(uint8_t* b, uint32_t len); // Relative write
		void putBytes(uint8_t* b, uint32_t len, uint32_t index); // Absolute write starting at index
		void putChar(char value); // Relative
		void putChar(char value, uint32_t index); // Absolute
		void putDouble(double value);
		void putDouble(double value, uint32_t index);
		void putFloat(float value);
		void putFloat(float value, uint32_t index);
		void putUnsignedInt(uint32_t value);
		void putUnsignedInt(uint32_t value, uint32_t index);

		//int32_t는 HLSL 딴에서 BOOL 값이랑 동일하게 활용되어야.
		void putInt(int32_t value); // = Also HLSL BOOL
		void putInt(int32_t value, uint32_t index); //// = Also HLSL BOOL
		void putLong(uint64_t value);
		void putLong(uint64_t value, uint32_t index);
		void putShort(uint16_t value);
		void putShort(uint16_t value, uint32_t index);

		///[TW] DirectX 확장을 위해 확장.
		void PutXMFloat2(DirectX::XMFLOAT2 value);
		void PutXMFloat2(DirectX::XMFLOAT2 value, uint32_t index);
		void PutXMFloat3(DirectX::XMFLOAT3 value);
		void PutXMFloat3(DirectX::XMFLOAT3 value, uint32_t index);
		void PutXMFloat4(DirectX::XMFLOAT4 value);
		void PutXMFloat4(DirectX::XMFLOAT4 value, uint32_t index);

		///전체를 반환하기 위해 설정.
		size_t ReturnAllFromBuffer(uint8_t* _buf);
		// Buffer Position Accessors & Mutators

		void setReadPos(uint32_t r) {
			rpos = r;
		}

		uint32_t getReadPos() const {
			return rpos;
		}

		void setWritePos(uint32_t w) {
			wpos = w;
		}

		uint32_t getWritePos() const {
			return wpos;
		}

		// Utility Functions
#ifdef BB_UTILITY
		void setName(std::string n);
		std::string getName();
		void printInfo();
		void printAH();
		void printAscii();
		void printHex();
		void printPosition();
#endif

	private:
		uint32_t wpos;
		mutable uint32_t rpos;
		std::vector<uint8_t> buf;

#ifdef BB_UTILITY
		std::string name;
#endif

		template<typename T> T read() const {
			T data = read<T>(rpos);
			rpos += sizeof(T);
			return data;
		}

		template<typename T> T read(uint32_t index) const {
			if (index + sizeof(T) <= buf.size())
				return *((T*)&buf[index]);
			return 0;
		}

		template<typename T> void append(T data) {
			uint32_t s = sizeof(data);

			if (size() < (wpos + s))
				buf.resize(wpos + s);
			memcpy(&buf[wpos], (uint8_t*)&data, s);
			//printf("writing %c to %i\n", (uint8_t)data, wpos);

			wpos += s;
		}

		template<typename T> void insert(T data, uint32_t index) {
			if ((index + sizeof(data)) > size()) {
				buf.resize(size() + (index + sizeof(data)));
			}

			memcpy(&buf[index], (uint8_t*)&data, sizeof(data));
			wpos = index + sizeof(data);
		}
	};

}
#endif
