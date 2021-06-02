/**
 * @file CollisionConcept.h
 * @author jupiter ()
 * @brief 当たり判定系の関数があるか判定するメタクラスorコンセプトを集めたヘッダ
 */

#pragma once

#include <concepts>

class ComponentBase;
class CCollisionBase;

/**
 * @brief コンセプトを使用した指定するやつ
 * @tparam componenttype 判定したいコンポーネント名
 */
template <class componenttype>
concept Has_OnTriggerEnter2D = requires(componenttype *component)
{
	std::derived_from<componenttype, ComponentBase>;
	//OnTriggerEnter2Dを持っている
	component->OnTriggerEnter2D;
};

/**
 * @brief コンセプトを使用した指定するやつ
 * @tparam componenttype 判定したいコンポーネント名
 */
template <class componenttype>
concept Has_OnTriggerStay2D = requires(componenttype *component)
{
	std::derived_from<componenttype, ComponentBase>;
	//OnTriggerStay2Dを持っている
	component->OnTriggerStay2D;
};

/**
 * @brief コンセプトを使用した指定するやつ
 * @tparam componenttype 判定したいコンポーネント名
 */
template <class componenttype>
concept Has_OnTriggerEnter3D = requires(componenttype *component)
{
	std::derived_from<componenttype, ComponentBase>;
	//OnTriggerEnter3Dを持っている
	component->OnTriggerEnter3D;
};

/**
 * @brief コンセプトを使用した指定するやつ
 * @tparam componenttype 判定したいコンポーネント名
 */
template <class componenttype>
concept Has_OnTriggerStay3D = requires(componenttype *component)
{
	std::derived_from<componenttype, ComponentBase>;
	//OnTriggerStay3Dを持っている
	component->OnTriggerStay3D;
};

/**
 * @brief コンセプトを使用した指定するやつ
 * @tparam componenttype 判定したいコンポーネント名
 */
template <class componenttype>
concept Has_OnCollisionEnter3D = requires(componenttype *component)
{
	std::derived_from<componenttype, ComponentBase>;
	//OnCollisionEnter3Dを持っている
	component->OnCollisionEnter3D;
};

/**
 * @brief コンセプトを使用した指定するやつ
 * @tparam componenttype 判定したいコンポーネント名
 */
template <class componenttype>
concept Has_OnCollisionStay3D = requires(componenttype *component)
{
	std::derived_from<componenttype, ComponentBase>;
	//OnCollisionStay3Dを持っている
	component->OnCollisionStay;
};

/**
 * @brief 当たり判定のコンポーネントか確認
 * @tparam componenttype 確認するコンポーネントクラス？
 */
template <class componenttype>
concept IsColliderComponent = requires(componenttype *component)
{
	std::derived_from<componenttype, ComponentBase>;

	std::derived_from<componenttype, CCollisionBase>;
};

//以下はSFINAEを使用した場合の処理

// {
//  /**
//   * @brief 当たり判定メンバ関数の名前1
//   */
//  #define CHECK_TRIGGER_ENTER2D OnTriggerEnter2D
//      /**
//   * @brief OnCollisionメンバ関数確認用メタクラス
//   * @tparam classtype 判定したいメンバのあるクラス
//   * @details SFINAEを使用
//   */
//      template <class classtype>
//      class CheckFunctionOnTriggerEnter2D
//      {
//          /**
//       * @brief 判定用メンバその１
//       * @details CHECK_COLLISION1で上記のものを展開
//       * @tparam type 判定したいクラス
//       * @tparam dummy ダミー ここでエラーが出るとfalse_typeに変更
//       * @tparam 0) ダミー
//       * @return std::true_type 成功している
//       */
//          template <class type, int dummy = (&type::CHECK_TRIGGER_ENTER2D, 0)>
//          static std::true_type CheckFunction(type *){};
//          /**
//       * @brief 判定用メンバその２
//       * @param ... 何が来ても通る？
//       * @return std::false_type 失敗した
//       */
//          static std::false_type CheckFunction(...){};
//          /**
//       * @brief ???
//       */
//          static classtype *m_pClass;
//      public:
//          /**
//       * @brief このクラスに指定したメンバがあるかどうか返すメンバ
//       */
//          static const bool value = decltype(CheckFunction(m_pClass))::value;
//      };
// }