///////////////////
//	Assimp�̃��f����p�̃V�F�[�_
///////////////////

#include"../commonpsvs.fx"


VS_OUTPUT main(
	float4 Pos		:	POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD,
	int4   BoneIndex : BONEINDEX,
	float4 BoneWeight : BONEWEIGHT
)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);
	output.WPos = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = Tex;

	float4 N = Normal;
	N.w = 0.0f;					// �@���̓x�N�g���Ȃ̂łv�̒l���O�ɂ���B

	N = mul(N, World);			// �@���x�N�g���Ƀ��[���h�ϊ��s����|���ĕ�����ς���
	N = normalize(N);			// ���K��

	output.Normal = N;          //�s�N�Z���V�F�[�_�[�ł������߂Ɋi�[

	output.Color = AssimpModelColor;

	return output;
}