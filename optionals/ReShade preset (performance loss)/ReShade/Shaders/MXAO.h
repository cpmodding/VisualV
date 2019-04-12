//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ReShade 3.0 effect file
// visit facebook.com/MartyMcModding for news/updates
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Ambient Obscurance with Indirect Lighting "MXAO" 3.4.3 by Marty McFly
// CC BY-NC-ND 3.0 licensed.
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Preprocessor Settings
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef MXAO_MIPLEVEL_AO
 #define MXAO_MIPLEVEL_AO		0	//[0 to 2]      Miplevel of AO texture. 0 = fullscreen, 1 = 1/2 screen width/height, 2 = 1/4 screen width/height and so forth. Best results: IL MipLevel = AO MipLevel + 2
#endif

#ifndef MXAO_MIPLEVEL_IL
 #define MXAO_MIPLEVEL_IL		2	//[0 to 4]      Miplevel of IL texture. 0 = fullscreen, 1 = 1/2 screen width/height, 2 = 1/4 screen width/height and so forth.
#endif

#ifndef MXAO_ENABLE_IL
 #define MXAO_ENABLE_IL			1	//[0 or 1]	Enables Indirect Lighting calculation. Will cause a major fps hit.
#endif

#ifndef MXAO_SMOOTHNORMALS
 #define MXAO_SMOOTHNORMALS             0       //[0 or 1]      This feature makes low poly surfaces smoother, especially useful on older games.
#endif

#ifndef MXAO_TWO_LAYER
 #define MXAO_TWO_LAYER                 0       //[0 or 1]      Splits MXAO into two separate layers that allow for both large and fine AO.
#endif

#ifndef MXAO_ENABLE_TSS
 #define MXAO_ENABLE_TSS                0       //[0 or 1]      Combines the current frame AO with older frames to improve quality a LOT, at the expense of some ghosting.
#endif

#define EXTRASUNNY_FADEOUT 		1.0f
#define CLEAR_FADEOUT 			1.0f
#define NEUTRAL_FADEOUT 		1.0f
#define SMOG_FADEOUT			0.5f
#define FOGGY_FADEOUT			0.4f
#define OVERCAST_FADEOUT		1.0f
#define CLOUDS_FADEOUT			0.9f
#define CLEARING_FADEOUT		0.7f
#define RAIN_FADEOUT			0.6f
#define THUNDER_FADEOUT			0.4f
#define SNOW_FADEOUT			0.5f
#define BLIZZARD_FADEOUT		0.4f
#define LIGHTSNOW_FADEOUT		0.7f
#define XMAS_FADEOUT			0.6f
#define HALLOWEEN_FADEOUT		0.6f
#define NULL_FADEOUT			0.0f

#define MXAO_H0			0.4f
#define MXAO_H1			0.4f
#define MXAO_H2			0.4f
#define MXAO_H3			0.4f
#define MXAO_H4			0.4f
#define MXAO_H5			0.6f
#define MXAO_H6			0.8f
#define MXAO_H7			1.0f
#define MXAO_H8			1.0f
#define MXAO_H9			1.0f
#define MXAO_H10		1.0f
#define MXAO_H11		1.0f
#define MXAO_H12		1.0f
#define MXAO_H13		1.0f
#define MXAO_H14		1.0f
#define MXAO_H15		1.0f
#define MXAO_H16		1.0f
#define MXAO_H17		1.0f
#define MXAO_H18		1.0f
#define MXAO_H19		1.0f
#define MXAO_H20		0.8f
#define MXAO_H21		0.6f
#define MXAO_H22		0.4f
#define MXAO_H23		0.4f

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// UI variables
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

uniform int MXAO_GLOBAL_SAMPLE_QUALITY_PRESET <
	ui_type = "combo";
	ui_label = "Sample Quality";
	ui_items = "Very Low\0Low\0Medium\0High\0Very High\0Ultra\0Maximum\0";
	ui_tooltip = "Global quality control, main performance knob. Higher radii might require higher quality.";
> = 1;

uniform float MXAO_SAMPLE_RADIUS <
	ui_type = "drag";
	ui_min = 1.0; ui_max = 20.0;
	ui_label = "Sample Radius";
	ui_tooltip = "Sample radius of MXAO, higher means more large-scale occlusion with less fine-scale details.";
> = 8.0;

uniform float MXAO_SAMPLE_NORMAL_BIAS <
	ui_type = "drag";
	ui_min = 0.0; ui_max = 0.8;
	ui_label = "Normal Bias";
	ui_tooltip = "Occlusion Cone bias to reduce self-occlusion of surfaces that have a low angle to each other.";
> = 0.2;

uniform float MXAO_GLOBAL_RENDER_SCALE <
	ui_type = "drag";
	ui_label = "Render Size Scale";
	ui_min = 0.50; ui_max = 1.00;
	ui_tooltip = "Factor of MXAO resolution, lower values greatly reduce performance overhead but decrease quality.\n1.0 = MXAO is computed in original resolution\n0.5 = MXAO is computed in 1/2 width 1/2 height of original resolution\n...";
> = 1.0;

uniform float MXAO_SSAO_AMOUNT <
	ui_type = "drag";
	ui_min = 0.00; ui_max = 3.00;
	ui_label = "Ambient Occlusion Amount";
	ui_tooltip = "Intensity of AO effect. Can cause pitch black clipping if set too high.";
> = 2.00;

#if(MXAO_ENABLE_IL != 0)
        uniform float MXAO_SSIL_AMOUNT <
        	ui_type = "drag";
        	ui_min = 0.00; ui_max = 12.00;
			ui_label = "Indirect Lighting Amount";
        	ui_tooltip = "Intensity of IL effect. Can cause overexposured white spots if set too high.";
        > = 3.00;

        uniform float MXAO_SSIL_SATURATION <
        	ui_type = "drag";
        	ui_min = 0.00; ui_max = 3.00;
			ui_label = "Indirect Lighting Saturation";
        	ui_tooltip = "Controls color saturation of IL effect.";
        > = 1.50;
#endif

#if (MXAO_TWO_LAYER != 0)
        uniform float MXAO_SAMPLE_RADIUS_SECONDARY <
           	ui_type = "drag";
           	ui_min = 0.1; ui_max = 1.00;
			ui_label = "Fine AO Scale";
           	ui_tooltip = "Multiplier of Sample Radius for fine geometry. A setting of 0.5 scans the geometry at half the radius of the main AO.";
        > = 0.2;

        uniform float MXAO_AMOUNT_FINE <
             ui_type = "drag";
             ui_min = 0.00; ui_max = 1.00;
             ui_label = "Fine AO intensity multiplier";
             ui_tooltip = "Intensity of small scale AO / IL.";
        > = 1.0;

        uniform float MXAO_AMOUNT_COARSE <
             ui_type = "drag";
             ui_min = 0.00; ui_max = 1.00;
             ui_label = "Coarse AO intensity multiplier";
             ui_tooltip = "Intensity of large scale AO / IL.";
        > = 1.0;
#endif

uniform int MXAO_DEBUG_VIEW_ENABLE <
	ui_type = "combo";
	ui_label = "Enable Debug View";
	ui_items = "None\0AO/IL channel\0Culling Mask\0";
	ui_tooltip = "Different debug outputs";
> = 0;

uniform int MXAO_BLEND_TYPE <
	ui_type = "drag";
	ui_min = 0; ui_max = 2;
	ui_label = "Blending Mode";
	ui_tooltip = "Different blending modes for merging AO/IL with original color.\0Blending mode 0 matches formula of MXAO 2.0 and older.";
> = 0;

uniform float MXAO_FADE_DEPTH_START <
	ui_type = "drag";
	ui_label = "Fade Out Start";
	ui_min = 0.00; ui_max = 1.00;
	ui_tooltip = "Distance where MXAO starts to fade out. 0.0 = camera, 1.0 = sky. Must be less than Fade Out End.";
> = 0.0;

uniform float MXAO_FADE_DEPTH_END <
	ui_type = "drag";
	ui_label = "Fade Out End";
	ui_min = 0.00; ui_max = 1.00;
	ui_tooltip = "Distance where MXAO completely fades out. 0.0 = camera, 1.0 = sky. Must be greater than Fade Out Start.";
> = 0.4;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Textures, Samplers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

uniform float MXAO_FRAME_COUNT < source = "framecount"; >;
uniform float MXAO_FRAME_TIME  < source = "frametime"; >;

#if(MXAO_ENABLE_TSS != 0)
texture2D texSSAOTSS 	    { Width = BUFFER_WIDTH; Height = BUFFER_HEIGHT; Format = RGBA8; };
sampler2D SamplerSSAOTSS	{ Texture = texSSAOTSS;	        };
#endif

texture2D MXAO_ColorTex 	{ Width = BUFFER_WIDTH;   Height = BUFFER_HEIGHT;   Format = RGBA8; MipLevels = 3+MXAO_MIPLEVEL_IL;};
texture2D MXAO_DepthTex 	{ Width = BUFFER_WIDTH;   Height = BUFFER_HEIGHT;   Format = R16F;  MipLevels = 3+MXAO_MIPLEVEL_AO;};
texture2D MXAO_NormalTex	{ Width = BUFFER_WIDTH;   Height = BUFFER_HEIGHT;   Format = RGBA8; MipLevels = 3+MXAO_MIPLEVEL_IL;};
texture2D MXAO_CullingTex	{ Width = BUFFER_WIDTH/8; Height = BUFFER_HEIGHT/8; Format = R8; };

sampler2D sMXAO_ColorTex	{ Texture = MXAO_ColorTex;	};
sampler2D sMXAO_DepthTex	{ Texture = MXAO_DepthTex;	};
sampler2D sMXAO_NormalTex	{ Texture = MXAO_NormalTex;	};
sampler2D sMXAO_CullingTex	{ Texture = MXAO_CullingTex;	};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Weather and time functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

float ComputeFadeOutMXAO()
{
	float3 fadeoutTex = tex2D(sMXAO_ColorTex, float2(0.0f, 0.0f)).rgb;
	
	[flatten]
	if (fadeoutTex.b < 1.0f)
	{
		return 0.0f;
	}
	
	int R = fadeoutTex.r * 255;
	
	int w0 = (R & 0x000000F0) >> 4;
	int w1 = R & 0x0000000F;
	
	static const float weathers[16] = {
		EXTRASUNNY_FADEOUT,
		CLEAR_FADEOUT,
		CLOUDS_FADEOUT,
		SMOG_FADEOUT,
		FOGGY_FADEOUT,
		OVERCAST_FADEOUT,
		RAIN_FADEOUT,
		THUNDER_FADEOUT,
		CLEARING_FADEOUT,
		NEUTRAL_FADEOUT,
		SNOW_FADEOUT,
		BLIZZARD_FADEOUT,
		LIGHTSNOW_FADEOUT,
		XMAS_FADEOUT,
		HALLOWEEN_FADEOUT,
		NULL_FADEOUT
	};
	
	return lerp(weathers[w0], weathers[w1], fadeoutTex.g);
}

float ComputeStrenghtMXAO()
{
	static const float hours[25] = {
		MXAO_H0,
		MXAO_H1,
		MXAO_H2,
		MXAO_H3,
		MXAO_H4,
		MXAO_H5,
		MXAO_H6,
		MXAO_H7,
		MXAO_H8,
		MXAO_H9,
		MXAO_H10,
		MXAO_H11,
		MXAO_H12,
		MXAO_H13,
		MXAO_H14,
		MXAO_H15,
		MXAO_H16,
		MXAO_H17,
		MXAO_H18,
		MXAO_H19,
		MXAO_H20,
		MXAO_H21,
		MXAO_H22,
		MXAO_H23,
		MXAO_H0
	};
	
	float3 timeTex = tex2D(sMXAO_ColorTex, float2(1.0, 0.0)).rgb;
	
	float hour = timeTex.r*255.0 + timeTex.g*255.0/60.0 + timeTex.b*255.0/3600.0;
	
	return lerp(hours[int(hour)], hours[(int(hour)+1)], timeTex.g*255.0/60.0);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Vertex Shader
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct MXAO_VSOUT
{
	float4              position    : SV_Position;
	float2              texcoord    : TEXCOORD0;
	float2              scaledcoord : TEXCOORD1;
	float				samples     : TEXCOORD2;
	float               blursamples : TEXCOORD3;
	float3              uvtoviewADD : TEXCOORD4;
	float3              uvtoviewMUL : TEXCOORD5;
};

MXAO_VSOUT VS_MXAO(in uint id : SV_VertexID)
{
        MXAO_VSOUT MXAO;

        MXAO.texcoord.x = (id == 2) ? 2.0 : 0.0;
        MXAO.texcoord.y = (id == 1) ? 2.0 : 0.0;
        MXAO.scaledcoord.xy = MXAO.texcoord.xy / MXAO_GLOBAL_RENDER_SCALE;
        MXAO.position = float4(MXAO.texcoord.xy * float2(2.0, -2.0) + float2(-1.0, 1.0), 0.0, 1.0);

        MXAO.samples   = 8;
        MXAO.blursamples = 3;

        int blurqualityTweak   = (MXAO_GLOBAL_RENDER_SCALE < 0.707) + MXAO_TWO_LAYER - MXAO_ENABLE_TSS;
        int samplequalityTweak = -MXAO_GLOBAL_SAMPLE_QUALITY_PRESET * 2 * MXAO_ENABLE_TSS;

        if(     MXAO_GLOBAL_SAMPLE_QUALITY_PRESET == 0) { MXAO.samples = 4   + samplequalityTweak; MXAO.blursamples = 3 + blurqualityTweak;}
        else if(MXAO_GLOBAL_SAMPLE_QUALITY_PRESET == 1) { MXAO.samples = 8   + samplequalityTweak; MXAO.blursamples = 3 + blurqualityTweak;}
        else if(MXAO_GLOBAL_SAMPLE_QUALITY_PRESET == 2) { MXAO.samples = 16  + samplequalityTweak; MXAO.blursamples = 2 + blurqualityTweak;}
        else if(MXAO_GLOBAL_SAMPLE_QUALITY_PRESET == 3) { MXAO.samples = 24  + samplequalityTweak; MXAO.blursamples = 2 + blurqualityTweak;}
        else if(MXAO_GLOBAL_SAMPLE_QUALITY_PRESET == 4) { MXAO.samples = 32  + samplequalityTweak; MXAO.blursamples = 2 + blurqualityTweak;}
        else if(MXAO_GLOBAL_SAMPLE_QUALITY_PRESET == 5) { MXAO.samples = 64  + samplequalityTweak; MXAO.blursamples = 1 + blurqualityTweak;}
        else if(MXAO_GLOBAL_SAMPLE_QUALITY_PRESET == 6) { MXAO.samples = 255 + samplequalityTweak; MXAO.blursamples = 0 + blurqualityTweak;}

        MXAO.blursamples = max(MXAO.blursamples,0);

        MXAO.uvtoviewADD = float3(-1.0,-1.0,1.0);
        MXAO.uvtoviewMUL = float3(2.0,2.0,0.0);
/*
        static const float FOV = 70.0;

        MXAO.uvtoviewADD = float3(-tan(radians(FOV * 0.5)).xx,1.0);
        MXAO.uvtoviewADD.y *= BUFFER_WIDTH * BUFFER_RCP_HEIGHT;
        MXAO.uvtoviewMUL = float3(-2.0 * MXAO.uvtoviewADD.xy,0.0);
*/
        return MXAO;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

float3 GetPosition(in float2 coords, in MXAO_VSOUT MXAO)
{
	return (coords.xyx * MXAO.uvtoviewMUL + MXAO.uvtoviewADD) * GetLinearDepth(coords.xy) * RESHADE_DEPTH_LINEARIZATION_FAR_PLANE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

float3 GetPositionLOD(in float2 coords, in MXAO_VSOUT MXAO, in int mipLevel)
{
	return (coords.xyx * MXAO.uvtoviewMUL + MXAO.uvtoviewADD) * tex2Dlod(sMXAO_DepthTex, float4(coords.xy,0,mipLevel)).x;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void GetBlurWeight(in float4 tempKey, in float4 centerKey, in float surfacealignment, inout float weight)
{
	float depthdiff = abs(tempKey.w - centerKey.w);
	float normaldiff = saturate(1.0 - dot(tempKey.xyz,centerKey.xyz));

	weight = saturate(0.15 / surfacealignment - depthdiff) * saturate(0.65 - normaldiff); 
	weight = saturate(weight * 4.0) * 2.0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void GetBlurKeyAndSample(in float2 texcoord, in float inputscale, in sampler inputsampler, inout float4 tempsample, inout float4 key)
{
	float4 lodcoord = float4(texcoord.xy,0,0);
	tempsample = tex2Dlod(inputsampler,lodcoord * inputscale);
	key = float4(tex2Dlod(sMXAO_NormalTex,lodcoord).xyz*2-1, tex2Dlod(sMXAO_DepthTex,lodcoord).x);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

float4 BlurFilter(in MXAO_VSOUT MXAO, in sampler inputsampler, float inputscale, float radius, int blursteps)
{
	float4 tempsample;
	float4 centerkey, tempkey;
	float  centerweight = 1.0, tempweight;
	float4 blurcoord = 0.0;

	GetBlurKeyAndSample(MXAO.texcoord.xy,inputscale,inputsampler,tempsample,centerkey);
	float surfacealignment = saturate(-dot(centerkey.xyz,normalize(float3(MXAO.texcoord.xy*2.0-1.0,1.0)*centerkey.w)));

	#undef BLUR_COMP_SWIZZLE

	#if(MXAO_ENABLE_IL != 0)
		#define BLUR_COMP_SWIZZLE xyzw
	#else
		#define BLUR_COMP_SWIZZLE w
	#endif

	float4 blurSum = tempsample.BLUR_COMP_SWIZZLE;
	float2 blurOffsets[8] = {float2(1.5,0.5),float2(-1.5,-0.5),float2(-0.5,1.5),float2(0.5,-1.5),float2(1.5,2.5),float2(-1.5,-2.5),float2(-2.5,1.5),float2(2.5,-1.5)};

	[loop]
	for(int iStep = 0; iStep < blursteps; iStep++)
	{
			float2 sampleCoord = MXAO.texcoord.xy + blurOffsets[iStep] * PixelSize * radius / inputscale; 

			GetBlurKeyAndSample(sampleCoord, inputscale, inputsampler, tempsample, tempkey);
			GetBlurWeight(tempkey, centerkey, surfacealignment, tempweight);

			blurSum += tempsample.BLUR_COMP_SWIZZLE * tempweight;
			centerweight  += tempweight;
	}

	blurSum.BLUR_COMP_SWIZZLE /= centerweight;

	#if(MXAO_ENABLE_IL == 0)
			blurSum.xyz = centerkey.xyz*0.5+0.5;
	#endif

	return blurSum;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void SetupAOParameters(in MXAO_VSOUT MXAO, in float3 P, in float layerID, out float scaledRadius, out float falloffFactor)
{
	scaledRadius  = 0.25 * MXAO_SAMPLE_RADIUS / (MXAO.samples * (P.z + 2.0));
	falloffFactor = -1.0/(MXAO_SAMPLE_RADIUS * MXAO_SAMPLE_RADIUS);

	#if(MXAO_TWO_LAYER != 0)
		scaledRadius  *= lerp(1.0,MXAO_SAMPLE_RADIUS_SECONDARY,layerID);
		falloffFactor *= lerp(1.0,1.0/(MXAO_SAMPLE_RADIUS_SECONDARY*MXAO_SAMPLE_RADIUS_SECONDARY),layerID);
	#endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void TesselateNormals(inout float3 N, in float3 P, in MXAO_VSOUT MXAO)
{
	float2 searchRadiusScaled = 0.018 / P.z * float2(1.0,AspectRatio);
	float3 likelyFace[4] = {N,N,N,N};

	for(int iDirection=0; iDirection < 4; iDirection++)
	{
		float2 cdir;
		sincos(6.28318548 * 0.25 * iDirection,cdir.y,cdir.x);
		for(int i=1; i<=5; i++)
		{
			float cSearchRadius = exp2(i);
			float2 cOffset = MXAO.scaledcoord.xy + cdir * cSearchRadius * searchRadiusScaled;

			float3 cN = tex2Dlod(sMXAO_NormalTex,float4(cOffset,0,0)).xyz * 2.0 - 1.0;
			float3 cP = GetPositionLOD(cOffset.xy,MXAO,0);

			float3 cDelta = cP - P;
			float validWeightDistance = saturate(1.0 - dot(cDelta,cDelta) * 20.0 / cSearchRadius);
			float Angle = dot(N.xyz,cN.xyz);
			float validWeightAngle = smoothstep(0.3,0.98,Angle) * smoothstep(1.0,0.98,Angle); //only take normals into account that are NOT equal to the current normal.

			float validWeight = saturate(3.0 * validWeightDistance * validWeightAngle / cSearchRadius);

			likelyFace[iDirection] = lerp(likelyFace[iDirection],cN.xyz, validWeight);
		}
	}

	N = normalize(likelyFace[0] + likelyFace[1] + likelyFace[2] + likelyFace[3]);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

float GetCullingMask(in MXAO_VSOUT MXAO)
{
	float4 cOffsets = float4(PixelSize.xy,-PixelSize.xy) * 8;
	float cullingArea = tex2D(sMXAO_CullingTex, MXAO.scaledcoord.xy + cOffsets.xy).x;
	cullingArea      += tex2D(sMXAO_CullingTex, MXAO.scaledcoord.xy + cOffsets.zy).x;
	cullingArea      += tex2D(sMXAO_CullingTex, MXAO.scaledcoord.xy + cOffsets.xw).x;
	cullingArea      += tex2D(sMXAO_CullingTex, MXAO.scaledcoord.xy + cOffsets.zw).x;
	return cullingArea  > 0.000001;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Pixel Shaders
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PS_InputBufferSetup(in MXAO_VSOUT MXAO, out float4 color : SV_Target0, out float4 depth : SV_Target1, out float4 normal : SV_Target2)
{
	float3 offs = float3(PixelSize.xy,0);

	float3 f 	 	=       GetPosition(MXAO.texcoord.xy, MXAO);
	float3 gradx1	= - f + GetPosition(MXAO.texcoord.xy + offs.xz, MXAO);
	float3 gradx2	=   f - GetPosition(MXAO.texcoord.xy - offs.xz, MXAO);
	float3 grady1	= - f + GetPosition(MXAO.texcoord.xy + offs.zy, MXAO);
	float3 grady2	=   f - GetPosition(MXAO.texcoord.xy - offs.zy, MXAO);

	gradx1 = lerp(gradx1, gradx2, abs(gradx1.z) > abs(gradx2.z));
	grady1 = lerp(grady1, grady2, abs(grady1.z) > abs(grady2.z));

	normal          = float4(normalize(cross(grady1,gradx1)) * 0.5 + 0.5,0.0);
    color 			= tex2D(SamplerColor, MXAO.texcoord.xy);
	depth 			= GetLinearDepth(MXAO.texcoord.xy)*RESHADE_DEPTH_LINEARIZATION_FAR_PLANE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PS_Culling(in MXAO_VSOUT MXAO, out float4 color : SV_Target0)
{
	color = 0.0;
	MXAO.scaledcoord.xy	= MXAO.texcoord.xy;
	MXAO.samples = min(MXAO.samples, 32);

	float3 P            = GetPositionLOD(MXAO.scaledcoord.xy, MXAO, 0);
	float3 N            = tex2D(sMXAO_NormalTex, MXAO.scaledcoord.xy).xyz * 2.0 - 1.0;

	P += N * P.z / RESHADE_DEPTH_LINEARIZATION_FAR_PLANE;

	float scaledRadius;
	float falloffFactor;
	SetupAOParameters(MXAO, P, 0, scaledRadius, falloffFactor);

	float randStep = dot(floor(MXAO.position.xy % 4 + 0.1),int2(1,4)) + 1;
	randStep *= 0.0625;

	float2 sampleUV, Dir;
	sincos(38.39941 * randStep, Dir.x, Dir.y); 

	Dir *= scaledRadius;       

	[loop]
	for(int iSample=0; iSample < MXAO.samples; iSample++)
	{                
		sampleUV = MXAO.scaledcoord.xy + Dir.xy * float2(1.0, AspectRatio) * (iSample + randStep);   
		Dir.xy = mul(Dir.xy, float2x2(0.76465,-0.64444,0.64444,0.76465));             

		float sampleMIP = saturate(scaledRadius * iSample * 20.0) * 3.0;

		float3 V 		= -P + GetPositionLOD(sampleUV, MXAO, sampleMIP);
		float  VdotV    = dot(V, V);
		float  VdotN    = dot(V, N) * rsqrt(VdotV);

		float fAO = saturate(1.0 + falloffFactor * VdotV) * saturate(VdotN - MXAO_SAMPLE_NORMAL_BIAS);
		color.w += fAO;
	}

	color = color.w;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PS_StencilSetup(in MXAO_VSOUT MXAO, out float4 color : SV_Target0)
{
	if(    GetLinearDepth(MXAO.scaledcoord.xy) >= MXAO_FADE_DEPTH_END * ComputeFadeOutMXAO()
		|| 0.25 * 0.5 * MXAO_SAMPLE_RADIUS / (tex2D(sMXAO_DepthTex,MXAO.scaledcoord.xy).x + 2.0) * BUFFER_HEIGHT < 1.0
		|| MXAO.scaledcoord.x > 1.0
		|| MXAO.scaledcoord.y > 1.0
		|| !GetCullingMask(MXAO)        
		) discard;

    color = 1.0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PS_AmbientObscurance(in MXAO_VSOUT MXAO, out float4 color : SV_Target0)
{
    color = 0.0;

	float3 P             = GetPositionLOD(MXAO.scaledcoord.xy, MXAO, 0);
    float3 N             = tex2D(sMXAO_NormalTex, MXAO.scaledcoord.xy).xyz * 2.0 - 1.0;
    float  layerID       = (MXAO.position.x + MXAO.position.y) % 2.0;

	#if(MXAO_SMOOTHNORMALS != 0)
		TesselateNormals(N, P, MXAO);
	#endif

	P += N * P.z / RESHADE_DEPTH_LINEARIZATION_FAR_PLANE;

	float scaledRadius;
	float falloffFactor;
	SetupAOParameters(MXAO, P, layerID, scaledRadius, falloffFactor);

	float randStep = dot(floor(MXAO.position.xy % 4 + 0.1),int2(1,4)) + 1;
	randStep *= 0.0625;

	float2 sampleUV, Dir;
	sincos(38.39941 * randStep, Dir.x, Dir.y); 

	Dir *= scaledRadius;       

	[loop]
	for(int iSample=0; iSample < MXAO.samples; iSample++)
	{                
		sampleUV = MXAO.scaledcoord.xy + Dir.xy * float2(1.0, AspectRatio) * (iSample + randStep);   
		Dir.xy = mul(Dir.xy, float2x2(0.76465,-0.64444,0.64444,0.76465));             

		float sampleMIP = saturate(scaledRadius * iSample * 20.0) * 3.0;

		float3 V 		= -P + GetPositionLOD(sampleUV, MXAO, sampleMIP);
		float  VdotV            = dot(V, V);
		float  VdotN            = dot(V, N) * rsqrt(VdotV);

		float fAO = saturate(1.0 + falloffFactor * VdotV) * saturate(VdotN - MXAO_SAMPLE_NORMAL_BIAS);

#if(MXAO_ENABLE_IL != 0)
		if(fAO > 0.1)
		{
			float3 fIL = tex2Dlod(sMXAO_ColorTex, float4(sampleUV,0,sampleMIP + MXAO_MIPLEVEL_IL)).xyz;
			float3 tN = tex2Dlod(sMXAO_NormalTex, float4(sampleUV,0,sampleMIP + MXAO_MIPLEVEL_IL)).xyz * 2.0 - 1.0;
			fIL = fIL - fIL*saturate(dot(V,tN)*rsqrt(VdotV)*2.0);
			color += float4(fIL*fAO,fAO - fAO * dot(fIL,0.333));
		}
#else
		color.w += fAO;
#endif
	}

	color = saturate(color/((1.0-MXAO_SAMPLE_NORMAL_BIAS)*MXAO.samples));
	color = sqrt(color); //AO denoise

#if(MXAO_TWO_LAYER != 0)
	color = pow(color,1.0 / lerp(MXAO_AMOUNT_COARSE, MXAO_AMOUNT_FINE, layerID));
#endif

#if(MXAO_ENABLE_TSS != 0)
	color = lerp(tex2D(SamplerSSAOTSS,MXAO.scaledcoord.xy),color,0.2);
#endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PS_BlurX(in MXAO_VSOUT MXAO, out float4 color : SV_Target0)
{
    color = BlurFilter(MXAO, SamplerColor, MXAO_GLOBAL_RENDER_SCALE, 1.0, 8);
}

void PS_BlurYandCombine(in MXAO_VSOUT MXAO, out float4 color : SV_Target0)
{
	if (!(MXAO.texcoord.x < PixelSize.x 		&& MXAO.texcoord.y < PixelSize.y) 		&&
		!(MXAO.texcoord.x > 1.0-PixelSize.x 	&& MXAO.texcoord.y < PixelSize.y) 		&&
		!(MXAO.texcoord.x < PixelSize.x 		&& MXAO.texcoord.y > 1.0-PixelSize.y) 	&&
		!(MXAO.texcoord.x > 1.0f-PixelSize.x 	&& MXAO.texcoord.y > 1.0f-PixelSize.y))
	{
#if(MXAO_ENABLE_TSS != 0)
        float4 aoil = BlurFilter(MXAO, SamplerSSAOTSS, 1.0, 0.75/MXAO_GLOBAL_RENDER_SCALE, 4);
#else
        float4 aoil = BlurFilter(MXAO, SamplerColor, 1.0, 0.75/MXAO_GLOBAL_RENDER_SCALE, 4);
#endif

        aoil *= aoil; //AO denoise
		color                   = tex2D(sMXAO_ColorTex, MXAO.texcoord.xy);

        float scenedepth        = GetLinearDepth(MXAO.texcoord.xy);
        float3 lumcoeff         = float3(0.2126, 0.7152, 0.0722);
        float colorgray         = dot(color.rgb,lumcoeff);
        float blendfact         = 1.0 - colorgray;
		
		float mxao_amount		= ComputeStrenghtMXAO();

#if(MXAO_ENABLE_IL != 0)
	    aoil.xyz  = lerp(dot(aoil.xyz,lumcoeff),aoil.xyz, MXAO_SSIL_SATURATION) * MXAO_SSIL_AMOUNT * 4.0 * mxao_amount;
#else
        aoil.xyz = 0.0;
#endif

		aoil.w  = 1.0-pow(abs(1.0-aoil.w), MXAO_SSAO_AMOUNT*4.0*mxao_amount);
        aoil    = lerp(aoil,0.0,smoothstep(MXAO_FADE_DEPTH_START, MXAO_FADE_DEPTH_END * ComputeFadeOutMXAO(), scenedepth * float4(2.0,2.0,2.0,1.0)));

        if(MXAO_BLEND_TYPE == 0)
        {
            color.rgb -= (aoil.www - aoil.xyz) * blendfact * color.rgb;
        }
        else if(MXAO_BLEND_TYPE == 1)
        {
            color.rgb = color.rgb * saturate(1.0 - aoil.www * blendfact * 1.2) + aoil.xyz * blendfact * colorgray * 2.0;
        }
        else if(MXAO_BLEND_TYPE == 2)
        {
            float colordiff = saturate(2.0 * distance(normalize(color.rgb + 1e-6),normalize(aoil.rgb + 1e-6)));
            color.rgb = color.rgb + aoil.rgb * lerp(color.rgb, dot(color.rgb, 0.3333), colordiff) * blendfact * blendfact * 4.0;
            color.rgb = color.rgb * (1.0 - aoil.www * (1.0 - dot(color.rgb, lumcoeff)));
        }
		else if(MXAO_BLEND_TYPE == 3)
        {
            color.rgb = pow(color.rgb,2.2);
			color.rgb -= (aoil.www - aoil.xyz) * color.rgb;
			color.rgb = pow(color.rgb,1.0/2.2);
        }

        color.rgb = saturate(color.rgb);

		if(MXAO_DEBUG_VIEW_ENABLE == 1) //can't move this into ternary as one is preprocessor def and the other is a uniform
		{
			color.rgb = max(0.0,1.0 - aoil.www + aoil.xyz);
			color.rgb *= (MXAO_ENABLE_IL != 0) ? 0.5 : 1.0;
		}
        else if(MXAO_DEBUG_VIEW_ENABLE == 2)
        {
            color.rgb = GetCullingMask(MXAO);
        }
	}
	else
	{
		color.rgb = tex2D(sMXAO_ColorTex, MXAO.texcoord.xy).rgb;
	}
	color.a = 1.0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Technique
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

technique MXAO
{
	pass
	{
		VertexShader = VS_MXAO;
		PixelShader  = PS_InputBufferSetup;
		RenderTarget0 = MXAO_ColorTex;
		RenderTarget1 = MXAO_DepthTex;
		RenderTarget2 = MXAO_NormalTex;
	}
    pass
	{
		VertexShader = VS_MXAO;
		PixelShader  = PS_Culling;
		RenderTarget = MXAO_CullingTex;
	}
    pass
    {
        VertexShader = VS_MXAO;
		PixelShader  = PS_StencilSetup;
        /*Render Target is Backbuffer*/
        ClearRenderTargets = true;
		StencilEnable = true;
		StencilPass = REPLACE;
        StencilRef = 1;
    }
    pass
    {
		VertexShader = VS_MXAO;
		PixelShader  = PS_AmbientObscurance;
		/*Render Target is Backbuffer*/
		ClearRenderTargets = true;
		StencilEnable = true;
		StencilPass = KEEP;
		StencilFunc = EQUAL;
		StencilRef = 1;
    }
    pass
	{
		VertexShader = VS_MXAO;
		PixelShader  = PS_BlurX;
#if(MXAO_ENABLE_TSS != 0)
        RenderTarget = texSSAOTSS;
#endif
	}
	pass
	{
		VertexShader = VS_MXAO;
		PixelShader  = PS_BlurYandCombine;
        /*Render Target is Backbuffer*/
	}
}
