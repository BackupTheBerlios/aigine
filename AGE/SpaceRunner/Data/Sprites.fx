TEXTURE Texture;

TECHNIQUE T1
{
	PASS P1
	{
		Texture[0]		= <Texture>;
		ZEnable			= True;
		ZWriteEnable		= False;

		ColorOp[0]		= Modulate;
		ColorArg1[0]		= Texture;
		ColorArg2[0]		= Current;
		AlphaOp[0]		= Modulate;
		AlphaArg1[0]		= Texture;
		AlphaArg2[0]		= Current;
		ColorOp[1]		= Disable;

		AlphaBlendEnable	= True;
		SrcBlend		= SrcAlpha;
		DestBlend		= One;
		AlphaTestEnable		= True;
		AlphaFunc		= Greater;
		AlphaRef		= 7;

		Lighting		= False;
		ColorVertex		= True;
		CullMode		= None;
	}
}