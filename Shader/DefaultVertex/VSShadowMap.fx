////
//�V���h�E�}�b�v�𐧍삷��̂Ɏg�p���钸�_�V�F�[�_
////

#include "../Utils/CommonPSVS.fx"


VS_OUTPUT main(float4 Pos	:	POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);
	output.Pos = mul(output.Pos, viewFromLight);
	output.Pos = mul(output.Pos, projectionFromLight);

	output.Depth = output.Pos;

	return output;
}
