#pragma once

class IIndiceBuffer {
	DECLARE_MEMBER( protected, i32, Size )

	public:
		virtual ~IIndiceBuffer() {
			m_Size = 0;
		}

		virtual void BufferElements( std::vector<u32> data, const i32 usage )  = 0;

		virtual void Draw()   const  = 0;
		virtual void Bind()   const  = 0;
		virtual void Unbind() const  = 0;
};
