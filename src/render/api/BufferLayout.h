#pragma once

#include <vector>
#include <memory>

namespace Engine {

    enum class BufferDataType {
        Int8,   UInt8,
        Int16,  UInt16,
        Int32,  UInt32,
        Int64,  UInt64,
        Float16, Float32, Float64
    };

    struct BufferElementFormat
    {
        BufferDataType type;
        uint32_t count;
        bool normalized = false;

        static uint32_t SizeOfType(BufferDataType type) {
            switch (type)
                {
                    case BufferDataType::Float32: return 4;
                    case BufferDataType::UInt32:  return 4;
                    case BufferDataType::UInt8:   return 1;
                    // TODO: add rest later
                }
                // AE_ASSERT(false); TODO: add engine pch with such utils
                return 0;
        }
    };

    class BufferLayout
    {
    private:
        std::vector<BufferElementFormat> m_Elements;
        unsigned int m_Stride;
    public:
        BufferLayout()
            : m_Stride(0) {}

        template<typename T>
        void Push(unsigned int count, bool normalized = false)
        {
            static_assert(false);
        }

        template<>
        void Push<float>(unsigned int count, bool normalized)
        {
            m_Elements.push_back({BufferDataType::Float32, count, normalized});
            m_Stride += count*sizeof(float);
        }

        template<>
        void Push<unsigned int>(unsigned int count, bool normalized)
        {
            m_Elements.push_back({BufferDataType::UInt32, count, normalized});
            m_Stride += count*sizeof(uint32_t);
        }

        template<>
        void Push<unsigned char>(unsigned int count, bool normalized)
        {
            m_Elements.push_back({BufferDataType::UInt8, count, normalized});
            m_Stride += count*sizeof(uint8_t);
        }

        inline const std::vector<BufferElementFormat>& GetElements() const { return m_Elements; }
        inline unsigned int GetStride() const { return m_Stride; }
    };
} // namespace ae