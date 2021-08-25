/**
 * @file ComTitleSelectorMove.cpp
 * @author jupiter ()
 * @brief ComTitleSelectorMoveの実装が書かれたcpp
 */

#include "ComTitleSelectorMove.h"

void ComTitleSelectorMove::Ready()
{
    std::tie(m_firstWidthSize.x, m_firstWidthSize.y, std::ignore) = m_gameObject->m_transform->m_size.GetValue();
}

void ComTitleSelectorMove::Update()
{
    auto [size_x, size_y, size_z] = m_gameObject->m_transform->m_size.GetValue();

    float plussize = m_plusSizePerSecond * static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());

    if (m_nowFlow == 1)
    { //拡大
        m_gameObject->m_transform->m_size.AddValue(plussize, plussize, 0.0f);
    }
    else
    { //縮小
        m_gameObject->m_transform->m_size.AddValue(-plussize, -plussize, 0.0f);
    }

    //ここから下は反転させる処理
    //現在は縮小で最初の横の大きさより小さいか
    if (size_x - plussize< m_firstWidthSize.x && m_nowFlow == -1)
    {
        //小さくなりすぎているので最初の大きさに戻す
        m_gameObject->m_transform->m_size.SetValue(m_firstWidthSize.x, m_firstWidthSize.y, 1);
        m_nowFlow = 1;
    }
    //最初の横の大きさより小さくないか
    else if (size_x + plussize > m_firstWidthSize.x + m_widthMaxPlusSize && m_nowFlow == 1)
    {
        //大っきくなりすぎているので最大の値にセット
        m_gameObject->m_transform->m_size.SetValue(m_firstWidthSize.x + m_widthMaxPlusSize, m_firstWidthSize.y + m_widthMaxPlusSize, 1);
        m_nowFlow = -1;
    }
}