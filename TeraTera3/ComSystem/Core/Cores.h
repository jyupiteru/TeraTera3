/**
 * @file Cores.h
 * @author jupiter ()
 * @brief core関連のヘッダのまとめ
 * @n ここをインクルードせず個別にするとエラー発生します
 */
#pragma once

#include "GameObject.h"

#include "ComponentBase.h"

//違うけどないと(前方宣言を使用しているのに読み込んでなくて)エラーが多発するので…
#include "../Components/Behavior/ComTransform/ComTransform.h"
