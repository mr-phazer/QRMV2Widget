

static const int t_Diffuse = 0;
static const int t_Normal = 1;
static const int t_Mask = 3;
static const int t_AmbientOcclusion = 5;
static const int t_TilingDirtUV2 = 7;
static const int t_DirtAlphaMask = 8;  
static const int t_SkinMask = 10;
static const int t_Specular = 11;
static const int t_GlossMap = 12;
static const int t_DecalDirtmap = 13;
static const int t_DecalDirtmask = 14;
static const int t_DecalMask = 15;
static const int t_DiffuseDamage = 17;

sampler SamplerLinear : register(s0); 
sampler SamplerDefault : register(s1);
sampler spBRDF_Sampler : register(s2);
sampler s_normal : register(s3);
sampler s_mask : register(s4);
sampler s_point : register(s5);

TextureCube tex_cube_diffuse : register(t0);
TextureCube tex_cube_specular : register(t1);
Texture2D tex_brdf_lut : register(t2);
Texture2D shaderTextures[30] : register(t3);