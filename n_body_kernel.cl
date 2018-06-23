__kernel void nbody_kernel(
	 float timestep, float eps,
	__global float4* pos_old, 
	__global float4* pos_new,
	__global float4* vel,
	__local float4* pblock
	)
{
	const float4 dt = float4(timestep, timestep, timestep, 0.0f);
	int global_id = get_global_id(0);
	int local_id = get_local_id(0);

	int n = get_global_size(0);
	int nt = get_local_size(0);
	int nb = n/nt;

	__local float4* pblock;

	float4 p = pos_old[global_id];
	float4 v = vel[global_id];
	float4 a = (float4)(0.0f, 0.0f, 0.0f, 0.0f);

	for(int jb = 0; jb < nb; jb++)
	{
		pblock[ti] = pos_old[jb*nt + local_id];
		barrier(CLK_LOCAL_MEM_FENCE);

		for (int j=0; j < nt; j++ )
		{
			float4 p2 =pblock[j];
			float4 d = p2-p;
			float r_ = rsqrt(d.x*d.x + d.y*d.y + d.z*d.z + eps);
			float f = p2.w*r_*r_*r_;
			a+=f*d;
		}
		barrier(CLK_LOCAL_MEM_FENCE);

		p += dt*v + 0.5f*dt*dt*a;
		v+=dt*a;

		pos_new[global_id] = p;
		vel[global_id] = v;

	}
}