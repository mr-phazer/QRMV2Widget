#include "layout.hlsli"
#include "textures.hlsli"
#include "PS_ConstBuffers.hlsli"
#include "tone_mapping.hlsli"
#include "pbr_lib.hlsli"


float3 getBlueNormal(PixelInputType input)
{
    //float3x3 basis = float3x3(normalize(input.tangent), normalize(input.normal), normalize(input.binormal));
    float3x3 basis = float3x3(normalize(input.tangent), normalize(input.binormal), normalize(input.normal));
    float4 NormalTex = shaderTextures[t_Normal].Sample(SamplerDefault, input.tex);
    
    //float3x3 basis = float3x3
    //(normalize(input.tangent), normalize(input.normal), normalize(-input.binormal));

    
    
    
    float3 Np = 0;
    Np.x = NormalTex.r * NormalTex.a;
    Np.y = NormalTex.g;
    Np = (Np * 2.0f) - 1.0f;
      
    Np.z = sqrt(1 - Np.x * Np.x - Np.y * Np.y);
	
    
    return normalize(mul(normalize(Np.xyz), basis));
    
}


float get_env_map_lod(in float roughness_in)
{
    float smoothness = pow(1.0 - roughness_in, 4.0);

    float roughness = 1.0 - smoothness;

	//	This must be the number of mip-maps in the environment map!
    float texture_num_lods = 1.0f;
	
    float env_map_lod = roughness * (texture_num_lods - 1.0);
	
    return env_map_lod;
}

float get_cube_env_scale_factor()
{
    return 1.0f;
}

float3 get_environment_colour(in float3 direction, in float lod)
{
    return (tex_cube_specular.SampleLevel(SamplerLinear, direction /*texcoordEnvSwizzle(direction)*/, lod).rgb);
}



float3 sample_environment_specular(in float roughness_in, in float3 reflected_view_vec)
{
#if 1
    const float env_lod_pow = 1.8f;
    const float env_map_lod_smoothness = /*adjust_linear_smoothness*/(1 - roughness_in);
    const float roughness = 1.0f - pow(env_map_lod_smoothness, env_lod_pow);

    float texture_num_lods = 7.0f;
    float env_map_lod = roughness * (texture_num_lods - 1);
    float3 environment_colour = get_environment_colour(reflected_view_vec, env_map_lod);
#else
	const float roughness = roughness_in;
	const float offset = 3;
	float texture_num_lods = 9.0f; // - offset;
	float env_map_lod = roughness * (texture_num_lods - 1);
	env_map_lod += offset;
	float3 environment_colour = get_environment_colour(reflected_view_vec, env_map_lod);
#endif

    float3 result = environment_colour * get_cube_env_scale_factor();

    return result;
}



float3 sample_environment_specular_old(in float roughness_in, in float3 reflected_view_vec)
{
    float env_map_lod = get_env_map_lod(roughness_in);
	
    return get_environment_colour(reflected_view_vec, env_map_lod);
}





static const float texture_alpha_ref = 0.7f;

void alpha_test(in const float pixel_alpha)
{
    //clip(-1);

    {
        
        clip(pixel_alpha - 0.7);
    }
}

float substance_smoothness_get_our_smoothness(in float substance_smoothness)
{
	//	This value is correct for roughnesses from second_join_pos to 1.0.  This is valid for
	//	the end of the roughness curve...
    float original_roughness = 1.0f - substance_smoothness;
	
    float original_roughness_x2 = original_roughness * original_roughness;
    float original_roughness_x3 = original_roughness_x2 * original_roughness;
    float original_roughness_x4 = original_roughness_x3 * original_roughness;
    float original_roughness_x5 = original_roughness_x4 * original_roughness;
		
    return 1.0f - saturate((28.09f * original_roughness_x5) - (64.578f * original_roughness_x4) + (48.629f * original_roughness_x3) - (12.659f * original_roughness_x2) + (1.5459f * original_roughness));
}

//	As a material becomes more rough, it also becomes more like a diffuse material.
float determine_facet_visibility(in float roughness, in float3 normal_vec, in float3 light_vec)
{
    const float n_dot_l = saturate(dot(normal_vec, light_vec));
    const float towards_diffuse_surface = sin(roughness * PI * 0.5f); //	( 0 - 1 ) output...
    const float facet_visibility = lerp(1.0f, n_dot_l, towards_diffuse_surface);

    return facet_visibility;
}


float3 get_reflectivity_env_light_material(in float3 light_vec, in float3 normal_vec, in float3 view_vec, in float roughness, in float3 specular_colour)
{
    float facet_visibility = determine_facet_visibility(roughness, normal_vec, view_vec);

	// saturate is added here to work around an nvidia bug: v_dot_n seemed to be
	// coming out as < 0, causing the val to be NaN.
    float v_dot_n = saturate(abs(dot(normal_vec, view_vec)));

    float fresnel_curve = 10;
    float val1 = 1 - v_dot_n;
    float val2 = pow(val1, fresnel_curve);

	// Masking based on smoothness, derived from Smith-GGX
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float masking = (2 * v_dot_n) / (v_dot_n + sqrt(alpha2 + (1 - alpha2) * v_dot_n * v_dot_n));

    masking = masking * masking;

    return lerp(specular_colour, saturate(60.0f * specular_colour), val2) * masking;
}



float4 main(in PixelInputType input) : SV_TARGET
{      
    
    // sample textures
    float4 SpecTex = shaderTextures[t_Specular].Sample(SamplerDefault, input.tex);
    float4 DiffuseTex = shaderTextures[t_Diffuse].Sample(SamplerDefault, input.tex);
    float4 GlossTex = shaderTextures[t_GlossMap].Sample(SamplerDefault, input.tex);
    float4 MaskTex = shaderTextures[t_Mask].Sample(SamplerDefault, input.tex);
    
    //float3 RGB = MaskTex.r;
    //return float4(RGB, MaskTex.r);    
    //return   float4(1, 0, 0, 1);
    
    if (PisTextureSRGB[t_Diffuse] != 0)
    {       
        //return float4(1, 0, 0, 1);
        DiffuseTex.rgb = pow(DiffuseTex.rgb, 2.2f);
        
        //DiffuseTex.rgb = lerp(DiffuseTex.rgb, float3(1, 0, 0), 0.5);

    }
    
    if (PisTextureSRGB[t_Specular] != 0)
    {
        //return float4(0, 0, 1, 1);
        SpecTex.rgb = pow(SpecTex.rgb, 2.2f);
        //SpecTex.rgb = lerp(SpecTex.rgb, float3(1, 0, 0), 0.5);
        
    }
        
    
    
    
    
        apply_faction_colours(DiffuseTex.rgb, shaderTextures[t_Mask], SamplerDefault, input.tex,
        color1.rgb,
        color2.rgb,
        color3.rgb
    );
   
    
    //apply_faction_colours(DiffuseTex.rgb, shaderTextures[t_Mask], SamplerDefault, input.tex,
    //    float3(62.0 / 255.0, 62.0 / 255.0, 62.0 / 255.0),
    //    float3(62.0 / 255.0, 62.0 / 255.0, 62.0 / 255.0),
    

    //float3(0, 0, 0)
        
    //);
    
    //DiffuseTex = pow(DiffuseTex, 2.2);
    //SpecTex = pow(DiffuseTex, 2.2);
    
    //float4 DiffuseTex = float4(0.5, 0.5, 0.5, 1);
    //float4 GlossTex = float4(0.5, 0, 0, 1);

    //float smoothness = _linear(GlossTex.r);
    //smoothness = adjust_linear_smoothness(smoothness);

    //float smoothness = substance_smoothness_get_our_smoothness(GlossTex.r);
 
    //return float4(GlossTex.rg, 0, 1);
    float smoothness = GlossTex.r;
    float roughness = 1.0 - smoothness;
    float alpha = DiffuseTex.a;

    
    float3 N = getBlueNormal(input);
    float3 N2 = getBlueNormal(input);
    float3 N_rotated = N;
    //mul(N, (float3x3) mRotEnv); / TODO: change back
    
   // TODO: when rotating environment, only rotate the normal used for texture lookup, not the "basic" one
   // TODO: so other rendering math is NOT affecteted by the rotation of the environment
    N_rotated = normalize(N_rotated);
    N_rotated = N;
        
//    float3 N2 = mul(N, (float3x3) mRotEnv);
    N2 = normalize(N2);
    
       
    float3 Lo = normalize(input.viewDirection);
       
	// Angle between surface normal and outgoing light direction.
    float cosLo = max(0.0, dot(N, Lo));
    
	// Specular reflection vector.
   // float3 Lr = 2.0 * cosLo * N- Lo;  // written out reflect formula
    float3 Lr = reflect(N, Lo); // HLSL intrisic reflection function  
    
    // rotate refletion map by rotating the reflect vector
    //Lr = mul(Lr, (float3x3) mRotEnv); // TODO:    change back                   
    // specular    
    float3 F0 = SpecTex.rgb;
    
	// Direct light, non-ambient types
    float3 directLighting = 0.0;
    
    
    
    [unroll]       
    for (uint i = 0; i < 1; ++i)
    {
        float3 Li = normalize(LightData[i].lightDirection);
        float3 Lradiance = LightData[i].radiannce;
        
        //if (light[i].oPointLight.is_active)
        //{
        //    Li = -normalize((light[i].oPointLight.position - input.worldPosition));
        //    Lradiance = getPointLightIntensity(light[i].oPointLight, input.worldPosition);

        //}
        //else
        //{
        //    Li = normalize(-light[i].lightDirection);
        //    Lradiance = light[i].radiannce;

        //}

		// Half-vector between Li and Lo.
        float3 Lh = normalize(Li + Lo);

		// Calculate angles between surface normal and various light vectors.
        float cosLi = max(0.0, dot(N, Li));
        float cosLh = max(0.0, dot(N, Lh));

		// Calculate Fresnel term for direct lighting. 
        float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
        
        //float new_roughness = get_direct_roughness(roughness);
        float new_roughness = (roughness);
        
        //float3 F = fresnelSchlickRoughness(max(dot(N, Lo), 0.0), F0, new_roughness);
		
        // Calculate normal distribution for specular BRDF.
        float D = ndfGGX(cosLh, new_roughness);
		
        // Calculate geometric attenuation for specular BRDF.
        float G = gaSchlickGGX(cosLi, cosLo, new_roughness);

		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
        float3 kd = float3(1, 1, 1) - F;

        
		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
        
        float3 diffuseBRDF = 0;
        
        //return debug_flags.scale_by_one_over_pi;
        
        //if (debug_flags.scale_by_one_over_pi)
        //    diffuseBRDF = kd * DiffuseTex.rgb * (1 / PI);
        //else
        diffuseBRDF = kd * DiffuseTex.rgb;
        
		// Cook-Torrance specular microfacet BRDF.
        float3 specularBRDF = ((F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo));
        //specularBRDF *= spec_intensity; // renderparam "spec_intensity": controls how shiny direct lighting will be
        
		// Total contribution for this light.
        // render_param "light[0].radiannce": control direct lighting
        directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi; // * light[0].radiannce * direct_light_factor;
        
    }

    
    
    float3 ambientLighting = 0;
    float3 irradiance = 0;
    float3 specularIrradiance = 0;
    float3 specularIBL = 0;
    float3 diffuseIBL = 0;
    float4 color = 0;
	{
        
      
		// Sample diffuse irradiance at normal direction.        
        
        // TODO: when rotating environment, make a rotate version of "Lr" here, so that cosLo and other things are not
        // TODO: affecteted by the rotation of the env
        float3 rot_N = mul(N, (float3x3) mEnv);
        irradiance = tex_cube_diffuse.Sample(SamplerDefault, rot_N).rgb;
                
        //return float4(irradiance, 1);                                     //           DEBUGGIN code
     
		// Calculate Fresnel term for ambient lighting.
		// Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
		// use cosLo instead of angle with light's half-vector (cosLh above).
		// See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
        
        float3 F = fresnelSchlickRoughness(cosLo, F0, roughness); // alternative code
//        float3 F = fresnelSchlick(F0, cosLo); // alternative code
                                                                         
        float3 kS = F;
        float3 kD = 1.0 - kS;
        
        //kD *= 1.0 - length(F0);
        
        //return float4(kD, 1);
		// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.

        // TODO: re-enable?
        //if (debug_flags.scale_by_one_over_pi)
        //    diffuseIBL = kD * DiffuseTex.rgb * irradiance;
        //else
        //    diffuseIBL = kD * DiffuseTex.rgb * irradiance * (1 / PI);

        diffuseIBL = kD * DiffuseTex.rgb * irradiance;
                                                                                                   
        
		// Sample pre-filtered specular reflection environment at correct mipmap level.     
        
        // TODO: when rotating environment, make a rotate version of "Lr" here, so that cosLo and other things are not
        // TODO: affecteted by the rotation of the env
        //specularIrradiance = sample_environment_specular_old(saturate(roughness /*+ lod_bias*/), normalize(Lr));
        float3 rot_lr = mul(Lr, (float3x3) mEnv);
        specularIrradiance = sample_environment_specular(saturate(roughness /*+ lod_bias*/), normalize(rot_lr));
        float3 specularIrradiance2 = sample_environment_specular(saturate(pow(roughness, 4) /*+ lod_bias*/), normalize(rot_lr));

        
        // make the specular gradually become diffuse-like = softer images
        //specularIrradiance = lerp(specularIrradiance, irradiance, roughness * roughness); // -- alternative
               
        F = fresnelSchlickRoughness(cosLo, F0, roughness);
        float2 brdf = tex_brdf_lut.Sample(spBRDF_Sampler, float2(cosLo, (1.0 - roughness)) /*float2(roughness, 1.0f - cosLo)*/).
        xy;

                
        //return float4(brdf, 0, 1); // DBUGGIN CODE: show BRDF look up table
        
        
        // ------ Edited to look more WH2 like / hack because TBN matrix was wrong ---------
        //specularIBL = F0 * (brdf.x + brdf.y) * specularIrradiance; 
        
        // ----------  The "full pbr" (yes is it "F", not F0 :) ) -------------
        
        //return float4(EnvBRDFApprox_DEBUG(F0, roughness, cosLo), 1); // DEBUGGING code
        
        
        
        //specularIBL = F0 * (brdf.x + brdf.y) * specularIrradiance;
        //specularIBL = EnvBRDFApprox(F0, roughness, cosLo) * specularIrradiance;
        
        // test versions
        //specularIBL = (F * brdf.x) * specularIrradiance + brdf.y* sample_environment_specular_new(roughness/3, normalize(Lr));
        //specularIBL = (F * brdf.x) * specularIrradiance + brdf.y* sample_environment_specular_new(roughness/3, normalize(Lr));
        //specularIBL = F * (brdf.x + brdf.y) * specularIrradiance;
        
        /////////////////////////////////////////////////////////////////
        // proper PBR-sum        
        /////////////////////////////////////////////////////////////////
        //specularIBL = (F * brdf.x + brdf.y) * specularIrradiance;
        
        ////////////////////////////////////////////////////////////////
        // "reduced realism PBR"
        ////////////////////////////////////////////////////////////////
        
        #define FRESNEL_TYPE 1
        
        #if FRESNEL_TYPE == 0
        
            if (SpecTex.r == SpecTex.g && SpecTex.g == SpecTex.b)
                specularIBL =  (F* brdf.x + brdf.y) * specularIrradiance;
            else
                specularIBL = (F * brdf.x + brdf.y) * lerp(specularIrradiance, specularIrradiance2, pow(smoothness, (1 / 4)));
        
        #elif FRESNEL_TYPE == 1
        
            specularIBL =  (F* brdf.x + brdf.y) * specularIrradiance;
        
        #elif FRESNEL_TYPE == 2        

        specularIBL = get_reflectivity_env_light_material(
        normalize(LightData[0].lightDirection), N, Lo, roughness, F0
                          
        ) * specularIrradiance;
        
        #elif FRESNEL_TYPE == 3
        
        specularIBL =  F* (brdf.x + brdf.y) * specularIrradiance;
        
        #elif FRESNEL_TYPE == 4
        
        specularIBL =  F0* (brdf.x + brdf.y) * specularIrradiance;
        
        #endif
        
                
        
        // TODO: re-enable
           // TODO: re-enable
        //if (show_reflections)
        //{
        //    //ambientLighting = specularIBL;
        //    specularIBL = F0 * (brdf.x + brdf.y) * specu-larIrradiance;
        //}
        
        
        
        ambientLighting = (specularIBL + diffuseIBL); // * light[0].ambientFactor; // * ambient_light_factor;
        
       
        
        
    }
    
    //float d_light_factor = (show_reflections | debug_flags.irrandiace_only) ? 0.0f : 1.0f;
    float d_light_factor = 1.0;
    //if (debug_flags.irrandiace_only) // only render diffuse env light
    //{
    //    color = float4(irradiance, 1.0);
    //}
    //else if (show_reflections)
    //{
    //    color = float4(ambientLighting, 1.0);
    //}
    //else
      
    color = float4(directLighting + ambientLighting, 1.0) * env_color;
    
    // Rome2 / Attila / Thrones extra gloss map param
    //color *= (1.0 - GlossTex.r);
    
    //if (alpha_used == 1)
    //{
    //    alpha_test(alpha);
    //}
    
    color.a = alpha;
    
    const float gamma_value = 2.2;
    
    float3 hdrColor = color.rgb /* * exposure*/ * 0.8;
    float3 mapped = Uncharted2ToneMapping(hdrColor);
                                
   
    return float4(mapped, 1);  
   
}
