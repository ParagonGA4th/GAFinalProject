#ifndef __DEFINED_APPENDS_SAMPLER_STATES_HLSL__
#define __DEFINED_APPENDS_SAMPLER_STATES_HLSL__

//���� SamplerState�� �ٸ��� �������� �ʾ���.

//Full Screen Quad : Sampler State.
SamplerState fullScreenQuadSS : register(s0);
SamplerState lightmapSS : register(s1);
SamplerState defaultTextureSS : register(s2);
SamplerState lutSS : register(s3);

#endif //__DEFINED_APPENDS_SAMPLER_STATES_HLSL__