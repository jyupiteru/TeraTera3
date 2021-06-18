
////
//	���f���p�̃t�H���O�̋��ʔ��˂̃s�N�Z���V�F�[�_�[
//	�@���K�{
////


#include	"../commonpsvs.fx"


float4 main(VS_OUTPUT input) : SV_Target
{
	float4 N = input.Normal;    //�@�����o��
	float4 L = LightDirection;	//���̕������o��
	N.w = 0.0f;					// �@���̓x�N�g���Ȃ̂łv�̒l���O�ɂ���B
	L.w = 0.0f;					// ���̕����̓x�N�g���Ȃ̂łv�̒l���O�ɂ���B

	//���K��������
	N = normalize(N);
	L = normalize(L);

	//�@���ƌ��̕�������ς���0�Ɣ�r �傫���ق����i�[
	float d = dot(N, L);

	//���ς��ă}�C�i�X�ɂȂ����̂����̕����������Ă���̂�-1��������0�ȉ���0�ɂ���
	d *= -1;
	d = max(0.0,d);

	//�������ǂꂭ�炢�e�������邩�v�Z
	float4 diffuseLig = d * Ambient;

	float4 col = input.Color;
	col.x /= 256.0f;
	col.y /= 256.0f;
	col.z /= 256.0f;

	//�}�e���A���F?�̎擾
	float4 outcol = g_Tex.Sample(g_SamplerLinear, input.Tex);

	//�I�u�W�F�N�g�F��������
	outcol *= col;

	//������������
	outcol *= diffuseLig;

	//�F��S�����f ����������Γ��߂ł������₯�ǖ�����
	outcol.a = 1.0f;

	return outcol;
}