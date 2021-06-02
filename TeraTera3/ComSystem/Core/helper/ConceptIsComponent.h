/**
 * @file ConceptIsComponent.h
 * @author jupiter ()
 * @brief 主にCoreで使用するコンセプトを乗せたヘッダ
 */

#include <concepts>

#pragma once

class ComponentBase;

/**
 * @brief 継承しているか確認用のコンセプト
 * @tparam component 確認したいクラス
 */
template <class component>
concept IsComponent = requires(component *x)
{
    //ComponentBaseを継承しているか確認
    std::derived_from<component, ComponentBase>;
};