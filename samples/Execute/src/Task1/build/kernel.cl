__kernel void doubleInput(__global float* input, __global float* output)
{
int tid = get_global_id(0);
output[tid]= 2*3.14575454*0.2312232*1.3232*0.99943*input[tid];
}
