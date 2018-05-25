__global__ void createVertices(float4* positions, float time, unsigned int width, unsigned int height)
{
	unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;

	// Calculate uv coordinates
	float u = x / (float)width; 
	float v = y / (float)height;
	u = u * 2.0f - 1.0f;
	v = v * 2.0f - 1.0f;

	// Calculate simple sine wave pattern
	float freq = 4.0f; 
	float w = sinf(u * freq + time) * cosf(v * freq + time) * 0.5f;

	//write positions
	positions[y*width + x] = make_float4(u, w, v, __int_as_float_(0xff00ff00));
}